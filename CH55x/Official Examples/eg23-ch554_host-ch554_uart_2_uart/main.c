
#include "CH554.H"                                                          //������Ϣ��ӡ
#include "DEBUG.H"
#include "USBHOST_IF.H"
#include "DataFlash_IF.H"
#include "stdio.h"
#include "string.h"

#pragma  NOAREGS

/*ʹ��˵��:�̽�P1.0���أ��ϵ��������ģʽ�������ʹ̶�9600������ʹ��������������ͨѶ������
			������ɺ󣬿���ʹ��END�˳������������ϵ��˳�����ģʽ���˳������µĲ�����ͨѶ��
			ʵ��͸�� */

/*������������(����״̬������ʼ��Ϊ9600�����������ϵ磨������ģʽ�����߷���"END")
"CHECK"       --> "Current COMM Baud is:9600"        ��ǰ���õĲ�����
  "SET115200"   --> "OK" "ERROR"  ���ò�����
  "END"         --> "Quit"        �Զ��˳�����ģʽ������֮��Ҫ�������õĲ�����ͨѶ
*/

/*��ȡ�豸������*/
UINT8C  SetupGetDevDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_DEVICE, 0x00, 0x00, sizeof( USB_DEV_DESCR ), 0x00 };
/*��ȡ����������*/
UINT8C  SetupGetCfgDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_CONFIG, 0x00, 0x00, 0x04, 0x00 };
/*����USB��ַ*/
UINT8C  SetupSetUsbAddr[] = { USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00 };
/*����USB����*/
UINT8C  SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
/*����˵�STALL*/
UINT8C  SetupClrEndpStall[] = { USB_REQ_TYP_OUT | USB_REQ_RECIP_ENDP, USB_CLEAR_FEATURE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

UINT8X  UsbDevEndp0Size;                                                     //* USB�豸�Ķ˵�0�������ߴ� */
UINT8X  RxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0000 ;                            // IN, must even address
UINT8X  TxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0040 ;                            // OUT, must even address
_RootHubDev xdata ThisUsbDev;
bit FoundNewDev = 0;

UINT8X Receive_USB_Buf[ USER_BUF_SIZE ];        //USB���ջ�����
UINT8X USBByteCount = 0;                        //USB��������ǰ���ݸ���
UINT8X USB_Input_Point = 0;                     //USB������дָ��
UINT8X USB_Output_Point = 0;                    //USB��������ָ��

UINT8X Receive_Uart0_Buf[USER_BUF_SIZE];   //���ڽ��ջ�����
UINT8X Uart0_Input_Point = 0;              //���ڽ��ջ�����дָ��
UINT8X Uart0ByteCount = 0;                 //���ڽ��ջ�������ǰ�����������ܸ���
UINT8X Uart0_Output_Point = 0;             //���ڽ��ջ�������ָ��

UINT32 Comm_Baud = 0;                      //ͨѶ������
/*******************************************************************************
* Function Name  : Config_Uart0(UINT8 *cfg_uart)
* Description    : ���ô���1����
* Input          : �������ò��� ��λ������(��˸�ʽ)
* Output         : None
* Return         : None
*******************************************************************************/
void Config_Uart0(UINT8 *cfg_uart) {
	UINT32 uart0_buad = 0;
	UINT32 x;
	UINT8 x2;
	uart0_buad = *(UINT32*)cfg_uart;
	ES = 0;
	TR1 = 0;
	x = 10 * FREQ_SYS / uart0_buad / 16;                                       //���������Ƶ��ע��x��ֵ��Ҫ���
	x2 = x % 10;
	x /= 10;
	if ( x2 >= 5 ) x ++;                                                       //��������
	TH1 = 0-x;
	TR1 = 1;
	ES = 1;
}
/*******************************************************************************
* Function Name  : UINT8 HexToChar(UINT32 Pram,UINT8* Buf)
* Description    : ��һ�����ְ�λ���
* Input          : Pram��ת������
* Output         : Buf���ָ�루��λ��ǰ��
* Return         : Buf����
*******************************************************************************/
UINT8 HexToChar(UINT32 Pram,UINT8* Buf) {
	UINT32 a;
	UINT8 s = 0;   //��¼����
	do {
		a = Pram/10;
		*Buf++ = Pram%10;
		s++;
		Pram = a;
	} while(a);
	return s;
}
/*******************************************************************************
* Function Name  : UINT32 CharToHex(UINT8* Buf,UINT8 len)
* Description    : ��һ��������������ϳ�����
* Input          : Buf��ת�����ַ����飬len����
* Output         : NULL
* Return         : �����������
*******************************************************************************/
UINT32 xy(UINT8 x,UINT8 j) {
	UINT8 i;
	UINT32 num = 1;
	for(i=0; i<j; i++)
		num *= x;
	return num;
}
UINT32 CharToHex(UINT8* Buf,UINT8 len) {
	char i;
	UINT32 num = 0;
	for(i=len-1; i>=0; i--) {
		num += (*Buf-'0') * (xy(10, i));
		Buf++;
	}
	return num;
}
/*******************************************************************************
* Function Name  : Uart0_ISR()
* Description    : ���ڽ����жϺ�����ʵ��ѭ���������
*******************************************************************************/
void Uart0_ISR(void) interrupt INT_NO_UART0 using 1 {
	if(RI) { //�յ�����
		Receive_Uart0_Buf[Uart0_Input_Point++] = SBUF;
		Uart0ByteCount++;                    //��ǰ������ʣ���ȡ�ֽ���
		if(Uart0_Input_Point>=USER_BUF_SIZE)
			Uart0_Input_Point = 0;           //д��ָ��
		RI =0;
	}
}





void main() {
	UINT8 i,s,edp_addr;
	UINT8 lenth;
	UINT16 Uart0_Timeout = 0;
	UINT8 USB_Sending= 0;                                                     //USB�´���־
	if(CHIP_ID != 0x54 ) while(1);                                        //оƬ���

	CfgFsys( );
	mDelaymS(50);	                                                            //�޸���Ƶ���ȴ�ʱ���ȶ�
	mInitSTDIO( );                                                             //�շ�����
	UART1Setup( );                                                             //printf��ӡ�˿ڣ�P1.7��

#if DE_PRINTF
	printf( "start\n" );
#endif
	//�ж�P1.0�����Ƿ��ǵ͵�ƽ
	P1_MOD_OC = P1_MOD_OC | (1<<0);
	P1_DIR_PU = P1_DIR_PU |	(1<<0);
	if(TIN0==0) {
		mDelaymS(5);
		if(TIN0==0) {
			CH554UART0SendString("Enter Command: \"CHECK\" \"SET(baud)\" \"END\"\n");
			s = 0;
			Uart0_Timeout = 0;
			while(1) {                  //���ò�����
				if(Uart0ByteCount>0) {
					Uart0_Timeout++;
					if(Uart0ByteCount>s) { //�ٴ��յ��µ��ַ�����Ҫ��ճ�ʱ
						Uart0_Timeout = 0;
					}
					s = Uart0ByteCount;   //���ݵ�ǰ�յ����ַ���
				}
				if(Uart0_Timeout>5000 || Uart0ByteCount>20 ) { //�ַ����ͼ����ʱ ���� ����̫������������
					//�������յ����ַ�
					Uart0_Timeout = 0;
					lenth = Uart0ByteCount;
					Uart0ByteCount -= lenth;
					s = lenth;                                 //���ȱ���
					memset(TxBuffer,0,sizeof(TxBuffer));
					if(Uart0_Output_Point+lenth>USER_BUF_SIZE) { //��Ҫת��
						memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],USER_BUF_SIZE-Uart0_Output_Point);
						lenth = Uart0_Output_Point+lenth-USER_BUF_SIZE;  //ʣ�೤��
						memcpy(&TxBuffer[USER_BUF_SIZE-Uart0_Output_Point],&Receive_Uart0_Buf[0],lenth);
						Uart0_Output_Point = lenth;
					} else {
						memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],lenth);
						Uart0_Output_Point+=lenth;
						if(Uart0_Output_Point==USER_BUF_SIZE)
							Uart0_Output_Point = 0;
					}
					//ƥ��TxBuffer[](����s)���������ƥ����ִ��
					if(memcmp(&TxBuffer[0],"CHECK",5)==0) {
						CH554UART0SendString("Current Baud is:");
						ReadDataFlash(0,4,(UINT8 *)&Comm_Baud);
						s = HexToChar(Comm_Baud,&TxBuffer[0]);
						for(i=s-1; i>0; i--)
							CH554UART0SendByte('0'+TxBuffer[i]);
						CH554UART0SendByte('0'+TxBuffer[0]);
						CH554UART0SendByte('\n');
					} else if(memcmp(&TxBuffer[0],"SET",3)==0) {
						s -= 3;   //��ǰs���������ַ�����
						if(s==0)
							CH554UART0SendString("Please Enter Baud\n");
						else {
							Comm_Baud = CharToHex(&TxBuffer[3],s);
							s = WriteDataFlash(0,(UINT8*)&Comm_Baud,4);
							if(s==4)
								CH554UART0SendString("Config Success!\n");
						}
					} else if(memcmp(&TxBuffer[0],"END",3)==0) {
						CH554UART0SendString("Quit!\n");
						CH554UART0SendString("Please Use New Baud\n");
						break;
					} else {
						CH554UART0SendString("Enter Error\n");
					}
					s = 0;              //����s,Ϊ��һ�ν�����׼��
				}
			}
		}
	}
	//��dataflash��ȡ����,������
	ReadDataFlash(0,4,(UINT8 *)&Comm_Baud);
//	Comm_Baud = 9600;
	Config_Uart0((UINT8 *)&Comm_Baud);

	InitUSB_Host();
	FoundNewDev = 0;
#if DE_PRINTF
	printf( "Wait Device In\n" );
#endif
	while(1) {
		s = ERR_SUCCESS;
		if ( UIF_DETECT ) {                                                    // �����USB��������ж�����
			UIF_DETECT = 0;                                                    // ���жϱ�־
			s = AnalyzeRootHub( );                                             // ����ROOT-HUB״̬
			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
		}
		if ( FoundNewDev ) {                                                   // ���µ�USB�豸����
			FoundNewDev = 0;
			mDelaymS( 200 );                                                    // ����USB�豸�ղ�����δ�ȶ�,�ʵȴ�USB�豸���ٺ���,������ζ���
			s = EnumAllRootDevice( );                                           // ö������ROOT-HUB�˿ڵ�USB�豸
			if ( s != ERR_SUCCESS ) {
#if DE_PRINTF
				printf( "EnumAllRootDev err = %02X\n", (UINT16)s );
#endif
			}
		}
		if(Uart0ByteCount)
			Uart0_Timeout++;
		if(ThisUsbDev.DeviceStatus == ROOT_DEV_SUCCESS) {
			//�жϴ��ڽ��ջ��������Ƿ���Ҫ���ͣ�
			if(USB_Sending==0) { //��ǰ�����´�
				lenth = Uart0ByteCount;
				if(lenth>0) {
					if(lenth>= ThisUsbDev.GpVar0_Lenth || Uart0_Timeout>2000) {
						Uart0_Timeout = 0;
						if(lenth>= ThisUsbDev.GpVar0_Lenth)
							lenth = ThisUsbDev.GpVar0_Lenth;
						UH_TX_LEN = lenth;
						Uart0ByteCount-=lenth;
						if(Uart0_Output_Point+lenth>USER_BUF_SIZE) {
							memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],USER_BUF_SIZE-Uart0_Output_Point);
							lenth = Uart0_Output_Point+lenth-USER_BUF_SIZE;  //ʣ�೤��
							memcpy(&TxBuffer[USER_BUF_SIZE-Uart0_Output_Point],&Receive_Uart0_Buf[0],lenth);
							Uart0_Output_Point = lenth;
						} else {
							memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],lenth);
							Uart0_Output_Point+=lenth;
							if(Uart0_Output_Point==USER_BUF_SIZE)
								Uart0_Output_Point = 0;
						}
						USB_Sending = 1;     //usb�����´���־�����������
					}
				}
			}
			//������
			if(USB_Sending==1) {
				edp_addr = ThisUsbDev.GpVar0 & 0x0f;
				s = USBHostTransact( USB_PID_OUT << 4 | edp_addr, ThisUsbDev.GpVar0 & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );// CH554��������,��ȡ����,NAK������
				if ( s == ERR_SUCCESS ) {
					ThisUsbDev.GpVar0 ^= 0x80;                                 // ͬ����־��ת
					USB_Sending = 0;
				} else if ( s != ( USB_PID_NAK | ERR_USB_TRANSFER ) ) {
#if DE_PRINTF
					printf("keyboard error %02x\n",(UINT16)s);                    // �����ǶϿ���
#endif
				}
			}

			//USB�����������ݣ���Ҫ����
			if(USBByteCount>0) {
				CH554UART0SendByte(Receive_USB_Buf[USB_Output_Point++]);
				USBByteCount--;
				if(USB_Output_Point==USER_BUF_SIZE)
					USB_Output_Point = 0;
			}
			//�ж�USB�������Ƿ񻹿��Խ�����һ����
			if( USER_BUF_SIZE-USBByteCount >= ThisUsbDev.GpVar1_Lenth ) {
				edp_addr = ThisUsbDev.GpVar1 & 0x0f;
				s = USBHostTransact( USB_PID_IN << 4 | edp_addr, ThisUsbDev.GpVar1 & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );// CH554��������,��ȡ����,NAK������
				if ( s == ERR_SUCCESS ) {
					ThisUsbDev.GpVar1 ^= 0x80;                                 // ͬ����־��ת
					lenth = USB_RX_LEN;
					USBByteCount+=lenth;                                        //���³���
					if(USB_Input_Point+lenth>USER_BUF_SIZE) { //��Ҫתͷ
						memcpy(&Receive_USB_Buf[USB_Input_Point],&RxBuffer[0],USER_BUF_SIZE-USB_Input_Point);
						lenth = USB_Input_Point+lenth-USER_BUF_SIZE;  //ʣ�೤��
						memcpy(&Receive_USB_Buf[0],&RxBuffer[USER_BUF_SIZE-USB_Input_Point],lenth);
						USB_Input_Point = lenth;
					} else {
						memcpy(&Receive_USB_Buf[USB_Input_Point],&RxBuffer[0],lenth);
						USB_Input_Point += lenth;
						if(USB_Input_Point==USER_BUF_SIZE)
							USB_Input_Point = 0;
					}

				} else if ( s != ( USB_PID_NAK | ERR_USB_TRANSFER ) ) {
#if DE_PRINTF
					printf("keyboard error %02x\n",(UINT16)s);                    // �����ǶϿ���
#endif
				}
			}



		}
	}


}

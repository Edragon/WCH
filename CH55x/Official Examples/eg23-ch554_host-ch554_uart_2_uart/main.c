
#include "CH554.H"                                                          //调试信息打印
#include "DEBUG.H"
#include "USBHOST_IF.H"
#include "DataFlash_IF.H"
#include "stdio.h"
#include "string.h"

#pragma  NOAREGS

/*使用说明:短接P1.0到地，上电进入配置模式，波特率固定9600，可以使用配置命令配置通讯波特率
			配置完成后，可以使用END退出，或者重新上电退出配置模式，退出后以新的波特率通讯，
			实现透传 */

/*串口配置命令(配置状态波特率始终为9600，除非重新上电（非配置模式）或者发送"END")
"CHECK"       --> "Current COMM Baud is:9600"        当前配置的波特率
  "SET115200"   --> "OK" "ERROR"  配置波特率
  "END"         --> "Quit"        自动退出配置模式，但是之后要以新配置的波特率通讯
*/

/*获取设备描述符*/
UINT8C  SetupGetDevDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_DEVICE, 0x00, 0x00, sizeof( USB_DEV_DESCR ), 0x00 };
/*获取配置描述符*/
UINT8C  SetupGetCfgDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_CONFIG, 0x00, 0x00, 0x04, 0x00 };
/*设置USB地址*/
UINT8C  SetupSetUsbAddr[] = { USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00 };
/*设置USB配置*/
UINT8C  SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
/*清除端点STALL*/
UINT8C  SetupClrEndpStall[] = { USB_REQ_TYP_OUT | USB_REQ_RECIP_ENDP, USB_CLEAR_FEATURE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

UINT8X  UsbDevEndp0Size;                                                     //* USB设备的端点0的最大包尺寸 */
UINT8X  RxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0000 ;                            // IN, must even address
UINT8X  TxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0040 ;                            // OUT, must even address
_RootHubDev xdata ThisUsbDev;
bit FoundNewDev = 0;

UINT8X Receive_USB_Buf[ USER_BUF_SIZE ];        //USB接收缓冲区
UINT8X USBByteCount = 0;                        //USB缓冲区当前数据个数
UINT8X USB_Input_Point = 0;                     //USB缓冲区写指针
UINT8X USB_Output_Point = 0;                    //USB缓冲区读指针

UINT8X Receive_Uart0_Buf[USER_BUF_SIZE];   //串口接收缓冲区
UINT8X Uart0_Input_Point = 0;              //串口接收缓冲区写指针
UINT8X Uart0ByteCount = 0;                 //串口接收缓冲区当前待处理数据总个数
UINT8X Uart0_Output_Point = 0;             //串口接收缓冲区读指针

UINT32 Comm_Baud = 0;                      //通讯波特率
/*******************************************************************************
* Function Name  : Config_Uart0(UINT8 *cfg_uart)
* Description    : 配置串口1参数
* Input          : 串口配置参数 四位波特率(大端格式)
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
	x = 10 * FREQ_SYS / uart0_buad / 16;                                       //如果更改主频，注意x的值不要溢出
	x2 = x % 10;
	x /= 10;
	if ( x2 >= 5 ) x ++;                                                       //四舍五入
	TH1 = 0-x;
	TR1 = 1;
	ES = 1;
}
/*******************************************************************************
* Function Name  : UINT8 HexToChar(UINT32 Pram,UINT8* Buf)
* Description    : 把一个数字按位输出
* Input          : Pram待转换数字
* Output         : Buf输出指针（低位在前）
* Return         : Buf长度
*******************************************************************************/
UINT8 HexToChar(UINT32 Pram,UINT8* Buf) {
	UINT32 a;
	UINT8 s = 0;   //记录长度
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
* Description    : 把一个个数字重新组合成数据
* Input          : Buf待转换的字符数组，len长度
* Output         : NULL
* Return         : 待输出的数据
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
* Description    : 串口接收中断函数，实现循环缓冲接收
*******************************************************************************/
void Uart0_ISR(void) interrupt INT_NO_UART0 using 1 {
	if(RI) { //收到数据
		Receive_Uart0_Buf[Uart0_Input_Point++] = SBUF;
		Uart0ByteCount++;                    //当前缓冲区剩余待取字节数
		if(Uart0_Input_Point>=USER_BUF_SIZE)
			Uart0_Input_Point = 0;           //写入指针
		RI =0;
	}
}





void main() {
	UINT8 i,s,edp_addr;
	UINT8 lenth;
	UINT16 Uart0_Timeout = 0;
	UINT8 USB_Sending= 0;                                                     //USB下传标志
	if(CHIP_ID != 0x54 ) while(1);                                        //芯片检测

	CfgFsys( );
	mDelaymS(50);	                                                            //修改主频，等待时钟稳定
	mInitSTDIO( );                                                             //收发数据
	UART1Setup( );                                                             //printf打印端口（P1.7）

#if DE_PRINTF
	printf( "start\n" );
#endif
	//判断P1.0引脚是否是低电平
	P1_MOD_OC = P1_MOD_OC | (1<<0);
	P1_DIR_PU = P1_DIR_PU |	(1<<0);
	if(TIN0==0) {
		mDelaymS(5);
		if(TIN0==0) {
			CH554UART0SendString("Enter Command: \"CHECK\" \"SET(baud)\" \"END\"\n");
			s = 0;
			Uart0_Timeout = 0;
			while(1) {                  //配置波特率
				if(Uart0ByteCount>0) {
					Uart0_Timeout++;
					if(Uart0ByteCount>s) { //再次收到新的字符，需要清空超时
						Uart0_Timeout = 0;
					}
					s = Uart0ByteCount;   //备份当前收到的字符数
				}
				if(Uart0_Timeout>5000 || Uart0ByteCount>20 ) { //字符发送间隔超时 或者 长度太长（参数错误）
					//分析接收到的字符
					Uart0_Timeout = 0;
					lenth = Uart0ByteCount;
					Uart0ByteCount -= lenth;
					s = lenth;                                 //长度备份
					memset(TxBuffer,0,sizeof(TxBuffer));
					if(Uart0_Output_Point+lenth>USER_BUF_SIZE) { //需要转折
						memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],USER_BUF_SIZE-Uart0_Output_Point);
						lenth = Uart0_Output_Point+lenth-USER_BUF_SIZE;  //剩余长度
						memcpy(&TxBuffer[USER_BUF_SIZE-Uart0_Output_Point],&Receive_Uart0_Buf[0],lenth);
						Uart0_Output_Point = lenth;
					} else {
						memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],lenth);
						Uart0_Output_Point+=lenth;
						if(Uart0_Output_Point==USER_BUF_SIZE)
							Uart0_Output_Point = 0;
					}
					//匹配TxBuffer[](长度s)和已有命令，匹配则执行
					if(memcmp(&TxBuffer[0],"CHECK",5)==0) {
						CH554UART0SendString("Current Baud is:");
						ReadDataFlash(0,4,(UINT8 *)&Comm_Baud);
						s = HexToChar(Comm_Baud,&TxBuffer[0]);
						for(i=s-1; i>0; i--)
							CH554UART0SendByte('0'+TxBuffer[i]);
						CH554UART0SendByte('0'+TxBuffer[0]);
						CH554UART0SendByte('\n');
					} else if(memcmp(&TxBuffer[0],"SET",3)==0) {
						s -= 3;   //当前s代表波特率字符长度
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
					s = 0;              //清零s,为下一次接收做准备
				}
			}
		}
	}
	//从dataflash读取参数,并配置
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
		if ( UIF_DETECT ) {                                                    // 如果有USB主机检测中断则处理
			UIF_DETECT = 0;                                                    // 清中断标志
			s = AnalyzeRootHub( );                                             // 分析ROOT-HUB状态
			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
		}
		if ( FoundNewDev ) {                                                   // 有新的USB设备插入
			FoundNewDev = 0;
			mDelaymS( 200 );                                                    // 由于USB设备刚插入尚未稳定,故等待USB设备数百毫秒,消除插拔抖动
			s = EnumAllRootDevice( );                                           // 枚举所有ROOT-HUB端口的USB设备
			if ( s != ERR_SUCCESS ) {
#if DE_PRINTF
				printf( "EnumAllRootDev err = %02X\n", (UINT16)s );
#endif
			}
		}
		if(Uart0ByteCount)
			Uart0_Timeout++;
		if(ThisUsbDev.DeviceStatus == ROOT_DEV_SUCCESS) {
			//判断串口接收缓冲区，是否需要发送？
			if(USB_Sending==0) { //当前不在下传
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
							lenth = Uart0_Output_Point+lenth-USER_BUF_SIZE;  //剩余长度
							memcpy(&TxBuffer[USER_BUF_SIZE-Uart0_Output_Point],&Receive_Uart0_Buf[0],lenth);
							Uart0_Output_Point = lenth;
						} else {
							memcpy(&TxBuffer[0],&Receive_Uart0_Buf[Uart0_Output_Point],lenth);
							Uart0_Output_Point+=lenth;
							if(Uart0_Output_Point==USER_BUF_SIZE)
								Uart0_Output_Point = 0;
						}
						USB_Sending = 1;     //usb正在下传标志，发送完清除
					}
				}
			}
			//发送中
			if(USB_Sending==1) {
				edp_addr = ThisUsbDev.GpVar0 & 0x0f;
				s = USBHostTransact( USB_PID_OUT << 4 | edp_addr, ThisUsbDev.GpVar0 & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );// CH554传输事务,获取数据,NAK不重试
				if ( s == ERR_SUCCESS ) {
					ThisUsbDev.GpVar0 ^= 0x80;                                 // 同步标志翻转
					USB_Sending = 0;
				} else if ( s != ( USB_PID_NAK | ERR_USB_TRANSFER ) ) {
#if DE_PRINTF
					printf("keyboard error %02x\n",(UINT16)s);                    // 可能是断开了
#endif
				}
			}

			//USB缓冲区有数据，需要发送
			if(USBByteCount>0) {
				CH554UART0SendByte(Receive_USB_Buf[USB_Output_Point++]);
				USBByteCount--;
				if(USB_Output_Point==USER_BUF_SIZE)
					USB_Output_Point = 0;
			}
			//判断USB缓冲区是否还可以接收下一包？
			if( USER_BUF_SIZE-USBByteCount >= ThisUsbDev.GpVar1_Lenth ) {
				edp_addr = ThisUsbDev.GpVar1 & 0x0f;
				s = USBHostTransact( USB_PID_IN << 4 | edp_addr, ThisUsbDev.GpVar1 & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );// CH554传输事务,获取数据,NAK不重试
				if ( s == ERR_SUCCESS ) {
					ThisUsbDev.GpVar1 ^= 0x80;                                 // 同步标志翻转
					lenth = USB_RX_LEN;
					USBByteCount+=lenth;                                        //更新长度
					if(USB_Input_Point+lenth>USER_BUF_SIZE) { //需要转头
						memcpy(&Receive_USB_Buf[USB_Input_Point],&RxBuffer[0],USER_BUF_SIZE-USB_Input_Point);
						lenth = USB_Input_Point+lenth-USER_BUF_SIZE;  //剩余长度
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
					printf("keyboard error %02x\n",(UINT16)s);                    // 可能是断开了
#endif
				}
			}



		}
	}


}

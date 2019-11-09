#include  "include.h"

/***********************************************************************************
****�������ƣ����������շ�������
****�������ã�
****����������
************************************************************************************/
unsigned char idata RDataBuf[64];//������ջ�����
unsigned char idata TDataBuf[64];//���巢�ͻ�����
unsigned char idata buf1[8];
/***********************************************************************************
****�������ƣ����嵥Ƭ������ӿ�
****�������ã�
****����������
************************************************************************************/
sbit D0=P1^0;//LED
sbit D1=P1^1;
sbit D2=P1^2;
sbit D3=P1^3;
sbit D4=P1^4;
sbit D5=P1^5;
sbit D6=P1^6;
sbit D7=P1^7;
sbit BEEP=P3^6;//������
/***********************************************************************************
****�������ƣ�����ȫ�ֱ���
****�������ã�
****����������
************************************************************************************/
unsigned char frq=0;//���е�Ƶ��
unsigned char num;

unsigned char mVarSetupRequest;						//	;USB������
unsigned char mVarSetupLength;					//		;�������ݳ���
unsigned char  code * VarSetupDescr;						//	;������ƫ�Ƶ�ַ

unsigned char VarUsbAddress	;					//
unsigned char code *SetupDesc;
bit CH375FLAGERR;						//������0
bit	CH375CONFLAG;

/***********************************************************************************
****�������ƣ�����ṹ��
****�������ã�
****����������
************************************************************************************/
typedef	union _REQUEST_PACK
{
	unsigned char  buffer[8];
	struct{
		unsigned char	 bmReuestType;    	 //��׼������
		unsigned char	 bRequest;		   	//�������
		unsigned int     wValue;			//����ѡ���
		unsigned int     wIndx;				//����
		unsigned int     wLength;				//���ݳ���
	}r;
} mREQUEST_PACKET,	*mpREQUEST_PACKET;
mREQUEST_PACKET request;
/***********************************************************************************
****�������ƣ��豸������
****�������ã�
****����������
************************************************************************************/
unsigned char  code DevDes[]=
{
	0x12,			//��������С			
    0x01,			//����DEVICE
    0x10,			//USB�淶�汾��Ϣ��2���ֽ�
    0x01,
    0x00,			//����룬��ֵ0X01~0XFEΪUSB������࣬���ӡ���ȣ�
	                //0XFF���ɳ���ָ�������0ΪHID�࣬��������������������еĽӿ�������ָ����
    0x00,			//�������	
    0x00,			//Э����
    0x08,			//�˵�0�������Ϣ����С
    0x48,			//����ID
    0x43,
    0x37,			//��ƷID	
    0x55,
    0x00,			//�豸�汾��Ϣ
    0x01,
    0x00,			//����ֵ	
    0x00,
    0x00,
    0x00,			//�������õ���Ŀ	
};
/***********************************************************************************
****�������ƣ�����������
****�������ã�
****����������
************************************************************************************/
unsigned char   code ConDes[]=
{	
	0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x04, 0xa0,  0x23,//�������������ڶ��ֽ�Ϊ0X02
	0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00,  0x05,//�ӿ����������ڶ��ֽ�Ϊ0X04 
	0x09, 0x21, 0x00, 0x01, 0x00, 0x01, 0x22, 0x22,  0x00,//HID���������ڶ��ֽ�Ϊ0X21
	0x07, 0x05, 0x82, 0x03, 0x40, 0x00, 0x18, //�˵����������ڶ��ֽ�Ϊ0X05
	0x07, 0x05, 0x02, 0x03, 0x40, 0x00, 0x18	 //�˵���������
};		//����������		
/***********************************************************************************
****�������ƣ�HID������
****�������ã�
****����������
************************************************************************************/	
unsigned char code Hid_des[]=
{
  0x06, 0x00,0xff,
  0x09, 0x01,  
  0xa1, 0x01, 	  //���Ͽ�ʼ
 
  0x09, 0x02, 		//Usage Page  �÷�
  0x15, 0x00, 	    //Logical  Minimun
  0x26, 0x00,0xff,		//Logical  Maximun
  0x75, 0x08,  		//Report Size
  0x95, 0x40,  		//Report Counet
  0x81, 0x06,  		//Input
  0x09, 0x02, 		//Usage Page  �÷�
  0x15, 0x00, 		//Logical  Minimun
  0x26, 0x00,0xff,		//Logical  Maximun
  0x75, 0x08,  		//Report Size
  0x95, 0x40,  		//Report Counet
  0x91, 0x06,  		//Output

  0xC0  
};
/***********************************************************************************
****�������ƣ�����������
****�������ã�
****����������
************************************************************************************/
unsigned char  code LangDes[]={0x04,0x03,0x09,0x04};		//����������
/***********************************************************************************
****�������ƣ��ַ���������
****�������ã�
****����������
************************************************************************************/
//unsigned char  code SerDes[]={0x12,0x03,'C',0,'H',0,'3',0,'7',0,'2',0,'U',0,'S',0,'B',0};		//�ַ���������
unsigned char code SerDes[]={0x12,0x03,0x43,0x00,0x48,0x00,0x33,0x00,0x37,0x00,0x32,0x00,0x55,0x00,0x53,0x00,
	0x42,0x00};
/***********************************************************************************
****�������ƣ�CH372/375��ʼ������
****�������ã�
****����������
************************************************************************************/
void	CH375_Init( )
{
/* ����USB����ģʽ, ��Ҫ���� */
	CH375_WRCMD( CMD_SET_USB_MODE );
	CH375_WRDAT( 1 );  									/* ����Ϊʹ�����ù̼���USB�豸��ʽ */
	for ( ;; ) 
	{  											/* �ȴ������ɹ�,ͨ����Ҫ�ȴ�10uS-20uS */
		if ( CH375_REDAT( )==CMD_RET_SUCCESS ) break;
	}

/* ���������ж�,�ٶ�CH375������INT0 */
	IT0 = 0;  /* ���ⲿ�ź�Ϊ�͵�ƽ���� */
	IE0 = 0;  /* ���жϱ�־ */
	EX0 = 1;  /* ����CH375�ж� */
}
/***********************************************************************************
****�������ƣ��˵�0�����ϴ�����
****�������ã�
****����������
************************************************************************************/
void mCh375Ep0Up()
{
	unsigned char i,len;
	if(mVarSetupLength)
	{												//���Ȳ�Ϊ0������峤�ȵ�����
		if(mVarSetupLength<=8)
		{
			len=mVarSetupLength;
			mVarSetupLength=0;
        }	//����С��8����Ҫ��ĳ���
		else
		{
			len=8;
			mVarSetupLength-=8;
		}							                        		//���ȴ���8����8�������ܳ��ȼ�8
	    CH375_WRCMD(CMD_WR_USB_DATA3);						//����д�˵�0������
       	CH375_WRDAT(len);										//д�볤��
    	for(i=0;i!=len;i++)
        CH375_WRDAT(request.buffer[i]);	              		//ѭ��д������
    }
	else
	{
		CH375_WRCMD(CMD_WR_USB_DATA3);						//����д�˵�0������
		CH375_WRDAT(0);					                   //�ϴ�0�������ݣ�����һ��״̬�׶�
	}
}
/***********************************************************************************
****�������ƣ����������ƺ���
****�������ã�
****����������
************************************************************************************/
void mCh375DesUp()
{
	unsigned char k;        
	for (k=0; k!=8; k++ ) 
	{
         request.buffer[k]=*VarSetupDescr;  								//���θ���8����������
         VarSetupDescr++;
    }
}
/***********************************************************************************
****�������ƣ��ⲿ�ж�0����Ӧ����
****�������ã�ͨ���ж�375���ն�����ֵ������Ӧ�Ĵ���
****����������
************************************************************************************/
void int0(void) interrupt 0 using 0
{
	unsigned char i,temp,length,len;
	unsigned char c1;
	//unsigned char data buf[64];                   //���ܵ������ݴ�ŵĻ������飬64��λ��
	//unsigned char   *pBuf;
	unsigned char   mBuf[64];
	CH375_WRCMD(CMD_GET_STATUS);               //����ȡ375���ж����ͣ�״̬�������ȡ���ж��ź�
	
	temp=CH375_REDAT();                        //��ȡ�ж����ͣ�״̬��ֵ
	IE0=0;
	switch(temp)                                  //�ж��ж����ͣ�״̬���������������375ͷ�ļ��еĶ���
	{
		case  USB_INT_EP2_OUT:  											// �����˵��´��ɹ� 
			//pBuf=mBuf;																	//����δ����
			CH375_WRCMD(CMD_RD_USB_DATA);									//��������������
			length=CH375_REDAT();											//���ȶ������ǳ���														
			//for(len=0;len!=length;len++,pBuf++)
				//*pBuf=CH375_REDAT();	//�����ݶ��뵽������			 
			//pBuf=mBuf;
			for(i=0;i<length;i++)
				RDataBuf[i]=CH375_REDAT();
			P1=RDataBuf[2];
            // ��ʾ�ش�
			CH375_WRCMD(CMD_WR_USB_DATA7);								//����д�ϴ��˵�����
			CH375_WRDAT(length);	
			//for(len=0;len!=length;len++,pBuf++)
				//CH375_WRDAT(*pBuf);	//������д���ϴ��˵�
			for(i=0;i<length;i++)
				CH375_WRDAT(RDataBuf[i]);
			break;
			
		case   USB_INT_EP2_IN:												 //�����˵��ϴ��ɹ�,δ����
			CH375_WRCMD(CMD_UNLOCK_USB);								//�ͷŻ�����
			break;
		case   USB_INT_EP1_IN:	 											//�ж϶˵��ϴ��ɹ���δ����
			CH375_WRCMD(CMD_UNLOCK_USB);								//�ͷŻ�����
			break;
		case   USB_INT_EP1_OUT:	  											//�ж϶˵��´��ɹ���δ����
			CH375_WRCMD(CMD_RD_USB_DATA);									//��������������
			if(length=CH375_REDAT())
			{										//����Ϊ0����
				for(len=0;len!=length;len++)
					c1=CH375_REDAT();					//ȡ���´�����
			}
//			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);								//�ͷŻ�����,���ǰ��û��CMD_RD_USB_DATA������ʹ�ñ�����
			break;
		case   USB_INT_EP0_SETUP: 											//���ƶ˵㽨���ɹ�
	    	CH375_WRCMD(CMD_RD_USB_DATA);
			length=CH375_REDAT();
			for(len=0;len!=length;len++)
				request.buffer[len]=CH375_REDAT();  // ȡ������
			if(length==0x08)
			{
			    mVarSetupLength=request.buffer[6];							//���ƴ������ݳ����������Ϊ128
				if((c1=request.r.bmReuestType)&0x40)
				{         					 //��������δ����
				}
				if((c1=request.r.bmReuestType)&0x20)  //������δ����
				{   
					if(request.buffer[1]==0x0a)       //���ýӿ�
					{//printf("c\n");
					}												//SET_IDLE
					else 
						if(request.buffer[1]==0x09)
					    {
					    }//������δ����
				}
				if(!((c1=request.r.bmReuestType)&0x60))
				{          				//��׼����
					mVarSetupRequest=request.r.bRequest;							//�ݴ��׼������
					switch(request.r.bRequest)
					{  // ������׼����
						case DEF_USB_CLR_FEATURE:									//�������
							if((c1=request.r.bmReuestType&0x1F)==0X02)
							{					//���Ƕ˵㲻֧��
								switch(request.buffer[4])
								{
									case 0x82:
										CH375_WRCMD(CMD_SET_ENDP7);					//����˵�2�ϴ�
										CH375_WRDAT(0x8E);                			//����������˵�
										break;
									case 0x02:
										CH375_WRCMD(CMD_SET_ENDP6);
										CH375_WRDAT(0x80);							//����˵�2�´�
										break;
									case 0x81:
										CH375_WRCMD(CMD_SET_ENDP5);					//����˵�1�ϴ�
										CH375_WRDAT(0x8E);
										break;
									case 0x01:
										CH375_WRCMD(CMD_SET_ENDP4);					//����˵�1�´�
										CH375_WRDAT(0x80);
										break;
									default:
										break;
								}
							}
							else
							{
								CH375FLAGERR=1;								//��֧�ֵ�������ԣ��ô����־
							}
							break;
						case DEF_USB_GET_STATUS:								//���״̬
							request.buffer[0]=0;
							request.buffer[1]=0;								//�ϴ�״̬
							break;
						case DEF_USB_SET_ADDRESS:								//���õ�ַ
							VarUsbAddress=request.buffer[2];					//�ݴ�USB���������ĵ�ַ
							break;
						case DEF_USB_GET_DESCR: 								//���������
							if(request.buffer[3]==1)							//�豸�������ϴ�
							{
								VarSetupDescr=DevDes;
								CH375FLAGERR=0;
					        }
							else if(request.buffer[3]==2)		 					//�����������ϴ�
							{
								VarSetupDescr=ConDes;
								CH375FLAGERR=0;
					        }
							else if(request.buffer[3]==0x22) 
							{
								VarSetupDescr=Hid_des;
								CH375FLAGERR=0;
							}
							else if(request.buffer[3]==3) 
							{
								if ( request.buffer[4]== 0 ) 
								{
									VarSetupDescr=LangDes;
									mVarSetupLength=4;
					        	}
								else 
								{
							        VarSetupDescr=SerDes;
						        	mVarSetupLength=0x12; 						//���ַ�������
						        }
							}
							else
								CH375FLAGERR=1;
							mCh375DesUp();									//������������֧��					          							
							break;
						case DEF_USB_GET_CONFIG:								//�������
							request.buffer[0]=0;									//û��������0
							if(CH375CONFLAG) 
								request.buffer[0]=1;								//�Ѿ�������1����������������涨��
							break;
						case DEF_USB_SET_CONFIG:                 				//��������
							CH375CONFLAG=0;
							//CH375ACT=1;
							if ( request.buffer[2] != 0 ) 
							{
								CH375CONFLAG=1;									//�������ñ�־
								//CH375ACT=0;										//�����������ź�
							}
							break;
						case DEF_USB_GET_INTERF:								//�õ��ӿ�
							request.buffer[0]=1;									//�ϴ��ӿ�����������ֻ֧��һ���ӿ�
							break;
						default :
							CH375FLAGERR=1;										//��֧�ֵı�׼����
							break;
					}
				}
			}
			else 
			{  //��֧�ֵĿ��ƴ��䣬����8�ֽڵĿ��ƴ���
				CH375FLAGERR=1;
			}
			if(!CH375FLAGERR) 
				mCh375Ep0Up();										    //û�д���/���������ϴ���������Ϊ0�ϴ�Ϊ״̬
			else 
			{
				CH375_WRCMD(CMD_SET_ENDP3);								//���ö˵�1ΪSTALL��ָʾһ������
				CH375_WRDAT(0x0F);
			}
			break;
		case   USB_INT_EP0_IN:												//���ƶ˵��ϴ��ɹ�
			if(mVarSetupRequest==DEF_USB_GET_DESCR)
			{	//�������ϴ�
				mCh375DesUp();
				mCh375Ep0Up();															
			}
			else if(mVarSetupRequest==DEF_USB_SET_ADDRESS)
			{	//���õ�ַ
				CH375_WRCMD(CMD_SET_USB_ADDR);
				CH375_WRDAT(VarUsbAddress);								//����USB��ַ,�����´������USB��ַ
			}
			CH375_WRCMD(CMD_UNLOCK_USB);								    //�ͷŻ�����
			break;
		case   USB_INT_EP0_OUT:												//���ƶ˵��´��ɹ�
			CH375_WRCMD(CMD_RD_USB_DATA);									//��������������
			if(length=CH375_REDAT())
			{										                        //����Ϊ0����
				for(len=0;len!=length;len++)
				{
				    RDataBuf[len]=CH375_REDAT();					    //ȡ���´�����
				}
			}
			break;
		default:
			if((temp&0x03)==0x03)
			{									//���߸�λ
				CH375FLAGERR=0;													//������0
				CH375CONFLAG=0;													//������0
				mVarSetupLength=0;
				//CH375ACT=1;														//������������
			}
			else
			{																//���֧��
				;
			}
			CH375_WRCMD (CMD_UNLOCK_USB);									//�ͷŻ�����
			break;
	}
}

/***********************************************************************************
****�������ƣ�������
****�������ã�
****����������
************************************************************************************/
void main(void)
{
	unsigned char i;

	delay50ms( );	/* ��ʱ�ȴ�CH375��ʼ�����,�����Ƭ����CH375�ṩ��λ�ź��򲻱���ʱ */
	CH375_Init( );  /* ��ʼ��CH375 */
    EA=1;
	while(1)		/* ������ */
	{
		
	}
}








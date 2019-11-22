
#include "include.h"


/***********************************************************************************
****�������ƣ����������շ�������
****�������ã�
****����������
************************************************************************************/
unsigned char idata RDataBuf[64];//������ջ�����
unsigned char idata TDataBuf[64];//���巢�ͻ�����
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
****�������ƣ�ȫ�ֱ���
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
****�������ƣ�������ṹ��
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
    0xFF,			//����룬��ֵ0X01~0XFEΪUSB������࣬���ӡ���ȣ�
	                //0XFF���ɳ���ָ�������0ΪHID�࣬��������������������еĽӿ�������ָ����
    0x80,			//�������	
    0x37,			//Э����
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
    0x01,			//�������õ���Ŀ	
 //   00,				//������
  //  00,
  //  00,
 	//00,
 //	00,
 	//00
};
/***********************************************************************************
****�������ƣ�����������
****�������ã�
****����������
************************************************************************************/
unsigned char   code ConDes[]=
{	
	//����������
	0x09,					//��������С
    0x02,					//����CONFIG
    0x27,					//�����ô����������ݴ�С
    0x00,					//
    0x01,					//�ӿ���
    0x01,					//����ֵ
    0x00,					//����
    0x80,					//��Դ����
    0x40,					//��Ҫ���ߵ�Դ
	//�ӿ�������
    0x09,					//��������С								
    0x04,					//����INTERFACE				
    0x00,					//ʶ����
    0x00,					//������ֵ
    0x03,					//֧�ֵĶ˵���
    0xFF,					//�����
    0x80,					//�������
    0x37,					//Э����
    0x00,					//����
	//�˵�������
    0x07,					//������С
    0x05,					//����ENDPOINT
    0x82,					//�˵���Ŀ������
    0x02,					//֧�ֵĴ�������
    0x40,					//֧�ֵ������Ϣ����С
    0x00,
    0x00,					//
	//	
    0x07,
    0x05,
    0x02,
    0x02,
    0x40,
    0x00,
    0x00,
					
    0x07,
    0x05,
    0x81,
    0x03,
    0x08,
    0x00,
    0x01,

    0x07,
    0x05,
    0x01,
    0x02,
    0x08,
    0x00,
    0x00
};		//����������		
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
unsigned char  code SerDes[]={0x12,0x03,'C',0,'H',0,'3',0,'7',0,'2',0,'U',0,'S',0,'B',0};		//�ַ���������
/***********************************************************************************
****�������ƣ�HID������
****�������ã�
****����������
************************************************************************************/
unsigned char code Hid_des[]={0x05, 0x01, 0x09, 0x06,  0xa1, 0x01, 0x05, 0x07,  0x19, 0xe0, 
							  0x29, 0xe7, 0x15, 0x00,  0x25, 0x01, 0x75, 0x01,  0x95, 0x08, 
							  0x81, 0x02, 0x95, 0x01,  0x75, 0x08, 0x81, 0x01,  0x95, 0x03, 0x75, 0x01,        
               				  0x05, 0x08, 0x19, 0x01,  0x29, 0x03, 0x91, 0x02,  0x95, 0x01, 0x75, 0x05,  
							  0x91, 0x01, 0x95, 0x06,  0x75, 0x08, 0x15, 0x00,  0x26, 0xff, 0x00, 0x05,
					    	  0x07, 0x19, 0x00, 0x2a,  0xff, 0x00, 0x81, 0x00,  0xc0
							  };
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
	for ( ;; ) {  											/* �ȴ������ɹ�,ͨ����Ҫ�ȴ�10uS-20uS */
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
void mCh375Ep0Up(){
	unsigned char i,len;
	if(mVarSetupLength){												//���Ȳ�Ϊ0������峤�ȵ�����
		if(mVarSetupLength<=8){
			len=mVarSetupLength;
			mVarSetupLength=0;
        }	//����С��8����Ҫ��ĳ���
		else{
			len=8;
			mVarSetupLength-=8;
		}							                        		//���ȴ���8����8�������ܳ��ȼ�8
	    CH375_WRCMD(CMD_WR_USB_DATA3);						//����д�˵�0������
       	CH375_WRDAT(len);										//д�볤��
    	for(i=0;i!=len;i++)
        CH375_WRDAT(request.buffer[i]);	              		//ѭ��д������
    }
	else{
		CH375_WRCMD(CMD_WR_USB_DATA3);						//����д�˵�0������
		CH375_WRDAT(0);					                   //�ϴ�0�������ݣ�����һ��״̬�׶�
	}
}
/***********************************************************************************
****�������ƣ�����������
****�������ã�
****����������
************************************************************************************/
void mCh375DesUp(){
	unsigned char k;        
	for (k=0; k!=8; k++ ) {
         request.buffer[k]=*VarSetupDescr;  								//���θ���8����������
         VarSetupDescr++;
    }
}
/***********************************************************************************
****�������ƣ���������ʼ������
****�������ã�
****����������
************************************************************************************/
void BEEPInit()
{
	frq=50;
	EA=1;
	ET0=0;
	TMOD=0x01;
	TH0=0xfe;
	TL0=frq%256;
	TR0=1;
}
/***********************************************************************************
****�������ƣ�LED���ƺ���
****�������ã�
****����������
************************************************************************************/
void LEDControl(void)
{
	switch (RDataBuf[5])
	{
		case 0x00:
			D0=RDataBuf[6];
			break;
		case 0x01:
			D1=RDataBuf[6];
			break;
		case 0x02:
			D2=RDataBuf[6];
			break;
		case 0x03:
			D3=RDataBuf[6];
			break;
		case 0x04:
			D4=RDataBuf[6];
			break;
		case 0x05:
			D5=RDataBuf[6];
			break;
		case 0x06:
			D6=RDataBuf[6];
			break;
		case 0x07:
			D7=RDataBuf[6];
			break;
		case 0x08:
			P1=0x00;
			break;
		case 0x09:
			P1=0xff;
			break;
		default:
			break;
	}
}
/***********************************************************************************
****�������ƣ����������ƺ���
****�������ã�
****����������
************************************************************************************/
void BEEPControl(void)
{
	switch(RDataBuf[5])
	{
		case 0x00:
			ET0=0;
			break;
		case 0x01:
			ET0=1;
			frq=RDataBuf[6];
			break;
		default:
			break;
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
	unsigned char   *pBuf;
	unsigned char   mBuf[64];
	CH375_WRCMD(CMD_GET_STATUS);               //����ȡ375���ж����ͣ�״̬�������ȡ���ж��ź�
	
	temp=CH375_REDAT();                        //��ȡ�ж����ͣ�״̬��ֵ
	IE0=0;
	switch(temp)                                  //�ж��ж����ͣ�״̬���������������375ͷ�ļ��еĶ���
	{
		case  USB_INT_EP2_OUT:  											// �����˵��´��ɹ� 
			pBuf=mBuf;																	//����δ����
			CH375_WRCMD(CMD_RD_USB_DATA);									//��������������
			length=CH375_REDAT();											//���ȶ������ǳ���														
			for(i=0;i<length;i++)
				RDataBuf[i]=CH375_REDAT();
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
			    mVarSetupLength=request.buffer[6]&0x7f;							//���ƴ������ݳ����������Ϊ128
				if((c1=request.r.bmReuestType)&0x40)
				{         					 //��������δ����
				}
				if((c1=request.r.bmReuestType)&0x20)
				{          					//������δ����
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
								VarSetupDescr=DevDes;
							else if(request.buffer[3]==2)		 					//�����������ϴ�
								VarSetupDescr=ConDes;
							/*else if(request.buffer[3]==3) 
							{
								if ( request.buffer[2]== 0 ) 
									VarSetupDescr=LangDes;
								else 
									VarSetupDescr=SerDes; 						//���ַ�������
							}*/
							else if(request.buffer[3]==0x22)
								VarSetupDescr=Hid_des;
							mCh375DesUp();											//������������֧��					          							
							break;
						case DEF_USB_GET_CONFIG:									//�������
							request.buffer[0]=0;									//û��������0
							if(CH375CONFLAG) request.buffer[0]=1;									//�Ѿ�������1����������������涨��
							break;
						case DEF_USB_SET_CONFIG:                 					//��������
							CH375CONFLAG=0;
							//CH375ACT=1;
							if ( request.buffer[2] != 0 ) 
							{
								CH375CONFLAG=1;											//�������ñ�־
								//CH375ACT=0;												//�����������ź�
							}
							break;
						case DEF_USB_GET_INTERF:										//�õ��ӿ�
							request.buffer[0]=1;									//�ϴ��ӿ�����������ֻ֧��һ���ӿ�
							break;
						default :
							CH375FLAGERR=1;											//��֧�ֵı�׼����
							break;
					}
				}
			}
			else {  //��֧�ֵĿ��ƴ��䣬����8�ֽڵĿ��ƴ���
				CH375FLAGERR=1;
			}
			if(!CH375FLAGERR) mCh375Ep0Up();										//û�д���/���������ϴ���������Ϊ0�ϴ�Ϊ״̬
			else {
				CH375_WRCMD(CMD_SET_ENDP3);								//���ö˵�1ΪSTALL��ָʾһ������
				CH375_WRDAT(0x0F);
			}
			break;
		case   USB_INT_EP0_IN:													//���ƶ˵��ϴ��ɹ�
			if(mVarSetupRequest==DEF_USB_GET_DESCR)
			{								//�������ϴ�
				mCh375DesUp();
				mCh375Ep0Up();															
			}
			else if(mVarSetupRequest==DEF_USB_SET_ADDRESS)
			{							//���õ�ַ
				CH375_WRCMD(CMD_SET_USB_ADDR);
				CH375_WRDAT(VarUsbAddress);								//����USB��ַ,�����´������USB��ַ
			}
			CH375_WRCMD(CMD_UNLOCK_USB);								//�ͷŻ�����
			break;
		case   USB_INT_EP0_OUT:													//���ƶ˵��´��ɹ�
			CH375_WRCMD(CMD_RD_USB_DATA);									//��������������
			if(length=CH375_REDAT())
			{										//����Ϊ0����
				for(len=0;len!=length;len++)c1=CH375_REDAT();					//ȡ���´�����
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
****�������ƣ����������к���
****�������ã�
****����������
************************************************************************************/
void Timer0(void) interrupt 1 using 1
{
	TH0=0xfe;
	TL0=frq%256;
	BEEP=~BEEP;
}
/***********************************************************************************
****�������ƣ�������
****�������ã�
****����������
************************************************************************************/
void main(void)
{
	unsigned char i;
	//delay50ms();                              //��ʱ50ms    
	
	//ע�͵���θ����Ӳ���Ƿ���ȷ��ʵ��Ӧ���п��Բ���
   /* CH375_WRCMD(CMD_CHECK_EXIST);
	CH375_WRDAT(0x01);
	i=~0x01;
    if(CH375_REDAT()==i)
	P1=i;
	else
    P1=0;
	*/
	delay50ms( );	/* ��ʱ�ȴ�CH375��ʼ�����,�����Ƭ����CH375�ṩ��λ�ź��򲻱���ʱ */
	CH375_Init( );  /* ��ʼ��CH375 */
    EA=1;
	BEEPInit();
    while(1)		/* ������ */
	{
		if(RDataBuf[0]==0x00)
		{
			switch(RDataBuf[4])
			{
				case 0x00:
					LEDControl();
					break;
				case 0x01:
					BEEPControl();
					break;
			}
		}
	}
}








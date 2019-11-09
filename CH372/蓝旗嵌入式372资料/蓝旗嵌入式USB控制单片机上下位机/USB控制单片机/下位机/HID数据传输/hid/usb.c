#include  "include.h"

/***********************************************************************************
****函数名称：定义数据收发缓冲区
****函数作用：
****函数描述：
************************************************************************************/
unsigned char idata RDataBuf[64];//定义接收缓存区
unsigned char idata TDataBuf[64];//定义发送缓存区
unsigned char idata buf1[8];
/***********************************************************************************
****函数名称：定义单片机外设接口
****函数作用：
****函数描述：
************************************************************************************/
sbit D0=P1^0;//LED
sbit D1=P1^1;
sbit D2=P1^2;
sbit D3=P1^3;
sbit D4=P1^4;
sbit D5=P1^5;
sbit D6=P1^6;
sbit D7=P1^7;
sbit BEEP=P3^6;//蜂鸣器
/***********************************************************************************
****函数名称：定义全局变量
****函数作用：
****函数描述：
************************************************************************************/
unsigned char frq=0;//鸣叫的频率
unsigned char num;

unsigned char mVarSetupRequest;						//	;USB请求码
unsigned char mVarSetupLength;					//		;后续数据长度
unsigned char  code * VarSetupDescr;						//	;描述符偏移地址

unsigned char VarUsbAddress	;					//
unsigned char code *SetupDesc;
bit CH375FLAGERR;						//错误清0
bit	CH375CONFLAG;

/***********************************************************************************
****函数名称：命令结构体
****函数作用：
****函数描述：
************************************************************************************/
typedef	union _REQUEST_PACK
{
	unsigned char  buffer[8];
	struct{
		unsigned char	 bmReuestType;    	 //标准请求字
		unsigned char	 bRequest;		   	//请求代码
		unsigned int     wValue;			//特性选择高
		unsigned int     wIndx;				//索引
		unsigned int     wLength;				//数据长度
	}r;
} mREQUEST_PACKET,	*mpREQUEST_PACKET;
mREQUEST_PACKET request;
/***********************************************************************************
****函数名称：设备描述符
****函数作用：
****函数描述：
************************************************************************************/
unsigned char  code DevDes[]=
{
	0x12,			//描述符大小			
    0x01,			//常数DEVICE
    0x10,			//USB规范版本信息，2个字节
    0x01,
    0x00,			//类别码，数值0X01~0XFE为USB定义的类，如打印机等，
	                //0XFF是由厂商指定的类别；0为HID类，它的类别在配置描述符中的接口描述符指定；
    0x00,			//子类别码	
    0x00,			//协议码
    0x08,			//端点0的最大信息包大小
    0x48,			//厂商ID
    0x43,
    0x37,			//产品ID	
    0x55,
    0x00,			//设备版本信息
    0x01,
    0x00,			//索引值	
    0x00,
    0x00,
    0x00,			//可能配置的数目	
};
/***********************************************************************************
****函数名称：配置描述符
****函数作用：
****函数描述：
************************************************************************************/
unsigned char   code ConDes[]=
{	
	0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x04, 0xa0,  0x23,//配置描述符；第二字节为0X02
	0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00,  0x05,//接口描述符；第二字节为0X04 
	0x09, 0x21, 0x00, 0x01, 0x00, 0x01, 0x22, 0x22,  0x00,//HID描述符；第二字节为0X21
	0x07, 0x05, 0x82, 0x03, 0x40, 0x00, 0x18, //端点描述符；第二字节为0X05
	0x07, 0x05, 0x02, 0x03, 0x40, 0x00, 0x18	 //端点描述符；
};		//配置描述符		
/***********************************************************************************
****函数名称：HID描述符
****函数作用：
****函数描述：
************************************************************************************/	
unsigned char code Hid_des[]=
{
  0x06, 0x00,0xff,
  0x09, 0x01,  
  0xa1, 0x01, 	  //集合开始
 
  0x09, 0x02, 		//Usage Page  用法
  0x15, 0x00, 	    //Logical  Minimun
  0x26, 0x00,0xff,		//Logical  Maximun
  0x75, 0x08,  		//Report Size
  0x95, 0x40,  		//Report Counet
  0x81, 0x06,  		//Input
  0x09, 0x02, 		//Usage Page  用法
  0x15, 0x00, 		//Logical  Minimun
  0x26, 0x00,0xff,		//Logical  Maximun
  0x75, 0x08,  		//Report Size
  0x95, 0x40,  		//Report Counet
  0x91, 0x06,  		//Output

  0xC0  
};
/***********************************************************************************
****函数名称：语言描述符
****函数作用：
****函数描述：
************************************************************************************/
unsigned char  code LangDes[]={0x04,0x03,0x09,0x04};		//语言描述符
/***********************************************************************************
****函数名称：字符串描述符
****函数作用：
****函数描述：
************************************************************************************/
//unsigned char  code SerDes[]={0x12,0x03,'C',0,'H',0,'3',0,'7',0,'2',0,'U',0,'S',0,'B',0};		//字符串描述符
unsigned char code SerDes[]={0x12,0x03,0x43,0x00,0x48,0x00,0x33,0x00,0x37,0x00,0x32,0x00,0x55,0x00,0x53,0x00,
	0x42,0x00};
/***********************************************************************************
****函数名称：CH372/375初始化函数
****函数作用：
****函数描述：
************************************************************************************/
void	CH375_Init( )
{
/* 设置USB工作模式, 必要操作 */
	CH375_WRCMD( CMD_SET_USB_MODE );
	CH375_WRDAT( 1 );  									/* 设置为使用内置固件的USB设备方式 */
	for ( ;; ) 
	{  											/* 等待操作成功,通常需要等待10uS-20uS */
		if ( CH375_REDAT( )==CMD_RET_SUCCESS ) break;
	}

/* 下述启用中断,假定CH375连接在INT0 */
	IT0 = 0;  /* 置外部信号为低电平触发 */
	IE0 = 0;  /* 清中断标志 */
	EX0 = 1;  /* 允许CH375中断 */
}
/***********************************************************************************
****函数名称：端点0数据上传函数
****函数作用：
****函数描述：
************************************************************************************/
void mCh375Ep0Up()
{
	unsigned char i,len;
	if(mVarSetupLength)
	{												//长度不为0传输具体长度的数据
		if(mVarSetupLength<=8)
		{
			len=mVarSetupLength;
			mVarSetupLength=0;
        }	//长度小于8则长输要求的长度
		else
		{
			len=8;
			mVarSetupLength-=8;
		}							                        		//长度大于8则传输8个，切总长度减8
	    CH375_WRCMD(CMD_WR_USB_DATA3);						//发出写端点0的命令
       	CH375_WRDAT(len);										//写入长度
    	for(i=0;i!=len;i++)
        CH375_WRDAT(request.buffer[i]);	              		//循环写入数据
    }
	else
	{
		CH375_WRCMD(CMD_WR_USB_DATA3);						//发出写端点0的命令
		CH375_WRDAT(0);					                   //上传0长度数据，这是一个状态阶段
	}
}
/***********************************************************************************
****函数名称：描述符复制函数
****函数作用：
****函数描述：
************************************************************************************/
void mCh375DesUp()
{
	unsigned char k;        
	for (k=0; k!=8; k++ ) 
	{
         request.buffer[k]=*VarSetupDescr;  								//依次复制8个描述符，
         VarSetupDescr++;
    }
}
/***********************************************************************************
****函数名称：外部中断0的响应函数
****函数作用：通过判断375的终端类型值进行相应的处理
****函数描述：
************************************************************************************/
void int0(void) interrupt 0 using 0
{
	unsigned char i,temp,length,len;
	unsigned char c1;
	//unsigned char data buf[64];                   //接受到的数据存放的缓冲数组，64个位置
	//unsigned char   *pBuf;
	unsigned char   mBuf[64];
	CH375_WRCMD(CMD_GET_STATUS);               //发获取375的中断类型（状态）命令，并取消中断信号
	
	temp=CH375_REDAT();                        //读取中断类型（状态）值
	IE0=0;
	switch(temp)                                  //判断中断类型（状态），具体区分请见375头文件中的定义
	{
		case  USB_INT_EP2_OUT:  											// 批量端点下传成功 
			//pBuf=mBuf;																	//数据未处理
			CH375_WRCMD(CMD_RD_USB_DATA);									//发出读数据命令
			length=CH375_REDAT();											//首先读出的是长度														
			//for(len=0;len!=length;len++,pBuf++)
				//*pBuf=CH375_REDAT();	//将数据读入到缓冲区			 
			//pBuf=mBuf;
			for(i=0;i<length;i++)
				RDataBuf[i]=CH375_REDAT();
			P1=RDataBuf[2];
            // 演示回传
			CH375_WRCMD(CMD_WR_USB_DATA7);								//发出写上传端点命令
			CH375_WRDAT(length);	
			//for(len=0;len!=length;len++,pBuf++)
				//CH375_WRDAT(*pBuf);	//将数据写入上传端点
			for(i=0;i<length;i++)
				CH375_WRDAT(RDataBuf[i]);
			break;
			
		case   USB_INT_EP2_IN:												 //批量端点上传成功,未处理
			CH375_WRCMD(CMD_UNLOCK_USB);								//释放缓冲区
			break;
		case   USB_INT_EP1_IN:	 											//中断端点上传成功，未处理
			CH375_WRCMD(CMD_UNLOCK_USB);								//释放缓冲区
			break;
		case   USB_INT_EP1_OUT:	  											//中断端点下传成功，未处理
			CH375_WRCMD(CMD_RD_USB_DATA);									//发出读数据命令
			if(length=CH375_REDAT())
			{										//长度为0跳出
				for(len=0;len!=length;len++)
					c1=CH375_REDAT();					//取出下传数据
			}
//			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);								//释放缓冲区,如果前面没有CMD_RD_USB_DATA命令则使用本命令
			break;
		case   USB_INT_EP0_SETUP: 											//控制端点建立成功
	    	CH375_WRCMD(CMD_RD_USB_DATA);
			length=CH375_REDAT();
			for(len=0;len!=length;len++)
				request.buffer[len]=CH375_REDAT();  // 取出数据
			if(length==0x08)
			{
			    mVarSetupLength=request.buffer[6];							//控制传输数据长度最大设置为128
				if((c1=request.r.bmReuestType)&0x40)
				{         					 //厂商请求，未处理
				}
				if((c1=request.r.bmReuestType)&0x20)  //类请求，未处理
				{   
					if(request.buffer[1]==0x0a)       //设置接口
					{//printf("c\n");
					}												//SET_IDLE
					else 
						if(request.buffer[1]==0x09)
					    {
					    }//类请求，未处理
				}
				if(!((c1=request.r.bmReuestType)&0x60))
				{          				//标准请求
					mVarSetupRequest=request.r.bRequest;							//暂存标准请求码
					switch(request.r.bRequest)
					{  // 分析标准请求
						case DEF_USB_CLR_FEATURE:									//清除特性
							if((c1=request.r.bmReuestType&0x1F)==0X02)
							{					//不是端点不支持
								switch(request.buffer[4])
								{
									case 0x82:
										CH375_WRCMD(CMD_SET_ENDP7);					//清除端点2上传
										CH375_WRDAT(0x8E);                			//发命令清除端点
										break;
									case 0x02:
										CH375_WRCMD(CMD_SET_ENDP6);
										CH375_WRDAT(0x80);							//清除端点2下传
										break;
									case 0x81:
										CH375_WRCMD(CMD_SET_ENDP5);					//清除端点1上传
										CH375_WRDAT(0x8E);
										break;
									case 0x01:
										CH375_WRCMD(CMD_SET_ENDP4);					//清除端点1下传
										CH375_WRDAT(0x80);
										break;
									default:
										break;
								}
							}
							else
							{
								CH375FLAGERR=1;								//不支持的清除特性，置错误标志
							}
							break;
						case DEF_USB_GET_STATUS:								//获得状态
							request.buffer[0]=0;
							request.buffer[1]=0;								//上传状态
							break;
						case DEF_USB_SET_ADDRESS:								//设置地址
							VarUsbAddress=request.buffer[2];					//暂存USB主机发来的地址
							break;
						case DEF_USB_GET_DESCR: 								//获得描述符
							if(request.buffer[3]==1)							//设备描述符上传
							{
								VarSetupDescr=DevDes;
								CH375FLAGERR=0;
					        }
							else if(request.buffer[3]==2)		 					//配置描述符上传
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
						        	mVarSetupLength=0x12; 						//做字符串处理
						        }
							}
							else
								CH375FLAGERR=1;
							mCh375DesUp();									//其余描述符不支持					          							
							break;
						case DEF_USB_GET_CONFIG:								//获得配置
							request.buffer[0]=0;									//没有配置则传0
							if(CH375CONFLAG) 
								request.buffer[0]=1;								//已经配置则传1；这是在描述符里规定的
							break;
						case DEF_USB_SET_CONFIG:                 				//设置配置
							CH375CONFLAG=0;
							//CH375ACT=1;
							if ( request.buffer[2] != 0 ) 
							{
								CH375CONFLAG=1;									//设置配置标志
								//CH375ACT=0;										//输出配置完成信号
							}
							break;
						case DEF_USB_GET_INTERF:								//得到接口
							request.buffer[0]=1;									//上传接口数，本事例只支持一个接口
							break;
						default :
							CH375FLAGERR=1;										//不支持的标准请求
							break;
					}
				}
			}
			else 
			{  //不支持的控制传输，不是8字节的控制传输
				CH375FLAGERR=1;
			}
			if(!CH375FLAGERR) 
				mCh375Ep0Up();										    //没有错误/调用数据上传，，长度为0上传为状态
			else 
			{
				CH375_WRCMD(CMD_SET_ENDP3);								//设置端点1为STALL，指示一个错误
				CH375_WRDAT(0x0F);
			}
			break;
		case   USB_INT_EP0_IN:												//控制端点上传成功
			if(mVarSetupRequest==DEF_USB_GET_DESCR)
			{	//描述符上传
				mCh375DesUp();
				mCh375Ep0Up();															
			}
			else if(mVarSetupRequest==DEF_USB_SET_ADDRESS)
			{	//设置地址
				CH375_WRCMD(CMD_SET_USB_ADDR);
				CH375_WRDAT(VarUsbAddress);								//设置USB地址,设置下次事务的USB地址
			}
			CH375_WRCMD(CMD_UNLOCK_USB);								    //释放缓冲区
			break;
		case   USB_INT_EP0_OUT:												//控制端点下传成功
			CH375_WRCMD(CMD_RD_USB_DATA);									//发出读数据命令
			if(length=CH375_REDAT())
			{										                        //长度为0跳出
				for(len=0;len!=length;len++)
				{
				    RDataBuf[len]=CH375_REDAT();					    //取出下传数据
				}
			}
			break;
		default:
			if((temp&0x03)==0x03)
			{									//总线复位
				CH375FLAGERR=0;													//错误清0
				CH375CONFLAG=0;													//配置清0
				mVarSetupLength=0;
				//CH375ACT=1;														//清配置完成输出
			}
			else
			{																//命令不支持
				;
			}
			CH375_WRCMD (CMD_UNLOCK_USB);									//释放缓冲区
			break;
	}
}

/***********************************************************************************
****函数名称：主函数
****函数作用：
****函数描述：
************************************************************************************/
void main(void)
{
	unsigned char i;

	delay50ms( );	/* 延时等待CH375初始化完成,如果单片机由CH375提供复位信号则不必延时 */
	CH375_Init( );  /* 初始化CH375 */
    EA=1;
	while(1)		/* 主程序 */
	{
		
	}
}








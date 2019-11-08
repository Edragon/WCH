/*
CH372 外部固件方式范例
这里仅处理标准请求和端点2的简单读写
*/

/* MCS-51单片机C语言的示例程序 */
//#pragma NOAREGS
#include <reg52.h>
//#include <AT89x52.H>
#include <string.h>
#include "CH375INC.H"
#include "stdio.h"
#include "main.h"


//unsigned char data BLK[10] _at_ 0x30;
unsigned char data BLK[10] _at_ 0x30;
unsigned char data RevBuffer[10];

//unsigned char bord;
unsigned char idata UPDATA_FLAG;
unsigned char idata buf[8];//unsigned char idata buf[8];
unsigned char  up_time;     


//unsigned char data BLK[10] _at_ 0x30; //读卡器的定义接收区
//sbit    OUT         =           P2^0;           //接收IO口定义


typedef	union _REQUEST_PACK{
	unsigned char  buffer[8];
	struct{
		unsigned char	 bmReuestType;    	 //标准请求字
		unsigned char	 bRequest;		   	//请求代码
		unsigned int     wValue;			//特性选择高
		unsigned int     wIndx;				//索引
		unsigned int     wLength;				//数据长度
	}r;
} mREQUEST_PACKET,	*mpREQUEST_PACKET;

//设备描述符
unsigned char  code DevDes[]={
						0x12			//描述符大小			
					  , 0x01			//常数DEVICE
					 , 0x10				//USB规范版本信息
				     , 	0x01
					,   0x00			//类别码，
					,  0x00				//子类别码	
					,   0x00			//协议码
					,  0x08				//端点0的最大信息包大小
					,  0x3c				//厂商ID
					,   0x41
					,   0x03			//产品ID	
					,   0x20
					,   0x00			//设备版本信息
					,   0x02
					,   0x01			//索引值	
					,   0x02
					,   0x00
					,   0x01			//可能配置的数目	
					};
//配置描述符
unsigned char   code ConDes[]={			//配置描述符
			   0x09, 0x02, 0x22, 0x00, 0x01, 0x01, 0x04, 0xa0,  0x23,
		       0x09, 0x04, 0x00, 0x00, 0x01, 0x03, 0x01, 0x01, 0x05, 
			   0x09, 0x21, 0x10, 0x01, 0x00, 0x01, 0x22, 0x41, 0x00,
		       0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x18
				};		//配置描述符
//unsigned char  code LangDes[]={0x04,0x03,0x09,0x04};		//语言描述符
//unsigned char  code SerDes[]={0x12,0x03,'C',0,'H',0,'3',0,'7',0,'5',0,'U',0,'S',0,'B',0};		//字符串描述符


unsigned char code Hid_des[]={0x05, 0x01, 0x09, 0x06,  0xa1, 0x01, 0x05, 0x07,  0x19, 0xe0, 
							  0x29, 0xe7, 0x15, 0x00,  0x25, 0x01, 0x75, 0x01,  0x95, 0x08, 
							  0x81, 0x02, 0x95, 0x01,  0x75, 0x08, 0x81, 0x01,  0x95, 0x03, 0x75, 0x01,        
               				  0x05, 0x08, 0x19, 0x01,  0x29, 0x03, 0x91, 0x02,  0x95, 0x01, 0x75, 0x05,  
							  0x91, 0x01, 0x95, 0x06,  0x75, 0x08, 0x15, 0x00,  0x26, 0xff, 0x00, 0x05,
					    	  0x07, 0x19, 0x00, 0x2a,  0xff, 0x00, 0x81, 0x00,  0xc0
							  };

unsigned char mVarSetupRequest;						//	;USB请求码
unsigned char mVarSetupLength;					//		;后续数据长度
unsigned char  code * VarSetupDescr;//	;描述符偏移地址
unsigned char buf1[8];
unsigned char VarUsbAddress	;					//
sbit	CH375_INT_WIRE		=		0xB0^2;	/* P3.2, INT0, 连接CH375的INT#引脚,用于查询中断状态 */
bit CH375FLAGERR;						//错误清0
bit	CH375CONFLAG;

unsigned char volatile xdata CH375_CMD_PORT _at_ 0xBDF1;		/* CH375命令端口的I/O地址 */
unsigned char volatile xdata CH375_DAT_PORT _at_ 0xBCF0;		/* CH375数据端口的I/O地址 */

mREQUEST_PACKET  request;
sbit  CH375ACT  = P1^4;

/*
void	mInitSTDIO( )
{
	SCON = 0x50;
	PCON = 0x80;
	TMOD = 0x20;
	TH1 = 0xf3;  // 24MHz晶振, 9600bps 
	TR1 = 1;
	TI = 0;
}
*/
/* 延时2微秒,不精确 */
void Delay1us(){
	;
}

void	Delay2us( )
{
	unsigned char i;
#define DELAY_START_VALUE	1  								/* 根据单片机的时钟选择初值,20MHz以下为0,30MHz以上为2 */
	for ( i=DELAY_START_VALUE; i!=0; i-- );
}

/* 延时50毫秒,不精确 */
void	Delay50ms( )
{
	unsigned char i, j;
	for ( i=200; i!=0; i-- ) for ( j=250; j!=0; j-- );
}

/* 将PC机的低字节在前的16位字数据转换为C51的高字节在前的数据 */
//unsigned int	BIG_ENDIAN( unsigned int value )
//{
//	unsigned int  in, out;
//	in = value;
//	((unsigned char *)&out)[1] = ((unsigned char *)&in)[0];
//	((unsigned char *)&out)[0] = ((unsigned char *)&in)[1];
//	return( out );
//}

void CH375_WR_CMD_PORT( unsigned char cmd ) { 				 /* 向CH375的命令端口写入命令,周期不小于4uS,如果单片机较快则延时 */
	//delay2us();
	CH375_CMD_PORT=cmd;
	Delay2us( );
}

void CH375_WR_DAT_PORT( unsigned char dat ) { 				 /* 向CH375的数据端口写入数据,周期不小于1.5uS,如果单片机较快则延时 */
	CH375_DAT_PORT=dat;
	Delay1us();  											/* 因为MCS51单片机较慢所以实际上无需延时 */
}

unsigned char CH375_RD_DAT_PORT() {  						/* 从CH375的数据端口读出数据,周期不小于1.5uS,如果单片机较快则延时 */
	Delay1us( );  										/* 因为MCS51单片机较慢所以实际上无需延时 */
	return( CH375_DAT_PORT );

}
/* CH375初始化子程序 */
void	CH375_Init( )
{
/* 设置USB工作模式, 必要操作 */
	CH375_WR_CMD_PORT( CMD_SET_USB_MODE );
	CH375_WR_DAT_PORT( 1 );  									/* 设置为使用内置固件的USB设备方式 */
	for ( ;; ) {  											/* 等待操作成功,通常需要等待10uS-20uS */
		if ( CH375_RD_DAT_PORT( )==CMD_RET_SUCCESS ) break;
	}
}

//*********************************************************

//*********************************************************
//端点0数据上传
void mCh375Ep0Up(){
	unsigned char i,len;
	if(mVarSetupLength){												//长度不为0传输具体长度的数据
		if(mVarSetupLength<=8){
			len=mVarSetupLength;
			mVarSetupLength=0;
        }	//长度小于8则长输要求的长度
		else{
			len=8;
			mVarSetupLength-=8;
		}							                        		//长度大于8则传输8个，切总长度减8
	    CH375_WR_CMD_PORT(CMD_WR_USB_DATA3);						//发出写端点0的命令
       	CH375_WR_DAT_PORT(len);										//写入长度
    	for(i=0;i!=len;i++)
        CH375_WR_DAT_PORT(request.buffer[i]);	              		//循环写入数据
    }
	else{
		CH375_WR_CMD_PORT(CMD_WR_USB_DATA3);						//发出写端点0的命令
		CH375_WR_DAT_PORT(0);					                   //上传0长度数据，这是一个状态阶段
	}
}

//*********************************************************

//复制描述符以便上传
void mCh375DesUp(){
	unsigned char k;        
	for (k=0; k!=8; k++ ) {
         request.buffer[k]=*VarSetupDescr;  								//依次复制8个描述符，
         VarSetupDescr++;
    }
}

/* CH375中断服务程序INT0,使用寄存器组1 */
void	mCH375Interrupt( )// interrupt 0 using 1
{
	unsigned char InterruptStatus;
	unsigned char length, c1, len;
	unsigned char   *pBuf;
	unsigned char    mBuf[14];//unsigned char    mBuf[64];
	CH375_WR_CMD_PORT(CMD_GET_STATUS);  									/* 获取中断状态并取消中断请求 */
	InterruptStatus =CH375_RD_DAT_PORT();  									/* 获取中断状态 */
//////////////////////////////////////////////	IE0 = 0;  																/* 清中断标志,对应于INT0中断 */
	switch(InterruptStatus){  // 分析中断状态
		case  USB_INT_EP2_OUT:  											// 批量端点下传成功 
			pBuf=mBuf;																	//数据未处理
			CH375_WR_CMD_PORT(CMD_RD_USB_DATA);									//发出读数据命令
			length=CH375_RD_DAT_PORT();											//首先读出的是长度														
			for(len=0;len!=length;len++,pBuf++)*pBuf=CH375_RD_DAT_PORT();	//将数据读入到缓冲区			 
			pBuf=mBuf;
// 演示回传

//			CH375_WR_CMD_PORT(CMD_WR_USB_DATA7);								//发出写上传端点命令
//			CH375_WR_DAT_PORT(length);	
//			for(len=0;len!=length;len++,pBuf++)CH375_WR_DAT_PORT(*pBuf);	//将数据写入上传端点
			break;
		case   USB_INT_EP2_IN:												 //批量端点上传成功,未处理
			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);								//释放缓冲区
			break;
		case   USB_INT_EP1_IN:	 											//中断端点上传成功，未处理
			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);								//释放缓冲区
  
/*
//             if(up_time==11){                 
                for(i=0;i<8;i++) buf[i]=0;               //发送八个字节0，相当于释放按键1
				CH375_WR_CMD_PORT(CMD_WR_USB_DATA5);							
			    CH375_WR_DAT_PORT(8);
				for(i=0;i!=8;i++)		CH375_WR_DAT_PORT(buf[i]);

//             }
*/
			break;

		case   USB_INT_EP1_OUT:	  											//中断端点下传成功，未处理
			CH375_WR_CMD_PORT(CMD_RD_USB_DATA);									//发出读数据命令
			if(length=CH375_RD_DAT_PORT()){										//长度为0跳出
				for(len=0;len!=length;len++)c1=CH375_RD_DAT_PORT();					//取出下传数据
			}
			break;
		case   USB_INT_EP0_SETUP: 											//控制端点建立成功
	    	CH375_WR_CMD_PORT(CMD_RD_USB_DATA);
			length=CH375_RD_DAT_PORT();
			for(len=0;len!=length;len++)request.buffer[len]=CH375_RD_DAT_PORT();  // 取出数据
			if(length==0x08){
			    mVarSetupLength=request.buffer[6];							//控制传输数据长度最大设置为128
				if((c1=request.r.bmReuestType)&0x40){         					 //厂商请求，未处理
				}
				if((c1=request.r.bmReuestType)&0x20){// printf("b\n");         					//类请求，未处理
					if(request.buffer[1]==0x0a){//printf("c\n");
						}												//SET_IDLE
					else if(request.buffer[1]==0x09){
							UPDATA_FLAG=1;
					}
				}
				if(!((c1=request.r.bmReuestType)&0x60)){          				//标准请求
					mVarSetupRequest=request.r.bRequest;							//暂存标准请求码
					switch(request.r.bRequest){  // 分析标准请求
						case DEF_USB_CLR_FEATURE:									//清除特性
							if((c1=request.r.bmReuestType&0x1F)==0X02){					//不是端点不支持
								switch(request.buffer[4]){
									case 0x82:
										CH375_WR_CMD_PORT(CMD_SET_ENDP7);					//清除端点2上传
										CH375_WR_DAT_PORT(0x8E);                			//发命令清除端点
										break;
									case 0x02:
										CH375_WR_CMD_PORT(CMD_SET_ENDP6);
										CH375_WR_DAT_PORT(0x80);							//清除端点2下传
										break;
									case 0x81:
										CH375_WR_CMD_PORT(CMD_SET_ENDP5);					//清除端点1上传
										CH375_WR_DAT_PORT(0x8E);
										break;
									case 0x01:
										CH375_WR_CMD_PORT(CMD_SET_ENDP4);					//清除端点1下传
										CH375_WR_DAT_PORT(0x80);
										break;
									default:
										break;
								}
							}
							else{
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
								VarSetupDescr=DevDes;
							else if(request.buffer[3]==2)		 					//配置描述符上传
								VarSetupDescr=ConDes;
							else if(request.buffer[3]==0x22) {
								VarSetupDescr=Hid_des;
//								if ( request.buffer[2]== 0 ) VarSetupDescr=LangDes;
//								else VarSetupDescr=SerDes; 						//做字符串处理
							}
								mCh375DesUp();											//其余描述符不支持					          							
							break;
						case DEF_USB_GET_CONFIG:									//获得配置
							request.buffer[0]=0;									//没有配置则传0
							if(CH375CONFLAG) request.buffer[0]=1;									//已经配置则传1；这是在描述符里规定的
							break;
						case DEF_USB_SET_CONFIG:                 					//设置配置
							CH375CONFLAG=0;
							CH375ACT=1;
							if ( request.buffer[2] != 0 ) {
								CH375CONFLAG=1;											//设置配置标志
								CH375ACT=0;												//输出配置完成信号
							}
							break;
						case DEF_USB_GET_INTERF:										//得到接口
							request.buffer[0]=1;									//上传接口数，本事例只支持一个接口
							break;
						default :
							CH375FLAGERR=1;											//不支持的标准请求
							break;
					}
				}
			}
			else {  //不支持的控制传输，不是8字节的控制传输
				CH375FLAGERR=1;
			}
			if(!CH375FLAGERR) mCh375Ep0Up();										//没有错误/调用数据上传，，长度为0上传为状态
			else {
				CH375_WR_CMD_PORT(CMD_SET_ENDP3);								//设置端点1为STALL，指示一个错误
				CH375_WR_DAT_PORT(0x0F);
			}
			break;
		case   USB_INT_EP0_IN:													//控制端点上传成功
			if(mVarSetupRequest==DEF_USB_GET_DESCR){								//描述符上传
				mCh375DesUp();
				mCh375Ep0Up();															
			}
			else if(mVarSetupRequest==DEF_USB_SET_ADDRESS){							//设置地址
				CH375_WR_CMD_PORT(CMD_SET_USB_ADDR);
				CH375_WR_DAT_PORT(VarUsbAddress);								//设置USB地址,设置下次事务的USB地址
			}
			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);								//释放缓冲区
			break;
		case   USB_INT_EP0_OUT:													//控制端点下传成功
			CH375_WR_CMD_PORT(CMD_RD_USB_DATA);									//发出读数据命令
			if(length=CH375_RD_DAT_PORT()){										//长度为0跳出
//				printf("len=%d\n",(unsigned int)length);
				for(len=0;len!=length;len++){buf1[len]=CH375_RD_DAT_PORT();					//取出下传数据
//					printf("buf=%02x",(unsigned short)buf1[len]);
				   }
			}
//			printf("\n");
			break;
		default:
			if((InterruptStatus&0x03)==0x03){									//总线复位
				CH375FLAGERR=0;													//错误清0
				CH375CONFLAG=0;													//配置清0
				mVarSetupLength=0;
				CH375ACT=1;														//清配置完成输出
			}
			else{																//命令不支持
				;
			}
			CH375_WR_CMD_PORT (CMD_UNLOCK_USB);									//释放缓冲区
			CH375_RD_DAT_PORT();
			break;
	}
}

up_data( )
{
	unsigned char i;

//	for(i=0;i!=8;i++)														//发送0按键值
//		buf[i]=0;
//		buf[2]=0x59;
		CH375_WR_CMD_PORT(CMD_WR_USB_DATA5);								//发出写上传端点命令
	    CH375_WR_DAT_PORT(8);
		for(i=0;i!=8;i++)
	     	CH375_WR_DAT_PORT(buf[i]);

}



main( ) {
    unsigned char data_flag=0;       //有无数据标志
    unsigned char i,time,k;
    unsigned char real_data[11];      //需要上传的数据缓冲区
    //unsigned char       hHD[6];
    unsigned long ulHex;
	unsigned char idata ucsenddata[11];
	unsigned char code uckeycode1[11]={	 0x62  //0           
									    ,0x59  //1 
										,0x5a  //2 
										,0x5b  //3
										,0x5c  //4
										,0x5d  //5
										,0x5e  //6
										,0x5f  //7
										,0x60  //8
										,0x61  //9
										,0x58
									    };
	SPK_P=1;
//SPK_P=1;
	Delay50ms( );
	Delay50ms( );
	Delay50ms( );
	Delay50ms( );


    RED_LED=0;
//	Delay50ms( );
//	Delay50ms( );
	GREEN_LED=1;
SPK_P=0;
	Delay50ms( );
	Delay50ms( );


	Delay50ms( );	// 延时等待CH375初始化完成,如果单片机由CH375提供复位信号则不必延时 //
	//mInitSTDIO( );
	CH375_Init( );  // 初始化CH375 //
    //EA=1;


 //启动后，不输出数据，
	//data_flag=1;
    up_time=0;  time=0;k=0;
    for(i=0;i<8;i++) buf[i]=0;

	////////
 	while(1){		// 主程序 //
  	 		  	
 		if(CH375_INT_WIRE == 0){
  SEND: 			mCH375Interrupt( ); 
			 
          if(data_flag==1) {
	            if((UPDATA_FLAG==0x01)&&(k<=1)){      //先发八个字节的0包，相当于确保按键的取消
	                 for(i=0;i<8;i++) buf[i]=0;
	                 up_data( );
	                 k++;
	             }
	
	            else {
		             if((UPDATA_FLAG==0x01)&&(time%2==1)){     //按键按下
						 if(up_time<=11){
			//                    printf("%d\n",(unsigned short)up_time);
				             buf[2]=real_data[up_time];                       //发送八个字节，相当于发送1个按键
						 	 up_data( ); 
			                 up_time++;                   
						 }	
		             }
		             else if((UPDATA_FLAG==0x01)&&(time%2==0)){  //按键取消
		                 for(i=0;i<8;i++) buf[i]=0;
		                 up_data( );
		             }
		             time++;
	            }
           }
           //if(up_time==11) { up_time=0;   data_flag=0;  goto RD;}        //循环上传     
           if(up_time>11) 
		   {
	         for(i=0;i<8;i++) buf[i]=0;
	         up_data( );
        	 data_flag=0; up_time=0; 
			 SPK_P=1;
			 RED_LED=1;
			 GREEN_LED=0;
			 Delay50ms( );
			 Delay50ms( );
			 RED_LED=0;
			 GREEN_LED=1;
			 Delay50ms( );
			 Delay50ms( );
			 //Delay50ms( );
			 //Delay50ms( );
			 SPK_P=0;

			 //蜂鸣器、LED 操作
			 goto SEND;
		   };
	   	}
	}
}


 
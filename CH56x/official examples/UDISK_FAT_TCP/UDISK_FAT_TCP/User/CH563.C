
/******************************************************************************/
/* 头文件包含 */
#include <stdio.h>
#include <string.h>
#include "absacc.h"
#include "SYSFREQ.H"                                                            /* 系统定义相关头文件 */
#include "CH563SFR.H"
#include "EHCI_HCD.H"                                                           /* 主机控制器低层操作相关头文件 */
#include "CH563USBSFR.H"                                                        /* 芯片USB寄存器相关头文件 */
#include "Udisk.H"                                                              /* Udisk定义相关头文件 */
#include "PRINTF.H"                                                             /* 串口打印输出控制头文件 */
#include "CH563NETConfig.H"                                                     
#include "CH563NET.H"

/*****************************  USB  ******************************************/
/* 常、变量定义 */
UINT8 EHCI_Allocate_Buf[ EHCI_ALLOCATE_BUF_LEN ];                               /* 内存分配缓冲区(由于QH、QTD等结构提需要32字节对齐, 仅用来分配QH、QTD相关内存使用) */
__align( 4096 ) UINT8 EHCI_Data_Buf[ EHCI_DATA_BUF_LEN ];                       /* 内存数据缓冲区(需要4K对齐) */                                
__align( 4096 ) UINT8 EHCI_PERIODIC_LIST_Buf[ 4096 ];          /* 周期性帧列表缓冲区(需要4K对齐) */                                

/* 定义U盘相关结构体 */
BULK_ONLY_CMD    mBOC;                                                          /* BulkOnly传输结构定义 */
DISK_INFO        UDisk;                                                         /* U盘相关结构体定义 */                                                        

/* 以下定义的详细说明请看CH56XUFI.H文件 */
#define DISK_BASE_BUF_LEN        512                                      /* 默认的磁盘数据缓冲区大小为512字节,建议选择为2048甚至4096以支持某些大扇区的U盘,为0则禁止在.H文件中定义缓冲区并由应用程序在pDISK_BASE_BUF中指定 */
/* 如果需要复用磁盘数据缓冲区以节约RAM,那么可将DISK_BASE_BUF_LEN定义为0以禁止在.H文件中定义缓冲区,而由应用程序在调用CH56xLibInit之前将与其它程序合用的缓冲区起始地址置入pDISK_BASE_BUF变量 */

//#define NO_DEFAULT_ACCESS_SECTOR    1                                         /* 禁止默认的磁盘扇区读写子程序,下面用自行编写的程序代替它 */
//#define NO_DEFAULT_FILE_ENUMER        1                                         /* 禁止默认的文件名枚举回调程序,下面用自行编写的程序代替它 */
#define UNSUPPORT_USB_HUB            1                                          /* 默认不支持HUB */

#include "CH56XUFI.H"
#include "CH56XUFI.C"
/******************************************************************************/ 

/*****************************  NET  ******************************************/
/* 下面的缓冲区和全局变量必须要定义，库中调用 */
__align(16)UINT8    CH563MACRxDesBuf[(RX_QUEUE_ENTRIES )*16];        /* MAC接收描述符缓冲区，16字节对齐 */
__align(4) UINT8    CH563MACRxBuf[RX_QUEUE_ENTRIES*RX_BUF_SIZE];	 /* MAC接收缓冲区，4字节对齐 */
__align(4) SOCK_INF SocketInf[CH563NET_MAX_SOCKET_NUM];			     /* Socket信息表，4字节对齐 */
const UINT16 MemNum[8] = {CH563NET_NUM_IPRAW,
	                     CH563NET_NUM_UDP,
	                     CH563NET_NUM_TCP,
	                     CH563NET_NUM_TCP_LISTEN,
	                     CH563NET_NUM_TCP_SEG,
	                     CH563NET_NUM_IP_REASSDATA,
	                     CH563NET_NUM_PBUF,
	                     CH563NET_NUM_POOL_BUF
	                     };
const UINT16 MemSize[8] = {CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_IPRAW_PCB),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_UDP_PCB),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_PCB),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_PCB_LISTEN),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_SEG),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_IP_REASSDATA),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_PBUF) + CH563NET_MEM_ALIGN_SIZE(0),
	                      CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_PBUF) + CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_POOL_BUF)
	                     };
__align(4)UINT8 Memp_Memory[CH563NET_MEMP_SIZE];
__align(4)UINT8 Mem_Heap_Memory[CH563NET_RAM_HEAP_SIZE];
__align(4)UINT8 Mem_ArpTable[CH563NET_RAM_ARP_TABLE_SIZE];

#define RECE_BUF_LEN                          1024                               /* 接收缓冲区的大小 */

/* CH563相关定义 */
const UINT8 MACAddr[6] = {0x02,0x03,0x04,0x05,0x06,0x07};                       /* CH563MAC地址 */
const UINT8 IPAddr[4] = {192,168,1,10};                                          /* CH563IP地址 */
const UINT8 GWIPAddr[4] = {192,168,1,1};                                        /* CH563网关 */
const UINT8 IPMask[4] = {255,255,255,0};                                        /* CH563子网掩码 */

const UINT8 DESIP[4] = {192,168,1,88};                                         /* 目的IP地址 */
const UINT32 PCDesPort = 5000;                                                  /* 目的端口   */
const UINT32 CH563SourPort = 4000;                                              /* 源端口    */
UINT8 SocketId;                                                                 /* 保存socket索引，可以不用定义 */
UINT8 SocketRecvBuf[RECE_BUF_LEN];                                              /* socket接收缓冲区 */

UINT8 FileStatFlag  = 0;//
/******************************************************************************/
/*
bit7
bit6
bit5 1：
bit4 1：创建文件完成
bit3 1：当前网络收到数据
bit2 1：USB u盘插入
bit1 1：Socket连接
bit0 1:PHY连接
*/
UINT8 G_Flag=0;         //全局flag

#define PHY_Connect        0x01
#define Socket_Connect     0x02
#define UdiskConnect       0x04
#define RecFlag            0x08
#define FileComp           0x10

UINT8 USBStep = 1;
UINT8 NetStep = 1;

UINT8	AUDIO_FILE_NAME[16] = {"/CESHI.TXT"};

UINT8   RecBuf[1024];   //1k的数据缓存区
UINT16  RecLen = 0;

UINT8   ReponseBuf[24];   //1k的数据缓存区
UINT16  ResponseLen = 0;

UINT32  BUADRATE = 115200;
UINT32  FILELEN = 0;

/* 连接一个LED用于监控演示程序的进度,低电平LED亮 */
#define LED_OUT_INIT( )        { R32_PA_OUT |= ELINK; R32_PA_DIR |= ELINK; }    /* ELINK 高电平为输出方向 */
#define LED_OUT_ACT( )        { R32_PA_CLR |= ELINK; }                          /* ELINK 低电平驱动LED显示 */
#define LED_OUT_INACT( )    { R32_PA_OUT |= ELINK; }                            /* ELINK 高电平关闭LED显示 */
 
/****************  中断处理  *******************/
__irq void IRQ_Handler( void )
{
    if(R32_INT_FLAG & 0x8000)                                                   /* 以太网中断 */
    {                                                                           /* 以太网中断中断服务函数 */
        CH563NET_ETHIsr();
    }
    if(R32_INT_FLAG & RB_IF_TMR0)                                               /* 定时器中断 */
    {
         CH563NET_TimeIsr(CH563NETTIMEPERIOD);                                  /* 定时器中断服务函数 */
         R8_TMR0_INT_FLAG |= 0xff;                                              /* 清除定时器中断标志 */
   }
}

__irq void FIQ_Handler( void )
{
    while( 1 );
}

/****************  串口初始化  *******************/
void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / BUADRATE;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* 四舍五入 */
    R8_UART1_LCR = 0x80;                                                        /* DLAB位置1 */
    R8_UART1_DIV = 1;                                                           /* 预分频 */
    R8_UART1_DLM = x>>8;
    R8_UART1_DLL = x&0xff;

    R8_UART1_LCR = RB_LCR_WORD_SZ ;                                             /* 设置字节长度为8 */
    R8_UART1_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* 设置FIFO触发点为14，清发送和接收FIFO，FIFO使能 */
    R8_UART1_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD1|TXD1;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD1;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD1;                                                         /* TXD0 output enable */
}


int fputc( int c, FILE *f )
{
    R8_UART1_THR = c;                                                           /* 发送数据 */
    while( ( R8_UART1_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* 等待数据发送 */
    return( c );
}

/****************  定时器初始化  ****************/
void SysTimeInit(void)
{
    R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
    R32_TMR0_COUNT = 0x00000000; 
    R32_TMR0_CNT_END = 0x186a0 * CH563NETTIMEPERIOD;                            /* 设置为10MS定时 */
    R8_TMR0_INTER_EN |= RB_TMR_IE_CYC_END;
    R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
}

/****************  中断开启  ****************/
void InitSysHal(void)
{
    R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR0;                                          /* 开启TIM0中断 */
    R8_INT_EN_IRQ_1 |= RB_IE_IRQ_ETH;                                           /* 开启ETH中断 */
    R8_INT_EN_IRQ_GLOB |= RB_IE_IRQ_GLOB;                                       /* 开启IRQ全局中断 */
}

/****************  网络错误处理  ****************/
void mStopIfNetError(UINT8 iError)
{
    if (iError == CH563NET_ERR_SUCCESS) return;                                 /* 操作成功 */

    printf("Error: %02X\n", (UINT16)iError);                                    /* 显示错误 */
 
}

/****************  USB错误处理  ****************/
void mStopIfError( UINT8 iError )
{
    if( iError == ERR_SUCCESS ) 
    {    
        return;                                                                 /* 操作成功 */
    }
    printf( "Error: %02X\n", (UINT16)iError );                                  /* 显示错误 */
    
    /* 遇到错误后,应该分析错误码以及CH56xDiskStatus状态,例如调用CH56xDiskConnect查询当前U盘是否连接,如果U盘已断开那么就重新等待U盘插上再操作,
       建议出错后的处理步骤:
       1、调用一次CH56xDiskReady,成功则继续操作,例如Open,Read/Write等,在CH56xDiskReady中会自动调用CH56xDiskConnect，不必另外调用
       2、如果CH56xDiskReady不成功,那么强行将CH56xDiskStatus置为DISK_CONNECT状态,然后从头开始操作(等待U盘连接CH56xDiskConnect，CH56xDiskReady等) */
    while( 1 ) 
    {
        LED_OUT_ACT( );                                                         /* LED闪烁 */
        Delay_ms( 200 );
        LED_OUT_INACT( );
        Delay_ms( 200 );
    }
}

/************************ USB 相关函数 ******************************/
UINT8 Flie_Save( UINT8 *pname, UINT32 Num, UINT8 *Data )
{
	UINT8 i;
	strcpy( (PCHAR)&mCmdParam.Open.mPathName, (PCCHAR)pname ); 
	i = CH56xFileOpen( );											            /* 打开文件 */
#if MY_DEBUG_PRINTF
	printf( "CH563FileOpen:%x \n", i );
#endif
	if( i == ERR_SUCCESS ){	
	    mCmdParam.Locate.mSectorOffset = 0xFFFFFFFF;
		CH56xFileLocate( );												        /* 文件偏移到文件尾 */
	}
	else if( i == ERR_MISS_FILE ){
		strcpy( (PCHAR)&mCmdParam.Create.mPathName, (PCCHAR)pname ); 
		CH56xFileCreate( );   
	}																	        /* 创建文件 */
	else return( i );
#if MY_DEBUG_PRINTF
	printf( "CH563ByteWrite:%x \n", i );
#endif
	mCmdParam.Write.mSectorCount = 1;
	mCmdParam.Write.mDataBuffer = Data;
	i = CH56xFileWrite( );											            /* 写文件 */
#if MY_DEBUG_PRINTF
	printf( "CH563Write:%x \n", i );
#endif
    R32_PB_OUT &= ~(1<<0);  
	if( i != ERR_SUCCESS )	 return ( i );
	mCmdParam.Close.mUpdateLen = 1;
	i = CH56xFileClose( );											            /* 关闭文件，更新文件长度 */
#if MY_DEBUG_PRINTF
	printf( "CH563FileClose:%x \n" , i );
#endif
	return( i );
}

/****************  网络相关函数  ****************/
UINT8 CH563NET_LibInit(const UINT8 *ip,const UINT8 *gwip,const UINT8 *mask,const UINT8 *macaddr)
{
    UINT8 i;
    struct _CH563_CFG cfg;
    if(CH563NET_GetVer() != CH563NET_LIB_VER)return 0xfc;             /* 获取库的版本号，检查是否和头文件一致 */
    CH563NETConfig = LIB_CFG_VALUE;									  /* 将配置信息传递给库的配置变量 */
    cfg.RxBufSize = RX_BUF_SIZE; 
    cfg.TCPMss   = CH563NET_TCP_MSS;
    cfg.HeapSize = CH563_MEM_HEAP_SIZE;
    cfg.ARPTableNum = CH563NET_NUM_ARP_TABLE;
    cfg.MiscConfig0 = CH563NET_MISC_CONFIG0;
	CH563NET_ConfigLIB(&cfg);
    i = CH563NET_Init(ip,gwip,mask,macaddr);
    
	return (i); 				     /* 库初始化 */
}

void CH563NET_HandleSockInt(UINT8 sockeid,UINT8 initstat)
{
    UINT32 len;
//    UINT32 totallen;
	UINT8 i;
    if(initstat & SINT_STAT_RECV)                                               /* 接收中断 */
    {
		if(G_Flag&RecFlag)   
		{
			//error 上次的网络数据还没有写入到U盘中
		}
		else
		{R32_PB_OUT |= (1<<0); 
			//printf("网络收到数据 \r\n"); 
			len = CH563NET_SocketRecvLen(SocketId,NULL); 
			CH563NET_SocketRecv(SocketId,RecBuf,&len);
			RecLen = len;
			G_Flag |= RecFlag;
		}
    }
    if(initstat & SINT_STAT_CONNECT)                                            /* TCP连接中断 */  /* 产生此中断表示TCP已经连接，可以进行收发数据 */
    {
		G_Flag |= Socket_Connect;
    }
    if(initstat & SINT_STAT_DISCONNECT)                                         /* TCP断开中断 */
    {                                                       					/* 产生此中断，CH563库内部会将此socket清除，置为关闭*/
// 		i = CH563NET_SocketConnect(SocketId);                                        /* TCP连接 */
		G_Flag &= ~Socket_Connect;
		if(NetStep==4)
		{
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP连接 */
			mStopIfNetError(i); 
		}
    }
    if(initstat & SINT_STAT_TIM_OUT)                                            /* TCP超时中断 */   /* 产生此中断，CH563库内部会将此socket清除，置为关闭*/
    {                             
		G_Flag &= ~Socket_Connect;
		if(NetStep==4)
		{
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP连接 */
			mStopIfNetError(i); 
		}
// 		i = CH563NET_SocketConnect(SocketId);                                        /* TCP连接 */
// 		mStopIfNetError(i);   
    }
}

void CH563NET_HandleGlobalInt(void)
{
	UINT8 initstat;
	UINT8 i;
	UINT8 socketinit;

	initstat = CH563NET_GetGlobalInt();                                          /* 读全局中断状态并清除 */
	if(initstat & GINT_STAT_UNREACH)                                            /* 不可达中断 */
	{
		
	}
	if(initstat & GINT_STAT_IP_CONFLI)                                           /* IP冲突中断 */
	{

	}
	if(initstat & GINT_STAT_PHY_CHANGE)                                          /* PHY改变中断 */
	{
		i = CH563NET_GetPHYStatus();                                             /* 获取PHY状态 */
		if(i&PHY_DISCONN)   //PHY断开
		{
			G_Flag &= ~PHY_Connect;
		}
		else
		{
			G_Flag |= PHY_Connect;
		}
	}
	if(initstat & GINT_STAT_SOCKET)                                              /* Socket中断 */
	{
		for(i = 0; i < CH563NET_MAX_SOCKET_NUM; i ++)                     
		{
			socketinit = CH563NET_GetSocketInt(i);                               /* 读socket中断并清零 */
			if(socketinit)CH563NET_HandleSockInt(i,socketinit);                  /* 如果有中断则清零 */
		}    
	}
}

void CH563NET_CreatTcpSocket(void)
{
   UINT8 i;                                                             
   SOCK_INF TmpSocketInf;                                                       /* 创建临时socket变量 */

   memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* 库内部会将此变量复制，所以最好将临时变量先全部清零 */
   memcpy((void *)TmpSocketInf.IPAddr,DESIP,4);                                 /* 设置目的IP地址 */
   TmpSocketInf.DesPort = PCDesPort;                                            /* 设置目的端口 */
   TmpSocketInf.SourPort = CH563SourPort;                                       /* 设置源端口 */
   TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* 设置socekt类型 */
   TmpSocketInf.RecvStartPoint = (UINT32)SocketRecvBuf;                         /* 设置接收缓冲区的接收缓冲区 */
   TmpSocketInf.RecvBufLen = RECE_BUF_LEN ;                                     /* 设置接收缓冲区的接收长度 */
   i = CH563NET_SocketCreat(&SocketId,&TmpSocketInf);                           /* 创建socket，将返回的socket索引保存在SocketId中 */
   mStopIfNetError(i);                                                             /* 检查错误 */                                                    
}

/****************  处理过程函数  ****************/
void USBProcess(void)
{
	UINT8  status,i=0;
	CH56xDiskConnect();
	if(CH56xDiskStatus < DISK_CONNECT)  //u盘设备不在了
	{
		USBStep = 1;
		G_Flag &= ~UdiskConnect; //更新标志
	}
	
	if(USBStep==1)                        //等待设备接入
	{
		if(CH56xDiskStatus < DISK_CONNECT)   //没有
		{
			
		}
		else                                   //有优盘接入
		{
			G_Flag |= UdiskConnect;
			USBStep = 2;     
		}
		
	}
	if(USBStep==2)                            //有优盘接入，端点0初始化
	{
		status = USBHOST_EHCI_EP0_Init( &UDisk.Device );                        /* USB主机控制端点0初始化 */
		if( status != USB_OPERATE_SUCCESS )  //失败
		{
			printf("端点0初始化error \r\n");
			USBStep = 0xff;
		}
		else
		{
			printf("端点0初始化完成 \r\n");
			USBStep = 3;
		}
	}
	if(USBStep == 3)               //等待U盘Ready
	{
		while(i<10)
		{
			Delay_ms( 50 );
			if( CH56xDiskReady( ) == ERR_SUCCESS ) /* 查询磁盘是否准备好 */
			{
				USBStep = 4;	
				break;
			}
			if( CH56xDiskStatus < DISK_CONNECT )  /* 检测到断开,重新检测并计时 */
			{
				USBStep = 1;	
				break;
			}
			if( (CH56xDiskStatus >= DISK_MOUNTED) && (i > 5) )   /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
			{
				USBStep = 4;	
				break;														
			}
			i++;
		}
		if( CH56xDiskStatus < DISK_CONNECT ) 
        {  
            /* 检测到断开,重新检测并计时 */
            printf( "Device gone\n" );
			G_Flag &= ~UdiskConnect;
            USBStep = 1;                                                          /* 重新等待 */
        }
        if( CH56xDiskStatus < DISK_MOUNTED ) 
        {  
            /* 未知USB设备,例如USB键盘、打印机等 */
            printf( "Unknown device\n" );
			G_Flag &= ~UdiskConnect;
            USBStep = 0xff;
        }
	}
	if(USBStep==4)    //U盘准备好了，显示相关信息
	{
		printf( "TotalSize = %lu MB \n", mCmdParam.DiskReady.mDiskSizeSec >> ( 20 - CH56xvSectorSizeB ) );  /* 显示为以MB为单位的容量 */
        printf( "Current disk sector size = %d Bytes \n", CH56xvSectorSize );   /* CH56xvSectorSize是U盘的实际扇区大小 */
#if DISK_BASE_BUF_LEN
        if( DISK_BASE_BUF_LEN < CH56xvSectorSize ) 
        {  
            /* 检查磁盘数据缓冲区是否足够大,CH56xvSectorSize是U盘的实际扇区大小 */
            printf( "Too large sector size\n" );
            USBStep = 0xff;
			//goto UnknownUsbDevice;
        }
#endif
		USBStep = 5;
	}
	if(USBStep==5)  //判断是否移除
	{
		
	}
	if(USBStep==0xff)   //未知设备
	{
		printf("UnknownUsbDevice\r\n");
		G_Flag &= ~UdiskConnect;
        /* 重新初始化U盘相关参数并回收资源 */
        USBHOST_DevicePara_Init( &UDisk.Device );                               /* USB主机相关参数初始化 */
        USBHOST_Structure_DeInit( );                                            /* USB主机数据结构初始化 */
		USBStep = 1;
	}
}

void NetProcess(void)
{
	UINT8 i = 0;
	CH563NET_MainTask(); 
	if(CH563NET_QueryGlobalInt()) CH563NET_HandleGlobalInt();         
	if(NetStep==1)   //等待PHY连接
	{
		i = CH563NET_GetPHYStatus(); 
		if(i&PHY_DISCONN)
		{
			
		}
		else
		{
			G_Flag |= PHY_Connect;
			NetStep = 2;
			printf("PHY  connect \r\n");
		}
	}
	if(NetStep==2)  //创建Socket
	{
		CH563NET_CreatTcpSocket();
		printf("Socket0 OK  \r\n");
		NetStep = 3;
	}
	if(NetStep==3)
	{
		if(G_Flag&UdiskConnect)             //等待U盘准备好，再连接Socket
		{
			printf("U盘连接，Socket开始连接 \r\n");
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP连接 */
			mStopIfNetError(i);                                                             /* 检查错误 */  
			NetStep = 4;
		}
	}
	if(NetStep==4)  //等待连接成功
	{
		if(G_Flag&Socket_Connect) 
		{
			printf("Socket connect \r\n");
			NetStep = 5;
		}
	}
	if(NetStep==5)      //判断网络连接，和数据。
	{
		if((G_Flag&Socket_Connect)==0)  //Socket断开
		{
			NetStep = 4;
		}
		if((G_Flag&PHY_Connect)==0)     //PHY断开连接
		{
			NetStep = 1;
		}
		if((G_Flag&UdiskConnect)==0)    //U盘不在了
		{
			printf("u盘移除，关闭连接 \r\n");
			i = CH563NET_SocketClose(SocketId,TCP_CLOSE_NORMAL);  //关闭当前的Socket
			mStopIfNetError(i);  
			
			NetStep = 3;
		}
	}
}

void WriteFile(void)      
{
  UINT16 i;
	if(G_Flag&RecFlag)
	{
		Flie_Save( AUDIO_FILE_NAME, RecLen, RecBuf );
		G_Flag &= ~RecFlag;
		RecLen = 0;
		printf("写入文件完成 \r\n");
	}
}

/*******************************************************************************
* Function Name  : main
* Description    : 主程序
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int    main( void ) 
{
//    UINT8  status;
    UINT8  i;
//	UINT8 Filename[ ] = {"/ABC.TXT"};
    R32_PB_PU  |= 1;                                                  
    R32_PB_DIR |= 1;                                                  
    R32_PB_OUT &= ~(1<<0);                                               

//    SysFreq( );                                                               /* 根据需要设置PLL获得相应的时钟频率 */
    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* 开机后LED亮一下以示工作 */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    mInitSTDIO( );                                                              /* 为了让计算机通过串口监控演示过程 */

    printf( "start \xd\xa" );

#if DISK_BASE_BUF_LEN == 0
    pDISK_BASE_BUF = &my_buffer[0];                                             /* 不在.H文件中定义CH563的专用缓冲区,而是用缓冲区指针指向其它应用程序的缓冲区便于合用以节约RAM */
#endif

    /* USB主机相关初始化 */
    USBHOST_ModeSet( );                                                         /* 设置USB主机模式 */
    USBHOST_EHCI_Init( );                                                       /* USB主机EHCI复位及初始化 */
    USBHOST_Structure_DeInit( );                                                /* USB主机数据结构初始化 */
    USBHOST_DevicePara_Init( &UDisk.Device );                                   /* USB主机相关参数初始化 */

    i = CH56xLibInit( );                                                        /* 初始化CH563程序库和CH563芯片,操作成功返回0 */
    mStopIfError( i );
    /* 其它电路初始化 */
	i = CH563NET_LibInit(IPAddr,GWIPAddr,IPMask,MACAddr);                       /* 库初始化 */
	mStopIfNetError(i);                                                            /* 检查错误 */

    SysTimeInit();                                                              /* 系统定时器初始化 */
    InitSysHal();                                                               /* 初始化中断 */
	
    while ( 1 ) 
    {
        USBProcess();
		NetProcess();
		WriteFile();
    }
}



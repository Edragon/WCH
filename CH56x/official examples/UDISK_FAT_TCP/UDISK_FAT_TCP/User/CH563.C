
/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "absacc.h"
#include "SYSFREQ.H"                                                            /* ϵͳ�������ͷ�ļ� */
#include "CH563SFR.H"
#include "EHCI_HCD.H"                                                           /* �����������Ͳ�������ͷ�ļ� */
#include "CH563USBSFR.H"                                                        /* оƬUSB�Ĵ������ͷ�ļ� */
#include "Udisk.H"                                                              /* Udisk�������ͷ�ļ� */
#include "PRINTF.H"                                                             /* ���ڴ�ӡ�������ͷ�ļ� */
#include "CH563NETConfig.H"                                                     
#include "CH563NET.H"

/*****************************  USB  ******************************************/
/* ������������ */
UINT8 EHCI_Allocate_Buf[ EHCI_ALLOCATE_BUF_LEN ];                               /* �ڴ���仺����(����QH��QTD�Ƚṹ����Ҫ32�ֽڶ���, ����������QH��QTD����ڴ�ʹ��) */
__align( 4096 ) UINT8 EHCI_Data_Buf[ EHCI_DATA_BUF_LEN ];                       /* �ڴ����ݻ�����(��Ҫ4K����) */                                
__align( 4096 ) UINT8 EHCI_PERIODIC_LIST_Buf[ 4096 ];          /* ������֡�б�������(��Ҫ4K����) */                                

/* ����U����ؽṹ�� */
BULK_ONLY_CMD    mBOC;                                                          /* BulkOnly����ṹ���� */
DISK_INFO        UDisk;                                                         /* U����ؽṹ�嶨�� */                                                        

/* ���¶������ϸ˵���뿴CH56XUFI.H�ļ� */
#define DISK_BASE_BUF_LEN        512                                      /* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��,Ϊ0���ֹ��.H�ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
/* �����Ҫ���ô������ݻ������Խ�ԼRAM,��ô�ɽ�DISK_BASE_BUF_LEN����Ϊ0�Խ�ֹ��.H�ļ��ж��建����,����Ӧ�ó����ڵ���CH56xLibInit֮ǰ��������������õĻ�������ʼ��ַ����pDISK_BASE_BUF���� */

//#define NO_DEFAULT_ACCESS_SECTOR    1                                         /* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_FILE_ENUMER        1                                         /* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */
#define UNSUPPORT_USB_HUB            1                                          /* Ĭ�ϲ�֧��HUB */

#include "CH56XUFI.H"
#include "CH56XUFI.C"
/******************************************************************************/ 

/*****************************  NET  ******************************************/
/* ����Ļ�������ȫ�ֱ�������Ҫ���壬���е��� */
__align(16)UINT8    CH563MACRxDesBuf[(RX_QUEUE_ENTRIES )*16];        /* MAC������������������16�ֽڶ��� */
__align(4) UINT8    CH563MACRxBuf[RX_QUEUE_ENTRIES*RX_BUF_SIZE];	 /* MAC���ջ�������4�ֽڶ��� */
__align(4) SOCK_INF SocketInf[CH563NET_MAX_SOCKET_NUM];			     /* Socket��Ϣ����4�ֽڶ��� */
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

#define RECE_BUF_LEN                          1024                               /* ���ջ������Ĵ�С */

/* CH563��ض��� */
const UINT8 MACAddr[6] = {0x02,0x03,0x04,0x05,0x06,0x07};                       /* CH563MAC��ַ */
const UINT8 IPAddr[4] = {192,168,1,10};                                          /* CH563IP��ַ */
const UINT8 GWIPAddr[4] = {192,168,1,1};                                        /* CH563���� */
const UINT8 IPMask[4] = {255,255,255,0};                                        /* CH563�������� */

const UINT8 DESIP[4] = {192,168,1,88};                                         /* Ŀ��IP��ַ */
const UINT32 PCDesPort = 5000;                                                  /* Ŀ�Ķ˿�   */
const UINT32 CH563SourPort = 4000;                                              /* Դ�˿�    */
UINT8 SocketId;                                                                 /* ����socket���������Բ��ö��� */
UINT8 SocketRecvBuf[RECE_BUF_LEN];                                              /* socket���ջ����� */

UINT8 FileStatFlag  = 0;//
/******************************************************************************/
/*
bit7
bit6
bit5 1��
bit4 1�������ļ����
bit3 1����ǰ�����յ�����
bit2 1��USB u�̲���
bit1 1��Socket����
bit0 1:PHY����
*/
UINT8 G_Flag=0;         //ȫ��flag

#define PHY_Connect        0x01
#define Socket_Connect     0x02
#define UdiskConnect       0x04
#define RecFlag            0x08
#define FileComp           0x10

UINT8 USBStep = 1;
UINT8 NetStep = 1;

UINT8	AUDIO_FILE_NAME[16] = {"/CESHI.TXT"};

UINT8   RecBuf[1024];   //1k�����ݻ�����
UINT16  RecLen = 0;

UINT8   ReponseBuf[24];   //1k�����ݻ�����
UINT16  ResponseLen = 0;

UINT32  BUADRATE = 115200;
UINT32  FILELEN = 0;

/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED_OUT_INIT( )        { R32_PA_OUT |= ELINK; R32_PA_DIR |= ELINK; }    /* ELINK �ߵ�ƽΪ������� */
#define LED_OUT_ACT( )        { R32_PA_CLR |= ELINK; }                          /* ELINK �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT( )    { R32_PA_OUT |= ELINK; }                            /* ELINK �ߵ�ƽ�ر�LED��ʾ */
 
/****************  �жϴ���  *******************/
__irq void IRQ_Handler( void )
{
    if(R32_INT_FLAG & 0x8000)                                                   /* ��̫���ж� */
    {                                                                           /* ��̫���ж��жϷ����� */
        CH563NET_ETHIsr();
    }
    if(R32_INT_FLAG & RB_IF_TMR0)                                               /* ��ʱ���ж� */
    {
         CH563NET_TimeIsr(CH563NETTIMEPERIOD);                                  /* ��ʱ���жϷ����� */
         R8_TMR0_INT_FLAG |= 0xff;                                              /* �����ʱ���жϱ�־ */
   }
}

__irq void FIQ_Handler( void )
{
    while( 1 );
}

/****************  ���ڳ�ʼ��  *******************/
void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / BUADRATE;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* �������� */
    R8_UART1_LCR = 0x80;                                                        /* DLABλ��1 */
    R8_UART1_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART1_DLM = x>>8;
    R8_UART1_DLL = x&0xff;

    R8_UART1_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART1_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART1_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD1|TXD1;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD1;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD1;                                                         /* TXD0 output enable */
}


int fputc( int c, FILE *f )
{
    R8_UART1_THR = c;                                                           /* �������� */
    while( ( R8_UART1_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* �ȴ����ݷ��� */
    return( c );
}

/****************  ��ʱ����ʼ��  ****************/
void SysTimeInit(void)
{
    R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
    R32_TMR0_COUNT = 0x00000000; 
    R32_TMR0_CNT_END = 0x186a0 * CH563NETTIMEPERIOD;                            /* ����Ϊ10MS��ʱ */
    R8_TMR0_INTER_EN |= RB_TMR_IE_CYC_END;
    R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
}

/****************  �жϿ���  ****************/
void InitSysHal(void)
{
    R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR0;                                          /* ����TIM0�ж� */
    R8_INT_EN_IRQ_1 |= RB_IE_IRQ_ETH;                                           /* ����ETH�ж� */
    R8_INT_EN_IRQ_GLOB |= RB_IE_IRQ_GLOB;                                       /* ����IRQȫ���ж� */
}

/****************  ���������  ****************/
void mStopIfNetError(UINT8 iError)
{
    if (iError == CH563NET_ERR_SUCCESS) return;                                 /* �����ɹ� */

    printf("Error: %02X\n", (UINT16)iError);                                    /* ��ʾ���� */
 
}

/****************  USB������  ****************/
void mStopIfError( UINT8 iError )
{
    if( iError == ERR_SUCCESS ) 
    {    
        return;                                                                 /* �����ɹ� */
    }
    printf( "Error: %02X\n", (UINT16)iError );                                  /* ��ʾ���� */
    
    /* ���������,Ӧ�÷����������Լ�CH56xDiskStatus״̬,�������CH56xDiskConnect��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
       ���������Ĵ�������:
       1������һ��CH56xDiskReady,�ɹ����������,����Open,Read/Write��,��CH56xDiskReady�л��Զ�����CH56xDiskConnect�������������
       2�����CH56xDiskReady���ɹ�,��ôǿ�н�CH56xDiskStatus��ΪDISK_CONNECT״̬,Ȼ���ͷ��ʼ����(�ȴ�U������CH56xDiskConnect��CH56xDiskReady��) */
    while( 1 ) 
    {
        LED_OUT_ACT( );                                                         /* LED��˸ */
        Delay_ms( 200 );
        LED_OUT_INACT( );
        Delay_ms( 200 );
    }
}

/************************ USB ��غ��� ******************************/
UINT8 Flie_Save( UINT8 *pname, UINT32 Num, UINT8 *Data )
{
	UINT8 i;
	strcpy( (PCHAR)&mCmdParam.Open.mPathName, (PCCHAR)pname ); 
	i = CH56xFileOpen( );											            /* ���ļ� */
#if MY_DEBUG_PRINTF
	printf( "CH563FileOpen:%x \n", i );
#endif
	if( i == ERR_SUCCESS ){	
	    mCmdParam.Locate.mSectorOffset = 0xFFFFFFFF;
		CH56xFileLocate( );												        /* �ļ�ƫ�Ƶ��ļ�β */
	}
	else if( i == ERR_MISS_FILE ){
		strcpy( (PCHAR)&mCmdParam.Create.mPathName, (PCCHAR)pname ); 
		CH56xFileCreate( );   
	}																	        /* �����ļ� */
	else return( i );
#if MY_DEBUG_PRINTF
	printf( "CH563ByteWrite:%x \n", i );
#endif
	mCmdParam.Write.mSectorCount = 1;
	mCmdParam.Write.mDataBuffer = Data;
	i = CH56xFileWrite( );											            /* д�ļ� */
#if MY_DEBUG_PRINTF
	printf( "CH563Write:%x \n", i );
#endif
    R32_PB_OUT &= ~(1<<0);  
	if( i != ERR_SUCCESS )	 return ( i );
	mCmdParam.Close.mUpdateLen = 1;
	i = CH56xFileClose( );											            /* �ر��ļ��������ļ����� */
#if MY_DEBUG_PRINTF
	printf( "CH563FileClose:%x \n" , i );
#endif
	return( i );
}

/****************  ������غ���  ****************/
UINT8 CH563NET_LibInit(const UINT8 *ip,const UINT8 *gwip,const UINT8 *mask,const UINT8 *macaddr)
{
    UINT8 i;
    struct _CH563_CFG cfg;
    if(CH563NET_GetVer() != CH563NET_LIB_VER)return 0xfc;             /* ��ȡ��İ汾�ţ�����Ƿ��ͷ�ļ�һ�� */
    CH563NETConfig = LIB_CFG_VALUE;									  /* ��������Ϣ���ݸ�������ñ��� */
    cfg.RxBufSize = RX_BUF_SIZE; 
    cfg.TCPMss   = CH563NET_TCP_MSS;
    cfg.HeapSize = CH563_MEM_HEAP_SIZE;
    cfg.ARPTableNum = CH563NET_NUM_ARP_TABLE;
    cfg.MiscConfig0 = CH563NET_MISC_CONFIG0;
	CH563NET_ConfigLIB(&cfg);
    i = CH563NET_Init(ip,gwip,mask,macaddr);
    
	return (i); 				     /* ���ʼ�� */
}

void CH563NET_HandleSockInt(UINT8 sockeid,UINT8 initstat)
{
    UINT32 len;
//    UINT32 totallen;
	UINT8 i;
    if(initstat & SINT_STAT_RECV)                                               /* �����ж� */
    {
		if(G_Flag&RecFlag)   
		{
			//error �ϴε��������ݻ�û��д�뵽U����
		}
		else
		{R32_PB_OUT |= (1<<0); 
			//printf("�����յ����� \r\n"); 
			len = CH563NET_SocketRecvLen(SocketId,NULL); 
			CH563NET_SocketRecv(SocketId,RecBuf,&len);
			RecLen = len;
			G_Flag |= RecFlag;
		}
    }
    if(initstat & SINT_STAT_CONNECT)                                            /* TCP�����ж� */  /* �������жϱ�ʾTCP�Ѿ����ӣ����Խ����շ����� */
    {
		G_Flag |= Socket_Connect;
    }
    if(initstat & SINT_STAT_DISCONNECT)                                         /* TCP�Ͽ��ж� */
    {                                                       					/* �������жϣ�CH563���ڲ��Ὣ��socket�������Ϊ�ر�*/
// 		i = CH563NET_SocketConnect(SocketId);                                        /* TCP���� */
		G_Flag &= ~Socket_Connect;
		if(NetStep==4)
		{
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP���� */
			mStopIfNetError(i); 
		}
    }
    if(initstat & SINT_STAT_TIM_OUT)                                            /* TCP��ʱ�ж� */   /* �������жϣ�CH563���ڲ��Ὣ��socket�������Ϊ�ر�*/
    {                             
		G_Flag &= ~Socket_Connect;
		if(NetStep==4)
		{
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP���� */
			mStopIfNetError(i); 
		}
// 		i = CH563NET_SocketConnect(SocketId);                                        /* TCP���� */
// 		mStopIfNetError(i);   
    }
}

void CH563NET_HandleGlobalInt(void)
{
	UINT8 initstat;
	UINT8 i;
	UINT8 socketinit;

	initstat = CH563NET_GetGlobalInt();                                          /* ��ȫ���ж�״̬����� */
	if(initstat & GINT_STAT_UNREACH)                                            /* ���ɴ��ж� */
	{
		
	}
	if(initstat & GINT_STAT_IP_CONFLI)                                           /* IP��ͻ�ж� */
	{

	}
	if(initstat & GINT_STAT_PHY_CHANGE)                                          /* PHY�ı��ж� */
	{
		i = CH563NET_GetPHYStatus();                                             /* ��ȡPHY״̬ */
		if(i&PHY_DISCONN)   //PHY�Ͽ�
		{
			G_Flag &= ~PHY_Connect;
		}
		else
		{
			G_Flag |= PHY_Connect;
		}
	}
	if(initstat & GINT_STAT_SOCKET)                                              /* Socket�ж� */
	{
		for(i = 0; i < CH563NET_MAX_SOCKET_NUM; i ++)                     
		{
			socketinit = CH563NET_GetSocketInt(i);                               /* ��socket�жϲ����� */
			if(socketinit)CH563NET_HandleSockInt(i,socketinit);                  /* ������ж������� */
		}    
	}
}

void CH563NET_CreatTcpSocket(void)
{
   UINT8 i;                                                             
   SOCK_INF TmpSocketInf;                                                       /* ������ʱsocket���� */

   memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
   memcpy((void *)TmpSocketInf.IPAddr,DESIP,4);                                 /* ����Ŀ��IP��ַ */
   TmpSocketInf.DesPort = PCDesPort;                                            /* ����Ŀ�Ķ˿� */
   TmpSocketInf.SourPort = CH563SourPort;                                       /* ����Դ�˿� */
   TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* ����socekt���� */
   TmpSocketInf.RecvStartPoint = (UINT32)SocketRecvBuf;                         /* ���ý��ջ������Ľ��ջ����� */
   TmpSocketInf.RecvBufLen = RECE_BUF_LEN ;                                     /* ���ý��ջ������Ľ��ճ��� */
   i = CH563NET_SocketCreat(&SocketId,&TmpSocketInf);                           /* ����socket�������ص�socket����������SocketId�� */
   mStopIfNetError(i);                                                             /* ������ */                                                    
}

/****************  �������̺���  ****************/
void USBProcess(void)
{
	UINT8  status,i=0;
	CH56xDiskConnect();
	if(CH56xDiskStatus < DISK_CONNECT)  //u���豸������
	{
		USBStep = 1;
		G_Flag &= ~UdiskConnect; //���±�־
	}
	
	if(USBStep==1)                        //�ȴ��豸����
	{
		if(CH56xDiskStatus < DISK_CONNECT)   //û��
		{
			
		}
		else                                   //�����̽���
		{
			G_Flag |= UdiskConnect;
			USBStep = 2;     
		}
		
	}
	if(USBStep==2)                            //�����̽��룬�˵�0��ʼ��
	{
		status = USBHOST_EHCI_EP0_Init( &UDisk.Device );                        /* USB�������ƶ˵�0��ʼ�� */
		if( status != USB_OPERATE_SUCCESS )  //ʧ��
		{
			printf("�˵�0��ʼ��error \r\n");
			USBStep = 0xff;
		}
		else
		{
			printf("�˵�0��ʼ����� \r\n");
			USBStep = 3;
		}
	}
	if(USBStep == 3)               //�ȴ�U��Ready
	{
		while(i<10)
		{
			Delay_ms( 50 );
			if( CH56xDiskReady( ) == ERR_SUCCESS ) /* ��ѯ�����Ƿ�׼���� */
			{
				USBStep = 4;	
				break;
			}
			if( CH56xDiskStatus < DISK_CONNECT )  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			{
				USBStep = 1;	
				break;
			}
			if( (CH56xDiskStatus >= DISK_MOUNTED) && (i > 5) )   /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
			{
				USBStep = 4;	
				break;														
			}
			i++;
		}
		if( CH56xDiskStatus < DISK_CONNECT ) 
        {  
            /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
            printf( "Device gone\n" );
			G_Flag &= ~UdiskConnect;
            USBStep = 1;                                                          /* ���µȴ� */
        }
        if( CH56xDiskStatus < DISK_MOUNTED ) 
        {  
            /* δ֪USB�豸,����USB���̡���ӡ���� */
            printf( "Unknown device\n" );
			G_Flag &= ~UdiskConnect;
            USBStep = 0xff;
        }
	}
	if(USBStep==4)    //U��׼�����ˣ���ʾ�����Ϣ
	{
		printf( "TotalSize = %lu MB \n", mCmdParam.DiskReady.mDiskSizeSec >> ( 20 - CH56xvSectorSizeB ) );  /* ��ʾΪ��MBΪ��λ������ */
        printf( "Current disk sector size = %d Bytes \n", CH56xvSectorSize );   /* CH56xvSectorSize��U�̵�ʵ��������С */
#if DISK_BASE_BUF_LEN
        if( DISK_BASE_BUF_LEN < CH56xvSectorSize ) 
        {  
            /* ���������ݻ������Ƿ��㹻��,CH56xvSectorSize��U�̵�ʵ��������С */
            printf( "Too large sector size\n" );
            USBStep = 0xff;
			//goto UnknownUsbDevice;
        }
#endif
		USBStep = 5;
	}
	if(USBStep==5)  //�ж��Ƿ��Ƴ�
	{
		
	}
	if(USBStep==0xff)   //δ֪�豸
	{
		printf("UnknownUsbDevice\r\n");
		G_Flag &= ~UdiskConnect;
        /* ���³�ʼ��U����ز�����������Դ */
        USBHOST_DevicePara_Init( &UDisk.Device );                               /* USB������ز�����ʼ�� */
        USBHOST_Structure_DeInit( );                                            /* USB�������ݽṹ��ʼ�� */
		USBStep = 1;
	}
}

void NetProcess(void)
{
	UINT8 i = 0;
	CH563NET_MainTask(); 
	if(CH563NET_QueryGlobalInt()) CH563NET_HandleGlobalInt();         
	if(NetStep==1)   //�ȴ�PHY����
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
	if(NetStep==2)  //����Socket
	{
		CH563NET_CreatTcpSocket();
		printf("Socket0 OK  \r\n");
		NetStep = 3;
	}
	if(NetStep==3)
	{
		if(G_Flag&UdiskConnect)             //�ȴ�U��׼���ã�������Socket
		{
			printf("U�����ӣ�Socket��ʼ���� \r\n");
			i = CH563NET_SocketConnect(SocketId);                                        /* TCP���� */
			mStopIfNetError(i);                                                             /* ������ */  
			NetStep = 4;
		}
	}
	if(NetStep==4)  //�ȴ����ӳɹ�
	{
		if(G_Flag&Socket_Connect) 
		{
			printf("Socket connect \r\n");
			NetStep = 5;
		}
	}
	if(NetStep==5)      //�ж��������ӣ������ݡ�
	{
		if((G_Flag&Socket_Connect)==0)  //Socket�Ͽ�
		{
			NetStep = 4;
		}
		if((G_Flag&PHY_Connect)==0)     //PHY�Ͽ�����
		{
			NetStep = 1;
		}
		if((G_Flag&UdiskConnect)==0)    //U�̲�����
		{
			printf("u���Ƴ����ر����� \r\n");
			i = CH563NET_SocketClose(SocketId,TCP_CLOSE_NORMAL);  //�رյ�ǰ��Socket
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
		printf("д���ļ���� \r\n");
	}
}

/*******************************************************************************
* Function Name  : main
* Description    : ������
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

//    SysFreq( );                                                               /* ������Ҫ����PLL�����Ӧ��ʱ��Ƶ�� */
    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* ������LED��һ����ʾ���� */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    mInitSTDIO( );                                                              /* Ϊ���ü����ͨ�����ڼ����ʾ���� */

    printf( "start \xd\xa" );

#if DISK_BASE_BUF_LEN == 0
    pDISK_BASE_BUF = &my_buffer[0];                                             /* ����.H�ļ��ж���CH563��ר�û�����,�����û�����ָ��ָ������Ӧ�ó���Ļ��������ں����Խ�ԼRAM */
#endif

    /* USB������س�ʼ�� */
    USBHOST_ModeSet( );                                                         /* ����USB����ģʽ */
    USBHOST_EHCI_Init( );                                                       /* USB����EHCI��λ����ʼ�� */
    USBHOST_Structure_DeInit( );                                                /* USB�������ݽṹ��ʼ�� */
    USBHOST_DevicePara_Init( &UDisk.Device );                                   /* USB������ز�����ʼ�� */

    i = CH56xLibInit( );                                                        /* ��ʼ��CH563������CH563оƬ,�����ɹ�����0 */
    mStopIfError( i );
    /* ������·��ʼ�� */
	i = CH563NET_LibInit(IPAddr,GWIPAddr,IPMask,MACAddr);                       /* ���ʼ�� */
	mStopIfNetError(i);                                                            /* ������ */

    SysTimeInit();                                                              /* ϵͳ��ʱ����ʼ�� */
    InitSysHal();                                                               /* ��ʼ���ж� */
	
    while ( 1 ) 
    {
        USBProcess();
		NetProcess();
		WriteFile();
    }
}


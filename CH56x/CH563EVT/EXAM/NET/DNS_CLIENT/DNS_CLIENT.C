/********************************** (C) COPYRIGHT *******************************
* File Name          : DNS_CLIENT.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH563 DNS DEMO
*                      (1)��CH563 TCP/IP Э����ӿ�;
*                      (2)������0��������Ϣ,115200bps;
*                      (3)��CH563NET����ʾ�ļ���������������ʾDNS������������,ARM966,Thumb,С��
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ�����*/
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "DNS.H"
#include "CH563NET.H"
#include "ISPXT56X.H"

#define CH563NET_DBG                          1

/* ����Ļ�������ȫ�ֱ�������Ҫ���壬���е��� */
__align(16)UINT8    CH563MACRxDesBuf[(RX_QUEUE_ENTRIES )*16];                   /* MAC������������������16�ֽڶ��� */
__align(4) UINT8    CH563MACRxBuf[RX_QUEUE_ENTRIES*RX_BUF_SIZE];                /* MAC���ջ�������4�ֽڶ��� */
__align(4) SOCK_INF SocketInf[CH563NET_MAX_SOCKET_NUM];                         /* Socket��Ϣ����4�ֽڶ��� */
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

/******************************************************************************/
/* ����ʾ�������غ� */
#define RECE_BUF_LEN                          536                               /* ���ջ������Ĵ�С */
                                                                   
/* CH563��ض��� */
UINT8 MACAddr[6] = {0x0e,0x0d,0x04,0x05,0x06,0x27};                             /* CH563MAC��ַ */
UINT8 IPAddr[4] =  {192,168,1,10};                                              /* CH563IP��ַ */
UINT8 GWIPAddr[4] ={192,168,1,1};                                               /* CH563���� */
UINT8 IPMask[4] =  {255,255,255,0};                                             /* CH563�������� */
UINT8 SocketId;                                                                 /* ����socket���������Բ��ö��� */
UINT8 SocketRecvBuf[RECE_BUF_LEN];                                              /* socket���ջ����� */
UINT8 MyBuf[RECE_BUF_LEN];                                                      /* ����һ����ʱ������ */

extern UINT8 dns_buf[512];

UINT8 status;
#define   MAX_URL_SIZE 128

UINT8  url_dn1[MAX_URL_SIZE] = "www.baidu.com";        
UINT8  url_dn2[MAX_URL_SIZE] = "www.google.com";        
UINT8  url_dn3[MAX_URL_SIZE] = "www.wch.cn";        

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϷ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
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
    while(1);
}

/*******************************************************************************
* Function Name  : SysTimeInit
* Description    : ϵͳ��ʱ����ʼ����CH563@100MHZ TIME0 10ms������CH563NETTIMEPERIOD
*                ������ʼ����ʱ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTimeInit(void)
{
    R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
    R32_TMR0_COUNT = 0x00000000; 
    R32_TMR0_CNT_END = 0x186a0 * CH563NETTIMEPERIOD;                            /* ����Ϊ10MS��ʱ */
    R8_TMR0_INTER_EN |= RB_TMR_IE_CYC_END;
    R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
}

/*******************************************************************************
* Function Name  : InitSysHal
* Description    : Ӳ����ʼ������������TIM0��ETH�ж�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitSysHal(void)
{
    R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR0;                                          /* ����TIM0�ж� */
    R8_INT_EN_IRQ_1 |= RB_IE_IRQ_ETH;                                           /* ����ETH�ж� */
    R8_INT_EN_IRQ_GLOB |= RB_IE_IRQ_GLOB;                                       /* ����IRQȫ���ж� */
}

/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CH563NET_ERR_SUCCESS) return;                                 /* �����ɹ� */
#if CH563NET_DBG
    printf("Error: %02X\n", (UINT16)iError);                                    /* ��ʾ���� */
#endif    
}

/*******************************************************************************
* Function Name  : CH563NET_LibInit
* Description    : ���ʼ������
* Input          : ip      ip��ַָ��
*                ��gwip    ����ip��ַָ��
*                 : mask    ����ָ��
*                 : macaddr MAC��ַָ�� 
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 CH563NET_LibInit(const UINT8 *ip,const UINT8 *gwip,const UINT8 *mask,const UINT8 *macaddr)
{
    UINT8 i;
    struct _CH563_CFG cfg;
    if(CH563NET_GetVer() != CH563NET_LIB_VER)return 0xfc;                       /* ��ȡ��İ汾�ţ�����Ƿ��ͷ�ļ�һ�� */
    CH563NETConfig = LIB_CFG_VALUE;                                             /* ��������Ϣ���ݸ�������ñ��� */
    cfg.RxBufSize = RX_BUF_SIZE; 
    cfg.TCPMss   = CH563NET_TCP_MSS;
    cfg.HeapSize = CH563_MEM_HEAP_SIZE;
    cfg.ARPTableNum = CH563NET_NUM_ARP_TABLE;
    cfg.MiscConfig0 = CH563NET_MISC_CONFIG0;
    CH563NET_ConfigLIB(&cfg);
    i = CH563NET_Init(ip,gwip,mask,macaddr);
    return (i);                      /* ���ʼ�� */
}

/*******************************************************************************
* Function Name  : CH563NET_HandleSockInt
* Description    : Socket�жϴ�������
* Input          : sockeid  socket����
*                ��initstat �ж�״̬
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_HandleSockInt(UINT8 sockeid,UINT8 initstat)
{
    UINT32 len;
    UINT32 i;
    if(initstat & SINT_STAT_RECV)                                               /* �����ж� */
    {
        len = CH563NET_SocketRecvLen(sockeid,NULL);                             /* �Ὣ��ǰ����ָ�봫�ݸ�precv*/
#if CH563NET_DBG
        printf("Receive Len = %02x\n",len);                           
#endif
            if (len > 0)
            {    
                 if(sockeid==0 & status ==3)
                {
                CH563NET_SocketRecv(sockeid,dns_buf,&len);
                status = 4;
                i = CH563NET_SocketClose( sockeid,TCP_CLOSE_NORMAL );
                mStopIfError(i);
#if CH563NET_DBG
                printf("status = 4!\n");
#endif
                }
                else
                  CH563NET_SocketRecv(sockeid,MyBuf,&len);                      /* �����ջ����������ݶ���MyBuf��*/
            }

    }
    if(initstat & SINT_STAT_CONNECT)                                            /* TCP�����ж� */
    {
    }
    if(initstat & SINT_STAT_DISCONNECT)                                         /* TCP�Ͽ��ж� */
    {
    }
    if(initstat & SINT_STAT_TIM_OUT)                                            /* TCP��ʱ�ж� */
    {
    }

}

/*******************************************************************************
* Function Name  : CH563NET_HandleGloableInt
* Description    : ȫ���жϴ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_HandleGlobalInt(void)
{
    UINT8 initstat;
    UINT8 i;
    UINT8 socketinit;
    
    initstat = CH563NET_GetGlobalInt();                                         /* ��ȫ���ж�״̬����� */
    if(initstat & GINT_STAT_UNREACH)                                            /* ���ɴ��ж� */
    {
#if CH563NET_DBG
        printf("UnreachCode ��%d\n",CH563Inf.UnreachCode);                      /* �鿴���ɴ���� */
        printf("UnreachProto ��%d\n",CH563Inf.UnreachProto);                    /* �鿴���ɴ�Э������ */
        printf("UnreachPort ��%d\n",CH563Inf.UnreachPort);                      /* ��ѯ���ɴ�˿� */
#endif       
    }
   if(initstat & GINT_STAT_IP_CONFLI)                                           /* IP��ͻ�ж� */
   {
   
   }
   if(initstat & GINT_STAT_PHY_CHANGE)                                          /* PHY�ı��ж� */
   {
       i = CH563NET_GetPHYStatus();                                             /* ��ȡPHY״̬ */
       if(i==0x08) status = 1;
#if CH563NET_DBG
       printf("status = 1!\n");
       printf("GINT_STAT_PHY_CHANGE %02x\n",i);
#endif   
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

/*******************************************************************************
* Function Name  : UDPSocketParamInit
* Description    : ����UDP socket
* Input          : socket����ֵ��Դ�˿ڣ�Ŀ�Ķ˿ڼ�Ŀ��IP
* Output         : None
* Return         : None
*******************************************************************************/
void UDPSocketParamInit( UINT8 sockindex,UINT8 *addr,UINT16 SourPort,UINT16 DesPort )
{
   UINT8 i;                                                             
   SOCK_INF TmpSocketInf;                                                       /* ������ʱsocket���� */

   memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
   memcpy((void *)TmpSocketInf.IPAddr,addr,4);                                  /* ����Ŀ��IP��ַ */
 
   TmpSocketInf.DesPort = DesPort;                                              /* ����Ŀ�Ķ˿� */
   TmpSocketInf.SourPort = SourPort;                                            /* ����Դ�˿� */
   TmpSocketInf.ProtoType = PROTO_TYPE_UDP;                                     /* ����socekt���� */
   TmpSocketInf.RecvStartPoint = (UINT32)SocketRecvBuf;                         /* ���ý��ջ������Ľ��ջ����� */
   TmpSocketInf.RecvBufLen = RECE_BUF_LEN ;                                     /* ���ý��ջ������Ľ��ճ��� */
   i = CH563NET_SocketCreat(&SocketId,&TmpSocketInf);                           /* ����socket�������ص�socket����������SocketId�� */
   mStopIfError(i);                                                             /* ������ */
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : Ϊprintf��getkey���������ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / 115200;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* �������� */
    R8_UART0_LCR = 0x80;                                                        /* DLABλ��1 */
    R8_UART0_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD1 schmitt input, TXD1 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD1 output enable */
}

/*******************************************************************************
* Function Name  : fputc
* Description    : ͨ��������������Ϣ
* Input          : c-- writes the character specified by c 
*                  *f--the output stream pointed to by *f
* Output         : None
* Return         : None
*******************************************************************************/

int fputc( int c, FILE *f )
{
    R8_UART0_THR = c;                                                           /* �������� */
    while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* �ȴ����ݷ��� */
    return( c );
}

/*******************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main( void ) 
{
    UINT8 ip[4],i = 0;
    UINT8 dns_flag = 0;

    mInitSTDIO();                                                               /* Ϊ���ü����ͨ�����ڼ����ʾ���� */
    i = CH56X_GetMac(MACAddr);
    i = CH563NET_LibInit(IPAddr,GWIPAddr,IPMask,MACAddr);                       /* ���ʼ�� */
    mStopIfError(i);                                                            /* ������ */
#if CH563NET_DBG
    printf("CH563IPLibInit Success\n");
#endif    
    SysTimeInit();                                                              /* ϵͳ��ʱ����ʼ�� */
    InitSysHal();                                                               /* ��ʼ���ж� */

    while(1)
    {
        CH563NET_MainTask();                                                    /* CH563NET��������������Ҫ����ѭ���в��ϵ��� */
        if(CH563NET_QueryGlobalInt())CH563NET_HandleGlobalInt();                /* ��ѯ�жϣ�������жϣ������ȫ���жϴ������� */
        if( dns_flag == 0 ){
            i = DnsQuery(0,url_dn3,ip);                                         /* ��ѯ */
             if(i){ 
                dns_flag = 1;
#if CH563NET_DBG
                if( i == 1 ){
                    printf("Domain name: %s \n",url_dn3);
                    printf(" HTTPs_IP= %d.%d.%d.%d\n\n",ip[0],ip[1],ip[2],ip[3]);
                }
#endif    
            }
        }     
    }
}

/******************************** endfile *************************************/
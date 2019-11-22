/********************************** (C) COPYRIGHT *********************************
* File Name          : CH563NET.H
* Author             : tech9
* Version            : V1.0
* Date               : 2015/01/15
* Description        : C Define for CH563NET.LIB
*                      Website: http://wch.cn
*                      Email:   tech@wch.cn         
**********************************************************************************/
/* ������־ */
/* �⹦������ */
/* Thumbָ���KEIL 3.63 Armcc.exe V3.1.0.939 */
/*$2013/9/18$2013/11/28$2014/01/15$ */
#ifndef __CH563NET_H__
#define __CH563NET_H__

/* ��汾��Ϣ */
#define CH563NET_LIB_VER                      0X05                   /* ��汾��Ϣ */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef     TRUE
#define     TRUE    1
#define     FALSE   0
#endif
#ifndef     NULL
#define     NULL    0
#endif


/* Socket�ĸ��� */
#ifndef CH563NET_MAX_SOCKET_NUM
#define CH563NET_MAX_SOCKET_NUM               4                                 /* Socket�ĸ������û��������ã�Ĭ��Ϊ4��Socket,���Ϊ32 */
#endif

#ifndef RX_QUEUE_ENTRIES
#define RX_QUEUE_ENTRIES                      7                                 /* CH563 MAC�������������У���ֵ����С��2,������Ĭ��Ϊ7�����Ϊ32 */
#endif                                                                          /* #ifndef RX_QUEUE_ENTRIES */
                                                                                
#ifndef RX_BUF_SIZE
#define RX_BUF_SIZE                           0x5ec                             /* CH563 MAC����ÿ�����������ȣ�Ϊ4�������� */
#endif

#ifndef CH563NET_PING_ENABLE                                                    
#define CH563NET_PING_ENABLE                  TRUE                              /* Ĭ��PING���� */
#endif                                                                          /* PINGʹ�� */   
                                                                                
#ifndef TCP_RETRY_COUNT                                                         
#define TCP_RETRY_COUNT                       20                                /* TCP���Դ�����λ��Ϊ5λ*/
#endif                                                                          
                                                                                
#ifndef TCP_RETRY_PERIOD                                                        
#define TCP_RETRY_PERIOD                      10                                /* TCP�������ڣ���λΪ50MS��λ��Ϊ5λ */
#endif                                                                          
                                                                                
#ifndef CH563NETTIMEPERIOD                                                      
#define CH563NETTIMEPERIOD                    10                                /* ��ʱ�����ڣ���λMs,���ô���500 */
#endif                                                                          
                                                                                
#define MAC_INT_TYPE_IRQ                      0                                 /* ��̫��ʹ��irq�ж� */
#define MAC_INT_TYPE_FIQ                      1                                 /* ��̫��ʹ��frq�ж� */
                                                                                
#ifndef MAC_INT_TYPE                                                            
#define MAC_INT_TYPE                          MAC_INT_TYPE_IRQ                  /* Ĭ����̫��ʹ��IRQ�ж� */
#endif                                                                          
                                                                                
#ifndef SOCKET_SEND_RETRY                                                       
#define SOCKET_SEND_RETRY                     1                                 /* Ĭ�Ϸ������� */
#endif                    

#define TX_QUEUE_ENTRIES                      2                                 /* CH563 MAC��������������Ϊ2 */
#define LIB_CFG_VALUE                         ((SOCKET_SEND_RETRY << 25) |\
                                              (MAC_INT_TYPE << 24) |\
                                              (TCP_RETRY_PERIOD << 19) |\
                                              (TCP_RETRY_COUNT << 14) |\
                                              (CH563NET_PING_ENABLE << 13) |\
                                              (TX_QUEUE_ENTRIES << 9) |\
                                              (RX_QUEUE_ENTRIES << 5) |\
                                              (CH563NET_MAX_SOCKET_NUM))
#ifndef MISCE_CFG0_TCP_SEND_COPY 
#define MISCE_CFG0_TCP_SEND_COPY              1                                 /* TCP���ͻ��������� */
#endif                                                                          

#ifndef MISCE_CFG0_TCP_RECV_COPY                                                
#define MISCE_CFG0_TCP_RECV_COPY              1                                 /* TCP���ո����Ż����ڲ�����ʹ�� */
#endif                                                                          

#ifndef MISCE_CFG0_TCP_OLD_DELETE                                               
#define MISCE_CFG0_TCP_OLD_DELETE             0                                 /* ɾ�������TCP���� */
#endif

/*�����ڴ���� */
#ifndef CH563NET_NUM_IPRAW
#define CH563NET_NUM_IPRAW                    4                                 /* IPRAW���ӵĸ��� */ 
#endif

#ifndef CH563NET_NUM_UDP
#define CH563NET_NUM_UDP                      4                                 /* UDP���ӵĸ��� */                                   
#endif

#ifndef CH563NET_NUM_TCP
#define CH563NET_NUM_TCP                      4                                 /* TCP���ӵĸ��� */ 
#endif

#ifndef CH563NET_NUM_TCP_LISTEN
#define CH563NET_NUM_TCP_LISTEN               4                                 /* TCP�����ĸ��� */
#endif

#ifndef CH563NET_NUM_PBUF
#define CH563NET_NUM_PBUF                     5                                 /* PBUF�ṹ�ĸ��� */
#endif

#ifndef CH563NET_NUM_POOL_BUF
#define CH563NET_NUM_POOL_BUF                 16                                /* POOL BUF�ĸ��� */
#endif

#ifndef CH563NET_NUM_TCP_SEG
#define CH563NET_NUM_TCP_SEG                  16                                /* tcp�εĸ���*/
#endif

#ifndef CH563NET_NUM_IP_REASSDATA
#define CH563NET_NUM_IP_REASSDATA             5                                 /* IP�ֶεĳ��� */
#endif

#ifndef CH563NET_TCP_MSS
#define CH563NET_TCP_MSS                      1460                              /* tcp MSS�Ĵ�С*/
#endif

#ifndef CH563_MEM_HEAP_SIZE
#define CH563_MEM_HEAP_SIZE                   4600                              /* �ڴ�Ѵ�С */
#endif

#ifndef CH563NET_NUM_ARP_TABLE
#define CH563NET_NUM_ARP_TABLE                16                                /* ARP�б����� */
#endif

#ifndef CH563NET_MEM_ALIGNMENT
#define CH563NET_MEM_ALIGNMENT                4                                 /* 4�ֽڶ��� */
#endif

#ifndef CH563NET_IP_REASS_PBUFS             
#if (CH563NET_NUM_POOL_BUF < 32)
#define CH563NET_IP_REASS_PBUFS               (CH563NET_NUM_POOL_BUF - 1)       /* IP��Ƭ��PBUF���������Ϊ31 */
#else
#define CH563NET_IP_REASS_PBUFS               31       
#endif
#endif

#define CH563NET_MISC_CONFIG0                 ((MISCE_CFG0_TCP_SEND_COPY << 0) |\
                                               (MISCE_CFG0_TCP_RECV_COPY << 1) |\
                                               (MISCE_CFG0_TCP_OLD_DELETE << 2)|\
                                               (CH563NET_IP_REASS_PBUFS)<<3)
/* PHY ״̬ */
#define PHY_DISCONN                           (1<<0)                            /* PHY�Ͽ� */
#define PHY_10M_FLL                           (1<<1)                            /* 10Mȫ˫�� */
#define PHY_10M_HALF                          (1<<2)                            /* 10M��˫�� */
#define PHY_100M_FLL                          (1<<3)                            /* 100Mȫ˫�� */
#define PHY_100M_HALF                         (1<<4)                            /* 100M��˫�� */
#define PHY_AUTO                              (1<<5)                            /* PHY�Զ�ģʽ��CMD10H_SET_PHY */
                                                                                
/* Socket ����ģʽ����,Э������ */                                                       
#define PROTO_TYPE_IP_RAW                     0                                 /* IP��ԭʼ���� */
#define PROTO_TYPE_UDP                        2                                 /* UDPЭ������ */
#define PROTO_TYPE_TCP                        3                                 /* TCPЭ������ */
                                                                                
/* �ж�״̬ */                                                                  
/* ����ΪGLOB_INT�������״̬ */                                                
#define GINT_STAT_UNREACH                     (1<<0)                            /* ���ɴ��ж�*/
#define GINT_STAT_IP_CONFLI                   (1<<1)                            /* IP��ͻ*/
#define GINT_STAT_PHY_CHANGE                  (1<<2)                            /* PHY״̬�ı� */
#define GINT_STAT_SOCKET                      (1<<4)                            /* scoket �����ж� */
                                                                                
/*����ΪSn_INT�������״̬*/                                                    
#define SINT_STAT_RECV                        (1<<2)                            /* socket�˿ڽ��յ����ݻ��߽��ջ�������Ϊ�� */
#define SINT_STAT_CONNECT                     (1<<3)                            /* ���ӳɹ�,TCPģʽ�²������ж� */
#define SINT_STAT_DISCONNECT                  (1<<4)                            /* ���ӶϿ�,TCPģʽ�²������ж� */
#define SINT_STAT_TIM_OUT                     (1<<6)                            /* ARP��TCPģʽ�»ᷢ�����ж� */

/* ������ */
#define CH563NET_ERR_SUCCESS                  0x00                              /* ��������ɹ� */
#define CH563NET_RET_ABORT                    0x5F                              /* �������ʧ�� */
#define CH563NET_ERR_BUSY                     0x10                              /* æ״̬����ʾ��ǰ����ִ������ */
#define CH563NET_ERR_MEM                      0x11                              /* �ڴ���� */
#define CH563NET_ERR_BUF                      0x12                              /* ���������� */
#define CH563NET_ERR_TIMEOUT                  0x13                              /* ��ʱ */
#define CH563NET_ERR_RTE                      0x14                              /* ·�ɴ���*/
#define CH563NET_ERR_ABRT                     0x15                              /* ����ֹͣ*/
#define CH563NET_ERR_RST                      0x16                              /* ���Ӹ�λ */
#define CH563NET_ERR_CLSD                     0x17                              /* ���ӹر�/socket �ڹر�״̬*/
#define CH563NET_ERR_CONN                     0x18                              /* ������ */
#define CH563NET_ERR_VAL                      0x19                              /* �����ֵ */
#define CH563NET_ERR_ARG                      0x1a                              /* ����Ĳ��� */
#define CH563NET_ERR_USE                      0x1b                              /* �Ѿ���ʹ�� */
#define CH563NET_ERR_IF                       0x1c                              /* MAC����  */
#define CH563NET_ERR_ISCONN                   0x1d                              /* ������ */
#define CH563NET_ERR_SOCKET_MEM               0X20                              /* Socket��Ϣ�б��������ߴ��� */
#define CH563NET_ERR_UNSUPPORT_PROTO          0X21                              /* ��֧�ֵ�Э������ */
#define CH563NET_ERR_UNKNOW                   0xFA                              /* δ֪���� */

/* ���ɴ���� */
#define UNREACH_CODE_HOST                     0                                 /* �������ɴ� */
#define UNREACH_CODE_NET                      1                                 /* ���粻�ɴ� */
#define UNREACH_CODE_PROTOCOL                 2                                 /* Э�鲻�ɴ� */
#define UNREACH_CODE_PROT                     3                                 /* �˿ڲ��ɴ� */
/*����ֵ��ο�RFC792�ĵ�*/                                                      

/* TCP�رղ��� */
#define TCP_CLOSE_NORMAL                      0                                 /* �����رգ�����4������ */
#define TCP_CLOSE_RST                         1                                 /* ��λ���ӣ����ر�  */
#define TCP_CLOSE_ABANDON                     2                                 /* CH563NET�ڲ��������ӣ����ᷢ���κ���ֹ���� */

/* socket״̬ */
#define  SOCK_STAT_CLOSED                     0X00                              /* socket�ر� */
#define  SOCK_STAT_OPEN                       0X05                              /* socket�� */

/* TCP״̬ */
#define TCP_CLOSED                            0                                 /* TCP���� */
#define TCP_LISTEN                            1                                 /* TCP�ر� */
#define TCP_SYN_SENT                          2                                 /* SYN���ͣ��������� */
#define TCP_SYN_RCVD                          3                                 /* SYN���գ����յ��������� */
#define TCP_ESTABLISHED                       4                                 /* TCP���ӽ��� */
#define TCP_FIN_WAIT_1                        5                                 /* WAIT_1״̬ */
#define TCP_FIN_WAIT_2                        6                                 /* WAIT_2״̬ */
#define TCP_CLOSE_WAIT                        7                                 /* �ȴ��ر� */
#define TCP_CLOSING                           8                                 /* ���ڹر� */
#define TCP_LAST_ACK                          9                                 /* LAST_ACK*/
#define TCP_TIME_WAIT                         10                                /* 2MSL�ȴ� */

/* sokcet��Ϣ�� */
typedef struct _SCOK_INF
{
    UINT32 IntStatus;                                                           /* �ж�״̬ */
    UINT32 SockIndex;                                                           /* Socket����ֵ */
    UINT32 RecvStartPoint;                                                      /* ���ջ������Ŀ�ʼָ�� */
    UINT32 RecvBufLen;                                                          /* ���ջ��������� */
    UINT32 RecvCurPoint;                                                        /* ���ջ������ĵ�ǰָ�� */
    UINT32 RecvReadPoint;                                                       /* ���ջ������Ķ�ָ�� */
    UINT32 RecvRemLen;                                                          /* ���ջ�������ʣ�೤�� */
    UINT32 ProtoType;                                                           /* Э������ */
    UINT32 ScokStatus;                                                          /* ���ֽ�Socket״̬���ε��ֽ�ΪTCP״̬����TCPģʽ�������� */
    UINT32 DesPort;                                                             /* Ŀ�Ķ˿� */
    UINT32 SourPort;                                                            /* Դ�˿���IPRAWģʽ��ΪЭ������ */
    UINT8  IPAddr[4];                                                           /* SocketĿ��IP��ַ 32bit*/
    void *Resv1;                                                                /* �������ڲ�ʹ�ã����ڱ������PCB */
    void *Resv2;                                                                /* �������ڲ�ʹ�ã�TCP Serverʹ�� */
 // void (*RecvCallBack)(struct _SCOK_INF *socinf,UINT32 ipaddr,UINT16 port,UINT8 *buf,UINT32 len); /* ���ջص�����*/
    void (*AppCallBack)(struct _SCOK_INF *,UINT32 ,UINT16 ,UINT8 *,UINT32 ); /* ���ջص�����*/

}SOCK_INF;

/* CH563ȫ����Ϣ */
struct _CH563_SYS
{
    UINT8  IPAddr[4];                                                           /* CH563IP��ַ 32bit */
    UINT8  GWIPAddr[4];                                                         /* CH563���ص�ַ 32bit */
    UINT8  MASKAddr[4];                                                         /* CH563�������� 32bit */
    UINT8  MacAddr[8];                                                          /* CH563MAC��ַ 48bit */
    UINT8  UnreachIPAddr[4];                                                    /* ���ɵ���IP */
    UINT32 RetranCount;                                                         /* ���Դ��� Ĭ��Ϊ10�� */
    UINT32 RetranPeriod;                                                        /* ��������,��λMS,Ĭ��200MS */
    UINT32 PHYStat;                                                             /* CH563PHY״̬�� 8bit */
    UINT32 CH563Stat;                                                           /* CH563��״̬ �������Ƿ�򿪵� */ 
    UINT32 MackFilt;                                                            /* CH563 MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit */
    UINT32 GlobIntStatus;                                                       /* ȫ���ж� */
    UINT32 UnreachCode;                                                         /* ���ɴ� */
    UINT32 UnreachProto;                                                        /* ���ɴ�Э�� */
    UINT32 UnreachPort;                                                         /* ���ɵ���˿� */
    UINT32 SendFlag;
};
/* �ڴ��Լ��������� */
struct _CH563_CFG
{
    UINT32 RxBufSize;                                                            /* MAC���ջ�������С */
    UINT32 TCPMss;                                                               /* TCP MSS��С */
    UINT32 HeapSize;                                                             /* �ѷ����ڴ��С */
    UINT32 ARPTableNum;                                                          /* ARP�б����� */
    UINT32 MiscConfig0;                                                          /* ������������ */
};

/* ���ڲ��������� */
extern UINT8  CH563IPIntStatus;                                                 /* �ж�״̬ */
extern struct _CH563_SYS   CH563Inf;                                            /* ϵͳ��Ϣ�б� */
extern UINT32 CH563NETConfig;                                                   /* �����ã�����˵�� */
/* λ0-4 Socket�ĸ���,���ֵΪ31 */
/* λ5-8 MAC �����������ĸ��������ֵΪ15 */
/* λ13 PINGʹ�ܣ�1Ϊ����PING��0Ϊ�ر�PING��Ĭ��Ϊ���� */
/* λ14-18 TCP���Դ���*/
/* λ19-23 TCP�������ڣ���λΪ50����*/
/* λ24 ��̫���ж����� */
/* λ25 ������������ */

/* ����ֵΪ�̶�ֵ�����Ը��� */
#define CH563NET_MEM_ALIGN_SIZE(size)         (((size) + CH563NET_MEM_ALIGNMENT - 1) & ~(CH563NET_MEM_ALIGNMENT-1))
#define CH563NET_SIZE_IPRAW_PCB               0x1C                              /* IPRAW PCB��С */
#define CH563NET_SIZE_UDP_PCB                 0x20                              /* UDP PCB��С */
#define CH563NET_SIZE_TCP_PCB                 0xA4                              /* TCP PCB��С */
#define CH563NET_SIZE_TCP_PCB_LISTEN          0x20                              /* TCP LISTEN PCB��С */
#define CH563NET_SIZE_IP_REASSDATA            0x20                              /* IP��Ƭ����  */
#define CH563NET_SIZE_PBUF                    0x10                              /* Packet Buf */
#define CH563NET_SIZE_TCP_SEG                 0x14                              /* TCP SEG�ṹ */
#define CH563NET_SIZE_MEM                     0x06                              /* sizeof(struct mem) */
#define CH563NET_SIZE_ARP_TABLE               0x10                              /* sizeof arp table */

#define CH563NET_SIZE_POOL_BUF                CH563NET_MEM_ALIGN_SIZE(CH563NET_TCP_MSS + 40 +14) /* pbuf�ش�С */
#define CH563NET_MEMP_SIZE                    ((CH563NET_MEM_ALIGNMENT - 1) + \
                                              (CH563NET_NUM_IPRAW * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_IPRAW_PCB)) + \
                                              (CH563NET_NUM_UDP * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_UDP_PCB)) + \
                                              (CH563NET_NUM_TCP * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_PCB)) + \
                                              (CH563NET_NUM_TCP_LISTEN * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_PCB_LISTEN)) + \
                                              (CH563NET_NUM_TCP_SEG * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_TCP_SEG)) + \
                                              (CH563NET_NUM_IP_REASSDATA * CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_IP_REASSDATA)) + \
                                              (CH563NET_NUM_PBUF * (CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_PBUF) + CH563NET_MEM_ALIGN_SIZE(0))) + \
                                              (CH563NET_NUM_POOL_BUF * (CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_PBUF) + CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_POOL_BUF))))                    


#define  HEAP_MEM_ALIGN_SIZE                  (CH563NET_MEM_ALIGN_SIZE(CH563NET_SIZE_MEM))
#define  CH563NET_RAM_HEAP_SIZE               (CH563_MEM_HEAP_SIZE + (2 * HEAP_MEM_ALIGN_SIZE) + CH563NET_MEM_ALIGNMENT)
#define  CH563NET_RAM_ARP_TABLE_SIZE          (CH563NET_SIZE_ARP_TABLE * CH563NET_NUM_ARP_TABLE)
/* DNS�ṹ��ص� */
typedef void (*dns_callback)(const char *name, UINT8 *ipaddr, void *callback_arg);

/* ������� */
/* ���CH563NET_NUM_IPRAW�Ƿ�С��1 */
#if(CH563NET_NUM_IPRAW < 1)
#error "CH563NET_NUM_IPRAW Error,Please Config CH563NET_NUM_IPRAW >= 1"
#endif
/* ���CH563NET_SIZE_UDP_PCB�Ƿ�С��1 */
#if(CH563NET_SIZE_UDP_PCB < 1)
#error "CH563NET_SIZE_UDP_PCB Error,Please Config CH563NET_SIZE_UDP_PCB >= 1"
#endif
/* ���CH563NET_NUM_TCP�Ƿ�С��1 */
#if(CH563NET_NUM_TCP < 1)
#error "CH563NET_NUM_TCP Error,Please Config CH563NET_NUM_TCP >= 1"
#endif
/* ���CH563NET_NUM_TCP_LISTEN�Ƿ�С��1 */
#if(CH563NET_NUM_TCP_LISTEN < 1)
#error "CH563NET_NUM_TCP_LISTEN Error,Please Config CH563NET_NUM_TCP_LISTEN >= 1"
#endif
/* ����ֽڶ������Ϊ4�������� */
#if((CH563NET_MEM_ALIGNMENT % 4) || (CH563NET_MEM_ALIGNMENT == 0))
#error "CH563NET_MEM_ALIGNMENT Error,Please Config CH563NET_MEM_ALIGNMENT = 4 * N, N >=1"
#endif
/* TCP����Ķγ��� */
#if((CH563NET_TCP_MSS > 1460) || (CH563NET_TCP_MSS < 60))
#error "CH563NET_TCP_MSS Error,Please Config CH563NET_TCP_MSS >= 60 && CH563NET_TCP_MSS <= 1460"
#endif
/* ARP��������� */
#if((CH563NET_NUM_ARP_TABLE > 0X7F) || (CH563NET_NUM_ARP_TABLE < 1))
#error "CH563NET_NUM_ARP_TABLE Error,Please Config CH563NET_NUM_ARP_TABLE >= 1 && CH563NET_NUM_ARP_TABLE <= 0X7F"
#endif
/* ���POOL BUF���� */
#if(CH563NET_NUM_POOL_BUF < 1)
#error "CH563NET_NUM_POOL_BUF Error,Please Config CH563NET_NUM_POOL_BUF >= 1"
#endif
/* ���PBUF�ṹ���� */
#if(CH563NET_NUM_PBUF < 1)
#error "CH563NET_NUM_PBUF Error,Please Config CH563NET_NUM_PBUF >= 1"
#endif
/* ���IP�������� */
#if((CH563NET_NUM_IP_REASSDATA > 10)||(CH563NET_NUM_IP_REASSDATA < 1))
#error "CH563NET_NUM_IP_REASSDATA Error,Please Config CH563NET_NUM_IP_REASSDATA < 10 && CH563NET_NUM_IP_REASSDATA >= 1 "
#endif
/* ���IP��Ƭ��С */
#if(CH563NET_IP_REASS_PBUFS > CH563NET_NUM_POOL_BUF)
#error "CH563NET_IP_REASS_PBUFS Error,Please Config CH563NET_IP_REASS_PBUFS < CH563NET_NUM_POOL_BUF"
#endif

/* ���ڲ��������� */
UINT8 CH563NET_Init(const UINT8* ip,const UINT8* gwip,const UINT8* mask,const UINT8* macaddr); /* ���ʼ�� */

UINT8 CH563NET_GetVer(void);                                                    /* ��ѯ��İ汾�� */
UINT8 CH563NET_ConfigLIB(struct _CH563_CFG *cfg);                               /* ���ÿ�*/
                                                                                
void  CH563NET_MainTask(void);                                                  /* ��������������Ҫһֱ���ϵ��� */
                                                                                
void CH563NET_TimeIsr(UINT16 timperiod);                                        /* ʱ���жϷ�����������ǰ������ʱ������ */
                                                                                
void CH563NET_ETHIsr(void);                                                     /* CH563ETH�жϷ����� */ 
                                                                                
UINT8 CH563NET_GetPHYStatus(void);                                              /* ��ȡPHY״̬ */
                                                                                
UINT8 CH563NET_QueryGlobalInt(void);                                             /* ��ѯȫ���ж� */
                                                                                
UINT8 CH563NET_GetGlobalInt (void);                                             /* ��ȫ���жϲ���ȫ���ж����� */
                                                                                
void CH563NET_OpenMac(void);                                                    /* ��MAC */
                                                                                
void CH563NET_CloseMac(void);                                                   /* �ر�MAC */
                                                                                
UINT8 CH563NET_SocketCreat(UINT8 *socketid,SOCK_INF *socinf);                   /* ����socket */
                                                                                
UINT8 CH563NET_SocketSend(UINT8 socketid,UINT8 *buf,UINT32 *len);               /* Socket�������� */
                                                                                
UINT8 CH563NET_SocketRecv(UINT8 socketid,UINT8 *buf,UINT32 *len);               /* Socket�������� */
                                                                                
UINT8 CH563NET_GetSocketInt(UINT8 sockedid);                                    /* ��ȡsocket�жϲ����� */
                                                                                
UINT32 CH563NET_SocketRecvLen(UINT8 socketid,UINT32 *bufaddr);                  /* ��ȡsocket���ճ��� */
                                                                                
UINT8 CH563NET_SocketConnect(UINT8 socketid);                                   /* TCP����*/
                                                                                
UINT8 CH563NET_SocketListen(UINT8 socindex);                                    /* TCP���� */
                                                                                
UINT8 CH563NET_SocketClose(UINT8 socindex,UINT8 flag);                          /* �ر����� */

void CH563NET_ModifyRecvBuf(UINT8 sockeid,UINT32 bufaddr,UINT32 bufsize);       /* �޸Ľ��ջ����� */

UINT8 CH563NET_SocketUdpSendTo(UINT8 socketid, UINT8 *buf, UINT32 *slen,UINT8 *sip,UINT16 port);/* ��ָ����Ŀ��IP���˿ڷ���UDP�� */

UINT8 CH563NET_Aton(const UINT8 *cp, UINT8 *addr);                              /* ASCII���ַת�����ַ */
                                                                                
UINT8 *CH563NET_Ntoa(UINT8 *ipaddr);                                            /* �����ַתASCII��ַ */
                                                                                
UINT8 CH563NET_SetSocketTTL(UINT8 socketid, UINT8 ttl);                         /* ����socket��TTL */
                                                                                
void CH563NET_RetrySendUnack(UINT8 socketid);                                   /* TCP�ش� */

UINT8 CH563NET_QueryUnack(SOCK_INF  *sockinf,UINT32 *addrlist,UINT16 lislen)    /* ��ѯδ���ͳɹ������ݰ� */;

UINT8 CH563NET_DHCPStart(UINT8(* usercall)(UINT8 status,void *));               /* DHCP���� */
                                                                                
UINT8 CH563NET_DHCPStop(void);                                                  /* DHCPֹͣ */

void CH563NET_InitDNS(UINT8 *dnsip,UINT16 port);                                /* DNS��ʼ�� */

UINT8 CH563NET_GetHostName(const char *hostname,UINT8 *addr,dns_callback found,void *arg);/* DNS��ȡ������ */

#ifdef __cplusplus
}
#endif
#endif


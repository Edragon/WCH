#ifndef __UPNP_H__
#define __UPNP_H__
/*=============================================================================*/
/* �Զ���������� */
/*-----------------------------------------------------------------------------*/
typedef unsigned long                             u32;
typedef unsigned short                            u16;
typedef unsigned char                             u8;
/*=============================================================================*/
/* ����ģʽ */
/*-----------------------------------------------------------------------------*/
#define UPNP_ENABLE                               1                             /* ����UPNP���� */
#define UPNP_BUG_ENABLE                           1                             /* DEBUGʹ�� */
/*=============================================================================*/
/* SSDP���ܽ��� */
/*-----------------------------------------------------------------------------*/
#define ENABLE_SSDP_FUN                           0                             /* ����/����SSDP����,UDP�㲥 */
/*=============================================================================*/
/* �˿ڷ�Χ */
/*-----------------------------------------------------------------------------*/
#define UVC_IP_MIN_PORT                           40000
#define UVC_IP_MAX_PORT                           41000
/*=============================================================================*/
/* UPNP״̬������ */
/*-----------------------------------------------------------------------------*/
#define DEF_UPNP_STOP                             0X00                          /* UPNP ֹͣ */
#define DEF_UPNP_START                            0X01                          /* UPNP ���� */
#define DEF_UPNP_WAIT_URL                         0X02                          /* �ȴ�URL��ַ */
#define DEF_UPNP_GET_CONNECT                      0X03                          /* ����GET��TCP���� */
#define DEF_UPNP_GET_RECV                         0X04                          /* GET���ӳɹ���������GET����󣬵ȴ�UPNP��Ӧ */
#define DEF_UPNP_GET_CLOSEING                     0X05                          /* GET���ӳɹ������������ݣ��ȴ��رգ��رպ���������POST */
#define DEF_UPNP_POST_CONNECT                     0X06                          /* ����GET��TCP���� */
#define DEF_UPNP_POST_RECV                        0X07                          /* POST�ȴ��������ݣ��Ա���ӳ��ʧ�ܻ��ǳɹ� */
#define DEF_UPNP_POST_CLOSEING                    0X08                          /* POST���ӳɹ������������ݣ��ȴ��رգ��رպ���������POST */
/*=============================================================================*/
/* �ڴ�鶨�� */
/*-----------------------------------------------------------------------------*/
#define UPNP_MEM_SIZE                             170
/*=============================================================================*/
/* ��ʱ���� */
/*-----------------------------------------------------------------------------*/
#define UPNP_TIME_PERIOD                          250                           /* ��ʱ������250MS*/
#define UPNP_TIME_SECOND                          1000                          /* 1S��ʱ�� */
#define UPNP_TIMEOUT_10S                          (UPNP_TIME_SECOND*10)         /* 10S��ʱ */
#define UPNP_TIMEOUT_180S                         (UPNP_TIME_SECOND*10)        /* 180S��ʱ */
#define UPNP_TIMEOUT_20S                          (UPNP_TIME_SECOND*20)         /* 10S��ʱ */
#define UPNP_MAX_AGE_ADD                          (UPNP_TIME_SECOND*5)          /* ��MAX-AGE�����ϼ�UPNP_MAX_AGE_ADD����ֹ���⵽UPNP�豸���� */
/*=============================================================================*/
/* ӳ��Э������ */                                                                   
/*-----------------------------------------------------------------------------*/
#define MAP_TYPE_TCP                              0                            /* ӳ��Э������ΪTCP */
#define MAP_TYPE_UDP                              1                            /* ӳ��Э������ΪUDP */
/*=============================================================================*/
/*�˿ڶ��� */                                                                   
/*-----------------------------------------------------------------------------*/
#define DEF_UPNP_MULTI_SPORT                      1890                          /* �ಥ���ض˿� */
#define DEF_UPNP_MULTI_DPORT                      1900                          /* �ಥĿ�Ķ˿� */
/*=============================================================================*/
/* ������Ч��־ */                                                              
/*-----------------------------------------------------------------------------*/
#define SOCKET_ENABLE_FLAG                        0X80                          /* socketʱ�Ƿ���Ч */
/*=============================================================================*/
/* ����3�з��� */
/*-----------------------------------------------------------------------------*/
#define  UPNP_SERVER_WANIPCONNECT                 1
#define  UPNP_SERVER_WANCOMMINTER                 2
#define  UPNP_SERVER_LAY3FORWARD                  3
/*=============================================================================*/
/* ������ */
/*-----------------------------------------------------------------------------*/
#define ERR_UPNP_SUCCESS                          0
#define ERR_UPNP_FAIL                             1
#define ERR_UPNP_TCP_CONNECT                      0x30
#define ERR_UPNP_MEM_ERR                          0x31

/*=============================================================================*/
/* PNP��־λ���� */
/*-----------------------------------------------------------------------------*/
#define  DEF_UPNP_FIND                            (1<<0)
/*=============================================================================*/
/* �������� */                                                                   
/*-----------------------------------------------------------------------------*/
typedef __packed struct _UP_CTRL
{
   u32  MaxAgeBack;                                                             /* MAX-AGE����ʱ�䣬��MAP�˿ڼ�ʱ */
   u32  MaxAgeCount;                                                            /* �������ʱ��  */
   u32  MapTime;                                                                /* ���ְ����ʱ */
   u32  TimeCount;                                                              /* ��ʱ����ǰʱ�� */
   u32  SSDPTime;                                                               /* SSDP��ʱδUDPר�ö�ʱ�� */
   u8   *pRecv;                                                                 /* ��������ָ�� */
   u16  RecvLen;                                                                /* �������ݵĳ��� */
   u16  RecvTotalLen;                                                           /* �������ݵ��ܳ��� */
   u16  ExternalPort;                                                           /* UPNP �ⲿ�˿� */
   u16  InternelPort;                                                           /* UPNP�ڲ��˿� */
   u8   UpnpProType;                                                            /* UPNPЭ�����ͣ�0ΪTCP,1ΪUDP*/
   u8   IPAddr[4];
   u8   GateIPAddr[4];
   u8   UdpSocket;                                                              /* UPNP��UDP�˿ڣ����λΪ1��ʾ��Ч */
   u8   TcpSocket;                                                              /* UPNP��TCP�˿ڣ����λΪ1��ʾ��Ч */
   u8   MaxAgeStart;                                                            /* ����MaxAge��ʱ */
   u8   TimeInt;                                                                /* ��ʱ���ж� */
   u8   UpnpMach;                                                               /* ״̬��  */
   u8   UpnpFlags;                                                              /* ��־λ���ο�UPNP��־λ���� */
   const u8 *MapPortDes;                                                        /* MAP�˿����� */
   void (*AppUpnpCallBack)(u8 errcdoe);                                         /* �û��ص����� */
}UPNP_CTRL;

/*=============================================================================*/
/* UPNPӦ�ò�ṹ�壬����ʱ���� */                                                                   
/*-----------------------------------------------------------------------------*/
typedef struct _UPNP
{
   u16  InternelPort;                                                           /* UPNP�ڲ��˿� */
   u16  ExternalPort;                                                           /* UPNP �ⲿ�˿� */
   u8   IPAddr[4];                                                              /* ӳ��˿ڵ�IP��ַ */
   u8   UpnpProType;                                                            /* UPNPЭ�����ͣ�0ΪTCP,1ΪUDP*/
   const u8 *MapPortDes;                                                        /* MAP�˿����� */
}UPNP_MAP;
/*=============================================================================*/
/* UPNPӦ�ò�ṹ�壬��ʼ�����ݲ���ʹ�� */                                                                   
/*-----------------------------------------------------------------------------*/
typedef struct _UPNP_CFG
{
   u8  *UpnpMemory;
   u32  MemoryLen;
   u32  TcpMss;
   u8   GateIPAddr[4];                                                          /* ����(UPNP�豸)��IP��ַ*/
   void (*AppUpnpCallBack)(u8 errcdoe);                                         /* �û��ص����� */
}UPNP_CFG;
/*=============================================================================*/
/* �������� */                                                                   
/*-----------------------------------------------------------------------------*/
u8 CH563NET_UPNPInit(UPNP_CFG *upnpcfg);

void CH563NET_UPNPStart(UPNP_MAP *upnpmap);

u8 CH563NET_UPNPCheckSocketAvail(u8 socket);

void UPNP_CreatTcpPostSocket(void);

void CH563NET_UPNPTimeHandle(void);

void CH563NET_UPNPCreatUpdSocket(void);

void CH563NET_UPNPSendSSDPacket(void);

void CH563NET_UPNPRecvMultiBrocast(u8 socketid,u8 *buf);

void CH563NET_UPNPCreatTcpGetSocket(void);

u8 CH563NET_UPNPStartMapPort(void);

void CH563NET_UPNPInitPostCommand(u8 socketindex);

void CH563NET_UPNPTcpSendGetCommand(u8 socketindex);

void CH563NET_UPNPReceivePostCmd(u8 *buf,u32 len);

void CH563NET_UPNPReceiveGetCmd(u8 *buf,u32 len);

void CH563NET_UPNPReceiveDataHandle(u8 socket );

void CH563NET_UPNPMainTask(void);

void CH563NET_UPNPTCPDisConnectHandle(u8 socketid);
#endif
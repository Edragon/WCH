/********************************** (C) COPYRIGHT *******************************
* File Name          : upnp.c
* Author             : lht
* Version            : V1.0
* Date               : 2013/04/12
* Description        : IGD�豸upnpע��˿�
*                      
*******************************************************************************/

/* include files*/
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "CH563NET.H"
#include "UPNP.H"
#define DBG                             0
#if DBG
#define  cprintf(format,arg...)         printf(format,##arg)
#else
#define  cprintf(format,arg...)         do{ if(0)printf(format,##arg);}while(0);
#endif
/* ȫ�ֳ������� */
u8 const Discover[97] = 
{
    0x4D, 0x2D, 0x53, 0x45, 0x41, 0x52, 0x43, 0x48, 0x20, 0x2A, 0x20, 0x48, 0x54, 0x54, 0x50, 0x2F, 
    0x31, 0x2E, 0x31, 0x0D, 0x0A, 0x48, 0x6F, 0x73, 0x74, 0x3A, 0x32, 0x33, 0x39, 0x2E, 0x32, 0x35, 
    0x35, 0x2E, 0x32, 0x35, 0x35, 0x2E, 0x32, 0x35, 0x30, 0x3A, 0x31, 0x39, 0x30, 0x30, 0x0D, 0x0A, 
    0x53, 0x54, 0x3A, 0x75, 0x70, 0x6E, 0x70, 0x3A, 0x72, 0x6F, 0x6F, 0x74, 0x64, 0x65, 0x76, 0x69, 
    0x63, 0x65, 0x0D, 0x0A, 0x4D, 0x61, 0x6E, 0x3A, 0x22, 0x73, 0x73, 0x64, 0x70, 0x3A, 0x64, 0x69, 
    0x73, 0x63, 0x6F, 0x76, 0x65, 0x72, 0x22, 0x0D, 0x0A, 0x4D, 0x58, 0x3A, 0x33, 0x0D, 0x0A, 0x0D, 
    0x0A
};
/**< SSDP Header */
const char SSDP[] = "\
M-SEARCH * HTTP/1.1\r\n\
Host:239.255.255.250:1900\r\n\
ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n\
Man:\"ssdp:discover\"\r\n\
MX:3\r\n\
\r\n\
";
u8 const IP_ANY[4] =  {255,255,255,255}; 
u8 const IP_ANY1[4] = {239,255,255,250}; 
/*******************************************************************************/
UPNP_CTRL  UpnpCtrl;
u8 *UpnpSockRecvBuf;                                                            /* socket���ջ����� */
u8 *UpnpSockSendBuf;                                                            /* ���ͻ����� */
u8 *UpnpTmpBuf;                                                                 /* ��ʱ������ */
u32 UpnpFlashAddr;
u8 UrlBuf[170];
u8 UrlDirBuf[170];
u8 UrlCtrlBuf[170];
static u16 TotalBufLen;
static u16 SocketRecvBufLen;
static u16 SocketSendBufLen;
static u16 UpnpTmpBufLen;

/*******************************************************************************
* Function Name  : strcasestr
* Description    : �����ַ��������Դ�Сд
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int strncasecmp(const char *s1, const char *s2, size_t n)
{
    int c1, c2;

    do {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
    } while ((--n > 0) && c1 == c2 && c1 != 0);
    return c1 - c2;
}

char* strcasestr(const char* str, const char* subStr)
{
    u16 len = strlen(subStr);
    
    if(len == 0)
    {
        return NULL;                                 
    }
    while(*str)
    {
        if(strncasecmp(str, subStr, len) == 0)       
        {
            return (u8 *)str;
        }
        str++;
    }
    return NULL;
}
/*******************************************************************************
* Function Name  : CH563NET_InetNtoa
* Description    : IP��ַת��Ϊ�ַ�
* Input          : ipddr ---- ��Ҫת����IP��ַ
* Output         :  cp   ---- ���IP��ַ�ַ���
* Return         : None
*******************************************************************************/
void CH563NET_InetNtoa(char *cp,u8 *ipddr)
{
    char *str = cp;
    char inv[3];
    char *rp;
    u8 *ap;
    u8 rem;
    u8 n;
    u8 i;

    rp = str;
    ap = ipddr;
    for(n = 0; n < 4; n++) {
        i = 0;
        do {
            rem = *ap % (UINT8)10;
            *ap /= (UINT8)10;
            inv[i++] = '0' + rem;
        } while(*ap);
        while(i--)
        *rp++ = inv[i];
        *rp++ = '.';
        ap++;
    }
    *--rp = 0;
}

/*******************************************************************************
* Function Name  : UVCIP_GetPort
* Description    : ��ȡ��ǰ�ö˿�
* Input          : None
* Output         : None
* Return         : ��ǰ���õĶ˿ں�
*******************************************************************************/
u16 UVCIP_GetPort(void)
{
    static usedport = UVC_IP_MIN_PORT;

    if(usedport >= UVC_IP_MAX_PORT)usedport = UVC_IP_MIN_PORT;
    return(usedport++);
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPMainTask
* Description    : UPNP��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPMainTask(void)
{
   CH563NET_UPNPTimeHandle();
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPInit
* Description    : UPNPģ���ʼ��
* Input          : upnpcfg ---- ����
* Output         : None
* Return         : ִ�н��
*******************************************************************************/
u8 CH563NET_UPNPInit(UPNP_CFG *upnpcfg)
{
    u32 addr;
    u16 i;

    i = sizeof(SSDP);
    memset(&UpnpCtrl,0,sizeof(UpnpCtrl));                                       /* ���ṹ������ */
    UpnpCtrl.GateIPAddr[0] = upnpcfg->GateIPAddr[0];
    UpnpCtrl.GateIPAddr[1] = upnpcfg->GateIPAddr[1];
    UpnpCtrl.GateIPAddr[2] = upnpcfg->GateIPAddr[2];
    UpnpCtrl.GateIPAddr[3] = upnpcfg->GateIPAddr[3];
    UpnpCtrl.AppUpnpCallBack = upnpcfg->AppUpnpCallBack;
    TotalBufLen = upnpcfg->MemoryLen;
    SocketRecvBufLen = SocketSendBufLen = 2* (upnpcfg->TcpMss); 
    UpnpTmpBufLen = TotalBufLen - SocketRecvBufLen - SocketSendBufLen;  
    if(TotalBufLen < (SocketRecvBufLen + SocketSendBufLen))return ERR_UPNP_MEM_ERR;
    if(UpnpTmpBufLen < SocketRecvBufLen)return ERR_UPNP_MEM_ERR;     
    UpnpSockRecvBuf = upnpcfg->UpnpMemory;                                      /* ͷ��Ϊsocket���ջ����� */
    UpnpSockSendBuf = UpnpSockRecvBuf + SocketRecvBufLen;                       /* ���ջ�������Ϊ���ͻ����� */
    UpnpTmpBuf = UpnpSockSendBuf + SocketSendBufLen;                            /* ���ͻ�����֮��Ϊ��ʱ������ */
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPTimeIsr
* Description    : UPNP��ʱ���жϣ�250MS����һ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTimeIsr(void)
{
    UpnpCtrl.TimeInt = 1;                                                       /* �����ж��¼���־���� */
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPTimeOutHandle
* Description    : UPNP��ʱ���ж��¼�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTimeOutHandle(void)
{
    if(UpnpCtrl.UpnpMach == DEF_UPNP_STOP)return;
    switch(UpnpCtrl.UpnpMach)
    {
        case DEF_UPNP_START:                                                    /* ״̬�������׶Σ���ʱ���󣬿�ʼ���� */
        case DEF_UPNP_WAIT_URL:
        case DEF_UPNP_GET_CONNECT:
        case DEF_UPNP_GET_RECV:
        case DEF_UPNP_GET_CLOSEING:
        case DEF_UPNP_POST_CONNECT:
        case DEF_UPNP_POST_RECV:
        case DEF_UPNP_POST_CLOSEING:
        default:                                                                /* ���ӳ�ʱ���� */
            UpnpCtrl.UpnpFlags &= ~DEF_UPNP_FIND;
            UpnpCtrl.MapTime = 0;
            if(UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG )
            {
               CH563NET_SocketClose(UpnpCtrl.TcpSocket & ~SOCKET_ENABLE_FLAG,0);    
            }
            UpnpCtrl.UpnpMach = DEF_UPNP_START;
        break;
    }
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPTimeHandle
* Description    : UPNP��ʱ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTimeHandle(void)
{
    if(UpnpCtrl.TimeInt)
    {
        UpnpCtrl.TimeInt = 0;                                                   /* ����ж��¼���־���� */
        if(UpnpCtrl.UpnpMach == DEF_UPNP_STOP)return;
        if(UpnpCtrl.SSDPTime)                                                   /* SSDPΪUDPר�ö�ʱ�� */
        {
            UpnpCtrl.SSDPTime -= UPNP_TIME_PERIOD;
            if(UpnpCtrl.SSDPTime == 0)
            {
                if(!(UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG))                  /* ���UDP�Ƿ���ڣ������������´��� */
                {
                   CH563NET_UPNPCreatUpdSocket(); 
                }
                if(UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG)
                {
                    if(!(UpnpCtrl.UpnpFlags &= DEF_UPNP_FIND))
                    {
                        cprintf("NO Find Upnp Device\n");
                    }
                    CH563NET_UPNPSendSSDPacket();
                }
                UpnpCtrl.SSDPTime = UPNP_TIMEOUT_180S;
            }

        }
        if(UpnpCtrl.TimeCount)
        {
            UpnpCtrl.TimeCount -= UPNP_TIME_PERIOD;                             /* ��������С */
            if(UpnpCtrl.TimeCount == 0)                                         /* ����ʱ�䵽 */
             {                                                                  
                 CH563NET_UPNPTimeOutHandle();                                 /* ��ʱ������ */
             }                                                                  
        }                                                                       
        if(UpnpCtrl.MaxAgeCount)                                                
        {                                                                       
            UpnpCtrl.MaxAgeCount -= UPNP_TIME_PERIOD;                           /* �������ʱ����� */
            if(UpnpCtrl.MaxAgeCount == 0)                                       /* ����ʱ����� */
            {                                                                   
                 cprintf("Upnp device Remove \n");                               
                 if(UpnpCtrl.UpnpFlags & DEF_UPNP_FIND)                         
                 {                                                              
                   UpnpCtrl.UpnpFlags &= ~DEF_UPNP_FIND;                        /* �����ڽ������豸��Ȼδ�յ�UPNP�豸��SSDP�������ֱ�־��� */
                 }
            }
        }
        if(UpnpCtrl.MapTime)                                                    /* �˶�ʱ�������ڶ˿�ӳ�䣬ӳ��һ��ʱ�����Ҫ����ӳ�� */
        {                                                                       
            UpnpCtrl.MapTime -= UPNP_TIME_PERIOD;                               
            if(UpnpCtrl.MapTime == 0)                                           /* ��ʱ��ϣ��ٴ�MAP�˿� */
            {                                                                   
                 cprintf("Map Time end and Map Port\n");                         
                 UpnpCtrl.TimeCount = 0;                                        /*stop timer */
                 UpnpCtrl.TcpSocket = 0;
                 CH563NET_UPNPStartMapPort();
            } 
        }
    }
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPStart
* Description    : ����UPNP
* Input          : upnpmap ---- ��Ҫӳ��Ķ˿���Ϣ
* Output         : None
* Return         : None
*******************************************************************************/
//CH563NET_UPNPUPNPFunStart
void CH563NET_UPNPStart(UPNP_MAP *upnpmap)
{
    u8 *p = "http://192.168.1.1:1900/igd.xml";

    if(UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG )
    {
        CH563NET_SocketClose(UpnpCtrl.UdpSocket & ~SOCKET_ENABLE_FLAG,0);    
    }
    if(UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG )
    {
        CH563NET_SocketClose(UpnpCtrl.TcpSocket & ~SOCKET_ENABLE_FLAG,0);    
    }
//    memset(&UpnpCtrl,0,sizeof(UpnpCtrl));
    UpnpCtrl.TimeCount = 0;                                                     /* ��ʼ�� */
    UpnpCtrl.MaxAgeCount = 0;                                                   /* �������ʱ������ */
    UpnpCtrl.UpnpMach = DEF_UPNP_START;                                         /* ״̬�����뿪ʼ״̬ */
    UpnpCtrl.SSDPTime = UPNP_TIMEOUT_180S;                                      /* SSDP���ְ���ʱΪ3���� */
    UpnpCtrl.ExternalPort = upnpmap->ExternalPort;                              
    UpnpCtrl.InternelPort = upnpmap->InternelPort;                              
    UpnpCtrl.UpnpProType  = upnpmap->UpnpProType;                               
    UpnpCtrl.MapPortDes   = upnpmap->MapPortDes;                                
    UpnpCtrl.IPAddr[0] = upnpmap->IPAddr[0];
    UpnpCtrl.IPAddr[1] = upnpmap->IPAddr[1];
    UpnpCtrl.IPAddr[2] = upnpmap->IPAddr[2];
    UpnpCtrl.IPAddr[3] = upnpmap->IPAddr[3];

    UpnpCtrl.UpnpFlags = 0;                                                     /* ����־λ��� */
#if ENABLE_SSDP_FUN
    CH563NET_UPNPCreatUpdSocket();                                              /* ����UDP��socket */

    if(UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG)                                 /* �ж�socket�Ƿ񴴽��ɹ� */
    {                                                                           /* �ɹ�������SSDP,(����UPNP�豸 ) */
        CH563NET_UPNPSendSSDPacket();                                                
        UpnpCtrl.SSDPTime = UPNP_TIMEOUT_180S;                                  /* 180S�ڣ�δ�ȵ�URL�����ٴη���SSDP */
    }                                                                          
    else                                                                       
    {                                                                          
        UpnpCtrl.SSDPTime = UPNP_TIMEOUT_20S;                                   /* ��ʼ��ʱ����20S���ٴ����Դ��� */
    }
    UpnpCtrl.UpnpMach = DEF_UPNP_START;
#endif
    memset(UrlBuf,0,sizeof(UrlBuf));                                            /* ��������ȫ������ */
    p = &UrlBuf[1];
    sprintf(p,"http://%d.%d.%d.%d:1900/igd.xml",UpnpCtrl.GateIPAddr[0],UpnpCtrl.GateIPAddr[1],UpnpCtrl.GateIPAddr[2],UpnpCtrl.GateIPAddr[3]);
    UrlBuf[0] = strlen(p) + 1;    
    CH563NET_UPNPCreatTcpGetSocket();   
    UpnpCtrl.MapTime = 0;
    UpnpCtrl.TimeCount = UPNP_TIMEOUT_10S;                                      /* ���볬ʱ */
    UpnpCtrl.UpnpMach = DEF_UPNP_GET_CONNECT;                                   /* ����ȴ����� */
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPPStop
* Description    : ֹͣUPNP
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPPStop(void)
{
    UpnpCtrl.UpnpMach = DEF_UPNP_STOP;
    
    if(UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG)                                 /* �ر�TCP socket */
    {
         UpnpCtrl.TcpSocket &= ~ SOCKET_ENABLE_FLAG;
         CH563NET_SocketClose(UpnpCtrl.TcpSocket,1);     
    }
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPCheckSocketAvail
* Description    : ��������Ҫ���ڽ����ж����ж��Ƿ�ΪUPNP�Ľӿ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 CH563NET_UPNPCheckSocketAvail(u8 socket)
{
    if(UpnpCtrl.UpnpMach == DEF_UPNP_STOP)return FALSE;                        /* upnp funciton may be stoped */
    socket |= SOCKET_ENABLE_FLAG;                                              /* if socket is available ,the socket index highest bit is 1 */
    if((socket == UpnpCtrl.UdpSocket) || (socket == UpnpCtrl.TcpSocket))       /* upnp use two sockets */
    {
        return TRUE;
    }
    else FALSE;
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPCreatUpdSocket
* Description    : ����һ���ಥUDP����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPCreatUpdSocket(void)
{
   u8 desip[4];                                                 
   u8 i,uSocketid;
   SOCK_INF  TmpSocketInf;
   desip[0] = 0xff;
   desip[1] = 0xff;
   desip[2] = 0xff;
   desip[3] = 0xff;

   memset(&TmpSocketInf,0,sizeof(TmpSocketInf));                                /* clear local variable*/
   memcpy((void *)TmpSocketInf.IPAddr,desip,4);                                 /* set destination  ip address */
   TmpSocketInf.SourPort = 1900;                                                
   TmpSocketInf.DesPort = 1025;                                                 
   TmpSocketInf.ProtoType = PROTO_TYPE_UDP;                                     /* protocol type is udp */
   TmpSocketInf.RecvStartPoint = (UINT32)UpnpSockRecvBuf;                       /* ���ý��ջ������Ľ��ջ����� */
   TmpSocketInf.RecvBufLen = SocketRecvBufLen ;                                 /* ���ý��ջ������Ľ��ճ��� */
   i = CH563NET_SocketCreat(&uSocketid,&TmpSocketInf);                          /* creat upnp socket */
   CH563NET_SetSocketTTL(uSocketid,1);                                          /* ��TTL ����Ϊ1 */
   if(i != CH563NET_ERR_SUCCESS)return;                                         /* check  creat socket succeccful ,if not ,next creat again */
   UpnpCtrl.UdpSocket = uSocketid | 0x80;                                       /* save socket index,and high bit set 1*/   
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPSendSSDPacket
* Description    : ����UPNP���ְ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPSendSSDPacket(void)
{
   u8 i;
   u32 len;

   if(!(UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG))return;                        /* UPNP��socket�����Ѿ����ر��� */
   i = UpnpCtrl.UdpSocket & ~SOCKET_ENABLE_FLAG;
   len = sizeof(Discover);
   CH563NET_SocketUdpSendTo(i,(u8 *)Discover,(u32 *)(&len),\
   (u8 *)IP_ANY1,DEF_UPNP_MULTI_DPORT);                                         /* ��Ŀ�Ķ˿ڶಥSSD���ݰ� */
}
/*******************************************************************************
* Function Name  : MultiBrocast
* Description    : ����һ���ಥUDP����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPRecvMultiBrocast(u8 socketid,u8 *buf)
{
    u8 *p;
    u8 *lp;
    u8 i = 0;
    u8 ipstr[20];
    u8 addr[4];
    u16 port;
    u8  server = 0; 
    UPNP_MAP upnpmap;


    p = buf;
    if(lp = strcasestr(p,"ssdp:alive"))                                         /*  ssdp�㲥���� */
    {
       i = 0;
    }
    else if(lp = strcasestr(buf,"ssdp:byebye"))                                 /* �����豸������ */
    {
        cprintf("byebye\n");
        cprintf("%s\n",buf);
        i = 1;
    }
    else return;
    p = buf;
    /* ��һ�����ҷ������� */
    if(lp = strcasestr(p, "service:WANIPConnection"))                           /* �����Ƿ�ΪWANIPConnection���� */
    {
         cprintf("Fine WANIPConnection server\n");
         server = UPNP_SERVER_WANIPCONNECT;
         if(i)                                                                  /* WANIPConnection �豸�Ƴ� */
         {
             cprintf("device bye bye\n");
             upnpmap.ExternalPort = UpnpCtrl.ExternalPort;  
             upnpmap.InternelPort = UpnpCtrl.InternelPort;
             upnpmap.UpnpProType  = UpnpCtrl.UpnpProType; 
             upnpmap.MapPortDes   = UpnpCtrl.MapPortDes;  
             upnpmap.IPAddr[0]= UpnpCtrl.IPAddr[0]  ;
             upnpmap.IPAddr[1]= UpnpCtrl.IPAddr[1]  ;
             upnpmap.IPAddr[2]= UpnpCtrl.IPAddr[2]  ;
             upnpmap.IPAddr[3]= UpnpCtrl.IPAddr[3]  ;
             //CH563NET_UPNPStart(&upnpmap);
             UpnpCtrl.UpnpFlags  &=  ~DEF_UPNP_FIND;
             return;
         }
    }
    else return;                                                                /* ��ʶ��WANIPConnection�������������� */
    /* �ڶ������ҷ������͵�URL����Ҫ�ҳ�IP,PORT��ROOT DIR���������� */
    if((lp = strcasestr(p,"LOCATION:")))                                        /* �ҵ�locationλ�� */
    {
       u8 blen;
       u8 *urlbuf;
       u8 wrflag = 0;

       p = strcasestr(lp,"http://");                                            /*����URL */
       lp = strstr(p,".xml");
       i = lp - p + 4;
       blen = UrlBuf[0];//FlashOneByteRead(FlashUrlAddr);
       if(blen != i)
       {
            wrflag = 1;
       }
       else
       {
           for(blen = 0; blen < i; blen++)
          {
              //if(p[blen] != FlashOneByteRead(FlashUrlAddr + 1 + blen))/* ��0���ֽڱ����˳��ȣ������ڶԱȵ�ʱ��ҪFlashCtrlUrlAddr + 1*/
              if(p[blen] != UrlBuf[1 + blen])                                   /* ��0���ֽڱ����˳��ȣ������ڶԱȵ�ʱ��ҪFlashCtrlUrlAddr + 1*/
              {
                  wrflag = 1;
                  break;   
             }
          }
       }
       if(wrflag)
       {
           if(i > (UPNP_MEM_SIZE - 1))return;                                 /* ���Ȳ��������������ݣ���ֹ��������³����쳣 */
           cprintf("****************Url Change and save it\n");
           urlbuf = UrlBuf;
           *urlbuf++ = i;                                                       /* �����ȱ��� */
           memset(urlbuf,0,UPNP_MEM_SIZE - 1);
           memcpy(urlbuf,p,i);
           cprintf("Url:%s\n",urlbuf);
           UpnpCtrl.UpnpFlags  &=  ~DEF_UPNP_FIND;                              /* UPNP�豸�ı䣬�����������ֱ�־��� */ 
       }                                                                        /* ����˱�־�󣬻ᷢ��GET�������»�ȡcontrol URL */
       p += sizeof("http://") - 1;                                              /* ���ʽΪhttp://192.168.1.1:1900/xxx.xml */                                   
       if(lp = strcasestr(p,":"))
       {
          i = lp - p;
          memset(ipstr,0,20);
          memcpy(ipstr,p,i);
          CH563NET_Aton(ipstr,addr);
          lp++;                                                                 /* lpָ��':'*/
          p = strcasestr(lp,"/");                                               /*Pָ��'/'���м�Ϊ�˿��ַ� */
          i = p - lp;                                                           /* ����˿��ַ����Ĵ�С */ 
          memset(ipstr,0,20);                                                   
          memcpy(ipstr,lp,i);                                                   /* ���ַ�����ȡ���������� */
          port = atoi(ipstr);
          lp = strstr(lp,"/");
          p = lp;
          lp = strstr(lp,".xml");
          i = lp - p + 4;                                                       /* �����·���ĳ��� */
        
          if(server == UPNP_SERVER_WANIPCONNECT)                     
          {
              p = buf;
              /* ����MAX-AGE,MAX-ageΪ������Чʱ�䣬��ֵ���¼�� */
              /* UpnpCtrl.MaxAgeCount�У���ÿ���յ�max-age�󣬶� */
              /* ��UpnpCtrl.MaxAgeCountˢ�£����MaxAgeCountΪ0��*/
              /* ���ʾUPNP�豸��ʧ�ˡ���ʵ�ʴ���ʱ��Ϊ�˷�ֹ��  */
              /* ֵ������0�����⵽�豸�����ߣ����Լ�����5��ʱ��*/
              if((lp = strcasestr(p, "CACHE-CONTROL:")))           
              {
                  lp = strcasestr(p,"max-age=");
                  if(lp)
                  {
                      lp += sizeof("max-age=") - 1; 
                      UpnpCtrl.MaxAgeCount = atoi(lp) * 1000;
                      UpnpCtrl.MaxAgeCount += UPNP_MAX_AGE_ADD;
                      cprintf("UpnpCtrl.MaxAgeCount = %d\n",UpnpCtrl.MaxAgeCount);                     
                  }
                  else
                  {
                      UpnpCtrl.MaxAgeCount = 100 * 1000;                        /* ��ֹû�ҵ���Ĭ��Ϊ100S */
                  } 
              }
              else
              {
                  UpnpCtrl.MaxAgeCount = 100 * 1000;                            /* ��ֹû�ҵ���Ĭ��Ϊ100S */
              }                                                                 
              UpnpCtrl.MaxAgeBack = UpnpCtrl.MaxAgeCount;                       /* ����MAX-AGE����ע����Ϻ�ʹ�� */
              UpnpCtrl.SSDPTime = UPNP_TIMEOUT_180S;                            /* ˢ��SSDPʱ���б� */
              cprintf("Find UPNP_SERVER_WANIPCONNECT\n");                       
              if(!(UpnpCtrl.UpnpFlags & DEF_UPNP_FIND))                         /* ���UPNP�豸�����ڣ��ڻ�ȡcontrol ��URL*/
              {
                  cprintf("Fine New Upnp Device and Creat Get Socket\n");
                  if(UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG)                   /* ���ܴ�ʱUPNP����TCP socket��ʱ�������ٴ�ʹ���� */
                  {
                     if((UpnpCtrl.UpnpMach >= DEF_UPNP_POST_CONNECT)&&\
                         (UpnpCtrl.UpnpMach <= DEF_UPNP_POST_CLOSEING))         /* ��ʱ������POST״̬��GET�����ȼ��Ƚϸߣ���POST���Ӹ�λ */
                     {
                         UpnpCtrl.TcpSocket &= ~ SOCKET_ENABLE_FLAG;
                         CH563NET_SocketClose(UpnpCtrl.TcpSocket,1);     
                     }
                     else return;                                               /* ˵����GET״̬����ֱ�ӷ��� */  
                  }
                  CH563NET_UPNPCreatTcpGetSocket();   
                  UpnpCtrl.MapTime = 0;
                  UpnpCtrl.TimeCount = UPNP_TIMEOUT_10S;                        /* ���볬ʱ */
                  UpnpCtrl.UpnpMach = DEF_UPNP_GET_CONNECT;                     /* ����ȴ����� */
              }
          }
       }
    }
}


/*******************************************************************************
* Function Name  : CH563NET_UPNPTcpSendGetCommand
* Description    : ��ʼ��GET����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTcpSendGetCommand(u8 socketindex)
{
   u8 *buf;
#define TMP_LEN   60
   u8 tmp[TMP_LEN];
   u8 *p,*urlbuf;
   u8 *lp;
   u8 i;
   u32 len;

   memset(UpnpSockSendBuf,0,SocketSendBufLen);
   buf = UpnpSockSendBuf;
   memcpy(buf,"GET ",strlen("GET "));
   urlbuf = (u8 *)&UrlBuf[1];//(u8 *)(UpnpFlashAddr + FLASH_LOCAT_URL + 1);
   p = strcasestr(urlbuf,"http://");                                 /* ͨ��URL���ҵ�IP��ַ */
   p += sizeof("http://");
   p = strcasestr(p,"/");
   lp = strcasestr(p,".xml"); 
   i = lp - p + sizeof(".xml");
   memset(tmp,0,TMP_LEN);
   memcpy(tmp,p,i);
   cprintf("Dir:%s\n",tmp);
   strcat(buf,tmp);
   strcat(buf," HTTP/1.1\r\n");
   strcat(buf,"HOST: ");
   p = strcasestr(urlbuf,"http://");
   p += sizeof("http://") - 1;
   lp = strcasestr(p,"/");
   i = lp - p;
   memset(tmp,0,TMP_LEN);
   memcpy(tmp,p,i);
   cprintf("Host:%s\n",tmp);
   strcat(buf,tmp);
   strcat(buf,"\r\nACCEPT-LANGUAGE: en\r\n\r\n");
   len = strlen(buf);
   CH563NET_SocketSend(socketindex,buf,(u32 *)(&len)); 
}


/*******************************************************************************
* Function Name  : CH563NET_UPNPCreatTcpGetSocket
* Description    : UPNP����GET TCP Sockets
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPCreatTcpGetSocket(void)
{
    u8 i;
    u8 *p,*lp,*urlbuf;
    u8 tmp[60];
    u8 addr[4];
    u16 port;
    SOCK_INF  TmpSocketInf;
    
    cprintf("CH563NET_UPNPCreatTcpGetSocket\n");
    urlbuf = (u8 *)&UrlBuf[1];//(u8 *)(UpnpFlashAddr + FLASH_LOCAT_URL + 1);    /* ֱ�ӽ���ַָ��FLASH�� */ 
    p = strcasestr(urlbuf,"http://");                                           /* ͨ��URL���ҵ�IP��ַ */
    p += sizeof("http://") - 1;                                                 /* ��ʽΪHTTP://+IP+:+PORT */
    lp = strcasestr(p,":");                                                     /* ����ð�� */
    i = lp - p;                                                                 /* ���㳤�� */
    memset(tmp,0,20);                                                           /* ����ʱ������ȫ������ */
    memcpy(tmp,p,i);                                                            /* ��IP�ַ������Ƶ���ʱ�������� */
    cprintf("ipstr %s\n",tmp);                                                  
    CH563NET_Aton(tmp,addr);                                                    /* ���ַ���ת��ΪIP��ַ */
    cprintf("IP: %d.%d.%d.%d\n",addr[0],addr[1],addr[2],addr[3]);               /* ��IP���Դ�ӡ */
    lp++;                                                                       
    p = strcasestr(lp,"/");                                                     /* ���Ҷ˿� */
    i = p - lp;                                                                 
    memset(tmp,0,20);                                                           /* ����ʱ������ȫ������ */
    memcpy(tmp,lp,i);                                                           /* ��IP�ַ������Ƶ���ʱ�������� */
    port = atoi(tmp);                                                           /* ����˿� */
    cprintf("Port = %d\n",port);                                                
                                                                                
   memset(&TmpSocketInf,0,sizeof(TmpSocketInf));                                /* ����ʱ��������� */
   memcpy((void *)TmpSocketInf.IPAddr,addr,4);                                  /* ����Ŀ��IP��ַ */
   TmpSocketInf.SourPort = UVCIP_GetPort();                                     /* ����Դ�˿� */
   TmpSocketInf.DesPort = port;                                                 /* ����Ŀ�Ķ˿� */
   TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* Э������ */
   TmpSocketInf.RecvStartPoint = (UINT32)UpnpSockRecvBuf;                       /* ���ý��ջ������Ľ��ջ����� */
   TmpSocketInf.RecvBufLen = SocketRecvBufLen ;                                 /* ���ý��ջ������Ľ��ճ��� */
   UpnpCtrl.TcpSocket = 0X00;                                                   /* ��TCP��־���� */
   i = CH563NET_SocketCreat(&UpnpCtrl.TcpSocket,&TmpSocketInf);                 /* ����TCP socket */
   if(i != CH563NET_ERR_SUCCESS) 
   {
      cprintf("Creat Socket Fail %02x\n",i);
      return;                                                                   /* ����ʧ��ֱ�ӷ��� */
   }
   cprintf("CH563NET_SocketCreat success %d\n\n",UpnpCtrl.TcpSocket);
   UpnpCtrl.TcpSocket |= SOCKET_ENABLE_FLAG;                                    /* �����ɹ�����socketΪ��Ч */

   i = CH563NET_SocketConnect(UpnpCtrl.TcpSocket & ~SOCKET_ENABLE_FLAG );
   if(i != CH563NET_ERR_SUCCESS)
   {
      cprintf(" Socket Connect Fail %02x\n",i);
      return;                                                                   /* ����ʧ��ֱ�ӷ��� */
   }
   cprintf("CH563NET_SocketConnect success\n");
}
/*******************************************************************************
* Function Name  : CH563NET_UPNPReceiveGetCmd
* Description    : GET������պ�����ѭ�����գ�������Ϻ����URL
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPReceiveGetCmd(u8 *buf,u32 len)
{
    u8  *p = buf;
    u8  *lp;
    u8  *wp; 
    u16  i;
    u8  fag = 0;
    //cprintf("UpnpCtrl.RecvTotalLen = %02x\n",UpnpCtrl.RecvTotalLen);
    //cprintf("%s\n",buf);
    if(UpnpCtrl.RecvTotalLen == 0)                                              /* ���յ��ܳ���Ϊ0����ʾ����������յ����ݳ��� ��Ϊ��һ������ */
    {                                                                           /* ������ܳ��� */
         p = strcasestr(p, "Content-Length:");                                  /* �ҵ������ڲ���־ */
         if(p)                                                                  /* ���㳤��  */
         {                                                                      
             u32 xlen = 0;                                                      
             //cprintf("Find Content-Length:\n");                               
             lp = strcasestr(p, "\r\n");                                        /* �ҵ����н��� */
             p += sizeof("Content-Length:") - 1;                                /* ��ָ��ƫ�Ƶ���ȷλ�� */
             i = *lp;                                                           /* ����*lp */
             *lp = 0;                                                           
             xlen = atoi(p);                                                    
             *lp = i;                                                           /* ��ԭlp */
             cprintf("Pose Receive Cmd = %d\n",xlen);
             UpnpCtrl.RecvTotalLen = xlen;
             
             UpnpCtrl.RecvLen = 0;
             p = buf;
             if(p = strcasestr(p,"<?xml version=\"1.0\""))
             {
                 //cprintf("find <?xml version=\"1.0\"?>\n");
                 UpnpCtrl.RecvLen = len - (p - buf);
                 UpnpCtrl.pRecv = UpnpTmpBuf; 
                 memcpy(UpnpCtrl.pRecv,p,UpnpCtrl.RecvLen);
                 UpnpCtrl.pRecv += UpnpCtrl.RecvLen;
                 //cprintf("First packet len = %d",UpnpCtrl.RecvLen);
             }
         }

         return;
    }
    else
    {
        i = 0;
        if(UpnpCtrl.RecvLen < UpnpTmpBufLen)
        {
            i = UpnpTmpBufLen - UpnpCtrl.RecvLen; 
        }
        else 
        {
            if(UpnpCtrl.AppUpnpCallBack)UpnpCtrl.AppUpnpCallBack(ERR_UPNP_MEM_ERR); /* tmp�ڴ����Ĺ��� */   
        }
        i = i > len? len:i;
        if(i)memcpy(UpnpCtrl.pRecv,buf,i);
        UpnpCtrl.RecvLen += len;
        UpnpCtrl.pRecv += len;
        //cprintf("UpnpCtrl.RecvLen = %d\n",UpnpCtrl.RecvLen);
        if(UpnpCtrl.RecvLen < UpnpCtrl.RecvTotalLen)return;                      /* ����δ�����꣬��ȴ����������� */
    }
    p = UpnpTmpBuf;
    while(1)
    {
        //cprintf("%s\n",p);
        p = strcasestr(p, "<serviceList>");                                     /* �����б�Ŀ�ʼ��ַ */
        lp = strcasestr(p, "</serviceList>");                                   /* �����б�Ľ�����ַ */
        if((p == NULL)||(lp == NULL))
        {
            cprintf("nono find serviceList = %d   %d\n",p,lp);
            break;                         /* δ�ҵ������б� */
        }
        if(wp = strcasestr(p, "service:WANIPConnection"))                       /* ˵����service:WANIPConnection�����б� */
        {
            u8 blen;
            u8 *CtrlUrlBuf;
            u8 wrflag = 0;

            if(wp = strcasestr(wp, "<controlURL>"))                             /* �ҵ�����URL*/
            {
                lp = strcasestr(wp, "</controlURL>");                           /* �ҵ�����URL�Ľ��� */
                if(lp == NULL)break;
                wp = strcasestr(wp, "/");                                       /* �ҵ�<controlURL>��ĵ�һ��'/'*/
                i = lp - wp;
                
                blen = UrlDirBuf[0];//FlashOneByteRead(FlashCtrlUrlAddr);
                if(blen != i)wrflag = 1;
                else
                {
                     for(blen = 0; blen < i; blen++)
                     {
                        // if(wp[blen] != FlashOneByteRead(FlashCtrlUrlAddr + 1 + blen))/* ��0���ֽڱ����˳��ȣ������ڶԱȵ�ʱ��ҪFlashCtrlUrlAddr + 1*/
                         if(wp[blen] != UrlDirBuf[1 + blen])                    /* ��0���ֽڱ����˳��ȣ������ڶԱȵ�ʱ��ҪFlashCtrlUrlAddr + 1*/
                         {
                            wrflag = 1;
                            break;   
                         }
                     }
                }
                if(wrflag)
                {
                    if(i > (UPNP_MEM_SIZE - 1))return;                        /* ��鳤�ȣ���ֹ�ַ������������³����쳣 */
                    //FlashBytes(UpnpFlashAddr,UpnpSockSendBuf,512);
                    CtrlUrlBuf =  UrlDirBuf;//UpnpSockSendBuf + FLASH_LOCAT_CTIRL_URL;
                    *CtrlUrlBuf++ = i;                                          /* �����ȱ��� */
                    memset(CtrlUrlBuf,0,UPNP_MEM_SIZE - 1);
                    memcpy(CtrlUrlBuf,wp,i);
                    //FlashWrite(UpnpFlashAddr,(u8 *)UpnpSockSendBuf, 512);
                    cprintf("Control Url:%s\n",CtrlUrlBuf); 
                }
                fag = 1;
                UpnpCtrl.UpnpFlags |= DEF_UPNP_FIND;                            /* �ҵ�UPNP�豸,״̬�������κθı�  */
                break; 
            }
            else break;
        }
        else p = lp;
        if(p > UpnpCtrl.pRecv)break;
    }
    UpnpCtrl.UpnpMach = DEF_UPNP_GET_CLOSEING;                                  /* ��Ϊ�ȴ��ر�״̬ */
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPInitPostCommand
* Description    : ��ʼ��Post����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPInitPostCommand(u8 socketindex)
{
   u8 *buf;
#define TMP_LEN   60
   u8 tmp[TMP_LEN];
   u8 *p,*urlbuf;
   u8 *lp;
   u8 i;
   u32 len,blen;
   u8 *CtrlUrlBuf;
   u32 *ip;
   u8 sysip[4];

   memset(UpnpSockSendBuf,0,SocketSendBufLen);
   buf = UpnpSockSendBuf;
   memcpy(buf,"POST ",strlen("POST "));                                         /* POST���� */
   CtrlUrlBuf = (u8 *)&UrlDirBuf[1];//(u8 *)(UpnpFlashAddr + FLASH_LOCAT_CTIRL_URL + 1);/* ����ֱַ��ָ��FLASH�� */
   strcat(buf,CtrlUrlBuf);                                                      /* ����CTRL URL*/
   strcat(buf," HTTP/1.1\r\n");
   strcat(buf,"HOST: ");

   urlbuf = (u8 *)&UrlBuf[1];//(u8 *)(UpnpFlashAddr + FLASH_LOCAT_URL + 1);     /* ����ֱַ��ָ��FLASH�� */

   p = strcasestr(urlbuf,"http://");
   p += sizeof("http://") - 1;
   lp = strcasestr(p,"/");
   i = lp - p;
   memset(tmp,0,TMP_LEN);
   memcpy(tmp,p,i);                                                    
   cprintf("Host:%s\n",tmp);
   strcat(buf,tmp);                                                             /* ����HOST:192.168.1.1:1900 */
   strcat(buf,"\r\n");
   /* ���볤��ֵ*/
   
   len = 607;
   len += strlen(UpnpCtrl.MapPortDes);
   if(UpnpCtrl.UpnpProType == MAP_TYPE_TCP)len += strlen("TCP");
   else len += strlen("UDP");

   memset(tmp,0,TMP_LEN);
   sprintf(tmp,"%d",UpnpCtrl.InternelPort);
   len += strlen(tmp);


   memset(tmp,0,TMP_LEN);
   sprintf(tmp,"%d",UpnpCtrl.ExternalPort);
   len += strlen(tmp);

   memset(tmp,0,TMP_LEN);
   sysip[0] = UpnpCtrl.IPAddr[0];
   sysip[1] = UpnpCtrl.IPAddr[1];
   sysip[2] = UpnpCtrl.IPAddr[2];
   sysip[3] = UpnpCtrl.IPAddr[3];
   CH563NET_InetNtoa(tmp,sysip);
   len += strlen(tmp);
   blen = len;
   cprintf("len len len = %d\n",blen);
   
   strcat(buf,"Content-Length: ");
   len = strlen(buf);

    sprintf(&buf[len],"%d\r\n",blen);
   strcat(buf,"Content-Type: text/xml; charset=\"utf-8\"\r\n");
   strcat(buf,"SOAPACTION: \"urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\"\r\n\r\n");
   len = strlen(buf);
   //cprintf("222222222222222 = %d\n",len);
   CH563NET_SocketSend(socketindex,buf,(u32 *)(&len)); 
   //cprintf("%s\n",buf);
   buf = UpnpSockSendBuf;                                                       /* ��һ���Ѿ���POSTͷ����װ��ϣ���ʼ��װ���� */
   memset(UpnpSockSendBuf,0,SocketSendBufLen);

   memcpy(buf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n",strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"));
   //memcpy(buf,"<s:Envelope\r\n",strlen("<s:Envelope\r\n"));
   //strcat(buf,"<s:Envelope\r\n");
   strcat(buf,"<s:Envelope");
   strcat(buf," xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"");
//   strcat(buf,"    xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"");

   strcat(buf," s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n");
   strcat(buf,"<s:Body>\r\n");
   strcat(buf,"<u:AddPortMapping xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\r\n");
   strcat(buf,"<NewRemoteHost></NewRemoteHost>\r\n");

   strcat(buf,"<NewExternalPort>");
   
   len = strlen(buf);                                                           /* ���㵱ǰ��buf�ַ����ĳ��� */
   sprintf(&buf[len],"%d",UpnpCtrl.ExternalPort);                               /* �ⲿ�˿� */
   strcat(buf,"</NewExternalPort>\r\n");                                        /* �н��� */

   strcat(buf,"<NewProtocol>");
   if(UpnpCtrl.UpnpProType == 0) strcat(buf,"TCP");
   else  strcat(buf,"UDP");

   strcat(buf,"</NewProtocol>\r\n");                                            /* �н��� */
                                                                                
   strcat(buf,"<NewInternalPort>");                                             /* �ڲ��˿� */
   len = strlen(buf);                                                           
   sprintf(&buf[len],"%d",UpnpCtrl.InternelPort);                               /* �ڲ��˿� */
   strcat(buf,"</NewInternalPort>\r\n");                                        /* �н��� */

   strcat(buf,"<NewInternalClient>");                                           /* �ڲ�����IP��ַ */
   sysip[0] = UpnpCtrl.IPAddr[0];
   sysip[1] = UpnpCtrl.IPAddr[1];
   sysip[2] = UpnpCtrl.IPAddr[2];
   sysip[3] = UpnpCtrl.IPAddr[3];
   len = strlen(buf);
   CH563NET_InetNtoa(&buf[len],sysip);
  
   strcat(buf,"</NewInternalClient>\r\n");                                      /* �н��� */

   strcat(buf,"<NewEnabled>1</NewEnabled>\r\n");                                /* ����UPNP�˿�ӳ�䣬Ϊ0��ʾ�ر� */

   strcat(buf,"<NewPortMappingDescription>");                                   /* ӳ��˿�����  */
   strcat(buf,UpnpCtrl.MapPortDes);
   strcat(buf,"</NewPortMappingDescription>\r\n");                              /* �н��� */

   strcat(buf,"<NewLeaseDuration>1600</NewLeaseDuration>\r\n");         
   strcat(buf,"</u:AddPortMapping>\r\n");         
   strcat(buf,"</s:Body>\r\n");         
   strcat(buf,"</s:Envelope>\r\n");         
   strcat(buf,"\r\n");         
   len = strlen(buf);
   blen = len;
   CH563NET_SocketSend(socketindex,buf,(u32 *)(&len));
   if(len == blen)
   {
      cprintf("Send post success\n");
   } 
}

/*******************************************************************************
* Function Name  : UPNP_CreatTcpPostSocket
* Description    : ����POST��socket����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UPNP_CreatTcpPostSocket(void)
{
    u8 i;
    u8 *p,*lp,*urlbuf;
    u8 tmp[60];
    u8 addr[4];
    u16 port;
    SOCK_INF  TmpSocketInf;
    
    cprintf("Creat Post Socket\n");
    //FlashBytes(UpnpFlashAddr,UpnpSockSendBuf,512);
    urlbuf =  (u8 *)&UrlBuf[1];//UpnpSockSendBuf + FLASH_LOCAT_URL + 1;
    p = strcasestr(urlbuf,"http://");                                           /* ͨ��URL���ҵ�IP��ַ */
    p += sizeof("http://") - 1;                                                 /* ��ʽΪHTTP://+IP+:+PORT */
    lp = strcasestr(p,":");                                                     /* ����ð�� */
    i = lp - p;                                                                 /* ���㳤�� */
    memset(tmp,0,20);                                                           /* ����ʱ������ȫ������ */
    memcpy(tmp,p,i);                                                            /* ��IP�ַ������Ƶ���ʱ�������� */
    cprintf("ipstr %s\n",tmp);                                                  
    CH563NET_Aton((const char *)tmp,addr);                                      /* ���ַ���ת��ΪIP��ַ */
    cprintf("IP: %d.%d.%d.%d\n",addr[0],addr[1],addr[2],addr[3]);               /* ��IP���Դ�ӡ */
    lp++;                                                                       
    p = strcasestr(lp,"/");                                                     /* ���Ҷ˿� */
    i = p - lp;                                                                 
    memset(tmp,0,20);                                                           /* ����ʱ������ȫ������ */
    memcpy(tmp,lp,i);                                                           /* ��IP�ַ������Ƶ���ʱ�������� */
    port = atoi(tmp);                                                           /* ����˿� */
    cprintf("Port = %d\n",port);                                                
                                                                                
   memset(&TmpSocketInf,0,sizeof(TmpSocketInf));                                /* ����ʱ��������� */
   memcpy((void *)TmpSocketInf.IPAddr,addr,4);                                  /* ����Ŀ��IP��ַ */
   TmpSocketInf.SourPort = UVCIP_GetPort();                                     /* ����Դ�˿� */
   TmpSocketInf.DesPort = port;                                                 /* ����Ŀ�Ķ˿� */
   TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* Э������ */
   TmpSocketInf.RecvStartPoint = (UINT32)UpnpSockRecvBuf;                       /* ���ý��ջ������Ľ��ջ����� */
   TmpSocketInf.RecvBufLen = SocketRecvBufLen ;                                 /* ���ý��ջ������Ľ��ճ��� */
   i = CH563NET_SocketCreat(&UpnpCtrl.TcpSocket,&TmpSocketInf);
   if(i != CH563NET_ERR_SUCCESS) 
   {
      cprintf("Creat Fail %02x\n",i);
      return;                                                                   /* ����ʧ��ֱ�ӷ��� */
   }
   UpnpCtrl.TcpSocket |= SOCKET_ENABLE_FLAG;                                    /* �����ɹ�����socketΪ��Ч */
   i = CH563NET_SocketConnect(UpnpCtrl.TcpSocket & ~SOCKET_ENABLE_FLAG);
   if(i != CH563NET_ERR_SUCCESS) 
   {
      cprintf("Connect Fail %02x\n",i);
      return;                                                                   /* ����ʧ��ֱ�ӷ��� */
   }
}
/*******************************************************************************
* Function Name  : CH563NET_UPNPStartMapPort
* Description    : APP���Ե��ô˺����������ж˿�ӳ��
* Input          : None
* Output         : None
* Return         : TRUE ʧ�ܣ�FALSE��ʾ�ɹ�
*******************************************************************************/
u8 CH563NET_UPNPStartMapPort(void)
{
    if(UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG)                                 /* ���ܴ�ʱUPNP����TCP socket��ʱ�������ٴ�ʹ���� */
    {                                                               
       if((UpnpCtrl.UpnpMach < DEF_UPNP_POST_CONNECT)|| (UpnpCtrl.UpnpMach > DEF_UPNP_POST_CLOSEING)) /* ��ʱ������GET״̬��GET�����ȼ��Ƚϸߣ������GET״̬�򲻻ᴴ��POST */
        {                                                                       /* �ж��Ƿ���GET״̬ */
           UpnpCtrl.TcpSocket &= ~ SOCKET_ENABLE_FLAG;                          /* ����GET״̬����TCPֱ�Ӹ�λ */
           CH563NET_SocketClose(UpnpCtrl.TcpSocket,1);     
           UpnpCtrl.TcpSocket = 0;
        }
        else return TRUE;  
    }
    if(UpnpCtrl.UpnpFlags & DEF_UPNP_FIND)                                      /* ���UPNP�豸�Ƿ��Ѿ����� */
    {                                                                           /* */
        UPNP_CreatTcpPostSocket();                                              /* ���� POST��SOCKET*/
        UpnpCtrl.TimeCount = UPNP_TIMEOUT_10S;                                  /* 10�볬ʱ */
        UpnpCtrl.UpnpMach = DEF_UPNP_POST_CONNECT;                              /* ����ȴ�POST����״̬ */
        return FALSE;
    }
    else return  TRUE;
}


/*******************************************************************************
* Function Name  : CH563NET_UPNPReceivePostCmd
* Description    : Post������պ�����ѭ�����գ�������Ϻ����UPNP�Ƿ�ɹ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPReceivePostCmd(u8 *buf,u32 len)
{
    
    u8  *p = buf;
    u8  *lp;
    u8  *wp; 
    u16  i;
    u8  fag = 0;

#if 1
    if(UpnpCtrl.RecvTotalLen == 0)                                              /* ���յ��ܳ���Ϊ0����ʾ����������յ����ݳ��� ��Ϊ��һ������ */
    {                                                                           /* ������ܳ��� */
         
         if(p = strcasestr(p, "OK"))
         {
            cprintf("*****************\n");
            cprintf("UPNP Successful\n");
            cprintf("*****************\n");
            if(UpnpCtrl.AppUpnpCallBack)UpnpCtrl.AppUpnpCallBack(ERR_UPNP_SUCCESS);
            UpnpCtrl.MapTime = UpnpCtrl.MaxAgeBack;
         }
         else
         {
            p = buf;
            if(p = strcasestr(p, "Error"))
            {
                UpnpCtrl.MapTime = 0;
                UpnpCtrl.UpnpFlags &= ~DEF_UPNP_FIND;                           /* ����δע��ɹ������豸��� */
                if(UpnpCtrl.AppUpnpCallBack)UpnpCtrl.AppUpnpCallBack(ERR_UPNP_FAIL);
                cprintf("*****************\n");
                cprintf("UPNP Fail\n");
                cprintf("*****************\n");
            }   
         }
         p = buf;
         p = strcasestr(p, "Content-Length:");                                  /* �ҵ������ڲ���־ */
         if(p)                                                                  /* ���㳤��  */
         {                                                                      
             u32 xlen = 0;                                                      
             cprintf("Find Content-Length:\n");                                 
             lp = strcasestr(p, "\r\n");                                        /* �ҵ����н��� */
             p += sizeof("Content-Length:") - 1;                                /* ��ָ��ƫ�Ƶ���ȷλ�� */
             i = *lp;                                                           /* ����*lp */
             *lp = 0;                                                           
             xlen = atoi(p);                                                    
             *lp = i;                                                           /* ��ԭlp */
             cprintf("Pose Receive Cmd = %d\n",xlen);
             UpnpCtrl.RecvTotalLen = xlen;
             cprintf("UpnpCtrl.RecvTotalLen = %d\n",xlen);
             UpnpCtrl.RecvLen = 0;
             p = buf;
             if(p = strcasestr(p,"<?xml version=\"1.0\""))
             {
                 cprintf("find <?xml version=\"1.0\"?>\n");
                 UpnpCtrl.RecvLen = len - (p - buf);
                 UpnpCtrl.pRecv = UpnpTmpBuf; 
                 memcpy(UpnpCtrl.pRecv,p,UpnpCtrl.RecvLen);
                 UpnpCtrl.pRecv += UpnpCtrl.RecvLen;
                 cprintf("First packet len = %d",UpnpCtrl.RecvLen);
             }
         }
         return;
    }
    else
    {
        memcpy(UpnpCtrl.pRecv,buf,len);
        UpnpCtrl.RecvLen += len;
        UpnpCtrl.pRecv += len;
        cprintf("UpnpCtrl.RecvLen = %d\n",UpnpCtrl.RecvLen);
        if(UpnpCtrl.RecvLen < UpnpCtrl.RecvTotalLen)return;                     /* ����δ�����꣬��ȴ����������� */
    }
    UpnpCtrl.UpnpMach = DEF_UPNP_POST_CLOSEING;                                 /* ��Ϊ�ȴ��ر�״̬ */
 #endif
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPTCPConnectHandle
* Description    : ����UPNP��TCP�����¼������ⲿ����
* Input          : socket ����
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTCPConnectHandle(u8 socketid)
{
    switch(UpnpCtrl.UpnpMach)
    {
        case DEF_UPNP_GET_CONNECT:                                              /* GET���ӳɹ� */
             cprintf("CH563NET_UPNPTcpSendGetCommand.....\n");                  
             CH563NET_UPNPTcpSendGetCommand(socketid);                          /* ����GET���� */
             UpnpCtrl.pRecv = UpnpTmpBuf;                                       /* ��ʼ������ָ�� */
             UpnpCtrl.RecvLen = 0;                                              /* ��ʼ�����ճ��� */
             UpnpCtrl.RecvTotalLen = 0;                                         /* ��ʼ�������ܳ��� */
             UpnpCtrl.UpnpMach = DEF_UPNP_GET_RECV;                             /* �ȴ����� GET��Ӧ�� */
             break;                                                                  
       case DEF_UPNP_POST_CONNECT:                                              /* POST���ӳɹ� */
            CH563NET_UPNPInitPostCommand(socketid);                             /* ����UPNPע�� */
            UpnpCtrl.pRecv = UpnpTmpBuf;                                        /* ��ʼ������ָ�� */
            UpnpCtrl.RecvLen = 0;                                               /* ��ʼ�����ճ��� */
            UpnpCtrl.RecvTotalLen = 0;                                          /* ��ʼ�������ܳ��� */
            UpnpCtrl.UpnpMach = DEF_UPNP_POST_RECV;                             /* �ȴ����� POST��Ӧ�� */
            break;
        default:
            break;
    }
}
/*******************************************************************************
* Function Name  : CH563NET_UPNPTCPDisConnectHandle
* Description    : ����UPNP��TCP�Ͽ������¼������ⲿ����,�ɶϿ����߳�ʱ����
* Input          : socket ����
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPTCPDisConnectHandle(u8 socketid)
{
    u8  i;

    switch(UpnpCtrl.UpnpMach)
    {
        case DEF_UPNP_GET_CLOSEING:                                             /* GET�ȴ��رգ�������һ��״̬ */
             UpnpCtrl.TcpSocket &= ~ SOCKET_ENABLE_FLAG;
             i = CH563NET_SocketClose(socketid,0);
             cprintf("Get Socket Close %02x\n",i);
             UpnpCtrl.TimeCount = 0;                                            /*�ر�����ʱ�����ʱ��stop timer */
             UpnpCtrl.TcpSocket = 0;
             UpnpCtrl.UpnpMach = DEF_UPNP_START;
             CH563NET_UPNPStartMapPort();                                       /* ����ӳ�� */
        break;
        case DEF_UPNP_POST_CLOSEING:                                            /* POST�ȴ��Ͽ� */
             i = CH563NET_SocketClose(socketid,0);
             UpnpCtrl.UpnpMach = DEF_UPNP_START;
             UpnpCtrl.TcpSocket = 0;
             cprintf("Post connect close \n");
             UpnpCtrl.TcpSocket = 0;
             UpnpCtrl.TimeCount = 0;                                            /*stop timer */
            break;
        default:
             CH563NET_SocketClose(socketid,1);
             if(UpnpCtrl.AppUpnpCallBack)UpnpCtrl.AppUpnpCallBack(ERR_UPNP_TCP_CONNECT);
             UpnpCtrl.TcpSocket = 0;
             UpnpCtrl.TimeCount = 0;
             UpnpCtrl.MapTime = 0;                                              /*stop timer */
             UpnpCtrl.UpnpFlags &= DEF_UPNP_FIND;                               /* ���������������ӱ��رգ���UPNP��� */
        break;
    }
}

/*******************************************************************************
* Function Name  : CH563NET_UPNPReceiveDataHandle
* Description    : �������ݴ����������ⲿ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH563NET_UPNPReceiveDataHandle(u8 socket)
{
    u32 rlen;
    u32 addr;

    if((UpnpCtrl.UdpSocket & SOCKET_ENABLE_FLAG)&&\
      (socket == (UpnpCtrl.UdpSocket & ~SOCKET_ENABLE_FLAG)))
    {
        rlen = CH563NET_SocketRecvLen(socket,&addr);
        CH563NET_UPNPRecvMultiBrocast(socket & ~SOCKET_ENABLE_FLAG,(u8 *)addr);
        CH563NET_SocketRecv(socket,NULL,&rlen);
    }
    else if((UpnpCtrl.TcpSocket & SOCKET_ENABLE_FLAG)&&\
            (socket == (UpnpCtrl.TcpSocket & ~SOCKET_ENABLE_FLAG)))
    {
        rlen = CH563NET_SocketRecvLen(socket,&addr);
        switch(UpnpCtrl.UpnpMach)
        {
            case DEF_UPNP_GET_RECV:
                CH563NET_UPNPReceiveGetCmd((u8 *)addr,rlen);
                CH563NET_SocketRecv(socket,NULL,&rlen);
            break;
            case DEF_UPNP_POST_RECV:
                cprintf("Post\n");
                CH563NET_UPNPReceivePostCmd((u8 *)addr,rlen);
                CH563NET_SocketRecv(socket,NULL,&rlen);
            break;
        }
    }
    else
    {
        rlen = CH563NET_SocketRecvLen(socket,&addr);
        CH563NET_SocketRecv(socket,NULL,&rlen);
    }
}

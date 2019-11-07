/********************************** (C) COPYRIGHT *******************************
* File Name          :Compound_Dev.C
* Author             : WCH
* Version            : V1.3
* Date               : 2016/1/15
* Description        : CH559模拟CH9326设备
*******************************************************************************/
#include "DEBUG.C"                                                       //调试信息打印
#include "DEBUG.H"
#include <string.h>

#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#define DEF_BUF_LENGTH          0x40
#define DEF_THIS_IC_VER         0x13

UINT8X  Ep0Buffer[8<(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;    //端点0 OUT&IN缓冲区，必须是偶地址
UINT8X  Ep2Buffer[128<(2*MAX_PACKET_SIZE+4)?128:(2*MAX_PACKET_SIZE+4)] _at_ 0x0010;//端点2 IN&OUT缓冲区,必须是偶地址
UINT8   SetupReq,SetupLen,Ready,Count,FLAG,UsbConfig;
PUINT8  pDescr;                                                                //USB配置标志
USB_SETUP_REQ   SetupReqBuf;                                                   //暂存Setup包
#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)
#define DEBUG 0
sbit Ep2InKey = P1^0;                                                          //K3按键
#pragma  NOAREGS
/*设备描述符*/
UINT8C DevDesc[18] = {0x12,0x01,0x00,0x01,0x00,0x00,0x00,0x08,
                      0x96,0x1A,0x10,0xE0,0x00,DEF_THIS_IC_VER,0x01,0x02,
                      0x03,0x01
                     };
UINT8C CfgDesc[41] ={
    0x09,0x02,0x29,0x00,0x01,0x01,0x04,0x80,0x32,             //配置描述符
    0x09,0x04,0x00,0x00,0x02,0x03,0x00,0x00,0x00,             //接口描述符,HID
    0x09,0x21,0x00,0x01,0x00,0x01,0x22,0x25,0x00,             //HID类描述符
    0x07,0x05,0x82,0x03,DEF_BUF_LENGTH,0x00,0x01,             //端点描述符
    0x07,0x05,0x02,0x03,DEF_BUF_LENGTH,0x00,0x01,             //端点描述符
};
/*字符串描述符*/
// 语言描述符
UINT8C  LangDesc[] = { 0x04, 0x03, 0x09, 0x04 };
UINT8C ProDesc[28] ={                                         //产品信息字符串描述符
    0x1C,0x03,'H',0x00,'I',0x00,'D',0x00,' ',0x00,
    'T',0x00,'o',0x00,' ',0x00,'S',0x00,'e',0x00,
    'r',0x00,'i',0x00,'a',0x00,'l',0x00
};
UINT8C ManuDesc[18] ={                                        //厂商信息字符串描述符
    0x12,0x03,'W',0x00,'C',0x00,'H',0x00,'.',0x00,
    'C',0x00,'N',0x00,' ',0x00,DEF_THIS_IC_VER,0x00
};
UINT8C SNDesc[18] ={                                          //设备序列号信息字符串描述符
    0x12,0x03,'1',0x00,'2',0x00,'3',0x00,'4',0x00,
    '5',0x00,'6',0x00,'7',0x00,'8',0x00
};
/*HID类报表描述符*/
UINT8C HIDRepDesc[37] =
{
    0x06, 0xA0,0xff,
    0x09, 0x01,
    0xa1, 0x01,                                                   //集合开始
    0x09, 0x01,                                                   //Usage Page  用法
    0x15, 0x00,                                                   //Logical  Minimun
    0x26, 0xFF,0x00,                                              //Logical  Maximun
    0x75, 0x08,                                                   //Report Size
    0x95, DEF_BUF_LENGTH,                                         //Report Counet
    0x81, 0x02,                                                   //Input
    0x09, 0x02,                                                   //Usage Page  用法
    0x75, 0x08,                                                   //Report Size
    0x95, DEF_BUF_LENGTH,                                         //Report Counet
    0x91, 0x02,                                                   //Output
    0x09, 0x03,
    0x75, 0x08,
    0x95, 0x05,
    0xB1, 0x02,
    0xC0
};
UINT8X UserEp2Buf[64] _at_ 0x0090;
/*******************************************************************************
* Function Name  : USBDeviceCfg()
* Description    : USB设备模式配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceCfg()
{
    USB_CTRL = 0x00;                                                           //清空USB控制寄存器
    USB_CTRL &= ~bUC_HOST_MODE;                                                //该位为选择设备模式
    USB_CTRL |=  bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                    //USB设备和内部上拉使能,在中断期间中断标志未清除前自动返回NAK
    USB_DEV_AD = 0x00;                                                         //设备地址初始化
    UDEV_CTRL &= ~bUD_RECV_DIS;                                                //使能接收器
//     USB_CTRL |= bUC_LOW_SPEED;
//     UDEV_CTRL |= bUD_LOW_SPEED;                                                //选择低速1.5M模式
    USB_CTRL &= ~bUC_LOW_SPEED;
    UDEV_CTRL &= ~bUD_LOW_SPEED;                                             //选择全速12M模式，默认方式
    UDEV_CTRL |= bUD_DP_PD_DIS | bUD_DM_PD_DIS;                                //禁止DM、DP下拉电阻
    UDEV_CTRL |= bUD_PORT_EN;                                                  //使能物理端口
}
/*******************************************************************************
* Function Name  : USBDeviceIntCfg()
* Description    : USB设备模式中断初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceIntCfg()
{
    USB_INT_EN |= bUIE_SUSPEND;                                               //使能设备挂起中断
    USB_INT_EN |= bUIE_TRANSFER;                                              //使能USB传输完成中断
    USB_INT_EN |= bUIE_BUS_RST;                                               //使能设备模式USB总线复位中断
    USB_INT_FG |= 0x1F;                                                       //清中断标志
    IE_USB = 1;                                                               //使能USB中断
    EA = 1;                                                                   //允许单片机中断
}
/*******************************************************************************
* Function Name  : USBDeviceEndPointCfg()
* Description    : USB设备模式端点配置，模拟兼容HID设备，除了端点0的控制传输，还包括端点2批量上下传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceEndPointCfg()
{
    UEP2_DMA = Ep2Buffer;                                                      //端点2数据传输地址
    UEP2_3_MOD |= bUEP2_TX_EN;                                                 //端点2发送使能
    UEP2_3_MOD |= bUEP2_RX_EN;                                                 //端点2接收使能
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;                                              //端点2收发各64字节缓冲区
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
    UEP0_DMA = Ep0Buffer;                                                      //端点0数据传输地址
    UEP4_1_MOD &= ~(bUEP4_RX_EN | bUEP4_TX_EN);                                //端点0单64字节收发缓冲区
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT事务返回ACK，IN事务返回NAK
}
/*******************************************************************************
* Function Name  : enp2BlukIn()
* Description    : USB设备模式端点2的批量上传
* Input          : UINT8 len
* Output         : None
* Return         : None
*******************************************************************************/
void enp2BlukIn(UINT8 len )
{
    Ep2Buffer[MAX_PACKET_SIZE] = len;
    memcpy( Ep2Buffer+MAX_PACKET_SIZE+1, UserEp2Buf, sizeof(UserEp2Buf));      //加载上传数据
    UEP2_T_LEN = sizeof(UserEp2Buf);                                           //上传数据长度
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                  //有数据时上传数据并应答ACK
}
/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB中断处理函数
*******************************************************************************/
void    DeviceInterrupt( void ) interrupt INT_NO_USB using 1                      //USB中断服务程序,使用寄存器组1
{
    UINT8 len,i;
#if DEBUG
    printf("%02X ",(UINT16)USB_INT_FG);
#endif
    if(UIF_TRANSFER)                                                            //USB传输完成标志
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
        {
        case UIS_TOKEN_IN | 2:                                                  //endpoint 2# 端点批量上传
            UEP2_T_LEN = 0;                                                    //预使用发送长度一定要清空
//            UEP1_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            break;
        case UIS_TOKEN_OUT | 2:                                                 //endpoint 2# 端点批量下传
//             if ( U_TOG_OK )                                                     // 不同步的数据包将丢弃
//             {
//                 len = USB_RX_LEN;                                                 //接收数据长度，数据从Ep2Buffer首地址开始存放
//                 for ( i = 0; i < len; i ++ )
//                 {
//                     Ep2Buffer[MAX_PACKET_SIZE+i] = Ep2Buffer[i] ^ 0xFF;           // OUT数据取反到IN由计算机验证
//                 }
//                 UEP2_T_LEN = len;
//                 UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;         // 允许上传
//             }
            break;
        case UIS_TOKEN_SETUP | 0:                                                //SETUP事务
            len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                if(UsbSetupBuf->wLengthH || SetupLen > 0x7F )
                {
                    SetupLen = 0x7F;    // 限制总长度
                }
                len = 0;                                                      // 默认为成功并且上传0长度
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )/* 只支持标准请求 */
                {
                    len = 0xFF;                                            // 操作失败
                    printf("ErrEp0ReqType=%02X\n",(UINT16)UsbSetupBuf->bRequestType);
                }
                else
                {
                    //标准请求
                    SetupReq = UsbSetupBuf->bRequest;
#if DEBUG
                    printf("REQ %02X ",(UINT16)SetupReq);
#endif
                    switch(SetupReq)                                                  //请求码
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                                       //设备描述符
                            pDescr = DevDesc;                                         //把设备描述符送到要发送的缓冲区
                            len = sizeof(DevDesc);
                            break;
                        case 2:                                                        //配置描述符													
                            pDescr = CfgDesc;                                          //把设备描述符送到要发送的缓冲区
                            len = sizeof(CfgDesc);
                            break;
                        case 3:                                                        //字符串描述符
														switch( UsbSetupBuf->wValueL )
														{
														case 0:
																pDescr = (PUINT8)( &LangDesc[0] );
																len = sizeof( LangDesc );
																break;															
														case 1:
																pDescr = (PUINT8)( &ManuDesc[0] );
																len = sizeof( ManuDesc );
																break;
														case 2:
																pDescr = (PUINT8)( &ProDesc[0] );
																len = sizeof( ProDesc );
																break;
														case 3:
																pDescr = (PUINT8)( &SNDesc[0] );
																len = sizeof( SNDesc );
																break;
														default:
																len = 0xFF;  // 不支持的字符串描述符
																break;
														}
                            break;												
                        case 0x22:                                                     //报表描述符
#if DEBUG
                            printf("RREQ %02X ",(UINT16)SetupReq);
#endif
                            pDescr = HIDRepDesc;                                       //数据准备上传
                            len = sizeof(HIDRepDesc);
                            Ready = 1;                                                 //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
                            break;											
                        default:
                            len = 0xff;                                                //不支持的命令或者出错
                            break;
                        }
                        if ( SetupLen > len )
                        {
                            SetupLen = len;    //限制总长度
                        }
                        len = SetupLen >= 8 ? 8 : SetupLen;                            //本次传输长度
                        memcpy(Ep0Buffer,pDescr,len);                                  //加载上传数据
                        SetupLen -= len;
                        pDescr += len;
                        break;
                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                              //暂存USB设备地址
                        break;
                    case USB_GET_CONFIGURATION:
                        Ep0Buffer[0] = UsbConfig;
                        if ( SetupLen >= 1 )
                        {
                            len = 1;
                        }
                        break;
                    case USB_SET_CONFIGURATION:
                        UsbConfig = UsbSetupBuf->wValueL;
                        break;
                    case 0x0A:
                        break;
                    case USB_CLEAR_FEATURE:                                            //Clear Feature
                        if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
                        {
                            switch( UsbSetupBuf->wIndexL )
                            {
                            case 0x82:
                                UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x81:
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x02:
                                UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
                                break;
                            default:
                                len = 0xFF;                                         // 不支持的端点
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                                // 不是端点不支持
                        }
                        break;
                    case USB_SET_FEATURE:                                          /* Set Feature */
                        if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )                  /* 设置设备 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
                            {
                                if( CfgDesc[ 7 ] & 0x20 )
                                {
                                    /* 设置唤醒使能标志 */
                                }
                                else
                                {
                                    len = 0xFF;                                        /* 操作失败 */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                            /* 操作失败 */
                            }
                        }
                        else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )             /* 设置端点 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
                            {
                                switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                                    /* 操作失败 */
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                      /* 操作失败 */
                            }
                        }
                        else
                        {
                            len = 0xFF;                                          /* 操作失败 */
                        }
                        break;
                    case USB_GET_STATUS:
                        Ep0Buffer[0] = 0x00;
                        Ep0Buffer[1] = 0x00;
                        if ( SetupLen >= 2 )
                        {
                            len = 2;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                        break;
                    default:
                        len = 0xff;                                                    //操作失败
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                         //包长度错误
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= 8)                                                       //上传数据或者状态阶段返回0长度包
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
#if DEBUG
                printf("S_U\n");
#endif
            }
            else
            {
                UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
            }
            break;
        case UIS_TOKEN_IN | 0:                                                      //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
                len = SetupLen >= 8 ? 8 : SetupLen;                                 //本次传输长度
                memcpy( Ep0Buffer, pDescr, len );                                   //加载上传数据
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                             //同步标志位翻转
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                                      //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
            break;
        case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
            len = USB_RX_LEN;
            if(SetupReq == 0x09)
            {
                if(Ep0Buffer[0])
                {
                    printf("Light on Num Lock LED!\n");
                }
                else if(Ep0Buffer[0] == 0)
                {
                    printf("Light off Num Lock LED!\n");
                }
            }
            UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA0,返回应答ACK
            break;
        default:
            break;
        }
        UIF_TRANSFER = 0;                                                           //写0清空中断
    }
    if(UIF_BUS_RST)                                                                 //设备模式USB总线复位中断
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;                                                             //清中断标志
    }
    if (UIF_SUSPEND)                                                                 //USB总线挂起/唤醒完成
    {
        UIF_SUSPEND = 0;
        if ( USB_MIS_ST & bUMS_SUSPEND )                                             //挂起
        {
#if DEBUG
            printf( "zz" );                                                             //睡眠状态
#endif
            while ( XBUS_AUX & bUART0_TX )
            {
                ;    //等待发送完成
            }
            SAFE_MOD = 0x55;
            SAFE_MOD = 0xAA;
            WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO;                                     //USB或者RXD0有信号时可被唤醒
            PCON |= PD;                                                                 //睡眠
            SAFE_MOD = 0x55;
            SAFE_MOD = 0xAA;
            WAKE_CTRL = 0x00;
        }
    }
    else {                                                                             //意外的中断,不可能发生的情况
        USB_INT_FG = 0xFF;                                                             //清中断标志
//      printf("UnknownInt  N");
    }
}
main()
{
    UINT8 i,len = 5;
    CfgFsys( );                                                           //CH559时钟选择配置
    mDelaymS(30);                                                         //上电延时,等待内部晶振稳定,必加	
    mInitSTDIO( );                                                        //串口0,可以用于调试
    printf("start ...\n");
    memset(UserEp2Buf,0,sizeof(UserEp2Buf));
    USBDeviceCfg();                                                       //模拟鼠标
    USBDeviceEndPointCfg();                                               //端点配置
    USBDeviceIntCfg();                                                    //中断初始化
    UEP1_T_LEN = 0;                                                       //预使用发送长度一定要清空
    UEP2_T_LEN = 0;                                                       //预使用发送长度一定要清空
    FLAG = 0;
    Ready = 0;
    while(1)
    {
        if(Ready&& (Ep2InKey==0))
        {
            enp2BlukIn(len);                                              //要上传的数据包长度，不超过64字节
            mDelaymS( 100 );
        }
        mDelaymS( 100 );                                                  //模拟单片机做其它事
    }
}

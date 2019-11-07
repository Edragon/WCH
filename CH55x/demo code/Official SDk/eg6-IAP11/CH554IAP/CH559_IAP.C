
/********************************** (C) COPYRIGHT ******************************
* File Name          : CH559_IAP.C
* Author             : WCH
* Version            : V1.3
* Date               : 2016/06/24
* Description        : IAP 功能演示例子程序
*                      1，支持串口下载，串口号为0，波特率为57600，由于采用内部晶振，晶振存在误差，所以增加串口累加和，累加和错误进行重发
*                      2，支持USB下载，USB为全速设备
                       3，支持EEPROM编程
                       4，支持芯片型号判断
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <intrins.h>
#include "../../public/CH554.H"
#include "CH559_IAP.H"

sbit DisableIAP            = P1^0;                     //返回用户程序检测引脚
#define IAP_CODE_ADDR        (0x3000)             //1k的整数倍，因为55X的Flash一次至少擦1K
#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE     //默认端点0的大小

// 设备描述符
UINT8C    MyDevDescr[] = { 0x12, 0x01, 0x10, 0x01,
                           0xFF, 0x80, 0x55, THIS_ENDP0_SIZE,
                           0x48, 0x43, 0xe0, 0x55,
                           0x00, 0x01, 0x00, 0x00,0x00, 0x01
                         };
// 配置描述符
UINT8C    MyCfgDescr[] = { 0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
                           0x09, 0x04, 0x00, 0x00, 0x02, 0xFF, 0x80, 0x55, 0x00,
                           0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,
                           0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00
                         };
UINT8    UsbConfig = 0;                                // USB配置标志
UINT8X    Ep0Buffer[THIS_ENDP0_SIZE] _at_ 0x0000 ;     // OUT&IN, must even address
UINT8X    Ep2Buffer[2*MAX_PACKET_SIZE] _at_ 0x0008 ;   // OUT+IN, must even address
#define   UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)

iap_cmd1 xdata iap_cmd  _at_ 0x0088;                   //IAP命令
UINT8   uart_bit;                                      //下载方式全局标志位，1表示为串口，2表示为USB口
UINT16  chip_id,eeprom_len;
PUINT8C    pCode;

#pragma NOAREGS


/*******************************************************************************
* Function Name  : ProgWord
* Description    : 芯片编程函数
* Input          : Addr    芯片编程地址地址，地址为偶数地址
                   Data    编程数据，以WORD为基准
* Output         : None
* Return         :         芯片编程返回状态
                    0x00   编程成功
                    0x01   编程超时
                    0x02   未知错误，编程失败
*******************************************************************************/
UINT8 ProgWord( UINT16 Addr, UINT16 Data )
{
	ROM_ADDR = Addr;
	ROM_DATA = Data;
	if ( ROM_STATUS & bROM_ADDR_OK ) {                                           // 操作地址有效
		ROM_CTRL = ROM_CMD_WRITE;
		return( ( ROM_STATUS ^ bROM_ADDR_OK ) & 0x7F );                            // 返回状态,0x00=success, 0x01=time out(bROM_CMD_TOUT), 0x02=unknown command(bROM_CMD_ERR)
	}
	else return( 0x40 );
}
/*******************************************************************************
* Function Name  : FlashVerify
* Description    : Flash校验
* Input          : Addr    芯片编程地址地址，地址为偶数地址
                   pData   编程数据，以WORD为基准
                   len     校验长度
* Output         : None
* Return         :         返回校验状态
                    0x00   校验成功
                    0xff   校验失败
*******************************************************************************/
UINT8 FlashVerify( UINT16 Addr, UINT8 *pData, UINT16 len )
{
    UINT16 i;
    pCode = (PUINT8C)( Addr );
    for( i=0; i!=len; i++ )
    {
        if( *pData != *pCode )
        {
            return 0xff;
        }
        pCode++;
        pData++;
    }
    return 0;
}

/*******************************************************************************
* Function Name  : CH55X_Respond
* Description    : IAP升级时，芯片应答函数
* Input          : s 有效应答字节
* Output         : None
* Return         : SBUF 串口接收字节
*******************************************************************************/
void CH55X_Respond( UINT8 s )
{
        Ep2Buffer[ MAX_PACKET_SIZE ]   = s;
        Ep2Buffer[ MAX_PACKET_SIZE+1 ] = 0X00;
        UEP2_T_LEN = 2;
        UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;  // 允许上传
}
/*******************************************************************************
* Function Name  : CH559_USB_ISPDownload
* Description    : CH559下载函数
*                ：
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH55X_IAPDownload( void )
{
    UINT8  s;
    UINT16 i;
    UINT16 len,Data;
    UINT32 addr;
    switch( iap_cmd.other.buf[0] )                                  // 分析命令码
    {
    case CMD_IAP_PROM:                                              // ISP 编程命令
        len = iap_cmd.program.len>>1;                               //必须为2的整数倍，按照半字进行操作
        addr = (iap_cmd.program.addr[0] | (UINT16)iap_cmd.program.addr[1]<<8);
        SAFE_MOD = 0x55;
        SAFE_MOD = 0xAA;
        GLOBAL_CFG |= bCODE_WE | bDATA_WE;                          //写Flash
        for( i=0; i!=len; i++ )
        {
            Data = (iap_cmd.program.buf[2*i] | (UINT16)iap_cmd.program.buf[2*i+1]<<8);
            s = ProgWord( addr,Data );
            addr+=2;
            if( s != 0x00 )
            {
                break;
            }
        }
        SAFE_MOD = 0x55;
        SAFE_MOD = 0xAA;
        GLOBAL_CFG &= ~ ( bCODE_WE | bDATA_WE );
        CH55X_Respond( s );                                         //返回校验
        break;
    case CMD_IAP_ERASE:                                             // ISP 擦除命令
        CH55X_Respond( 0 );
        break;
    case CMD_IAP_VERIFY:                                             // ISP 校验命令
        addr = (iap_cmd.verify.addr[0] | (UINT16)iap_cmd.verify.addr[1]<<8);
        len = iap_cmd.verify.len>>1;                                 // 必须为2的整数倍，按照字进行操作
        s = FlashVerify( addr,&(iap_cmd.verify.buf[0]),iap_cmd.verify.len );
        CH55X_Respond( s );
        break;
    case CMD_IAP_END:                                                // ISP 结束命令
        SAFE_MOD = 0x55;
        SAFE_MOD = 0xAA;
        GLOBAL_CFG |= bSW_RESET;                                     // 复位单片机,进入用户程序 
        break;
    default:
        CH55X_Respond( 0xfe );                                       // 未知的命令
        break;
    }
}
/*******************************************************************************
* Function Name  : USB_DeviceInterrupt
* Description    : USB中断查询函数，IAP程序无法使用中断
*                ：
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_DeviceInterrupt( void )
{
    UINT8    len;
    static    UINT8    SetupReqCode, SetupLen;
    static    PUINT8    pDescr;
    if( UIF_TRANSFER )                                                // USB传输完成
    {
        if ( U_IS_NAK ){}                                             // 本例子可以不必处理NAK
        else
        {
            switch ( USB_INT_ST & ( MASK_UIS_TOKEN | MASK_UIS_ENDP ) )// 分析操作令牌和端点号
            {
            case UIS_TOKEN_OUT | 2:                                   // endpoint 2# 批量端点下传
                if ( U_TOG_OK )                                       // 不同步的数据包将丢弃
                {
                    len = USB_RX_LEN;
                    memcpy( iap_cmd.other.buf,Ep2Buffer,len );
                    uart_bit = 2;
                    CH55X_IAPDownload( );
                }
                break;
            case UIS_TOKEN_IN | 2:                                    // endpoint 2# 批量端点上传
                UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;// 暂停上传
                break;
            case UIS_TOKEN_IN | 1:                                    // endpoint 1# 中断端点上传
                UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;// 暂停上传
                break;
            case UIS_TOKEN_SETUP | 0:                                 // endpoint 0# SETUP
                len = USB_RX_LEN;
                if ( len == sizeof( USB_SETUP_REQ ) )                 // SETUP包长度
                {
                    SetupLen = UsbSetupBuf->wLengthL;
                    if ( UsbSetupBuf->wLengthH || SetupLen > 0x7F )
                    {
                        SetupLen = 0x7F;                              // 限制总长度
                    }
                    len = 0;                                          // 默认为成功并且上传0长度
                    if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )//只支持标准请求
                    {
                        len = 0xFF;                                   // 操作失败
                    }
                    else                                              // 标准请求
                    {
                        SetupReqCode = UsbSetupBuf->bRequest;
                        switch( SetupReqCode )                        // 请求码
                        {
                        case USB_GET_DESCRIPTOR:
                            switch( UsbSetupBuf->wValueH )
                            {
                            case 1:                                   // 设备描述符
                                pDescr = (PUINT8)( &MyDevDescr[0] );
                                len = sizeof( MyDevDescr );
                                break;
                            case 2:                                   // 配置描述符
                                pDescr = (PUINT8)( &MyCfgDescr[0] );
                                len = sizeof( MyCfgDescr );
                                break;
                            default:
                                len = 0xFF;                           // 不支持的描述符类型
                                break;
                            }
                            if ( SetupLen > len )
                            {
                                SetupLen = len;                       // 限制总长度
                            }
                            len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;// 本次传输长度
                            memcpy( Ep0Buffer, pDescr, len );         // 加载上传数据 
                            SetupLen -= len;
                            pDescr += len;
                            break;
                        case USB_SET_ADDRESS:
                            SetupLen = UsbSetupBuf->wValueL;          // 暂存USB设备地址
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
                        default:
                            len = 0xFF;                               // 操作失败
                            break;
                        }
                    }
                }
                else
                {
                    len = 0xFF;                                       // SETUP包长度错误
                }
                if ( len == 0xFF )                                    // 操作失败
                {
                    SetupReqCode = 0xFF;
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;// STALL
                }
                else if ( len <= THIS_ENDP0_SIZE )                    // 上传数据或者状态阶段返回0长度包
                {
                    UEP0_T_LEN = len;
                    UEP0_CTRL  = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;// 默认数据包是DATA1
                }
                else                                                  // 下传数据或其它
                {
                    UEP0_T_LEN = 0;                                   // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;// 默认数据包是DATA1
                }
                break;
            case UIS_TOKEN_IN | 0:                                    // endpoint 0# IN
                switch( SetupReqCode )
                {
                case USB_GET_DESCRIPTOR:
                    len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;// 本次传输长度
                    memcpy( Ep0Buffer, pDescr, len );                 // 加载上传数据 
                    SetupLen -= len;
                    pDescr += len;
                    UEP0_T_LEN = len;
                    UEP0_CTRL ^= bUEP_T_TOG;                          // 翻转
                    break;
                case USB_SET_ADDRESS:
                    USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                    break;
                default:
                    UEP0_T_LEN = 0;                                   // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                    break;
                }
                break;
            case UIS_TOKEN_OUT | 0:                                   // endpoint 0# OUT
                switch( SetupReqCode )
                {
                default:
                    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;        // 准备下一控制传输
                    break;
                }
                break;
            default:
                break;
            }
        }
        UIF_TRANSFER = 0;                                             // 清中断标志
    }
    else if ( UIF_BUS_RST )                                           // USB总线复位
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;                                              // 清中断标志
    }
    else if ( UIF_SUSPEND )                                           // USB总线挂起/唤醒完成
    {
        UIF_SUSPEND = 0;
    }
    else                                                              // 意外的中断,不可能发生的情况
    {
        USB_INT_FG = 0xFF;                                            // 清中断标志
    }
}

/*******************************************************************************
* Function Name  : InitUSB_Device()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void	InitUSB_Device( void )                                      // 初始化USB设备
{
	IE_USB = 0;
	USB_CTRL = 0x00;                                                // 先设定模式
	UEP4_1_MOD = bUEP1_TX_EN;                                       // 端点1上传IN
	UEP2_3_MOD = bUEP2_RX_EN | bUEP2_TX_EN;                         // 端点2下传OUT和上传IN
	UEP0_DMA = Ep0Buffer;
	UEP2_DMA = Ep2Buffer;
	UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
	UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
	USB_DEV_AD = 0x00;
	UDEV_CTRL = bUD_PD_DIS;                                         // 禁止DP/DM下拉电阻
	USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;           // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
	UDEV_CTRL |= bUD_PORT_EN;                                       // 允许USB端口
	USB_INT_FG = 0xFF;                                              // 清中断标志
	USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
} 
/*******************************************************************************
* Function Name  : mDelay20us(UNIT16 n)
* Description    : 20us延时函数，主频12MHz，延时不准，其他主频参考DEBUG.C的延时函数
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void mDelay20us( UINT16 n )
{
    for( n <<= 3; n; --n )
    {
        _nop_( );
    }
}

/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void main( void )
{
    UINT16 i=0,j;
    EA = 0;                                                              // 关闭中断，IAP方式无法使用中断
    uart_bit = 0;
    InitUSB_Device( );                                                   /* USB设备模式初始化函数，查询方式 */
    while(1)
    {
        i++;
        j++;
        if( j > 30 )
        {
            j = 0;
            if( USB_INT_FG )
            {
                USB_DeviceInterrupt( );                                  // 查询usb中断,建议不要频繁查询
            }
        }
        if( i == 20000 )
        {
            i = 0;
        }
        if( i == 0 )                                                     //仅仅是点灯指示，无意义
        {
        }
        if( i == 10000 )
        {
                                           //闪灯
        }
        mDelay20us(1);                                                    // 延时
        /* 退出iap下载 */
        if( DisableIAP == 0 )                                             // 查询P10低电平时执行软复位，从新执行用户程序
        {
            SAFE_MOD = 0x55;
            SAFE_MOD = 0xAA;
            GLOBAL_CFG |= bSW_RESET;                                      //软件复位单片机,进入用户程序 */
        }
    }
}

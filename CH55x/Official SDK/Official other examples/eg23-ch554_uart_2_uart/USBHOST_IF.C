
/********************************** (C) COPYRIGHT *******************************
* File Name          : USBHOST_IF.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 USB 主机接口函数
*******************************************************************************/

#include "CH554.H"                                                          //调试信息打印
#include "DEBUG.H"
#include "stdio.h"
#include "string.h"
#include "USBHOST_IF.H"

#define CH34x_VENDOR_ID_O		0x4348
#define CH34x_VENDOR_ID_N		0x1A86
#define CH340_PRODUCT_ID	    0x7523
#define CH341_PRODUCT_ID	    0x5523

#define PL2303_VENDOR_ID		0x067B
#define PL2303_PRODUCT_ID	    0x2303

//#define CP2102_VENDOR_ID		0x10C4
//#define CP2102_PRODUCT_ID	    0xEA60


const UINT16 CH34X_ID_Table[]={CH34x_VENDOR_ID_O,CH340_PRODUCT_ID,
								CH34x_VENDOR_ID_O,CH341_PRODUCT_ID,
								CH34x_VENDOR_ID_N,CH340_PRODUCT_ID,
								CH34x_VENDOR_ID_N,CH341_PRODUCT_ID,};

const UINT16 PL2303_ID_Table[]={PL2303_VENDOR_ID,PL2303_PRODUCT_ID,};								

const UINT16 CP210x_ID_Table[]={
	 0x045B, 0x0053,   /* Renesas RX610 RX-Stick */
	 0x0471, 0x066A , /* AKTAKOM ACE-1001 cable */
	 0x0489, 0xE000 , /* Pirelli Broadband S.p.A, DP-L10 SIP/GSM Mobile */
	 0x0489, 0xE003 , /* Pirelli Broadband S.p.A, DP-L10 SIP/GSM Mobile */
	 0x0745, 0x1000 , /* CipherLab USB CCD Barcode Scanner 1000 */
	 0x0846, 0x1100 , /* NetGear Managed Switch M4100 series, M5300 series, M7100 series */
	 0x08e6, 0x5501 , /* Gemalto Prox-PU/CU contactless smartcard reader */
	 0x08FD, 0x000A , /* Digianswer A/S , ZigBee/802.15.4 MAC Device */
	 0x0BED, 0x1100 , /* MEI (TM) Cashflow-SC Bill/Voucher Acceptor */
	 0x0BED, 0x1101 , /* MEI series 2000 Combo Acceptor */
	 0x0FCF, 0x1003 , /* Dynastream ANT development board */
	 0x0FCF, 0x1004 , /* Dynastream ANT2USB */
	 0x0FCF, 0x1006 , /* Dynastream ANT development board */
	 0x0FDE, 0xCA05 , /* OWL Wireless Electricity Monitor CM-160 */
	 0x10A6, 0xAA26 , /* Knock-off DCU-11 cable */
	 0x10AB, 0x10C5 , /* Siemens MC60 Cable */
	 0x10B5, 0xAC70 , /* Nokia CA-42 USB */
	 0x10C4, 0x0F91 , /* Vstabi */
	 0x10C4, 0x1101 , /* Arkham Technology DS101 Bus Monitor */
	 0x10C4, 0x1601 , /* Arkham Technology DS101 Adapter */
	 0x10C4, 0x800A , /* SPORTident BSM7-D-USB main station */
	 0x10C4, 0x803B , /* Pololu USB-serial converter */
	 0x10C4, 0x8044 , /* Cygnal Debug Adapter */
	 0x10C4, 0x804E , /* Software Bisque Paramount ME build-in converter */
	 0x10C4, 0x8053 , /* Enfora EDG1228 */
	 0x10C4, 0x8054 , /* Enfora GSM2228 */
	 0x10C4, 0x8066 , /* Argussoft In-System Programmer */
	 0x10C4, 0x806F , /* IMS USB to RS422 Converter Cable */
	 0x10C4, 0x807A , /* Crumb128 board */
	 0x10C4, 0x80C4 , /* Cygnal Integrated Products, Inc., Optris infrared thermometer */
	 0x10C4, 0x80CA , /* Degree Controls Inc */
	 0x10C4, 0x80DD , /* Tracient RFID */
	 0x10C4, 0x80F6 , /* Suunto sports instrument */
	 0x10C4, 0x8115 , /* Arygon NFC/Mifare Reader */
	 0x10C4, 0x813D , /* Burnside Telecom Deskmobile */
	 0x10C4, 0x813F , /* Tams Master Easy Control */
	 0x10C4, 0x814A , /* West Mountain Radio RIGblaster P&P */
	 0x10C4, 0x814B , /* West Mountain Radio RIGtalk */
	 0x2405, 0x0003 , /* West Mountain Radio RIGblaster Advantage */
	 0x10C4, 0x8156 , /* B&G H3000 link cable */
	 0x10C4, 0x815E , /* Helicomm IP-Link 1220-DVM */
	 0x10C4, 0x815F , /* Timewave HamLinkUSB */
	 0x10C4, 0x818B , /* AVIT Research USB to TTL */
	 0x10C4, 0x819F , /* MJS USB Toslink Switcher */
	 0x10C4, 0x81A6 , /* ThinkOptics WavIt */
	 0x10C4, 0x81A9 , /* Multiplex RC Interface */
	 0x10C4, 0x81AC , /* MSD Dash Hawk */
	 0x10C4, 0x81AD , /* INSYS USB Modem */
	 0x10C4, 0x81C8 , /* Lipowsky Industrie Elektronik GmbH, Baby-JTAG */
	 0x10C4, 0x81E2 , /* Lipowsky Industrie Elektronik GmbH, Baby-LIN */
	 0x10C4, 0x81E7 , /* Aerocomm Radio */
	 0x10C4, 0x81E8 , /* Zephyr Bioharness */
	 0x10C4, 0x81F2 , /* C1007 HF band RFID controller */
	 0x10C4, 0x8218 , /* Lipowsky Industrie Elektronik GmbH, HARP-1 */
	 0x10C4, 0x822B , /* Modem EDGE(GSM) Comander 2 */
	 0x10C4, 0x826B , /* Cygnal Integrated Products, Inc., Fasttrax GPS demonstration module */
	 0x10C4, 0x8293 , /* Telegesis ETRX2USB */
	 0x10C4, 0x82F9 , /* Procyon AVS */
	 0x10C4, 0x8341 , /* Siemens MC35PU GPRS Modem */
	 0x10C4, 0x8382 , /* Cygnal Integrated Products, Inc. */
	 0x10C4, 0x83A8 , /* Amber Wireless AMB2560 */
	 0x10C4, 0x83D8 , /* DekTec DTA Plus VHF/UHF Booster/Attenuator */
	 0x10C4, 0x8411 , /* Kyocera GPS Module */
	 0x10C4, 0x8418 , /* IRZ Automation Teleport SG-10 GSM/GPRS Modem */
	 0x10C4, 0x846E , /* BEI USB Sensor Interface (VCP) */
	 0x10C4, 0x8477 , /* Balluff RFID */
	 0x10C4, 0x85EA , /* AC-Services IBUS-IF */
	 0x10C4, 0x85EB , /* AC-Services CIS-IBUS */
	 0x10C4, 0x85F8 , /* Virtenio Preon32 */
	 0x10C4, 0x8664 , /* AC-Services CAN-IF */
	 0x10C4, 0x8665 , /* AC-Services OBD-IF */
	 0x10C4, 0x88A4 , /* MMB Networks ZigBee USB Device */
	 0x10C4, 0x88A5 , /* Planet Innovation Ingeni ZigBee USB Device */
	 0x10C4, 0xEA60 , /* Silicon Labs factory default */
	 0x10C4, 0xEA61 , /* Silicon Labs factory default */
	 0x10C4, 0xEA70 , /* Silicon Labs factory default */
	 0x10C4, 0xEA80 , /* Silicon Labs factory default */
	 0x10C4, 0xEA71 , /* Infinity GPS-MIC-1 Radio Monophone */
	 0x10C4, 0xF001 , /* Elan Digital Systems USBscope50 */
	 0x10C4, 0xF002 , /* Elan Digital Systems USBwave12 */
	 0x10C4, 0xF003 , /* Elan Digital Systems USBpulse100 */
	 0x10C4, 0xF004 , /* Elan Digital Systems USBcount50 */
	 0x10C5, 0xEA61 , /* Silicon Labs MobiData GPRS USB Modem */
	 0x10CE, 0xEA6A , /* Silicon Labs MobiData GPRS USB Modem 100EU */
	 0x13AD, 0x9999 , /* Baltech card reader */
	 0x1555, 0x0004 , /* Owen AC4 USB-RS485 Converter */
	 0x166A, 0x0201 , /* Clipsal 5500PACA C-Bus Pascal Automation Controller */
	 0x166A, 0x0301 , /* Clipsal 5800PC C-Bus Wireless PC Interface */
	 0x166A, 0x0303 , /* Clipsal 5500PCU C-Bus USB interface */
	 0x166A, 0x0304 , /* Clipsal 5000CT2 C-Bus Black and White Touchscreen */
	 0x166A, 0x0305 , /* Clipsal C-5000CT2 C-Bus Spectrum Colour Touchscreen */
	 0x166A, 0x0401 , /* Clipsal L51xx C-Bus Architectural Dimmer */
	 0x166A, 0x0101 , /* Clipsal 5560884 C-Bus Multi-room Audio Matrix Switcher */
	 0x16D6, 0x0001 , /* Jablotron serial interface */
	 0x16DC, 0x0010 , /* W-IE-NE-R Plein & Baus GmbH PL512 Power Supply */
	 0x16DC, 0x0011 , /* W-IE-NE-R Plein & Baus GmbH RCM Remote Control for MARATON Power Supply */
	 0x16DC, 0x0012 , /* W-IE-NE-R Plein & Baus GmbH MPOD Multi Channel Power Supply */
	 0x16DC, 0x0015 , /* W-IE-NE-R Plein & Baus GmbH CML Control, Monitoring and Data Logger */
	 0x17A8, 0x0001 , /* Kamstrup Optical Eye/3-wire */
	 0x17A8, 0x0005 , /* Kamstrup M-Bus Master MultiPort 250D */
	 0x17F4, 0xAAAA , /* Wavesense Jazz blood glucose meter */
	 0x1843, 0x0200 , /* Vaisala USB Instrument Cable */
	 0x18EF, 0xE00F , /* ELV USB-I2C-Interface */
	 0x1ADB, 0x0001 , /* Schweitzer Engineering C662 Cable */
	 0x1BE3, 0x07A6 , /* WAGO 750-923 USB Service Cable */
	 0x1E29, 0x0102 , /* Festo CPX-USB */
	 0x1E29, 0x0501 , /* Festo CMSP */
	 0x1FB9, 0x0100 , /* Lake Shore Model 121 Current Source */
	 0x1FB9, 0x0200 , /* Lake Shore Model 218A Temperature Monitor */
	 0x1FB9, 0x0201 , /* Lake Shore Model 219 Temperature Monitor */
	 0x1FB9, 0x0202 , /* Lake Shore Model 233 Temperature Transmitter */
	 0x1FB9, 0x0203 , /* Lake Shore Model 235 Temperature Transmitter */
	 0x1FB9, 0x0300 , /* Lake Shore Model 335 Temperature Controller */
	 0x1FB9, 0x0301 , /* Lake Shore Model 336 Temperature Controller */
	 0x1FB9, 0x0302 , /* Lake Shore Model 350 Temperature Controller */
	 0x1FB9, 0x0303 , /* Lake Shore Model 371 AC Bridge */
	 0x1FB9, 0x0400 , /* Lake Shore Model 411 Handheld Gaussmeter */
	 0x1FB9, 0x0401 , /* Lake Shore Model 425 Gaussmeter */
	 0x1FB9, 0x0402 , /* Lake Shore Model 455A Gaussmeter */
	 0x1FB9, 0x0403 , /* Lake Shore Model 475A Gaussmeter */
	 0x1FB9, 0x0404 , /* Lake Shore Model 465 Three Axis Gaussmeter */
	 0x1FB9, 0x0600 , /* Lake Shore Model 625A Superconducting MPS */
	 0x1FB9, 0x0601 , /* Lake Shore Model 642A Magnet Power Supply */
	 0x1FB9, 0x0602 , /* Lake Shore Model 648 Magnet Power Supply */
	 0x1FB9, 0x0700 , /* Lake Shore Model 737 VSM Controller */
	 0x1FB9, 0x0701 , /* Lake Shore Model 776 Hall Matrix */
	 0x3195, 0xF190 , /* Link Instruments MSO-19 */
	 0x3195, 0xF280 , /* Link Instruments MSO-28 */
	 0x3195, 0xF281 , /* Link Instruments MSO-28 */
	 0x413C, 0x9500 , /* DW700 GPS USB interface */
 };

								
//匹配VID PID
UINT8 MatchID(UINT16 vid,UINT16 pid,const UINT16 *id_table,UINT16 element_count)
{
	UINT16 i;
	if(element_count%2)
		element_count--;   //变成偶数
	for(i=0;i<element_count;i+=2)
	{
		if(vid==id_table[i] && pid==id_table[i+1])
			break;
	}
	if(i<element_count) return 1;   //成功匹配
	else return 0;
}
/*******************************************************************************
* Function Name  : DisableRootHubPort( )
* Description    : 关闭HUB端口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void    DisableRootHubPort( )          
{
	ThisUsbDev.DeviceStatus = ROOT_DEV_DISCONNECT;
	ThisUsbDev.DeviceAddress = 0x00;
	ThisUsbDev.DeviceType = 0;
	ThisUsbDev.GpVar0 = 0;
	ThisUsbDev.GpVar1 = 0;
	ThisUsbDev.GpVar2 = 0;
	UHOST_CTRL &= ~bUH_PORT_EN;                  //端口失能
}
/*******************************************************************************
* Function Name  : AnalyzeRootHub(void)
* Description    : 分析ROOT-HUB状态,处理ROOT-HUB端口的设备插拔事件
                   如果设备拔出,函数中调用DisableRootHubPort()函数,将端口关闭,插入事件,置相应端口的状态位
* Input          : None
* Output         : None
* Return         : 返回ERR_SUCCESS为没有情况,返回ERR_USB_CONNECT为检测到新连接,返回ERR_USB_DISCON为检测到断开
*******************************************************************************/
UINT8   AnalyzeRootHub( void )
{ 
	UINT8	s;
	s = ERR_SUCCESS;
	if ( USB_MIS_ST & bUMS_DEV_ATTACH ) {                                        // 设备存在
		if ( ThisUsbDev.DeviceStatus == ROOT_DEV_DISCONNECT || ( UHOST_CTRL & bUH_PORT_EN ) == 0x00 )  // 检测到有设备插入,但尚未允许,说明是刚插入
		{                                                                                            
			DisableRootHubPort( );                                                   // 关闭端口
			ThisUsbDev.DeviceStatus = ROOT_DEV_CONNECTED;                            //置连接标志
#if DE_PRINTF
			printf( "USB dev in\n" );
#endif
			s = ERR_USB_CONNECT;
		}
	}
	else if ( ThisUsbDev.DeviceStatus >= ROOT_DEV_CONNECTED )                      //检测到设备拔出 
	{                  
		DisableRootHubPort( );                                                     // 关闭端口
#if DE_PRINTF		
		printf( "USB dev out\n" );
#endif
		if ( s == ERR_SUCCESS ) s = ERR_USB_DISCON;
	}
	return( s );
}
/*******************************************************************************
* Function Name  : SetHostUsbAddr
* Description    : 设置USB主机当前操作的USB设备地址
* Input          : UINT8 addr
* Output         : None
* Return         : None
*******************************************************************************/
void    SetHostUsbAddr( UINT8 addr )
{
	USB_DEV_AD = 0;
	USB_DEV_AD |= (bUDA_GP_BIT + addr);
}

#ifndef	FOR_ROOT_UDISK_ONLY
/*******************************************************************************
* Function Name  : SetUsbSpeed
* Description    : 设置当前USB速度
* Input          : UINT8 FullSpeed
* Output         : None
* Return         : None
*******************************************************************************/
void    SetUsbSpeed( UINT8 FullSpeed )  
{
    if ( FullSpeed )                                                           // 全速
    {
        USB_CTRL &= ~ bUC_LOW_SPEED;                                           // 全速
		UHOST_CTRL &= ~bUH_LOW_SPEED;
        UH_SETUP &= ~ bUH_PRE_PID_EN;                                          // 禁止PRE PID
    }
    else
    {
        USB_CTRL |= bUC_LOW_SPEED;                                             // 低速
		UHOST_CTRL |= bUH_LOW_SPEED;
    }
}
#endif

/*******************************************************************************
* Function Name  : ResetRootHubPort( )
* Description    : 检测到设备后,复位总线,为枚举设备准备,设置为默认为全速
* Input          : None   
* Output         : None
* Return         : None
*******************************************************************************/
void    ResetRootHubPort( )
{
    UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;                                      //USB设备的端点0的最大包尺寸
    SetHostUsbAddr( 0x00 );
	UHOST_CTRL = UHOST_CTRL | bUH_BUS_RESET;                                   //开始复位
    mDelaymS( 15 );                                                            // 复位时间10mS到20mS
    UHOST_CTRL = UHOST_CTRL & ~ bUH_BUS_RESET;                                 // 结束复位
    mDelayuS( 250 );
    UIF_DETECT = 0;                                                            // 清中断标志
}
/*******************************************************************************
* Function Name  : EnableRootHubPort( )
* Description    : 使能ROOT-HUB端口,相应的bUH_PORT_EN置1开启端口,设备断开可能导致返回失败
* Input          : None
* Output         : None
* Return         : 返回ERR_SUCCESS为检测到新连接,返回ERR_USB_DISCON为无连接
*******************************************************************************/
UINT8   EnableRootHubPort( )
{
#ifdef DISK_BASE_BUF_LEN
	if ( CH554DiskStatus < DISK_CONNECT ) CH554DiskStatus = DISK_CONNECT;
#else
	if ( ThisUsbDev.DeviceStatus < ROOT_DEV_CONNECTED ) ThisUsbDev.DeviceStatus = ROOT_DEV_CONNECTED;
#endif
	if ( USB_MIS_ST & bUMS_DEV_ATTACH ) {                                        // 有设备
#ifndef DISK_BASE_BUF_LEN
		if ( ( UHOST_CTRL & bUH_PORT_EN ) == 0x00 ) {                              // 尚未使能
			ThisUsbDev.DeviceSpeed = USB_MIS_ST & bUMS_DM_LEVEL ? 0 : 1; 
			SetUsbSpeed( ThisUsbDev.DeviceSpeed );
		}
#endif
		UHOST_CTRL |= bUH_PORT_EN;                                                 //使能HUB端口
		return( ERR_SUCCESS );
	}
	return( ERR_USB_DISCON );
}

/*******************************************************************************
* Function Name  : WaitUSB_Interrupt
* Description    : 等待USB中断
* Input          : None
* Output         : None
* Return         : 返回ERR_SUCCESS 数据接收或者发送成功
                   ERR_USB_UNKNOWN 数据接收或者发送失败
*******************************************************************************/
UINT8   WaitUSB_Interrupt( void )
{
    UINT16  i;
    for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- ){;}
    return( UIF_TRANSFER ? ERR_SUCCESS : ERR_USB_UNKNOWN );
}
/*******************************************************************************
* Function Name  : USBHostTransact
* Description    : CH554传输事务,输入目的端点地址/PID令牌,同步标志,以20uS为单位的NAK重试总时间(0则不重试,0xFFFF无限重试),返回0成功,超时/出错重试
                   本子程序着重于易理解,而在实际应用中,为了提供运行速度,应该对本子程序代码进行优化
* Input          : UINT8 endp_pid 令牌和地址  endp_pid: 高4位是token_pid令牌, 低4位是端点地址
                   UINT8 tog      同步标志
                   UINT16 timeout 超时时间
* Output         : None
* Return         : ERR_USB_UNKNOWN 超时，可能硬件异常
                   ERR_USB_DISCON  设备断开
                   ERR_USB_CONNECT 设备连接
                   ERR_SUCCESS     传输完成
*******************************************************************************/
UINT8   USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout )
{
#define	TransRetry	UEP0_T_LEN	                                               // 节约内存
	UINT8	s, r;
	UINT16	i;
	UH_RX_CTRL = UH_TX_CTRL = tog;
	TransRetry = 3;                                                               //超时重试次数
	do {
		UH_EP_PID = endp_pid;                                                      // 指定令牌PID和目的端点号
		UIF_TRANSFER = 0;                                                          // 清除传输完成标志
		for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- );
		UH_EP_PID = 0x00;                                                          // 停止USB传输
		if ( UIF_TRANSFER == 0 ) return( ERR_USB_UNKNOWN );
		if ( UIF_DETECT ) {                                                        // USB设备插拔事件
			UIF_DETECT = 0;                                                          // 清中断标志
			s = AnalyzeRootHub( );                                                   // 分析ROOT-HUB状态
			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
#ifdef DISK_BASE_BUF_LEN
			if ( CH554DiskStatus == DISK_DISCONNECT ) return( ERR_USB_DISCON );      // USB设备断开事件
#else
			if ( ThisUsbDev.DeviceStatus == ROOT_DEV_DISCONNECT ) return( ERR_USB_DISCON );// USB设备断开事件
#endif
		}
		if ( UIF_TRANSFER ) {  // 传输完成
			UIF_TRANSFER = 0;                                                          // 清除传输完成标志
			if ( U_TOG_OK ) return( ERR_SUCCESS );                                     //返回ACK，同时包翻转同步
			r = USB_INT_ST & MASK_UIS_H_RES;  // USB设备应答状态
			if ( r == USB_PID_STALL ) return( r | ERR_USB_TRANSFER );
			if ( r == USB_PID_NAK ) {
				if ( timeout == 0 ) return( r | ERR_USB_TRANSFER );
				if ( timeout < 0xFFFF ) timeout --;
				TransRetry++;
			}
			else switch ( endp_pid >> 4 ) {
				case USB_PID_SETUP:
				case USB_PID_OUT:
					if ( r ) return( r | ERR_USB_TRANSFER );  // 非0不是超时/出错,意外应答
					break;  // 超时重试
				case USB_PID_IN:
					if ( r == USB_PID_DATA0 && r == USB_PID_DATA1 ) {  // 不同步则需丢弃后重试
					}  // 不同步重试
					else if ( r ) return( r | ERR_USB_TRANSFER );  // 不是超时/出错,意外应答
					break;  // 超时重试
				default:
					return( ERR_USB_UNKNOWN );  // 不可能的情况
					break;
			}
		}
		else {  // 其它中断,不应该发生的情况
			USB_INT_FG = 0xFF;  /* 清中断标志 */
		}
		mDelayuS( 10 );
		
		//OUT包需要多加点延时
		if((endp_pid >> 4) == USB_PID_OUT)  mDelayuS( 80 );
		
	} while ( --TransRetry );
	return( ERR_USB_TRANSFER );  // 应答超时
}
/*******************************************************************************
* Function Name  : HostCtrlTransfer
* Description    : 执行控制传输,8字节请求码在pSetupReq中,DataBuf为可选的收发缓冲区
* Input          : PUINT8X DataBuf 如果需要接收和发送数据,那么DataBuf需指向有效缓冲区用于存放后续数据
                   PUINT8 RetLen  实际成功收发的总长度保存在RetLen指向的字节变量中
* Output         : None
* Return         : ERR_USB_BUF_OVER IN状态阶段出错
                   ERR_SUCCESS     数据交换成功
                   其他错误状态
*******************************************************************************/
UINT8   HostCtrlTransfer( PUINT8X DataBuf, PUINT8 RetLen )  
{
    UINT16  RemLen  = 0;
    UINT8   s, RxLen, RxCnt, TxCnt;
    PUINT8  xdata   pBuf;
    PUINT8  xdata   pLen;
    pBuf = DataBuf;
    pLen = RetLen;
    mDelayuS( 200 );
    if ( pLen )
    {
        *pLen = 0;                                                              // 实际成功收发的总长度
    }
    UH_TX_LEN = sizeof( USB_SETUP_REQ );
    s = USBHostTransact( USB_PID_SETUP << 4 | 0x00, 0x00, 200000/20 );          // SETUP阶段,200mS超时
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
	
	UH_RX_CTRL = bUH_R_TOG | bUH_R_AUTO_TOG;                                    // 默认DATA1
	UH_TX_CTRL = bUH_T_TOG | bUH_T_AUTO_TOG;
    RemLen = (pSetupReq -> wLengthH << 8)|( pSetupReq -> wLengthL);
    if ( RemLen && pBuf )                                                       // 需要收发数据
    {
        if ( pSetupReq -> bRequestType & USB_REQ_TYP_IN )                       // 收
        {
            while ( RemLen )
            {
                mDelayuS( 200 );
                s = USBHostTransact( USB_PID_IN << 4 | 0x00, UH_RX_CTRL, 200000/20 );// IN数据
                if ( s != ERR_SUCCESS )
                {
                    return( s );
                }
                RxLen = USB_RX_LEN < RemLen ? USB_RX_LEN : RemLen;
                RemLen -= RxLen;
                if ( pLen )
                {
                    *pLen += RxLen;                                              // 实际成功收发的总长度
                }

                for ( RxCnt = 0; RxCnt != RxLen; RxCnt ++ )
                {
                    *pBuf = RxBuffer[ RxCnt ];
                    pBuf ++;
                }
                if ( USB_RX_LEN == 0 || ( USB_RX_LEN & ( UsbDevEndp0Size - 1 ) ) )
                {
                    break;                                                       // 短包
                }
            }
            UH_TX_LEN = 0x00;                                                    // 状态阶段为OUT
        }
        else                                                                     // 发
        {
            while ( RemLen )
            {
                mDelayuS( 200 );
                UH_TX_LEN = RemLen >= UsbDevEndp0Size ? UsbDevEndp0Size : RemLen;
                for ( TxCnt = 0; TxCnt != UH_TX_LEN; TxCnt ++ )
                {
                    TxBuffer[ TxCnt ] = *pBuf;
                    pBuf ++;
                }
                s = USBHostTransact( USB_PID_OUT << 4 | 0x00, UH_TX_CTRL, 200000/20 );// OUT数据
                if ( s != ERR_SUCCESS )
                {
                    return( s );
                }
                RemLen -= UH_TX_LEN;
                if ( pLen )
                {
                    *pLen += UH_TX_LEN;                                           // 实际成功收发的总长度
                }
            }
            UH_TX_LEN = 0x01;                                                     // 状态阶段为IN
        }
    }
    mDelayuS( 200 );
    s = USBHostTransact( ( UH_TX_LEN ? USB_PID_IN << 4 | 0x00: USB_PID_OUT << 4 | 0x00 ), bUH_R_TOG | bUH_T_TOG, 200000/20 );  // STATUS阶段
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
    if ( UH_TX_LEN == 0 )
    {
        return( ERR_SUCCESS );                                                    // 状态OUT
    }
    if ( USB_RX_LEN == 0 )
    {
        return( ERR_SUCCESS );                                                    // 状态IN,检查IN状态返回数据长度
    }
    return( ERR_USB_BUF_OVER );                                                   // IN状态阶段错误
}
/*******************************************************************************
* Function Name  : CopySetupReqPkg
* Description    : 复制控制传输的请求包
* Input          : PUINT8C pReqPkt 控制请求包地址
* Output         : None
* Return         : None
*******************************************************************************/
void    CopySetupReqPkg( PUINT8C pReqPkt )                                        // 复制控制传输的请求包
{
    UINT8   i;
    for ( i = 0; i != sizeof( USB_SETUP_REQ ); i ++ )
    {
        ((PUINT8X)pSetupReq)[ i ] = *pReqPkt;
        pReqPkt ++;
    }
}
/*******************************************************************************
* Function Name  : CtrlGetDeviceDescr
* Description    : 获取设备描述符,返回在TxBuffer中
* Input          : None
* Output         : None
* Return         : ERR_USB_BUF_OVER 描述符长度错误
                   ERR_SUCCESS      成功
                   其他
*******************************************************************************/
UINT8   CtrlGetDeviceDescr( void )  
{
    UINT8   s;
    UINT8   len;
    UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;
    CopySetupReqPkg( SetupGetDevDescr );
	pSetupReq -> wLengthL = 8;
    s = HostCtrlTransfer( Receive_USB_Buf, (PUINT8)&len );                                      // 执行控制传输
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
    UsbDevEndp0Size = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> bMaxPacketSize0;          // 端点0最大包长度,这是简化处理,正常应该先获取前8字节后立即更新UsbDevEndp0Size再继续
    CopySetupReqPkg( SetupGetDevDescr );
    s = HostCtrlTransfer( Receive_USB_Buf, (PUINT8)&len );                                      // 执行控制传输
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }	
	if ( len < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL )
    {
        return( ERR_USB_BUF_OVER );                                              // 描述符长度错误
    }
    return( ERR_SUCCESS );
}
/*******************************************************************************
* Function Name  : CtrlGetConfigDescr
* Description    : 获取配置描述符,返回在TxBuffer中
* Input          : None
* Output         : None
* Return         : ERR_USB_BUF_OVER 描述符长度错误
                   ERR_SUCCESS      成功
                   其他
*******************************************************************************/
UINT8   CtrlGetConfigDescr( void )
{
    UINT8   s;
    UINT8  len;
    CopySetupReqPkg( SetupGetCfgDescr );
    s = HostCtrlTransfer( Receive_USB_Buf, (PUINT8)&len );                                      // 执行控制传输
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
    if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL )
    {
        return( ERR_USB_BUF_OVER );                                              // 返回长度错误
    }
    len = ( (PXUSB_CFG_DESCR)Receive_USB_Buf ) -> wTotalLengthL;
    CopySetupReqPkg( SetupGetCfgDescr );
    pSetupReq -> wLengthL = len;                                                 // 完整配置描述符的总长度
    s = HostCtrlTransfer( Receive_USB_Buf, (PUINT8)&len );                                      // 执行控制传输
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
    if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL || len < ( (PXUSB_CFG_DESCR)Receive_USB_Buf ) -> wTotalLengthL )
    {
        return( ERR_USB_BUF_OVER );                                              // 描述符长度错误
    }
    return( ERR_SUCCESS );
}
/*******************************************************************************
* Function Name  : CtrlSetUsbAddress
* Description    : 设置USB设备地址
* Input          : UINT8 addr 设备地址
* Output         : None
* Return         : ERR_SUCCESS      成功
                   其他
*******************************************************************************/
UINT8   CtrlSetUsbAddress( UINT8 addr ) 
{
    UINT8   s;
    CopySetupReqPkg( SetupSetUsbAddr );
    pSetupReq -> wValueL = addr;                                                // USB设备地址
    s = HostCtrlTransfer( NULL, (PUINT8)NULL );                                         // 执行控制传输
    if ( s != ERR_SUCCESS )
    {
        return( s );
    }
    SetHostUsbAddr( addr );                                                     // 设置USB主机当前操作的USB设备地址
    mDelaymS( 10 );                                                             // 等待USB设备完成操作
    return( ERR_SUCCESS );
}
/*******************************************************************************
* Function Name  : CtrlSetUsbConfig
* Description    : 设置USB设备配置
* Input          : UINT8 cfg       配置值
* Output         : None
* Return         : ERR_SUCCESS      成功
                   其他
*******************************************************************************/
UINT8   CtrlSetUsbConfig( UINT8 cfg )                   
{
    CopySetupReqPkg( SetupSetUsbConfig );
    pSetupReq -> wValueL = cfg;                                                // USB设备配置
    return( HostCtrlTransfer( NULL, (PUINT8)NULL ) );                                  // 执行控制传输
}
/*******************************************************************************
* Function Name  : CtrlClearEndpStall
* Description    : 清除端点STALL
* Input          : UINT8 endp       端点地址
* Output         : None
* Return         : ERR_SUCCESS      成功
                   其他
*******************************************************************************/
UINT8   CtrlClearEndpStall( UINT8 endp )  
{
    CopySetupReqPkg( SetupClrEndpStall );                                      // 清除端点的错误
    pSetupReq -> wIndexL = endp;                                               // 端点地址
    return( HostCtrlTransfer( NULL, (PUINT8)NULL ) );                                  // 执行控制传输
}

#ifndef DISK_BASE_BUF_LEN
/*******************************************************************************
* Function Name  : AnalyzeEndp
* Description    : 从描述符中分析出端点的地址，找中断和批量端点
* Input          : PUINT8X buf       待分析数据缓冲区地址
* Output         : None
* Return         : 
*******************************************************************************/
void   AnalyzeEndp( PUINT8X buf ) 
{
    UINT8   i, l;
    for ( i = 0; i < ( (PXUSB_CFG_DESCR)buf ) -> wTotalLengthL; i += l )       // 搜索端点描述符,跳过配置描述符和接口描述符
    {
        if ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bDescriptorType == USB_DESCR_TYP_ENDP)   // 端点描述符 
        {
			if(( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bmAttributes & USB_ENDP_TYPE_MASK ) == USB_ENDP_TYPE_INTER )   // 中断端点描述符 
			{
				if(( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_DIR_MASK ))                   //IN
					ThisUsbDev.GpVar2 = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_ADDR_MASK;
			}
			else if(( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bmAttributes & USB_ENDP_TYPE_MASK ) == USB_ENDP_TYPE_BULK )
			{
				if(( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_DIR_MASK ))                   //IN
				{
					ThisUsbDev.GpVar1 = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_ADDR_MASK;	
					ThisUsbDev.GpVar1_Lenth = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> wMaxPacketSizeL;
				}
				else                                                                                            //OUT
				{
					ThisUsbDev.GpVar0 = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_ADDR_MASK;	
					ThisUsbDev.GpVar0_Lenth = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> wMaxPacketSizeL;
				}
			}                                                          
        }
        l = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bLength;                          // 当前描述符长度,跳过
    }
}
//配置341波特率 ,成功返回0
UINT8 CH34X_Baud_Config(UINT32 baud)
{
	UINT8 s=0;
	UINT8 divisor = 0; 
	UINT8 factor = 0;
	UINT8 reg_value=0;
	UINT32 tmp;
	
	reg_value|=0x03;           //数据位8位
	tmp=(1532620800/baud);
	divisor=3; 
	while((tmp>0xfff0)&&divisor){
		tmp>>=3;
		divisor --;
	}	
	if(tmp>0xfff0)
		return ERR_USB_UNKNOWN;	
	tmp=0x10000-tmp;                    
	tmp=(tmp&0xff00)>>8;                   
	factor=tmp;
	reg_value|=0xc0; 
	
	TxBuffer[0] = 0x40;
	TxBuffer[1] = 0xa1;	
	TxBuffer[2] = 0x9c;
	TxBuffer[3] = reg_value;  
	TxBuffer[4] = 0x80|divisor; 
	TxBuffer[5] = factor;
	TxBuffer[6] = 0x00;
	TxBuffer[7] = 0x00;
	s = HostCtrlTransfer( NULL, NULL ); 
	return( s );		
}
//配置2303波特率 ,成功返回0
UINT8 PL2303_Baud_Config(UINT32 baud)
{
	UINT8 s=0;
#define VENDOR_READ_REQUEST_TYPE	0xc0
#define VENDOR_READ_REQUEST		    0x01
#define VENDOR_WRITE_REQUEST_TYPE	0x40
#define VENDOR_WRITE_REQUEST		0x01
#define GET_LINE_REQUEST_TYPE		0xa1
#define GET_LINE_REQUEST		    0x21
#define SET_LINE_REQUEST_TYPE		0x21
#define SET_LINE_REQUEST		    0x20
#define SET_CONTROL_REQUEST_TYPE	0x21
#define SET_CONTROL_REQUEST		    0x22
	
#define READ(a,b,c,d,e)  {TxBuffer[0]=a;TxBuffer[1]=b;TxBuffer[2]=(UINT8)c;TxBuffer[3]=(UINT8)(c>>8);  \
						TxBuffer[4]=(UINT8)d;TxBuffer[5]=(UINT8)(d>>8);TxBuffer[6]=e;TxBuffer[7]=0;  \
						HostCtrlTransfer( Receive_USB_Buf, NULL );}
#define WRITE(a,b,c,d,e)  {TxBuffer[0]=a;TxBuffer[1]=b;TxBuffer[2]=(UINT8)c;TxBuffer[3]=(UINT8)(c>>8); \
						TxBuffer[4]=(UINT8)d;TxBuffer[5]=(UINT8)(d>>8);TxBuffer[6]=e;TxBuffer[7]=0; \
						HostCtrlTransfer( Receive_USB_Buf, NULL );}				
						
	
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0,1);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0x0404, 0,0);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0,1);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8383, 0,1);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0,1);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0x0404, 1,0);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0,1);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8383, 0,1);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0, 1,0);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 1, 0,0);						
	
	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 2, 0x44,0);
	WRITE (USB_REQ_TYP_OUT, USB_CLEAR_FEATURE, 1, 0,0);
	
						
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x80, 0,2);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0, 1,0);
//	WRITE (SET_CONTROL_REQUEST_TYPE,SET_CONTROL_REQUEST, 1, 0,0);	
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x80, 0,2);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0, 1,0);
//	WRITE (SET_CONTROL_REQUEST_TYPE,SET_CONTROL_REQUEST, 3, 0,0);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x80, 0,2);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0, 1,0);
//	READ (VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x80, 0,2);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0, 1,0);

//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0x0B0B, 2,0);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0x0909, 0,0);
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0x0808, 0,0);

//	WRITE (SET_CONTROL_REQUEST_TYPE,SET_CONTROL_REQUEST, 3, 0,0);
//	WRITE (SET_CONTROL_REQUEST_TYPE,SET_CONTROL_REQUEST, 3, 0,0);
	Receive_USB_Buf[0] = (UINT8)baud;
	Receive_USB_Buf[1] = (UINT8)(baud>>8);
	Receive_USB_Buf[2] = (UINT8)(baud>>16);
	Receive_USB_Buf[3] = (UINT8)(baud>>24);
	Receive_USB_Buf[4] = 0;
	Receive_USB_Buf[5] = 0;
	Receive_USB_Buf[6] = 8;             //数据位长度	
	WRITE (SET_LINE_REQUEST_TYPE,SET_LINE_REQUEST, 0, 0,7);	
//	WRITE (VENDOR_WRITE_REQUEST_TYPE,VENDOR_WRITE_REQUEST, 0x0505, 0x1311,0);
	return( s );	
}
//配置2102波特率 ,成功返回0
UINT8 CP2102_Baud_Config(UINT32 baud)
{
	UINT8 s;
	UINT16 tmp;
	TxBuffer[0] = 0x41;
	TxBuffer[1] = 0x00;	
	TxBuffer[2] = 0x01;   //为1开启，为0关闭
	TxBuffer[3] = 0x00;  
	TxBuffer[4] = 0x00; 
	TxBuffer[5] = 0x00;
	TxBuffer[6] = 0x00;
	TxBuffer[7] = 0x00;
	s = HostCtrlTransfer( NULL, NULL );
	
	tmp=(3686400/baud);
	TxBuffer[0] = 0x41;
	TxBuffer[1] = 0x01;	
	TxBuffer[2] = (UINT8)(tmp&0Xff);  
	TxBuffer[3] = (UINT8)((tmp>>8)&0xff);
	TxBuffer[4] = 0x00; 
	TxBuffer[5] = 0x00;
	TxBuffer[6] = 0x00;
	TxBuffer[7] = 0x00;	
	s = HostCtrlTransfer( NULL, NULL ); 
	
	TxBuffer[0] = 0x41;
	TxBuffer[1] = 0x03;	
	TxBuffer[2] = 0x00;   //停止位 1位
	TxBuffer[3] = 0x08;   //数据位
	TxBuffer[4] = 0x00; 
	TxBuffer[5] = 0x00;
	TxBuffer[6] = 0x00;
	TxBuffer[7] = 0x00;	
	s = HostCtrlTransfer( NULL, NULL ); 	
	
	return s;
}

UINT8 code SetLineCode[8] = {0x21,0x20,0x00,0x00,0x00,0x00,0x07,0x00};
//配置CDC类设备波特率
UINT8 CDC_Baud_Config(UINT32 baud)
{
	UINT8 s;
    CopySetupReqPkg( SetLineCode );
	Receive_USB_Buf[0] = (UINT8)baud;
	Receive_USB_Buf[1] = (UINT8)(baud>>8);
	Receive_USB_Buf[2] = (UINT8)(baud>>16);
	Receive_USB_Buf[3] = (UINT8)(baud>>24);	
	Receive_USB_Buf[4] = 0;
	Receive_USB_Buf[5] = 0;
	Receive_USB_Buf[6] = 8;                    //8字节	
    s = HostCtrlTransfer( Receive_USB_Buf, NULL); 	
	return s;
}
/*******************************************************************************
* Function Name  : InitRootDevice
* Description    : 初始化指定ROOT-HUB端口的USB设备
* Input          : UINT8 RootHubIndex 指定端口，内置HUB端口号0/1
* Output         : None
* Return         :
*******************************************************************************/
UINT8   InitRootDevice( UINT8 RootHubIndex ) 
{
    UINT8   i, s, cfg, dv_cls, if_cls, if_ptl;
	UINT16 dev_vid,dev_pid;
#if DE_PRINTF	
    printf( "Reset root hub %1d# port\n", (UINT16)RootHubIndex );
#endif
    ResetRootHubPort( );                                                    // 检测到设备后,复位相应端口的USB总线
    for ( i = 0, s = 0; i < 100; i ++ )                                     // 等待USB设备复位后重新连接,100mS超时
    {
        mDelaymS( 1 );
        if ( EnableRootHubPort( ) == ERR_SUCCESS )                          // 使能ROOT-HUB端口
        {
            i = 0;
            s ++;                                                           // 计时等待USB设备连接后稳定
            if ( s > 20 )
            {
                break;                                                      // 已经稳定连接15mS
            }
        }
    }	
    if ( i )                                                                 // 复位后设备没有连接
    {
        DisableRootHubPort( );
#if DE_PRINTF			
        printf( "Disable root hub %1d# port because of disconnect\n", (UINT16)RootHubIndex );
#endif	
        return( ERR_USB_DISCON );
    }
#if DE_PRINTF		
    printf( "GetDevDescr: " );
#endif
    s = CtrlGetDeviceDescr( );                                               // 获取设备描述符
    if ( s == ERR_SUCCESS )
    {
#if DE_PRINTF	
        for ( i = 0; i < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL; i ++ )
        {				
            printf( "x%02X ", (UINT16)( Receive_USB_Buf[i] ) );				
        }
        printf( "\n" );                                                       // 显示出描述符
#endif				
        dv_cls = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> bDeviceClass;               // 设备类代码
		
		*(UINT8 *)&dev_vid = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> idVendorH;        // PID
		*((UINT8 *)&dev_vid+1) = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> idVendorL;		
		*(UINT8 *)&dev_pid = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> idProductH;        // PID
		*((UINT8 *)&dev_pid+1) = ( (PXUSB_DEV_DESCR)Receive_USB_Buf ) -> idProductL;
		
        s = CtrlSetUsbAddress( RootHubIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL );// 设置USB设备地址,加上RootHubIndex可以保证2个HUB端口分配不同的地址
        if ( s == ERR_SUCCESS )
        {
            ThisUsbDev.DeviceAddress = RootHubIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL;  // 保存USB地址
#if DE_PRINTF						
            printf( "GetCfgDescr: " );
#endif					
            s = CtrlGetConfigDescr( );                                        // 获取配置描述符
            if ( s == ERR_SUCCESS )
            {
                cfg = ( (PXUSB_CFG_DESCR)Receive_USB_Buf ) -> bConfigurationValue;
#if DE_PRINTF							
                for ( i = 0; i < ( (PXUSB_CFG_DESCR)Receive_USB_Buf ) -> wTotalLengthL; i ++ )
                {
                    printf( "x%02X ", (UINT16)( Receive_USB_Buf[i] ) );
                }
                printf("\n");
#endif			
				AnalyzeEndp( Receive_USB_Buf );                                                      //分析出端点				
                if_cls = ( (PXUSB_CFG_DESCR_LONG)Receive_USB_Buf ) -> itf_descr.bInterfaceClass;     // 接口类代码                                                                // 可以进一步分析
				if_ptl = ( (PXUSB_CFG_DESCR_LONG)Receive_USB_Buf ) -> itf_descr.bInterfaceProtocol;  // 接口协议码

				s = CtrlSetUsbConfig( cfg );                                                 // 设置USB设备配置            
				if ( s != ERR_SUCCESS )
					return s;	
				
				if( MatchID(dev_vid,dev_pid,CH34X_ID_Table,sizeof(CH34X_ID_Table)/sizeof(UINT16)) )   //CH34X	
				{
					ThisUsbDev.DeviceType = DEV_TYPE_CH34X;
					//初始化波特率参数(类命令)
					s = CH34X_Baud_Config(Comm_Baud);
					if(s==ERR_SUCCESS)
					{
						ThisUsbDev.DeviceStatus = ROOT_DEV_SUCCESS;                        //设备初始化成功
						return ERR_SUCCESS;
					}
                }	
				else if( MatchID(dev_vid,dev_pid,PL2303_ID_Table,sizeof(PL2303_ID_Table)/sizeof(UINT16)) )  //PL2303
				{
					ThisUsbDev.DeviceType = DEV_TYPE_PL2303; 
					s = PL2303_Baud_Config(Comm_Baud);
					if(s==ERR_SUCCESS)
					{
						ThisUsbDev.DeviceStatus = ROOT_DEV_SUCCESS;                        //设备初始化成功
						return ERR_SUCCESS;	
					}						
				}
				else if(MatchID(dev_vid,dev_pid,CP210x_ID_Table,sizeof(CP210x_ID_Table)/sizeof(UINT16)))  //CP2102
				{
					
					ThisUsbDev.DeviceType = DEV_TYPE_CP210x;
					s = CP2102_Baud_Config(Comm_Baud);
					if(s==ERR_SUCCESS)
					{
						ThisUsbDev.DeviceStatus = ROOT_DEV_SUCCESS;                        //设备初始化成功
						return ERR_SUCCESS;	
					}					
				}
				else if(dv_cls == USB_DEV_CLASS_COMMUNIC)                                  //通讯类 默认为CDC设备
				{
					ThisUsbDev.DeviceType = DEV_TYPE_CDC;
					s = CDC_Baud_Config(Comm_Baud);
					if(s==ERR_SUCCESS)
					{
						ThisUsbDev.DeviceStatus = ROOT_DEV_SUCCESS;                        //设备初始化成功
						return ERR_SUCCESS;	
					}						
				}
				else return DEV_TYPE_UNKNOWN;
				
            }
        }
    }
#if DE_PRINTF			
    printf( "InitRootDev Err = %02X\n", (UINT16)s );
#endif		
    ThisUsbDev.DeviceStatus = ROOT_DEV_FAILED;
    SetUsbSpeed( 1 );                                                                 // 默认为全速
    return( s );
}
/*******************************************************************************
* Function Name  : EnumAllRootDevice
* Description    : 枚举所有ROOT-HUB端口的USB设备
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8   EnumAllRootDevice( void )   
{
    UINT8I   s, RootHubIndex;
#if DE_PRINTF	
    printf( "EnumAllRootDev\n" );
#endif
    for ( RootHubIndex = 0; RootHubIndex < 1; RootHubIndex ++ )
    {
#if DE_PRINTF			
        printf( "RootHubIndex %02x\n",(UINT16)RootHubIndex );
#endif		
        if ( ThisUsbDev.DeviceStatus == ROOT_DEV_CONNECTED )                         // 刚插入设备尚未初始化
        {
            s = InitRootDevice( RootHubIndex );                                      // 初始化/枚举指定HUB端口的USB设备
            if ( s != ERR_SUCCESS )
            {
                return( s );
            }
        }
    }
    return( ERR_SUCCESS );
}

#endif

/*******************************************************************************
* Function Name  : InitUSB_Host
* Description    : 初始化USB主机
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void    InitUSB_Host( void )
{
    IE_USB = 0;                                                                     //查询方式
    USB_CTRL = bUC_HOST_MODE;                                                       // 先设定模式
    USB_DEV_AD = 0x00;
    UH_EP_MOD = bUH_EP_TX_EN | bUH_EP_RX_EN ;                                       //单64字节收发缓冲区
    UH_RX_DMA = RxBuffer;
    UH_TX_DMA = TxBuffer;
    UH_RX_CTRL = 0x00;
    UH_TX_CTRL = 0x00;
    USB_CTRL = bUC_HOST_MODE | bUC_INT_BUSY | bUC_DMA_EN;                           // 启动USB主机及DMA,在中断标志未清除前自动暂停    
	UHOST_CTRL = 0;
	UH_SETUP = bUH_SOF_EN;
    USB_INT_FG = 0xFF;                                                              // 清中断标志
    DisableRootHubPort( );                                                          // 清空
    USB_INT_EN = bUIE_TRANSFER | bUIE_DETECT;
}

/*
****************************************
**  Copyright  (C)  W.ch  1999-2015   **
**  Web:              http://wch.cn   **
****************************************
*/
/* USB host example for CH559, RootHUB下操作鼠标和键盘 */

#include <CH559.H>
#include <stdio.h>
#include <string.h>
#include "HID.h"

// 各子程序返回状态码
#define	ERR_SUCCESS			  0x00	// 操作成功
#define	ERR_USB_CONNECT		0x15	/* 检测到USB设备连接事件,已经连接 */
#define	ERR_USB_DISCON		0x16	/* 检测到USB设备断开事件,已经断开 */
#define	ERR_USB_BUF_OVER	0x17	/* USB传输的数据有误或者数据太多缓冲区溢出 */
#define	ERR_USB_DISK_ERR	0x1F	/* USB存储器操作失败,在初始化时可能是USB存储器不支持,在读写操作中可能是磁盘损坏或者已经断开 */
#define	ERR_USB_TRANSFER	0x20	/* NAK/STALL等更多错误码在0x20~0x2F */
#define	ERR_USB_UNSUPPORT	0xFB
#define	ERR_USB_UNKNOWN		0xFE

// 获取设备描述符
UINT8C	SetupGetDevDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_DEVICE, 0x00, 0x00, sizeof( USB_DEV_DESCR ), 0x00 };
// 获取配置描述符
UINT8C	SetupGetCfgDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_CONFIG, 0x00, 0x00, 0x04, 0x00 };
// 设置USB地址
UINT8C	SetupSetUsbAddr[] = { USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 设置USB配置
UINT8C	SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 清除端点STALL
UINT8C	SetupClrEndpStall[] = { USB_REQ_TYP_OUT | USB_REQ_RECIP_ENDP, USB_CLEAR_FEATURE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 获取HUB描述符
UINT8C	SetupGetHubDescr[] = { HUB_GET_HUB_DESCRIPTOR, HUB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_HUB, 0x00, 0x00, sizeof( USB_HUB_DESCR ), 0x00 };
// 获取HID设备报表描述符
UINT8C	SetupGetHIDDevReport[] = { 0x81, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_REPORT, 0x00, 0x00, 0x49, 0x00 };
// SET_Report
UINT8C	SetupSetKeyReport[] = { 0x21, 0x09, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00 };

UINT8X	UsbDevEndp0Size;	/* USB设备的端点0的最大包尺寸 */

//USB设备相关信息表,CH559最多支持2个设备
#define	ROOT_DEV_DISCONNECT		0
#define	ROOT_DEV_CONNECTED		1
#define	ROOT_DEV_FAILED			2
#define	ROOT_DEV_SUCCESS		3

#define	DEV_TYPE_KEYBOARD	( USB_DEV_CLASS_HID | 0x20 )
#define	DEV_TYPE_MOUSE		( USB_DEV_CLASS_HID | 0x30 )

MOUSE xdata Mouse;
UINT8 Set_Port = 0;
struct _RootHubDev{
	UINT8	DeviceStatus;			// 设备状态,0-无设备,1-有设备但尚未初始化,2-有设备但初始化枚举失败,3-有设备且初始化枚举成功
	UINT8	DeviceAddress;		// 设备被分配的USB地址
	UINT8	DeviceSpeed;			// 0为低速,非0为全速
	UINT8	DeviceType;				// 设备类型
	UINT8	GpVar;					  // 通用变量
	UINT8	GpVar1;					  // 通用变量1
} xdata RootHubDev[2];

#define	HUB_MAX_PORTS	4

/*
约定: USB设备地址分配规则(参考USB_DEVICE_ADDR)
地址值  设备位置
0x02    内置Root-HUB0下的USB设备或外部HUB
0x03    内置Root-HUB1下的USB设备或外部HUB
0x1x    内置Root-HUB0下的外部HUB的端口x下的USB设备,x为1~n
0x2x    内置Root-HUB1下的外部HUB的端口x下的USB设备,x为1~n
*/

struct _DevOnHubPort{
	UINT8	DeviceStatus;			// 设备状态,0-无设备,1-有设备但尚未初始化,2-有设备但初始化枚举失败,3-有设备且初始化枚举成功
	UINT8	DeviceAddress;		// 设备被分配的USB地址
	UINT8	DeviceSpeed;			// 0为低速,非0为全速
	UINT8	DeviceType;				// 设备类型
	UINT8	GpVar;					  // 通用变量
} xdata DevOnHubPort[2][HUB_MAX_PORTS];          // 假定:不超过2个外部HUB,每个外部HUB不超过HUB_MAX_PORTS个端口(多了不管)

UINT8X	RxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0000 ;// IN, must even address
UINT8X	TxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0100 ;// OUT, must even address
#define	pSetupReq	((PXUSB_SETUP_REQ)TxBuffer)
bit		RootHubId;		                             // 当前正在操作的root-hub端口号:0=HUB0,1=HUB1
bit		FoundNewDev;

#define	WAIT_USB_TOUT_200US		200                // 等待USB中断超时时间200uS@Fsys=12MHz
//#define	FREQ_SYS	20000000	                   // 系统主频20MHz,默认主频12MHz
#define UART0_DEBUG   1

#pragma NOAREGS

void	mDelayuS( UINT16 n );                      // 以uS为单位延时
void	mDelaymS( UINT16 n );                      // 以mS为单位延时
void	DisableRootHubPort( UINT8 RootHubIndex );  // 关闭指定的ROOT-HUB端口,实际上硬件已经自动关闭,此处只是清除一些结构状态
UINT8	AnalyzeRootHub( void );                    // 分析ROOT-HUB状态,处理ROOT-HUB端口的设备插拔事件
                                                 // 返回ERR_SUCCESS为没有情况,返回ERR_USB_CONNECT为检测到新连接,返回ERR_USB_DISCON为检测到断开
void	SetHostUsbAddr( UINT8 addr );              // 设置USB主机当前操作的USB设备地址
void	SetUsbSpeed( UINT8 FullSpeed );            // 设置当前USB速度
void	ResetRootHubPort( UINT8 RootHubIndex );    // 检测到设备后,复位相应端口的总线,为枚举设备准备,设置为默认为全速
UINT8	EnableRootHubPort( UINT8 RootHubIndex );   // 使能ROOT-HUB端口,相应的bUH_PORT_EN置1开启端口,设备断开可能导致返回失败
void	SelectHubPort( UINT8 RootHubIndex, UINT8 HubPortIndex );  // HubPortIndex=0选择操作指定的ROOT-HUB端口,否则选择操作指定的ROOT-HUB端口的外部HUB的指定端口
UINT8	WaitUSB_Interrupt( void );                 // 等待USB中断
                                                 // CH559传输事务,输入目的端点地址/PID令牌,同步标志,以20uS为单位的NAK重试总时间(0则不重试,0xFFFF无限重试),返回0成功,超时/出错重试
UINT8	USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout );  // endp_pid: 高4位是token_pid令牌, 低4位是端点地址
UINT8	HostCtrlTransfer( PUINT8X DataBuf, PUINT8I RetLen );  // 执行控制传输,8字节请求码在pSetupReq中,DataBuf为可选的收发缓冲区
                                                 // 如果需要接收和发送数据,那么DataBuf需指向有效缓冲区用于存放后续数据,实际成功收发的总长度返回保存在ReqLen指向的字节变量中
void	CopySetupReqPkg( PUINT8C pReqPkt );        // 复制控制传输的请求包
UINT8	CtrlGetDeviceDescr( void );                // 获取设备描述符,返回在TxBuffer中
UINT8	CtrlGetConfigDescr( void );                // 获取配置描述符,返回在TxBuffer中
UINT8	CtrlSetUsbAddress( UINT8 addr );           // 设置USB设备地址
UINT8	CtrlSetUsbConfig( UINT8 cfg );             // 设置USB设备配置
UINT8	CtrlClearEndpStall( UINT8 endp );          // 清除端点STALL
UINT8	AnalyzeHidIntEndp( PUINT8X buf );          // 从描述符中分析出HID中断端点的地址
UINT8	InitRootDevice( UINT8 RootHubIndex );      // 初始化指定ROOT-HUB端口的USB设备,输入: 内置HUB端口号0/1
UINT8	EnumAllRootDevice( void );                 // 枚举所有ROOT-HUB端口的USB设备
UINT8	InitDevOnHub( UINT8 RootHubIndex, UINT8 HubPortIndex );  // 初始化枚举外部HUB后的二级USB设备
UINT8	EnumHubPort( UINT8 RootHubIndex );         // 枚举指定ROOT-HUB端口上的外部HUB集线器的各个端口,检查各端口有无连接或移除事件并初始化二级USB设备
UINT8	EnumAllHubPort( void );                    // 枚举所有ROOT-HUB端口下外部HUB后的二级USB设备
UINT16	SearchTypeDevice( UINT8 type );          // 在ROOT-HUB以及外部HUB各端口上搜索指定类型的设备所在的端口号,输出端口号为0xFFFF则未搜索到
                                                 // 输出高8位为ROOT-HUB端口号,低8位为外部HUB的端口号,低8位为0则设备直接在ROOT-HUB端口上
void	mInitSTDIO( void );                        //为printf和getkey输入输出初始化串口
void	InitUSB_Host( void );                      // 初始化USB主机
void GetKey(UINT16 Port_Ep);                     //获取键值

void	mDelayuS( UINT16 n )  // 以uS为单位延时
{
	while ( n ) {  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 34000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 36000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 38000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 40000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}

void	mDelaymS( UINT16 n )  // 以mS为单位延时
{
	while ( n ) {
		mDelayuS( 1000 );
		-- n;
	}
}

void	DisableRootHubPort( UINT8 RootHubIndex )  // 关闭指定的ROOT-HUB端口,实际上硬件已经自动关闭,此处只是清除一些结构状态
{
	RootHubDev[ RootHubIndex ].DeviceStatus = ROOT_DEV_DISCONNECT;
	RootHubDev[ RootHubIndex ].DeviceAddress = 0x00;
	if ( RootHubIndex == 1 ) UHUB1_CTRL = 0x00;  // 清除有关HUB1的控制数据,实际上不需要清除
	else UHUB0_CTRL = 0x00;  // 清除有关HUB0的控制数据,实际上不需要清除
}

UINT8	AnalyzeRootHub( void )   // 分析ROOT-HUB状态,处理ROOT-HUB端口的设备插拔事件
// 返回ERR_SUCCESS为没有情况,返回ERR_USB_CONNECT为检测到新连接,返回ERR_USB_DISCON为检测到断开
{ //处理HUB端口的插拔事件,如果设备拔出,函数中调用DisableRootHubPort()函数,将端口关闭,插入事件,置相应端口的状态位
	UINT8	s;
	s = ERR_SUCCESS;
	if ( USB_HUB_ST & bUHS_H0_ATTACH ) {  // 设备存在
		if ( RootHubDev[0].DeviceStatus == ROOT_DEV_DISCONNECT  // 检测到有设备插入
			|| ( UHUB0_CTRL & bUH_PORT_EN ) == 0x00 ) {  // 检测到有设备插入,但尚未允许,说明是刚插入
			DisableRootHubPort( 0 );  // 关闭端口
			RootHubDev[0].DeviceSpeed = USB_HUB_ST & bUHS_DM_LEVEL ? 0 : 1;
			RootHubDev[0].DeviceStatus = ROOT_DEV_CONNECTED;  //置连接标志
#ifdef UART0_DEBUG
			printf( "HUB 0 dev in\n" );
#endif	
			s = ERR_USB_CONNECT;
		}
	}
	else if ( RootHubDev[0].DeviceStatus >= ROOT_DEV_CONNECTED ) {  //检测到设备拔出
		DisableRootHubPort( 0 );  // 关闭端口
#ifdef UART0_DEBUG
		printf( "HUB 0 dev out\n" );
#endif
		if ( s == ERR_SUCCESS ) s = ERR_USB_DISCON;
	}
	if ( USB_HUB_ST & bUHS_H1_ATTACH ) {  // 设备存在
		if ( RootHubDev[1].DeviceStatus == ROOT_DEV_DISCONNECT  // 检测到有设备插入
			|| ( UHUB1_CTRL & bUH_PORT_EN ) == 0x00 ) {  // 检测到有设备插入,但尚未允许,说明是刚插入
			DisableRootHubPort( 1 );  // 关闭端口
			RootHubDev[1].DeviceSpeed = USB_HUB_ST & bUHS_HM_LEVEL ? 0 : 1;
			RootHubDev[1].DeviceStatus = ROOT_DEV_CONNECTED;  //置连接标志
#ifdef UART0_DEBUG
			printf( "HUB 1 dev in\n" );
#endif
			s = ERR_USB_CONNECT;
		}
	}
	else if ( RootHubDev[1].DeviceStatus >= ROOT_DEV_CONNECTED ) {  //检测到设备拔出
		DisableRootHubPort( 1 );  // 关闭端口
#ifdef UART0_DEBUG
		printf( "HUB 1 dev out\n" );
#endif
		if ( s == ERR_SUCCESS ) s = ERR_USB_DISCON;
	}
	return( s );
}

void	SetHostUsbAddr( UINT8 addr )  // 设置USB主机当前操作的USB设备地址
{
	USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | addr & 0x7F;
}

void	SetUsbSpeed( UINT8 FullSpeed )  // 设置当前USB速度
{
	if ( FullSpeed ) {  // 全速
		USB_CTRL &= ~ bUC_LOW_SPEED;  // 全速
		UH_SETUP &= ~ bUH_PRE_PID_EN;  // 禁止PRE PID
	}
	else {
		USB_CTRL |= bUC_LOW_SPEED;  // 低速
	}
}

void	ResetRootHubPort( UINT8 RootHubIndex )  // 检测到设备后,复位相应端口的总线,为枚举设备准备
{
	UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;  /* USB设备的端点0的最大包尺寸 */
	SetHostUsbAddr( 0x00 );
	if ( RootHubIndex == 1 ) {
		UHUB1_CTRL = UHUB1_CTRL | bUH_BUS_RESET;  // 开始复位
		mDelaymS( 15 );  // 复位时间10mS到20mS
		UHUB1_CTRL = UHUB1_CTRL & (~ bUH_BUS_RESET);  // 结束复位
	}
	else {
		UHUB0_CTRL = UHUB0_CTRL | bUH_BUS_RESET;  // 开始复位
		mDelaymS( 15 );  // 复位时间10mS到20mS
		UHUB0_CTRL = UHUB0_CTRL & (~ bUH_BUS_RESET);  // 结束复位
	}
	mDelayuS( 250 );
	UIF_DETECT = 0;  // 清中断标志
}

UINT8	EnableRootHubPort( UINT8 RootHubIndex )  // 使能ROOT-HUB端口,相应的bUH_PORT_EN置1开启端口,设备断开可能导致返回失败
{
	if ( RootHubDev[ RootHubIndex ].DeviceStatus < ROOT_DEV_CONNECTED ) RootHubDev[ RootHubIndex ].DeviceStatus = ROOT_DEV_CONNECTED;
	if ( RootHubIndex == 1 ) {
		if ( USB_HUB_ST & bUHS_H1_ATTACH ) {  // 有设备
			if ( ( UHUB1_CTRL & bUH_PORT_EN ) == 0x00 ) {  // 尚未使能
				RootHubDev[1].DeviceSpeed = USB_HUB_ST & bUHS_HM_LEVEL ? 0 : 1;
				if ( RootHubDev[1].DeviceSpeed == 0 ) UHUB1_CTRL |= bUH_LOW_SPEED;  // 低速			
			}	
			UHUB1_CTRL |= bUH_PORT_EN;  //使能HUB端口			
			return( ERR_SUCCESS );
		}
	}
	else {
		if ( USB_HUB_ST & bUHS_H0_ATTACH ) {  // 有设备
			if ( ( UHUB0_CTRL & bUH_PORT_EN ) == 0x00 ) {  // 尚未使能
				RootHubDev[0].DeviceSpeed = USB_HUB_ST & bUHS_DM_LEVEL ? 0 : 1;
				if ( RootHubDev[0].DeviceSpeed == 0 ) UHUB0_CTRL |= bUH_LOW_SPEED;  // 低速			
			}		
			UHUB0_CTRL |= bUH_PORT_EN;  //使能HUB端口
			return( ERR_SUCCESS );
		}
	}
	return( ERR_USB_DISCON );
}

void	SelectHubPort( UINT8 RootHubIndex, UINT8 HubPortIndex )  // HubPortIndex=0选择操作指定的ROOT-HUB端口,否则选择操作指定的ROOT-HUB端口的外部HUB的指定端口
{
	if ( HubPortIndex ) {  // 选择操作指定的ROOT-HUB端口的外部HUB的指定端口
		SetHostUsbAddr( DevOnHubPort[RootHubIndex][HubPortIndex-1].DeviceAddress );  // 设置USB主机当前操作的USB设备地址
		if ( DevOnHubPort[RootHubIndex][HubPortIndex-1].DeviceSpeed == 0 ) {  // 通过外部HUB与低速USB设备通讯需要前置ID
			UH_SETUP |= bUH_PRE_PID_EN;  // 启用PRE PID
		}
		SetUsbSpeed( DevOnHubPort[RootHubIndex][HubPortIndex-1].DeviceSpeed );  // 设置当前USB速度
	}
	else {  // 选择操作指定的ROOT-HUB端口
		SetHostUsbAddr( RootHubDev[RootHubIndex].DeviceAddress );  // 设置USB主机当前操作的USB设备地址
		SetUsbSpeed( RootHubDev[RootHubIndex].DeviceSpeed );  // 设置当前USB速度
	}
	RootHubId = RootHubIndex ? 1 : 0;
}

UINT8	WaitUSB_Interrupt( void )  // 等待USB中断
{
	UINT16	i;
	for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- );
	return( UIF_TRANSFER ? ERR_SUCCESS : ERR_USB_UNKNOWN );
}

// CH559传输事务,输入目的端点地址/PID令牌,同步标志,以20uS为单位的NAK重试总时间(0则不重试,0xFFFF无限重试),返回0成功,超时/出错重试
UINT8	USBHostTransact( UINT8 endp_pid, UINT8 tog, UINT16 timeout )  // endp_pid: 高4位是token_pid令牌, 低4位是端点地址
{  // 本子程序着重于易理解,而在实际应用中,为了提供运行速度,应该对本子程序代码进行优化
//	UINT8	TransRetry;
#define	TransRetry	UEP0_T_LEN	// 节约内存
	UINT8	s, r;
	UINT16	i;
	UH_RX_CTRL = UH_TX_CTRL = tog;
	TransRetry = 0;
	do {
		UH_EP_PID = endp_pid;  // 指定令牌PID和目的端点号
		UIF_TRANSFER = 0;  // 允许传输
		for ( i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i -- );
		UH_EP_PID = 0x00;  // 停止USB传输
		if ( UIF_TRANSFER == 0 ) return( ERR_USB_UNKNOWN );
		if ( UIF_DETECT ) {  // USB设备插拔事件
			UIF_DETECT = 0;  // 清中断标志
			s = AnalyzeRootHub( );   // 分析ROOT-HUB状态
			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
			if ( RootHubDev[RootHubId].DeviceStatus == ROOT_DEV_DISCONNECT ) return( ERR_USB_DISCON );  // USB设备断开事件
			if ( RootHubDev[RootHubId].DeviceStatus == ROOT_DEV_CONNECTED ) return( ERR_USB_CONNECT );  // USB设备连接事件
			mDelayuS( 200 );  // 等待传输完成
		}
		if ( UIF_TRANSFER ) {  // 传输完成
			if ( U_TOG_OK ) return( ERR_SUCCESS );
			r = USB_INT_ST & MASK_UIS_H_RES;  // USB设备应答状态
			if ( r == USB_PID_STALL ) return( r | ERR_USB_TRANSFER );
			if ( r == USB_PID_NAK ) {
				if ( timeout == 0 ) return( r | ERR_USB_TRANSFER );
				if ( timeout < 0xFFFF ) timeout --;
				-- TransRetry;
			}
			else switch ( endp_pid >> 4 ) {
				case USB_PID_SETUP:
				case USB_PID_OUT:
//					if ( U_TOG_OK ) return( ERR_SUCCESS );
//					if ( r == USB_PID_ACK ) return( ERR_SUCCESS );
//					if ( r == USB_PID_STALL || r == USB_PID_NAK ) return( r | ERR_USB_TRANSFER );
					if ( r ) return( r | ERR_USB_TRANSFER );  // 不是超时/出错,意外应答
					break;  // 超时重试
				case USB_PID_IN:
//					if ( U_TOG_OK ) return( ERR_SUCCESS );
//					if ( tog ? r == USB_PID_DATA1 : r == USB_PID_DATA0 ) return( ERR_SUCCESS );
//					if ( r == USB_PID_STALL || r == USB_PID_NAK ) return( r | ERR_USB_TRANSFER );
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
		mDelayuS( 400 );
	} while ( ++ TransRetry < 3 );
	return( ERR_USB_TRANSFER );  // 应答超时
}

UINT8	HostCtrlTransfer( PUINT8X DataBuf, PUINT8 RetLen )  // 执行控制传输,8字节请求码在pSetupReq中,DataBuf为可选的收发缓冲区
// 如果需要接收和发送数据,那么DataBuf需指向有效缓冲区用于存放后续数据,实际成功收发的总长度保存在ReqLen指向的字节变量中
{
	UINT8	s, RemLen, RxLen;
	PUINT8X	xdata	pBuf;
	PUINT8	xdata	pLen;
	pBuf = DataBuf;
	pLen = RetLen;
	mDelayuS( 200 );
	if ( pLen ) *pLen = 0;  // 实际成功收发的总长度
	UH_TX_LEN = sizeof( USB_SETUP_REQ );
	s = USBHostTransact( USB_PID_SETUP << 4 | 0x00, 0x00, 200000/20 );  // SETUP阶段,200mS超时
	if ( s != ERR_SUCCESS ) return( s );
	UH_RX_CTRL = UH_TX_CTRL = bUH_R_TOG | bUH_R_AUTO_TOG | bUH_T_TOG | bUH_T_AUTO_TOG;  // 默认DATA1
	UH_TX_LEN = 0x01;  // 默认无数据故状态阶段为IN
	RemLen = pSetupReq -> wLengthH ? 0xFF : pSetupReq -> wLengthL;
	if ( RemLen && pBuf ) {  // 需要收发数据
		if ( pSetupReq -> bRequestType & USB_REQ_TYP_IN ) {  // 收
			while ( RemLen ) {
				mDelayuS( 200 );
				s = USBHostTransact( USB_PID_IN << 4 | 0x00, UH_RX_CTRL, 200000/20 );  // IN数据
				if ( s != ERR_SUCCESS ) return( s );
				RxLen = USB_RX_LEN < RemLen ? USB_RX_LEN : RemLen;
				RemLen -= RxLen;
				if ( pLen ) *pLen += RxLen;  // 实际成功收发的总长度
				memcpy( pBuf, RxBuffer, RxLen );
				pBuf += RxLen;
        printf("%02x  \n",(UINT16)USB_RX_LEN);				
				if ( USB_RX_LEN == 0 || ( USB_RX_LEN & ( UsbDevEndp0Size - 1 ) ) ) break;  // 短包
			}
			UH_TX_LEN = 0x00;  // 状态阶段为OUT
		}
		else {  // 发
			while ( RemLen ) {
				mDelayuS( 200 );
				UH_TX_LEN = RemLen >= UsbDevEndp0Size ? UsbDevEndp0Size : RemLen;
				memcpy( TxBuffer, pBuf, UH_TX_LEN );
				pBuf += UH_TX_LEN;
				if(pBuf[1] == 0x09)//类命令
        {
            Set_Port = Set_Port^1 ? 1:0;
					  *pBuf = Set_Port;
#ifdef UART0_DEBUG
				    printf("SET_PORT  %02X  %02X ",(UINT16)(*pBuf),(UINT16)(Set_Port));
#endif
        }
				s = USBHostTransact( USB_PID_OUT << 4 | 0x00, UH_TX_CTRL, 200000/20 );  // OUT数据
				if ( s != ERR_SUCCESS ) return( s );
				RemLen -= UH_TX_LEN;
				if ( pLen ) *pLen += UH_TX_LEN;  // 实际成功收发的总长度
			}
		}
	}
	mDelayuS( 200 );
	s = USBHostTransact( ( UH_TX_LEN ? USB_PID_IN << 4 | 0x00: USB_PID_OUT << 4 | 0x00 ), bUH_R_TOG | bUH_T_TOG, 200000/20 );  // STATUS阶段
	if ( s != ERR_SUCCESS ) return( s );
	if ( UH_TX_LEN == 0 ) return( ERR_SUCCESS );  // 状态OUT
	if ( USB_RX_LEN == 0 ) return( ERR_SUCCESS );  // 状态IN,检查IN状态返回数据长度
	return( ERR_USB_BUF_OVER );  // IN状态阶段错误
}

void	CopySetupReqPkg( PUINT8C pReqPkt )  // 复制控制传输的请求包
{
	UINT8	i;
	for ( i = 0; i != sizeof( USB_SETUP_REQ ); i ++ ) {
		((PUINT8X)pSetupReq)[ i ] = *pReqPkt;
		pReqPkt ++;
	}
}

UINT8	CtrlGetDeviceDescr( void )  // 获取设备描述符,返回在TxBuffer中
{
	UINT8	s;
	UINT8	len;
	UsbDevEndp0Size = DEFAULT_ENDP0_SIZE;
	CopySetupReqPkg( SetupGetDevDescr );
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	UsbDevEndp0Size = ( (PXUSB_DEV_DESCR)TxBuffer ) -> bMaxPacketSize0;  // 端点0最大包长度,这是简化处理,正常应该先获取前8字节后立即更新UsbDevEndp0Size再继续
	if ( len < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL ) return( ERR_USB_BUF_OVER );  // 描述符长度错误
	return( ERR_SUCCESS );
}

UINT8	CtrlGetConfigDescr( void )  // 获取配置描述符,返回在TxBuffer中
{
	UINT8	s;
	UINT8D	len;
	CopySetupReqPkg( SetupGetCfgDescr );
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL ) return( ERR_USB_BUF_OVER );  // 返回长度错误
	len = ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL;
	if ( len > MAX_PACKET_SIZE ) return( ERR_USB_BUF_OVER );  // 返回长度错误
	CopySetupReqPkg( SetupGetCfgDescr );
	pSetupReq -> wLengthL = len;  // 完整配置描述符的总长度
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	if ( len < ( (PUSB_SETUP_REQ)SetupGetCfgDescr ) -> wLengthL || len < ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL ) return( ERR_USB_BUF_OVER );  // 描述符长度错误
	return( ERR_SUCCESS );
}

UINT8	CtrlSetUsbAddress( UINT8 addr )  // 设置USB设备地址
{
	UINT8	s;
	CopySetupReqPkg( SetupSetUsbAddr );
	pSetupReq -> wValueL = addr;  // USB设备地址
	s = HostCtrlTransfer( NULL, NULL );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	SetHostUsbAddr( addr );  // 设置USB主机当前操作的USB设备地址
	mDelaymS( 10 );  // 等待USB设备完成操作
	return( ERR_SUCCESS );
}

UINT8	CtrlSetUsbConfig( UINT8 cfg )  // 设置USB设备配置
{
	CopySetupReqPkg( SetupSetUsbConfig );
	pSetupReq -> wValueL = cfg;  // USB设备配置
	return( HostCtrlTransfer( NULL, NULL ) );  // 执行控制传输
}

UINT8	CtrlSetUsbRepot( UINT8 cfg )  // 设置USB设备配置
{
	CopySetupReqPkg( SetupSetKeyReport );
	return( HostCtrlTransfer( &cfg, NULL ) );  // 执行控制传输
}

UINT8	CtrlClearEndpStall( UINT8 endp )  // 清除端点STALL
{
	CopySetupReqPkg( SetupClrEndpStall );  // 清除端点的错误
	pSetupReq -> wIndexL = endp;  // 端点地址
	return( HostCtrlTransfer( NULL, NULL ) );  /* 执行控制传输 */
}

UINT8	CtrlGetHIDDeviceReport( UINT8 infc )  // 获取HID设备报表描述符,返回在TxBuffer中
{
	UINT8	s;
	UINT8	len;
  UINT8 tmp[]={0x21,0x0a,0x00,0x00,0x00,0x00,0x00,0x00};
  UINT8 tmp1[]={0x81,0x06,0x00,0x22,0x00,0x00,0xff,0x00};
  tmp[4] = infc;
	for ( s = 0; s != sizeof( tmp ); s ++ ) {
		   ((PUINT8X)pSetupReq)[ s ] = tmp[s];
	}
  s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
  tmp1[4] = infc;
	for ( s = 0; s != sizeof( tmp1 ); s ++ ) {
		   ((PUINT8X)pSetupReq)[ s ] = tmp1[s];
	}			
	s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	if ( s != ERR_SUCCESS ) return( s );
	return( ERR_SUCCESS );
}

UINT8	AnalyzeHidIntEndp( PUINT8X buf )  // 从描述符中分析出HID中断端点的地址
{
	UINT8	i, s, l;
	s = 0;
	for ( i = 0; i < ( (PXUSB_CFG_DESCR)buf ) -> wTotalLengthL; i += l ) {  // 搜索中断端点描述符,跳过配置描述符和接口描述符
		if ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bDescriptorType == USB_DESCR_TYP_ENDP  // 是端点描述符
			&& ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bmAttributes & USB_ENDP_TYPE_MASK ) == USB_ENDP_TYPE_INTER  // 是中断端点
			&& ( ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_DIR_MASK ) ) {  // 是IN端点
				s = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bEndpointAddress & USB_ENDP_ADDR_MASK;  // 中断端点的地址
				break;  // 可以根据需要保存wMaxPacketSize和bInterval
		}
		l = ( (PXUSB_ENDP_DESCR)(buf+i) ) -> bLength;  // 当前描述符长度,跳过
		if ( l > 16 ) break;
	}
	return( s );
}

UINT8	AnalyzeMouseReportDesc( PUINT8X buf,UINT16 len )  // 分析鼠标报表
{
  UINT16 i =0;
  UINT8 flagX,num;
  flagX = 0;
  while(i<len){
    if((buf[i]&Global_USAGE_PAGE) && (buf[i+1] == UsagePage_BUTTON)){
					printf("button     \n");
      i=i+2;
			while(1){			
				if(((buf[i]&Local_USAGE_MINIMUM) == Local_USAGE_MINIMUM) && (buf[i] <= 0x1B)){
					printf("button min %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节							
					i=i+1;							
					Mouse.Button.Usage_min = buf[i];							
				}
				else if(((buf[i]&Local_USAGE_MAXMUM) == Local_USAGE_MAXMUM) && (buf[i] <= 0x2B)){
					printf("button max %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
					i=i+1;							
					Mouse.Button.Usage_min = buf[i];		              							
				}		
				else if(((buf[i]&Global_LOGICAL_MINIMUM) == Global_LOGICAL_MINIMUM) && (buf[i] <= 0x17)){
					printf("button lmin %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
					i=i+1;							
					Mouse.Button.Logical_min = buf[i];		              							
				}	
				else if(((buf[i]&Global_LOGICAL_MAXIMUM) == Global_LOGICAL_MAXIMUM) && (buf[i] <= 0x27)){
					printf("button lmax %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
					i=i+1;							
					Mouse.Button.Logical_max = buf[i];		              							
				}	
				else if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
					printf("button cou %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
					i=i+1;							
					Mouse.Button.Size_bit = buf[i]*buf[i+2];
				  printf("%02x  %02x ",(UINT16)buf[i],(UINT16)buf[i+2]);				
					printf("Mouse.Button.Size_bit  %02x  \n",(UINT16)Mouse.Button.Size_bit);	
          if(Mouse.Button.Size_bit%8){	//有保留位			
          while(1){						
					  i=i+1;		
					  if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
						  num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
						  i=i+1;							
						  Mouse.Button.Size_reserved = buf[i]*buf[i+3];	
					    break;									
					    }
				   }
			   } 
         break; 				 
				}	
        i++;				
      }      
    }
		if((buf[i]==9)&&(buf[i+1]==Usage_X)){
					printf("xxx  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);	
			i=i+2;	
			if((buf[i]==9)&&(buf[i+1]==Usage_Y)){	
					printf("yyy  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);					
				i=i+2;								
			}
			if((buf[i]==9)&&(buf[i+1]==Usage_WHEEL)){	
					printf("wheel  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);					
				i=i+2;
				flagX = 0x55;								
			}	
      while(1){
				if(((buf[i]&Global_LOGICAL_MINIMUM) == Global_LOGICAL_MINIMUM) && (buf[i] <= 0x17)){
					printf("xx lmin  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);						
					num = buf[i]&0x03; 					
					if(num>=2) {Mouse.X_Y.Logical_min = buf[i+1];Mouse.X_Y.Logical_min <<= 8; Mouse.X_Y.Logical_min |= buf[i];i=i+2;}
					else {Mouse.X_Y.Logical_min = 0;Mouse.X_Y.Logical_min = buf[i];i=i+1;}											              							
				}	
				else if(((buf[i]&Global_LOGICAL_MAXIMUM) == Global_LOGICAL_MAXIMUM) && (buf[i] <= 0x27)){
					printf("xx lmax  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);					
					num = buf[i]&0x03; 				
					if(num>=2) {Mouse.X_Y.Logical_max = buf[i+1];Mouse.X_Y.Logical_max <<= 8; Mouse.X_Y.Logical_max |= buf[i];i=i+2;}
					else {Mouse.X_Y.Logical_max = 0;Mouse.X_Y.Logical_max = buf[i];i=i+1;}		              							
				}	 
				else if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节		
					printf("xx cou  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);				
					i=i+1;							
					Mouse.X_Y.Size_bit = buf[i]*buf[i+2];	
				  printf("%02x  %02x ",(UINT16)buf[i],(UINT16)buf[i+2]);				
					printf("Mouse.X_Y.Size_bit  %02x  \n",(UINT16)Mouse.X_Y.Size_bit);				
          if(Mouse.X_Y.Size_bit%8){	//有保留位			
          while(1){						
					  i=i+1;			
					  if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
						  num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
						  i=i+1;							
						  Mouse.X_Y.Size_reserved = buf[i]*buf[i+3];		
              break;							
					  }	
				   }
			    }
					if(flagX == 0x55){
						Mouse.Wheel_.Logical_min = Mouse.X_Y.Logical_min;					
						Mouse.Wheel_.Logical_max = Mouse.X_Y.Logical_max;	
						Mouse.Wheel_.Size_bit = Mouse.X_Y.Size_bit;					
						Mouse.Wheel_.Size_reserved = Mouse.X_Y.Size_reserved;							
					}	
          break;					
				}
        i++;					
			}	
    }	
		if((buf[i]==9)&&(buf[i+1]==Usage_WHEEL)&&(flagX == 0)){
					printf("wheel  %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);				
			i=i+2;	
      while(1){
			if(((buf[i]&Global_LOGICAL_MINIMUM) == Global_LOGICAL_MINIMUM) && (buf[i] <= 0x17)){
					printf("wheel lmin %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);
				num = buf[i]&0x03; 				
				if(num>=2) {Mouse.Wheel_.Logical_min = buf[i+1];Mouse.Wheel_.Logical_min <<= 8; Mouse.Wheel_.Logical_min |= buf[i];i=i+2;}
				else {Mouse.Wheel_.Logical_min = 0;Mouse.Wheel_.Logical_min = buf[i];i=i+1;}											              							
			}	
			else if(((buf[i]&Global_LOGICAL_MAXIMUM) == Global_LOGICAL_MAXIMUM) && (buf[i] <= 0x27)){
					printf("wheel  lmax %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);
				num = buf[i]&0x03; 					
				if(num>=2) {Mouse.Wheel_.Logical_max = buf[i+1];Mouse.Wheel_.Logical_max <<= 8; Mouse.Wheel_.Logical_max |= buf[i];i=i+2;}
				else {Mouse.Wheel_.Logical_max = 0;Mouse.Wheel_.Logical_max = buf[i];i=i+1;}		              							
			}	 
			else if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
				num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节	
					printf("wheel  cou %02x  %02x  \n",(UINT16)i,(UINT16)buf[i]);			
				i=i+1;							
				Mouse.Wheel_.Size_bit = buf[i]*buf[i+2];
				printf("%02x  %02x ",(UINT16)buf[i],(UINT16)buf[i+2]);				
				printf("Mouse.Wheel_.Size_bit  %02x  \n",(UINT16)Mouse.Wheel_.Size_bit);			
        if(Mouse.Wheel_.Size_bit%8){	//有保留位			
        while(1){						
					i=i+1;			
				  if((((buf[i]&Global_REPORT_SIZE) == Global_REPORT_SIZE) && (buf[i] <= 0x77)||(((buf[i]&Global_REPORT_COUNT) == Global_REPORT_COUNT)) && (buf[i] <= 0x97))){
					num = buf[i]&0x03; if(num > 1) return Len_Err; //一般是1字节						
					i=i+1;							
					Mouse.Wheel_.Size_reserved = buf[i]*buf[i+3];		
					break;
				  }
			   }				
				}	
       break;				
			}
			i++;
		}		
	}
		i=i+1;if(i>=len) return Success;	
  }
  return Unknow;
}

UINT8	InitRootDevice( UINT8 RootHubIndex )  // 初始化指定ROOT-HUB端口的USB设备
// 输入: 内置HUB端口号0/1
{
	UINT8	i, s, cfg, dv_cls, if_cls;
  UINT8 infcNum = 1;
  UINT8 cfg1 = 0;
  UINT16 hidReportLen;
#ifdef UART0_DEBUG
	printf( "Reset root hub %1d# port\n", (UINT16)RootHubIndex );
#endif
	mDelaymS( 10 );
	ResetRootHubPort( RootHubIndex );  // 检测到设备后,复位相应端口的USB总线
	for ( i = 0, s = 0; i < 100; i ++ ) {  // 等待USB设备复位后重新连接,100mS超时
		if ( EnableRootHubPort( RootHubIndex ) == ERR_SUCCESS ) {  // 使能ROOT-HUB端口
			i = 0;
			s ++;  // 计时等待USB设备连接后稳定
			if ( s > 15 ) break;  // 已经稳定连接15ms 
		}
 		mDelaymS( 1 );		
	}
  SelectHubPort( RootHubIndex, 0 );		
	if ( i ) {  // 复位后设备没有连接
		DisableRootHubPort( RootHubIndex );
#ifdef UART0_DEBUG
		printf( "Disable root hub %1d# port because of disconnect\n", (UINT16)RootHubIndex );
#endif
		return( ERR_USB_DISCON );
	}
#ifdef UART0_DEBUG
	printf( "GetDevDescr: " );
#endif
	s = CtrlGetDeviceDescr( );  // 获取设备描述符
	if ( s == ERR_SUCCESS ) {
#ifdef UART0_DEBUG
		for ( i = 0; i < ( (PUSB_SETUP_REQ)SetupGetDevDescr ) -> wLengthL; i ++ ) printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
		printf( "\n" ); // 显示出描述符
#endif
		dv_cls = ( (PXUSB_DEV_DESCR)TxBuffer ) -> bDeviceClass;  // 设备类代码
		s = CtrlSetUsbAddress( RootHubIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL );  // 设置USB设备地址,加上RootHubIndex可以保证2个HUB端口分配不同的地址
		if ( s == ERR_SUCCESS ) {
			RootHubDev[RootHubIndex].DeviceAddress = RootHubIndex + ( (PUSB_SETUP_REQ)SetupSetUsbAddr ) -> wValueL;  // 保存USB地址
#ifdef UART0_DEBUG
			printf( "GetCfgDescr: " );
#endif
			s = CtrlGetConfigDescr( );  // 获取配置描述符
			if ( s == ERR_SUCCESS ) {
        infcNum = ( (PXUSB_CFG_DESCR)TxBuffer )->bNumInterfaces;//接口数目				
				cfg = ( (PXUSB_CFG_DESCR)TxBuffer ) -> bConfigurationValue;
#ifdef UART0_DEBUG
				for ( i = 0; i < ( (PXUSB_CFG_DESCR)TxBuffer ) -> wTotalLengthL; i ++ ) printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
				printf("\n");
#endif
/* 分析配置描述符,获取端点数据/各端点地址/各端点大小等,更新变量endp_addr和endp_size等 */
				if_cls = ( (PXUSB_CFG_DESCR_LONG)TxBuffer ) -> itf_descr.bInterfaceClass;  // 接口类代码
        if ( dv_cls == 0x00 && if_cls == USB_DEV_CLASS_HID && ( (PXUSB_CFG_DESCR_LONG)TxBuffer ) -> itf_descr.bInterfaceSubClass <= 0x01 ) {  // 是HID类设备,键盘/鼠标等
					s = AnalyzeHidIntEndp( TxBuffer );  // 从描述符中分析出HID中断端点的地址
					RootHubDev[RootHubIndex].GpVar = s & USB_ENDP_ADDR_MASK ;  // 保存中断端点的地址,位7用于同步标志位,清0
					if_cls = ( (PXUSB_CFG_DESCR_LONG)TxBuffer ) -> itf_descr.bInterfaceProtocol;
          hidReportLen = TxBuffer[26];hidReportLen <<= 8;hidReportLen |= TxBuffer[25];	//只保留第一个报表长度，复合设备不管		
          printf( "hidReportLen:%02X \n", (UINT16)( hidReportLen ) );					
					s = CtrlSetUsbConfig( cfg );  // 设置USB设备配置
					if ( s == ERR_SUCCESS ) {
#ifdef UART0_DEBUG
						  printf( "GetHIDReport: " );
#endif				
              s = CtrlGetHIDDeviceReport(0);//获取报表描述符
						  if(s == ERR_SUCCESS)
							{
#ifdef UART0_DEBUG
				          for ( i = 0; i < hidReportLen; i++ ){
                    printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
                  }printf("\n");			
#endif								
              }
              if ( if_cls == 2 ){//分析鼠标报表
								printf("begin\n");
                	AnalyzeMouseReportDesc(TxBuffer,hidReportLen);	
                  printf( "%02x  %02x  %02x  \n", (UINT16)( Mouse.Button.Size_bit ) ,(UINT16)( Mouse.X_Y.Size_bit ),(UINT16)( Mouse.Wheel_.Size_bit ));      							
             	}		
              infcNum--;							
              while(infcNum >= 1){
#ifdef UART0_DEBUG
						  printf( "GetHIDReport: %02x ",(UINT16)infcNum );
#endif				
              s = CtrlGetHIDDeviceReport(infcNum);//获取报表描述符
						  if(s == ERR_SUCCESS)
							{
#ifdef UART0_DEBUG
                  i = 0;								
				          while(TxBuffer[i] != 0xc0){
                    printf( "x%02X ", (UINT16)( TxBuffer[i] ) );
                    i++;										
                  }printf("\n");			
#endif								
              }
              infcNum--;							
              }								
//						Set_Idle( );
//						需保存端点信息以便主程序进行USB传输
						RootHubDev[RootHubIndex].DeviceStatus = ROOT_DEV_SUCCESS;
						if ( if_cls == 1 ) {
							RootHubDev[RootHubIndex].DeviceType = DEV_TYPE_KEYBOARD;
//							进一步初始化,例如设备键盘指示灯LED等
              CtrlSetUsbRepot(1);						
							printf( "USB-Keyboard Ready\n" );
							SetUsbSpeed( 1 );  // 默认为全速
							return( ERR_SUCCESS );
						}
						else if ( if_cls == 2 ) {
							RootHubDev[RootHubIndex].DeviceType = DEV_TYPE_MOUSE;
//							为了以后查询鼠标状态,应该分析描述符,取得中断端口的地址,长度等信息
							printf( "USB-Mouse Ready\n" );
							SetUsbSpeed( 1 );  // 默认为全速
							return( ERR_SUCCESS );
						}
						s = ERR_USB_UNSUPPORT;
					}
				}
				else {   // 可以进一步分析
					s = CtrlSetUsbConfig( cfg );  // 设置USB设备配置
					if ( s == ERR_SUCCESS ) {//						需保存端点信息以便主程序进行USB传输
						RootHubDev[RootHubIndex].DeviceStatus = ROOT_DEV_SUCCESS;
						RootHubDev[RootHubIndex].DeviceStatus = dv_cls ? dv_cls : if_cls;
						SetUsbSpeed( 1 );  // 默认为全速
#ifdef UART0_DEBUG
	          printf( "Nonsupport Dev\n");    					
#endif							
						return( ERR_SUCCESS );  /* 未知设备初始化成功 */
					}
				}
			}
		}
	}
#ifdef UART0_DEBUG
	printf( "InitRootDev Err = %02X\n", (UINT16)s );
#endif
	RootHubDev[RootHubIndex].DeviceStatus = ROOT_DEV_FAILED;
	SetUsbSpeed( 1 );  // 默认为全速
	return( s );
}

UINT8	EnumAllRootDevice( void )  // 枚举所有ROOT-HUB端口的USB设备
{
	UINT8	s, RootHubIndex;
	for ( RootHubIndex = 0; RootHubIndex != 2; RootHubIndex ++ ) {
		if ( RootHubDev[RootHubIndex].DeviceStatus == ROOT_DEV_CONNECTED ) {   // 刚插入设备尚未初始化
			s = InitRootDevice( RootHubIndex );  // 初始化/枚举指定HUB端口的USB设备
			if ( s != ERR_SUCCESS ) return( s );
		}
	}
	return( ERR_SUCCESS );
}


UINT16	SearchTypeDevice( UINT8 type )  // 在ROOT-HUB以及外部HUB各端口上搜索指定类型的设备所在的端口号,输出端口号为0xFFFF则未搜索到
{// 输出高8位为ROOT-HUB端口号,低8位为外部HUB的端口号,低8位为0则设备直接在ROOT-HUB端口上
// 当然也可以根据USB的厂商VID产品PID进行搜索(事先要记录各设备的VID和PID),以及指定搜索序号
	UINT8	RootHubIndex, HubPortIndex;
	for ( RootHubIndex = 0; RootHubIndex != 2; RootHubIndex ++ ) {  // 现时搜索可以避免设备中途拔出而某些信息未及时更新的问题
		if ( RootHubDev[RootHubIndex].DeviceType == type && RootHubDev[RootHubIndex].DeviceStatus >= ROOT_DEV_SUCCESS ) GetKey((UINT16)RootHubIndex << 8 );  // 类型匹配且枚举成功,在ROOT-HUB端口上
		if ( RootHubDev[RootHubIndex].DeviceType == USB_DEV_CLASS_HUB && RootHubDev[RootHubIndex].DeviceStatus >= ROOT_DEV_SUCCESS ) {  // 外部集线器HUB且枚举成功
			for ( HubPortIndex = 1; HubPortIndex <= RootHubDev[RootHubIndex].GpVar; HubPortIndex ++ ) {  // 搜索外部HUB的各个端口
				if ( DevOnHubPort[RootHubIndex][HubPortIndex-1].DeviceType == type && DevOnHubPort[RootHubIndex][HubPortIndex-1].DeviceStatus >= ROOT_DEV_SUCCESS )
					GetKey( ( (UINT16)RootHubIndex << 8 ) | HubPortIndex );  // 类型匹配且枚举成功
			}
		}
	}
	return( 0xFFFF );
}

/* 为printf和getkey输入输出初始化串口 */
void	mInitSTDIO( void )
{
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;
	PCON |= SMOD;
	TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;  // 0X20
	T2MOD = T2MOD | bTMR_CLK | bT1_CLK;  // *12
	TH1 = 0-13;//0xF3;  /* 12MHz晶振, 4800bps*12=57600bps */
	TR1 = 1;
	TI = 1;
}

void	InitUSB_Host( void )  // 初始化USB主机
{
	UINT8	i;
	IE_USB = 0;
	USB_CTRL = bUC_HOST_MODE;  // 先设定模式
	USB_DEV_AD = 0x00;
	UH_EP_MOD = bUH_EP_TX_EN | bUH_EP_RX_EN ;
	UH_RX_DMA = RxBuffer;
	UH_TX_DMA = TxBuffer;
	UH_RX_CTRL = 0x00;
	UH_TX_CTRL = 0x00;
	USB_CTRL = bUC_HOST_MODE | bUC_INT_BUSY | bUC_DMA_EN;  // 启动USB主机及DMA,在中断标志未清除前自动暂停
//	UHUB0_CTRL = 0x00;
//	UHUB1_CTRL = 0x00;
	UH_SETUP = bUH_SOF_EN;
	USB_INT_FG = 0xFF;  // 清中断标志
	for ( i = 0; i != 2; i ++ ) DisableRootHubPort( i );  // 清空
	USB_INT_EN = bUIE_TRANSFER | bUIE_DETECT;
//	IE_USB = 1;  // 查询方式
}


UINT8 SETorOFFNumLock(PUINT8 buf)//NumLock的点灯判断
{
    UINT8 tmp[]={0x21,0x09,0x00,0x02,0x00,0x00,0x01,0x00};
    UINT8 len,s;
    if((buf[2]==0x53)&(buf[0]|buf[1]|buf[3]|buf[4]|buf[5]|buf[6]|buf[7]==0))
    {
	    for ( s = 0; s != sizeof( tmp ); s ++ ) {
		   ((PUINT8X)pSetupReq)[ s ] = tmp[s];
	    }
        s = HostCtrlTransfer( TxBuffer, &len );  // 执行控制传输
	      if ( s != ERR_SUCCESS ) return( s );    
    }
	return( ERR_SUCCESS );
}

void GetKey(UINT16 Port_Ep)
{
	UINT8	i, s, len, endp;
	UINT16	loc = Port_Ep;
	if ( Port_Ep != 0xFFFF ) {  // 找到了,如果有两个KeyBoard如何处理?
		i = (UINT8)( loc >> 8 );
		len = (UINT8)loc;
		SelectHubPort( i, len );  // 选择操作指定的ROOT-HUB端口,设置当前USB速度以及被操作设备的USB地址
		endp = len ? DevOnHubPort[i][len-1].GpVar : RootHubDev[i].GpVar;  // 中断端点的地址,位7用于同步标志位
		if ( endp & USB_ENDP_ADDR_MASK ) {  // 端点有效
			s = USBHostTransact( USB_PID_IN << 4 | endp & 0x7F, endp & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );  // CH559传输事务,获取数据,NAK不重试
			if ( s == ERR_SUCCESS ) {
				endp ^= 0x80;  // 同步标志翻转
				if ( len ) DevOnHubPort[i][len-1].GpVar = endp;  // 保存同步标志位
				else RootHubDev[i].GpVar = endp;
				len = USB_RX_LEN;  // 接收到的数据长度
				if ( len ) {
					printf("%04X data: ",loc);
					for ( i = 0; i < len; i ++ ) printf("x%02X ",(UINT16)(RxBuffer[i]) );
					printf("\n");
				}
			}
			else if ( s != ( USB_PID_NAK | ERR_USB_TRANSFER ) ) printf("Dev error %02x\n",(UINT16)s);  // 可能是断开了
		}
		else printf("Dev no interrupt endpoint\n");
		SetUsbSpeed( 1 );  // 默认为全速
	}	
}


main( ) {
	UINT8	s;
	UINT16	loc;
//     SAFE_MOD = 0x55;                                                   //开启安全模式
//     SAFE_MOD = 0xAA;
//     CLOCK_CFG |= bOSC_EN_XT;                                           //使能外部晶振
//     CLOCK_CFG &= ~bOSC_EN_INT;
// 	  CLOCK_CFG &= ~MASK_SYS_CK_DIV;
//     CLOCK_CFG |= 6;                                                    //配置系统时钟 
//     SAFE_MOD = 0xFF;                                                   //关闭安全模式
	mInitSTDIO( );  /* 为了让计算机通过串口监控演示过程 */
  memset(&Mouse,0,sizeof(Mouse));	
#ifdef UART0_DEBUG
	printf( "Start @ChipID=%02X\n", (UINT16)CHIP_ID );
#endif
	InitUSB_Host( );
	FoundNewDev = 0;
#ifdef UART0_DEBUG
	printf( "Wait Device In\n" );
#endif
	while ( 1 ) {
		s = ERR_SUCCESS;
		if ( UIF_DETECT ) {  // 如果有USB主机检测中断则处理
			UIF_DETECT = 0;  // 清中断标志
			s = AnalyzeRootHub( );   // 分析ROOT-HUB状态
			if ( s == ERR_USB_CONNECT ) FoundNewDev = 1;
		}

		if ( FoundNewDev || s == ERR_USB_CONNECT ) {  // 有新的USB设备插入
			FoundNewDev = 0;
			mDelaymS( 200 );  // 由于USB设备刚插入尚未稳定,故等待USB设备数百毫秒,消除插拔抖动
			s = EnumAllRootDevice( );  // 枚举所有ROOT-HUB端口的USB设备
			if ( s != ERR_SUCCESS ) printf( "EnumAllRootDev err = %02X\n", (UINT16)s );
		}
		loc = SearchTypeDevice( DEV_TYPE_KEYBOARD );  // 在ROOT-HUB以及外部HUB各端口上搜索指定类型的设备所在的端口号
		loc = SearchTypeDevice( DEV_TYPE_MOUSE );  // 在ROOT-HUB以及外部HUB各端口上搜索指定类型的设备所在的端口号
		SetUsbSpeed( 1 );  // 默认为全速		
	}
}

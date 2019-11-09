
/* 以下内容复制于CH375INC.H文件 */

/* Define for CH372 & CH375          */
/* Website:  http://winchiphead.com  */
/* Email:    tech@winchiphead.com    */
/* Author:   W.ch 2003.09            */

/* ********************************************************************************************************************* */
/* 硬件特性 */

#define	CH375_MAX_DATA_LEN	0x40			/* 最大数据包的长度, 内部缓冲区的长度 */

/* ********************************************************************************************************************* */
/* 命令代码 */

#define	CMD_RESET_ALL		0x05			/* 执行硬件复位 */

#define	CMD_CHECK_EXIST		0x06			/* 测试工作状态 */
/* 输入: 任意数据 */
/* 输出: 输入数据的按位取反 */

#define	CMD_SET_USB_ID		0x12			/* 设备方式: 设置USB厂商VID和产品PID */
/* 输入: 厂商ID低字节, 厂商ID高字节, 产品ID低字节, 产品ID高字节 */

#define	CMD_SET_USB_ADDR	0x13			/* 设置USB地址 */
/* 输入: 地址值 */

#define	CMD_SET_USB_MODE	0x15			/* 设置USB工作模式 */
/* 输入: 模式代码 */
/*       00H=未启用的设备方式, 01H=已启用的设备方式并且使用外部固件模式, 02H=已启用的设备方式并且使用内置固件模式, 03H=已启用的设备方式并且使用中断端点和内置固件模式 */
/*       04H=未启用的主机方式, 05H=已启用的主机方式, 06H=已启用的主机方式并且自动产生SOF包, 07H=已启用的主机方式并且复位USB总线 */
/* 输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

#define	CMD_SET_ENDP2		0x18			/* 设备方式: 设置USB端点0的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP3		0x19			/* 设备方式: 设置USB端点0的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP4		0x1A			/* 设备方式: 设置USB端点1的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP5		0x1B			/* 设备方式: 设置USB端点1的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP6		0x1C			/* 设置USB端点2/主机端点的接收器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但不返回ACK, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_SET_ENDP7		0x1D			/* 设置USB端点2/主机端点的发送器 */
/* 输入: 工作方式 */
/*           位7为1则位6为同步触发位, 否则同步触发位不变 */
/*           位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但无须应答, 1110-正忙NAK, 1111-错误STALL */

#define	CMD_GET_TOGGLE		0x0A			/* 获取OUT事务的同步状态 */
/* 输入: 数据1AH */
/* 输出: 同步状态 */
/*           位4为1则OUT事务同步, 否则OUT事务不同步 */

#define	CMD_GET_STATUS		0x22			/* 获取中断状态并取消中断请求 */
/* 输出: 中断状态 */

#define	CMD_UNLOCK_USB		0x23			/* 设备方式: 释放当前USB缓冲区 */

#define	CMD_RD_USB_DATA		0x28			/* 从当前USB中断的端点缓冲区读取数据块, 并释放缓冲区 */
/* 输出: 长度, 数据流 */

#define	CMD_WR_USB_DATA3	0x29			/* 设备方式: 向USB端点0的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define	CMD_WR_USB_DATA5	0x2A			/* 设备方式: 向USB端点1的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

#define	CMD_WR_USB_DATA7	0x2B			/* 向USB端点2的发送缓冲区写入数据块 */
/* 输入: 长度, 数据流 */

/* ************************************************************************** */
/* 以下命令用于USB主机方式, 只有CH375支持 */

#define	CMD_SET_BAUDRATE	0x02			/* 串口方式: 设置串口通讯波特率 */
/* 输入: 波特率分频系数, 波特率分频常数 */
/* 输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

#define	CMD_ABORT_NAK		0x17			/* 主机方式: 放弃当前NAK的重试 */

#define	CMD_SET_RETRY		0x0B			/* 主机方式: 设置USB事务操作的重试次数 */
/* 输入: 数据25H, 重试次数 */
/*                    位7为1则收到NAK时无限重试, 位3~位0为超时后的重试次数 */

#define	CMD_ISSUE_TOKEN		0x4F			/* 主机方式: 发出令牌, 执行事务 */
/* 输入: 事务属性 */
/*           低4位是令牌, 高4位是端点号 */
/* 输出中断 */

#define	CMD_CLR_STALL		0x41			/* 主机方式: 控制传输-清除端点错误 */
/* 输入: 端点号 */
/* 输出中断 */

#define	CMD_SET_ADDRESS		0x45			/* 主机方式: 控制传输-设置USB地址 */
/* 输入: 地址值 */
/* 输出中断 */

#define	CMD_GET_DESCR		0x46			/* 主机方式: 控制传输-获取描述符 */
/* 输入: 描述符类型 */
/* 输出中断 */

#define	CMD_SET_CONFIG		0x49			/* 主机方式: 控制传输-设置USB配置 */
/* 输入: 配置值 */
/* 输出中断 */

#define	CMD_DISK_INIT		0x51			/* 主机方式: 初始化USB存储器 */
/* 输出中断 */

#define	CMD_DISK_RESET		0x52			/* 主机方式: 复位USB存储器 */
/* 输出中断 */

#define	CMD_DISK_SIZE		0x53			/* 主机方式: 获取USB存储器的容量 */
/* 输出中断 */

#define	CMD_DISK_READ		0x54			/* 主机方式: 从USB存储器读数据块(以扇区512字节为单位) */
/* 输入: LBA扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
/* 输出中断 */

#define	CMD_DISK_RD_GO		0x55			/* 主机方式: 继续执行USB存储器的读操作 */
/* 输出中断 */

#define	CMD_DISK_WRITE		0x56			/* 主机方式: 向USB存储器写数据块(以扇区512字节为单位) */
/* 输入: LBA扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
/* 输出中断 */

#define	CMD_DISK_WR_GO		0x57			/* 主机方式: 继续执行USB存储器的写操作 */
/* 输出中断 */

/* ************************************************************************** */
/* 以下新增的V2.0命令码, 仅CH372A/CH375A支持 */

#define	CMD_GET_IC_VER		0x01			/* 获取芯片及固件版本 */
/* 输出: 版本号( 位7为1, 位6为0, 位5~位0为版本号 ) */
/*           CH375返回无效值为5FH, CH375A返回版本号的值为0A2H */

#define	CMD_ENTER_SLEEP		0x03			/* 进入睡眠状态 */

#define	CMD_RD_USB_DATA0	0x27			/* 从当前USB中断的端点缓冲区读取数据块 */
/* 输出: 长度, 数据流 */

#define	CMD_DELAY_100US		0x0F			/* 并口方式: 延时100uS */
/* 输出: 延时期间输出0, 延时结束输出非0 */

#define	CMD_CHK_SUSPEND		0x0B			/* 设备方式: 设置检查USB总线挂起状态的方式 */
/* 输入: 数据10H, 检查方式 */
/*                    00H=不检查USB挂起, 04H=以50mS为间隔检查USB挂起, 05H=以10mS为间隔检查USB挂起 */

#define	CMD_SET_SYS_FREQ	0x04			/* 设置系统工作频率 */
/* 输入: 频率 */
/*           00H=12MHz, 01H=1.5MHz */

/* ************************************************************************** */
/* 以下改进的V2.0命令码, 用于USB主机方式, 仅CH375A支持 */

/*#define	CMD_SET_RETRY		0x0B*/			/* 主机方式: 设置USB事务操作的重试次数 */
/* 输入: 数据25H, 重试次数 */
/*                    位7为0则收到NAK时不重试, 位7为1位6为0则收到NAK时无限重试, 位7为1位6为1则收到NAK时重试200mS, 位5~位0为超时后的重试次数 */

/* ************************************************************************** */
/* 以下新增的V2.0命令码, 用于USB主机方式, 仅CH375A支持 */

#define	CMD_TEST_CONNECT	0x16			/* 主机方式: 检查USB设备连接状态 */
/* 输出: 状态( USB_INT_CONNECT或USB_INT_DISCONNECT, 其它值说明操作未完成 ) */

#define	CMD_AUTO_SETUP		0x4D			/* 主机方式: 自动配置USB设备 */
/* 输出中断 */

#define	CMD_ISSUE_TKN_X		0x4E			/* 主机方式: 发出同步令牌, 执行事务 */
/* 输入: 同步标志, 事务属性 */
/*           同步标志的位7为主机端点IN的同步触发位, 位6为主机端点OUT的同步触发位, 位5~位0必须为0 */
/*           事务属性的低4位是令牌, 高4位是端点号 */
/* 输出中断 */

#define	CMD_SET_DISK_LUN	0x0B			/* 主机方式: 设置USB存储器的当前逻辑单元号 */
/* 输入: 数据34H, 新的当前逻辑单元号(00H-0FH) */

#define	CMD_DISK_BOC_CMD	0x50			/* 主机方式: 对USB存储器执行BulkOnly传输协议的命令 */
/* 输出中断 */

#define	CMD_DISK_INQUIRY	0x58			/* 主机方式: 查询USB存储器特性 */
/* 输出中断 */

#define	CMD_DISK_READY		0x59			/* 主机方式: 检查USB存储器就绪 */
/* 输出中断 */

#define	CMD_DISK_R_SENSE	0x5A			/* 主机方式: 检查USB存储器错误 */
/* 输出中断 */

#define	CMD_DISK_MAX_LUN	0x5D			/* 主机方式: 获取USB存储器最大逻辑单元号 */
/* 输出中断 */

/* ********************************************************************************************************************* */
/* 操作状态 */

#define	CMD_RET_SUCCESS		0x51			/* 命令操作成功 */
#define	CMD_RET_ABORT		0x5F			/* 命令操作失败 */

/* ********************************************************************************************************************* */
/* USB中断状态 */

/* 以下状态代码为特殊事件中断, 仅CH372A/CH375A支持, 如果通过CMD_CHK_SUSPEND启用USB总线挂起检查, 那么必须处理USB总线挂起和睡眠唤醒的中断状态 */
#define	USB_INT_USB_SUSPEND	0x05			/* USB总线挂起事件 */
#define	USB_INT_WAKE_UP		0x06			/* 从睡眠中被唤醒事件 */

/* 以下状态代码0XH用于USB设备方式 */
/*   内置固件模式下只需要处理: USB_INT_EP1_IN, USB_INT_EP2_OUT, USB_INT_EP2_IN, 对于CH372A/CH375A还需要处理: USB_INT_EP1_OUT */
/*   位7-位4为0000 */
/*   位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
/*   位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2, 11=USB总线复位 */
#define	USB_INT_EP0_SETUP	0x0C			/* USB端点0的SETUP */
#define	USB_INT_EP0_OUT		0x00			/* USB端点0的OUT */
#define	USB_INT_EP0_IN		0x08			/* USB端点0的IN */
#define	USB_INT_EP1_OUT		0x01			/* USB端点1的OUT */
#define	USB_INT_EP1_IN		0x09			/* USB端点1的IN */
#define	USB_INT_EP2_OUT		0x02			/* USB端点2的OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB端点2的IN */
/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB总线复位 */
#define	USB_INT_BUS_RESET1	0x03			/* USB总线复位 */
#define	USB_INT_BUS_RESET2	0x07			/* USB总线复位 */
#define	USB_INT_BUS_RESET3	0x0B			/* USB总线复位 */
#define	USB_INT_BUS_RESET4	0x0F			/* USB总线复位 */

/* 以下状态代码2XH-3XH用于USB主机方式的通讯失败代码, 仅CH375/CH375A支持 */
/*   位7-位6为00 */
/*   位5为1 */
/*   位4指示当前接收的数据包是否同步 */
/*   位3-位0指示导致通讯失败时USB设备的应答: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=超时 */
/* USB_INT_RET_ACK	0x001X0010B */			/* 错误:对于IN事务返回ACK */
/* USB_INT_RET_NAK	0x001X1010B */			/* 错误:返回NAK */
/* USB_INT_RET_STALL	0x001X1110B */		/* 错误:返回STALL */
/* USB_INT_RET_DATA0	0x001X0011B */		/* 错误:对于OUT/SETUP事务返回DATA0 */
/* USB_INT_RET_DATA1	0x001X1011B */		/* 错误:对于OUT/SETUP事务返回DATA1 */
/* USB_INT_RET_TOUT	0x001XXX00B */			/* 错误:返回超时 */
/* USB_INT_RET_TOGX	0x0010X011B */			/* 错误:对于IN事务返回数据不同步 */
/* USB_INT_RET_PID	0x001XXXXXB */			/* 错误:未定义 */

/* 以下状态代码1XH用于USB主机方式的操作状态代码, 仅CH375/CH375A支持 */

#define	USB_INT_SUCCESS		0x14			/* USB事务或者传输操作成功 */
#define	USB_INT_CONNECT		0x15			/* 检测到USB设备连接事件 */
#define	USB_INT_DISCONNECT	0x16			/* 检测到USB设备断开事件 */
#define	USB_INT_BUF_OVER	0x17			/* USB控制传输的数据太多, 缓冲区溢出 */
#define	USB_INT_DISK_READ	0x1D			/* USB存储器读数据块, 请求数据读出 */
#define	USB_INT_DISK_WRITE	0x1E			/* USB存储器写数据块, 请求数据写入 */
#define	USB_INT_DISK_ERR	0x1F			/* USB存储器操作失败 */

/* ********************************************************************************************************************* */
/* 常用USB定义 */

/* USB的包标识PID, 主机方式可能用到 */
#define	DEF_USB_PID_NULL	0x00			/* 保留PID, 未定义 */
#define	DEF_USB_PID_SOF		0x05
#define	DEF_USB_PID_SETUP	0x0D
#define	DEF_USB_PID_IN		0x09
#define	DEF_USB_PID_OUT		0x01
#define	DEF_USB_PID_ACK		0x02
#define	DEF_USB_PID_NAK		0x0A
#define	DEF_USB_PID_STALL	0x0E
#define	DEF_USB_PID_DATA0	0x03
#define	DEF_USB_PID_DATA1	0x0B
#define	DEF_USB_PID_PRE		0x0C

/* USB请求类型, 外置固件模式可能用到 */
#define	DEF_USB_REQ_READ	0x80			/* 控制读操作 */
#define	DEF_USB_REQ_WRITE	0x00			/* 控制写操作 */
#define	DEF_USB_REQ_TYPE	0x60			/* 控制请求类型 */
#define	DEF_USB_REQ_STAND	0x00			/* 标准请求 */
#define	DEF_USB_REQ_CLASS	0x20			/* 设备类请求 */
#define	DEF_USB_REQ_VENDOR	0x40			/* 厂商请求 */
#define	DEF_USB_REQ_RESERVE	0x60			/* 保留请求 */

/* USB标准设备请求, RequestType的位6位5=00(Standard), 外置固件模式可能用到 */
#define	DEF_USB_CLR_FEATURE	0x01
#define	DEF_USB_SET_FEATURE	0x03
#define	DEF_USB_GET_STATUS	0x00
#define	DEF_USB_SET_ADDRESS	0x05
#define	DEF_USB_GET_DESCR	0x06
#define	DEF_USB_SET_DESCR	0x07
#define	DEF_USB_GET_CONFIG	0x08
#define	DEF_USB_SET_CONFIG	0x09
#define	DEF_USB_GET_INTERF	0x0A
#define	DEF_USB_SET_INTERF	0x0B
#define	DEF_USB_SYNC_FRAME	0x0C

/* ********************************************************************************************************************* */

// 以下为新的定义,原来不属于CH375INC.H文件 *******************************************

#define CH375_BLOCK_SIZE		64		/* CH375 maximum data block size */
#define CH375_BLK_PER_SEC		8		/* CH375 block per sector, SECTOR_SIZE/CH375_BLOCK_SIZE */

/* ********************************************************************************************************************* */
#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	/* 生成指定时分秒的文件时间数据 */
#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	/* 生成指定年月日的文件日期数据 */

/*
// RBC commands
*/
#define RBC_CMD_FORMAT					0x04
#define RBC_CMD_READCAPACITY			0x25
#define RBC_CMD_STARTSTOPUNIT			0x1B
#define RBC_CMD_SYNCCACHE				0x35

/*
// SPC-2 commands
*/
#define SPC_CMD_MODESELECT6				0x15
#define SPC_CMD_MODESENSE6				0x1A
#define SPC_CMD_PERSISTANTRESERVIN		0x5E
#define SPC_CMD_PERSISTANTRESERVOUT		0x5F
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL	0x1E
#define SPC_CMD_RELEASE6				0x17
#define SPC_CMD_REQUESTSENSE			0x03
#define SPC_CMD_RESERVE6				0x16
#define SPC_CMD_TESTUNITREADY			0x00
#define SPC_CMD_WRITEBUFFER				0x3B
#define SPC_CMD_READLONG				0x23

typedef struct _MODE_SENSE_10 {
		INT8 OperationCode;
		INT8 Reserved1;
		INT8 PageCode : 6;
		INT8 Pc : 2;
		INT8 Reserved2[4];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved3[3];
} MODE_SENSE_10, *PMODE_SENSE_10;

typedef struct _MODE_SELECT_10 {
		INT8 OperationCode;
		INT8 Reserved1 : 4;
		INT8 PFBit : 1;
		INT8 Reserved2 : 3;
		INT8 Reserved3[5];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved4[3];
} MODE_SELECT_10, *PMODE_SELECT_10;
/*
////////////////////////////////////////////////////////////////////////////////////
// Command Descriptor Block
//      _RBC : Reduced Block Command
//      _SPC : SPC-2 SCSI primary Command - 2
////////////////////////////////////////////////////////////////////////////////////
*/

typedef struct _GENERIC_CDB {
	INT8 OperationCode;
    INT8 Reserved[15];
} GENERIC_CDB,*PGENERIC_CDB;

typedef struct _GENERIC_RBC {
	INT8 OperationCode;
    INT8 Reserved[8];
	INT8 Control;
} GENERIC_RBC,*PGENERIC_RBC;

/*
// format unit
*/
typedef struct _FORMAT_RBC {
	INT8 OperationCode;	/* 04H */
	INT8 VendorSpecific;
	INT8 Increment : 1;
	INT8 PercentorTime : 1;
	INT8 Progress : 1;
    INT8 Immediate : 1;
	INT8 VendorSpecific1 : 4;
	INT8 Reserved2[2];
	INT8 Control;
} FORMAT_RBC, *PFORMAT_RBC;

//////////////////////////////////////////////////
typedef struct _READ_LONG_CMD{
    INT8 OperationCode;
    INT8 LogicalUnitNum	:3;
    INT8 RES_1		:5;
    INT8 RES_2;
    INT8 RES_3;
    INT8 RES_4;
    INT8 RES_5;
    INT8 RES_6;
    INT16 AllocationLen;
    INT8 RES_7;
    INT8 RES_8;
    INT8 RES_9;
}READ_LONG_CMD, *PREAD_LONG_CMD;

typedef struct _READ_LONG {
    INT8 RES_1;
    INT8 RES_2;
    INT8 RES_3;
    INT8 CAP_LIST_LEN;
    INT8 LBA_3;
    INT8 LBA_2;
    INT8 LBA_1;
    INT8 LBA_0;
    INT8 Descripter;
    INT8 BlockLen_2;
    INT8 BlockLen_1;
    INT8 BlockLen_0;
} READ_LONG, *PREAD_LONG;
/*
// START_STOP_UNIT
*/
typedef struct _START_STOP_RBC {
    INT8 OperationCode;    /*1BH*/
    INT8 Immediate: 1;
    INT8 Reserved1 : 7;
    INT8 Reserved2[2];
	union _START_STOP_FLAGS
    {
        struct
        {
            INT8 Start          : 1;
            INT8 LoadEject      : 1;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits0;

        struct
        {
            INT8 MediumState    : 2;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits1;
    } Flags;
    INT8 Control;
} START_STOP_RBC, *PSTART_STOP_RBC;

/*
// Synchronize Cache
*/
typedef struct _SYNCHRONIZE_CACHE_RBC {

	INT8 OperationCode;    /* 0x35 */
	INT8 Reserved[8];
	INT8 Control;

} SYNCHRONIZE_CACHE_RBC, *PSYNCHRONIZE_CACHE_RBC;


/*
//***********************************************************************************
// SPC-2 of SCSI-3 commands
//***********************************************************************************
*/

typedef struct _STD_INQUIRYDATA {
    INT8 DeviceType : 5;
    INT8 Reserved0 : 3;

    INT8 Reserved1 : 7;
    INT8 RemovableMedia : 1;

    INT8 Reserved2;

    INT8 Reserved3 : 5;
    INT8 NormACA : 1;
    INT8 Obsolete0 : 1;
    INT8 AERC : 1;

    INT8 Reserved4[3];

    INT8 SoftReset : 1;
    INT8 CommandQueue : 1;
	INT8 Reserved5 : 1;
	INT8 LinkedCommands : 1;
	INT8 Synchronous : 1;
	INT8 Wide16Bit : 1;
	INT8 Wide32Bit : 1;
	INT8 RelativeAddressing : 1;

	INT8 VendorId[8];

	INT8 ProductId[16];

	INT8 ProductRevisionLevel[4];

/*//  Above is 36 bytes//  can be tranmitted by Bulk*/
    INT8 VendorSpecific[20];
    INT8 InfoUnitSupport : 1;
    INT8 QuickArbitSupport : 1;
    INT8 Clocking : 2;
    INT8 Reserved6 : 4;

    INT8  Reserved7 ;
    INT16 VersionDescriptor[8] ;

    INT8 Reserved8[22];
} STD_INQUIRYDATA, *PSTD_INQUIRYDATA;

typedef struct _SERIALNUMBER_PAGE {
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;
    INT8 SerialNumber[24] ;

}VPD_SERIAL_PAGE,* PVPD_SERIAL_PAGE;

#define ASCII_ID_STRING 32
typedef struct _ID_DESCRIPTOR {
	INT8   CodeSet : 4;
	INT8   Reserved0 : 4;

	INT8   IDType : 4;
    INT8   Association : 2;
    INT8   Reserved1 : 2;

    INT8   Reserved2;

	INT8   IDLength ;
	INT8   AsciiID[ASCII_ID_STRING];
} ASCII_ID_DESCRIPTOR,* PASCII_ID_DESCRIPTOR;

typedef struct _DEVICE_ID_PAGE
{
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;

    ASCII_ID_DESCRIPTOR   AsciiIdDescriptor[1];
} VPD_DEVICE_ID_PAGE, * PVPD_DEVICE_ID_PAGE;

typedef struct _READ_CAPACITY_RSP {
	INT32 LastLBA;
	INT32 BlockSize;

} READ_CAPACITY_RSP, *PREAD_CAPACITY_RSP;
/*
// Mode Select
*/
typedef struct _MODE_SELECT_SPC {
	INT8 OperationCode;	/* 15H */
	INT8 SavePage : 1 ;
	INT8 Reseved0 : 3 ;
	INT8 PageFormat : 1 ;
	INT8 Reserved1 : 3 ;
	INT8 Reserved2[2];
	INT8 ParameterLen;
	INT8 Control;
} MODE_SELECT_SPC, * PMODE_SELECT_SPC;

/*
// Mode Sense
*/
typedef struct _MODE_SENSE_SPC {
    INT8 OperationCode;	/* 1AH */
    INT8 Reseved0 : 3 ;
    INT8 DisableBlockDescriptor : 1 ;
    INT8 Reserved0 : 4 ;
    INT8 PageCode:6 ;
    INT8 PageControl : 2 ;
    INT8 Reserved1;
    INT8 ParameterLen;
    INT8 Control;
} MODE_SENSE_SPC, * PMODE_SENSE_SPC;

typedef struct _MODE_PARAMETER_HEAD {
    INT8 DataLen;
    INT8 MediumType;
    INT8 DeviceParameter;
    INT8 BlockDescriptorLen;
} MODE_PARAMETER_HEAD, * PMODE_PARAMETER_HEAD;

/*
// Define Device Capabilities page.
*/
typedef struct _MODE_RBC_DEVICE_PARAMETERS_PAGE {
    INT8 PageCode : 6;
	INT8 Reserved : 1;
    INT8 PageSavable : 1;
    INT8 PageLength;
    INT8 WriteCacheDisable : 1;
    INT8 Reserved1 : 7;
    INT8 LogicalBlockSize[2];
    INT8 NumberOfLogicalBlocks[5];
    INT8 PowerPerformance;
    INT8 Lockable : 1;
    INT8 Formattable : 1;
    INT8 Writable : 1;
    INT8 Readable : 1;
    INT8 Reserved2 : 4;
    INT8 Reserved3;
}MODE_RBC_DEVICE_PARAMETERS_PAGE, *PMODE_RBC_DEVICE_PARAMETERS_PAGE;

/*
// prevent/allow medium removal
*/
typedef struct _MEDIA_REMOVAL_SPC {
	INT8 OperationCode;    /* 1EH */
	INT8 Reserved0[3];
	INT8 Prevent;
	//INT8 Reserved1:6 ;
	//INT8 Control;
} MEDIA_REMOVAL_SPC, *PMEDIA_REMOVAL_SPC;

/*
// Request Sense
*/
typedef struct _REQUEST_SENSE_SPC {
    INT8 OperationCode;    /* 03H */
    INT8 Reserved[3];
    INT8 AllocationLen;
    INT8 Control;
} REQUEST_SENSE_SPC, *PREQUEST_SENSE_SPC;

typedef struct _REQUEST_SENSE_DATA {
    INT8 ResponseCode : 7;
    INT8 Valid : 1;

    INT8 SegmentNum;

    INT8 SenseKey : 4;
    INT8 Reserved0 : 1;
    INT8 WrongLenIndicator : 1;
    INT8 EndofMedium : 1;
    INT8 FileMark : 1;

    INT8 Info_0;
    INT8 Info_1;
    INT8 Info_2;
    INT8 Info_3;

    INT8 AdditionalSenseLen;

    INT8 CommandSpecInfo_0;
    INT8 CommandSpecInfo_1;
    INT8 CommandSpecInfo_2;
    INT8 CommandSpecInfo_3;

    INT8 ASC;
    INT8 ASCQ;
    INT8 FieldReplacableUnitCode;
    INT8 SenseKeySpec_0 : 7;
    INT8 SenseKeySpecValid : 1;
    INT8 SenseKeySpec_1;
    INT8 SenseKeySpec_2;

} REQUEST_SENSE_DATA, *PREQUEST_SENSE_DATA;

/*
// Test Unit Ready
*/
typedef struct _TEST_UNIT_SPC {
	INT8 OperationCode;    /* 00H */
	INT8 Reserved[4];
	INT8 Control;
} TEST_UNIT_SPC, *PTEST_UNIT_SPC;

/*
// Write Buffer
*/
typedef struct _WRITE_BUFFER_SPC {
    INT8 OperationCode;    /* 3BH */
    INT8 Mode:4 ;
    INT8 Reserved0:4 ;
	INT8 BufferID;
    INT8 BufferOff_2;
    INT8 BufferOff_1;
    INT8 BufferOff_0;
    INT8 ParameterLen_2;
    INT8 ParameterLen_1;
	INT8 ParameterLen_0;
    INT8 Control;
} WRITE_BUFFER_SPC, *PWRITE_BUFFER_SPC;

typedef union _CDB_RBC {
    GENERIC_CDB             Cdb_Generic;
     // RBC commands
    GENERIC_RBC             RbcCdb_Generic;
    FORMAT_RBC              RbcCdb_Format;
    START_STOP_RBC          RbcCdb_OnOffUnit;
    SYNCHRONIZE_CACHE_RBC   RbcCdb_SyncCache;

    // SPC-2 commands
    MODE_SELECT_SPC         SpcCdb_ModeSelect;
    MODE_SENSE_SPC          SpcCdb_ModeSense;
    MEDIA_REMOVAL_SPC       SpcCdb_Remove;
    REQUEST_SENSE_SPC       SpcCdb_RequestSense;
    TEST_UNIT_SPC           SpcCdb_TestUnit;
    WRITE_BUFFER_SPC        SpcCdb_WriteBuffer;

    // ATAPI Commands
    MODE_SELECT_10  CmdModeSel10;
    MODE_SENSE_10   CmdModeSen10;
    //////////////////////////////////////
    READ_LONG_CMD	SpcCdb_ReadLong;

} CDB_RBC, *PCDB_RBC;
//***********************************************

//***********************************************////
#define CBW_SIGNATURE   0x55534243
#define CSW_SIGNATURE   0x55534253

typedef struct _COMMAND_BLOCK_WRAPPER{
    INT32   dCBW_Signature;
    INT32   dCBW_Tag;
    INT32   dCBW_DataXferLen;
    INT8    bCBW_Flag;
    INT8    bCBW_LUN;
    INT8    bCBW_CDBLen;
    CDB_RBC cdbRBC;
} CBW, *PCBW;

typedef struct _COMMAND_STATUS_WRAPPER{
    INT32   dCSW_Signature;
    INT32   dCSW_Tag;
    INT32   dCSW_DataResidue;
    INT8    bCSW_Status;
} CSW, *PCSW;

typedef union _TPBULK_STRUC {
    CBW     TPBulk_CommandBlock;
    CSW     TPBulk_CommandStatus;
}TPBLK_STRUC, * PTPBLK_STRUC;

//unsigned char TPBulk_GetMaxLUN(void);

unsigned char	mCH375Init(void );
unsigned char	mWaitInterrupt(void );
void	mDelay2uS(void );
void	DelayMs(unsigned char nFactor);
unsigned long SwapINT32(unsigned long dData);
unsigned int SwapINT16(unsigned int dData);
unsigned char mInitDisk( void );
unsigned char ReadCapacity(void);
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer);
unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer);
unsigned char SPC_RequestSense(void);
unsigned char SPC_TestUnit(void);
unsigned char SPC_LockMedia(void);

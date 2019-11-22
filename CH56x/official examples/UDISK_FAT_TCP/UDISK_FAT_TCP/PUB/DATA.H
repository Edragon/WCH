/******************** (C) COPYRIGHT ********************************************
* File Name          : DATA.H
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : 串口打印输出控制头文件
*******************************************************************************/

#ifndef	__DATA_H__
#define __DATA_H__

enum
{
    // 接收原始数据所处状态
    P_START         = 0x2a,
    P_END           = 0x0a,
    P_RESET         = 0x5f,
    P_ERR           = 0x23,
    P_SE0           = 0x60,
};

enum
{
    // 当前分析数据所处状态定义
    S_INVALID       = 0x00,
    S_START         = 0x01,
    S_TOKEN         = 0x02,
    S_DATA          = 0x03,
    S_HAND          = 0x04,
    S_ERR           = 0x05,
    S_SOF           = 0xFF
};
enum
{
    // 事务有效标志位
    TOKEN_BIT       = 0x01,
    DATA_BIT        = 0x02,
    HAND_BIT        = 0x04,
    ERR_BIT         = 0x08,

    // 数据校验错误
    TOKEN_ERR       = 0x100,
    DATA_ERR        = 0x200,
    HAND_ERR        = 0x400,

    // 当前显示有效是错误
    IS_ERR          = 1,
    // 是包裹
    IS_PAT          = 2
};
// 数据时相最大长度
#define     DATA_MAX_LEN    (1024+2)
// PID定义
#define     PID_SOF         0xA5
#define     PID_SETUP       0x2D
#define     PID_IN          0x69
#define     PID_OUT         0xE1
#define     PID_DATA0       0xC3
#define     PID_DATA1       0x4B
#define     PID_ACK         0xD2
#define     PID_NAK         0x5A
#define     PID_STALL       0x1E
#define     PID_PRE         0x3C
// 自定义无效PID
#define     PID_OK         	0X00
#define     PID_ERR         0X01
#define     PID_RESET       0X02
#define     PID_EOP         0X03
#define     PID_STATE_ERR   0X04
#define     PID_SCHAR_ERR   0X05
#define     PID_STUF_ERR    0X06
#define     PID_PID_ERR     0X07
#define     PID_CRC5_ERR    0x08
#define     PID_CRC16_ERR   0x09
#define     PID_END         0x0A
#define     PID_INVALID     0xff

// 令牌时相结构
typedef struct _TokenPhase
{
	UINT8   Index;
    UINT8   Pid;
    UINT8   Buf[2];
    BOOL    Crc5_Ok;
} TokenPhase,*PTokenPhase;

// 数据时相结构
typedef struct _DataPhase
{
    UINT16  Index;
	UINT8   Pid;
	UINT8   Buf[DATA_MAX_LEN];
    BOOL    Crc16_Ok;
} DataPhase, *PDataPhase;

// 握手时相结构
typedef struct _HandPhase
{
	UINT8   Pid;
} HandPhase,*PHandPhase;
// 事务结构定义
typedef struct _TransactionPacket
{
	UINT32       Bit;
	TokenPhase  Token;
	DataPhase   Data;
	HandPhase   Hand;
    UINT32       Delta;
} TransactionPacket,*PTransactionPacket;

TransactionPacket   gCurrentPacket;
UINT32               gShowPacketIndex;   // 显示序号包裹索引
//ShowPacketDef       gLastShowPacket;    // 最后显示包裹
TransactionPacket   gLastPacket;        // 最后一次包裹定义
UINT32              gRepeatPacket;      // 重复包裹个数
UINT32               gShowPacketIndex;   // 显示序号包裹索引

/*struct CAnalyse
{
    enum
    {
        // 定义连续SOF包数
        MAX_REPEAT      = 1000,
        // 分析使用内存
        MAX_BYTE        = (4096),
        INVALID_TIME    = 0xffffffff
    };

    enum
    {
        SETUP_INDEX     = 0,
        IN_INDEX        = 1,
        OUT_INDEX       = 2,
        DATA0_INDEX     = 3,
        DATA1_INDEX     = 4,
        ACK_INDEX       = 5,
        NAK_INDEX       = 6,
        STALL_INDEX     = 7,
        SOF_EOP_INDEX   = 8,
        RESET_INDEX     = 9,
        ERR_INDEX       = 10,

        // 栏目颜色定义
        MAX_TOKEN_INDEX = 11
    };

    enum
    {
        // 当前分析数据所处状态定义
        S_INVALID       = 0x00,
        S_START         = 0x01,
        S_TOKEN         = 0x02,
        S_DATA          = 0x03,
        S_HAND          = 0x04,
        S_ERR           = 0x05,
        S_SOF           = 0xFF
    };

    enum
    {
        // 接收原始数据所处状态
        P_START         = 0x2a,
        P_END           = 0x0a,
        P_RESET         = 0x5f,
        P_ERR           = 0x23,
        P_SE0           = 0x60,
    };

    enum
    {
        // 事务有效标志位
        TOKEN_BIT       = 0x01,
        DATA_BIT        = 0x02,
        HAND_BIT        = 0x04,
        ERR_BIT         = 0x08,

        // 数据校验错误
        TOKEN_ERR       = 0x100,
        DATA_ERR        = 0x200,
        HAND_ERR        = 0x400,

        // 当前显示有效是错误
        IS_ERR          = 1,
        // 是包裹
        IS_PAT          = 2
    };

    enum
    {
        // 图标索引定义
        TOKEN_ICO           = 0,
        SETUP_ICO           = 1,
        IDATA_ICO           = 2,
        ODATA_ICO           = 3,
        ACK_ICO             = 4,
        NAK_ICO             = 5,
        SOF_ICO             = 6,
        SE0_ICO             = 7,
        ERROR_ICO           = 8,
    };
}  */

#endif


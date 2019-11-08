
#include "total.h"


/* **************************************************************************************************** */
// 主程序示例

//ULONG	CH375DBG_Index = 0;			/* CH372/CH375设备序号 */
//BOOL	CH375DBG_Opened = FALSE;	/* CH372/CH375设备被打开标志 */

BOOL	CH375DBG_GetFirmwareInfo( PUCHAR ver ) 
{  // 获取调试固件程序的版本,并取消未完成的上传数据块
// 输入参数: ver 指向一个字节变量单元,用于存放获取的版本号
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( CH375DBG_Opened == FALSE ) 
	{  // CH372/CH375设备尚未被打开
		CH375DBG_Opened = CH375OpenDevice( CH375DBG_Index ) != INVALID_HANDLE_VALUE;  // 打开CH375设备
		if ( CH375DBG_Opened == FALSE ) 
			return( FALSE );
	}
	down.mCommand = USB_CMD_GET_FW_INFO;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = 0;
	down.mLength = 0;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // 操作成功,并且返回数据
				*ver = up.mBuffer[0];  // 返回版本号
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

VOID	CH375DBG_CloseExit( VOID ) 
{  // 关闭CH372/CH375设备
	CH375DBG_Opened = FALSE;
	CH375CloseDevice( CH375DBG_Index );
}

BOOL	CH375DBG_GetAppSysInfo( PUCHAR ver, PUCHAR StringBuffer ) 
{  // 获取当前应用系统的版本和说明字符串
// 输入参数: ver 指向一个字节变量单元,用于存放获取的版本号
//           StringBuffer 指向一个足够大的字符串缓冲区,用于存放获取的说明字符串
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	down.mCommand = USB_CMD_GET_APP_INFO;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = 0;
	down.mLength = 0;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // 操作成功,并且返回数据
				*ver = up.mBuffer[0];  // 返回版本号
				for ( len = 0; len < (ULONG)( up.mLength - 1 ); len ++ ) 
					StringBuffer[ len ] = up.mBuffer[ len + 1 ];  // 返回说明字符串
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadSFR( ULONG addr, PUCHAR data ) 
{  // 从SFR一个单元读取数据
// 输入参数: addr 指定SFR的地址
//           data 指向一个字节变量单元,用于存放读出的数据
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_SFR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = addr;
	down.mLength = 1;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // 操作成功,并且返回数据
				*data = up.mBuffer[0];  // 返回数据
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteSFR( ULONG addr, UCHAR data ) 
{  // 向SFR一个单元写入数据
// 输入参数: addr 指定SFR的地址
//           data 指定准备写入的数据
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_SFR | USB_CMD_MEM_DIR_WR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = addr;
	down.mLength = 1;
	down.mBuffer[0] = data;
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // 操作成功
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // 从内部RAM读取数据块
// 输入参数: StartAddr 指定内部RAM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_IRAM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // 操作成功,并且返回数据
				for ( len = 0; len < (ULONG)count; len ++ ) 
					buffer[ len ] = up.mBuffer[ len ];  // 返回数据
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) {  // 向内部RAM写入数据块
// 输入参数: StartAddr 指定内部RAM的起始地址
//           buffer 指向一个数据缓冲区,存放准备写入的数据块
//           count 指定写入的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_IRAM | USB_CMD_MEM_DIR_WR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	for ( len = 0; len < (ULONG)count; len ++ ) 
		down.mBuffer[ len ] = buffer[ len ];
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) // 写出命令块
	{
		
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // 操作成功
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // 从外部RAM读取数据块
// 输入参数: StartAddr 指定外部RAM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_XRAM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // 操作成功,并且返回数据
				for ( len = 0; len < (ULONG)count; len ++ ) 
					buffer[ len ] = up.mBuffer[ len ];  // 返回数据
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // 向外部RAM写入数据块
// 输入参数: StartAddr 指定外部RAM的起始地址
//           buffer 指向一个数据缓冲区,存放准备写入的数据块
//           count 指定写入的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_XRAM | USB_CMD_MEM_DIR_WR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	for ( len = 0; len < (ULONG)count; len ++ ) 
		down.mBuffer[ len ] = buffer[ len ];
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // 操作成功
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadROM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // 从程序ROM读取数据块
// 输入参数: StartAddr 指定程序ROM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_ROM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // 操作成功,并且返回数据
				for ( len = 0; len < (ULONG)count; len ++ ) buffer[ len ] = up.mBuffer[ len ];  // 返回数据
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

UCHAR	CH375DBG_GetPortP1( VOID ) 
{  // 获取MCS51单片机端口P1的状态
// 输出结果: 返回当前端口状态值
	UCHAR	d;
	CH375DBG_ReadSFR( 0x90, &d );  // 从SFR一个单元读取数据
	return( d );
}

VOID	CH375DBG_PutPortP1( UCHAR d ) 
{  // 设置MCS51单片机端口P1的状态
// 输入参数: d 指定新的端口状态值
	CH375DBG_WriteSFR( 0x90, d );  // 向SFR一个单元写入数据
}




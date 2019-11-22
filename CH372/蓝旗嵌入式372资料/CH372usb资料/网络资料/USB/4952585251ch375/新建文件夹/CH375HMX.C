/* 2004.06.05
****************************************
**  Copyright  (C)  W.ch  1999-2004   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Module      @CH375  **
**  TC2.0@PC, KC7.0@MCS51             **
****************************************
*/
/* U盘文件读写模块, 连接方式: 3线制串口+事件中断通知 */
/* MCS-51单片机C语言示例程序 */
/* 因为使用U盘文件读写模块而不是使用U盘文件级子程序库,所以占用较少的单片机资源,可以使用89C51单片机测试 */
/* 模块工作于USB设备模式,用于连接上位机PC机,单片机的RAM只需要几十个字节,不需要外部RAM */
/* 本程序用于演示USB设备模式连接PC机及模式切换 */

#include <reg51.h>
#include <absacc.h>
#include <string.h>
#include <stdio.h>

#define MAX_PATH_LEN			65		/* 最大路径长度,含所有斜杠分隔符和小数点间隔符以及路径结束符00H,CH375模块支持的最大值是65,最小值是13 */
/* 如果模块可能会工作于USB设备模式,那么MAX_PATH_LEN必须大于64,因为上位机PC机发给模块的下传数据包可能达到64字节 */
#include "..\CH375HM.H"

/* 电路连接方式,只需要连接3根线,使用串口同步码启动操作
   单片机    模块
    TXD   =  SIN
    RXD   =  SOUT
             STA# 悬空或接高电平
             INT# 接地或接低电平
    GND   =  GND
*/

CMD_PARAM	idata	mCmdParam;			/* 默认情况下该结构将占用60字节的RAM,可以修改MAX_PATH_LEN常量,当修改为32时,只占用32字节的RAM */

sbit	LED_OUT		=	P1^4;			/* P1.4 低电平驱动LED显示,用于监控演示程序的进度 */

/* 以毫秒为单位延时,适用于24MHz时钟 */
void	mDelaymS( unsigned char delay )
{
	unsigned char	i, j, c;
	for ( i = delay; i != 0; i -- ) {
		for ( j = 200; j != 0; j -- ) c += 3;  /* 在24MHz时钟下延时500uS */
		for ( j = 200; j != 0; j -- ) c += 3;  /* 在24MHz时钟下延时500uS */
	}
}

/* 发送一个字节数据给CH375模块 */
void	mSendByte( unsigned char c )
{
	TI = 0;
	SBUF = c;
	while ( TI == 0 );
}

/* 从CH375模块接收一个字节数据 */
unsigned char	mRecvByte( )
{
	unsigned char	c;
	while ( RI == 0 );
	c = SBUF;
	RI = 0;
	return( c );
}

unsigned char	EventStatus = 0;	/* 保存在命令执行期间收到的事件自动通知的事件状态码 */

/* 执行命令 */
unsigned char	ExecCommand( unsigned char cmd, unsigned char len )
/* 输入命令码和输入参数长度,返回操作状态码,输入参数和返回参数都在CMD_PARAM结构中 */
{
	unsigned char		i, j, status;
	mSendByte( SER_SYNC_CODE1 );  /* 发送串口同步码通知模块,说明命令码开始发送,请求开始执行命令 */
	mSendByte( SER_SYNC_CODE2 );  /* 用两个串口同步码代替STA#的下降沿 */
/* 上面两个串口同步码应该连续发送,如果不连续,那么间隔时间不能超过20mS,否则命令无效 */
	if ( RI ) EventStatus = SBUF;  /* 收到事件自动通知的事件状态码,保存备用 */
	RI = 0;
	mSendByte( cmd );  /* 写入命令码 */
	mSendByte( len );  /* 写入后续参数的长度 */
	if ( len ) {  /* 有参数 */
		for ( i = 0; i != len; i ++ ) mSendByte( mCmdParam.Other.mBuffer[ i ] );  /* 依次写入参数 */
	}
	while ( 1 ) {  /* 处理数据传输,直到操作完成才退出 */
		status = mRecvByte( );  /* 等待模块完成操作并返回操作状态 */
		if ( status == ERR_SUCCESS ) {  /* 操作成功 */
			i = mRecvByte( );  /* 返回结果数据的长度 */
			if ( i ) {  /* 有结果数据 */
				j = 0;
				do {  /* 使用do+while结构是因为其效率高于for */
					mCmdParam.Other.mBuffer[ j ] = mRecvByte( );  /* 接收结果数据并保存到参数结构中 */
					j ++;
				} while ( -- i );
			}
			break;  /* 操作成功返回 */
		}
		else if ( status == USB_INT_DISK_READ || status == USB_INT_DISK_WRITE || status == USB_INT_DISK_RETRY ) {  /* 正在从U盘读数据块,请求数据读出,正在向U盘写数据块,请求数据写入,读写数据块失败重试 */
			break;  /* 本程序只使用以字节为单位的文件读写子程序,所以正常情况下不会收到该状态码,操作失败返回 */
		}
		else {  /* 操作失败 */
			if ( status == ERR_DISK_DISCON || status == ERR_USB_CONNECT ) mDelaymS( 100 );  /* U盘刚刚连接或者断开,应该延时几十毫秒再操作 */
			break;  /* 操作失败返回 */
		}
	}
	return( status );
}

/* 检查操作状态,如果错误则显示错误代码并停机 */
void	mStopIfError( unsigned char iError )
{
	unsigned char	led;
	if ( iError == ERR_SUCCESS ) return;  /* 操作成功 */
/*	printf( "Error: %02X\n", (unsigned short)iError );*/  /* 显示错误 */
	led=0;
	while ( 1 ) {
		LED_OUT = led&1;  /* LED闪烁 */
		mDelaymS( 100 );
		led^=1;
	}
}

main( ) {
	unsigned char	es, i;
	unsigned short	len;
	unsigned char	*name;
	LED_OUT = 0;  /* 开机后LED亮一下以示工作 */
	mDelaymS( 100 );  /* 延时100毫秒,CH375模块上电后需要100毫秒左右的复位时间 */
	mDelaymS( 100 );
	LED_OUT = 1;
/* 设置与CH375模块通讯的串口 */
	SCON = 0x50;
	PCON = 0x80;
	TMOD = 0x20;
	TH1 = 0xE6;  /* 24MHz晶振, 4800bps */
	TR1 = 1;
/* 由于4800bps较慢,所以下面用命令将其修改为9600bps */
	mCmdParam.BaudRate.mDivisor = 18432000/32/9600;  /* 输入参数: 通讯波特率除数,假定模块的晶体X2的频率为18.432MHz */
	i = ExecCommand( CMD_BaudRate, 1 );  /* 设置串口通讯波特率 */
	mStopIfError( i );
	TH1 = 0xF3;  /* 24MHz晶振, 将自身串口的通讯波特率调整到9600bps */
	mDelaymS( 5 );  /* 延时5毫秒,确保CH375模块切换到新设定的通讯波特率 */
/* CMD_BaudRate命令修改模块的通讯波特率,CMD_SetupModule命令设置模块的配置,启动事件自动通知 */
/* 这两个命令都可以在功能配置时以硬件方式直接指定,从而不必每次开机后执行 */
	mCmdParam.Setup.mSetup = 0x01;  /* 输入参数: 模块配置值,位0为1则事件自动通知,USB主机模式下空闲时查询U盘连接状态并自动通知,USB设备模式下下传或者上传成功自动通知 */
	i = ExecCommand( CMD_SetupModule, 1 );  /* 设置模块配置 */
	mStopIfError( i );

	mCmdParam.SetUsbMode.mUsbMode = 2;  /* 进入USB设备模式 */
	i = ExecCommand( CMD_SetUsbMode, 1 );  /* 设置模块的工作模式 */
	mStopIfError( i );

/*	printf( "Start USB Device\n" );*/
	while ( 1 ) {  /* USB设备模式的主循环 */
/*		printf( "Wait download & upload\n" );*/

		while ( 1 ) {  /* 等待模块的事件通知 */
			if ( RI == 1 ) {  /* 查询是否收到模块的事件通知,也可以用串口接收中断处理 */
				es = mRecvByte( );  /* 检测到PC机下传或者上传成功,自动发送状态码通知本单片机 */
				break;  /* 开始处理 */
			}
			else if ( EventStatus != 0 ) {  /* 上次命令执行过程中收到的事件状态码 */
				es = EventStatus; 
				EventStatus = 0;  /* 清保存的事件状态码 */
				break;  /* 开始处理 */
			}
			mDelaymS( 10 );  /* 单片机做其它事情 */
		}
		if ( es == ERR_USB_DAT_DOWN ) {  /* 事件通知是数据下传成功,上位机下传的数据已经在模块中 */
/*			printf( "download ok\n" );*/
			i = ExecCommand( CMD_ReadUsbData, 0 );  /* 从模块的数据下传端点读取数据块 */
			mStopIfError( i );
			if ( EventStatus != 0 ) continue;  /* 上次命令执行过程中收到事件状态码,先分析 */
			for ( i = 0; i < mCmdParam.ReadUsbData.mByteCount; ++ i ) {
				mCmdParam.WriteUsbData.mByteBuffer[ i ] = ~ mCmdParam.WriteUsbData.mByteBuffer[ i ];  /* 演示下传数据取反后作为上传数据返回 */
			}
			mCmdParam.WriteUsbData.mByteCount = mCmdParam.ReadUsbData.mByteCount;
			i = ExecCommand( CMD_WriteUsbData, mCmdParam.WriteUsbData.mByteCount + 1 );  /* 向模块的数据上传端点写入数据块 */
			mStopIfError( i );
			if ( mCmdParam.ReadUsbData.mByteCount == 0 ) break;  /* 假定在收到0长度的下传数据时退出USB设备模式,回到USB主机模式,实际应用不宜采用此方法 */
		}
		else if ( es == ERR_USB_DAT_UP ) {  /* 事件通知是数据上传成功,模块中的上传数据已经被上位机取走 */
/*			printf( "upload ok\n" );*/
/*			可以继续上传后续数据 */
		}
	}

	mCmdParam.SetUsbMode.mUsbMode = 6;  /* 进入USB主机模式 */
	i = ExecCommand( CMD_SetUsbMode, 1 );  /* 设置模块的工作模式 */
	mStopIfError( i );

/*	printf( "Start USB Host\n" );*/
	while ( 1 ) {  /* USB主机模式的主循环 */
/*		printf( "Wait download & upload\n" );*/

		while ( 1 ) {  /* 等待模块的事件通知 */
			if ( RI == 1 ) {  /* 查询是否收到模块的事件通知,也可以用串口接收中断处理 */
				i = mRecvByte( );  /* 检测到U盘连接或者断开后,自动发送状态码通知本单片机 */
				if ( i == ERR_USB_CONNECT ) {  /* 事件通知是U盘已经连接 */
/*					printf( "Disk Connected\n" );*/
					break;
				}
				else if ( i == ERR_DISK_DISCON ) {  /* 事件通知是U盘已经断开 */
/*					printf( "Disk Disconnected\n" );*/
				}
			}
			mDelaymS( 100 );  /* 可以在打算读写U盘时再查询,没有必要一直连续不停地查询,可以让单片机做其它事,没事可做就延时等待一会再查询 */
		}

		mDelaymS( 200 );  /* 延时,可选操作,有的USB存储器需要几十毫秒的延时 */
		LED_OUT = 0;  /* LED亮 */
/* 检查U盘是否准备好,大多数U盘不需要这一步,但是某些U盘必须要执行这一步才能工作 */
		for ( es = 0; es < 5; es ++ ) {
			mDelaymS( 100 );
//			printf( "Ready ?\n" );
			i = ExecCommand( CMD_DiskReady, 0 );  /* 查询磁盘是否准备好 */
			if ( i == ERR_SUCCESS ) break;
		}
/* 首先打开已有文件,如果文件不存在,则新建一个 */
		name = "/MY_ADC.TXT";  /* 文件名,斜杠说明是从根目录开始 */
/*		printf( "Open\n" );*/
		strcpy( mCmdParam.Open.mPathName, name );  /* 原文件名 */
		i = ExecCommand( CMD_FileOpen, MAX_PATH_LEN );  /* 打开文件,输入参数置为最大值,省得再计算参数长度 */
		if ( i == ERR_MISS_FILE ) {  /* ERR_MISS_FILE说明没有找到文件,所以新建一个 */
/*			printf( "Create\n" );*/
			strcpy( mCmdParam.Create.mPathName, name );  /* 新文件名,在根目录下 */
			i = ExecCommand( CMD_FileCreate, MAX_PATH_LEN );  /* 新建文件并打开,如果文件已经存在则先删除后再新建 */
			mStopIfError( i );
		}
		else {  /* 找到文件,说明文件已存在,因为不打算覆盖原数据,所以移动文件指针到末尾,以便追加数据 */
			mStopIfError( i );
			mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF;  /* 移动到文件尾,用于在CMD_FileOpen打开文件后,继续追加数据到已打开文件的末尾 */
			i = ExecCommand( CMD_ByteLocate, 4 );  /* 以字节为单位移动文件指针 */
			mStopIfError( i );
		}
/*		printf( "Write or append data\n" );*/
		strcpy( mCmdParam.ByteWrite.mByteBuffer, "只是演示一下USB主机和USB设备模式切换的功能\xd\xa" );
		len = strlen( mCmdParam.ByteWrite.mByteBuffer );  /* 计算字符串长度 */
		mCmdParam.ByteWrite.mByteCount = len;  /* 将原文件中的20个字节的数据添加到新文件的末尾 */
		i = ExecCommand( CMD_ByteWrite, len+1 );  /* 以字节为单位向文件写入数据 */
		mStopIfError( i );
/*		printf( "Close\n" );*/
		mCmdParam.Close.mUpdateLen = 1;  /* 自动计算文件长度,当以字节为单位向文件写入数据后,如果没有用0长度的CMD_ByteWrite更新文件长度,那么可以在关闭文件时让模块自动更新文件长度 */
		i = ExecCommand( CMD_FileClose, 1 );  /* 关闭文件,当以字节为单位向文件写入(追加)数据后,必须在用完文件后关闭文件 */
		mStopIfError( i );

/* 等待U盘断开 */
		LED_OUT = 1;  /* LED灭 */
		while ( 1 );
	}
}

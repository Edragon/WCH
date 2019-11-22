/* 2004.03.05
****************************************
**                                    **
**                                    **
****************************************
**  USB 1.1 Host Examples for CH375   **
**  KC7.0@MCS-51                      **
****************************************
*/
/* CH375作为USB主机接口的程序示例 */

/* MCS-51单片机C语言的示例程序, U盘数据读写 */

#include "common.h"

unsigned char volatile xdata	CH375_CMD_PORT _at_ 0xBDF1;	/* CH375命令端口的I/O地址 */
unsigned char volatile xdata	CH375_DAT_PORT _at_ 0xBCF0;	/* CH375数据端口的I/O地址 */
extern unsigned char xdata DBUF[BUFFER_LENGTH];

sbit	CH375_INT_WIRE	=		0xB0^2;	/* P3.2, INT0, 连接CH375的INT#引脚,用于查询中断状态 */

#define mDelay1uS( )			/* 对于MCS51单片机,通常不需要1uS延时 */

/* 延时2微秒,请根据单片机速度 */
//void	mDelay1uS( )
//{
//	unsigned char i;
//	for ( i = 1; i != 0; i -- );
//}

/* 延时2微秒,请根据单片机速度 */
void	mDelay2uS( )
{
	unsigned char i;
	for ( i = 2; i != 0; i -- );
}

/* 向CH375命令端口写命令数据 */
void CH375_CMD_PORT_d_out( INT8 d_out )
{
	mDelay1uS( );
	CH375_CMD_PORT=d_out;
	mDelay2uS( );  /* 至少延时2uS */
}

/* 向CH375数据端口写数据 */
void CH375_DAT_PORT_d_out( INT8 d_out )
{
	CH375_DAT_PORT=d_out;
	mDelay1uS( );
}

/* 从CH375命令端口读数据 */
INT8 CH375_DAT_PORT_in( )
{
	mDelay1uS( );
	return( CH375_DAT_PORT );
}

/* 延时毫秒,不精确 */
void DelayMs(unsigned char nFactor)
{
	unsigned char i;
	unsigned int j;
	for(i=0; i<nFactor; i++) for(j=0;j<1000;j++) j=j;
}

/* 等待CH375中断并获取状态 */
unsigned char	mWaitInterrupt( )
{
	unsigned char c;
	while ( CH375_INT_WIRE );  /* 如果CH375的中断引脚输出高电平则等待 */
	CH375_CMD_PORT_d_out( CMD_GET_STATUS);  /* 获取当前中断状态 */
	c = CH375_DAT_PORT_in();  /* 返回中断状态 */
/*	if ( c == USB_INT_DISCONNECT )  /* 检测到USB设备断开事件 */
/*	else if ( c == USB_INT_CONNECT )  /* 检测到USB设备连接事件 */
	return( c );
}

/* 设置CH375为USB主机方式 */
unsigned char	mCH375Init( )
{
	unsigned char i;
	CH375_CMD_PORT_d_out( CMD_GET_IC_VER);  /* 获取芯片及固件版本 */
	i = CH375_DAT_PORT_in( );  /* 版本 */
	if ( i < 0xA2 ) return FALSE;  /* 不支持该芯片 */
	CH375_CMD_PORT_d_out( CMD_SET_USB_MODE);  /* 设置USB工作模式 */
	CH375_DAT_PORT_d_out( 6);  /* 模式代码,自动检测USB设备连接 */
	for ( i = 0xff; i != 0; i -- ) {  /* 等待操作成功,通常需要等待10uS-20uS */
		if ( CH375_DAT_PORT_in() == CMD_RET_SUCCESS ) return TRUE;  /* 操作成功 */
	}
	return FALSE;  /* CH375出错,例如芯片型号错或者处于串口方式或者不支持 */
}

/* 初始化磁盘 */
unsigned char	mInitDisk( )
{
	unsigned char mIntStatus;
	CH375_CMD_PORT_d_out( CMD_DISK_INIT);  /* 初始化USB存储器 */
	mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
	if ( mIntStatus == USB_INT_SUCCESS ) return TRUE;  /* U盘已经成功初始化 */
	return FALSE;  /* 出现错误 */
}

unsigned char ReadCapacity(void)
{
	unsigned char *buf;
	unsigned char  mIntStatus,mLength;
	CH375_CMD_PORT_d_out( CMD_DISK_SIZE);  /* 读取容量 */
	buf=DBUF;
	mIntStatus = mWaitInterrupt( );
	if ( mIntStatus == USB_INT_SUCCESS ) { 
		CH375_CMD_PORT_d_out( CMD_RD_USB_DATA);  /* 从CH375缓冲区读取数据块 */
		mLength = CH375_DAT_PORT_in();  /* 后续数据的长度 */
		while ( mLength ) {  /* 根据长度读取数据 */
			*buf = CH375_DAT_PORT_in();  /* 读出数据并保存 */
			buf ++;
			mLength --;
		}
		return TRUE;
	}
	return FALSE;
}

unsigned long SwapINT32(unsigned long dData)
{
    dData = (dData&0xff)<<24|(dData&0xff00)<<8|(dData&0xff000000)>>24|(dData&0xff0000)>>8;
	return dData;
}

unsigned int SwapINT16(unsigned int dData)
{
    dData = (dData&0xff00)>>8|(dData&0x00ff)<<8;
	return dData;
}

/* 从U盘中读取多个扇区的数据块到缓冲区中 */
unsigned char RBC_Read(unsigned long iLbaStart, unsigned char iSectorCount,unsigned char *mBufferPoint)
/* iLbaStart 是读取的线起始性扇区号, iSectorCount 是读取的扇区数 */
{
	unsigned char mIntStatus;
	unsigned int  mBlockCount;
	unsigned char mLength;
	CH375_CMD_PORT_d_out(CMD_DISK_READ);  /* 从USB存储器读数据块 */
	CH375_DAT_PORT_d_out((unsigned char)iLbaStart);  /* LBA的最低8位 */
	CH375_DAT_PORT_d_out ((unsigned char)( iLbaStart >> 8 ));
	CH375_DAT_PORT_d_out((unsigned char)( iLbaStart >> 16 ));
	CH375_DAT_PORT_d_out((unsigned char)( iLbaStart >> 24 ));  /* LBA的最高8位 */
	CH375_DAT_PORT_d_out( iSectorCount);  /* 扇区数 */
	for ( mBlockCount = iSectorCount * CH375_BLK_PER_SEC; mBlockCount != 0; mBlockCount -- ) {  /* 数据块计数 */
		mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
		if ( mIntStatus == USB_INT_DISK_READ ) {  /* USB存储器读数据块,请求数据读出 */
			CH375_CMD_PORT_d_out(CMD_RD_USB_DATA);  /* 从CH375缓冲区读取数据块 */
			mLength = CH375_DAT_PORT_in();  /* 后续数据的长度 */
/* 通常数据长度是64,有些U盘要求单片机必须在2mS之内取走64字节数据,否则U盘可能数据丢失 */
/* 建议优化下面的循环程序,确保单片机在1mS之内完成64字节的数据传输 */
			if ( mLength ) {  /* 根据长度读取数据 */
				do {  // 对于C51,这个DO+WHILE结构效率高,速度快
					*mBufferPoint = CH375_DAT_PORT_in();  /* 读出数据并保存 */
					mBufferPoint ++;
				} while ( -- mLength );
			}
			CH375_CMD_PORT_d_out( CMD_DISK_RD_GO);  /* 继续执行USB存储器的读操作 */
		}
		else break;  /* 返回错误状态 */
	}
	if ( mBlockCount == 0 ) {
		mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
		if ( mIntStatus == USB_INT_SUCCESS ) return 1;  /* 操作成功 */
	}
	return 0;  /* 操作失败 */
}

/* 将缓冲区中的多个扇区的数据块写入U盘 */
unsigned char	RBC_Write( unsigned long iLbaStart, unsigned char iSectorCount,unsigned char *mBufferPoint )
/* iLbaStart 是写入的线起始性扇区号, iSectorCount 是写入的扇区数 */
{
	unsigned char mIntStatus;
	unsigned int  mBlockCount;
	unsigned char mLength;
	CH375_CMD_PORT_d_out( CMD_DISK_WRITE);  /* 向USB存储器写数据块 */
	CH375_DAT_PORT_d_out((unsigned char)iLbaStart);  /* LBA的最低8位 */
	CH375_DAT_PORT_d_out((unsigned char)( iLbaStart >> 8 ));
	CH375_DAT_PORT_d_out((unsigned char)( iLbaStart >> 16 ));
	CH375_DAT_PORT_d_out((unsigned char)( iLbaStart >> 24 ));  /* LBA的最高8位 */
	CH375_DAT_PORT_d_out(iSectorCount);  /* 扇区数 */
	//mBufferPoint = &DATA_BUFFER;  /* 指向缓冲区起始地址 */
	for ( mBlockCount = iSectorCount * CH375_BLK_PER_SEC; mBlockCount != 0; mBlockCount -- ) {  /* 数据块计数 */
		mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
		if ( mIntStatus == USB_INT_DISK_WRITE ) {  /* USB存储器写数据块,请求数据写入 */
			CH375_CMD_PORT_d_out( CMD_WR_USB_DATA7);  /* 向CH375缓冲区写入数据块 */
			mLength = CH375_BLOCK_SIZE;
			CH375_DAT_PORT_d_out(mLength);  /* 后续数据的长度 */
/* 通常数据长度是64,有些U盘要求单片机必须在2mS之内写入64字节数据,否则U盘可能数据丢失 */
/* 建议优化下面的循环程序,确保单片机在1mS之内完成64字节的数据传输 */
			do {  // 对于C51,这个DO+WHILE结构效率高,速度快
				CH375_DAT_PORT_d_out(*mBufferPoint);  /* 将数据写入 */
				mBufferPoint ++;
			} while ( -- mLength );
			CH375_CMD_PORT_d_out( CMD_DISK_WR_GO);  /* 继续执行USB存储器的写操作 */
		}
		else break;  /* 返回错误状态 */
	}
	if ( mBlockCount == 0 ) {
		mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
		if ( mIntStatus == USB_INT_SUCCESS ) return 1;  /* 操作成功 */
	}
	return 0;  /* 操作失败 */
}

unsigned char SPC_RequestSense(void)
{
	CH375_CMD_PORT_d_out( CMD_DISK_R_SENSE);
	if ( mWaitInterrupt( ) == USB_INT_SUCCESS ) return TRUE;
	return FALSE;
}

unsigned char SPC_TestUnit(void)
{
	CH375_CMD_PORT_d_out( CMD_DISK_READY);
	if ( mWaitInterrupt( ) == USB_INT_SUCCESS ) return TRUE;
	return FALSE;
}

unsigned char SPC_LockMedia(void)
{
	return TRUE;
}

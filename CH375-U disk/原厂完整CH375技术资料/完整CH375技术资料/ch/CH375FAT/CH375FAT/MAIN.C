#include "common.h"
#include <string.h>

unsigned char xdata DBUF[BUFFER_LENGTH];
XXGFLAGS bdata bXXGFlags;

extern unsigned char xdata CHBUF[CHBUF_LENGTH];
extern SYS_INFO_BLOCK xdata DeviceInfo;
extern CH_CMD_BLOCK xdata CHCmdBlock;
extern CH_RSP_BLOCK xdata CHRspBlock;
extern FILE_INFO xdata ThisFile;
extern FREE_FAT_INFO xdata FreeFat;
code unsigned char cmd_test_createfile[]=
	{
	 0x54,0x45,0x53,0x54,0x20,0x20,0x20,0x20,
	 0x54,0x58,0x54,0x20,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x21,0x30,0x00,0x00,0x20,0x20,0x20,0x20,
	 0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,
	 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00
	 };
code unsigned char cmd_test_writefile[]=
	{
	0x09,
	0x00,
	0x00,0x70,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00
	};
/* 将程序空间的字符串复制到内部RAM中,返回字符串长度 */
void	mCopyCodeStringToIRAM( unsigned char *iDestination, unsigned char code *iSource, unsigned char len)
{
	//unsigned char	i = 0;
	while ( len ) {
			*iDestination = *iSource;
		iDestination ++;
		iSource ++;
		len --;
	}
//	return( i );
}
void Init_Port()
{
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
}
void	mInitSTDIO( )
{
	SCON = 0x50;
	PCON = 0x80;
	TMOD = 0x20;
	TH1 = 0xf3;  /* 24MHz晶振, 9600bps */
	TR1 = 1;
	TI = 0;
}


void main(void)
{
	unsigned char  temp;
	unsigned char mIntStatus,retStatus;
	unsigned int len,tempDataLength;
 
	Init_Port();
//	mInitSTDIO( );  /* 为了从串口输出监控信息,调试完成后应该禁止 */

//	printf( "star \n" );
	if ( !mCH375Init( ) ) {
//		printf( "Check hardware connect, and use CH375A, not CH375S \n" );
		while(1);
	}
	//mInitSTDIO( );
//	printf( " wait usb connect  \n" );
	DelayMs(50);
	for(temp=0;temp<64;temp++)DBUF[temp]=0;
	mIntStatus = mWaitInterrupt( );  /* 等待中断并获取状态 */
	if( mIntStatus != USB_INT_CONNECT ); /* U盘连接 */

	retStatus=EnumMassDev();
	retStatus=GetCapacity();

		
		strcpy( CHCmdBlock.CmdBlock.Cmd_OpenFile.filename, "CH375HFTC  " );  /* 文件名,该文件在C51子目录下 */

		retStatus=OpenFile(CHCmdBlock.CmdBlock.Cmd_OpenFile.filename);
		len=ThisFile.LengthInByte;
		tempDataLength=ThisFile.LengthInByte;
		retStatus=ReadFile(len,CHBUF);
//
		//strcpy( CHCmdBlock.CmdBlock.Cmd_CreateFile.item, cmd_test_createfile );

		mCopyCodeStringToIRAM( CHCmdBlock.CmdBlock.Cmd_CreateFile.item, cmd_test_createfile ,32);

		 retStatus=CreateFile(CHCmdBlock.CmdBlock.Cmd_CreateFile.item);

//
    	len=tempDataLength;
   		retStatus=WriteFile(len,CHBUF);

	//	strcpy( CHCmdBlock.CmdBlock.Cmd_RemoveFile.filename, "CH375HFTC  " );
    //	retStatus=RemoveFile(CHCmdBlock.CmdBlock.Cmd_RemoveFile.filename);

	while(1);

}

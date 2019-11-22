/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : CH563 USB Host Examples
*                      (1)、CH563 USB Examples by KEIL3；    
*                      (2)、串口0输出监控信息,115200bps；
*                      (3)、USB HOST以查询方式处理；
*                      (4)、本程序功能: 
*                           以FAT文件系统方式操作U盘。
*                           本程序用于演示列出指定目录下的所有文件，以及用于搜索/枚举文件名
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include <stdio.h>
#include <string.h>
#include "absacc.h"
#include "SYSFREQ.H"                                                            /* 系统定义相关头文件 */
#include "CH563SFR.H"
#include "EHCI_HCD.H"                                                           /* 主机控制器低层操作相关头文件 */
#include "CH563USBSFR.H"                                                        /* 芯片USB寄存器相关头文件 */
#include "Udisk.H"                                                              /* Udisk定义相关头文件 */
#include "PRINTF.H"                                                             /* 串口打印输出控制头文件 */
#include "SD_INC.H" 

/******************************************************************************/
/* 常、变量定义 */
UINT8 EHCI_Allocate_Buf[ EHCI_ALLOCATE_BUF_LEN ];                               /* 内存分配缓冲区(由于QH、QTD等结构提需要32字节对齐, 仅用来分配QH、QTD相关内存使用) */
__align( 4096 ) UINT8 EHCI_Data_Buf[ EHCI_DATA_BUF_LEN ];                       /* 内存数据缓冲区(需要4K对齐) */                                
__align( 4096 ) UINT8 EHCI_PERIODIC_LIST_Buf[ EHCI_PER_LISE_BUF_LEN ];          /* 周期性帧列表缓冲区(需要4K对齐) */                                

/* 定义U盘相关结构体 */
BULK_ONLY_CMD    mBOC;                                                          /* BulkOnly传输结构定义 */
DISK_INFO        UDisk;                                                         /* U盘相关结构体定义 */                                                        

/******************************************************************************/                
/* 以下定义的详细说明请看CH56XUFI.H文件 */
#define DISK_BASE_BUF_LEN        512                                      /* 默认的磁盘数据缓冲区大小为512字节,建议选择为2048甚至4096以支持某些大扇区的U盘,为0则禁止在.H文件中定义缓冲区并由应用程序在pDISK_BASE_BUF中指定 */
/* 如果需要复用磁盘数据缓冲区以节约RAM,那么可将DISK_BASE_BUF_LEN定义为0以禁止在.H文件中定义缓冲区,而由应用程序在调用CH56xLibInit之前将与其它程序合用的缓冲区起始地址置入pDISK_BASE_BUF变量 */

#define NO_DEFAULT_ACCESS_SECTOR    1                                         /* 禁止默认的磁盘扇区读写子程序,下面用自行编写的程序代替它 */
#define NO_DEFAULT_FILE_ENUMER        1                                         /* 禁止默认的文件名枚举回调程序,下面用自行编写的程序代替它 */
#define UNSUPPORT_USB_HUB            1                                          /* 默认不支持HUB */

#include "CH56XUFI.H"
#include "CH56XUFI.C"


/* 连接一个LED用于监控演示程序的进度,低电平LED亮 */
#define LED_OUT_INIT( )        { R32_PA_OUT |= ELINK; R32_PA_DIR |= ELINK; }    /* ELINK 高电平为输出方向 */
#define LED_OUT_ACT( )        { R32_PA_CLR |= ELINK; }                          /* ELINK 低电平驱动LED显示 */
#define LED_OUT_INACT( )    { R32_PA_OUT |= ELINK; }                            /* ELINK 高电平关闭LED显示 */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ中断处理函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void IRQ_Handler( void )
{
    while( 1 );
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : FIQ中断处理函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void FIQ_Handler( void )
{
    while( 1 );
}

UINT8 CH56xReadSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* 从磁盘读取多个扇区的数据到缓冲区中 */
{
	return(SD_ReadMultiBlock( StartLba, DataBuf, SectCount));
}

UINT8 CH56xWriteSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* 将缓冲区中的多个扇区的数据块写入磁盘 */
{
	return(SD_WriteMultiBlock(StartLba, DataBuf, SectCount ));
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : 为printf和getkey输入输出初始化串口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / 115200;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* 四舍五入 */
    R8_UART0_LCR = 0x80;                                                        /* DLAB位置1 */
    R8_UART0_DIV = 1;                                                           /* 预分频 */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* 设置字节长度为8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* 设置FIFO触发点为14，清发送和接收FIFO，FIFO使能 */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 output enable */
}

/*******************************************************************************
* Function Name  : fputc
* Description    : 通过串口输出监控信息
* Input          : c-- writes the character specified by c 
*                  *f--the output stream pointed to by *f
* Output         : None
* Return         : None
*******************************************************************************/

int fputc( int c, FILE *f )
{
    R8_UART0_THR = c;                                                           /* 发送数据 */
    while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* 等待数据发送 */
    return( c );
}

/*******************************************************************************
* Function Name  : mStopIfError
* Description    : 检查操作状态
*                  如果错误则显示错误代码并停机,应该替换为实际的处理措施
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void mStopIfError( UINT8 iError )
{
    if( iError == ERR_SUCCESS ) 
    {    
        return;                                                                 /* 操作成功 */
    }
    printf( "Error: %02X\n", (UINT16)iError );                                  /* 显示错误 */
    
    /* 遇到错误后,应该分析错误码以及CH56xDiskStatus状态,例如调用CH56xDiskConnect查询当前U盘是否连接,如果U盘已断开那么就重新等待U盘插上再操作,
       建议出错后的处理步骤:
       1、调用一次CH56xDiskReady,成功则继续操作,例如Open,Read/Write等,在CH56xDiskReady中会自动调用CH56xDiskConnect，不必另外调用
       2、如果CH56xDiskReady不成功,那么强行将CH56xDiskStatus置为DISK_CONNECT状态,然后从头开始操作(等待U盘连接CH56xDiskConnect，CH56xDiskReady等) */
    while( 1 ) 
    {
        LED_OUT_ACT( );                                                         /* LED闪烁 */
        Delay_ms( 200 );
        LED_OUT_INACT( );
        Delay_ms( 200 );
    }
}

/*******************************************************************************
以下程序为EXAM13枚举U盘下文件所用
*******************************************************************************/
// typedef struct _FILE_NAME 
// {
//     UINT32    DirStartClust;                                                    /* 文件所在目录的起始簇号 */
// //    UINT32    Size;                                                           /* 文件长度 */
//     UINT8    Name[8+3+1+1];                                                     /* 文件名,共8+3字节,分隔符,结束符,因为未包含目录名所以是相对路径 */
//     UINT8    Attr;                                                              /* 文件属性 */
//     UINT8    Reserved[2];                                                       /* for 4 bytes align */
// }FILE_NAME;
typedef struct _FILE_NAME 
{
    UINT32    DirStartClust;                                                    /* 文件所在目录的起始簇号 */
    UINT32    Size;                                                           /* 文件长度 */
    UINT8      Name[8+3+1+1];                                                     /* 文件名,共8+3字节,分隔符,结束符,因为未包含目录名所以是相对路径 */
    UINT8     Attr;                                                              /* 文件属性 */
	 UINT16    Date;
	 UINT16    Time;
    UINT8     Reserved[2];                                                       /* for 4 bytes align */
}FILE_NAME;


#define    MAX_FILE_COUNT               200
FILE_NAME    FileNameBuffer[ MAX_FILE_COUNT ];                                  /* 文件名结构 */
UINT16    FileCount;                                                            /* ? */
UINT32    CurrentDirStartClust;                                                 /* 保存当前目录的起始簇号,用于加快文件枚举和打开速度 */

/*******************************************************************************
* Function Name  : ListFile
* Description    : 列举指定目录下的所有文件
*                  输入参数mCmdParam.Open.mPathName[]为目录名字符串,形式与文件名
*                  相同,单个斜线则代表根目录
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 ListFile( void )
{
    UINT8  i;

    printf( "List Directory: %s\n", mCmdParam.Open.mPathName );                 /* 显示目录名 */
    i = strlen( (PCCHAR)mCmdParam.Open.mPathName );                             /* 计算路径的长度,找目录名的结束符 */
    if( i && mCmdParam.Open.mPathName[ i - 1 ] == '/' )                         /* 是根目录,或者是已经有路径分隔符 */
    { 
    }  
    else 
    {
        mCmdParam.Open.mPathName[ i++ ] = '/';                                  /* 在当前目录下进行枚举,除根目录外都是相对路径,不是根目录则加路径分隔符 */
    }
    mCmdParam.Open.mPathName[ i++ ] = '*';                                      /* 枚举通配符,完整的路径例如"\*"或者"\C51\*"或者"\C51\CH563*"等 */
    mCmdParam.Open.mPathName[ i ] = 0;                                          /* 结束符 */
    printf("ListFile : mPathName = %s \r\n",mCmdParam.Open.mPathName);
	CH56xvFileSize = 0xFFFFFFFF;                                                /* 快速连续枚举,每找到一个文件调用一次xFileNameEnumer回调子程序,如果值小于0x80000000则每次只枚举一个文件太慢 */
    i = CH56xFileOpen( );                                                       /* 枚举,由回调程序xFileNameEnumer产生记录保存到结构中 */
    if( i == ERR_SUCCESS || i == ERR_FOUND_NAME || i == ERR_MISS_FILE ) 
    {  
        /* 操作成功,通常不会返回ERR_SUCCESS,仅在xFileNameEnumer提前退出时才会返回ERR_FOUND_NAME */
        printf( "Success, new FileCount = %d\n", FileCount );
        return( ERR_SUCCESS );
    }
    else 
    {
        printf( "Failed, new FileCount = %d\n", FileCount );
        return( i );
    }
}

/*******************************************************************************
* Function Name  : ListAll
* Description    : 以广度优先的算法枚举整个U盘中的所有文件及目录
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 ListAll( void )
{
    UINT8  i;
    UINT16 OldFileCount;
	UINT16 temp1;
	
	temp1 = 1;
    OldFileCount = FileCount = 0;                                               /* 清文件结构计数 */
    FileNameBuffer[ 0 ].Name[ 0 ] = '/';                                        /* 根目录,是完整路径名,除根目录是绝对路径之外都是相对路径 */
    FileNameBuffer[ 0 ].Name[ 1 ] = 0;
    FileNameBuffer[ 0 ].DirStartClust = 0;                                      /* 根目录的这个参数无意义 */
    FileNameBuffer[ 0 ].Attr = ATTR_DIRECTORY;                                  /* 根目录也是目录,作为第一个记录保存 */

    for( FileCount = 1; OldFileCount < FileCount; OldFileCount ++ ) 
	{  
		printf("temp1 = %d FileCount = %d OldFileCount = %d \r\n",temp1++,FileCount,OldFileCount);
        /* 尚有新枚举到的文件名结构未进行分析 */
        if( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) 
        {  
            /* 是目录则继续进行深度搜索 */
            strcpy( (PCHAR)mCmdParam.Open.mPathName, (PCCHAR)FileNameBuffer[ OldFileCount ].Name );  /* 目录名,除根目录外都是相对路径 */
			printf("mPathName = %s \r\n",mCmdParam.Open.mPathName);
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* 当前目录的上级目录的起始簇号,便于用相对路径打开,比完整路径名速度快 */
            i = CH56xFileOpen( );                                               /* 打开目录,仅为了获取目录的起始簇号以提高速度 */
            if( i == ERR_SUCCESS ) 
            {
                return( ERR_MISS_DIR );                                         /* 应该是打开了目录,但是返回结果是打开了文件 */
            }
            if( i != ERR_OPEN_DIR ) 
            {
                return( i );
            }
            if( OldFileCount ) 
            {
                CurrentDirStartClust = CH56xvStartCluster;                      /* 不是根目录,获取目录的起始簇号 */
            }
            else
            {  
                /* 是根目录,获取根目录的起始簇号 */
                if( CH56xvDiskFat == DISK_FAT32 ) 
                {
                    CurrentDirStartClust = CH56xvDiskRoot;                      /* FAT32根目录 */
                }
                else 
                {
                    CurrentDirStartClust = 0;                                   /* FAT12/FAT16根目录 */
                }
            }
            CH56xFileClose();                                                  /* 对于根目录一定要关闭 */
//            strcpy( mCmdParam.Open.mPathName, FileNameBuffer[ OldFileCount ].Name );  /* 目录名,由于mPathName未被修改所以无需再复制 */
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* 当前目录的上级目录的起始簇号,便于用相对路径打开,比完整路径名速度快 */
            i = ListFile( );                                                    /* 枚举目录,由回调程序xFileNameEnumer产生记录保存到结构中 */
            if( i != ERR_SUCCESS ) 
            {
                return( i );
            }
        }
    }

    /* U盘中的文件及目录全部枚举完毕,下面开始根据结构记录依次打开文件 */
    printf( "Total file&dir = %d, Open every file:\n", FileCount );
    for( OldFileCount = 0; OldFileCount < FileCount; OldFileCount ++ ) 
    {
        if( ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) == 0 ) 
        {  
            /* 是文件则打开,目录则跳过 */
            printf( "Open file: %s DirStartClust = %d \n", FileNameBuffer[ OldFileCount ].Name ,FileNameBuffer[ OldFileCount ].DirStartClust);
            strcpy( (PCHAR)mCmdParam.Open.mPathName, (PCCHAR)FileNameBuffer[ OldFileCount ].Name );  /* 相对路径 */
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* 当前文件的上级目录的起始簇号,便于用相对路径打开,比完整路径名速度快 */
            i = CH56xFileOpen();                                               /* 打开文件 */
            if( i == ERR_SUCCESS ) 
            {  
                /* 成功打开了文件 */
                mCmdParam.Read.mDataBuffer = (PUINT8)( & EHCI_Data_Buf[ 0 ] );  /* 指向文件数据缓冲区的起始地址 */
                mCmdParam.Read.mSectorCount = 1;                                /* 读取扇区数 */
                CH56xFileRead();
				
				
//                CH56xFileClose( );                                            /* 不做写操作可以无需关闭 */
            }
        }
    }
    return( ERR_SUCCESS );
}

/*******************************************************************************
* Function Name  : xFileNameEnumer
* Description    : 文件名枚举回调子程序,参考CH56XUFI.H文件中的例子
*                  每搜索到一个文件FileOpen都会调用本回调程序,xFileNameEnumer返
                   回后,FileOpen递减CH56xvFileSize并继续枚举直到搜索不到文件或
                   者目录.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void xFileNameEnumer( void )        
{  
    UINT8  i, c;
    P_FAT_DIR_INFO    pFileDir;
    PUINT8            pNameBuf;

	printf("xFileNameEnumer\r\n");
    pFileDir = (P_FAT_DIR_INFO)( pDISK_BASE_BUF + CH56xvFdtOffset );            /* 当前FDT的起始地址 */
    if( pFileDir -> DIR_Name[ 0 ] == '.' ) 
    {
        return;                                                                 /* 是本级或者上级目录名,必须丢弃不处理 */
    }    
    if( ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) == 0 ) 
    {  
        /* 判断是文件名 */
        if( pFileDir -> DIR_Name[ 8 ] == 'H' && pFileDir -> DIR_Name[ 9 ] == ' ' /* 过滤文件的扩展名,是".H"文件,则丢弃,不登记不处理 */
         || pFileDir -> DIR_Name[ 8 ] == 'E' && pFileDir -> DIR_Name[ 9 ] == 'X' && pFileDir -> DIR_Name[10] == 'E' ) 
        {
            return;                                                             /* 扩展名是".EXE"的文件,则丢弃 */
        }
    }
    pNameBuf = & FileNameBuffer[ FileCount ].Name[ 0 ];                         /* 文件名结构中的文件名缓冲区 */
    for( i = 0; i < 11; i ++ ) 
    {  
        /* 复制文件名,长度为11个字符 */
        c = pFileDir -> DIR_Name[ i ];
        if ( i == 0 && c == 0x05 ) 
        {
            c = 0xE5;                                                           /* 特殊字符 */
        }
        if( c != 0x20 ) 
        {  
            /* 有效字符 */
            if( i == 8 ) 
            {  
                /* 处理扩展名 */
                *pNameBuf = '.';                                                /* 分隔符 */
                pNameBuf ++;
            }
            *pNameBuf = c;                                                      /* 复制文件名的一个字符 */
            pNameBuf ++;
        }
    }
    *pNameBuf = 0;                                                              /* 当前文件名完整路径的结束符 */
    FileNameBuffer[ FileCount ].DirStartClust = CurrentDirStartClust;           /* 记录当前目录的起始簇号,用于加快文件打开速度 */
    FileNameBuffer[ FileCount ].Attr = pFileDir -> DIR_Attr;                    /* 文件属性 */
    if( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) 
    {  
        /* 判断是目录名 */
        printf( "Dir %4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );
    }
    else 
    {  
        /* 判断是文件名 */
        printf( "File%4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );
    }
	printf("当前FileCount = %d \r\n",FileCount);
    FileCount ++;                                                               /* 文件计数 */
    if( FileCount >= MAX_FILE_COUNT ) 
    {  
        /* 文件名结构缓冲区太小,结构数量不足 */
        CH56xvFileSize = 1;                                                     /* 强行提前结束枚举,本次FileOpen将不再回调xFileNameEnumer而是提前返回,防止缓冲区溢出 */
        printf( "FileName Structure Full\n" );
    }
}


/************************ USB 相关函数 ******************************/
UINT8 Flie_Save( UINT8 *pname, UINT32 Num, UINT8 *Data )
{
	UINT8 i;
	strcpy( (PCHAR)&mCmdParam.Open.mPathName, (PCCHAR)pname ); 
	i = CH56xFileOpen( );											            /* 打开文件 */
#if MY_DEBUG_PRINTF
	printf( "CH563FileOpen:%x \n", i );
#endif
	if( i == ERR_SUCCESS ){	
	    mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF;
		CH56xByteLocate( );												        /* 文件偏移到文件尾 */
	}
	else if( i == ERR_MISS_FILE ){
		strcpy( (PCHAR)&mCmdParam.Create.mPathName, (PCCHAR)pname ); 
		CH56xFileCreate( );   
	}																	        /* 创建文件 */
	else return( i );
	mCmdParam.ByteWrite.mByteBuffer = Data;
	mCmdParam.ByteWrite.mByteCount = Num;
	i = CH56xByteWrite( );											            /* 写文件 */
#if MY_DEBUG_PRINTF
	printf( "CH563ByteWrite:%x \n", i );
#endif
	if( i != ERR_SUCCESS )	 return ( i );
	mCmdParam.Close.mUpdateLen = 1;
	i = CH56xFileClose( );											            /* 关闭文件，更新文件长度 */
#if MY_DEBUG_PRINTF
	printf( "CH563FileClose:%x \n" , i );
#endif
	return( i );
}

/*******************************************************************************
* Function Name  : main
* Description    : 主程序
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int    main( void ) 
{
    UINT8 s,i,c;
    UINT8 buf[100];

    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* 开机后LED亮一下以示工作 */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    mInitSTDIO( );                                                              /* 为了让计算机通过串口监控演示过程 */

// 	i = CH56xLibInit( );                                                        /* 初始化CH563程序库和CH563芯片,操作成功返回0 */
//     mStopIfError( i );

// 	SD_Port_Init();
// 	i = DISK_Init();
// 	if(i!=0x00)    //err
// 	{
// 		printf("sd card err \n");
// 		while(1);
// 	}
	
    SD_Card_Init();
    /* 其它电路初始化 */
	Delay_ms( 20 );
	printf("list file\n");
//	ListAll();
  strcpy( mCmdParam.Open.mPathName, "/1.C" );
	i=CH56xFileOpen();
										if ( i == ERR_MISS_DIR || i== ERR_MISS_FILE )//??????
										{
												printf( "NO\n" );
										}
										else                                        //????????
										{	
										i = sprintf( buf,"Note: \xd\xa这个程序是以字节为单位进行U盘文件读写,559简单演示功能。\xd\xa");/*演示 */
										for(c=0; c<2; c++)
										{
												mCmdParam.ByteWrite.mByteCount = i;     /* 指定本次写入的字节数 */
												mCmdParam.ByteWrite.mByteBuffer = buf;  /* 指向缓冲区 */
												s = CH56xByteWrite( );                  /* 以字节为单位向文件写入数据 */
												mStopIfError( s);
												printf("成功写入 %02X次\n",(UINT16)c);
											Delay_ms(100);
										}	
										printf( "Close\n" );
										mCmdParam.Close.mUpdateLen = 1;             //自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度
										i = CH56xFileClose( );		
										mCmdParam.Close.mUpdateLen = 1;             //自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度
										i = CH56xFileClose( );										
										}
  strcpy( mCmdParam.Open.mPathName, "/1.C" );
	i=CH56xFileOpen();
										if ( i == ERR_MISS_DIR || i== ERR_MISS_FILE )//??????
										{
												printf( "NO\n" );
										}
										else                                        //????????
										{	
												mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF; //跳过文件的前608个字节开始读写
												CH56xByteLocate( );											
										i = sprintf( buf,"Note: \xd\xa这个程序是以字节为单位进行U盘文件读写,559简单演示功能。\xd\xa");/*演示 */
										for(c=0; c<2; c++)
										{
												mCmdParam.ByteWrite.mByteCount = i;     /* 指定本次写入的字节数 */
												mCmdParam.ByteWrite.mByteBuffer = buf;  /* 指向缓冲区 */
												s = CH56xByteWrite( );                  /* 以字节为单位向文件写入数据 */
												mStopIfError( s );
												printf("成功写入 %02X次\n",(UINT16)c);
											Delay_ms(100);
										}	
										printf( "Close\n" );
										mCmdParam.Close.mUpdateLen = 1;             //自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度
										i = CH56xFileClose( );		
										mCmdParam.Close.mUpdateLen = 1;             //自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度
										i = CH56xFileClose( );										
										}										
    while ( 1 ) 
    {
		
	  }
}



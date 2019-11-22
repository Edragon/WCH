/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : CH563 USB Host Examples
*                      (1)��CH563 USB Examples by KEIL3��    
*                      (2)������0��������Ϣ,115200bps��
*                      (3)��USB HOST�Բ�ѯ��ʽ������
*                      (4)����������: 
*                           ��FAT�ļ�ϵͳ��ʽ����U�̡�
*                           ������������ʾ�г�ָ��Ŀ¼�µ������ļ����Լ���������/ö���ļ���
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "absacc.h"
#include "SYSFREQ.H"                                                            /* ϵͳ�������ͷ�ļ� */
#include "CH563SFR.H"
#include "EHCI_HCD.H"                                                           /* �����������Ͳ�������ͷ�ļ� */
#include "CH563USBSFR.H"                                                        /* оƬUSB�Ĵ������ͷ�ļ� */
#include "Udisk.H"                                                              /* Udisk�������ͷ�ļ� */
#include "PRINTF.H"                                                             /* ���ڴ�ӡ�������ͷ�ļ� */
#include "SD_INC.H" 

/******************************************************************************/
/* ������������ */
UINT8 EHCI_Allocate_Buf[ EHCI_ALLOCATE_BUF_LEN ];                               /* �ڴ���仺����(����QH��QTD�Ƚṹ����Ҫ32�ֽڶ���, ����������QH��QTD����ڴ�ʹ��) */
__align( 4096 ) UINT8 EHCI_Data_Buf[ EHCI_DATA_BUF_LEN ];                       /* �ڴ����ݻ�����(��Ҫ4K����) */                                
__align( 4096 ) UINT8 EHCI_PERIODIC_LIST_Buf[ EHCI_PER_LISE_BUF_LEN ];          /* ������֡�б�������(��Ҫ4K����) */                                

/* ����U����ؽṹ�� */
BULK_ONLY_CMD    mBOC;                                                          /* BulkOnly����ṹ���� */
DISK_INFO        UDisk;                                                         /* U����ؽṹ�嶨�� */                                                        

/******************************************************************************/                
/* ���¶������ϸ˵���뿴CH56XUFI.H�ļ� */
#define DISK_BASE_BUF_LEN        512                                      /* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��,Ϊ0���ֹ��.H�ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
/* �����Ҫ���ô������ݻ������Խ�ԼRAM,��ô�ɽ�DISK_BASE_BUF_LEN����Ϊ0�Խ�ֹ��.H�ļ��ж��建����,����Ӧ�ó����ڵ���CH56xLibInit֮ǰ��������������õĻ�������ʼ��ַ����pDISK_BASE_BUF���� */

#define NO_DEFAULT_ACCESS_SECTOR    1                                         /* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
#define NO_DEFAULT_FILE_ENUMER        1                                         /* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */
#define UNSUPPORT_USB_HUB            1                                          /* Ĭ�ϲ�֧��HUB */

#include "CH56XUFI.H"
#include "CH56XUFI.C"


/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED_OUT_INIT( )        { R32_PA_OUT |= ELINK; R32_PA_DIR |= ELINK; }    /* ELINK �ߵ�ƽΪ������� */
#define LED_OUT_ACT( )        { R32_PA_CLR |= ELINK; }                          /* ELINK �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT( )    { R32_PA_OUT |= ELINK; }                            /* ELINK �ߵ�ƽ�ر�LED��ʾ */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϴ�������
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
* Description    : FIQ�жϴ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void FIQ_Handler( void )
{
    while( 1 );
}

UINT8 CH56xReadSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* �Ӵ��̶�ȡ������������ݵ��������� */
{
	return(SD_ReadMultiBlock( StartLba, DataBuf, SectCount));
}

UINT8 CH56xWriteSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* ���������еĶ�����������ݿ�д����� */
{
	return(SD_WriteMultiBlock(StartLba, DataBuf, SectCount ));
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : Ϊprintf��getkey���������ʼ������
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
    if ( x2 >= 5 ) x ++;                                                        /* �������� */
    R8_UART0_LCR = 0x80;                                                        /* DLABλ��1 */
    R8_UART0_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 output enable */
}

/*******************************************************************************
* Function Name  : fputc
* Description    : ͨ��������������Ϣ
* Input          : c-- writes the character specified by c 
*                  *f--the output stream pointed to by *f
* Output         : None
* Return         : None
*******************************************************************************/

int fputc( int c, FILE *f )
{
    R8_UART0_THR = c;                                                           /* �������� */
    while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* �ȴ����ݷ��� */
    return( c );
}

/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ������״̬
*                  �����������ʾ������벢ͣ��,Ӧ���滻Ϊʵ�ʵĴ�����ʩ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void mStopIfError( UINT8 iError )
{
    if( iError == ERR_SUCCESS ) 
    {    
        return;                                                                 /* �����ɹ� */
    }
    printf( "Error: %02X\n", (UINT16)iError );                                  /* ��ʾ���� */
    
    /* ���������,Ӧ�÷����������Լ�CH56xDiskStatus״̬,�������CH56xDiskConnect��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
       ���������Ĵ�������:
       1������һ��CH56xDiskReady,�ɹ����������,����Open,Read/Write��,��CH56xDiskReady�л��Զ�����CH56xDiskConnect�������������
       2�����CH56xDiskReady���ɹ�,��ôǿ�н�CH56xDiskStatus��ΪDISK_CONNECT״̬,Ȼ���ͷ��ʼ����(�ȴ�U������CH56xDiskConnect��CH56xDiskReady��) */
    while( 1 ) 
    {
        LED_OUT_ACT( );                                                         /* LED��˸ */
        Delay_ms( 200 );
        LED_OUT_INACT( );
        Delay_ms( 200 );
    }
}

/*******************************************************************************
���³���ΪEXAM13ö��U�����ļ�����
*******************************************************************************/
// typedef struct _FILE_NAME 
// {
//     UINT32    DirStartClust;                                                    /* �ļ�����Ŀ¼����ʼ�غ� */
// //    UINT32    Size;                                                           /* �ļ����� */
//     UINT8    Name[8+3+1+1];                                                     /* �ļ���,��8+3�ֽ�,�ָ���,������,��Ϊδ����Ŀ¼�����������·�� */
//     UINT8    Attr;                                                              /* �ļ����� */
//     UINT8    Reserved[2];                                                       /* for 4 bytes align */
// }FILE_NAME;
typedef struct _FILE_NAME 
{
    UINT32    DirStartClust;                                                    /* �ļ�����Ŀ¼����ʼ�غ� */
    UINT32    Size;                                                           /* �ļ����� */
    UINT8      Name[8+3+1+1];                                                     /* �ļ���,��8+3�ֽ�,�ָ���,������,��Ϊδ����Ŀ¼�����������·�� */
    UINT8     Attr;                                                              /* �ļ����� */
	 UINT16    Date;
	 UINT16    Time;
    UINT8     Reserved[2];                                                       /* for 4 bytes align */
}FILE_NAME;


#define    MAX_FILE_COUNT               200
FILE_NAME    FileNameBuffer[ MAX_FILE_COUNT ];                                  /* �ļ����ṹ */
UINT16    FileCount;                                                            /* ? */
UINT32    CurrentDirStartClust;                                                 /* ���浱ǰĿ¼����ʼ�غ�,���ڼӿ��ļ�ö�ٺʹ��ٶ� */

/*******************************************************************************
* Function Name  : ListFile
* Description    : �о�ָ��Ŀ¼�µ������ļ�
*                  �������mCmdParam.Open.mPathName[]ΪĿ¼���ַ���,��ʽ���ļ���
*                  ��ͬ,����б���������Ŀ¼
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 ListFile( void )
{
    UINT8  i;

    printf( "List Directory: %s\n", mCmdParam.Open.mPathName );                 /* ��ʾĿ¼�� */
    i = strlen( (PCCHAR)mCmdParam.Open.mPathName );                             /* ����·���ĳ���,��Ŀ¼���Ľ����� */
    if( i && mCmdParam.Open.mPathName[ i - 1 ] == '/' )                         /* �Ǹ�Ŀ¼,�������Ѿ���·���ָ��� */
    { 
    }  
    else 
    {
        mCmdParam.Open.mPathName[ i++ ] = '/';                                  /* �ڵ�ǰĿ¼�½���ö��,����Ŀ¼�ⶼ�����·��,���Ǹ�Ŀ¼���·���ָ��� */
    }
    mCmdParam.Open.mPathName[ i++ ] = '*';                                      /* ö��ͨ���,������·������"\*"����"\C51\*"����"\C51\CH563*"�� */
    mCmdParam.Open.mPathName[ i ] = 0;                                          /* ������ */
    printf("ListFile : mPathName = %s \r\n",mCmdParam.Open.mPathName);
	CH56xvFileSize = 0xFFFFFFFF;                                                /* ��������ö��,ÿ�ҵ�һ���ļ�����һ��xFileNameEnumer�ص��ӳ���,���ֵС��0x80000000��ÿ��ֻö��һ���ļ�̫�� */
    i = CH56xFileOpen( );                                                       /* ö��,�ɻص�����xFileNameEnumer������¼���浽�ṹ�� */
    if( i == ERR_SUCCESS || i == ERR_FOUND_NAME || i == ERR_MISS_FILE ) 
    {  
        /* �����ɹ�,ͨ�����᷵��ERR_SUCCESS,����xFileNameEnumer��ǰ�˳�ʱ�Ż᷵��ERR_FOUND_NAME */
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
* Description    : �Թ�����ȵ��㷨ö������U���е������ļ���Ŀ¼
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
    OldFileCount = FileCount = 0;                                               /* ���ļ��ṹ���� */
    FileNameBuffer[ 0 ].Name[ 0 ] = '/';                                        /* ��Ŀ¼,������·����,����Ŀ¼�Ǿ���·��֮�ⶼ�����·�� */
    FileNameBuffer[ 0 ].Name[ 1 ] = 0;
    FileNameBuffer[ 0 ].DirStartClust = 0;                                      /* ��Ŀ¼��������������� */
    FileNameBuffer[ 0 ].Attr = ATTR_DIRECTORY;                                  /* ��Ŀ¼Ҳ��Ŀ¼,��Ϊ��һ����¼���� */

    for( FileCount = 1; OldFileCount < FileCount; OldFileCount ++ ) 
	{  
		printf("temp1 = %d FileCount = %d OldFileCount = %d \r\n",temp1++,FileCount,OldFileCount);
        /* ������ö�ٵ����ļ����ṹδ���з��� */
        if( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) 
        {  
            /* ��Ŀ¼���������������� */
            strcpy( (PCHAR)mCmdParam.Open.mPathName, (PCCHAR)FileNameBuffer[ OldFileCount ].Name );  /* Ŀ¼��,����Ŀ¼�ⶼ�����·�� */
			printf("mPathName = %s \r\n",mCmdParam.Open.mPathName);
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* ��ǰĿ¼���ϼ�Ŀ¼����ʼ�غ�,���������·����,������·�����ٶȿ� */
            i = CH56xFileOpen( );                                               /* ��Ŀ¼,��Ϊ�˻�ȡĿ¼����ʼ�غ�������ٶ� */
            if( i == ERR_SUCCESS ) 
            {
                return( ERR_MISS_DIR );                                         /* Ӧ���Ǵ���Ŀ¼,���Ƿ��ؽ���Ǵ����ļ� */
            }
            if( i != ERR_OPEN_DIR ) 
            {
                return( i );
            }
            if( OldFileCount ) 
            {
                CurrentDirStartClust = CH56xvStartCluster;                      /* ���Ǹ�Ŀ¼,��ȡĿ¼����ʼ�غ� */
            }
            else
            {  
                /* �Ǹ�Ŀ¼,��ȡ��Ŀ¼����ʼ�غ� */
                if( CH56xvDiskFat == DISK_FAT32 ) 
                {
                    CurrentDirStartClust = CH56xvDiskRoot;                      /* FAT32��Ŀ¼ */
                }
                else 
                {
                    CurrentDirStartClust = 0;                                   /* FAT12/FAT16��Ŀ¼ */
                }
            }
            CH56xFileClose();                                                  /* ���ڸ�Ŀ¼һ��Ҫ�ر� */
//            strcpy( mCmdParam.Open.mPathName, FileNameBuffer[ OldFileCount ].Name );  /* Ŀ¼��,����mPathNameδ���޸����������ٸ��� */
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* ��ǰĿ¼���ϼ�Ŀ¼����ʼ�غ�,���������·����,������·�����ٶȿ� */
            i = ListFile( );                                                    /* ö��Ŀ¼,�ɻص�����xFileNameEnumer������¼���浽�ṹ�� */
            if( i != ERR_SUCCESS ) 
            {
                return( i );
            }
        }
    }

    /* U���е��ļ���Ŀ¼ȫ��ö�����,���濪ʼ���ݽṹ��¼���δ��ļ� */
    printf( "Total file&dir = %d, Open every file:\n", FileCount );
    for( OldFileCount = 0; OldFileCount < FileCount; OldFileCount ++ ) 
    {
        if( ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) == 0 ) 
        {  
            /* ���ļ����,Ŀ¼������ */
            printf( "Open file: %s DirStartClust = %d \n", FileNameBuffer[ OldFileCount ].Name ,FileNameBuffer[ OldFileCount ].DirStartClust);
            strcpy( (PCHAR)mCmdParam.Open.mPathName, (PCCHAR)FileNameBuffer[ OldFileCount ].Name );  /* ���·�� */
            CH56xvStartCluster = FileNameBuffer[ OldFileCount ].DirStartClust;  /* ��ǰ�ļ����ϼ�Ŀ¼����ʼ�غ�,���������·����,������·�����ٶȿ� */
            i = CH56xFileOpen();                                               /* ���ļ� */
            if( i == ERR_SUCCESS ) 
            {  
                /* �ɹ������ļ� */
                mCmdParam.Read.mDataBuffer = (PUINT8)( & EHCI_Data_Buf[ 0 ] );  /* ָ���ļ����ݻ���������ʼ��ַ */
                mCmdParam.Read.mSectorCount = 1;                                /* ��ȡ������ */
                CH56xFileRead();
				
				
//                CH56xFileClose( );                                            /* ����д������������ر� */
            }
        }
    }
    return( ERR_SUCCESS );
}

/*******************************************************************************
* Function Name  : xFileNameEnumer
* Description    : �ļ���ö�ٻص��ӳ���,�ο�CH56XUFI.H�ļ��е�����
*                  ÿ������һ���ļ�FileOpen������ñ��ص�����,xFileNameEnumer��
                   �غ�,FileOpen�ݼ�CH56xvFileSize������ö��ֱ�����������ļ���
                   ��Ŀ¼.
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
    pFileDir = (P_FAT_DIR_INFO)( pDISK_BASE_BUF + CH56xvFdtOffset );            /* ��ǰFDT����ʼ��ַ */
    if( pFileDir -> DIR_Name[ 0 ] == '.' ) 
    {
        return;                                                                 /* �Ǳ��������ϼ�Ŀ¼��,���붪�������� */
    }    
    if( ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) == 0 ) 
    {  
        /* �ж����ļ��� */
        if( pFileDir -> DIR_Name[ 8 ] == 'H' && pFileDir -> DIR_Name[ 9 ] == ' ' /* �����ļ�����չ��,��".H"�ļ�,����,���Ǽǲ����� */
         || pFileDir -> DIR_Name[ 8 ] == 'E' && pFileDir -> DIR_Name[ 9 ] == 'X' && pFileDir -> DIR_Name[10] == 'E' ) 
        {
            return;                                                             /* ��չ����".EXE"���ļ�,���� */
        }
    }
    pNameBuf = & FileNameBuffer[ FileCount ].Name[ 0 ];                         /* �ļ����ṹ�е��ļ��������� */
    for( i = 0; i < 11; i ++ ) 
    {  
        /* �����ļ���,����Ϊ11���ַ� */
        c = pFileDir -> DIR_Name[ i ];
        if ( i == 0 && c == 0x05 ) 
        {
            c = 0xE5;                                                           /* �����ַ� */
        }
        if( c != 0x20 ) 
        {  
            /* ��Ч�ַ� */
            if( i == 8 ) 
            {  
                /* ������չ�� */
                *pNameBuf = '.';                                                /* �ָ��� */
                pNameBuf ++;
            }
            *pNameBuf = c;                                                      /* �����ļ�����һ���ַ� */
            pNameBuf ++;
        }
    }
    *pNameBuf = 0;                                                              /* ��ǰ�ļ�������·���Ľ����� */
    FileNameBuffer[ FileCount ].DirStartClust = CurrentDirStartClust;           /* ��¼��ǰĿ¼����ʼ�غ�,���ڼӿ��ļ����ٶ� */
    FileNameBuffer[ FileCount ].Attr = pFileDir -> DIR_Attr;                    /* �ļ����� */
    if( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) 
    {  
        /* �ж���Ŀ¼�� */
        printf( "Dir %4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );
    }
    else 
    {  
        /* �ж����ļ��� */
        printf( "File%4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );
    }
	printf("��ǰFileCount = %d \r\n",FileCount);
    FileCount ++;                                                               /* �ļ����� */
    if( FileCount >= MAX_FILE_COUNT ) 
    {  
        /* �ļ����ṹ������̫С,�ṹ�������� */
        CH56xvFileSize = 1;                                                     /* ǿ����ǰ����ö��,����FileOpen�����ٻص�xFileNameEnumer������ǰ����,��ֹ��������� */
        printf( "FileName Structure Full\n" );
    }
}


/************************ USB ��غ��� ******************************/
UINT8 Flie_Save( UINT8 *pname, UINT32 Num, UINT8 *Data )
{
	UINT8 i;
	strcpy( (PCHAR)&mCmdParam.Open.mPathName, (PCCHAR)pname ); 
	i = CH56xFileOpen( );											            /* ���ļ� */
#if MY_DEBUG_PRINTF
	printf( "CH563FileOpen:%x \n", i );
#endif
	if( i == ERR_SUCCESS ){	
	    mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF;
		CH56xByteLocate( );												        /* �ļ�ƫ�Ƶ��ļ�β */
	}
	else if( i == ERR_MISS_FILE ){
		strcpy( (PCHAR)&mCmdParam.Create.mPathName, (PCCHAR)pname ); 
		CH56xFileCreate( );   
	}																	        /* �����ļ� */
	else return( i );
	mCmdParam.ByteWrite.mByteBuffer = Data;
	mCmdParam.ByteWrite.mByteCount = Num;
	i = CH56xByteWrite( );											            /* д�ļ� */
#if MY_DEBUG_PRINTF
	printf( "CH563ByteWrite:%x \n", i );
#endif
	if( i != ERR_SUCCESS )	 return ( i );
	mCmdParam.Close.mUpdateLen = 1;
	i = CH56xFileClose( );											            /* �ر��ļ��������ļ����� */
#if MY_DEBUG_PRINTF
	printf( "CH563FileClose:%x \n" , i );
#endif
	return( i );
}

/*******************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int    main( void ) 
{
    UINT8 s,i,c;
    UINT8 buf[100];

    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* ������LED��һ����ʾ���� */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    mInitSTDIO( );                                                              /* Ϊ���ü����ͨ�����ڼ����ʾ���� */

// 	i = CH56xLibInit( );                                                        /* ��ʼ��CH563������CH563оƬ,�����ɹ�����0 */
//     mStopIfError( i );

// 	SD_Port_Init();
// 	i = DISK_Init();
// 	if(i!=0x00)    //err
// 	{
// 		printf("sd card err \n");
// 		while(1);
// 	}
	
    SD_Card_Init();
    /* ������·��ʼ�� */
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
										i = sprintf( buf,"Note: \xd\xa������������ֽ�Ϊ��λ����U���ļ���д,559����ʾ���ܡ�\xd\xa");/*��ʾ */
										for(c=0; c<2; c++)
										{
												mCmdParam.ByteWrite.mByteCount = i;     /* ָ������д����ֽ��� */
												mCmdParam.ByteWrite.mByteBuffer = buf;  /* ָ�򻺳��� */
												s = CH56xByteWrite( );                  /* ���ֽ�Ϊ��λ���ļ�д������ */
												mStopIfError( s);
												printf("�ɹ�д�� %02X��\n",(UINT16)c);
											Delay_ms(100);
										}	
										printf( "Close\n" );
										mCmdParam.Close.mUpdateLen = 1;             //�Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ�����
										i = CH56xFileClose( );		
										mCmdParam.Close.mUpdateLen = 1;             //�Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ�����
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
												mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF; //�����ļ���ǰ608���ֽڿ�ʼ��д
												CH56xByteLocate( );											
										i = sprintf( buf,"Note: \xd\xa������������ֽ�Ϊ��λ����U���ļ���д,559����ʾ���ܡ�\xd\xa");/*��ʾ */
										for(c=0; c<2; c++)
										{
												mCmdParam.ByteWrite.mByteCount = i;     /* ָ������д����ֽ��� */
												mCmdParam.ByteWrite.mByteBuffer = buf;  /* ָ�򻺳��� */
												s = CH56xByteWrite( );                  /* ���ֽ�Ϊ��λ���ļ�д������ */
												mStopIfError( s );
												printf("�ɹ�д�� %02X��\n",(UINT16)c);
											Delay_ms(100);
										}	
										printf( "Close\n" );
										mCmdParam.Close.mUpdateLen = 1;             //�Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ�����
										i = CH56xFileClose( );		
										mCmdParam.Close.mUpdateLen = 1;             //�Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ�����
										i = CH56xFileClose( );										
										}										
    while ( 1 ) 
    {
		
	  }
}


/* 2014.09.09
*****************************************
**   Copyright  (C)  W.ch  1999-2014   **
**   Web:      http://wch.cn           **
*****************************************
**  USB-flash File Interface for CH56x **
**  ADS 1.2, Keil 3.11/4.12 @ARM946E-S,v5TE **
*****************************************
*/
/* CH56x U�������ļ�ϵͳ�ӿ�, ֧��: FAT12/FAT16/FAT32 */

//#define DISK_BASE_BUF_LEN		512	/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��,Ϊ0���ֹ�ڱ��ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
/* �����Ҫ���ô������ݻ������Խ�ԼRAM,��ô�ɽ�DISK_BASE_BUF_LEN����Ϊ0�Խ�ֹ�ڱ��ļ��ж��建����,����Ӧ�ó����ڵ���CH56xLibInit֮ǰ��������������õĻ�������ʼ��ַ����pDISK_BASE_BUF���� */

//#define NO_DEFAULT_ACCESS_SECTOR	1		/* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_FILE_ENUMER		1		/* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */

//#include "CH56XBAS.H"
#include "CH56XUFI.H"

CMD_PARAM mCmdParam;						/* ������� */
#if DISK_BASE_BUF_LEN > 0
__align(8)	UINT8  DISK_BASE_BUF[ DISK_BASE_BUF_LEN ];	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
__align(8)	UINT8  DISK_FAT_BUF[ DISK_BASE_BUF_LEN ];	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
#endif

/* ���³�����Ը�����Ҫ�޸� */

#ifndef NO_DEFAULT_ACCESS_SECTOR		/* ��Ӧ�ó����ж���NO_DEFAULT_ACCESS_SECTOR���Խ�ֹĬ�ϵĴ���������д�ӳ���,Ȼ�������б�д�ĳ�������� */

//if ( use_external_interface ) {  // �滻U�������ײ��д�ӳ���
//    CH374vSectorSize=512;  // ����ʵ�ʵ�������С,��ֵ������512�ı���
//    CH56xvSectorSizeB=9;   // ����ʵ�ʵ�������С��λ����,512���Ӧ9,1024��Ӧ10,2048��Ӧ11
//    CH374DiskStatus=DISK_MOUNTED;  // ǿ�ƿ��豸���ӳɹ�(ֻ������ļ�ϵͳ)
//}

#ifdef __UDISK_H__

extern BULK_ONLY_CMD	mBOC;

#else

typedef union _BULK_ONLY_CMD0 {
	struct {
		UINT32	mCBW_Sig;
		UINT32	mCBW_Tag;
		UINT32	mCBW_DataLen;			/* ����: ���ݴ��䳤�� */
		UINT8	mCBW_Flag;				/* ����: ���䷽��ȱ�־ */
		UINT8	mCBW_LUN;
		UINT8	mCBW_CB_Len;			/* ����: �����ĳ���,��Чֵ��1��16 */
		UINT8	mCBW_CB_Buf[16];		/* ����: �����,�û��������Ϊ16���ֽ� */
	} mCBW;								/* BulkOnlyЭ��������, ����CBW�ṹ */
	struct {
		UINT32	mCSW_Sig;
		UINT32	mCSW_Tag;
		UINT32	mCSW_Residue;			/* ����: ʣ�����ݳ��� */
		UINT8	mCSW_Status;			/* ����: ����ִ�н��״̬ */
		UINT8	mReserved;
	} mCSW;								/* BulkOnlyЭ�������״̬��, ���CSW�ṹ */
} BULK_ONLY_CMD0;						/* CMD_BulkOnlyCmd, ִ�л���BulkOnlyЭ�������, ��������ݴ�����ô������pDISK_BASE_BUF�� */

extern BULK_ONLY_CMD0	mBOC;

#endif

extern UINT8 MS_ScsiCmd_Process( UINT8 *DataBuf );

UINT8	CH56xReadSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* �Ӵ��̶�ȡ������������ݵ��������� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extReadSector( StartLba, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		mBOC.mCBW.mCBW_DataLen = SectCount << CH56xvSectorSizeB;
		mBOC.mCBW.mCBW_Flag = 0x80;
		mBOC.mCBW.mCBW_CB_Len = 10;
		mBOC.mCBW.mCBW_CB_Buf[ 0 ] = SPC_CMD_READ10;
		mBOC.mCBW.mCBW_CB_Buf[ 1 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 2 ] = (UINT8)( StartLba >> 24 );
		mBOC.mCBW.mCBW_CB_Buf[ 3 ] = (UINT8)( StartLba >> 16 );
		mBOC.mCBW.mCBW_CB_Buf[ 4 ] = (UINT8)( StartLba >> 8 );
		mBOC.mCBW.mCBW_CB_Buf[ 5 ] = (UINT8)( StartLba );
		mBOC.mCBW.mCBW_CB_Buf[ 6 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 7 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 8 ] = SectCount;
		mBOC.mCBW.mCBW_CB_Buf[ 9 ] = 0x00;
		CH56xIntStatus = MS_ScsiCmd_Process( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH56xIntStatus == ERR_SUCCESS ) {
			return( CH56xIntStatus );
		}
		CH56xIntStatus = CH56xAnalyzeError( retry );
		if ( CH56xIntStatus != ERR_SUCCESS ) {
			return( CH56xIntStatus );
		}
	}
	return( CH56xIntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}

#ifdef EN_DISK_WRITE

extern void Delay_us( int n );

UINT8	CH56xWriteSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )  /* ���������еĶ�����������ݿ�д����� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extWriteSector( StartLba, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		mBOC.mCBW.mCBW_DataLen = SectCount << CH56xvSectorSizeB;
		mBOC.mCBW.mCBW_Flag = 0x00;
		mBOC.mCBW.mCBW_CB_Len = 10;
		mBOC.mCBW.mCBW_CB_Buf[ 0 ] = SPC_CMD_WRITE10;
		mBOC.mCBW.mCBW_CB_Buf[ 1 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 2 ] = (UINT8)( StartLba >> 24 );
		mBOC.mCBW.mCBW_CB_Buf[ 3 ] = (UINT8)( StartLba >> 16 );
		mBOC.mCBW.mCBW_CB_Buf[ 4 ] = (UINT8)( StartLba >> 8 );
		mBOC.mCBW.mCBW_CB_Buf[ 5 ] = (UINT8)( StartLba );
		mBOC.mCBW.mCBW_CB_Buf[ 6 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 7 ] = 0x00;
		mBOC.mCBW.mCBW_CB_Buf[ 8 ] = SectCount;
		mBOC.mCBW.mCBW_CB_Buf[ 9 ] = 0x00;
		CH56xIntStatus = MS_ScsiCmd_Process( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH56xIntStatus == ERR_SUCCESS ) {
			Delay_us( 200 );  /* д��������ʱ */
			return( CH56xIntStatus );
		}
		CH56xIntStatus = CH56xAnalyzeError( retry );
		if ( CH56xIntStatus != ERR_SUCCESS ) {
			return( CH56xIntStatus );
		}
	}
	return( CH56xIntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}

#endif
#endif  // NO_DEFAULT_ACCESS_SECTOR

#ifndef NO_DEFAULT_FILE_ENUMER			/* ��Ӧ�ó����ж���NO_DEFAULT_FILE_ENUMER���Խ�ֹĬ�ϵ��ļ���ö�ٻص�����,Ȼ�������б�д�ĳ�������� */
void xFileNameEnumer( void )			/* �ļ���ö�ٻص��ӳ��� */
{
/* ���ָ��ö�����CH56xvFileSizeΪ0xFFFFFFFF�����FileOpen����ôÿ������һ���ļ�FileOpen������ñ��ص�����
   �ص�����xFileNameEnumer���غ�FileOpen�ݼ�CH56xvFileSize������ö��ֱ�����������ļ�����Ŀ¼�����������ǣ�
   �ڵ���FileOpen֮ǰ����һ��ȫ�ֱ���Ϊ0����FileOpen�ص�������󣬱�������CH56xvFdtOffset�õ��ṹFAT_DIR_INFO��
   �����ṹ�е�DIR_Attr�Լ�DIR_Name�ж��Ƿ�Ϊ�����ļ�������Ŀ¼������¼�����Ϣ������ȫ�ֱ�������������
   ��FileOpen���غ��жϷ���ֵ�����ERR_MISS_FILE��ERR_FOUND_NAME����Ϊ�����ɹ���ȫ�ֱ���Ϊ����������Ч�ļ�����
   ����ڱ��ص�����xFileNameEnumer�н�CH56xvFileSize��Ϊ1����ô����֪ͨFileOpen��ǰ���������������ǻص��������� */
#if 0
	UINT8			i;
	UINT16			FileCount;
	P_FAT_DIR_INFO	pFileDir;
	PUINT8			NameBuf;
	pFileDir = (P_FAT_DIR_INFO)( pDISK_BASE_BUF + CH56xvFdtOffset );  /* ��ǰFDT����ʼ��ַ */
	FileCount = (UINT16)( 0xFFFFFFFF - CH56xvFileSize );  /* ��ǰ�ļ�����ö�����,CH56xvFileSize��ֵ��0xFFFFFFFF,�ҵ��ļ�����ݼ� */
	if ( FileCount < sizeof( FILE_DATA_BUF ) / 12 ) {  /* ��黺�����Ƿ��㹻���,�ٶ�ÿ���ļ�����ռ��12���ֽڴ�� */
		NameBuf = & FILE_DATA_BUF[ FileCount * 12 ];  /* ���㱣�浱ǰ�ļ����Ļ�������ַ */
		for ( i = 0; i < 11; i ++ ) NameBuf[ i ] = pFileDir -> DIR_Name[ i ];  /* �����ļ���,����Ϊ11���ַ�,δ�����ո� */
		if ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) NameBuf[ i ] = 1;  /* �ж���Ŀ¼�� */
		NameBuf[ i ] = 0;  /* �ļ��������� */
	}
#endif
}
#endif  // NO_DEFAULT_FILE_ENUMER

UINT8	CH56xLibInit( void )  /* ��ʼ��CH56x������CH56xоƬ,�����ɹ�����0 */
{
	if ( CH56xGetVer( ) < CH56x_LIB_VER ) return( 0xFF );  /* ��ȡ��ǰ�ӳ����İ汾��,�汾̫���򷵻ش��� */
#if DISK_BASE_BUF_LEN > 0
	pDISK_BASE_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ������ݻ����� */
	pDISK_FAT_BUF = & DISK_FAT_BUF[0];  /* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,������pDISK_BASE_BUF�����Խ�ԼRAM */
/* ���ϣ������ļ���ȡ�ٶ�,��ô�������������е���CH56xLibInit֮��,��pDISK_FAT_BUF����ָ����һ�������������pDISK_BASE_BUFͬ����С�Ļ����� */
#endif
	CH56xDiskStatus = DISK_UNKNOWN;  /* δ֪״̬ */
	return( ERR_SUCCESS );
}
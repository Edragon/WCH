// CH37XDBG.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CH37XDBG.h"
//#include "CH37XDBGDlg.h"
//#include "total.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGApp

BEGIN_MESSAGE_MAP(CCH37XDBGApp, CWinApp)
	//{{AFX_MSG_MAP(CCH37XDBGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGApp construction
ULONG	CH375DBG_Index;			/* CH372/CH375�豸��� */
BOOL	CH375DBG_Opened;	/* CH372/CH375�豸���򿪱�־ */

CCH37XDBGApp::CCH37XDBGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCH37XDBGApp object

CCH37XDBGApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGApp initialization

BOOL CCH37XDBGApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CCH37XDBGDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL	CH375DBG_GetFirmwareInfo( PUCHAR ver ) 
{  // ��ȡ���Թ̼�����İ汾,��ȡ��δ��ɵ��ϴ����ݿ�
// �������: ver ָ��һ���ֽڱ�����Ԫ,���ڴ�Ż�ȡ�İ汾��
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( CH375DBG_Opened == FALSE ) 
	{  // CH372/CH375�豸��δ����
		CH375DBG_Opened = CH375OpenDevice( CH375DBG_Index ) != INVALID_HANDLE_VALUE;  // ��CH375�豸
		if ( CH375DBG_Opened == FALSE ) 
			return( FALSE );
	}
	down.mCommand = USB_CMD_GET_FW_INFO;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = 0;
	down.mLength = 0;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // �����ɹ�,���ҷ�������
				*ver = up.mBuffer[0];  // ���ذ汾��
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

VOID	CH375DBG_CloseExit( VOID ) 
{  // �ر�CH372/CH375�豸
	CH375DBG_Opened = FALSE;
	CH375CloseDevice( CH375DBG_Index );
}

BOOL	CH375DBG_GetAppSysInfo( PUCHAR ver, PUCHAR StringBuffer ) 
{  // ��ȡ��ǰӦ��ϵͳ�İ汾��˵���ַ���
// �������: ver ָ��һ���ֽڱ�����Ԫ,���ڴ�Ż�ȡ�İ汾��
//           StringBuffer ָ��һ���㹻����ַ���������,���ڴ�Ż�ȡ��˵���ַ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	down.mCommand = USB_CMD_GET_APP_INFO;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = 0;
	down.mLength = 0;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // �����ɹ�,���ҷ�������
				*ver = up.mBuffer[0];  // ���ذ汾��
				for ( len = 0; len < (ULONG)( up.mLength - 1 ); len ++ ) 
					StringBuffer[ len ] = up.mBuffer[ len + 1 ];  // ����˵���ַ���
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadSFR( ULONG addr, PUCHAR data ) 
{  // ��SFRһ����Ԫ��ȡ����
// �������: addr ָ��SFR�ĵ�ַ
//           data ָ��һ���ֽڱ�����Ԫ,���ڴ�Ŷ���������
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_SFR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = addr;
	down.mLength = 1;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) 
			{  // �����ɹ�,���ҷ�������
				*data = up.mBuffer[0];  // ��������
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteSFR( ULONG addr, UCHAR data ) 
{  // ��SFRһ����Ԫд������
// �������: addr ָ��SFR�ĵ�ַ
//           data ָ��׼��д�������
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
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // �����ɹ�
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // ���ڲ�RAM��ȡ���ݿ�
// �������: StartAddr ָ���ڲ�RAM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // ���Ƶ��δ�������ݳ���,�����뵥Ƭ�������MAX_DATA_SIZEһ��ͬ���޸�Ϊ������С��ֵ
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_IRAM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // �����ɹ�,���ҷ�������
				for ( len = 0; len < (ULONG)count; len ++ ) 
					buffer[ len ] = up.mBuffer[ len ];  // ��������
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) {  // ���ڲ�RAMд�����ݿ�
// �������: StartAddr ָ���ڲ�RAM����ʼ��ַ
//           buffer ָ��һ�����ݻ�����,���׼��д������ݿ�
//           count ָ��д����ֽ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // ���Ƶ��δ�������ݳ���,�����뵥Ƭ�������MAX_DATA_SIZEһ��ͬ���޸�Ϊ������С��ֵ
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_IRAM | USB_CMD_MEM_DIR_WR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	for ( len = 0; len < (ULONG)count; len ++ ) 
		down.mBuffer[ len ] = buffer[ len ];
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) // д�������
	{
		
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // �����ɹ�
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // ���ⲿRAM��ȡ���ݿ�
// �������: StartAddr ָ���ⲿRAM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // ���Ƶ��δ�������ݳ���,�����뵥Ƭ�������MAX_DATA_SIZEһ��ͬ���޸�Ϊ������С��ֵ
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_XRAM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // �����ɹ�,���ҷ�������
				for ( len = 0; len < (ULONG)count; len ++ ) 
					buffer[ len ] = up.mBuffer[ len ];  // ��������
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_WriteXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // ���ⲿRAMд�����ݿ�
// �������: StartAddr ָ���ⲿRAM����ʼ��ַ
//           buffer ָ��һ�����ݻ�����,���׼��д������ݿ�
//           count ָ��д����ֽ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) 
		return( FALSE );  // ���Ƶ��δ�������ݳ���,�����뵥Ƭ�������MAX_DATA_SIZEһ��ͬ���޸�Ϊ������С��ֵ
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_XRAM | USB_CMD_MEM_DIR_WR;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	for ( len = 0; len < (ULONG)count; len ++ ) 
		down.mBuffer[ len ] = buffer[ len ];
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot ) 
			{  // �����ɹ�
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

BOOL	CH375DBG_ReadROM( ULONG StartAddr, PUCHAR buffer, UCHAR count ) 
{  // �ӳ���ROM��ȡ���ݿ�
// �������: StartAddr ָ������ROM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) return( FALSE );  // ���Ƶ��δ�������ݳ���,�����뵥Ƭ�������MAX_DATA_SIZEһ��ͬ���޸�Ϊ������С��ֵ
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_ROM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   // д�������
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // ��ȡӦ���
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) 
			{  // �����ɹ�,���ҷ�������
				for ( len = 0; len < (ULONG)count; len ++ ) buffer[ len ] = up.mBuffer[ len ];  // ��������
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

UCHAR	CH375DBG_GetPortP1( VOID ) 
{  // ��ȡMCS51��Ƭ���˿�P1��״̬
// ������: ���ص�ǰ�˿�״ֵ̬
	UCHAR	d;
	CH375DBG_ReadSFR( 0x90, &d );  // ��SFRһ����Ԫ��ȡ����
	return( d );
}

VOID	CH375DBG_PutPortP1( UCHAR d ) 
{  // ����MCS51��Ƭ���˿�P1��״̬
// �������: d ָ���µĶ˿�״ֵ̬
	CH375DBG_WriteSFR( 0x90, d );  // ��SFRһ����Ԫд������
}
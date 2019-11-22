// CH37XDBG.h : main header file for the CH37XDBG application
//

#if !defined(AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_)
#define AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "total.h"
/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGApp:
// See CH37XDBG.cpp for the implementation of this class
//


class CCH37XDBGApp : public CWinApp
{
public:
	CCH37XDBGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCH37XDBGApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCH37XDBGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/* **************************************************************************************************** */
// �ṩ����������õ��ӳ���


// Ӧ�ó���������,�����в���֮ǰ,�������ȵ���һ�������CH375DBG_GetFirmwareInfo�ӳ���
BOOL	CH375DBG_GetFirmwareInfo( PUCHAR ver );  //��ȡ���Թ̼�����İ汾,��ȡ��δ��ɵ��ϴ����ݿ�
// �������: ver ָ��һ���ֽڱ�����Ԫ,���ڴ�Ż�ȡ�İ汾��


// Ӧ�ó����˳�ǰ,�����в���֮��,Ӧ�õ���һ�������CH375DBG_CloseExit�ӳ���
VOID	CH375DBG_CloseExit( VOID );  //�ر�CH372/CH375�豸


BOOL	CH375DBG_GetAppSysInfo( PUCHAR ver, PUCHAR StringBuffer );  //��ȡ��ǰӦ��ϵͳ�İ汾��˵���ַ���
// �������: ver ָ��һ���ֽڱ�����Ԫ,���ڴ�Ż�ȡ�İ汾��
//           StringBuffer ָ��һ���㹻����ַ���������,���ڴ�Ż�ȡ��˵���ַ���


BOOL	CH375DBG_ReadSFR( ULONG addr, PUCHAR data );  //��SFRһ����Ԫ��ȡ����
// �������: addr ָ��SFR�ĵ�ַ
//           data ָ��һ���ֽڱ�����Ԫ,���ڴ�Ŷ���������


BOOL	CH375DBG_WriteSFR( ULONG addr, UCHAR data );  //��SFRһ����Ԫд������
// �������: addr ָ��SFR�ĵ�ַ
//           data ָ��׼��д�������


BOOL	CH375DBG_ReadIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //���ڲ�RAM��ȡ���ݿ�
// �������: StartAddr ָ���ڲ�RAM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���


BOOL	CH375DBG_WriteIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //���ڲ�RAMд�����ݿ�
// �������: StartAddr ָ���ڲ�RAM����ʼ��ַ
//           buffer ָ��һ�����ݻ�����,���׼��д������ݿ�
//           count ָ��д����ֽ���


BOOL	CH375DBG_ReadXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //���ⲿRAM��ȡ���ݿ�
// �������: StartAddr ָ���ⲿRAM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���


BOOL	CH375DBG_WriteXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //���ⲿRAMд�����ݿ�
// �������: StartAddr ָ���ⲿRAM����ʼ��ַ
//           buffer ָ��һ�����ݻ�����,���׼��д������ݿ�
//           count ָ��д����ֽ���


BOOL	CH375DBG_ReadROM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //�ӳ���ROM��ȡ���ݿ�
// �������: StartAddr ָ������ROM����ʼ��ַ
//           buffer ָ��һ���㹻������ݻ�����,���ڴ�Ŷ��������ݿ�
//           count ָ����ȡ���ֽ���


UCHAR	CH375DBG_GetPortP1( VOID );  //��ȡMCS51��Ƭ���˿�P1��״̬
// ������: ���ص�ǰ�˿�״ֵ̬


VOID	CH375DBG_PutPortP1( UCHAR d );  //����MCS51��Ƭ���˿�P1��״̬
// �������: d ָ���µĶ˿�״ֵ̬

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_)

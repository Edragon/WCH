#if !defined(AFX_SFRPAGE_H__9C3CCC71_33A6_46F7_A8E3_8CACD24910B9__INCLUDED_)
#define AFX_SFRPAGE_H__9C3CCC71_33A6_46F7_A8E3_8CACD24910B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SFRPage.h : header file
//
#include "CH37XDBGDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSFRPage dialog

class CSFRPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSFRPage)

// Construction
public:
	BOOL mWriteSFR(ULONG add,CString value);
//	BOOL m_open;
//	BOOL CH375DBG_GetFirmwareInfo( PUCHAR ver );
	CCH37XDBGDlg * m_mdlg;
	CSFRPage();
	~CSFRPage();

// Dialog Data
	//{{AFX_DATA(CSFRPage)
	enum { IDD = IDD_DLGSFR };
	CString	m_name1;
	CString	m_name2;
	CString	m_name3;
	CString	m_name4;
	CString	m_name5;
	CString	m_name6;
	CString	m_name7;
	CString	m_name8;
	CString	m_value2;
	CString	m_value3;
	CString	m_value4;
	CString	m_value5;
	CString	m_value6;
	CString	m_value7;
	CString	m_value8;
	CString	m_ver;
	CString	m_name10;
	CString	m_name9;
	CString	m_name11;
	CString	m_name12;
	CString	m_name13;
	CString	m_name14;
	CString	m_name15;
	CString	m_name16;
	CString	m_value9;
	CString	m_value16;
	CString	m_value15;
	CString	m_value14;
	CString	m_value13;
	CString	m_value12;
	CString	m_value11;
	CString	m_value10;
	CString	m_value1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSFRPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSFRPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnButtonRefresh();
	afx_msg void OnKillfocusEditValue1();
	afx_msg void OnKillfocusEditValue2();
	afx_msg void OnKillfocusEditValue3();
	afx_msg void OnKillfocusEditValue4();
	afx_msg void OnKillfocusEditValue5();
	afx_msg void OnKillfocusEditValue6();
	afx_msg void OnKillfocusEditValue7();
	afx_msg void OnKillfocusEditValue8();
	afx_msg void OnKillfocusEditValue9();
	afx_msg void OnKillfocusEditValue10();
	afx_msg void OnKillfocusEditValue11();
	afx_msg void OnKillfocusEditValue12();
	afx_msg void OnKillfocusEditValue13();
	afx_msg void OnKillfocusEditValue14();
	afx_msg void OnKillfocusEditValue15();
	afx_msg void OnKillfocusEditValue16();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFRPAGE_H__9C3CCC71_33A6_46F7_A8E3_8CACD24910B9__INCLUDED_)

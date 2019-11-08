#if !defined(AFX_EXTERNPAGE_H__378E9886_AAC7_40C1_A4FB_EDA5468C3E55__INCLUDED_)
#define AFX_EXTERNPAGE_H__378E9886_AAC7_40C1_A4FB_EDA5468C3E55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExternPage.h : header file
//
#include "CH37XDBGDlg.h"
#include "total.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CExternPage dialog

class CExternPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CExternPage)

// Construction
public:
	BOOL mBcdToStr(PUCHAR buf,ULONG len);
	BOOL mStrToBcd(PUCHAR buf,ULONG len);
	CCH37XDBGDlg * m_mdlg;
	CExternPage();
	~CExternPage();

// Dialog Data
	//{{AFX_DATA(CExternPage)
	enum { IDD = IDD_DLGEXTERN };
	CString	m_len;
	CString	m_comm;
	CString	m_para1;
	CString	m_para2;
	CString	m_para3;
	CString	m_para4;
	CString	m_buf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExternPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExternPage)
	afx_msg void OnClose();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonReceive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNPAGE_H__378E9886_AAC7_40C1_A4FB_EDA5468C3E55__INCLUDED_)

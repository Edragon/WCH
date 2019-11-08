// CH37XDBGDlg.h : header file
//
//#include "total.h"


#if !defined(AFX_CH37XDBGDLG_H__D26D8FF1_1494_4A7C_B8FE_D19CAA9FA378__INCLUDED_)
#define AFX_CH37XDBGDLG_H__D26D8FF1_1494_4A7C_B8FE_D19CAA9FA378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGDlg dialog

class CCH37XDBGDlg : public CDialog
{
// Construction
public:
	//BOOL m_open;
	CCH37XDBGDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCH37XDBGDlg)
	enum { IDD = IDD_CH37XDBG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCH37XDBGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCH37XDBGDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH37XDBGDLG_H__D26D8FF1_1494_4A7C_B8FE_D19CAA9FA378__INCLUDED_)

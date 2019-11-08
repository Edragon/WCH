//{{AFX_INCLUDES()
#include "msdgridctrl.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_XRAMPAGE_H__066A51BF_9451_406B_B8DE_D6FC57A82F82__INCLUDED_)
#define AFX_XRAMPAGE_H__066A51BF_9451_406B_B8DE_D6FC57A82F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XRAMPage.h : header file
//
#include "CH37XDBGDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CXRAMPage dialog

class CXRAMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CXRAMPage)

// Construction
public:
	BOOL mReadXRam();
	UCHAR mCharToBcd(UCHAR iChar );
	CCH37XDBGDlg * m_mdlg;
	CXRAMPage();
	~CXRAMPage();

// Dialog Data
	//{{AFX_DATA(CXRAMPage)
	enum { IDD = IDD_DLGXRAM };
	CMSFlexGrid	m_xgrid;
	CString	m_addr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CXRAMPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CXRAMPage)
	afx_msg void OnClose();
	afx_msg void OnButtonRead();
	afx_msg void OnDblClickXgrid();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRAMPAGE_H__066A51BF_9451_406B_B8DE_D6FC57A82F82__INCLUDED_)

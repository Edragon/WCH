//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_IRAMPAGE_H__3EA6AB33_6E55_4F6E_B5FE_DDF4FC79F425__INCLUDED_)
#define AFX_IRAMPAGE_H__3EA6AB33_6E55_4F6E_B5FE_DDF4FC79F425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IRAMPage.h : header file
//
#include "CH37XDBGDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CIRAMPage dialog

class CIRAMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIRAMPage)

// Construction
public:
	BOOL mReadIRam();
//	BOOL CH375DBG_ReadIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );
	CCH37XDBGDlg * m_mdlg;
	CIRAMPage();
	~CIRAMPage();

// Dialog Data
	//{{AFX_DATA(CIRAMPage)
	enum { IDD = IDD_DLGIRAM };
	CMSFlexGrid	m_igrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIRAMPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIRAMPage)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickIgrid();
	afx_msg void OnButtonRead();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IRAMPAGE_H__3EA6AB33_6E55_4F6E_B5FE_DDF4FC79F425__INCLUDED_)

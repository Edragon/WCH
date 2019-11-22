#if !defined(AFX_ALLCONTROLSSHEET_H__778776FC_BC28_43CA_9F99_31D4DD13EB51__INCLUDED_)
#define AFX_ALLCONTROLSSHEET_H__778776FC_BC28_43CA_9F99_31D4DD13EB51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllControlsSheet.h : header file
//

//#include "total.h"
#include "CH37XDBGDlg.h"
//#include "XRAMPage.h"
//#include "ExternPage.h"
//#include "SFRPage.h"
//#include "IRAMPage.h"	




/////////////////////////////////////////////////////////////////////////////
// CAllControlsSheet

class CAllControlsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAllControlsSheet)

// Construction
public:
	CCH37XDBGDlg * m_mdlg;
	CAllControlsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAllControlsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllControlsSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	
//	CExternPage m_extern;
	
	
	virtual ~CAllControlsSheet();

	// Generated message map functions
protected:
	void AddControlPages();
	//{{AFX_MSG(CAllControlsSheet)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLCONTROLSSHEET_H__778776FC_BC28_43CA_9F99_31D4DD13EB51__INCLUDED_)

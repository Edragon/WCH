#if !defined(AFX_ENTER_H__36036736_7A07_4390_B1D3_46D00687F41A__INCLUDED_)
#define AFX_ENTER_H__36036736_7A07_4390_B1D3_46D00687F41A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ENTER.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CENTER dialog

class CENTER : public CDialog
{
// Construction
public:
	int m_ram;
	BOOL mWriteData();
	LONG m_addr;
	CENTER(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CENTER)
	enum { IDD = IDD_DLGENTER };
	CString	m_data;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CENTER)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CENTER)
	afx_msg void OnButtonEnter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTER_H__36036736_7A07_4390_B1D3_46D00687F41A__INCLUDED_)

// MyUsbHidTestApp.h : main header file for the MYUSBHIDTESTAPP application
//

#if !defined(AFX_MYUSBHIDTESTAPP_H__3EB84FB6_9F1C_49C5_AADA_BE7AFC776B3F__INCLUDED_)
#define AFX_MYUSBHIDTESTAPP_H__3EB84FB6_9F1C_49C5_AADA_BE7AFC776B3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyUsbHidTestAppApp:
// See MyUsbHidTestApp.cpp for the implementation of this class
//

class CMyUsbHidTestAppApp : public CWinApp
{
public:
	CMyUsbHidTestAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUsbHidTestAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyUsbHidTestAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYUSBHIDTESTAPP_H__3EB84FB6_9F1C_49C5_AADA_BE7AFC776B3F__INCLUDED_)

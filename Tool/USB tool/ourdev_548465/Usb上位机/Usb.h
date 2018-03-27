// Usb.h : main header file for the USB application
//

#if !defined(AFX_USB_H__FF2C15E3_FE10_4538_B19F_6003A677B108__INCLUDED_)
#define AFX_USB_H__FF2C15E3_FE10_4538_B19F_6003A677B108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Usb_i.h"

/////////////////////////////////////////////////////////////////////////////
// CUsbApp:
// See Usb.cpp for the implementation of this class
//

class CUsbApp : public CWinApp
{
public:
	CUsbApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsbApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUsbApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USB_H__FF2C15E3_FE10_4538_B19F_6003A677B108__INCLUDED_)

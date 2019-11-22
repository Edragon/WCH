// AllControlsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "total.h"
#include "CH37XDBG.h"

//#include "AllControlsSheet.h"
//#include "XRAMPage.h"
//#include "ExternPage.h"
//#include "SFRPage.h"
//#include "IRAMPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllControlsSheet

IMPLEMENT_DYNAMIC(CAllControlsSheet, CPropertySheet)

CAllControlsSheet::CAllControlsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CAllControlsSheet::CAllControlsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CAllControlsSheet::~CAllControlsSheet()
{
}


BEGIN_MESSAGE_MAP(CAllControlsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAllControlsSheet)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllControlsSheet message handlers

void CAllControlsSheet::AddControlPages()
{
	CIRAMPage  m_iram;
	CSFRPage   m_sfr;
	CExternPage m_extern;
	CXRAMPage  m_xram;
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button

	AddPage(&m_sfr);
	AddPage(&m_iram);
	AddPage(&m_xram);
	AddPage(&m_extern);
}

void CAllControlsSheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->m_mdlg->EndDialog(0);
	CPropertySheet::OnClose();
}

BOOL CAllControlsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	
	
	return bResult;
}

int CAllControlsSheet::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertySheet::DoModal();
}

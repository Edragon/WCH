// CH37XDBGDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "total.h"
#include "CH37XDBG.h"
//#include "CH37XDBGDlg.h"

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
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGDlg dialog

CCH37XDBGDlg::CCH37XDBGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCH37XDBGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCH37XDBGDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCH37XDBGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCH37XDBGDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCH37XDBGDlg, CDialog)
	//{{AFX_MSG_MAP(CCH37XDBGDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGDlg message handlers

BOOL CCH37XDBGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
//	m_open = FALSE;
	CAllControlsSheet * m_allcontrolssheet = new CAllControlsSheet(_T("CH37XDBG µ÷ÊÔMCS51"));
	CExternPage m_extern;
	CSFRPage    m_sfr;
	CIRAMPage   m_iram;
	CXRAMPage   m_xram;
	m_allcontrolssheet->m_mdlg = this;
	m_extern.m_mdlg = this;
	m_sfr.m_mdlg = this;
	m_iram.m_mdlg = this;
	m_xram.m_mdlg = this;
	m_allcontrolssheet->m_psh.dwFlags |= PSP_USEHICON;
	m_allcontrolssheet->m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
//	m_allcontrolssheet->m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button

	m_allcontrolssheet->AddPage(&m_sfr);
	m_allcontrolssheet->AddPage(&m_iram);
	m_allcontrolssheet->AddPage(&m_xram);
	m_allcontrolssheet->AddPage(&m_extern);
	m_allcontrolssheet->DoModal();
	this->OnOK();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCH37XDBGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCH37XDBGDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCH37XDBGDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

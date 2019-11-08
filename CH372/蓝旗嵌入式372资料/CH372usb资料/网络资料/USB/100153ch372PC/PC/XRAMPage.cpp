// XRAMPage.cpp : implementation file
//

#include "stdafx.h"
//#include "total.h"
#include "CH37XDBG.h"
//#include "AllControlsSheet.h"
//#include "XRAMPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXRAMPage property page
extern ULONG	CH375DBG_Index;			/* CH372/CH375设备序号 */
extern BOOL	CH375DBG_Opened;	/* CH372/CH375设备被打开标志 */

IMPLEMENT_DYNCREATE(CXRAMPage, CPropertyPage)

CXRAMPage::CXRAMPage() : CPropertyPage(CXRAMPage::IDD)
{
	//{{AFX_DATA_INIT(CXRAMPage)
	m_addr = _T("");
	//}}AFX_DATA_INIT
}

CXRAMPage::~CXRAMPage()
{
}

void CXRAMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXRAMPage)
	DDX_Control(pDX, IDC_XGRID, m_xgrid);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_addr);
	DDV_MaxChars(pDX, m_addr, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXRAMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXRAMPage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXRAMPage message handlers

void CXRAMPage::OnClose() 
{
	m_mdlg->EndDialog(0);	
	CPropertyPage::OnClose();
}

void CXRAMPage::OnButtonRead() 
{
	if(!mReadXRam())
		MessageBox("读XRAM失败！","CH37XDBG",MB_OK|MB_ICONSTOP);
}

UCHAR CXRAMPage::mCharToBcd(UCHAR iChar)
{
	UCHAR	mBCD;
	if ( iChar >= '0' && iChar <= '9' ) mBCD = iChar -'0';
	else if ( iChar >= 'A' && iChar <= 'F' ) mBCD = iChar - 'A' + 0x0a;
	else if ( iChar >= 'a' && iChar <= 'f' ) mBCD = iChar - 'a' + 0x0a;
	else mBCD = 0xff;
	return( mBCD );
}

BEGIN_EVENTSINK_MAP(CXRAMPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CXRAMPage)
	ON_EVENT(CXRAMPage, IDC_XGRID, -601 /* DblClick */, OnDblClickXgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CXRAMPage::OnDblClickXgrid() 
{
	long x,y;
	long addrbase;
	CXRAMPage m_xram;
	UpdateData(TRUE);
	x = m_xgrid.GetRow();
	y = m_xgrid.GetCol();
	//m_igrid.SetTextArray(x*18 + y,"aa");
	CENTER *m_dlg = new CENTER;
	if(strlen(m_addr) > 1)
		addrbase = (m_xram.mCharToBcd(m_addr[0])<<4) | (m_xram.mCharToBcd(m_addr[1]));
	else if(strlen(m_addr) == 1)
		addrbase = m_xram.mCharToBcd(m_addr[0]);
	else
		addrbase = 0;
	m_dlg->m_addr = (addrbase<<8) | (((x-1)&0x0f)<<4) | ((y-1)&0x0f);	//xram中地址
	m_dlg->m_ram = 1;		//xram标志
	m_dlg->DoModal();
	if(!mReadXRam())
		MessageBox("刷新失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

BOOL CXRAMPage::mReadXRam()
{
//	UCHAR ch[3];
	UCHAR adh;
	ULONG  i,j;
	ULONG  addr;
	ULONG ad;
	UCHAR c;
	UCHAR  val[20];
	CHAR  str[10];

	UpdateData(TRUE);
	if(CH375DBG_Opened == TRUE)
	{
		if(m_addr != "")
		{
			adh = mCharToBcd(m_addr[0])<<4 | mCharToBcd(m_addr[1]);
			ad = (ULONG)adh << 8;
		}
		else
			ad = 0;
		for(i=0;i<16;i++)
		{
			addr = (i << 4) + ad;
			if(CH375DBG_ReadXRAM(addr,val,16)) 
			{
				for(j=0;j<16;j++)
				{
					c=(val[j]>>4)&0x0f;
					str[0]=c>=10?c-0x0a+'A':c+'0';
					c=val[j]&0x0f;
					str[1]=c>=10?c-0x0a+'A':c+'0';
					str[2]=0;
					m_xgrid.SetTextArray((18+i*17+j),str);
				}
			}
			else
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	else
		return FALSE;
}

// ENTER.cpp : implementation file
//

#include "stdafx.h"
#include "CH37XDBG.h"
#include "ENTER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CENTER dialog


CENTER::CENTER(CWnd* pParent /*=NULL*/)
	: CDialog(CENTER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CENTER)
	m_data = _T("");
	//}}AFX_DATA_INIT
}


void CENTER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CENTER)
	DDX_Text(pDX, IDC_EDIT_DATA, m_data);
	DDV_MaxChars(pDX, m_data, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CENTER, CDialog)
	//{{AFX_MSG_MAP(CENTER)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CENTER message handlers

void CENTER::OnButtonEnter() 
{
	if(!mWriteData())
		MessageBox("дIRAMʧ��","37XDBG",MB_OK|MB_ICONSTOP);
	this->OnOK();
}

BOOL CENTER::mWriteData()
{
	UCHAR c;
	CXRAMPage m_xram;
	ULONG addr;
	UpdateData(TRUE);

	if(strlen(m_data) > 1 )
		c = m_xram.mCharToBcd(m_data[0])<<4 |m_xram.mCharToBcd(m_data[1]);
	else if(strlen(m_data) == 1)
		c = m_xram.mCharToBcd(m_data[0]);
	else
		c = 0;
	addr = (ULONG)m_addr;
	if(m_ram == 0)		//дIRAM
	{
		if(!CH375DBG_WriteIRAM( addr, &c, 0x01 ))
		{
			return FALSE;
		}
	}
	else if(m_ram == 1)	//дXRAM
	{
		if(!CH375DBG_WriteXRAM( addr, &c, 0x01))
		{
			return FALSE;
		}
	}
	return TRUE;
}

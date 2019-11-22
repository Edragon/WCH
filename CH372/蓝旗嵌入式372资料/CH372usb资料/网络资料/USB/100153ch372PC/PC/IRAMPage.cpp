// IRAMPage.cpp : implementation file
//

#include "stdafx.h"
//#include   "total.h"
#include "CH37XDBG.h"


/////////////////////////////////////////////////////////////////////////////
// CIRAMPage property page
extern ULONG	CH375DBG_Index;			/* CH372/CH375设备序号 */
extern BOOL	CH375DBG_Opened;	/* CH372/CH375设备被打开标志 */
IMPLEMENT_DYNCREATE(CIRAMPage, CPropertyPage)

CIRAMPage::CIRAMPage() : CPropertyPage(CIRAMPage::IDD)
{
	//{{AFX_DATA_INIT(CIRAMPage)
	//}}AFX_DATA_INIT
}

CIRAMPage::~CIRAMPage()
{
}

void CIRAMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRAMPage)
	DDX_Control(pDX, IDC_IGRID, m_igrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIRAMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CIRAMPage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIRAMPage message handlers

void CIRAMPage::OnClose() 
{
	this->m_mdlg->EndDialog(0);	
	CPropertyPage::OnClose();
}

BOOL CIRAMPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	if(!mReadIRam())
	{
		MessageBox("读IRAM错误！","CH37XDBG",MB_OK|MB_ICONSTOP);
		return FALSE;
	}
		return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
BOOL CIRAMPage::CH375DBG_ReadIRAM(ULONG StartAddr, PUCHAR buffer, UCHAR count)// 从内部RAM读取数据块
{
// 输入参数: StartAddr 指定内部RAM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( count > MAX_DATA_SIZE ) return( FALSE );  // 限制单次处理的数据长度,可以与单片机程序的MAX_DATA_SIZE一起同步修改为更大或更小的值
	down.mCommand = USB_CMD_MEM_ACCESS | USB_CMD_MEM_W_BYTE | USB_CMD_MEM_S_IRAM;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = StartAddr;
	down.mLength = count;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) {   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) {  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= count ) {  // 操作成功,并且返回数据
				for ( len = 0; len < (ULONG)count; len ++ ) buffer[ len ] = up.mBuffer[ len ];  // 返回数据
				return( TRUE );
			}
		}
	}
	return( FALSE );
}*/

BEGIN_EVENTSINK_MAP(CIRAMPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CIRAMPage)
	ON_EVENT(CIRAMPage, IDC_IGRID, -601 /* DblClick */, OnDblClickIgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CIRAMPage::OnDblClickIgrid() 
{
	// TODO: Add your control notification handler code here
	long x,y;
	x = m_igrid.GetRow();
	y = m_igrid.GetCol();
	//m_igrid.SetTextArray(x*18 + y,"aa");
	CENTER *m_dlg = new CENTER;
	m_dlg->m_addr = (((x-1)&0x0f)<<4) | ((y-1)&0x0f);	//iram中地址
	m_dlg->m_ram = 0;		//iram标志
	m_dlg->DoModal();
	if(!mReadIRam())
		MessageBox("刷新失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

BOOL CIRAMPage::mReadIRam()
{
	ULONG  i,j;
	ULONG  addr;
	UCHAR c;
	UCHAR  val[20];
	CHAR  str[3];

	if(CH375DBG_Opened == TRUE)
	{
		for(i=0;i<16;i++)
		{
			addr = i<<4;
			if(CH375DBG_ReadIRAM(addr,val,16)) 
			{
				for(j=0;j<16;j++)
				{
					c=(val[j]>>4)&0x0f;
					str[0]=c>=10?c-0x0a+'A':c+'0';
					c=val[j]&0x0f;
					str[1]=c>=10?c-0x0a+'A':c+'0';
					str[2]=0;
					m_igrid.SetTextArray((18+i*17+j),str);
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

void CIRAMPage::OnButtonRead() 
{
	if(!mReadIRam())
	{
		MessageBox("读IRAM错误！","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
}

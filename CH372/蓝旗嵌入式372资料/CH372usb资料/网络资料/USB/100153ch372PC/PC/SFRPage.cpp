// SFRPage.cpp : implementation file
//

#include "stdafx.h"
//#include "total.h"
#include "CH37XDBG.h"
//#include "SFRPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSFRPage property page
extern ULONG	CH375DBG_Index;			/* CH372/CH375设备序号 */
extern BOOL	CH375DBG_Opened;	/* CH372/CH375设备被打开标志 */

IMPLEMENT_DYNCREATE(CSFRPage, CPropertyPage)

CSFRPage::CSFRPage() : CPropertyPage(CSFRPage::IDD)
{
	//{{AFX_DATA_INIT(CSFRPage)
	m_name1 = _T("");
	m_name2 = _T("");
	m_name3 = _T("");
	m_name4 = _T("");
	m_name5 = _T("");
	m_name6 = _T("");
	m_name7 = _T("");
	m_name8 = _T("");
	m_value2 = _T("");
	m_value3 = _T("");
	m_value4 = _T("");
	m_value5 = _T("");
	m_value6 = _T("");
	m_value7 = _T("");
	m_value8 = _T("");
	m_ver = _T("");
	m_name10 = _T("");
	m_name9 = _T("");
	m_name11 = _T("");
	m_name12 = _T("");
	m_name13 = _T("");
	m_name14 = _T("");
	m_name15 = _T("");
	m_name16 = _T("");
	m_value9 = _T("");
	m_value16 = _T("");
	m_value15 = _T("");
	m_value14 = _T("");
	m_value13 = _T("");
	m_value12 = _T("");
	m_value11 = _T("");
	m_value10 = _T("");
	m_value1 = _T("");
	//}}AFX_DATA_INIT
}

CSFRPage::~CSFRPage()
{
}

void CSFRPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSFRPage)
	DDX_Text(pDX, IDC_EDIT_NAME1, m_name1);
	DDX_Text(pDX, IDC_EDIT_NAME2, m_name2);
	DDX_Text(pDX, IDC_EDIT_NAME3, m_name3);
	DDX_Text(pDX, IDC_EDIT_NAME4, m_name4);
	DDX_Text(pDX, IDC_EDIT_NAME5, m_name5);
	DDX_Text(pDX, IDC_EDIT_NAME6, m_name6);
	DDX_Text(pDX, IDC_EDIT_NAME7, m_name7);
	DDX_Text(pDX, IDC_EDIT_NAME8, m_name8);
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_value2);
	DDX_Text(pDX, IDC_EDIT_VALUE3, m_value3);
	DDX_Text(pDX, IDC_EDIT_VALUE4, m_value4);
	DDX_Text(pDX, IDC_EDIT_VALUE5, m_value5);
	DDX_Text(pDX, IDC_EDIT_VALUE6, m_value6);
	DDX_Text(pDX, IDC_EDIT_VALUE7, m_value7);
	DDX_Text(pDX, IDC_EDIT_VALUE8, m_value8);
	DDX_Text(pDX, IDC_STATIC_VER, m_ver);
	DDX_Text(pDX, IDC_EDIT_NAME10, m_name10);
	DDX_Text(pDX, IDC_EDIT_NAME9, m_name9);
	DDX_Text(pDX, IDC_EDIT_NAME11, m_name11);
	DDX_Text(pDX, IDC_EDIT_NAME12, m_name12);
	DDX_Text(pDX, IDC_EDIT_NAME13, m_name13);
	DDX_Text(pDX, IDC_EDIT_NAME14, m_name14);
	DDX_Text(pDX, IDC_EDIT_NAME15, m_name15);
	DDX_Text(pDX, IDC_EDIT_NAME16, m_name16);
	DDX_Text(pDX, IDC_EDIT_VALUE9, m_value9);
	DDX_Text(pDX, IDC_EDIT_VALUE16, m_value16);
	DDX_Text(pDX, IDC_EDIT_VALUE15, m_value15);
	DDX_Text(pDX, IDC_EDIT_VALUE14, m_value14);
	DDX_Text(pDX, IDC_EDIT_VALUE13, m_value13);
	DDX_Text(pDX, IDC_EDIT_VALUE12, m_value12);
	DDX_Text(pDX, IDC_EDIT_VALUE11, m_value11);
	DDX_Text(pDX, IDC_EDIT_VALUE10, m_value10);
	DDX_Text(pDX, IDC_EDIT_VALUE1, m_value1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSFRPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSFRPage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE1, OnKillfocusEditValue1)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE2, OnKillfocusEditValue2)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE3, OnKillfocusEditValue3)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE4, OnKillfocusEditValue4)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE5, OnKillfocusEditValue5)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE6, OnKillfocusEditValue6)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE7, OnKillfocusEditValue7)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE8, OnKillfocusEditValue8)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE9, OnKillfocusEditValue9)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE10, OnKillfocusEditValue10)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE11, OnKillfocusEditValue11)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE12, OnKillfocusEditValue12)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE13, OnKillfocusEditValue13)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE14, OnKillfocusEditValue14)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE15, OnKillfocusEditValue15)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE16, OnKillfocusEditValue16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSFRPage message handlers

BOOL CSFRPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_name1 = _T("P0");
	m_name2 = _T("PCON");
	m_name3 = _T("TCON");
	m_name4 = _T("TMOD");
	m_name5 = _T("TL0");
	m_name6 = _T("TL1");
	m_name7 = _T("TH0");
	m_name8 = _T("TH1");
	m_name9 = _T("P1");
	m_name10 = _T("SCON");
	m_name11 = _T("SBUF");
	m_name12 = _T("P2");
	m_name13 = _T("IE");
	m_name14 = _T("P3");
	m_name15 = _T("IP");
	m_name16 = _T("T2CON");
	m_ver = _T("??");
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSFRPage::OnClose() 
{
	m_mdlg->EndDialog(0);	
	CPropertyPage::OnClose();
}
/*
BOOL CSFRPage::CH375DBG_GetFirmwareInfo(PUCHAR ver)// 获取调试固件程序的版本,并取消未完成的上传数据块
{
// 输入参数: ver 指向一个字节变量单元,用于存放获取的版本号
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
	ULONG			len;
	if ( CH375DBG_Opened == FALSE ) {  // CH372/CH375设备尚未被打开
		CH375DBG_Opened = CH375OpenDevice( CH375DBG_Index ) != INVALID_HANDLE_VALUE;  // 打开CH375设备
		if ( CH375DBG_Opened == FALSE ) return( FALSE );
	}
	down.mCommand = USB_CMD_GET_FW_INFO;
	down.mCommandNot = ~ down.mCommand;
	down.u.mDword = 0;
	down.mLength = 0;
	len = mOFFSET( USB_DOWN_PKT, mBuffer );
	if ( CH375WriteData( CH375DBG_Index, &down, &len ) ) {   // 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) {  // 读取应答块
			if ( up.mStatus == ERR_SUCCESS && up.mCommandNot == down.mCommandNot && up.mLength >= 1 ) {  // 操作成功,并且返回数据
				*ver = up.mBuffer[0];  // 返回版本号
				return( TRUE );
			}
		}
	}
	return( FALSE );
}
*/
//读SFR各地址
void CSFRPage::OnButtonRefresh() 
{
	UCHAR ver;
	UCHAR val1,val2,val3,val4,val5,val6,val7,val8,val9,val10,val11,val12,val13,val14,val15,val16;
	if(CH375DBG_GetFirmwareInfo(&ver))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",ver);
		m_ver = _T(str);
	}
	else
	{
		MessageBox("无法打开CH372/CH375,可能未工作或者单片机未执行调试程序","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x80, &val1 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val1);
		m_value1 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址P0失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x87, &val2 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val2);
		m_value2 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址PCON失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x88, &val3 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val3);
		m_value3 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TCON失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x89, &val4 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val4);
		m_value4 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TMOD失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x8A, &val5 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val5);
		m_value5 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TL0失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x8B, &val6 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val6);
		m_value6 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TL1失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x8C, &val7 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val7);
		m_value7 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TH0失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x8D, &val8 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val8);
		m_value8 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址TH1失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x90, &val9 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val9);
		m_value9 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址P1失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x98, &val10 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val10);
		m_value10 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址SCON失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0x99, &val11 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val11);
		m_value11 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址SBUF失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0xA0, &val12 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val12);
		m_value12 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址P2失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0xA8, &val13 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val13);
		m_value13 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址IE失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0xB0, &val14 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val14);
		m_value14 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址P3失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0xB8, &val15 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val15);
		m_value15 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址IP失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	if(CH375DBG_ReadSFR( 0xC8, &val16 ))
	{
		UCHAR str[3];
		sprintf((char *)str,"%02x",val16);
		m_value16 = _T(str);
	}
	else
	{
		MessageBox("SFR读地址T2CON失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}

	UpdateData(FALSE);	
}
//写入SFR
BOOL CSFRPage::mWriteSFR(ULONG add, CString value)
{
	UCHAR old[3];
	UCHAR c;
	CXRAMPage m_xram;
	strcpy((char *)old,value);
	UpdateData(TRUE);
	if(value == "")
		return TRUE;
	c = (m_xram.mCharToBcd(value[0])<<4) | m_xram.mCharToBcd(value[1]);
//	if(memcmp(old,value,2) != 0)
	{
		if(CH375DBG_WriteSFR(add,c))
			return TRUE;
		else
			return FALSE;
	}
//	else
//		return TRUE;
}

void CSFRPage::OnKillfocusEditValue1() 
{
	CString old;
	old = m_value1;
	UpdateData(TRUE);
	if(old != m_value1)
		if(!mWriteSFR(0x80,m_value1))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
	
}

void CSFRPage::OnKillfocusEditValue2() 
{
	CString old;
	old = m_value2;
	UpdateData(TRUE);
	if(old != m_value2)
		if(!mWriteSFR(0x87,m_value2))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
	
}

void CSFRPage::OnKillfocusEditValue3() 
{
	CString old;
	old = m_value3;
	UpdateData(TRUE);
	if(old != m_value3)
		if(!mWriteSFR(0x88,m_value3))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
	
}

void CSFRPage::OnKillfocusEditValue4() 
{
	CString old;
	old = m_value4;
	UpdateData(TRUE);
	if(old != m_value4)
		if(!mWriteSFR(0x89,m_value4))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
	
}

void CSFRPage::OnKillfocusEditValue5() 
{
	CString old;
	old = m_value5;
	UpdateData(TRUE);
	if(old != m_value5)
		if(!mWriteSFR(0x8a,m_value5))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
	
}

void CSFRPage::OnKillfocusEditValue6() 
{
	CString old;
	old = m_value6;
	UpdateData(TRUE);
	if(old != m_value6)
		if(!mWriteSFR(0x8b,m_value6))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue7() 
{
	CString old;
	old = m_value7;
	UpdateData(TRUE);
	if(old != m_value7)
		if(!mWriteSFR(0x8c,m_value7))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue8() 
{
	CString old;
	old = m_value8;
	UpdateData(TRUE);
	if(old != m_value8)
		if(!mWriteSFR(0x8d,m_value8))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue9() 
{
	CString old;
	old = m_value9;
	UpdateData(TRUE);
	if(old != m_value9)
		if(!mWriteSFR(0x90,m_value9))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue10() 
{
	CString old;
	old = m_value10;
	UpdateData(TRUE);
	if(old != m_value10)
		if(!mWriteSFR(0x98,m_value10))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue11() 
{
	CString old;
	old = m_value11;
	UpdateData(TRUE);
	if(old != m_value11)
		if(!mWriteSFR(0x99,m_value11))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue12() 
{
	CString old;
	old = m_value12;
	UpdateData(TRUE);
	if(old != m_value12)
		if(!mWriteSFR(0xa0,m_value12))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue13() 
{
	CString old;
	old = m_value13;
	UpdateData(TRUE);
	if(old != m_value13)
		if(!mWriteSFR(0xa8,m_value13))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue14() 
{
	CString old;
	old = m_value14;
	UpdateData(TRUE);
	if(old != m_value14)
		if(!mWriteSFR(0xb0,m_value14))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue15() 
{
	CString old;
	old = m_value15;
	UpdateData(TRUE);
	if(old != m_value15)
		if(!mWriteSFR(0xb8,m_value15))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

void CSFRPage::OnKillfocusEditValue16() 
{
	CString old;
	old = m_value16;
	UpdateData(TRUE);
	if(old != m_value16)
		if(!mWriteSFR(0xc8,m_value16))
			MessageBox("写失败","CH37XDBG",MB_OK|MB_ICONSTOP);
}

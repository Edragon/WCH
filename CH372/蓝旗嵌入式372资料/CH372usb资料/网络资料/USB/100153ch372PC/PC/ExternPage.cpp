// ExternPage.cpp : implementation file
//

#include "stdafx.h"
//#include "total.h"
#include "CH37XDBG.h"
//#include "AllControlsSheet.h"
//#include "ExternPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExternPage property page
extern ULONG	CH375DBG_Index;			/* CH372/CH375设备序号 */
extern BOOL	CH375DBG_Opened;	/* CH372/CH375设备被打开标志 */
USB_UP_PKT up;
USB_DOWN_PKT down;
IMPLEMENT_DYNCREATE(CExternPage, CPropertyPage)

CExternPage::CExternPage() : CPropertyPage(CExternPage::IDD)
{
	//{{AFX_DATA_INIT(CExternPage)
	m_len = _T("");
	m_comm = _T("");
	m_para1 = _T("");
	m_para2 = _T("");
	m_para3 = _T("");
	m_para4 = _T("");
	m_buf = _T("");
	//}}AFX_DATA_INIT
}

CExternPage::~CExternPage()
{
}

void CExternPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExternPage)
	DDX_Text(pDX, IDC_EDIT_LEN, m_len);
	DDV_MaxChars(pDX, m_len, 2);
	DDX_Text(pDX, IDC_EDIT_COMM, m_comm);
	DDV_MaxChars(pDX, m_comm, 2);
	DDX_Text(pDX, IDC_EDIT_PARA1, m_para1);
	DDV_MaxChars(pDX, m_para1, 2);
	DDX_Text(pDX, IDC_EDIT_PARA2, m_para2);
	DDV_MaxChars(pDX, m_para2, 2);
	DDX_Text(pDX, IDC_EDIT_PARA3, m_para3);
	DDV_MaxChars(pDX, m_para3, 2);
	DDX_Text(pDX, IDC_EDIT_PARA4, m_para4);
	DDV_MaxChars(pDX, m_para4, 2);
	DDX_Text(pDX, IDC_EDIT_BUF, m_buf);
	DDV_MaxChars(pDX, m_buf, 121);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExternPage, CPropertyPage)
	//{{AFX_MSG_MAP(CExternPage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVE, OnButtonReceive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternPage message handlers

void CExternPage::OnClose() 
{
	this->m_mdlg->EndDialog(0);	
	CPropertyPage::OnClose();
}

void CExternPage::OnButtonSend() 
{
	CXRAMPage m_xram;
//	USB_DOWN_PKT	down;
//	USB_UP_PKT		up;
	UCHAR buffer[120];
	UCHAR length;
//	int buflen;
	ULONG len;
	UpdateData(TRUE);
	if(CH375DBG_Opened == FALSE)
	{
		MessageBox("设备未打开","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	//命令码
	if( strlen(m_comm) > 1 )
		down.mCommand = (m_xram.mCharToBcd(m_comm[0])<<4) | (m_xram.mCharToBcd(m_comm[1]));
	else if( strlen(m_comm) == 1 )
		down.mCommand = m_xram.mCharToBcd(m_comm[0]);
	else
		down.mCommand = 0;
	//反命令码
	down.mCommandNot = ~down.mCommand;
	//参数1
	if(strlen(m_para1) > 1 )
		down.u.mByte[0] = (m_xram.mCharToBcd(m_para1[0])<<4) | (m_xram.mCharToBcd(m_para1[1]));
	else if( strlen(m_para1) == 1 )
		down.u.mByte[0] = m_xram.mCharToBcd(m_para1[0]);
	else
		down.u.mByte[0] = 0;
	//参数2
	if(strlen(m_para2) > 1 )
		down.u.mByte[1] = (m_xram.mCharToBcd(m_para2[0])<<4) | (m_xram.mCharToBcd(m_para2[1]));
	else if( strlen(m_para2) == 1 )
		down.u.mByte[1] = m_xram.mCharToBcd(m_para2[0]);
	else
		down.u.mByte[1] = 0;
	//参数3
	if(strlen(m_para3) > 1 )
		down.u.mByte[2] = (m_xram.mCharToBcd(m_para3[0])<<4) | (m_xram.mCharToBcd(m_para3[1]));
	else if( strlen(m_para3) == 1 )
		down.u.mByte[3] = m_xram.mCharToBcd(m_para3[0]);
	else
		down.u.mByte[3] = 0;
	//参数4
	if(strlen(m_para4) > 1 )
		down.u.mByte[4] = (m_xram.mCharToBcd(m_para4[0])<<4) | (m_xram.mCharToBcd(m_para4[1]));
	else if( strlen(m_para4) == 1 )
		down.u.mByte[4] = m_xram.mCharToBcd(m_para4[0]);
	else
		down.u.mByte[4] = 0;
	//长度
	if(strlen(m_len) > 1 )
		length = (m_xram.mCharToBcd(m_len[0])<<4) | (m_xram.mCharToBcd(m_len[1]));
	else if( strlen(m_len) == 1 )
		length = m_xram.mCharToBcd(m_len[0]);	
	else
		length = 0;
	
//	buflen = (m_buf.GetLength()/2) & 0xff;
//	down.mLength = ((UCHAR)buflen) < length?((UCHAR)buflen):length;		//长度取小值
	down.mLength = length;
	if(m_buf != "")
	{
		memcpy(buffer,m_buf,((UINT)down.mLength) * 2);
		if( !mStrToBcd(&buffer[0],(ULONG)down.mLength) )
			return;
		for(len=0; len<down.mLength;len++)		//发送数据
			down.mBuffer[len] = buffer[len];
	}
	len = mOFFSET( USB_DOWN_PKT, mBuffer ) + down.mLength;
	if (!CH375WriteData( CH375DBG_Index, &down, &len ) ) 
	{   
		MessageBox("发送失败","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
	/**********读取************/
	{
		
	}
}

BOOL CExternPage::mStrToBcd(PUCHAR buf, ULONG len)
{
	UCHAR buff[MAX_DATA_SIZE];
	ULONG i,j;
	CXRAMPage m_xram;
	if((len == 0) || (buf == NULL))
		return FALSE;
//	if(len % 2 != 0)
//		len += 1;
	for(i=0,j=0;i<len;i++)
	{
		buff[i] = (m_xram.mCharToBcd(buf[j])<<4) | (m_xram.mCharToBcd(buf[j+1]));
		j += 2;
	}
	memset(buf,0x00,sizeof(buf));
	memcpy(buf,buff,len*2);
	return TRUE;
}

BOOL CExternPage::mBcdToStr(PUCHAR buf, ULONG len)
{
	UCHAR buff[MAX_DATA_SIZE * 2];
	ULONG i,j;
	UCHAR c1,c2,c3;
	if(len == 0 || buf == NULL)
		return FALSE;
	for(i=0,j=0;i<len;i++)
	{
		c1 = (buf[i]>>4) & 0x0f;
		c2 = c1>=10?c1-0x0a+'A':c1+'0';
		c1 = buf[i] & 0x0f;
		c3 = c1>=10?c1-0x0a+'A':c1+'0';
		buff[j] = c2;
		buff[j+1] = c3;
		j += 2;
	}
	buff[j] = '\0';
	memset(buf,0x00,sizeof(buf));
	strcpy((char *)&buf[0],(char *)&buff[0]);
	return TRUE;
}

void CExternPage::OnButtonReceive() 
{
	CXRAMPage m_xram;
	UCHAR buffer[MAX_DATA_SIZE *2];
	ULONG			len;
	if(CH375DBG_Opened == FALSE)
	{
		MessageBox("设备未打开","CH37XDBG",MB_OK|MB_ICONSTOP);
		return;
	}
/*	if(strlen(m_len) > 1)
		length = (m_xram.mCharToBcd(m_len[0])<<4) | (m_xram.mCharToBcd(m_len[1]));
	else if(strlen(m_len) == 1)
		length = m_xram.mCharToBcd(m_len[0]);
	else
		length = 0;
	if(length > MAX_DATA_SIZE)
	{
		MessageBox("长度超出范围！请重新输入","CH37DXDBG",MB_OK|MB_ICONINFORMATION);
		return;
	}
	up.mLength = length;

*/	// 写出命令块
		len = sizeof( USB_UP_PKT );
		if ( CH375ReadData( CH375DBG_Index, &up, &len ) ) 
		{  // 读取应答块
			if ( up.mCommandNot == down.mCommandNot ) 
			{  // 操作成功
				UCHAR c;
				UCHAR str[3];
				//状态码
				c = (up.mStatus >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mStatus & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_comm = _T(str);
				//返回码1
				c = (up.mReserved[0] >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mReserved[0] & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_para1 = _T(str);
				//返回码2
				c = (up.mReserved[1] >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mReserved[1] & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_para2 = _T(str);
				//返回码3
				c = (up.mReserved[2] >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mReserved[2] & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_para3 = _T(str);
				//返回码4
				c = (up.mReserved[3] >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mReserved[3] & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_para4 = _T(str);
				//返回长度
				c = (up.mLength >> 4) & 0x0f;
				str[0] = c >= 10?c-0x0a+'A':c+'0';
				c = up.mLength & 0x0f;
				str[1] = c >= 10?c-0x0a+'A':c+'0';
				str[2] = 0;
				m_len = _T(str);

				for ( len = 0; len < (ULONG)up.mLength; len ++ ) 
					buffer[ len ] = up.mBuffer[ len ];  // 返回数据
				mBcdToStr(buffer,up.mLength);
				buffer[len*2] =0;
				m_buf = _T(buffer);
				UpdateData(FALSE);
	
			}
			else
			{
				MessageBox("返回状态错误！","CH37XDBG",MB_OK|MB_ICONSTOP);
				return;
			}
		}
		else
		{
			MessageBox("读失败！","CH37XDBG",MB_OK|MB_ICONSTOP);
		}

	
}

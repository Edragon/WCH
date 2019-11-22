// testUSBDlg.h : header file
//

#if !defined(AFX_TESTUSBDLG_H__EA27F639_9C88_427F_84D1_12C0BE44C6FD__INCLUDED_)
#define AFX_TESTUSBDLG_H__EA27F639_9C88_427F_84D1_12C0BE44C6FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CTestUSBDlg dialog

class CTestUSBDlg : public CDialog
{
// Construction
public:
	void ShowUdisk();
	char chFirstDriveFromMask(ULONG unitmask);
	void ShowMore(HDEVINFO hDevInfo,SP_DEVICE_INTERFACE_DATA strtInterfaceData);
	void ShowHidInfo();
	PCHAR WideStrToMultiStr (PWCHAR WideStr);
	void ShowCenctInfo(PUSB_NODE_CONNECTION_INFORMATION connectionInfo);
	PUSB_DESCRIPTOR_REQUEST GetConfigDescriptor(HANDLE  hHubDevice,ULONG   ConnectionIndex,UCHAR   DescriptorIndex);
	PCHAR GetDriverKeyName(HANDLE  Hub,ULONG   ConnectionIndex);
	PCHAR GetRootHubName(HANDLE HostController);
	PCHAR GetHCDDriverKeyName (HANDLE  HCD);
	void NumUsbAll();
	void ShowMessage(CString str);
	CTestUSBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestUSBDlg)
	enum { IDD = IDD_TESTUSB_DIALOG };
	CString	m_showMessage;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestUSBDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestUSBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowusball();
	afx_msg void OnShowusbhid();
	afx_msg void OnShowusbdisk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTUSBDLG_H__EA27F639_9C88_427F_84D1_12C0BE44C6FD__INCLUDED_)

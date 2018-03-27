// UsbDlg.h : header file
//

#if !defined(AFX_USBDLG_H__56BE126C_BCC7_4017_B7D4_4540CF0C3948__INCLUDED_)
#define AFX_USBDLG_H__56BE126C_BCC7_4017_B7D4_4540CF0C3948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUsbDlg dialog

class CUsbDlg : public CDialog
{
// Construction
public:
	CUsbDlg(CWnd* pParent = NULL);	// standard constructor
 
	GetInputData(INT nID, CString ErrorMsg, DWORD &Value);
	void GetMyIDs(void);
    CString itos(INT value, INT radix=10);
	void AddToInfOut(char *p, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
	void AddToInfOut(CString InStr, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
    
// Dialog Data
	//{{AFX_DATA(CUsbDlg)
	enum { IDD = IDD_USB_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUsbDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnRemove();
	afx_msg void OnBUTTONOpen();
	afx_msg BOOL OnBUTTONSend();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBUTTONClose();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBDLG_H__56BE126C_BCC7_4017_B7D4_4540CF0C3948__INCLUDED_)

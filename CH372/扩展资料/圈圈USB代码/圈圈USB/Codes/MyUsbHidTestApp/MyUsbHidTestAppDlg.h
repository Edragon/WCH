// MyUsbHidTestAppDlg.h : header file
//

#if !defined(AFX_MYUSBHIDTESTAPPDLG_H__370CF2E6_14B7_4D4A_885A_6A7CC1B0961A__INCLUDED_)
#define AFX_MYUSBHIDTESTAPPDLG_H__370CF2E6_14B7_4D4A_885A_6A7CC1B0961A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyUsbHidTestAppDlg dialog

class CMyUsbHidTestAppDlg : public CDialog
{
// Construction
public:
	void SetLedStatus(INT nID, INT nLed);
	void SetCounterNumber(DWORD Counter);
	BOOL GetInputData(INT nID, CString ErrorMsg, DWORD &Value);
	CString itos(INT value, INT radix=10);
	void GetMyIDs(void);
	void SetKeyStatus(void);
	BOOL SendLedStatus(void);
	void AddToInfOut(char *p, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
	void AddToInfOut(CString InStr, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
	CMyUsbHidTestAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyUsbHidTestAppDlg)
	enum { IDD = IDD_MYUSBHIDTESTAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyUsbHidTestAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyUsbHidTestAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClearInf();
	afx_msg void OnLed1();
	afx_msg void OnLed2();
	afx_msg void OnLed3();
	afx_msg void OnLed4();
	afx_msg void OnLed5();
	afx_msg void OnLed6();
	afx_msg void OnLed7();
	afx_msg void OnLed8();
	afx_msg void OnOpenDevice();
	afx_msg void OnCloseDevice();
	afx_msg void OnQuit();
	afx_msg void OnClose();
	afx_msg void OnClearCounter();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnDeviceChange(WPARAM nEventType, LPARAM dwData);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYUSBHIDTESTAPPDLG_H__370CF2E6_14B7_4D4A_885A_6A7CC1B0961A__INCLUDED_)

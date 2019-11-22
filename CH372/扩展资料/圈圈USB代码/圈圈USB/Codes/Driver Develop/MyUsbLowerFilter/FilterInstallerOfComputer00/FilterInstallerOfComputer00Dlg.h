// FilterInstallerOfComputer00Dlg.h : header file
//

#if !defined(AFX_FILTERINSTALLEROFCOMPUTER00DLG_H__B5237848_009C_4302_88F9_592721A10A16__INCLUDED_)
#define AFX_FILTERINSTALLEROFCOMPUTER00DLG_H__B5237848_009C_4302_88F9_592721A10A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C" {
#include <setupapi.h>
}

/////////////////////////////////////////////////////////////////////////////
// CFilterInstallerOfComputer00Dlg dialog

class CFilterInstallerOfComputer00Dlg : public CDialog
{
// Construction
public:

	BOOL RemoveDeviceDriver(GUID *pGuid, LPCTSTR HardwareID,DWORD Flags);
	DWORD InstallFilter(void);
	DWORD UninstallFilter(void);
	DWORD AddService(LPCTSTR pszFilterName, LPCTSTR pszServiceBinary);
 DWORD CFilterInstallerOfComputer00Dlg::RemoveService(LPCTSTR pszFilterName);
 
	DWORD ChangeDeviceState(DWORD StateChange, 
		                       DWORD Scope, 
																									HDEVINFO hDevInfoSet,
																									PSP_DEVINFO_DATA pDevInfo);
 DWORD StartDevice(HDEVINFO hDevInfoSet, 
																			PSP_DEVINFO_DATA pDevInfo);
	DWORD StopDevice(HDEVINFO hDevInfoSet, 
																			PSP_DEVINFO_DATA pDevInfo);
	DWORD RestartDevice(HDEVINFO hDevInfoSet,
																			PSP_DEVINFO_DATA pDevInfo);

 void AddToInfOut(CString InStr, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
 void AddToInfOut(char *p, BOOL AddTime=TRUE, BOOL NewLine=TRUE);
 void ClrInfOut(void);

	DWORD m_NeedSteps;
 DWORD m_CurrentSteps;

	void DisableButtons(void);
	void EnableButtons(void);

	CFilterInstallerOfComputer00Dlg(CWnd* pParent = NULL);	// standard constructor
 
//	CDeviceWinVersion m_WinVer;
// Dialog Data
	//{{AFX_DATA(CFilterInstallerOfComputer00Dlg)
	enum { IDD = IDD_FILTERINSTALLEROFCOMPUTER00_DIALOG };
	CProgressCtrl	m_Percent;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterInstallerOfComputer00Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFilterInstallerOfComputer00Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInstall();
	afx_msg void OnUninstall();
	afx_msg void OnAbout();
	afx_msg void OnQuit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERINSTALLEROFCOMPUTER00DLG_H__B5237848_009C_4302_88F9_592721A10A16__INCLUDED_)

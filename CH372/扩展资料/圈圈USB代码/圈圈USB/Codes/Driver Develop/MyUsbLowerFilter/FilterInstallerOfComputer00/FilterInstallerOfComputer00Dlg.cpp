/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

���ߣ�����ȦȦ
��������: 2008.08.28
�޸�����: 2008.08.30
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/
// FilterInstallerOfComputer00Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <winsvc.h>
#include "FilterInstallerOfComputer00.h"
#include "FilterInstallerOfComputer00Dlg.h"

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
// CFilterInstallerOfComputer00Dlg dialog

CFilterInstallerOfComputer00Dlg::CFilterInstallerOfComputer00Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterInstallerOfComputer00Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterInstallerOfComputer00Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFilterInstallerOfComputer00Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterInstallerOfComputer00Dlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Percent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterInstallerOfComputer00Dlg, CDialog)
	//{{AFX_MSG_MAP(CFilterInstallerOfComputer00Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_UNINSTALL, OnUninstall)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterInstallerOfComputer00Dlg message handlers

BOOL CFilterInstallerOfComputer00Dlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFilterInstallerOfComputer00Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFilterInstallerOfComputer00Dlg::OnPaint() 
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
HCURSOR CFilterInstallerOfComputer00Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//����Ϣ��ӵ���Ϣ��ʾ��ĺ���
void CFilterInstallerOfComputer00Dlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
{
	CString str,str1;
	UINT i;
	CHAR SysTime[10];
 
	//�Ȼ�ȡԭ����ʾ���ı�
	GetDlgItemText(IDC_INF_OUT,str);
	//���µ���Ϣ׷�������
	str+=InStr;

	//��Ҫ���ʱ����Ϣ
	if(AddTime==TRUE)
	{
		_strtime(SysTime);
		str1=SysTime;
		str1=" ("+str1+")";
		str+=str1;
	}

	//��Ҫ����
	if(NewLine==TRUE)
	{
		str+="\r\n";
	}

	//�����µ��ı�
	SetDlgItemText(IDC_INF_OUT,str);
	//�������Զ����������һ��
	i=((CEdit*)GetDlgItem(IDC_INF_OUT))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_INF_OUT))->LineScroll(i,0);

	//���ý�����
	if(AddTime==TRUE)
	{
		m_CurrentSteps++;
		m_Percent.SetPos(m_CurrentSteps*100/m_NeedSteps);
	}
}
//////////////////////////////End of function//////////////////////

//����������������ڲ�����һ����
void CFilterInstallerOfComputer00Dlg::AddToInfOut(char *p, BOOL AddTime, BOOL NewLine)
{
	CString str=p;
	AddToInfOut(str,AddTime,NewLine);
}
//////////////////////////////End of function//////////////////////

//�����Ϣ��ʾ��
void CFilterInstallerOfComputer00Dlg::ClrInfOut(void)
{
	SetDlgItemText(IDC_INF_OUT,"");
}
//////////////////////////////End of function//////////////////////

///////////////////////////////////////////////////
//�ı��豸״̬�ĺ���
DWORD CFilterInstallerOfComputer00Dlg::ChangeDeviceState(
																													DWORD StateChange, 
																													DWORD Scope, 
																													HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD dwError = ERROR_SUCCESS;
 SP_PROPCHANGE_PARAMS PropChangeParams;

 memset(&PropChangeParams, 0, sizeof(SP_PROPCHANGE_PARAMS));

 PropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
 PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
 PropChangeParams.StateChange = StateChange;
 PropChangeParams.Scope       = Scope;
 PropChangeParams.HwProfile   = 0;

 if(!SetupDiSetClassInstallParams(
			     hDevInfoSet,
		     	pDevInfo,
		     	(PSP_CLASSINSTALL_HEADER) &PropChangeParams,
		     	sizeof(SP_PROPCHANGE_PARAMS))) 
	{
		dwError = GetLastError();
 }
	else
	{
		if(!SetupDiCallClassInstaller(
				DIF_PROPERTYCHANGE,
				hDevInfoSet,
				pDevInfo))
		{
			dwError = GetLastError();
		}
	}
	return dwError;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////////////
//�����豸�ĺ���
DWORD CFilterInstallerOfComputer00Dlg::StartDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD Status;
	//�����豸Ϊ����״̬
	Status = ChangeDeviceState(DICS_START,
		                          DICS_FLAG_CONFIGSPECIFIC,
																												hDevInfoSet,
																												pDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////////
//ֹͣ�豸�ĺ���
DWORD CFilterInstallerOfComputer00Dlg::StopDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD Status;
	//�����豸Ϊֹͣ״̬
	Status = ChangeDeviceState(DICS_STOP,
		                          DICS_FLAG_CONFIGSPECIFIC,
																												hDevInfoSet,
																												pDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

//////////////////////////////////////////////////////
//���������豸������ֹͣ�豸��Ȼ������������
DWORD CFilterInstallerOfComputer00Dlg::RestartDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD dwError;

	AddToInfOut("�������������豸...");

 dwError = StopDevice(hDevInfoSet,pDevInfo);

	if (ERROR_SUCCESS == dwError)
	{
		dwError = StartDevice(hDevInfoSet,pDevInfo);
	}

	return dwError;
}
////////////////////////End of Function///////////////

////////////////////////////////////
//�����Ƿ����ָ�����ַ�������
LPTSTR SearchMultiSz(LPTSTR Value,ULONG ValueLength,LPCTSTR SearchString)
{
 if(ValueLength < _tcslen(SearchString)*sizeof(TCHAR))
  return NULL;

 SIZE_T Length = ValueLength - _tcslen(SearchString)*sizeof(TCHAR);
 while(1)
 {
  if (!_tcsicmp(&Value[Length/sizeof(TCHAR)], SearchString))
   return &Value[Length];
		if(Length==0)return NULL;
  --Length;
 }
}
////////////////////////End of Function///////////////

/////////////////////////////////////////////////////////////////////
//ж��ָ���豸��������������������ı����豸�����ͣ�
//��ôҪ��ж�ص������˵���������ĸ�һ����豸������
//�������������װ���֮�󲢲��ᵼ�����豸�ĳ��֣�
//��������豸�����ᵼ��ϵͳ������
//��ڲ���pGuid: Ϊɾ���豸��GUID��
//��ڲ���HardwareID��Ϊɾ���豸��Ӳ��ID��
//��ڲ���Flags��ָ��������Щ�豸�Լ�ʲô���͵�GUID��
//               ����ѡ��ΪDIGCF_PRESENT��DIGCF_ALLCLASSES��
//               �Լ�DIGCF_DEVICEINTERFACE����ϡ�
//               DIGCF_PRESENT��ʾֻ���������ӵ��豸��
//               DIGCF_ALLCLASSES��ʾȫ���豸��
//               DIGCF_DEVICEINTERFACEָ��GUIDΪ�ӿ���GUID��
//               ����Ϊ��װ��GUID��
BOOL CFilterInstallerOfComputer00Dlg::RemoveDeviceDriver(GUID *pGuid, LPCTSTR HardwareID,DWORD Flags)
{
 UINT i;
 BOOL Result=FALSE;
 HDEVINFO hDevInfo;
 SP_DEVINFO_DATA DeviceInfoData;
 TCHAR DeviceHardwareID[MAX_PATH]; //����Ӳ��ID
 TCHAR DevDesc[MAX_PATH];  //�����豸������
 DWORD Status;
	DWORD nSize;

 DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);
	
	//��ȡָ��GUID���豸����Ϣ���
	hDevInfo=SetupDiGetClassDevs(pGuid, NULL, NULL,Flags);

	//������Ϣ���������е��豸����Ӳ��ID�Ƿ����
 i=0;
 while(SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData))
 {
		//��ȡ�豸��Ӳ��ID
	 Status=SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_HARDWAREID,
            NULL,
            (PBYTE)DeviceHardwareID,
            sizeof(DeviceHardwareID),
            &nSize);
  if(Status) //�����ȡ�ɹ�
  {
   //�ж�Ӳ��ID�Ƿ�ƥ��
   if(_tcsicmp((LPCTSTR)(PBYTE) DeviceHardwareID, HardwareID)==0)
   {
	  	//��ȡ�豸������
	   Status=SetupDiGetDeviceRegistryProperty(
               hDevInfo,
               &DeviceInfoData,
               SPDRP_DEVICEDESC,
               NULL,
               (PBYTE)DevDesc,
               sizeof(DevDesc),
               &nSize);
				//ɾ���豸
    if(SetupDiCallClassInstaller(DIF_REMOVE,hDevInfo,&DeviceInfoData))
    {
     Result=TRUE;
					AddToInfOut("Ӳ��IDΪ\r\n��",FALSE,FALSE);
					AddToInfOut(DeviceHardwareID,FALSE,FALSE);
					AddToInfOut("����\r\n��",FALSE,FALSE);
					AddToInfOut(DevDesc,FALSE,FALSE);
					AddToInfOut("���豸ж�سɹ�.");
    }
   }
  }
  i++; //Next device.
 }
 return Result;
}
////////////////////////End of Function///////////////

//������̵İ�װGUID��������ж�ؼ����豸
GUID KeyboardClassGuid=
{
	0x4D36E96B,0xE325,0x11CE,
	{0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}
};

//����Ҫɾ���ļ����豸��Ӳ��ID
LPCTSTR MyKeyboardHardwareID =
	_T("HID\\Vid_8888&Pid_0002&Rev_0100");
 
//����HID�豸�İ�װ��GUID�����ӹ���������ж�ؼ���HID�豸ʱ��
GUID HidClassGuid=
{
	0x745A17A0,0x74D3,0x11D0,
	{0xB6,0xFE,0x00,0xA0,0xC9,0x0F,0x57,0xDA}
};

//������Ҫ��װ�����������豸��Ӳ��ID
LPCTSTR FilterHardwareID = 
	_T("USB\\Vid_8888&Pid_0002&Rev_0100");
//����Ҫж�ع�������ʱ����Ҫ��ж��HID�����豸��
//���Ӳ��ID��������ж��HID�����豸�õ�
LPCTSTR MyHidHardwareID =
	_T("HID\\Vid_8888&Pid_0002&Rev_0100");

/////////////////////////////////////////////////
//��ӷ���ĺ��������������������
DWORD CFilterInstallerOfComputer00Dlg::AddService(LPCTSTR pszFilterName, 
																																	LPCTSTR pszServiceBinary)
{
 DWORD status = ERROR_SUCCESS;
 SC_HANDLE schSCManager;
 SC_HANDLE schService;

	schSCManager=OpenSCManager(NULL, //�������
                            NULL, //SCM���ݿ���
                            SC_MANAGER_ALL_ACCESS); //���ʷ�ʽ
 if (schSCManager!=NULL)
 {
  schService=CreateService(schSCManager, //SCManager���ݿ�
                           pszFilterName, //���������
                           pszFilterName, //��ʾ������
                           SERVICE_ALL_ACCESS,  //����Ȩ��
                           SERVICE_KERNEL_DRIVER,//��������
                           SERVICE_DEMAND_START, //������ʽ
                           SERVICE_ERROR_NORMAL, //�����������
                           pszServiceBinary, //�ṩ������ļ�·��
                           NULL,   //�޼���˳�����
                           NULL,   //�ޱ�ǩ��ʶ
                           NULL,   //û������
                           NULL,   //ʹ�ñ���ϵͳ�ʻ�
                           NULL);  //������
  //�����������ɹ�                             
  if(schService!=NULL)
  {
			//�رվ��
   CloseServiceHandle(schService);
  }
  else
  {
   status = GetLastError(); //��ȡ�������
  }
		//�رվ��
  CloseServiceHandle(schSCManager);
 }
	else
 {
  status = GetLastError(); //��ȡ�������
 }
 return status;
}
////////////////////////End of Function///////////////

/////////////////////////////////////////////////
//ɾ������ĺ�����ֹͣ������
DWORD CFilterInstallerOfComputer00Dlg::RemoveService(LPCTSTR pszFilterName)
{
 DWORD status = ERROR_SUCCESS;

 SC_HANDLE schSCManager;

	schSCManager=OpenSCManager(NULL, //�������
                            NULL, //SCM���ݿ���
                            SC_MANAGER_ALL_ACCESS); //���ʷ�ʽ

 if(schSCManager!=NULL)
 {
  SC_HANDLE schService;
		schService=OpenService(schSCManager, //SCM���ݿ���
                         pszFilterName, //������
                         SERVICE_ALL_ACCESS); //���ʷ�ʽ
  if(schService!=NULL)
  {
   status=DeleteService(schService); //ɾ������
   CloseServiceHandle(schService); //�رվ��
  }
  else
  {
   status = GetLastError();
  }
  CloseServiceHandle(schSCManager); //�رվ��
 }
 else
 {
  status = GetLastError();
 }
 return status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////
//��װ���������ĺ���
DWORD CFilterInstallerOfComputer00Dlg::InstallFilter(void)
{
	DWORD Status = ERROR_SUCCESS;
	HDEVINFO hDevInfo;
 TCHAR WindowsDirectory[MAX_PATH];
 TCHAR NewFileName[MAX_PATH];
 SP_DEVINFO_DATA DeviceInfoData;
 DWORD Index;
 TCHAR DeviceHardwareID[MAX_PATH]; //����Ӳ��ID
 TCHAR DevDesc[MAX_PATH];  //�����豸������
 DWORD nSize = 0;

 ZeroMemory(WindowsDirectory, sizeof(WindowsDirectory));
 ZeroMemory(NewFileName, sizeof(NewFileName));

	//��ȡwindowsĿ¼
 if (!GetWindowsDirectory(WindowsDirectory, sizeof(WindowsDirectory)))
 {
  Status = GetLastError();
  return Status;
 }

	//����Windows\System32\Drivers\MyUsbLowerFilter.sys�ļ�·��
 _tcscpy(NewFileName, WindowsDirectory);
 _tcscat(NewFileName, _T("\\System32\\Drivers\\"));
 _tcscat(NewFileName, _T("MyUsbLowerFilter.sys"));
 
	AddToInfOut("���ڸ����ļ�...");

 //���������ļ���system32/driversĿ¼
 if(!CopyFile(_T("MyUsbLowerFilter.sys"),//Դ�ļ���
              NewFileName,       //Ŀ���ļ�����·��
              FALSE))    //���Ŀ���ļ��Ѿ����ڣ���ǿ�и���
 {
  Status=GetLastError();
		if(Status==ERROR_FILE_NOT_FOUND)
		{
			AddToInfOut("ϵͳ�Ҳ���ָ�����ļ���MyUsbLowerFilter.sys��");
			MessageBox("ϵͳ�Ҳ���ָ�����ļ���MyUsbLowerFilter.sys��.",NULL,16);
		}
		//ע�⣺�������VC�µ��ԣ������ļ��᲻�ɹ�
		//�����ֶ��ȸ����ļ�������ֱ�ӽ���Ŀ¼���У���
		//Ȼ��ɾ������ķ��ش��룬������ԡ�
  return Status;
 }
 
	AddToInfOut("�ļ�������ϣ���ʼж�ؾ�����...");

	//ɾ��ԭ��USB���̵�������ʹ�õ�GUIDΪ��װ��GUID��
 Status=RemoveDeviceDriver(&KeyboardClassGuid, //�������GUID
		                         MyKeyboardHardwareID, //Ҫɾ����Ӳ��ID
																       				DIGCF_PRESENT); //ֻɾ�������ӵ��豸

	if(Status==FALSE)
	{
		AddToInfOut("ж�ؾ�����ʧ�ܣ�û���ҵ�ָ�����豸");
		return 0xFFFF; //ж������ʧ��
 }

	AddToInfOut("�������ӷ���...");

	//���ӷ���
	Status=AddService(_T("MyUsbLowerFilter"), 
		        _T("System32\\Drivers\\MyUsbLowerFilter.sys"));
 
	//�����װʧ���򷵻�
	if ((Status!=ERROR_SUCCESS)&&(Status!=ERROR_SERVICE_EXISTS))
 {
		if(Status==ERROR_SERVICE_MARKED_FOR_DELETE)
			AddToInfOut("ָ�����񱻱�־Ϊ׼��ɾ������ע�����������������������");
  return Status;
 }
 
	AddToInfOut("���ڲ�����Ҫ��װ�����������豸...");

	//��ָ�����豸���ϣ��Բ��Ұ�װ���豸
 hDevInfo=SetupDiGetClassDevs(&HidClassGuid, 
		                            NULL, 
																														NULL,
																														DIGCF_PRESENT);
 
	if (hDevInfo==INVALID_HANDLE_VALUE) //���Ϊ��Ч���
 {
  Status = GetLastError(); //��ȡ�������
  return Status; //���ش������
 }
 
	DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);

	//�о������豸���ж��豸Ӳ��ID�Ƿ�ƥ��
	Index=0;
 while(SetupDiEnumDeviceInfo(hDevInfo, Index, &DeviceInfoData))
 {
		Index++; //ָ����һ���豸
		//��ȡ�豸��Ӳ��ID
	 Status=SetupDiGetDeviceRegistryProperty(
             hDevInfo,
             &DeviceInfoData,
             SPDRP_HARDWAREID,
             NULL,
             (PBYTE)DeviceHardwareID,
             sizeof(DeviceHardwareID),
             &nSize);
  if (Status==FALSE)continue; //�����ȡʧ�ܣ�������һ��

		//��ȡ�豸������
  Status=SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_DEVICEDESC,
                NULL,
                (PBYTE)&DevDesc,
                sizeof(DevDesc),
                &nSize);
		if (Status==FALSE)continue; //�����ȡʧ�ܣ�������һ��
		Status=ERROR_SUCCESS; //����״̬Ϊ�ɹ�
		//�Ƚ�Ӳ��ID�Ƿ�ƥ�䣬��Ȼ�㻹�����ٱȽ�һ���豸������
  if (_tcscmp(DeviceHardwareID, FilterHardwareID)==0)
  {
   TCHAR Filters[MAX_PATH];
   DWORD FiltersLength=0;
			//���ƥ�䣬��ô��ע����л�ȡ���豸���²���������������б�
			//��ע�����HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\
			//Enum\USB\Vid_8888&Pid_0002\2008-07-12�ұߵ�LowerFilters
			//�����ֵ
			
			//��ʾһЩ��Ϣ
			AddToInfOut("ָ�����豸���ҵ���Ӳ��IDΪ��\r\n��",FALSE,FALSE);
			AddToInfOut(DeviceHardwareID,FALSE,FALSE);
   AddToInfOut("��\r\n�豸����Ϊ��",FALSE,FALSE);
			AddToInfOut(DevDesc,FALSE,FALSE);
			AddToInfOut("��");

			AddToInfOut("����Ϊ���豸�����²���������б�...");

   if (SetupDiGetDeviceRegistryProperty(
                        hDevInfo,
                        &DeviceInfoData,
                        SPDRP_LOWERFILTERS,
                        NULL,
                        (PBYTE)Filters,
                        sizeof(Filters),
                        &FiltersLength))
   {
    LPTSTR p;
				//�����ȡ�ɹ�����ô�������濴��û��MyUsbLowerFilter����ַ�����
				//������ˣ���˵���Ѿ���װ���ˣ����������
				p=SearchMultiSz(Filters, FiltersLength, _T("MyUsbLowerFilter"));
    if (p==NULL) //���û���ҵ�����˵��û�а�װ����Ҫ���
    {
					//��"MyUsbLowerFilter"���Ƶ��������������б����
     //��Ϊ���������0������ӵ�Ҫ���ڵ�һ��0����
     FiltersLength -=sizeof(TCHAR);
     _tcscpy(&Filters[FiltersLength/sizeof(TCHAR)],_T("MyUsbLowerFilter"));
					//�޸��²�������������б�ĳ���
     FiltersLength += (DWORD)_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR);
					//ע���������͵��ַ�����������0Ϊ��β�ģ�������2�ֽ�Ҫ����Ϊ0
     Filters[FiltersLength/sizeof(TCHAR)]=0;
     Filters[FiltersLength/sizeof(TCHAR)+1]=0;
					FiltersLength +=2*sizeof(TCHAR); //����������2��0
					//�޸�ע������²�������������б�
     if (SetupDiSetDeviceRegistryProperty(
                                hDevInfo,
                                &DeviceInfoData,
                                SPDRP_LOWERFILTERS,
                                (PBYTE)Filters,
                                FiltersLength))
     {
						//������óɹ��������������豸����������������豸�Ļ���
						//�����ᷢ����Ӳ������Ϊ���ǵ������ᵼ���豸���͸ı䡣
						RestartDevice(hDevInfo,&DeviceInfoData);
     }
					else
					{
      Status = GetLastError();
					}
    }
				else
				{
					AddToInfOut("���ø����²���������б�(�Ѱ�װ��)");
				}
   }
   else
   {
    Status = GetLastError();
    //���û�л�ȡ�������Ҵ���Ϊ��Ч���ݣ���˵������δ��װ��
				//�²����������ע�����HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\
			 //Enum\USB\Vid_8888&Pid_0002\2008-07-12�ұߵ�û��
				//LowerFilters��������ô����Ҫ������������ֵΪ
				//"MyUsbLowerFilter"
    if (Status == ERROR_INVALID_DATA)
    {
     Status = ERROR_SUCCESS;
					//�����²�������������б�Ϊ"MyUsbLowerFilter"
     _tcscpy(Filters, _T("MyUsbLowerFilter"));
     FiltersLength = (DWORD)_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR);
					//�����������ֽڽ�����
     Filters[FiltersLength/sizeof(TCHAR)] = 0;
     Filters[FiltersLength/sizeof(TCHAR) + 1] = 0;
     FiltersLength +=2*sizeof(TCHAR); //����������2��0
					//�޸�ע������²�������������б�
     if (SetupDiSetDeviceRegistryProperty(
                  hDevInfo,
                  &DeviceInfoData,
                  SPDRP_LOWERFILTERS,
                  (PBYTE)Filters,
                  FiltersLength))
     {
						//������óɹ��������������豸����������������豸�Ļ���
						//�����ᷢ����Ӳ������Ϊ���ǵ������ᵼ���豸���͸ı䡣
						RestartDevice(hDevInfo,&DeviceInfoData);
     }
					else
					{
      Status = GetLastError();
					}
    }
   }
  }
 }
	//������Դ
 SetupDiDestroyDeviceInfoList(hDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////
//ж�ع��������ĺ���
DWORD CFilterInstallerOfComputer00Dlg::UninstallFilter(void)
{
	DWORD Status = ERROR_SUCCESS;
	HDEVINFO hDevInfo;
 SP_DEVINFO_DATA DeviceInfoData;
 DWORD Index;
 TCHAR DeviceHardwareID[MAX_PATH]; //����Ӳ��ID
 TCHAR DevDesc[MAX_PATH];  //�����豸������
 DWORD nSize = 0;

	AddToInfOut("����ж�ؾ�����...");
	//ɾ��USB�����豸����������װ��������֮��ͱ��
	//USB�����豸�ˡ�ʹ�õ�GUIDΪ��װ��GUID��
 Status=RemoveDeviceDriver(&HidClassGuid, //HID���GUID
		                          MyHidHardwareID, //HID�豸��Ӳ��ID
																       					DIGCF_PRESENT); //ֻɾ�������ӵ��豸

	if(Status==FALSE)
	{
		AddToInfOut("ж�ؾ�����ʧ�ܣ�û���ҵ�ָ�����豸");
		return 0xFFFF; //ж������ʧ��
 }
	AddToInfOut("���ڲ�����Ҫж�ع����������豸...");
	//��ָ�����豸��Ϣ����
 hDevInfo=SetupDiGetClassDevs(&HidClassGuid,
		                            NULL, 
																														NULL,
																														DIGCF_PRESENT);
 
	if (hDevInfo==INVALID_HANDLE_VALUE) //���Ϊ��Ч���
 {
  Status = GetLastError(); //��ȡ�������
  return Status; //���ش������
 }
 
	DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);

	//�о������豸���ж��豸Ӳ��ID�Ƿ�ƥ��
	Index=0;
 while(SetupDiEnumDeviceInfo(hDevInfo, Index, &DeviceInfoData))
 {
		Index++; //ָ����һ���豸
		//��ȡ�豸��Ӳ��ID
	 Status=SetupDiGetDeviceRegistryProperty(
             hDevInfo,
             &DeviceInfoData,
             SPDRP_HARDWAREID,
             NULL,
             (PBYTE)DeviceHardwareID,
             sizeof(DeviceHardwareID),
             &nSize);
  if (Status==FALSE)continue; //�����ȡʧ�ܣ�������һ��

		//��ȡ�豸������
  Status=SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_DEVICEDESC,
                NULL,
                (PBYTE)&DevDesc,
                sizeof(DevDesc),
                &nSize);
		if (Status==FALSE)continue; //�����ȡʧ�ܣ�������һ��

  Status=ERROR_SUCCESS; //����״̬Ϊ�ɹ�
		//�Ƚ�Ӳ��ID�Ƿ���ָ����Ӳ��ID����Ȼ�����ԱȽ�һ���豸����
  if (_tcscmp(DeviceHardwareID, FilterHardwareID)==0)
  {
   TCHAR Filters[MAX_PATH];
   DWORD FiltersLength=0;
   //��ʾ��Ϣ
			AddToInfOut("ָ�����豸���ҵ���Ӳ��IDΪ��\r\n��",FALSE,FALSE);
			AddToInfOut(DeviceHardwareID,FALSE,FALSE);
   AddToInfOut("��\r\n�豸����Ϊ��",FALSE,FALSE);
			AddToInfOut(DevDesc,FALSE,FALSE);
			AddToInfOut("��");
			AddToInfOut("����Ϊ���豸�����²���������б�...");

			//�����ָ�����豸�����ȡ�Ͳ������������Ϣ
   if (SetupDiGetDeviceRegistryProperty(
                        hDevInfo,
                        &DeviceInfoData,
                        SPDRP_LOWERFILTERS,
                        NULL,
                        (PBYTE)Filters,
                        sizeof(Filters),
                        &FiltersLength))
   {
    LPTSTR p;
				//���ҵͲ�������������б����Ƿ���ָ����"MyUsbLowerFilter"�ַ�����
				//����У���˵���Ѿ���װ���ˣ���Ҫɾ��
				p=SearchMultiSz(Filters, FiltersLength, _T("MyUsbLowerFilter"));
    if (p!=NULL) //����ҵ���ָ�����ĵ�ַ��pλ�ã�����ôɾ��֮
    {
					//��p+"MyUsbLowerFilter"���ȣ������������������������ǰ�ƶ���pλ��
     LPTSTR StartPoint;
     size_t MoveLength;
					//�ƶ������Ϊp����"MyUsbLowerFilter"�ַ������ȼ���һ��������
					StartPoint=p
						          +_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR)
						          +sizeof(TCHAR);
					//�ƶ��ĳ���Ϊ���ʣ���ֽ��������������ƶ����ǰ��ĳ���
					MoveLength=FiltersLength-(StartPoint-Filters);
     //�ƶ�����
     MoveMemory(p,StartPoint,MoveLength);
     
					//���ĳ���Ϊ
     FiltersLength -=(DWORD)(_tcslen(_T("MyUsbLowerFilter"))+1)*sizeof(TCHAR);

     if (SetupDiSetDeviceRegistryProperty(
                                hDevInfo,
                                &DeviceInfoData,
                                SPDRP_LOWERFILTERS,
                                (PBYTE)Filters,
                                FiltersLength))
     {
						//������óɹ��������������豸����������������豸�Ļ���
						//�����ᷢ����Ӳ������Ϊ���ǵ������ᵼ���豸���͸ı䡣
						RestartDevice(hDevInfo,&DeviceInfoData);
     }
					else
					{
      Status = GetLastError();
					}
    }
   }
   else
   {
				Status=GetLastError();
   }
  }
 }
 SetupDiDestroyDeviceInfoList(hDevInfo);

	AddToInfOut("����ɾ������...");
	//ɾ������
	Status=!RemoveService(_T("MyUsbLowerFilter"));

	return Status;
}
////////////////////////End of Function///////////////

/////////////////////////////////////////////////////
void CFilterInstallerOfComputer00Dlg::DisableButtons(void)
{
	GetDlgItem(IDC_INSTALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_UNINSTALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_ABOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_QUIT)->EnableWindow(FALSE);
}
////////////////////////End of Function///////////////

/////////////////////////////////////////////////////
void CFilterInstallerOfComputer00Dlg::EnableButtons(void)
{
	GetDlgItem(IDC_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_UNINSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_ABOUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_QUIT)->EnableWindow(TRUE);
}
////////////////////////End of Function///////////////

void CFilterInstallerOfComputer00Dlg::OnInstall()
{
	// TODO: Add your control notification handler code here
 ClrInfOut(); //�����Ϣ
	m_Percent.SetPos(0);
	m_CurrentSteps=0;
	m_NeedSteps=14;
	AddToInfOut("��ʼ��װ...");
	DisableButtons();
	if(InstallFilter())
	{
		AddToInfOut("��װʧ��");
	}
	else
	{
		AddToInfOut("���������ѳɹ���װ");
	}
	EnableButtons();
	m_Percent.SetPos(100);
}
////////////////////////End of Function///////////////

void CFilterInstallerOfComputer00Dlg::OnUninstall()
{
	// TODO: Add your control notification handler code here
	ClrInfOut(); //�����Ϣ
	m_Percent.SetPos(0);
	m_CurrentSteps=0;
	m_NeedSteps=10;
	AddToInfOut("��ʼж��...");
	DisableButtons();
	if(UninstallFilter())
 {
		AddToInfOut("ж��ʧ��");
 }
	else
	{
		AddToInfOut("���������ѳɹ�ж��");
	}
	EnableButtons();
	m_Percent.SetPos(100);
}
////////////////////////End of Function///////////////

void CFilterInstallerOfComputer00Dlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}

void CFilterInstallerOfComputer00Dlg::OnQuit()
{
	// TODO: Add your control notification handler code here
	DestroyWindow(); //���ٴ���
}

void CFilterInstallerOfComputer00Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Percent.SetRange(0,100);
	m_Percent.SetPos(0);
}

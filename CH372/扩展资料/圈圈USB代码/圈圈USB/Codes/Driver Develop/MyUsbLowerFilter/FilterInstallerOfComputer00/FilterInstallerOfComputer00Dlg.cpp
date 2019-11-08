/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

作者：电脑圈圈
建立日期: 2008.08.28
修改日期: 2008.08.30
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
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

//将信息添加到信息显示框的函数
void CFilterInstallerOfComputer00Dlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
{
	CString str,str1;
	UINT i;
	CHAR SysTime[10];
 
	//先获取原来显示的文本
	GetDlgItemText(IDC_INF_OUT,str);
	//将新的信息追加在最后
	str+=InStr;

	//需要添加时间信息
	if(AddTime==TRUE)
	{
		_strtime(SysTime);
		str1=SysTime;
		str1=" ("+str1+")";
		str+=str1;
	}

	//需要换行
	if(NewLine==TRUE)
	{
		str+="\r\n";
	}

	//设置新的文本
	SetDlgItemText(IDC_INF_OUT,str);
	//滚动条自动滚动到最后一行
	i=((CEdit*)GetDlgItem(IDC_INF_OUT))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_INF_OUT))->LineScroll(i,0);

	//设置进度条
	if(AddTime==TRUE)
	{
		m_CurrentSteps++;
		m_Percent.SetPos(m_CurrentSteps*100/m_NeedSteps);
	}
}
//////////////////////////////End of function//////////////////////

//重载上述函数，入口参数不一样。
void CFilterInstallerOfComputer00Dlg::AddToInfOut(char *p, BOOL AddTime, BOOL NewLine)
{
	CString str=p;
	AddToInfOut(str,AddTime,NewLine);
}
//////////////////////////////End of function//////////////////////

//清除信息显示框
void CFilterInstallerOfComputer00Dlg::ClrInfOut(void)
{
	SetDlgItemText(IDC_INF_OUT,"");
}
//////////////////////////////End of function//////////////////////

///////////////////////////////////////////////////
//改变设备状态的函数
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
//启动设备的函数
DWORD CFilterInstallerOfComputer00Dlg::StartDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD Status;
	//设置设备为启动状态
	Status = ChangeDeviceState(DICS_START,
		                          DICS_FLAG_CONFIGSPECIFIC,
																												hDevInfoSet,
																												pDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////////
//停止设备的函数
DWORD CFilterInstallerOfComputer00Dlg::StopDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD Status;
	//设置设备为停止状态
	Status = ChangeDeviceState(DICS_STOP,
		                          DICS_FLAG_CONFIGSPECIFIC,
																												hDevInfoSet,
																												pDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

//////////////////////////////////////////////////////
//重新启动设备，即先停止设备，然后再重新启动
DWORD CFilterInstallerOfComputer00Dlg::RestartDevice(HDEVINFO hDevInfoSet, 
																													PSP_DEVINFO_DATA pDevInfo)
{
	DWORD dwError;

	AddToInfOut("正在重新启动设备...");

 dwError = StopDevice(hDevInfoSet,pDevInfo);

	if (ERROR_SUCCESS == dwError)
	{
		dwError = StartDevice(hDevInfoSet,pDevInfo);
	}

	return dwError;
}
////////////////////////End of Function///////////////

////////////////////////////////////
//搜索是否出现指定的字符串函数
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
//卸载指定设备的驱动，如果过滤驱动改变了设备的类型，
//那么要先卸载掉被过滤的驱动程序的高一层的设备驱动，
//否则过滤驱动安装完毕之后并不会导致新设备的出现，
//如果拔下设备，还会导致系统死机。
//入口参数pGuid: 为删除设备的GUID。
//入口参数HardwareID：为删除设备的硬件ID。
//入口参数Flags：指定查找哪些设备以及什么类型的GUID。
//               可以选择为DIGCF_PRESENT、DIGCF_ALLCLASSES、
//               以及DIGCF_DEVICEINTERFACE的组合。
//               DIGCF_PRESENT表示只查找已连接的设备。
//               DIGCF_ALLCLASSES表示全部设备。
//               DIGCF_DEVICEINTERFACE指定GUID为接口类GUID，
//               否则为安装类GUID。
BOOL CFilterInstallerOfComputer00Dlg::RemoveDeviceDriver(GUID *pGuid, LPCTSTR HardwareID,DWORD Flags)
{
 UINT i;
 BOOL Result=FALSE;
 HDEVINFO hDevInfo;
 SP_DEVINFO_DATA DeviceInfoData;
 TCHAR DeviceHardwareID[MAX_PATH]; //保存硬件ID
 TCHAR DevDesc[MAX_PATH];  //保存设备的描述
 DWORD Status;
	DWORD nSize;

 DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);
	
	//获取指定GUID的设备类信息句柄
	hDevInfo=SetupDiGetClassDevs(pGuid, NULL, NULL,Flags);

	//查找信息集合中所有的设备，看硬件ID是否相符
 i=0;
 while(SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData))
 {
		//获取设备的硬件ID
	 Status=SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_HARDWAREID,
            NULL,
            (PBYTE)DeviceHardwareID,
            sizeof(DeviceHardwareID),
            &nSize);
  if(Status) //如果获取成功
  {
   //判断硬件ID是否匹配
   if(_tcsicmp((LPCTSTR)(PBYTE) DeviceHardwareID, HardwareID)==0)
   {
	  	//获取设备的描述
	   Status=SetupDiGetDeviceRegistryProperty(
               hDevInfo,
               &DeviceInfoData,
               SPDRP_DEVICEDESC,
               NULL,
               (PBYTE)DevDesc,
               sizeof(DevDesc),
               &nSize);
				//删除设备
    if(SetupDiCallClassInstaller(DIF_REMOVE,hDevInfo,&DeviceInfoData))
    {
     Result=TRUE;
					AddToInfOut("硬件ID为\r\n“",FALSE,FALSE);
					AddToInfOut(DeviceHardwareID,FALSE,FALSE);
					AddToInfOut("”的\r\n“",FALSE,FALSE);
					AddToInfOut(DevDesc,FALSE,FALSE);
					AddToInfOut("”设备卸载成功.");
    }
   }
  }
  i++; //Next device.
 }
 return Result;
}
////////////////////////End of Function///////////////

//定义键盘的安装GUID，用来先卸载键盘设备
GUID KeyboardClassGuid=
{
	0x4D36E96B,0xE325,0x11CE,
	{0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}
};

//定义要删除的键盘设备的硬件ID
LPCTSTR MyKeyboardHardwareID =
	_T("HID\\Vid_8888&Pid_0002&Rev_0100");
 
//定义HID设备的安装类GUID，增加过滤驱动和卸载兼容HID设备时用
GUID HidClassGuid=
{
	0x745A17A0,0x74D3,0x11D0,
	{0xB6,0xFE,0x00,0xA0,0xC9,0x0F,0x57,0xDA}
};

//定义需要安装过滤驱动的设备的硬件ID
LPCTSTR FilterHardwareID = 
	_T("USB\\Vid_8888&Pid_0002&Rev_0100");
//当需要卸载过滤驱动时，需要先卸载HID兼容设备，
//这个硬件ID就是用来卸载HID兼容设备用的
LPCTSTR MyHidHardwareID =
	_T("HID\\Vid_8888&Pid_0002&Rev_0100");

/////////////////////////////////////////////////
//添加服务的函数（启动驱动程序服务）
DWORD CFilterInstallerOfComputer00Dlg::AddService(LPCTSTR pszFilterName, 
																																	LPCTSTR pszServiceBinary)
{
 DWORD status = ERROR_SUCCESS;
 SC_HANDLE schSCManager;
 SC_HANDLE schService;

	schSCManager=OpenSCManager(NULL, //计算机名
                            NULL, //SCM数据库名
                            SC_MANAGER_ALL_ACCESS); //访问方式
 if (schSCManager!=NULL)
 {
  schService=CreateService(schSCManager, //SCManager数据库
                           pszFilterName, //服务的名称
                           pszFilterName, //显示的名称
                           SERVICE_ALL_ACCESS,  //访问权限
                           SERVICE_KERNEL_DRIVER,//服务类型
                           SERVICE_DEMAND_START, //启动方式
                           SERVICE_ERROR_NORMAL, //出错控制类型
                           pszServiceBinary, //提供服务的文件路径
                           NULL,   //无加载顺序分组
                           NULL,   //无标签标识
                           NULL,   //没有依赖
                           NULL,   //使用本地系统帐户
                           NULL);  //无密码
  //如果创建服务成功                             
  if(schService!=NULL)
  {
			//关闭句柄
   CloseServiceHandle(schService);
  }
  else
  {
   status = GetLastError(); //获取出错代码
  }
		//关闭句柄
  CloseServiceHandle(schSCManager);
 }
	else
 {
  status = GetLastError(); //获取出错代码
 }
 return status;
}
////////////////////////End of Function///////////////

/////////////////////////////////////////////////
//删除服务的函数（停止驱动）
DWORD CFilterInstallerOfComputer00Dlg::RemoveService(LPCTSTR pszFilterName)
{
 DWORD status = ERROR_SUCCESS;

 SC_HANDLE schSCManager;

	schSCManager=OpenSCManager(NULL, //计算机名
                            NULL, //SCM数据库名
                            SC_MANAGER_ALL_ACCESS); //访问方式

 if(schSCManager!=NULL)
 {
  SC_HANDLE schService;
		schService=OpenService(schSCManager, //SCM数据库句柄
                         pszFilterName, //服务名
                         SERVICE_ALL_ACCESS); //访问方式
  if(schService!=NULL)
  {
   status=DeleteService(schService); //删除服务
   CloseServiceHandle(schService); //关闭句柄
  }
  else
  {
   status = GetLastError();
  }
  CloseServiceHandle(schSCManager); //关闭句柄
 }
 else
 {
  status = GetLastError();
 }
 return status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////
//安装过滤驱动的函数
DWORD CFilterInstallerOfComputer00Dlg::InstallFilter(void)
{
	DWORD Status = ERROR_SUCCESS;
	HDEVINFO hDevInfo;
 TCHAR WindowsDirectory[MAX_PATH];
 TCHAR NewFileName[MAX_PATH];
 SP_DEVINFO_DATA DeviceInfoData;
 DWORD Index;
 TCHAR DeviceHardwareID[MAX_PATH]; //保存硬件ID
 TCHAR DevDesc[MAX_PATH];  //保存设备的描述
 DWORD nSize = 0;

 ZeroMemory(WindowsDirectory, sizeof(WindowsDirectory));
 ZeroMemory(NewFileName, sizeof(NewFileName));

	//获取windows目录
 if (!GetWindowsDirectory(WindowsDirectory, sizeof(WindowsDirectory)))
 {
  Status = GetLastError();
  return Status;
 }

	//产生Windows\System32\Drivers\MyUsbLowerFilter.sys文件路径
 _tcscpy(NewFileName, WindowsDirectory);
 _tcscat(NewFileName, _T("\\System32\\Drivers\\"));
 _tcscat(NewFileName, _T("MyUsbLowerFilter.sys"));
 
	AddToInfOut("正在复制文件...");

 //复制驱动文件到system32/drivers目录
 if(!CopyFile(_T("MyUsbLowerFilter.sys"),//源文件名
              NewFileName,       //目标文件名及路径
              FALSE))    //如果目标文件已经存在，则强行覆盖
 {
  Status=GetLastError();
		if(Status==ERROR_FILE_NOT_FOUND)
		{
			AddToInfOut("系统找不到指定的文件“MyUsbLowerFilter.sys”");
			MessageBox("系统找不到指定的文件“MyUsbLowerFilter.sys”.",NULL,16);
		}
		//注意：如果是在VC下调试，复制文件会不成功
		//可以手动先复制文件（或者直接进入目录运行），
		//然后删除下面的返回代码，方便调试。
  return Status;
 }
 
	AddToInfOut("文件复制完毕，开始卸载旧驱动...");

	//删除原来USB键盘的驱动，使用的GUID为安装类GUID。
 Status=RemoveDeviceDriver(&KeyboardClassGuid, //键盘类的GUID
		                         MyKeyboardHardwareID, //要删除的硬件ID
																       				DIGCF_PRESENT); //只删除已连接的设备

	if(Status==FALSE)
	{
		AddToInfOut("卸载旧驱动失败，没有找到指定的设备");
		return 0xFFFF; //卸载驱动失败
 }

	AddToInfOut("正在增加服务...");

	//增加服务
	Status=AddService(_T("MyUsbLowerFilter"), 
		        _T("System32\\Drivers\\MyUsbLowerFilter.sys"));
 
	//如果安装失败则返回
	if ((Status!=ERROR_SUCCESS)&&(Status!=ERROR_SERVICE_EXISTS))
 {
		if(Status==ERROR_SERVICE_MARKED_FOR_DELETE)
			AddToInfOut("指定服务被标志为准备删除，请注销或重新启动来解决此问题");
  return Status;
 }
 
	AddToInfOut("正在查找需要安装过滤驱动的设备...");

	//打开指定的设备集合，以查找安装的设备
 hDevInfo=SetupDiGetClassDevs(&HidClassGuid, 
		                            NULL, 
																														NULL,
																														DIGCF_PRESENT);
 
	if (hDevInfo==INVALID_HANDLE_VALUE) //如果为无效句柄
 {
  Status = GetLastError(); //获取错误代码
  return Status; //返回错误代码
 }
 
	DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);

	//列举所有设备，判断设备硬件ID是否匹配
	Index=0;
 while(SetupDiEnumDeviceInfo(hDevInfo, Index, &DeviceInfoData))
 {
		Index++; //指向下一个设备
		//获取设备的硬件ID
	 Status=SetupDiGetDeviceRegistryProperty(
             hDevInfo,
             &DeviceInfoData,
             SPDRP_HARDWAREID,
             NULL,
             (PBYTE)DeviceHardwareID,
             sizeof(DeviceHardwareID),
             &nSize);
  if (Status==FALSE)continue; //如果获取失败，查找下一个

		//获取设备的描述
  Status=SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_DEVICEDESC,
                NULL,
                (PBYTE)&DevDesc,
                sizeof(DevDesc),
                &nSize);
		if (Status==FALSE)continue; //如果获取失败，查找下一个
		Status=ERROR_SUCCESS; //设置状态为成功
		//比较硬件ID是否匹配，当然你还可以再比较一下设备的描述
  if (_tcscmp(DeviceHardwareID, FilterHardwareID)==0)
  {
   TCHAR Filters[MAX_PATH];
   DWORD FiltersLength=0;
			//如果匹配，那么从注册表中获取该设备的下层过滤驱动的名称列表
			//即注册表中HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\
			//Enum\USB\Vid_8888&Pid_0002\2008-07-12右边的LowerFilters
			//表项的值
			
			//显示一些信息
			AddToInfOut("指定的设备已找到，硬件ID为：\r\n“",FALSE,FALSE);
			AddToInfOut(DeviceHardwareID,FALSE,FALSE);
   AddToInfOut("”\r\n设备描述为“",FALSE,FALSE);
			AddToInfOut(DevDesc,FALSE,FALSE);
			AddToInfOut("”");

			AddToInfOut("正在为该设备更新下层过滤名称列表...");

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
				//如果获取成功，那么查找里面看有没有MyUsbLowerFilter这个字符串，
				//如果有了，则说明已经安装过了，不用再添加
				p=SearchMultiSz(Filters, FiltersLength, _T("MyUsbLowerFilter"));
    if (p==NULL) //如果没有找到，则说明没有安装，需要添加
    {
					//将"MyUsbLowerFilter"复制到过滤驱动名称列表最后
     //因为最后有两个0，而添加的要放在第一个0背后
     FiltersLength -=sizeof(TCHAR);
     _tcscpy(&Filters[FiltersLength/sizeof(TCHAR)],_T("MyUsbLowerFilter"));
					//修改下层过滤驱动名称列表的长度
     FiltersLength += (DWORD)_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR);
					//注意这种类型的字符串是以两个0为结尾的，接下来2字节要设置为0
     Filters[FiltersLength/sizeof(TCHAR)]=0;
     Filters[FiltersLength/sizeof(TCHAR)+1]=0;
					FiltersLength +=2*sizeof(TCHAR); //后面增加了2个0
					//修改注册表中下层过滤驱动名称列表
     if (SetupDiSetDeviceRegistryProperty(
                                hDevInfo,
                                &DeviceInfoData,
                                SPDRP_LOWERFILTERS,
                                (PBYTE)Filters,
                                FiltersLength))
     {
						//如果设置成功，则重新启动设备。如果不重新启动设备的话，
						//将不会发现新硬件，因为我们的驱动会导致设备类型改变。
						RestartDevice(hDevInfo,&DeviceInfoData);
     }
					else
					{
      Status = GetLastError();
					}
    }
				else
				{
					AddToInfOut("不用更新下层过滤名称列表(已安装过)");
				}
   }
   else
   {
    Status = GetLastError();
    //如果没有获取到，并且错误为无效数据，则说明从来未安装过
				//下层过滤驱动即注册表中HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\
			 //Enum\USB\Vid_8888&Pid_0002\2008-07-12右边的没有
				//LowerFilters这个表项，那么就需要设置这个表项的值为
				//"MyUsbLowerFilter"
    if (Status == ERROR_INVALID_DATA)
    {
     Status = ERROR_SUCCESS;
					//设置下层过滤驱动名称列表为"MyUsbLowerFilter"
     _tcscpy(Filters, _T("MyUsbLowerFilter"));
     FiltersLength = (DWORD)_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR);
					//设置最后的两字节结束符
     Filters[FiltersLength/sizeof(TCHAR)] = 0;
     Filters[FiltersLength/sizeof(TCHAR) + 1] = 0;
     FiltersLength +=2*sizeof(TCHAR); //后面增加了2个0
					//修改注册表中下层过滤驱动名称列表
     if (SetupDiSetDeviceRegistryProperty(
                  hDevInfo,
                  &DeviceInfoData,
                  SPDRP_LOWERFILTERS,
                  (PBYTE)Filters,
                  FiltersLength))
     {
						//如果设置成功，则重新启动设备。如果不重新启动设备的话，
						//将不会发现新硬件，因为我们的驱动会导致设备类型改变。
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
	//销毁资源
 SetupDiDestroyDeviceInfoList(hDevInfo);
	return Status;
}
////////////////////////End of Function///////////////

///////////////////////////////////////////////////
//卸载过滤驱动的函数
DWORD CFilterInstallerOfComputer00Dlg::UninstallFilter(void)
{
	DWORD Status = ERROR_SUCCESS;
	HDEVINFO hDevInfo;
 SP_DEVINFO_DATA DeviceInfoData;
 DWORD Index;
 TCHAR DeviceHardwareID[MAX_PATH]; //保存硬件ID
 TCHAR DevDesc[MAX_PATH];  //保存设备的描述
 DWORD nSize = 0;

	AddToInfOut("正在卸载旧驱动...");
	//删除USB兼容设备的驱动，安装过滤驱动之后就变成
	//USB兼容设备了。使用的GUID为安装类GUID。
 Status=RemoveDeviceDriver(&HidClassGuid, //HID类的GUID
		                          MyHidHardwareID, //HID设备的硬件ID
																       					DIGCF_PRESENT); //只删除已连接的设备

	if(Status==FALSE)
	{
		AddToInfOut("卸载旧驱动失败，没有找到指定的设备");
		return 0xFFFF; //卸载驱动失败
 }
	AddToInfOut("正在查找需要卸载过滤驱动的设备...");
	//打开指定类设备信息集合
 hDevInfo=SetupDiGetClassDevs(&HidClassGuid,
		                            NULL, 
																														NULL,
																														DIGCF_PRESENT);
 
	if (hDevInfo==INVALID_HANDLE_VALUE) //如果为无效句柄
 {
  Status = GetLastError(); //获取错误代码
  return Status; //返回错误代码
 }
 
	DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);

	//列举所有设备，判断设备硬件ID是否匹配
	Index=0;
 while(SetupDiEnumDeviceInfo(hDevInfo, Index, &DeviceInfoData))
 {
		Index++; //指向下一个设备
		//获取设备的硬件ID
	 Status=SetupDiGetDeviceRegistryProperty(
             hDevInfo,
             &DeviceInfoData,
             SPDRP_HARDWAREID,
             NULL,
             (PBYTE)DeviceHardwareID,
             sizeof(DeviceHardwareID),
             &nSize);
  if (Status==FALSE)continue; //如果获取失败，查找下一个

		//获取设备的描述
  Status=SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_DEVICEDESC,
                NULL,
                (PBYTE)&DevDesc,
                sizeof(DevDesc),
                &nSize);
		if (Status==FALSE)continue; //如果获取失败，查找下一个

  Status=ERROR_SUCCESS; //设置状态为成功
		//比较硬件ID是否是指定的硬件ID，当然还可以比较一下设备描述
  if (_tcscmp(DeviceHardwareID, FilterHardwareID)==0)
  {
   TCHAR Filters[MAX_PATH];
   DWORD FiltersLength=0;
   //显示信息
			AddToInfOut("指定的设备已找到，硬件ID为：\r\n“",FALSE,FALSE);
			AddToInfOut(DeviceHardwareID,FALSE,FALSE);
   AddToInfOut("”\r\n设备描述为“",FALSE,FALSE);
			AddToInfOut(DevDesc,FALSE,FALSE);
			AddToInfOut("”");
			AddToInfOut("正在为该设备更新下层过滤名称列表...");

			//如果是指定的设备，则获取低层过滤驱动的信息
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
				//查找低层过滤驱动名称列表中是否含有指定的"MyUsbLowerFilter"字符串，
				//如果有，则说明已经安装过了，需要删除
				p=SearchMultiSz(Filters, FiltersLength, _T("MyUsbLowerFilter"));
    if (p!=NULL) //如果找到（指定串的地址在p位置），那么删除之
    {
					//将p+"MyUsbLowerFilter"长度（包括结束符）后面的数据往前移动到p位置
     LPTSTR StartPoint;
     size_t MoveLength;
					//移动的起点为p后面"MyUsbLowerFilter"字符串长度加上一个结束符
					StartPoint=p
						          +_tcslen(_T("MyUsbLowerFilter"))*sizeof(TCHAR)
						          +sizeof(TCHAR);
					//移动的长度为最后剩余字节数，即除掉从移动起点前面的长度
					MoveLength=FiltersLength-(StartPoint-Filters);
     //移动数据
     MoveMemory(p,StartPoint,MoveLength);
     
					//最后的长度为
     FiltersLength -=(DWORD)(_tcslen(_T("MyUsbLowerFilter"))+1)*sizeof(TCHAR);

     if (SetupDiSetDeviceRegistryProperty(
                                hDevInfo,
                                &DeviceInfoData,
                                SPDRP_LOWERFILTERS,
                                (PBYTE)Filters,
                                FiltersLength))
     {
						//如果设置成功，则重新启动设备。如果不重新启动设备的话，
						//将不会发现新硬件，因为我们的驱动会导致设备类型改变。
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

	AddToInfOut("正在删除服务...");
	//删除服务
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
 ClrInfOut(); //清除信息
	m_Percent.SetPos(0);
	m_CurrentSteps=0;
	m_NeedSteps=14;
	AddToInfOut("开始安装...");
	DisableButtons();
	if(InstallFilter())
	{
		AddToInfOut("安装失败");
	}
	else
	{
		AddToInfOut("过滤驱动已成功安装");
	}
	EnableButtons();
	m_Percent.SetPos(100);
}
////////////////////////End of Function///////////////

void CFilterInstallerOfComputer00Dlg::OnUninstall()
{
	// TODO: Add your control notification handler code here
	ClrInfOut(); //清除信息
	m_Percent.SetPos(0);
	m_CurrentSteps=0;
	m_NeedSteps=10;
	AddToInfOut("开始卸载...");
	DisableButtons();
	if(UninstallFilter())
 {
		AddToInfOut("卸载失败");
 }
	else
	{
		AddToInfOut("过滤驱动已成功卸载");
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
	DestroyWindow(); //销毁窗口
}

void CFilterInstallerOfComputer00Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Percent.SetRange(0,100);
	m_Percent.SetPos(0);
}

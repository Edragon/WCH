// testUSBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testUSB.h"
#include "testUSBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define NUM_HCS_TO_CHECK 10
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "ScrollerCtrl.h"
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
	public:
		CScrollerCtrl m_scroller;
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void SwitchTip();
	virtual BOOL OnInitDialog();
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
		ON_COMMAND(1, SwitchTip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestUSBDlg dialog
CHAR buf[512];
TCHAR   szMoveDiskName[33];
TCHAR	szDrvName[33];
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_scroller.SetFont("Microsoft Sans Serif", 10, FW_SEMIBOLD);
   m_scroller.SetText("\tUSB例程说明");
   m_scroller.SetWrapping(FALSE);
   m_scroller.SetScrollDelay(10);
   m_scroller.SetScrollPause(3000);
   m_scroller.SetFgColor(RGB(255,0,0));

	CRect rect;
	GetClientRect(&rect);
	m_scroller.Create(rect, this, WS_CHILD|WS_VISIBLE, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CAboutDlg::SwitchTip()
{
   static const CString astrTips[] = {
      "  这是一个关于USB的例程。",
      "  需要win2000或者winXP的DDK才能编译。",
	 };
   static const int nNumTips = sizeof(astrTips)/sizeof(CString);
   static int nCurrentTip = 0;
   m_scroller.SetText(astrTips[nCurrentTip%nNumTips]);
   ++nCurrentTip;
}
CTestUSBDlg::CTestUSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestUSBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestUSBDlg)
	m_showMessage = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestUSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestUSBDlg)
	DDX_Text(pDX, IDC_SHOWMESSAGE, m_showMessage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestUSBDlg, CDialog)
	//{{AFX_MSG_MAP(CTestUSBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHOWUSBALL, OnShowusball)
	ON_BN_CLICKED(IDC_SHOWUSBHID, OnShowusbhid)
	ON_BN_CLICKED(IDC_SHOWUSBDISK, OnShowusbdisk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestUSBDlg message handlers

BOOL CTestUSBDlg::OnInitDialog()
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

void CTestUSBDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestUSBDlg::OnPaint() 
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
HCURSOR CTestUSBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestUSBDlg::OnShowusball() 
{
	m_showMessage.Empty();
	CString str;
	str="查找所有USB设备：";
	ShowMessage(str);
	NumUsbAll();

}

void CTestUSBDlg::OnShowusbhid() 
{
	// TODO: Add your control notification handler code here
	m_showMessage.Empty();
	ShowHidInfo();
}

void CTestUSBDlg::OnShowusbdisk() 
{
	// TODO: Add your control notification handler code here
	m_showMessage.Empty();
	ShowUdisk();
}

void CTestUSBDlg::ShowMessage(CString str)
{
	m_showMessage += str;
	SetDlgItemText(IDC_SHOWMESSAGE,m_showMessage);
}

void CTestUSBDlg::NumUsbAll()
{

	CString strShow;
	char        HCName[16];
    int         HCNum;
    HANDLE      hHCDev;
    PCHAR       rootHubName;
    PCHAR       leafName;

	ULONG       index;
    BOOL        success;

    PUSB_NODE_CONNECTION_INFORMATION    connectionInfo;
	HANDLE hHubDevice;

	   for (HCNum = 0; HCNum < NUM_HCS_TO_CHECK; HCNum++)
    {
        wsprintf(HCName, "\\\\.\\HCD%d", HCNum);
        hHCDev = CreateFile(HCName,
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);
		if (hHCDev != INVALID_HANDLE_VALUE)
        {
			strShow.Format("\r\n找到USB控制器:%d\r\n",HCNum+1);
			ShowMessage(strShow);
		}
		else
			break;
		  PCHAR driverKeyName, deviceDesc;
          driverKeyName = GetHCDDriverKeyName(hHCDev);
		  if(driverKeyName)
		  {	 		//想获得此设备的字符名，但不成功!!有高招请告诉我!
//			  deviceDesc = DriverNameToDeviceDesc(driverKeyName);
		  }
		  else
		  {
			 strShow.Format("错误！\r\n");
			 ShowMessage(strShow);
			  goto end;	  
		  }
		strShow.Format("USB控制器:%s\r\n",driverKeyName);
		ShowMessage(strShow);
        leafName = HCName + sizeof("\\\\.\\") - sizeof("");
		strShow.Format("USB控制器名称:%s\r\n",leafName);
		ShowMessage(strShow);
		ULONG           nBytes;

        rootHubName =(char*) GetRootHubName(hHCDev);
		if(rootHubName==NULL)
		 {
			 strShow.Format("错误！\r\n");
			 ShowMessage(strShow);
			  goto end;
		  }
		strShow.Format("USB Hub名称:\r\n%s\r\n",rootHubName);
		ShowMessage(strShow);

		 PUSB_NODE_INFORMATION HubInfo;
		 HubInfo = (PUSB_NODE_INFORMATION)malloc(sizeof(USB_NODE_INFORMATION));
		 PCHAR deviceName;
		 deviceName = (PCHAR)malloc(strlen(rootHubName) + sizeof("\\\\.\\"));
         if (rootHubName != NULL)
         {
			strcpy(deviceName, "\\\\.\\");
			strcpy(deviceName + sizeof("\\\\.\\") - 1, rootHubName);
			hHubDevice = CreateFile(deviceName,
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);
			free(deviceName);
			if (hHubDevice == INVALID_HANDLE_VALUE)
			{
				 strShow.Format("错误！\r\n");
				 ShowMessage(strShow);			
				goto end;
			}
			success = DeviceIoControl(hHubDevice,
                              IOCTL_USB_GET_NODE_INFORMATION,
                              HubInfo,
                              sizeof(USB_NODE_INFORMATION),
                              HubInfo,
                              sizeof(USB_NODE_INFORMATION),
                              &nBytes,
                              NULL);
			if (!success)
			{
				 strShow.Format("错误！\r\n");
				 ShowMessage(strShow);
				goto end;
			}

		 }
			
		 int port;
		 port=HubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts;
		 for (index=1; index <= port; index++)
		 {
			 ULONG nBytes;
			 nBytes = sizeof(USB_NODE_CONNECTION_INFORMATION) +
                sizeof(USB_PIPE_INFO) * 30;
			 connectionInfo = (PUSB_NODE_CONNECTION_INFORMATION)malloc(nBytes);
			 if (connectionInfo == NULL)
			 {
				  strShow.Format("错误！\r\n");
				 ShowMessage(strShow);
				 goto end;
			 }
			 connectionInfo->ConnectionIndex = index;
			 success = DeviceIoControl(hHubDevice,
                                  IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
                                  connectionInfo,
                                  nBytes,
                                  connectionInfo,
                                  nBytes,
                                  &nBytes,
                                  NULL);
			 if (!success)
			 {
				 free(connectionInfo);
				 strShow.Format("错误！\r\n");
				 ShowMessage(strShow);
				 goto end;
			 }
			 if(connectionInfo)
		      if (connectionInfo->ConnectionStatus == DeviceConnected)
			  {
				strShow.Format("第%d端口有USB设备连接\r\n",index);
			  }
			  else
			  {
				 strShow.Format("第%d端口没有USB设备联接\r\n",index);
				
			  }
			  ShowMessage(strShow);
			   if (connectionInfo->DeviceIsHub)
			   {
				   strShow="HUB";
				   ShowMessage(strShow);
			   }
			   deviceDesc = NULL;
			   if (connectionInfo->ConnectionStatus != NoDeviceConnected)
			   {
				   driverKeyName = GetDriverKeyName(hHubDevice,
                                             index);
				   if (driverKeyName)
				   {
					   strShow.Format("状态：已连接！\r\n连接设备名:%s\r\n",driverKeyName);
					   ShowMessage(strShow);
					   free(driverKeyName);
				   }
			   }

	      if (connectionInfo->ConnectionStatus == DeviceConnected)
        {
			ShowCenctInfo(connectionInfo);
        }
		 }
end:; 
             }
	 			CloseHandle(hHubDevice);
				CloseHandle(hHCDev);

}

PCHAR CTestUSBDlg::GetHCDDriverKeyName(HANDLE HCD)
{
   BOOL                    success;
    ULONG                   nBytes;
    USB_HCD_DRIVERKEY_NAME  driverKeyName;
    PUSB_HCD_DRIVERKEY_NAME driverKeyNameW;
    PCHAR                   driverKeyNameA;

    driverKeyNameW = NULL;
    driverKeyNameA = NULL;

    success = DeviceIoControl(HCD,
                              IOCTL_GET_HCD_DRIVERKEY_NAME,
                              &driverKeyName,
                              sizeof(driverKeyName),
                              &driverKeyName,
                              sizeof(driverKeyName),
                              &nBytes,
                              NULL);

    if (!success)
	{
        goto GetHCDDriverKeyNameError;
    }

    nBytes = driverKeyName.ActualLength;

    if (nBytes <= sizeof(driverKeyName))
    {
        goto GetHCDDriverKeyNameError;
    }

    driverKeyNameW =(PUSB_HCD_DRIVERKEY_NAME) malloc(nBytes);

    if (driverKeyNameW == NULL)
    {
        goto GetHCDDriverKeyNameError;
    }

    success = DeviceIoControl(HCD,
                              IOCTL_GET_HCD_DRIVERKEY_NAME,
                              driverKeyNameW,
                              nBytes,
                              driverKeyNameW,
                              nBytes,
                              &nBytes,
                              NULL);

    if (!success)
    {
        goto GetHCDDriverKeyNameError;
    }

    driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName);
    free(driverKeyNameW);

    return driverKeyNameA;


GetHCDDriverKeyNameError:
    if (driverKeyNameW != NULL)
    {
		ShowMessage("出错!\r\n");
        free(driverKeyNameW);
        driverKeyNameW = NULL;
    }

    return NULL;
}

PCHAR CTestUSBDlg::GetRootHubName(HANDLE HostController)
{
    BOOL                success;
    ULONG               nBytes;
    USB_ROOT_HUB_NAME   rootHubName;
    PUSB_ROOT_HUB_NAME  rootHubNameW;
    PCHAR               rootHubNameA;

    rootHubNameW = NULL;
    rootHubNameA = NULL;

   success = DeviceIoControl(HostController,
                              IOCTL_USB_GET_ROOT_HUB_NAME,
                              0,
                              0,
                              &rootHubName,
                              sizeof(rootHubName),
                              &nBytes,
                              NULL);

    if (!success)
    {
        goto GetRootHubNameError;
    }

    nBytes = rootHubName.ActualLength;

    rootHubNameW =(PUSB_ROOT_HUB_NAME) malloc(nBytes);

    if (rootHubNameW == NULL)
    {

        goto GetRootHubNameError;
    }

    success = DeviceIoControl(HostController,
                              IOCTL_USB_GET_ROOT_HUB_NAME,
                              NULL,
                              0,
                              rootHubNameW,
                              nBytes,
                              &nBytes,
                              NULL);

    if (!success)
    {
       goto GetRootHubNameError;
    }

    rootHubNameA = WideStrToMultiStr(rootHubNameW->RootHubName);
    free(rootHubNameW);

    return rootHubNameA;


GetRootHubNameError:
    if (rootHubNameW != NULL)
    {
        free(rootHubNameW);
        rootHubNameW = NULL;
		ShowMessage("出错!\r\n");
    }

    return NULL;
}

PCHAR CTestUSBDlg::GetDriverKeyName(HANDLE Hub, ULONG ConnectionIndex)
{
    BOOL                                success;
    ULONG                               nBytes;
    USB_NODE_CONNECTION_DRIVERKEY_NAME  driverKeyName;
    PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW;
    PCHAR                               driverKeyNameA;

    driverKeyNameW = NULL;
    driverKeyNameA = NULL;

    driverKeyName.ConnectionIndex = ConnectionIndex;

    success = DeviceIoControl(Hub,
                              IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                              &driverKeyName,
                              sizeof(driverKeyName),
                              &driverKeyName,
                              sizeof(driverKeyName),
                              &nBytes,
                              NULL);

    if (!success)
    {
        goto GetDriverKeyNameError;
    }

    nBytes = driverKeyName.ActualLength;

    if (nBytes <= sizeof(driverKeyName))
    {
        goto GetDriverKeyNameError;
    }

    driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)malloc(nBytes);

    if (driverKeyNameW == NULL)
    {
        goto GetDriverKeyNameError;
    }

    driverKeyNameW->ConnectionIndex = ConnectionIndex;

    success = DeviceIoControl(Hub,
                              IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                              driverKeyNameW,
                              nBytes,
                              driverKeyNameW,
                              nBytes,
                              &nBytes,
                              NULL);

    if (!success)
    {
        goto GetDriverKeyNameError;
    }
    driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName);
    free(driverKeyNameW);

    return driverKeyNameA;


GetDriverKeyNameError:
    if (driverKeyNameW != NULL)
    {
        free(driverKeyNameW);
        driverKeyNameW = NULL;
    }

    return NULL;
}

PUSB_DESCRIPTOR_REQUEST CTestUSBDlg::GetConfigDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex)
{
   BOOL    success;
    ULONG   nBytes;
    ULONG   nBytesReturned;

    UCHAR   configDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
                             sizeof(USB_CONFIGURATION_DESCRIPTOR)];

    PUSB_DESCRIPTOR_REQUEST         configDescReq;
    PUSB_CONFIGURATION_DESCRIPTOR   configDesc;


    nBytes = sizeof(configDescReqBuf);

    configDescReq = (PUSB_DESCRIPTOR_REQUEST)configDescReqBuf;
    configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq+1);

    memset(configDescReq, 0, nBytes);
    configDescReq->ConnectionIndex = ConnectionIndex;

    configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
                                        | DescriptorIndex;

    configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

    success = DeviceIoControl(hHubDevice,
                              IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
                              configDescReq,
                              nBytes,
                              configDescReq,
                              nBytes,
                              &nBytesReturned,
                              NULL);

    if (!success)
    {

        return NULL;
    }

    if (nBytes != nBytesReturned)
    {
        return NULL;
    }

    if (configDesc->wTotalLength < sizeof(USB_CONFIGURATION_DESCRIPTOR))
    {

        return NULL;
    }
    nBytes = sizeof(USB_DESCRIPTOR_REQUEST) + configDesc->wTotalLength;

    configDescReq = (PUSB_DESCRIPTOR_REQUEST)malloc(nBytes);

    if (configDescReq == NULL)
    {

        return NULL;
    }

    configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq+1);

    configDescReq->ConnectionIndex = ConnectionIndex;

    configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
                                        | DescriptorIndex;

    configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

    success = DeviceIoControl(hHubDevice,
                              IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
                              configDescReq,
                              nBytes,
                              configDescReq,
                              nBytes,
                              &nBytesReturned,
                              NULL);

    if (!success)
    {
        free(configDescReq);
        return NULL;
    }

    if (nBytes != nBytesReturned)
    {
        free(configDescReq);
        return NULL;
    }

    if (configDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
    {

        free(configDescReq);
        return NULL;
    }

    return configDescReq;
}

void CTestUSBDlg::ShowCenctInfo(PUSB_NODE_CONNECTION_INFORMATION connectionInfo)
{
		CString strShow;
		strShow.Format("connection index:[Port:%d]\r\n",connectionInfo->ConnectionIndex);
		ShowMessage(strShow);
			strShow.Format("Device class:0x%X,idVendor:0x%X,\r\nidProduct:0x%X,\r\n ",
				connectionInfo->DeviceDescriptor.bDeviceClass,
				connectionInfo->DeviceDescriptor.idVendor,
				connectionInfo->DeviceDescriptor.idProduct);
			ShowMessage(strShow);
			  strShow.Format("iManufacturer:0x%X,\r\niSerialNumber:0x%X,\r\n",
				  connectionInfo->DeviceDescriptor.iManufacturer,
				  connectionInfo->DeviceDescriptor.iSerialNumber);
			  ShowMessage(strShow);
			  if(connectionInfo->LowSpeed)
				  strShow="bus speed：low,\r\n";
			  else
				  strShow="bus speed：full,\r\n";
			  ShowMessage(strShow);
			  strShow.Format("Device Address:0x%X,\r\nOpen Pipes:0x%X\r\n",
			  connectionInfo->DeviceAddress,connectionInfo->NumberOfOpenPipes);
			  ShowMessage(strShow);

}

PCHAR CTestUSBDlg::WideStrToMultiStr(PWCHAR WideStr)
{
   ULONG nBytes;
    PCHAR MultiStr;
    nBytes = WideCharToMultiByte(
                 CP_ACP,
                 0,
                 WideStr,
                 -1,
                 NULL,
                 0,
                 NULL,
                 NULL);

    if (nBytes == 0)
    {
        return NULL;
    }
    MultiStr =(PCHAR) malloc(nBytes);

    if (MultiStr == NULL)
    {
        return NULL;
    }
    nBytes = WideCharToMultiByte(
                 CP_ACP,
                 0,
                 WideStr,
                 -1,
                 MultiStr,
                 nBytes,
                 NULL,
                 NULL);

    if (nBytes == 0)
    {
        free(MultiStr);
        return NULL;
    }

    return MultiStr;
}

void CTestUSBDlg::ShowHidInfo()//不能打开USB鼠标。能打开USB游戏手柄等
{
	CString strShow;
	strShow="正在查找系统中HID类的GUID标识...\r\n";
	ShowMessage(strShow);
	GUID guidHID;
	HidD_GetHidGuid(&guidHID);

	strShow="HID类的GUID标识为:\r\n";
	ShowMessage(strShow);
	strShow.Format("%08x-%04x-%04x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\r\n",
		guidHID.Data1,guidHID.Data2,guidHID.Data3,guidHID.Data4[0],
		guidHID.Data4[1],guidHID.Data4[2],guidHID.Data4[3],guidHID.Data4[4],
		guidHID.Data4[5],guidHID.Data4[6],guidHID.Data4[7]);
	ShowMessage(strShow);

		HDEVINFO hDevInfo = SetupDiGetClassDevs(&guidHID,NULL,0,
		DIGCF_PRESENT|DIGCF_DEVICEINTERFACE );
	
	if(hDevInfo==INVALID_HANDLE_VALUE)
	{
		ShowMessage("查找USB设备出错!\r\n");
		return;
	}
	else
		ShowMessage("找到USB设备.\r\n");
	UINT nIndex = 0;
	strShow= "正在查找可用的USB设备...\r\n";
	ShowMessage(strShow);

	SP_DEVICE_INTERFACE_DATA strtInterfaceData;

	strtInterfaceData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);
	BOOL bSuccess ;
	DWORD index=0;
	CString temp;
	for(int i=0;i<10;i++)
	{
	bSuccess= SetupDiEnumDeviceInterfaces(hDevInfo,NULL,&guidHID,index,
		&strtInterfaceData);
	if (!bSuccess)
	{	
		ShowMessage("查找USB设备出错!\r\n");
		break;
	}
	else
	{
		if(strtInterfaceData.Flags==SPINT_ACTIVE )
		{
			temp.Format("%d 连接 \r\n",index+1);
			ShowMessage(temp);
			ShowMore(hDevInfo,strtInterfaceData);
		}
	}
	index++;

	}
	temp.Format("总共%d个设备连接",index);
	ShowMessage(temp);
}

void CTestUSBDlg::ShowMore(HDEVINFO hDevInfo, SP_DEVICE_INTERFACE_DATA strtInterfaceData)
{
	CString strShow;
PSP_DEVICE_INTERFACE_DETAIL_DATA strtDetailData;

	DWORD strSzie=0,requiesize=0;
	SetupDiGetDeviceInterfaceDetail(hDevInfo,&strtInterfaceData,NULL,0,&strSzie,NULL);

	requiesize=strSzie;
	strtDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiesize);
	strtDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	SP_DEVINFO_DATA infodata;
	infodata.cbSize=sizeof(SP_DEVINFO_DATA);
	if (!SetupDiGetDeviceInterfaceDetail(hDevInfo,&strtInterfaceData,
		strtDetailData,strSzie,&requiesize,NULL))
	{
		ShowMessage("查找设备路径时出错!");
		SetupDiDestroyDeviceInfoList(hDevInfo);
		free(strtDetailData);
		return;
	}
	strShow.Format("设备路径:\r\n%s\r\n",strtDetailData->DevicePath);
	ShowMessage(strShow);
	ShowMessage("打开通信端口....\r\n");
		HANDLE hCom = CreateFile (
                  strtDetailData->DevicePath,
                  GENERIC_READ | GENERIC_WRITE,
                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL, 
                  OPEN_EXISTING, 0, 
                  NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
	ShowMessage("无法打开通信端口。不能读取设备信息!(设备连接可能不正确?)\r\n");
		SetupDiDestroyDeviceInfoList(hDevInfo);
		free(strtDetailData);
		return;
	}
	ShowMessage("打开通信端口成功！\r\n");
		HIDD_ATTRIBUTES strtAttrib;
	ShowMessage("正在读取设备的标识...\r\n");

	strtAttrib.Size=sizeof(HIDD_ATTRIBUTES);
	if (!HidD_GetAttributes(hCom,&strtAttrib))
	{
		ShowMessage("查询设备状态时出错!");
		CloseHandle(hCom);

		free(strtDetailData);
		return;	
	}

	ShowMessage("已读取，如下所示:\r\n");
	strShow.Format("VendorID:0x%X\r\n",strtAttrib.VendorID);
	ShowMessage(strShow);
	strShow.Format("ProductID:0x%X\r\n",strtAttrib.ProductID);
	ShowMessage(strShow);
	strShow.Format("VersionNumber:0x%X\r\n",strtAttrib.VersionNumber);
	ShowMessage(strShow);

}
BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	
	DWORD dwOutBytes;				
	BOOL bResult;					

	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	bResult = ::DeviceIoControl(hDevice,			
			IOCTL_STORAGE_QUERY_PROPERTY,			
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	
			pDevDesc, pDevDesc->Size,				
			&dwOutBytes,							
			(LPOVERLAPPED)NULL);					

	return bResult;
}
BOOL CTestUSBDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

char CTestUSBDlg::chFirstDriveFromMask(ULONG unitmask)
{
      char i;
      for (i = 0; i < 26; ++i)  
      {
           if (unitmask & 0x1) 
				break;
            unitmask = unitmask >> 1;
      }
    return (i + 'A');
}

void CTestUSBDlg::ShowUdisk()
{
	int k = 0;
	DWORD			MaxDriveSet, CurDriveSet;
	DWORD			drive, drivetype;
	TCHAR			szBuf[300];
	HANDLE			hDevice;
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;

		char* p;

		CString str;
	for(k=0; k<26; k++)
		szMoveDiskName[k] = '\0';	
	k = 1;		
	MaxDriveSet = CurDriveSet = 0;

	MaxDriveSet = GetLogicalDrives();
	CurDriveSet = MaxDriveSet;
	for ( drive = 0; drive < 32; ++drive )  
	{
		if ( MaxDriveSet & (1 << drive) )  
		{
			DWORD temp = 1<<drive;
			_stprintf( szDrvName, _T("%c:\\"), 'A'+drive );
			if(GetDriveType( szDrvName )== DRIVE_REMOVABLE)
			{
					drivetype = DRVREMOVE;

					sprintf(szBuf, "\\\\?\\%c:", 'A'+drive);
					hDevice = CreateFile(szBuf, GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

					if (hDevice != INVALID_HANDLE_VALUE)
					{

						pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];

						pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;

						if(GetDisksProperty(hDevice, pDevDesc))
						{
								szMoveDiskName[k] = chFirstDriveFromMask(temp);
								szMoveDiskName[0]=k;
								k++;
								ShowMessage("\r\n找到U盘，名称:\r\n");
								p= (char*)pDevDesc;
								str = (pDevDesc->VendorIdOffset ? &p[pDevDesc->VendorIdOffset]:"(NULL)");
								ShowMessage(str);

								ShowMessage("\r\n产品ID:\r\n");
								str=(pDevDesc->ProductIdOffset ? &p[pDevDesc->ProductIdOffset]:"(NULL)");
								ShowMessage(str);
								ShowMessage("\r\n产品版本:\r\n");
								str = (pDevDesc->ProductRevisionOffset ? &p[pDevDesc->ProductRevisionOffset] : "(NULL)");
								ShowMessage(str);
								ShowMessage("\r\n序列号:\r\n");
								str = (pDevDesc->SerialNumberOffset ? &p[pDevDesc->SerialNumberOffset] : "(NULL)");
								ShowMessage(str);
						}

						delete pDevDesc;
						CloseHandle(hDevice);
					}
			}
			}

		}
		str.Format("\r\n U盘符:\r\n%s",&szMoveDiskName[1]);

		if(szMoveDiskName[0]==0)
		{
			str="\r\n没有U盘\r\n";
			m_showMessage.Empty();
		}
	ShowMessage(str);
}

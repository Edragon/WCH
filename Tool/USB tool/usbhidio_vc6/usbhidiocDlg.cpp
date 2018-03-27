// usbhidiocDlg.cpp : implementation file
/*
Project: usbhidioc.cpp
Version: 3.0
Date: 7/18/05
by Jan Axelson (jan@Lvr.com)

Purpose: demonstrates USB communications with a HID-class device.

Description: 
	Finds an attached HID-class device that matches a specified Vendor ID and Product ID.
	Retrieves the HID's capabilities.
	Sends a report to the HID and receives a report from the HID.
	Supports Input, Output, and Feature reports. 
	A list box displays a log of activity.
	A list box displays the most recent received report bytes.
	Combo boxes enable the user to select bytes to send.
	Edit boxes enable the user to specify a Vendor ID and Product ID.
	A check box causes the application to increment the bytes sent
with each report.
	Clicking the Once button causes the application to exchange one set of reports.
	Clicking the Continuous button causes the application to exchange reports periodically
(every 5 seconds).
	A button enables setting the number of Input reports the HID buffer can store.
	
This application was created with Visual C++ 6's AppWizard as a dialog-based application.

Companion firmware and other sample code is available from www.Lvr.com.

Send comments, questions, bug reports, etc. to jan@Lvr.com.

About overlapped I/O

The API function ReadFile retrieves Input reports from the HID driver's buffer. 
Non-overlapped ReadFile is a blocking call. 
If a report isn't available, the function waits.

With overlapped I/O, the call to ReadFile returns immediately. The application then calls 
WaitForSingleObject, which returns when either the data has arrived or the specified timeout has elapsed.

This application has been tested on Windows 98 SE, Windows 2000, and Windows XP.
*/


#include "stdafx.h"

#include "usbhidioc.h"
#include "usbhidiocDlg.h"

#include <wtypes.h>
#include <initguid.h>

#define MAX_LOADSTRING 256

extern "C" {

// This file is in the Windows DDK available from Microsoft.
#include "hidsdi.h"

#include "setupapi.h"
#include <dbt.h>
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//function prototypes

BOOL DeviceNameMatch(LPARAM lParam);
bool FindTheHID();
LRESULT Main_OnDeviceChange(WPARAM wParam, LPARAM lParam);
void CloseHandles();
void DisplayCurrentTime();
void DisplayData(CString cstrDataToDisplay);
void DisplayFeatureReport();
void DisplayInputReport();
void DisplayLastError(CString Operation);
void DisplayReceivedData(char ReceivedByte);
void GetDeviceCapabilities();
void PrepareForOverlappedTransfer();
void ScrollToBottomOfListBox(USHORT idx);
void ReadAndWriteToDevice();
void ReadFeatureReport();
void ReadInputReport();
void RegisterForDeviceNotifications();
void WriteFeatureReport();
void WriteOutputReport();

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
// CUsbhidiocDlg dialog

CUsbhidiocDlg::CUsbhidiocDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbhidiocDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsbhidiocDlg)
	m_AutoIncrement = FALSE;
	m_ResultsString = _T("");
	m_strBytesReceived = _T("");
	m_strByteToSend0 = _T("");
	m_strByteToSend1 = _T("");
	m_ReportType = 0;
	m_UseControlTransfersOnly = FALSE;
	m_ProductIDString = _T("1299");
	m_VendorIDString = _T("0925");
	m_InputReportBufferSize = _T("32");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


//Application global variables 
	DWORD								ActualBytesRead;
	DWORD								BytesRead;
	HIDP_CAPS							Capabilities;
	DWORD								cbBytesRead;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
	HANDLE								DeviceHandle;
	DWORD								dwError;
	char								FeatureReport[256];
	HANDLE								hEventObject;
	HANDLE								hDevInfo;
	GUID								HidGuid;
	OVERLAPPED							HIDOverlapped;
	char								InputReport[256];
	ULONG								Length;
	LPOVERLAPPED						lpOverLap;
	bool								MyDeviceDetected = FALSE; 
	CString								MyDevicePathName;
	DWORD								NumberOfBytesRead;
	char								OutputReport[256];
	HANDLE								ReadHandle;
	DWORD								ReportType;
	ULONG								Required;
	CString								ValueToDisplay;
	HANDLE								WriteHandle;
	

	//These are the vendor and product IDs to look for.
	//Uses Lakeview Research's Vendor ID.
	int VendorID = 0x0925;
	int ProductID = 0x1299;


void CUsbhidiocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbhidiocDlg)
	DDX_Control(pDX, IDC_txtVendorID, m_VendorID);
	DDX_Control(pDX, IDC_txtProductID, m_ProductID);
	DDX_Control(pDX, IDC_Continuous, m_Continuous);
	DDX_Control(pDX, IDC_AutoIncrement, m_cbutAutoIncrement);
	DDX_Control(pDX, IDC_cboByteToSend1, m_cboByteToSend1);
	DDX_Control(pDX, IDC_cboByteToSend0, m_cboByteToSend0);
	DDX_Control(pDX, IDC_lstBytesReceived, m_BytesReceived);
	DDX_Control(pDX, IDC_LIST2, m_ResultsList);
	DDX_Control(pDX, IDC_Once, m_Once);
	DDX_Check(pDX, IDC_AutoIncrement, m_AutoIncrement);
	DDX_LBString(pDX, IDC_LIST2, m_ResultsString);
	DDX_LBString(pDX, IDC_lstBytesReceived, m_strBytesReceived);
	DDX_CBString(pDX, IDC_cboByteToSend0, m_strByteToSend0);
	DDX_CBString(pDX, IDC_cboByteToSend1, m_strByteToSend1);
	DDX_Radio(pDX, IDC_optExchangeInputAndOutputReports, m_ReportType);
	DDX_Check(pDX, IDC_chkUseControlTransfersOnly, m_UseControlTransfersOnly);
	DDX_Text(pDX, IDC_txtProductID, m_ProductIDString);
	DDX_Text(pDX, IDC_txtVendorID, m_VendorIDString);
	DDX_Text(pDX, IDC_txtInputReportBufferSize, m_InputReportBufferSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog)
	//{{AFX_MSG_MAP(CUsbhidiocDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Once, OnOnce)
	ON_EN_CHANGE(IDC_Results, OnChangeResults)
	ON_BN_CLICKED(IDC_Continuous, OnContinuous)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_cmdFindMyDevice, On_cmdFindMyDevice)
	ON_EN_CHANGE(IDC_txtVendorID, OnChange_txtVendorID)
	ON_EN_CHANGE(IDC_txtProductID, OnChange_txtProductID)
	ON_BN_CLICKED(IDC_optExchangeInputAndOutputReports, On_optExchangeInputAndOutputReports)
	ON_BN_CLICKED(IDC_optExchangeFeatureReports, On_optExchangeFeatureReports)
	ON_BN_CLICKED(IDC_chkUseControlTransfersOnly, On_chkUseControlTransfersOnly)
	ON_EN_CHANGE(IDC_txtInputReportBufferSize, OnChange_txtInputReportBufferSize)
	ON_BN_CLICKED(IDC_cmdInputReportBufferSize, On_cmdInputReportBufferSize)
	//}}AFX_MSG_MAP

	//ON_WM_DEVICECHANGE()
	ON_MESSAGE(WM_DEVICECHANGE, Main_OnDeviceChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg message handlers

BOOL CUsbhidiocDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//My declares begin here

	int		ByteToSend = 0;
	CString	strByteToSend = "";
	CString	strComboBoxText="";

	//End my declares

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
	
	/*
	My code begins here.
	Anything that needs to happen when the application starts goes in this routine.
	*/

	//Populate the combo boxes with values from 00 to FF.

	MyDeviceDetected=FALSE;

	for (ByteToSend=0; ByteToSend < 256; ByteToSend++)
	{
		//Display the value as a 2-digit Hex value.

		strByteToSend.Format("%.2X",ByteToSend);

		//Add the value to the combo boxes.

		m_cboByteToSend0.AddString(strByteToSend);
		m_cboByteToSend1.AddString(strByteToSend);	
	}

	m_ResultsList.InsertString(-1,"This program originated from Jan Axelson's Program usbhidio2");
	m_ResultsList.InsertString(-1,"Please visit http://www.lvr.com/hidpage.htm  for more information");
	m_ResultsList.InsertString(-1,"Modified by Yang.shunyong(Thomas) (dfysy@sina.com). Thanks Jan Axelson again");
	m_ResultsList.InsertString(-1,"Remove Confusing Report ID from Received Bytes.");
	//Select default values for the combo boxes.

	m_cboByteToSend0.SetCurSel(0);
	m_cboByteToSend1.SetCurSel(128);

	//Check the autoincrement check box.

	m_cbutAutoIncrement.SetCheck(1);

	//Set the caption for the Continous button.

	m_Continuous.SetWindowText("Continuous");

	//end my code

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUsbhidiocDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbhidiocDlg::OnPaint() 
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
HCURSOR CUsbhidiocDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
My routines (routines specific to this application) start here.
*/

void CUsbhidiocDlg::CloseHandles()
	{
	//Close open handles.
	
	if (DeviceHandle != INVALID_HANDLE_VALUE)
		{
		CloseHandle(DeviceHandle);
		}

	if (ReadHandle != INVALID_HANDLE_VALUE)
		{
		CloseHandle(ReadHandle);
		}

	if (WriteHandle != INVALID_HANDLE_VALUE)
		{
		CloseHandle(WriteHandle);
		}
	}


BOOL CUsbhidiocDlg::DeviceNameMatch(LPARAM lParam)
	{

	// Compare the device path name of a device recently attached or removed 
	// with the device path name of the device we want to communicate with.
	
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

	DisplayData("MyDevicePathName = " + MyDevicePathName);

	if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) 
	{
		
		PDEV_BROADCAST_DEVICEINTERFACE lpdbi = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;

		
		CString DeviceNameString;

		//The dbch_devicetype parameter indicates that the event applies to a device interface.
		//So the structure in LParam is actually a DEV_BROADCAST_INTERFACE structure, 
		//which begins with a DEV_BROADCAST_HDR.

		//The dbcc_name parameter of DevBroadcastDeviceInterface contains the device name. 
 
		//Compare the name of the newly attached device with the name of the device 
		//the application is accessing (myDevicePathName).

		DeviceNameString = lpdbi->dbcc_name;

		DisplayData("DeviceNameString = " + DeviceNameString);


		if ((DeviceNameString.CompareNoCase(MyDevicePathName)) == 0)

		{
			//The name matches.

            return true;
		}
		else
		{
            //It's a different device.

            return false;
    	}
		
	}
		else
		{
			return false;
		}	
}


void CUsbhidiocDlg::DisplayCurrentTime()
{
	//Get the current time and date and display them in the log List Box.

	CTime curTime = CTime::GetCurrentTime();
	CString CurrentTime = curTime.Format( "%H:%M:%S, %B %d, %Y" );
	DisplayData(CurrentTime);
}


void CUsbhidiocDlg::DisplayData(CString cstrDataToDisplay)
{
	//Display data in the log List Box

	USHORT	Index;
	Index=m_ResultsList.InsertString(-1, (LPCTSTR)cstrDataToDisplay);
	ScrollToBottomOfListBox(Index);
}


void CUsbhidiocDlg::DisplayFeatureReport()
{
	USHORT	ByteNumber;
	CHAR	ReceivedByte;
	
	//Display the received data in the log and the Bytes Received List boxes.
	//Start at the top of the List Box.

	m_BytesReceived.ResetContent();
	
	//Step through the received bytes and display each.

	for (ByteNumber=0; ByteNumber < Capabilities.FeatureReportByteLength; ByteNumber++)
	{
		//Get a byte.

		ReceivedByte = FeatureReport[ByteNumber];

		//Display it.

		DisplayReceivedData(ReceivedByte);
	}
}


void CUsbhidiocDlg::DisplayInputReport()
{
	USHORT	ByteNumber;
	CHAR	ReceivedByte;
	
	//Display the received data in the log and the Bytes Received List boxes.
	//Start at the top of the List Box.

	m_BytesReceived.ResetContent();
	
	//Step through the received bytes and display each.

	for (ByteNumber=0; ByteNumber < Capabilities.InputReportByteLength; ByteNumber++)
	{
		//Get a byte.
        
		ReceivedByte = InputReport[ByteNumber];

		//Display it.
        if(ByteNumber>0)
		DisplayReceivedData(ReceivedByte);
	}
}


void CUsbhidiocDlg::DisplayLastError(CString Operation)
{
	//Display a message and the last error in the log List Box.
	
	LPVOID lpMsgBuf;
	USHORT Index = 0;
	CString	strLastError = "";
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	//Display the last error.

	strLastError = Operation + (LPCTSTR)lpMsgBuf; 

	//Trim CR/LF from the error message.

	strLastError.TrimRight(); 

	Index = m_ResultsList.InsertString(-1, strLastError);
	ScrollToBottomOfListBox(Index);
	LocalFree(lpMsgBuf); 
}


void CUsbhidiocDlg::DisplayReceivedData(char ReceivedByte)
{
	//Display data received from the device.

	CString	strByteRead;

	//Convert the value to a 2-character Cstring.

	strByteRead.Format("%02X", ReceivedByte);
	strByteRead = strByteRead.Right(2); 

	//Display the value in the Bytes Received List Box.

	m_BytesReceived.InsertString(-1, strByteRead);

	//Display the value in the log List Box (optional).
	//MessageToDisplay.Format("%s%s", "Byte 0: ", strByteRead); 
	//DisplayData(MessageToDisplay);	
	//UpdateData(false);
}


bool CUsbhidiocDlg::FindTheHID()
{
	//Use a series of API calls to find a HID with a specified Vendor IF and Product ID.

	HIDD_ATTRIBUTES						Attributes;
	DWORD								DeviceUsage;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	bool								LastDevice = FALSE;
	int									MemberIndex = 0;
	LONG								Result;	
	CString								UsageDescription;

	Length = 0;
	detailData = NULL;
	DeviceHandle=NULL;

	/*
	API function: HidD_GetHidGuid
	Get the GUID for all system HIDs.
	Returns: the GUID in HidGuid.
	*/

	HidD_GetHidGuid(&HidGuid);	
	
	/*
	API function: SetupDiGetClassDevs
	Returns: a handle to a device information set for all installed devices.
	Requires: the GUID returned by GetHidGuid.
	*/
	
	hDevInfo=SetupDiGetClassDevs 
		(&HidGuid, 
		NULL, 
		NULL, 
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
		
	devInfoData.cbSize = sizeof(devInfoData);

	//Step through the available devices looking for the one we want. 
	//Quit on detecting the desired device or checking all available devices without success.

	MemberIndex = 0;
	LastDevice = FALSE;

	do
	{
		/*
		API function: SetupDiEnumDeviceInterfaces
		On return, MyDeviceInterfaceData contains the handle to a
		SP_DEVICE_INTERFACE_DATA structure for a detected device.
		Requires:
		The DeviceInfoSet returned in SetupDiGetClassDevs.
		The HidGuid returned in GetHidGuid.
		An index to specify a device.
		*/

		Result=SetupDiEnumDeviceInterfaces 
			(hDevInfo, 
			0, 
			&HidGuid, 
			MemberIndex, 
			&devInfoData);

		if (Result != 0)
		{
			//A device has been detected, so get more information about it.

			/*
			API function: SetupDiGetDeviceInterfaceDetail
			Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
			containing information about a device.
			To retrieve the information, call this function twice.
			The first time returns the size of the structure in Length.
			The second time returns a pointer to the data in DeviceInfoSet.
			Requires:
			A DeviceInfoSet returned by SetupDiGetClassDevs
			The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.
			
			The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
			This application doesn't retrieve or use the structure.			
			If retrieving the structure, set 
			MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
			and pass the structure's address.
			*/
			
			//Get the Length value.
			//The call will return with a "buffer too small" error which can be ignored.

			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&Length, 
				NULL);

			//Allocate memory for the hDevInfo structure, using the returned Length.

			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
			
			//Set cbSize in the detailData structure.

			detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.

			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				detailData, 
				Length, 
				&Required, 
				NULL);

			// Open a handle to the device.
			// To enable retrieving information about a system mouse or keyboard,
			// don't request Read or Write access for this handle.

			/*
			API function: CreateFile
			Returns: a handle that enables reading and writing to the device.
			Requires:
			The DevicePath in the detailData structure
			returned by SetupDiGetDeviceInterfaceDetail.
			*/

			DeviceHandle=CreateFile 
				(detailData->DevicePath, 
				0, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				0, 
				NULL);

			DisplayLastError("CreateFile: ");

			/*
			API function: HidD_GetAttributes
			Requests information from the device.
			Requires: the handle returned by CreateFile.
			Returns: a HIDD_ATTRIBUTES structure containing
			the Vendor ID, Product ID, and Product Version Number.
			Use this information to decide if the detected device is
			the one we're looking for.
			*/

			//Set the Size to the number of bytes in the structure.

			Attributes.Size = sizeof(Attributes);

			Result = HidD_GetAttributes 
				(DeviceHandle, 
				&Attributes);
			
			DisplayLastError("HidD_GetAttributes: ");
			
			//Is it the desired device?

			MyDeviceDetected = FALSE;
			

			if (Attributes.VendorID == VendorID)
			{
				if (Attributes.ProductID == ProductID)
				{
					//Both the Vendor ID and Product ID match.

					MyDeviceDetected = TRUE;
					MyDevicePathName = detailData->DevicePath;
					DisplayData("Device detected");

					//Register to receive device notifications.

					RegisterForDeviceNotifications();

					//Get the device's capablities.

					GetDeviceCapabilities();

					// Find out if the device is a system mouse or keyboard.
					
					DeviceUsage = (Capabilities.UsagePage * 256) + Capabilities.Usage;

					if (DeviceUsage == 0x102)
						{
						UsageDescription = "mouse";
						}
				
					if (DeviceUsage == 0x106)
						{
						UsageDescription = "keyboard";
						}

					if ((DeviceUsage == 0x102) | (DeviceUsage == 0x106)) 
						{
						DisplayData("");
						DisplayData("*************************");
						DisplayData("The device is a system " + UsageDescription + ".");
						DisplayData("Windows 2000 and Windows XP don't allow applications");
						DisplayData("to directly request Input reports from or "); 
						DisplayData("write Output reports to these devices.");
						DisplayData("*************************");
						DisplayData("");
						}

					// Get a handle for writing Output reports.

					WriteHandle=CreateFile 
						(detailData->DevicePath, 
						GENERIC_WRITE, 
						FILE_SHARE_READ|FILE_SHARE_WRITE, 
						(LPSECURITY_ATTRIBUTES)NULL,
						OPEN_EXISTING, 
						0, 
						NULL);

					DisplayLastError("CreateFile: ");

					// Prepare to read reports using Overlapped I/O.

					PrepareForOverlappedTransfer();

				} //if (Attributes.ProductID == ProductID)

				else
					//The Product ID doesn't match.

					CloseHandle(DeviceHandle);

			} //if (Attributes.VendorID == VendorID)

			else
				//The Vendor ID doesn't match.

				CloseHandle(DeviceHandle);

		//Free the memory used by the detailData structure (no longer needed).

		free(detailData);

		}  //if (Result != 0)

		else
			//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.

			LastDevice=TRUE;

		//If we haven't found the device yet, and haven't tried every available device,
		//try the next one.

		MemberIndex = MemberIndex + 1;

	} //do

	while ((LastDevice == FALSE) && (MyDeviceDetected == FALSE));

	if (MyDeviceDetected == FALSE)
		DisplayData("Device not detected");
	else
		DisplayData("Device detected");

	//Free the memory reserved for hDevInfo by SetupDiClassDevs.

	SetupDiDestroyDeviceInfoList(hDevInfo);
	DisplayLastError("SetupDiDestroyDeviceInfoList");

	return MyDeviceDetected;
}


void CUsbhidiocDlg::GetDeviceCapabilities()
{
	//Get the Capabilities structure for the device.

	PHIDP_PREPARSED_DATA	PreparsedData;

	/*
	API function: HidD_GetPreparsedData
	Returns: a pointer to a buffer containing the information about the device's capabilities.
	Requires: A handle returned by CreateFile.
	There's no need to access the buffer directly,
	but HidP_GetCaps and other API functions require a pointer to the buffer.
	*/

	HidD_GetPreparsedData 
		(DeviceHandle, 
		&PreparsedData);
	DisplayLastError("HidD_GetPreparsedData: ");

	/*
	API function: HidP_GetCaps
	Learn the device's capabilities.
	For standard devices such as joysticks, you can find out the specific
	capabilities of the device.
	For a custom device, the software will probably know what the device is capable of,
	and the call only verifies the information.
	Requires: the pointer to the buffer returned by HidD_GetPreparsedData.
	Returns: a Capabilities structure containing the information.
	*/
	
	HidP_GetCaps 
		(PreparsedData, 
		&Capabilities);
	DisplayLastError("HidP_GetCaps: ");

	//Display the capabilities

	ValueToDisplay.Format("%s%X", "Usage Page: ", Capabilities.UsagePage);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Input Report Byte Length: ", Capabilities.InputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Output Report Byte Length: ", Capabilities.OutputReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Feature Report Byte Length: ", Capabilities.FeatureReportByteLength);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Link Collection Nodes: ", Capabilities.NumberLinkCollectionNodes);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Input Button Caps: ", Capabilities.NumberInputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of InputValue Caps: ", Capabilities.NumberInputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of InputData Indices: ", Capabilities.NumberInputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Button Caps: ", Capabilities.NumberOutputButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Value Caps: ", Capabilities.NumberOutputValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Output Data Indices: ", Capabilities.NumberOutputDataIndices);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Button Caps: ", Capabilities.NumberFeatureButtonCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Value Caps: ", Capabilities.NumberFeatureValueCaps);
	DisplayData(ValueToDisplay);
	ValueToDisplay.Format("%s%d", "Number of Feature Data Indices: ", Capabilities.NumberFeatureDataIndices);
	DisplayData(ValueToDisplay);

	//No need for PreparsedData any more, so free the memory it's using.

	HidD_FreePreparsedData(PreparsedData);
	DisplayLastError("HidD_FreePreparsedData: ") ;
}


LRESULT CUsbhidiocDlg::Main_OnDeviceChange(WPARAM wParam, LPARAM lParam)  
	{
  
	//DisplayData("Device change detected.");

	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

	switch(wParam) 
		{
		// Find out if a device has been attached or removed.
		// If yes, see if the name matches the device path name of the device we want to access.

		case DBT_DEVICEARRIVAL:
			DisplayData("A device has been attached.");

			if (DeviceNameMatch(lParam))
			{
				DisplayData("My device has been attached.");
			}
		
			return TRUE; 
	
		case DBT_DEVICEREMOVECOMPLETE:
			DisplayData("A device has been removed.");

			if (DeviceNameMatch(lParam))
			{
				DisplayData("My device has been removed.");

				// Look for the device on the next transfer attempt.

				MyDeviceDetected = false;
			}
			return TRUE; 
	
		default:
			return TRUE; 
		} 
  }



void CUsbhidiocDlg::OnChangeResults() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}



void CUsbhidiocDlg::OnChange_txtInputReportBufferSize() 
{
}


void CUsbhidiocDlg::OnChange_txtProductID() 
{
	
	CString ProductIDtext;

	// Get the text in the edit box.

	CEdit* m_ProductID = (CEdit*) GetDlgItem(IDC_txtProductID);
	m_ProductID->GetWindowText(ProductIDtext); 

	// Convert the hex string in the edit box to an integer.

	ProductID = strtoul("0x" + ProductIDtext, 0, 16); 
	
	MyDeviceDetected=false;
}


void CUsbhidiocDlg::OnChange_txtVendorID() 
{
	
	CString VendorIDtext;

	// Get the text in the edit box.

	CEdit* m_VendorID = (CEdit*) GetDlgItem(IDC_txtVendorID);
	m_VendorID->GetWindowText(VendorIDtext); 

	// Convert the hex string in the edit box to an integer.

	VendorID = strtoul("0x" + VendorIDtext, 0, 16); 
	
	MyDeviceDetected=false;
}



void CUsbhidiocDlg::On_chkUseControlTransfersOnly() 
{
	// TODO: Add your control notification handler code here
	
}


void CUsbhidiocDlg::OnClose() 
{
	//Anything that needs to occur on closing the application goes here.
	//Free any resources used by previous API calls and still allocated.

	CDialog::OnClose();
}


void CUsbhidiocDlg::On_cmdFindMyDevice() 
{
	CUsbhidiocDlg::FindTheHID();
}


void CUsbhidiocDlg::OnContinuous() 
{
	//Click the Continuous button to
	//begin or stop requesting and sending periodic reports.

	CString Caption;

	//Find out whether Continuous is currently selected 
	//and take appropriate action.

	m_Continuous.GetWindowText(Caption);

	if (Caption == "Continuous")
	{
		//Enable periodic exchanges of reports.
		//Change the button caption.

		m_Continuous.SetWindowText("Stop Continuous");

		//Start by reading and writing one pair of reports.

		ReadAndWriteToDevice();

		//Enable the timer to cause periodic exchange of reports.
		//The second parameter is the number of milliseconds between report requests.

		SetTimer(ID_CLOCK_TIMER, 5000, NULL);
	}
	else
	{
		//Stop periodic exchanges of reports.
		//Change the button caption.

		m_Continuous.SetWindowText("Continuous");

		//Disable the timer.

		KillTimer(ID_CLOCK_TIMER);
	}
}


void CUsbhidiocDlg::OnOK() 
{
	CDialog::OnOK();
}


void CUsbhidiocDlg::OnOnce() 
{
	//Click the Once button to read and write one pair of reports.

	ReadAndWriteToDevice();
}


void CUsbhidiocDlg::On_cmdInputReportBufferSize() 
{

	// Change the number of reports the HID driver's buffer can store.
	// Read back and display the result.

	CString InputReportBufferSizeText;
	ULONG	InputReportBufferSize;
	boolean	result;

	// Look for the device if necessary.

	if (MyDeviceDetected==FALSE)
		{
		MyDeviceDetected=FindTheHID();
		}
	
	if (MyDeviceDetected==TRUE)
		{
		// Get the text in the edit box.

		CEdit* m_InputReportBufferSize = (CEdit*) GetDlgItem(IDC_txtInputReportBufferSize);
		m_InputReportBufferSize->GetWindowText(InputReportBufferSizeText); 

		// Convert the string in the edit box to an integer.

		InputReportBufferSize = strtoul(InputReportBufferSizeText, 0, 10); 	

		// Set the number of Input-report buffers.

		result = HidD_SetNumInputBuffers 
		   (DeviceHandle, 
		   InputReportBufferSize);

		DisplayLastError("HidD_SetNumInputBuffers: ");

		/// Retrieve the number of Input-report buffers.

		result = HidD_GetNumInputBuffers 
		   (DeviceHandle, 
		   &InputReportBufferSize);

		DisplayLastError("HidD_GetNumInputBuffers: ");

		// Display the result.

		if (result) 
			{
			SetDlgItemInt (IDC_txtInputReportBufferSize, InputReportBufferSize); 
			}
		else
			{

			// Windows 98 Gold doesn't support these API calls. 
			// The buffer size is always 2.

			SetDlgItemInt (IDC_txtInputReportBufferSize, 2); 
			}
	}
}


void CUsbhidiocDlg::On_optExchangeFeatureReports() 
{

	// Exchange Feature reports.

	ReportType = 1;
}


void CUsbhidiocDlg::On_optExchangeInputAndOutputReports() 
{
	// Exchange Input and Output reports.
	
	ReportType = 0;	
}


void CUsbhidiocDlg::OnTimer(UINT nIDEvent) 
{
	//The timer event.
	//Read and Write one pair of reports.

	ReadAndWriteToDevice();

	CDialog::OnTimer(nIDEvent);
}


void CUsbhidiocDlg::PrepareForOverlappedTransfer()
{
	//Get a handle to the device for the overlapped ReadFiles.

	ReadHandle=CreateFile 
		(detailData->DevicePath, 
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL);

	DisplayLastError("CreateFile (ReadHandle): ");

	//Get an event object for the overlapped structure.

	/*API function: CreateEvent
	Requires:
	  Security attributes or Null
	  Manual reset (true). Use ResetEvent to set the event object's state to non-signaled.
	  Initial state (true = signaled) 
	  Event object name (optional)
	Returns: a handle to the event object
	*/

	if (hEventObject == 0)
	{
		hEventObject = CreateEvent 
			(NULL, 
			TRUE, 
			TRUE, 
			"");
	DisplayLastError("CreateEvent: ") ;

	//Set the members of the overlapped structure.

	HIDOverlapped.hEvent = hEventObject;
	HIDOverlapped.Offset = 0;
	HIDOverlapped.OffsetHigh = 0;
	}
}


void CUsbhidiocDlg::ReadAndWriteToDevice()
{
	//If necessary, find the device and learn its capabilities.
	//Then send a report and request a report.

	//Clear the List Box (optional).
	//m_ResultsList.ResetContent();

	DisplayData("***HID Test Report***");
	DisplayCurrentTime();

	//If the device hasn't been detected already, look for it.

	if (MyDeviceDetected==FALSE)
		{
		MyDeviceDetected=FindTheHID();
		}
	
	// Do nothing if the device isn't detected.

	if (MyDeviceDetected==TRUE)
		{
		switch (ReportType)
		{
			case 0:
				{
				// Output and Input Reports

				//Write a report to the device.

				WriteOutputReport();

				//Read a report from the device.

				ReadInputReport();	
			
				break;
				}
			case 1:
				{
				// Feature reports

				//Write a report to the device.

				WriteFeatureReport();

				//Read a report from the device.

				ReadFeatureReport();

				break;
				}

			default:
				{
				break;
				}
			}
		} // else
}


void CUsbhidiocDlg::ReadFeatureReport()
{
	
	// Read a Feature report from the device.

	CString	ByteToDisplay = "";
	BOOLEAN	Result;
	
	//The first byte is the report number.
	FeatureReport[0]=0;

	//Read a report from the device.

		/*
		HidD_GetFeature
		Returns:
		True on success
		Requires: 
		A device handle returned by CreateFile.
		A buffer to hold the report.
		The report length returned by HidP_GetCaps in Capabilities.InputReportByteLength.
		*/
		
		if (DeviceHandle != INVALID_HANDLE_VALUE)
			{
				Result = HidD_GetFeature
				(DeviceHandle,
				FeatureReport,
				Capabilities.FeatureReportByteLength);

				DisplayLastError("HidD_GetFeature: ");
			}
				
			if (!Result)
			{
				//The read attempt failed, so close the handles, display a message,
				//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

				CloseHandles();
				DisplayData("Can't read from device");
				MyDeviceDetected = FALSE;
			}
			else
			{
				DisplayData("Received Feature report: ");
				
				DisplayFeatureReport();
			}
}


void CUsbhidiocDlg::ReadInputReport()
{

	// Retrieve an Input report from the device.

	CString	ByteToDisplay = "";

	DWORD	Result;
	
	//The first byte is the report number.
	InputReport[0]=0;

	// Find out if the "Use Control Transfers Only" check box is checked.

	UpdateData(true);

	if (m_UseControlTransfersOnly) 
	{

		//Read a report from the device using a control transfer.
	
		/*
		HidD_GetInputReport
		Returns:
		True on success
		Requires: 
		A device handle returned by CreateFile.
		A buffer to hold the report.
		The report length returned by HidP_GetCaps in Capabilities.InputReportByteLength.
		*/
		
		if (ReadHandle != INVALID_HANDLE_VALUE)
			{
			Result = HidD_GetInputReport
			(ReadHandle,
			InputReport,
			Capabilities.InputReportByteLength);

			DisplayLastError("HidD_GetInputReport: ");
			}
		else
			{
			Result = FALSE;
			}

		if (!Result)
			{
			//The read attempt failed, so close the handles, display a message,
			//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

			CloseHandles();
			DisplayData("Can't read from device");
			MyDeviceDetected = FALSE;
			}
		else
			{
			DisplayData("Received Input report: ");
			
			//Display the report data.

			DisplayInputReport();
		
			/*
			USHORT	ByteNumber;
			CHAR	ReceivedByte;
		
			//Display the received data in the log and the Bytes Received List boxes.
			//Start at the top of the List Box.

			m_BytesReceived.ResetContent();
	
			//Step through the received bytes and display each.

			for (ByteNumber=0; ByteNumber < Capabilities.InputReportByteLength; ByteNumber++)
			{
				//Get a byte.

				ReceivedByte = InputReport[ByteNumber];

				//Display it.

				DisplayReceivedData(ReceivedByte);
			}
			*/
		}
	} 

	else
	{
 	
	/*API call:ReadFile
	'Returns: the report in InputReport.
	'Requires: a device handle returned by CreateFile
	'(for overlapped I/O, CreateFile must be called with FILE_FLAG_OVERLAPPED),
	'the Input report length in bytes returned by HidP_GetCaps,
	'and an overlapped structure whose hEvent member is set to an event object.
	*/

	if (ReadHandle != INVALID_HANDLE_VALUE)
		{
		Result = ReadFile 
		(ReadHandle, 
		InputReport, 
		Capabilities.InputReportByteLength, 
		&NumberOfBytesRead,
		(LPOVERLAPPED) &HIDOverlapped); 
		}
 
	DisplayLastError("ReadFile: ") ;

	/*API call:WaitForSingleObject
	'Used with overlapped ReadFile.
	'Returns when ReadFile has received the requested amount of data or on timeout.
	'Requires an event object created with CreateEvent
	'and a timeout value in milliseconds.
	*/

	Result = WaitForSingleObject 
		(hEventObject, 
		6000);

	DisplayLastError("WaitForSingleObject: ") ;
 
	switch (Result)
	{
	case WAIT_OBJECT_0:
		{
		DisplayData("Received Input report,");
					
		break;
		}
	case WAIT_TIMEOUT:
		{
		ValueToDisplay.Format("%s", "ReadFile timeout");
		DisplayData(ValueToDisplay);
		//Cancel the Read operation.

		/*API call: CancelIo
		Cancels the ReadFile
        Requires the device handle.
        Returns non-zero on success.
		*/
		
		Result = CancelIo(ReadHandle);
		
		//A timeout may mean that the device has been removed. 
		//Close the device handles and set MyDeviceDetected = False 
		//so the next access attempt will search for the device.
		CloseHandles();
		DisplayData("Can't read from device");
		MyDeviceDetected = FALSE;
		break;
		}
	default:
		{
		ValueToDisplay.Format("%s", "Undefined error");

		//Close the device handles and set MyDeviceDetected = False 
		//so the next access attempt will search for the device.

		CloseHandles();
		DisplayData("Can't read from device");
		MyDeviceDetected = FALSE;
		break;
		}
	}

	/*
	API call: ResetEvent
	Sets the event object to non-signaled.
	Requires a handle to the event object.
	Returns non-zero on success.
	*/

	ResetEvent(hEventObject);

	//Display the report data.

	DisplayInputReport();

	}
}


void CUsbhidiocDlg::RegisterForDeviceNotifications()
{

	// Request to receive messages when a device is attached or removed.
	// Also see WM_DEVICECHANGE in BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog).

	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = HidGuid;

	DeviceNotificationHandle =
		RegisterDeviceNotification(m_hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

}


void CUsbhidiocDlg::ScrollToBottomOfListBox(USHORT Index)
{
	/* 
	Scroll to the bottom of the list box. 
	To do so, add a line and set it as the current selection,
	possibly scrolling the window.
	Then deselect the line, 
	leaving the list box scrolled to the bottom with nothing selected.
	*/

	m_ResultsList.SetCurSel( Index );
	m_ResultsList.SetCurSel( -1 );
}


void CUsbhidiocDlg::WriteFeatureReport()
{
	//Send a report to the device.

	DWORD	BytesWritten = 0;
	INT		Index =0;
	ULONG	Result;
	CString	strBytesWritten = "";

	//The first byte is the report number.

	FeatureReport[0]=0;

	//Can set the other report values here, or get them from the combo boxes.
	//OutputReport[1]=33;
	//OutputReport[2]=6;

	//Get the bytes to send from the combo boxes.
	
	//If Autoincrement is checked, increment the selection.

	if (m_cbutAutoIncrement.GetCheck()>0)
		{
		Index=m_cboByteToSend0.GetCurSel();
		Index=Index+1;
		m_cboByteToSend0.SetCurSel(Index);
		}

	if (m_cbutAutoIncrement.GetCheck()>0)
		{
		Index=m_cboByteToSend1.GetCurSel();
		Index=Index+1;
		m_cboByteToSend1.SetCurSel(Index);
		}

	//Get the values from the combo boxes.

	FeatureReport[1]=m_cboByteToSend0.GetCurSel();
	FeatureReport[2]=m_cboByteToSend1.GetCurSel();


	//The first byte is the report number.

	FeatureReport[0]=0;

	//Send a report to the device.

	/*
	HidD_SetFeature
	Sends a report to the device.
	Returns: success or failure.
	Requires:
	A device handle returned by CreateFile.
	A buffer that holds the report.
	The Output Report length returned by HidP_GetCaps,
	*/

	if (DeviceHandle != INVALID_HANDLE_VALUE)
		{
		Result = HidD_SetFeature
		(DeviceHandle,
		FeatureReport,
		Capabilities.FeatureReportByteLength);
	}

	DisplayLastError("HidD_SetFeature: ");

	if (!Result)
		{
		//The write attempt failed, so close the handles, display a message,
		//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

		CloseHandles();
		DisplayData("Can't write to device");
		MyDeviceDetected = FALSE;
		}
	else
		{
		DisplayData("A Feature report was written to the device.");
		}

}


void CUsbhidiocDlg::WriteOutputReport()
{
	//Send a report to the device.

	DWORD	BytesWritten = 0;
	INT		Index =0;
	ULONG	Result;
	CString	strBytesWritten = "";

	UpdateData(true);

	//The first byte is the report number.

	OutputReport[0]=0;

	//Can set the other report values here, or get them from the combo boxes.
	//OutputReport[1]=33;
	//OutputReport[2]=6;

	//Get the bytes to send from the combo boxes.
	
	//If Autoincrement is checked, increment the selection.

	if (m_cbutAutoIncrement.GetCheck()>0)
		{
		Index=m_cboByteToSend0.GetCurSel();
		Index=Index+1;
		m_cboByteToSend0.SetCurSel(Index);
		}

	if (m_cbutAutoIncrement.GetCheck()>0)
		{
		Index=m_cboByteToSend1.GetCurSel();
		Index=Index+1;
		m_cboByteToSend1.SetCurSel(Index);
		}	

	//Get the values from the combo boxes.

	OutputReport[1]=m_cboByteToSend0.GetCurSel();
	OutputReport[2]=m_cboByteToSend1.GetCurSel();


	//The first byte is the report number.

	OutputReport[0]=0;

	if (m_UseControlTransfersOnly)
		{

		//Send a report to the device.

		/*
		HidD_SetOutputReport
		Sends a report to the device.
		Returns: success or failure.
		Requires:
		The device handle returned by CreateFile.
		A buffer that holds the report.
		The Output Report length returned by HidP_GetCaps,
		*/

		if (WriteHandle != INVALID_HANDLE_VALUE)
			{
			Result = HidD_SetOutputReport
			(WriteHandle,
			OutputReport,
			Capabilities.OutputReportByteLength);

			DisplayLastError("HidD_SetOutputReport: ");
			}

		if (Result)
			{
			DisplayData("An Output report was written to the device.");
			}
		else
			{
			//The write attempt failed, so close the handles, display a message,
			//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

			CloseHandles();
			DisplayData("Can't write to device");
			MyDeviceDetected = FALSE;
			}

		}
	else
		{

		/*
		API Function: WriteFile
		Sends a report to the device.
		Returns: success or failure.
		Requires:
		A device handle returned by CreateFile.
		A buffer that holds the report.
		The Output Report length returned by HidP_GetCaps,
		A variable to hold the number of bytes written.
		*/

		if (WriteHandle != INVALID_HANDLE_VALUE)
			{
			Result = WriteFile 
			(WriteHandle, 
			OutputReport, 
			Capabilities.OutputReportByteLength, 
			&BytesWritten, 
			NULL);
		}

		//Display the result of the API call and the report bytes.

		DisplayLastError("WriteFile: ");

		if (!Result)
			{
			//The WriteFile failed, so close the handles, display a message,
			//and set MyDeviceDetected to FALSE so the next attempt will look for the device.

			CloseHandles();
			DisplayData("Can't write to device");
			MyDeviceDetected = FALSE;
			}
		else
			{
			DisplayData("An Output report was written to the device.");
			strBytesWritten.Format("%s%d", "Bytes Written: ", BytesWritten); 
			DisplayData(strBytesWritten);
			}
		}
}







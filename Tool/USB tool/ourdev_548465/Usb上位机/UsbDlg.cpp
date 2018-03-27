// UsbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Usb.h"
#include "UsbDlg.h"

#include "dbt.h"

extern "C" {
#include "hidsdi.h"
#include "setupapi.h"
}


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//���������ҵ����豸·��
CString MyDevPathName="";
//���������豸�Ƿ��Ѿ��ҵ�
BOOL MyDevFound=FALSE;

//���������������VID��PID���汾��
DWORD MyVid,MyPid,MyPvn;

//������������ݵ��豸���
HANDLE hReadHandle=INVALID_HANDLE_VALUE;
//��������д���ݵ��豸���
HANDLE hWriteHandle=INVALID_HANDLE_VALUE;

//���ڷ������ݵı�־
BOOL DataInSending=FALSE;

//���ͱ���Ļ�������1�ֽڱ���ID+8�ֽڱ������ݡ�
UCHAR WriteReportBuffer[512]={0};

//���ձ���Ļ�������1�ֽڱ���ID+8�ֽڱ������ݡ�
UCHAR ReadReportBuffer[512]={0};

//���ͱ����õ�OVERLAPPED��
OVERLAPPED WriteOverlapped;
//���ձ����õ�OVERLAPPED��
OVERLAPPED ReadOverlapped;

//ָ��������̵߳�ָ��
CWinThread * pReadReportThread;
//ָ��д�����̵߳�ָ��
CWinThread * pWriteReportThread;

//����ע���豸֪ͨ�¼��õĹ㲥�ӿڡ�
//Ҫʹ�øýṹ�壬��Ҫ��StdAfx.h�н��������#define WINVER 0x0500
DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;

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
// CUsbDlg dialog

CUsbDlg::CUsbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsbDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUsbDlg, CDialog)
	//{{AFX_MSG_MAP(CUsbDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Remove, OnRemove)
	ON_BN_CLICKED(IDC_BUTTON_Open, OnBUTTONOpen)
	ON_BN_CLICKED(IDC_BUTTON_Send, OnBUTTONSend)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_Close, OnBUTTONClose)
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbDlg message handlers

BOOL CUsbDlg::OnInitDialog()
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

void CUsbDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbDlg::OnPaint() 
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
HCURSOR CUsbDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//����Ϣ��ӵ���Ϣ��ʾ��ĺ���
void CUsbDlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
{
	CString str,str1;
	UINT i;
	CHAR SysTime[10];
 
	//�Ȼ�ȡԭ����ʾ���ı�
	GetDlgItemText(IDC_Receive,str);
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
	SetDlgItemText(IDC_Receive,str);
	//�������Զ����������һ��
	i=((CEdit*)GetDlgItem(IDC_Receive))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_Receive))->LineScroll(i,0);
}
//////////////////////////////End of function//////////////////////

void CUsbDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CUsbDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
    SetDlgItemText(IDC_Receive,"");
	
}

void CUsbDlg::OnBUTTONOpen() 
{
	// TODO: Add your control notification handler code here
	//����һ��GUID�Ľṹ��HidGuid������HID�豸�Ľӿ���GUID��
   GUID HidGuid;
 //����һ��DEVINFO�ľ��hDevInfoSet�������ȡ�����豸��Ϣ���Ͼ����
	HDEVINFO hDevInfoSet;
 //����MemberIndex����ʾ��ǰ�������ڼ����豸��0��ʾ��һ���豸��
	DWORD MemberIndex;
	//DevInterfaceData�����������豸�������ӿ���Ϣ
	SP_DEVICE_INTERFACE_DATA DevInterfaceData;
 //����һ��BOOL���������溯�������Ƿ񷵻سɹ�
	BOOL Result;
 //����һ��RequiredSize�ı���������������Ҫ������ϸ��Ϣ�Ļ��峤�ȡ�
	DWORD RequiredSize;
 //����һ��ָ���豸��ϸ��Ϣ�Ľṹ��ָ�롣
 PSP_DEVICE_INTERFACE_DETAIL_DATA	pDevDetailData;
 //����һ������������豸�ľ����
	HANDLE hDevHandle;
 //����һ��HIDD_ATTRIBUTES�Ľṹ������������豸�����ԡ�
	HIDD_ATTRIBUTES DevAttributes;

 //��ʼ���豸δ�ҵ�
	MyDevFound=FALSE;

	//��ȡ���ı��������õ�VID��PID��PVN��
	GetMyIDs();

	//��ʼ������д���Ϊ��Ч�����
	hReadHandle=INVALID_HANDLE_VALUE;
	hWriteHandle=INVALID_HANDLE_VALUE;

	//��DevInterfaceData�ṹ���cbSize��ʼ��Ϊ�ṹ���С
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);
	//��DevAttributes�ṹ���Size��ʼ��Ϊ�ṹ���С
	DevAttributes.Size=sizeof(DevAttributes);

	//����HidD_GetHidGuid������ȡHID�豸��GUID����������HidGuid�С�
	HidD_GetHidGuid(&HidGuid);

	//����HidGuid����ȡ�豸��Ϣ���ϡ�����Flags��������Ϊ
	//DIGCF_DEVICEINTERFACE|DIGCF_PRESENT��ǰ�߱�ʾʹ�õ�GUIDΪ
	//�ӿ���GUID�����߱�ʾֻ�о�����ʹ�õ��豸����Ϊ��������ֻ
	//�����Ѿ������ϵ��豸�����صľ��������hDevinfo�С�ע���豸
	//��Ϣ������ʹ����Ϻ�Ҫʹ�ú���SetupDiDestroyDeviceInfoList
	//���٣���Ȼ������ڴ�й©��
	hDevInfoSet=SetupDiGetClassDevs(&HidGuid,
		                            NULL,
							       	NULL,
						            DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
 
	AddToInfOut("��ʼ�����豸");
	//Ȼ����豸������ÿ���豸�����о٣�����Ƿ�������Ҫ�ҵ��豸
	//���ҵ�����ָ�����豸�������豸�Ѿ��������ʱ�����˳����ҡ�
	//����ָ���һ���豸������MemberIndex��Ϊ0��
	MemberIndex=0;
	while(1)
	{
		//����SetupDiEnumDeviceInterfaces���豸��Ϣ�����л�ȡ���Ϊ
		//MemberIndex���豸��Ϣ��
	 Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
		                                NULL,
			 						    &HidGuid,
				 					    MemberIndex,
					 				    &DevInterfaceData);

		//�����ȡ��Ϣʧ�ܣ���˵���豸�Ѿ�������ϣ��˳�ѭ����
		if(Result==FALSE) break;

  //��MemberIndexָ����һ���豸
		MemberIndex++;

		//�����ȡ��Ϣ�ɹ����������ȡ���豸����ϸ��Ϣ���ڻ�ȡ�豸
		//��ϸ��Ϣʱ����Ҫ��֪��������ϸ��Ϣ��Ҫ���Ļ���������ͨ��
		//��һ�ε��ú���SetupDiGetDeviceInterfaceDetail����ȡ����ʱ
		//�ṩ�������ͳ��ȶ�ΪNULL�Ĳ��������ṩһ������������Ҫ���
		//�������ı���RequiredSize��
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			                                   &DevInterfaceData,
											   NULL,
											   NULL,
											   &RequiredSize,
											   NULL);

		//Ȼ�󣬷���һ����СΪRequiredSize�����������������豸��ϸ��Ϣ��
		pDevDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
		if(pDevDetailData==NULL) //����ڴ治�㣬��ֱ�ӷ��ء�
		{
			MessageBox("�ڴ治��!");
			SetupDiDestroyDeviceInfoList(hDevInfoSet);
			return;
		}

		//������pDevDetailData��cbSizeΪ�ṹ��Ĵ�С��ע��ֻ�ǽṹ���С��
		//���������滺��������
		pDevDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//Ȼ���ٴε���SetupDiGetDeviceInterfaceDetail��������ȡ�豸��
		//��ϸ��Ϣ����ε�������ʹ�õĻ������Լ���������С��
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			                                      &DevInterfaceData,
												pDevDetailData,
												RequiredSize,
												NULL,
												NULL);

		//���豸·�����Ƴ�����Ȼ�����ٸո�������ڴ档
		MyDevPathName=pDevDetailData->DevicePath;
		free(pDevDetailData);

		//�������ʧ�ܣ��������һ���豸��
		if(Result==FALSE) continue;

		//������óɹ�����ʹ�ò�����д���ʵ�CreateFile����
		//����ȡ�豸�����ԣ�����VID��PID���汾�ŵȡ�
		//����һЩ��ռ�豸������USB���̣���ʹ�ö����ʷ�ʽ���޷��򿪵ģ�
		//��ʹ�ò�����д���ʵĸ�ʽ�ſ��Դ���Щ�豸���Ӷ���ȡ�豸�����ԡ�
             hDevHandle=CreateFile(MyDevPathName, 
			                     NULL,
                        FILE_SHARE_READ|FILE_SHARE_WRITE, 
                        NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

		//����򿪳ɹ������ȡ�豸���ԡ�
		if(hDevHandle!=INVALID_HANDLE_VALUE)
		{
			//��ȡ�豸�����Բ�������DevAttributes�ṹ����
			Result=HidD_GetAttributes(hDevHandle,
				                         &DevAttributes);

			//�رոոմ򿪵��豸
			CloseHandle(hDevHandle);

   //��ȡʧ�ܣ�������һ��
   if(Result==FALSE) continue;

			//�����ȡ�ɹ����������е�VID��PID�Լ��豸�汾����������Ҫ��
			//���бȽϣ������һ�µĻ�����˵������������Ҫ�ҵ��豸��
			if(DevAttributes.VendorID==MyVid) //���VID���
				if(DevAttributes.ProductID==MyPid) //����PID���
					if(DevAttributes.VersionNumber==MyPvn) //�����豸�汾�����
					{
						MyDevFound=TRUE; //�����豸�Ѿ��ҵ�
						AddToInfOut("�豸�Ѿ��ҵ�");

						//��ô��������Ҫ�ҵ��豸���ֱ�ʹ�ö�д��ʽ��֮������������
						//����ѡ��Ϊ�첽���ʷ�ʽ��
      //����ʽ���豸
      hReadHandle=CreateFile(MyDevPathName, 
			                          GENERIC_READ,
                             FILE_SHARE_READ|FILE_SHARE_WRITE, 
                             NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							 NULL);
						if(hReadHandle!=INVALID_HANDLE_VALUE)AddToInfOut("�����ʴ��豸�ɹ�");
						else AddToInfOut("�����ʴ��豸ʧ��");

						//д��ʽ���豸
      hWriteHandle=CreateFile(MyDevPathName, 
			                  GENERIC_WRITE,
                             FILE_SHARE_READ|FILE_SHARE_WRITE, 
                             NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							 NULL);
						if(hWriteHandle!=INVALID_HANDLE_VALUE)AddToInfOut("д���ʴ��豸�ɹ�");
						else AddToInfOut("д���ʴ��豸ʧ��");

						DataInSending=FALSE; //���Է�������

      //�ֶ������¼����ö������ָ̻߳����С���Ϊ����֮ǰ��û�е���
      //�����ݵĺ�����Ҳ�Ͳ��������¼��Ĳ�����������Ҫ���ֶ�����һ
      //���¼����ö������ָ̻߳����С�
						SetEvent(ReadOverlapped.hEvent);
      
						//��ʾ�豸��״̬��
//						SetDlgItemText(IDC_DS,"�豸�Ѵ�");

						//�ҵ��豸���˳�ѭ����������ֻ���һ��Ŀ���豸�����ҵ�����˳�
						//�����ˡ��������Ҫ�����е�Ŀ���豸���г����Ļ�����������һ��
						//���飬�ҵ���ͱ����������У�ֱ�������豸��������ϲ��˳�����
						break;
					}
		}
		//�����ʧ�ܣ��������һ���豸
		else continue;
	}

	//����SetupDiDestroyDeviceInfoList���������豸��Ϣ����
	SetupDiDestroyDeviceInfoList(hDevInfoSet);

 //����豸�Ѿ��ҵ�����ôӦ��ʹ�ܸ�������ť����ͬʱ��ֹ���豸��ť
	if(MyDevFound)
	{
		//��ֹ���豸������ʹ�ܹر��豸�������������
		GetDlgItem(IDC_BUTTON_Open)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Close)->EnableWindow(TRUE);
//		GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(TRUE);
	}
	else
	{
		AddToInfOut("�豸δ�ҵ�");
	}
	
}

BOOL CUsbDlg::OnBUTTONSend() 
{
	// TODO: Add your control notification handler code here
	BOOL Result;
	UINT LastError;
	UINT i;
	CString Str,str_send;


	GetDlgItemText(IDC_SEND,str_send);
 
	//����豸û���ҵ����򷵻�ʧ��
	if(MyDevFound==FALSE)
	{
        AddToInfOut("�豸δ�ҵ�");
		return FALSE;
	}

	//��������Ч����˵�����豸ʧ��
	if(hWriteHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("��Ч��д�������������Ǵ��豸ʱʧ��");
		return FALSE;
	}

	//����������ڷ����У��򷵻�ʧ��
	if(DataInSending==TRUE)
	{
		AddToInfOut("�������ڷ����У���ʱ���ܷ���");
		return FALSE;
	}

	//����Ҫ���ͱ��������
	WriteReportBuffer[0]=0x00;   //����IDΪ0
    
    for(int ii=0;ii<256;ii++)
    {
      WriteReportBuffer[ii]=ii;
    }
    
   
	

//	sscanf(str_send,"%x",&Value);



//	WriteReportBuffer[1] = str_send[0];

	//��ʾ�������ݵ���Ϣ
	AddToInfOut("�����������256�ֽ�");
	Str="";
	for(i=0;i<9;i++)
	{
		Str+=itos(WriteReportBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//�������ڷ��ͱ�־
 DataInSending=TRUE;

	//����WriteFile������������
	Result=WriteFile(hWriteHandle,
			           &WriteReportBuffer, 
					   9,
					   NULL,
					&WriteOverlapped);

	//�����������ʧ�ܣ�����������ʧ�ܣ�Ҳ������IO������
	if(Result==FALSE)
	{
        //��ȡ���������
		LastError=GetLastError();
		//���Ƿ������IO����
		if((LastError==ERROR_IO_PENDING)||(LastError==ERROR_SUCCESS))
		{
			return TRUE;
		}
		//�����Ǻ�������ʱ����������ʾ�������
		else
		{
			DataInSending=FALSE;
			AddToInfOut("����ʧ�ܣ�������룺"+itos(LastError));
   //���������Ϊ1��˵�����豸��֧�ָú�����
			if(LastError==1)
			{
				AddToInfOut("���豸��֧��WriteFile������", FALSE);
			}
			return FALSE;
		}
	}
	//���򣬺������سɹ�
	else
	{
		DataInSending=FALSE;
	 return TRUE;
	}
	
}

void CUsbDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
	OnBUTTONClose();  //�رմ���ǰ�ȹر��豸
	
	CDialog::OnClose();
}



//����������������ڲ�����һ����
void CUsbDlg::AddToInfOut(char *p, BOOL AddTime, BOOL NewLine)
{
	CString str=p;
	AddToInfOut(str,AddTime,NewLine);
}

//////////////////////////////End of function//////////////////////

//������ת��ΪCString
CString CUsbDlg::itos(INT value, INT radix)
{
	static CString Str;
	UCHAR strBuffer[20];

	Str=itoa(value,(char *)strBuffer,radix);
	if(radix==16)
	{
		if(Str.GetLength()==1)	//���ֻ��1λ���ݣ�����ǰ���3��0
		{
			Str="000"+Str;
		}
		if(Str.GetLength()==2)	//���ֻ��2λ���ݣ�����ǰ���2��0
		{
			Str="00"+Str;
		}
			if(Str.GetLength()==3)	//���ֻ��3λ���ݣ�����ǰ���1��0
		{
			Str="0"+Str;
		}
	}
	Str.MakeUpper();
	return Str;
}
//////////////////////////////End of function//////////////////////



//��������̡߳�����ʹ�õ����첽���ã�����ڵ���ReadFile
//����ʱ�ṩһ��Overlapped�Ľṹ���ýṹ�к���һ���¼���
//�����ƽʱ���¼��Ǵ������ź�״̬�ģ�����ȴ��¼��ĺ���
//�ͻᱻ���𣬴Ӷ����̱߳���������������ȷ���غ��¼���
//�������ָ̻߳����С�����鷵�ص��������Լ�����ID�Ƿ���
//ȷ���Ӷ����ý����ϸ����ص�״̬�����ڸú���������
//CMyUsbHidTestAppDlg�ࣨ���Ǹù������������ࣩ�еĳ�Ա������
//�����޷�ֱ�ӵ���CMyUsbHidTestAppDlg���еĳ�Ա������
//�ڴ������߳�ʱ��ͨ��pParam����������һ��thisָ�룬������pParam
//ǿ��ת��ΪCMyUsbHidTestAppDlg���ָ�뼴�ɷ���CMyUsbHidTestAppDlg��
//�еĳ�Ա������
UINT ReadReportThread(LPVOID pParam)
{
	CUsbDlg *pAppDlg;
	DWORD Length;
	UINT i;
	CString Str;

 //������pParamȡ������ת��ΪCMyUsbHidTestAppDlg��ָ�룬
	//�Թ�����������Ա������
  pAppDlg=(CUsbDlg*)pParam;
    
    
	//���߳��Ǹ���ѭ����ֱ�������˳�ʱ�������˳�
	while(1)
	{
		//�����¼�Ϊ��Ч״̬
		ResetEvent(ReadOverlapped.hEvent);
        
  //����豸�Ѿ��ҵ�
		if(MyDevFound==TRUE)
		{
			if(hReadHandle==INVALID_HANDLE_VALUE) //����������Ч
			{
				pAppDlg->AddToInfOut("��Ч�Ķ��������������Ǵ��豸ʱʧ��");
			}
			else  //���򣬾����Ч
			{
				//�����ReadFile��������9�ֽڵı�������
				ReadFile(hReadHandle,
						ReadReportBuffer,
						512,
						NULL,
						&ReadOverlapped);
			
			}

			//�ȴ��¼�����
			WaitForSingleObject(ReadOverlapped.hEvent,INFINITE);
             	
			//����ȴ��������豸���γ���Ҳ�ᵼ���¼�����������ʱMyDevFound
			//������Ϊ�٣�����������ж�MyDevFoundΪ�ٵĻ��ͽ�����һ��ѭ����
			if(MyDevFound==FALSE) continue;

			//����豸û�б����£�����ReadFile��������������ɡ�
			//ͨ��GetOverlappedResult��������ȡʵ�ʶ�ȡ�����ֽ�����
			GetOverlappedResult(hReadHandle,&ReadOverlapped,&Length,TRUE);

			//����ֽ�����Ϊ0���򽫶�����������ʾ����Ϣ����
			if(Length!=0)
			{
			 pAppDlg->AddToInfOut("��ȡ����"+pAppDlg->itos(Length)+"�ֽ�");
			 Str="";
			 for(i=0;i<Length;i++)
			 {
			 	Str+=pAppDlg->itos(ReadReportBuffer[i],16).Right(2)+" ";
			 }
			 pAppDlg->AddToInfOut(Str, FALSE);
			}

		
		}
		else
		{
			//�����̣߳�ֱ���´��¼�������
		 WaitForSingleObject(ReadOverlapped.hEvent,INFINITE);
		}
	}
	return 0;
}

//д������̣߳����̱߳Ƚϼ򵥣�ֻ�Ǽ򵥵صȴ��¼���������
//Ȼ������������ڷ��͵ı�־��
UINT WriteReportThread(LPVOID pParam)
{
	while(1)
	{
		//�����¼�Ϊ��Ч״̬
     ResetEvent(WriteOverlapped.hEvent);

		//�ȴ��¼�����
	 WaitForSingleObject(WriteOverlapped.hEvent,INFINITE);

		//����������ڷ��ͱ�־
		DataInSending=FALSE;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

void CUsbDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{   
	GUID HidGuid;
	CDialog::OnShowWindow(bShow, nStatus);

	((CEdit*)GetDlgItem(IDC_SEND))->SetLimitText(1);
	
	// TODO: Add your message handler code here
	//��ʼ��VID��PID��PVN������
	SetDlgItemText(IDC_VID,"0483");
	SetDlgItemText(IDC_PID,"5750");
	SetDlgItemText(IDC_PVN,"0200");
	
	//��ʼ��д����ʱ�õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	WriteOverlapped.Offset=0;
	WriteOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ��WriteFileʹ�ã���WriteFile���ʱ��
	//�����ø��¼�Ϊ����״̬��
	WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);


	//��ʼ��������ʱ�õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	ReadOverlapped.Offset=0;
	ReadOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ��ReadFileʹ�ã���ReadFile���ʱ��
	//�����ø��¼�Ϊ����״̬��
	ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//����д������̣߳����ڹ���״̬��
	pWriteReportThread=AfxBeginThread(WriteReportThread,
		                                this,
										THREAD_PRIORITY_NORMAL,
										0,
										CREATE_SUSPENDED,
										NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pWriteReportThread!=NULL)
	{
		pWriteReportThread->ResumeThread();
	}

	//����һ����������̣߳����ڹ���״̬��
 pReadReportThread=AfxBeginThread(ReadReportThread,
		                                this,
									THREAD_PRIORITY_NORMAL,
									0,
									CREATE_SUSPENDED,
									NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pReadReportThread!=NULL)
	{
		pReadReportThread->ResumeThread();
	}

	//��ȡHID�豸�Ľӿ���GUDI
 HidD_GetHidGuid(&HidGuid);
	//����DevBroadcastDeviceInterface�ṹ�壬����ע���豸�ı�ʱ��֪ͨ
 DevBroadcastDeviceInterface.dbcc_size=sizeof(DevBroadcastDeviceInterface);
 DevBroadcastDeviceInterface.dbcc_devicetype=DBT_DEVTYP_DEVICEINTERFACE;
 DevBroadcastDeviceInterface.dbcc_classguid=HidGuid;
	//ע���豸�ı�ʱ�յ�֪ͨ
	RegisterDeviceNotification(m_hWnd,
							   &DevBroadcastDeviceInterface,
							   DEVICE_NOTIFY_WINDOW_HANDLE);
}

//���ı����л�ȡ�����VID��PID��PVN
void CUsbDlg::GetMyIDs()
{
 //���ı����л�ȡVID�ţ�������MyVid�С�
	GetInputData(IDC_VID,"����ID(VID)�����ʽ�������������롣",MyVid);
 //���ı����л�ȡPID�ţ�������MyPid�С�
 GetInputData(IDC_PID,"��ƷID(PID)�����ʽ�������������롣",MyPid);
	//���ı����л�ȡPVN�ţ�������MyPvn�С�
	GetInputData(IDC_PVN,"��Ʒ�汾��(PVN)�����ʽ�������������롣",MyPvn);
}
//////////////////////////////End of function//////////////////////

//��ȡ�ı����е����ݲ�ת��Ϊ��ֵ
BOOL CUsbDlg::GetInputData(INT nID, CString ErrorMsg, DWORD &Value)
{
	CString InText;

	GetDlgItemText(nID,InText);	//��ȡ������ı�

	if((InText.GetLength()<=4)&&(sscanf(InText,"%x",&Value)))	//�ж��Ƿ�������ȷ
	{
		SetDlgItemText(nID,itos(Value,16));	//�����ı�
		return TRUE;
	}
	else	//���������ʾ�������룬���۽�������������
	{
		MessageBox(ErrorMsg,NULL,MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(nID)->SetFocus();
		((CEdit*)GetDlgItem(nID))->SetSel(0,-1);
		return FALSE;
	}
}

void CUsbDlg::OnBUTTONClose() 
{
	// TODO: Add your control notification handler code here
	


	//��������ݵľ��������Ч�������ر�֮
	if(hReadHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReadHandle);
		hReadHandle=INVALID_HANDLE_VALUE;
	}

	//�����豸״̬Ϊδ�ҵ�
	MyDevFound=FALSE;
	//�޸İ���ʹ�����
	GetDlgItem(IDC_BUTTON_Open)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Close)->EnableWindow(FALSE);

}

//�豸״̬�ı�ʱ�Ĵ�����
BOOL CUsbDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
 PDEV_BROADCAST_DEVICEINTERFACE pdbi;
	CString DevPathName;

	//dwData��һ��ָ��DEV_BROADCAST_DEVICEINTERFACE�ṹ���ָ�룬
	//�ڸýṹ���б������豸�����͡�·�����Ȳ�����ͨ��������ָ���豸
	//��·�����Ƚϣ��������ж��Ƿ�������ָ�����豸���»��߲����ˡ�
	pdbi=(PDEV_BROADCAST_DEVICEINTERFACE)dwData;

	switch(nEventType) //����nEventType�б������¼�������
	{
  //�豸�����¼�
		case DBT_DEVICEARRIVAL:
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //���淢��״̬�ı���豸��·����
				//�Ƚ��Ƿ�������ָ�����豸
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
			 	AddToInfOut("�豸������");
			 }
			}
			return TRUE;

  //�豸�γ��¼�
		case DBT_DEVICEREMOVECOMPLETE: 
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //���淢��״̬�ı���豸��·����
				//�Ƚ��Ƿ�������ָ�����豸
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
				 AddToInfOut("�豸���γ�");
					//�豸���γ���Ӧ�ùر��豸��������ڴ�״̬�Ļ�����ֹͣ����
					if(MyDevFound==TRUE)
					{
					 MyDevFound=FALSE;
			 	    OnBUTTONClose();
					}
				}
			}
			return TRUE;

		default:
			return TRUE;
	} 
}
//////////////////////////////End of function//////////////////////



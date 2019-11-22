/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

���ߣ�����ȦȦ
��������: 2008.07.22
�޸�����: 2008.07.23
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/
// MyUsbHidTestAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyUsbHidTestApp.h"
#include "MyUsbHidTestAppDlg.h"

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

UCHAR LedStatus=0;	//LED״̬
UCHAR KeyStatus=0;	//����״̬

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
UCHAR WriteReportBuffer[9];

//���ձ���Ļ�������1�ֽڱ���ID+8�ֽڱ������ݡ�
UCHAR ReadReportBuffer[9];

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
//////////////////////////////End of function//////////////////////

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//////////////////////////////End of function//////////////////////

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//////////////////////////////End of function//////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUsbHidTestAppDlg dialog

CMyUsbHidTestAppDlg::CMyUsbHidTestAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyUsbHidTestAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyUsbHidTestAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//////////////////////////////End of function//////////////////////

void CMyUsbHidTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUsbHidTestAppDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//////////////////////////////End of function//////////////////////

BEGIN_MESSAGE_MAP(CMyUsbHidTestAppDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUsbHidTestAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CLEAR_INF, OnClearInf)
	ON_BN_CLICKED(IDC_LED1, OnLed1)
	ON_BN_CLICKED(IDC_LED2, OnLed2)
	ON_BN_CLICKED(IDC_LED3, OnLed3)
	ON_BN_CLICKED(IDC_LED4, OnLed4)
	ON_BN_CLICKED(IDC_LED5, OnLed5)
	ON_BN_CLICKED(IDC_LED6, OnLed6)
	ON_BN_CLICKED(IDC_LED7, OnLed7)
	ON_BN_CLICKED(IDC_LED8, OnLed8)
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLEAR_COUNTER, OnClearCounter)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUsbHidTestAppDlg message handlers

BOOL CMyUsbHidTestAppDlg::OnInitDialog()
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
//SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//////////////////////////////End of function//////////////////////

void CMyUsbHidTestAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//////////////////////////////End of function//////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyUsbHidTestAppDlg::OnPaint() 
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
//////////////////////////////End of function//////////////////////

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyUsbHidTestAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//////////////////////////////End of function//////////////////////

void CMyUsbHidTestAppDlg::OnAbout() 
{
 CAboutDlg dlgAbout;
	dlgAbout.DoModal();
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
	CMyUsbHidTestAppDlg *pAppDlg;
	DWORD Length, Counter;
	UINT i;
	CString Str;

 //������pParamȡ������ת��ΪCMyUsbHidTestAppDlg��ָ�룬
	//�Թ�����������Ա������
 pAppDlg=(CMyUsbHidTestAppDlg*)pParam;

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
													9,
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

			//����ֽ���Ϊ9����˵����ȡ������ȷ��9�ֽڱ���
			if(Length==9)
			{
				//��һ�ֽ�Ϊ����ID��Ӧ��Ϊ0
				if(ReadReportBuffer[0]==0)
				{
					//�ڶ��ֽ�Ϊ����״̬�����䱣�浽KeyStatus�С�
					KeyStatus=ReadReportBuffer[1];

					//ˢ�°����������
					pAppDlg->SetKeyStatus();

					//��3��4��5��6�ֽ�Ϊ�豸���صķ��ʹ���ֵ�������ֵ����ʾ��
					Counter=ReadReportBuffer[5];
					Counter=(Counter<<8)+ReadReportBuffer[4];
					Counter=(Counter<<8)+ReadReportBuffer[3];
					Counter=(Counter<<8)+ReadReportBuffer[2];
					pAppDlg->SetCounterNumber(Counter);
				}
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
//////////////////////////////End of function//////////////////////

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

		//WriteReportBuffer[2]Ϊ��0ֵʱ�����豸������ļ���ֵ��
		//������������������ʱ���������ø�ֵΪ��0�������ݷ���
		//��Ϻ󣬽����Ļ�0�������ڷ���LED״̬ʱ���Ϳ��Բ���ȥ����
		//WriteReportBuffer[2]��ֵ�ˡ�
		WriteReportBuffer[2]=0;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//���ڽ�Ҫ��ʾʱ�ĺ���
void CMyUsbHidTestAppDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	HICON hIcon;
 GUID HidGuid;

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	//��ʼ��
	//LED����Ϩ��̬ͼ��
	hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
	GetDlgItem(IDC_LED1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);

	//��ֹLED��ť�����豸�����ʹ��
	GetDlgItem(IDC_LED1)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED3)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED5)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED6)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED7)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED8)->EnableWindow(FALSE);

	//���ؼ��ص���״̬ͼ��
	GetDlgItem(IDC_KEY1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);	

	//��ʼ��VID��PID��PVN������
	SetDlgItemText(IDC_VID_EDIT,"8888");
	SetDlgItemText(IDC_PID_EDIT,"0006");
	SetDlgItemText(IDC_PVN_EDIT,"0100");

	//����VID��PID��PVN�������ı����
	((CEdit*)GetDlgItem(IDC_VID_EDIT))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_PID_EDIT))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_PVN_EDIT))->SetLimitText(4);

	SetTimer(1,600,NULL);	//�����ʱ������ˢ��ͼ��

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
//////////////////////////////End of function//////////////////////

//����Ϣ��ӵ���Ϣ��ʾ��ĺ���
void CMyUsbHidTestAppDlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
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
}
//////////////////////////////End of function//////////////////////

//����������������ڲ�����һ����
void CMyUsbHidTestAppDlg::AddToInfOut(char *p, BOOL AddTime, BOOL NewLine)
{
	CString str=p;
	AddToInfOut(str,AddTime,NewLine);
}
//////////////////////////////End of function//////////////////////

void CMyUsbHidTestAppDlg::OnClearInf() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_INF_OUT,"");
}
//////////////////////////////End of function//////////////////////

//����LED��״̬��
BOOL CMyUsbHidTestAppDlg::SendLedStatus()
{
	BOOL Result;
	UINT LastError;
	UINT i;
	CString Str;
 
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
	WriteReportBuffer[1]=LedStatus;	//��LED״̬�ŵ���������

	//��ʾ�������ݵ���Ϣ
	AddToInfOut("�����������9�ֽ�");
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
			               WriteReportBuffer,
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
//////////////////////////////End of function//////////////////////

void CMyUsbHidTestAppDlg::SetLedStatus(INT nID, INT nLed)
{
 UCHAR Mask;
	HICON hIcon;

	//������ĳλLEDʱ����Ӧ��bitΪ1
	Mask=(1<<(nLed-1));
 
	//��ת��Ӧ��λ
	LedStatus^=Mask;

	//����LED״̬�ɹ�
	if(SendLedStatus()==TRUE)
	{
		//LED������״̬��ʹ������ͼƬ
		if(LedStatus&Mask)
		{
			hIcon=AfxGetApp()->LoadIcon(IDI_LED_ON);
			AddToInfOut("LED" + itos(nLed) + " ����");
		}
		//LED������״̬��ʹ�����ͼƬ
		else
		{
			hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
   AddToInfOut("LED" + itos(nLed) + " Ϩ��");
		}
  GetDlgItem(nID)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	}
	else
	{
		LedStatus^=Mask;  //û�����óɹ����Ļ�ԭ����ֵ
	}
}
//////////////////////////////End of function//////////////////////

//LED1��������
void CMyUsbHidTestAppDlg::OnLed1()
{
	SetLedStatus(IDC_LED1,1);
	return;
}
//////////////////////////////End of function//////////////////////

//LED2��������
void CMyUsbHidTestAppDlg::OnLed2() 
{
	SetLedStatus(IDC_LED2,2);
}
//////////////////////////////End of function//////////////////////

//LED3��������
void CMyUsbHidTestAppDlg::OnLed3() 
{
	SetLedStatus(IDC_LED3,3);
}
//////////////////////////////End of function//////////////////////

//LED4��������
void CMyUsbHidTestAppDlg::OnLed4() 
{
	SetLedStatus(IDC_LED4,4);
}
//////////////////////////////End of function//////////////////////

//LED5��������
void CMyUsbHidTestAppDlg::OnLed5() 
{
	SetLedStatus(IDC_LED5,5);
}
//////////////////////////////End of function//////////////////////

//LED6��������
void CMyUsbHidTestAppDlg::OnLed6() 
{
	SetLedStatus(IDC_LED6,6);
}
//////////////////////////////End of function//////////////////////

//LED7��������
void CMyUsbHidTestAppDlg::OnLed7() 
{
	SetLedStatus(IDC_LED7,7);
}
//////////////////////////////End of function//////////////////////

//LED8��������
void CMyUsbHidTestAppDlg::OnLed8() 
{
	SetLedStatus(IDC_LED8,8);
}
//////////////////////////////End of function//////////////////////

//��ʾ�������
void CMyUsbHidTestAppDlg::SetKeyStatus()
{
	HICON hIcon;
 UCHAR Mask;
	UINT i;
 UINT nIDs[8]=
	{
		IDC_KEY1,IDC_KEY2,IDC_KEY3,IDC_KEY4,
	 IDC_KEY5,IDC_KEY6,IDC_KEY7,IDC_KEY8
	};

	//������һ�ΰ��������
	static UCHAR LastKeyStatus=0x00;

	if(LastKeyStatus==KeyStatus)
	{
		return;		//����״̬û�иı䣬����Ҫ���°���״̬
	}

	//KEYi��������ı�
	for(i=0;i<8;i++)
	{
		Mask=(1<<i);
		if((LastKeyStatus^KeyStatus)&Mask)
		{
			if(KeyStatus&Mask)
			{
				hIcon=AfxGetApp()->LoadIcon(IDI_LED_ON);
				AddToInfOut("KEY" + itos(i+1) + "����");
			}
			else
			{
				hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
				AddToInfOut("KEY" + itos(i+1) +"����");
			}
			GetDlgItem(nIDs[i])->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
		}
 }

	//������󰴼������
	LastKeyStatus=KeyStatus;
}
//////////////////////////////End of function//////////////////////

//������豸��ť�Ĵ�����
void CMyUsbHidTestAppDlg::OnOpenDevice()
{
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
						SetDlgItemText(IDC_DS,"�豸�Ѵ�");

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
		GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(TRUE);

  //ʹ��LED���ư�����
		GetDlgItem(IDC_LED1)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED2)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED3)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED4)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED5)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED6)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED7)->EnableWindow(TRUE);
		GetDlgItem(IDC_LED8)->EnableWindow(TRUE);
	}
	else
	{
		AddToInfOut("�豸δ�ҵ�");
	}
}
//////////////////////////////End of function//////////////////////

//������ת��ΪCString
CString CMyUsbHidTestAppDlg::itos(INT value, INT radix)
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

//��ȡ�ı����е����ݲ�ת��Ϊ��ֵ
BOOL CMyUsbHidTestAppDlg::GetInputData(INT nID, CString ErrorMsg, DWORD &Value)
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
//////////////////////////////End of function//////////////////////

//���ı����л�ȡ�����VID��PID��PVN
void CMyUsbHidTestAppDlg::GetMyIDs()
{
 //���ı����л�ȡVID�ţ�������MyVid�С�
	GetInputData(IDC_VID_EDIT,"����ID(VID)�����ʽ�������������롣",MyVid);
 //���ı����л�ȡPID�ţ�������MyPid�С�
 GetInputData(IDC_PID_EDIT,"��ƷID(PID)�����ʽ�������������롣",MyPid);
	//���ı����л�ȡPVN�ţ�������MyPvn�С�
	GetInputData(IDC_PVN_EDIT,"��Ʒ�汾��(PVN)�����ʽ�������������롣",MyPvn);
}
//////////////////////////////End of function//////////////////////

//�����ر��豸��ť����Ӧ����
void CMyUsbHidTestAppDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	HICON hIcon;

	LedStatus=0;  //����LED�ر�
 KeyStatus=0;  //���а���̧��

	//��������ݵľ��������Ч�������ر�֮
	if(hReadHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReadHandle);
		hReadHandle=INVALID_HANDLE_VALUE;
	}
	//���д���ݵľ��������Ч���������������LEDΪ�ر�״̬��
	//Ȼ��رվ����
	if(hWriteHandle!=INVALID_HANDLE_VALUE)
	{
		SendLedStatus();
		CloseHandle(hWriteHandle);
		hWriteHandle=INVALID_HANDLE_VALUE;
	}

	//�����豸״̬Ϊδ�ҵ�
	MyDevFound=FALSE;

	//�޸İ���ʹ�����
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
	//����LEDΪ�ر�״̬
	hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
	GetDlgItem(IDC_LED1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	//��ֹLED
	GetDlgItem(IDC_LED1)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED3)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED5)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED6)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED7)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED8)->EnableWindow(FALSE);

 //���ð�������ʾ��״̬
	KeyStatus=0x00;
	SetKeyStatus();

	//��ʾ�ر���Ϣ
	AddToInfOut("�ر��豸");
 //��ʾ�豸��״̬
	SetDlgItemText(IDC_DS,"�豸�ѹر�");
}
//////////////////////////////End of function//////////////////////

//�����˳�����İ�ť��Ӧ
void CMyUsbHidTestAppDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	OnCloseDevice(); //�˳�����ǰ�ȹر��豸
 DestroyWindow(); //���ٴ���
}
//////////////////////////////End of function//////////////////////

//���ڹر�ʱ�Ĵ���
void CMyUsbHidTestAppDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnCloseDevice();  //�رմ���ǰ�ȹر��豸
	CDialog::OnClose();
}
//////////////////////////////End of function//////////////////////

//��ʾ��������ֵ
void CMyUsbHidTestAppDlg::SetCounterNumber(DWORD Counter)
{
	SetDlgItemText(IDC_COUNTER, "������ֵ��" + itos(Counter));
}
//////////////////////////////End of function//////////////////////

//��������������ťʱ����Ӧ����
void CMyUsbHidTestAppDlg::OnClearCounter() 
{
 //���������ĵڶ��ֽ�Ϊ��0ֵʱ�����������ֵ��
	WriteReportBuffer[2]=0xFF;
	if(SendLedStatus()==TRUE)
	{
		SetCounterNumber(0);
	}
}
//////////////////////////////End of function//////////////////////

//��ʱ���¼�����
void CMyUsbHidTestAppDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1)	//ˢ��ͼ��ʱ�䵽����������������Сͼ��
	{
		HICON hIcon;
		static UINT i=0;
		switch(i)
		{
		case 0:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON7);
			i++;
			break;
		case 1:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 2:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON2);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 3:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON3);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 4:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON4);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 5:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON5);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 6:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON6);
			SetIcon(hIcon,TRUE);
			i++;
			break;
		case 7:
			hIcon=AfxGetApp()->LoadIcon(IDI_ICON7);
			SetIcon(hIcon,TRUE);
			i=0;
			break;
		default:
			i=0;
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////End of function//////////////////////

//�豸״̬�ı�ʱ�Ĵ�����
LRESULT CMyUsbHidTestAppDlg::OnDeviceChange(WPARAM nEventType, LPARAM dwData)
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
			 	 OnCloseDevice();
					}
				}
			}
			return TRUE;

		default:
			return TRUE;
	} 
}
//////////////////////////////End of function//////////////////////

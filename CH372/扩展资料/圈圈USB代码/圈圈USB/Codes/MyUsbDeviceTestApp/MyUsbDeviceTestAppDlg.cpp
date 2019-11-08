/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

���ߣ�����ȦȦ
��������: 2008.08.22
�޸�����: 2008.08.27
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/
// MyUsbDeviceTestAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyUsbDeviceTestApp.h"
#include "MyUsbDeviceTestAppDlg.h"
#include "intrface.h"

#include "dbt.h"

extern "C" {
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

//����������豸�ľ��
HANDLE hMyDevHandle=INVALID_HANDLE_VALUE;

//������������ݵ��豸���
//HANDLE hReadHandle=INVALID_HANDLE_VALUE;
//��������д���ݵ��豸���
//HANDLE hWriteHandle=INVALID_HANDLE_VALUE;

//�˵�1���ڷ������ݵı�־
BOOL Ep1DataInSending=FALSE;
//�˵�2���ڷ������ݵı�־
BOOL Ep2DataInSending=FALSE;

//�˵�1�������ݵĻ�������16�ֽ����ݡ�
UCHAR Ep1WriteBuffer[16];

//�˵�1�������ݵĻ�������16�ֽ����ݡ�
UCHAR Ep1ReadBuffer[16];

//�˵�2�������ݵĻ�������64�ֽ����ݡ�
UCHAR Ep2WriteBuffer[64];

//�˵�2�������ݵĻ�������64�ֽ����ݡ�
UCHAR Ep2ReadBuffer[64];

//д�˵�1�õ�OVERLAPPED��
OVERLAPPED Ep1WriteOverlapped;
//���˵�1�õ�OVERLAPPED��
OVERLAPPED Ep1ReadOverlapped;

//д�˵�2�õ�OVERLAPPED��
OVERLAPPED Ep2WriteOverlapped;
//���˵�2�õ�OVERLAPPED��
OVERLAPPED Ep2ReadOverlapped;

//ָ����˵�1�̵߳�ָ��
CWinThread * pEp1ReadThread;
//ָ��д�˵�1�̵߳�ָ��
CWinThread * pEp1WriteThread;

//ָ����˵�2�̵߳�ָ��
CWinThread * pEp2ReadThread;
//ָ��д�˵�2�̵߳�ָ��
CWinThread * pEp2WriteThread;

//�ܷ����ֽ������ܽ����ֽ���
UINT TotalSent;
UINT TotalReceived;

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
// CMyUsbDeviceTestAppDlg dialog

CMyUsbDeviceTestAppDlg::CMyUsbDeviceTestAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyUsbDeviceTestAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyUsbDeviceTestAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//////////////////////////////End of function//////////////////////

void CMyUsbDeviceTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUsbDeviceTestAppDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//////////////////////////////End of function//////////////////////

BEGIN_MESSAGE_MAP(CMyUsbDeviceTestAppDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUsbDeviceTestAppDlg)
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
	ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUsbDeviceTestAppDlg message handlers

BOOL CMyUsbDeviceTestAppDlg::OnInitDialog()
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

void CMyUsbDeviceTestAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyUsbDeviceTestAppDlg::OnPaint() 
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
HCURSOR CMyUsbDeviceTestAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//////////////////////////////End of function//////////////////////

void CMyUsbDeviceTestAppDlg::OnAbout() 
{
 CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//////////////////////////////End of function//////////////////////

//���˵�1���ݵ��̡߳�����ʹ�õ����첽���ã�����ڵ���ReadFile
//����ʱ�ṩһ��Overlapped�Ľṹ���ýṹ�к���һ���¼���
//�����ƽʱ���¼��Ǵ������ź�״̬�ģ�����ȴ��¼��ĺ���
//�ͻᱻ���𣬴Ӷ����̱߳���������������ȷ���غ��¼���
//�������ָ̻߳����С�Ȼ���鷵�ص��������Ƿ���
//ȷ���Ӷ����ý����ϸ����ص�״̬�����ڸú���������
//CMyUsbDeviceTestAppDlg�ࣨ���Ǹù������������ࣩ�еĳ�Ա������
//�����޷�ֱ�ӵ���CMyUsbDeviceTestAppDlg���еĳ�Ա������
//�ڴ������߳�ʱ��ͨ��pParam����������һ��thisָ�룬������pParam
//ǿ��ת��ΪCMyUsbDeviceTestAppDlg���ָ�뼴�ɷ���CMyUsbDeviceTestAppDlg��
//�еĳ�Ա������
UINT Ep1ReadThread(LPVOID pParam)
{
	CMyUsbDeviceTestAppDlg *pAppDlg;
	DWORD Length, Counter;
	UINT i;
	CString Str;

 //������pParamȡ������ת��ΪCMyUsbDeviceTestAppDlg��ָ�룬
	//�Թ�����������Ա������
 pAppDlg=(CMyUsbDeviceTestAppDlg*)pParam;

	//���߳��Ǹ���ѭ����ֱ�������˳�ʱ�������˳�
	while(1)
	{
		//�����¼�Ϊ��Ч״̬
		ResetEvent(Ep1ReadOverlapped.hEvent);

  //����豸�Ѿ��ҵ�
		if(MyDevFound==TRUE)
		{
			if(hMyDevHandle==INVALID_HANDLE_VALUE) //����������Ч
			{
				pAppDlg->AddToInfOut("��Ч�Ķ�����������Ǵ��豸ʱʧ��");
			}
			else  //���򣬾����Ч
			{
				//�����ReadFile��������16�ֽڵ�����
				ReadFile(hMyDevHandle,
													Ep1ReadBuffer,
													16,
													NULL,
													&Ep1ReadOverlapped);
			}

			//�ȴ��¼�����
			WaitForSingleObject(Ep1ReadOverlapped.hEvent,INFINITE);

			//����ȴ��������豸���γ���Ҳ�ᵼ���¼�����������ʱMyDevFound
			//������Ϊ�٣�����������ж�MyDevFoundΪ�ٵĻ��ͽ�����һ��ѭ����
			if(MyDevFound==FALSE) continue;

			//����豸û�б����£�����ReadFile��������������ɡ�
			//ͨ��GetOverlappedResult��������ȡʵ�ʶ�ȡ�����ֽ�����
			GetOverlappedResult(hMyDevHandle,&Ep1ReadOverlapped,&Length,TRUE);

			//����ֽ�����Ϊ0���򽫶�����������ʾ����Ϣ����
			if(Length!=0)
			{
			 pAppDlg->AddToInfOut("�˵�1��ȡ����"+pAppDlg->itos(Length)+"�ֽ�");
			 Str="";
			 for(i=0;i<Length;i++)
			 {
			 	Str+=pAppDlg->itos(Ep1ReadBuffer[i],16).Right(2)+" ";
			 }
			 pAppDlg->AddToInfOut(Str, FALSE);
			}

			//����ֽ���Ϊ8����˵����ȡ������ȷ��8�ֽ���������
			if(Length==8)
			{
				//��һ�ֽ�Ϊ����״̬�����䱣�浽KeyStatus�С�
				KeyStatus=Ep1ReadBuffer[0];

				//ˢ�°����������
				pAppDlg->SetKeyStatus();

				//��2��3��4��5�ֽ�Ϊ�豸���صķ��ʹ���ֵ�������ֵ����ʾ��
				Counter=Ep1ReadBuffer[4];
				Counter=(Counter<<8)+Ep1ReadBuffer[3];
				Counter=(Counter<<8)+Ep1ReadBuffer[2];
				Counter=(Counter<<8)+Ep1ReadBuffer[1];
				pAppDlg->SetCounterNumber(Counter);
			}
		}
		else
		{
			//�����̣߳�ֱ���´��¼�������
		 WaitForSingleObject(Ep1ReadOverlapped.hEvent,INFINITE);
  }
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//�˵�1�ȴ�д���ݵ��̣߳����̱߳Ƚϼ򵥣�ֻ�Ǽ򵥵صȴ��¼���������
//Ȼ������������ڷ��͵ı�־��
UINT Ep1WriteThread(LPVOID pParam)
{
	while(1)
	{
		//�����¼�Ϊ��Ч״̬
  ResetEvent(Ep1WriteOverlapped.hEvent);

		//�ȴ��¼�����
	 WaitForSingleObject(Ep1WriteOverlapped.hEvent,INFINITE);

		//����������ڷ��ͱ�־
		Ep1DataInSending=FALSE;

		//EP1WriteBuffer[1]Ϊ��0ֵʱ�����豸������ļ���ֵ��
		//������������������ʱ���������ø�ֵΪ��0�������ݷ���
		//��Ϻ󣬽����Ļ�0�������ڷ���LED״̬ʱ���Ϳ��Բ���ȥ����
		//EP1WriteBuffer[1]��ֵ�ˡ�
		Ep1WriteBuffer[1]=0;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//�˵�2�����ݵ��߳�
UINT Ep2ReadThread(LPVOID pParam)
{
	CMyUsbDeviceTestAppDlg *pAppDlg;
	DWORD Length;
	UINT i;
	CString Str;

 //������pParamȡ������ת��ΪCMyUsbDeviceTestAppDlg��ָ�룬
	//�Թ�����������Ա������
 pAppDlg=(CMyUsbDeviceTestAppDlg*)pParam;

	//���߳��Ǹ���ѭ����ֱ�������˳�ʱ�������˳�
	while(1)
	{
		//�����¼�Ϊ��Ч״̬
		ResetEvent(Ep2ReadOverlapped.hEvent);

  //����豸�Ѿ��ҵ�
		if(MyDevFound==TRUE)
		{
			if(hMyDevHandle==INVALID_HANDLE_VALUE) //����������Ч
			{
				pAppDlg->AddToInfOut("��Ч�Ķ�����������Ǵ��豸ʱʧ��");
			}
			else  //���򣬾����Ч
			{
   	//ʹ��DeviceIoControl�Ӷ˵�2��ȡ64�ֽ�����
    DeviceIoControl(hMyDevHandle,	//���ǵ��豸
                 EP2_READ,		  //�����������IoControl�Ĺ��ܴ���
                 NULL,	          //���뻺�壬��
                 0,		            //�����ֽ���
                 Ep2ReadBuffer,  //�������
                 64,		           //����ֽ���
                 NULL,           //ʹ��Overlappedʱ�������ﱣ�洫���ֽ���
                 &Ep2ReadOverlapped);	 //ָ����Overlapped�ṹ
			}

			//�ȴ��¼�����
			WaitForSingleObject(Ep2ReadOverlapped.hEvent,INFINITE);

			//����ȴ��������豸���γ���Ҳ�ᵼ���¼�����������ʱMyDevFound
			//������Ϊ�٣�����������ж�MyDevFoundΪ�ٵĻ��ͽ�����һ��ѭ����
			if(MyDevFound==FALSE) continue;

			//����豸û�б����£�����ReadFile��������������ɡ�
			//ͨ��GetOverlappedResult��������ȡʵ�ʶ�ȡ�����ֽ�����
			GetOverlappedResult(hMyDevHandle,&Ep2ReadOverlapped,&Length,TRUE);

			//����ֽ�����Ϊ0���򽫶�����������ʾ����Ϣ����
			if(Length!=0)
			{
				TotalReceived+=Length;
    pAppDlg->SetDlgItemText(IDC_TOTAL_RECEIVED, pAppDlg->itos(TotalReceived)+"�ֽ�");
			 pAppDlg->AddToInfOut("�˵�2��ȡ����"+pAppDlg->itos(Length)+"�ֽ�");
			 Str="";
			 for(i=0;i<Length;i++)
			 {
			 	Str+=pAppDlg->itos(Ep2ReadBuffer[i],16).Right(2)+" ";
			 }
			 pAppDlg->AddToInfOut(Str, FALSE);
			}
		}
		else
		{
			//�����̣߳�ֱ���´��¼�������
		 WaitForSingleObject(Ep2ReadOverlapped.hEvent,INFINITE);
  }
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//�˵�2�ȴ�д���ݵ��̣߳����̱߳Ƚϼ򵥣�ֻ�Ǽ򵥵صȴ��¼���������
//Ȼ������������ڷ��͵ı�־��
UINT Ep2WriteThread(LPVOID pParam)
{
	ULONG Length;
 CMyUsbDeviceTestAppDlg * pAppDlg;

	pAppDlg=(CMyUsbDeviceTestAppDlg *)pParam;

	while(1)
	{
		//�����¼�Ϊ��Ч״̬
  ResetEvent(Ep2WriteOverlapped.hEvent);

		//�ȴ��¼�����
	 WaitForSingleObject(Ep2WriteOverlapped.hEvent,INFINITE);

  //ͨ��GetOverlappedResult��������ȡʵ�ʷ��͵��ֽ�����
		GetOverlappedResult(hMyDevHandle,&Ep2WriteOverlapped,&Length,TRUE);
		//�ۼ��ܷ����ֽ���

		TotalSent+=Length;
  //�����ܷ����ֽ���
  pAppDlg->SetDlgItemText(IDC_TOTAL_SENT, pAppDlg->itos(TotalSent)+"�ֽ�");

		//����������ڷ��ͱ�־
		Ep2DataInSending=FALSE;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//���ڽ�Ҫ��ʾʱ�ĺ���
void CMyUsbDeviceTestAppDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	HICON hIcon;
 //��ȡ�����Զ���USB�豸�Ľӿ���GUDI
 GUID MyGuid=GUID_DEVINTERFACE_COMPUTER00USB; //��interface.h�ļ��ж�����

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

	//���ð���ʹ�����
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND_DATA)->EnableWindow(FALSE);

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

	//��ʼ������������
	SetDlgItemText(IDC_DATA0,"01");
	SetDlgItemText(IDC_DATA1,"02");
	SetDlgItemText(IDC_DATA2,"03");
	SetDlgItemText(IDC_DATA3,"04");
	SetDlgItemText(IDC_DATA4,"05");
	SetDlgItemText(IDC_DATA5,"06");
	SetDlgItemText(IDC_DATA6,"07");
	SetDlgItemText(IDC_DATA7,"08");

	//���÷����������ı����
	((CEdit*)GetDlgItem(IDC_DATA0))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA1))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA2))->SetLimitText(2);
 ((CEdit*)GetDlgItem(IDC_DATA3))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA4))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA5))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA6))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA7))->SetLimitText(2);

	SetTimer(1,600,NULL);	//�����ʱ������ˢ��ͼ��

	//�ܷ��ͺ��ܽ����ֽ���Ϊ0
	TotalSent=0;
	TotalReceived=0;

	//��ʼ���˵�1д�����õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	Ep1WriteOverlapped.Offset=0;
	Ep1WriteOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ��WriteFileʹ�ã���WriteFile���ʱ��
	//�����ø��¼�Ϊ����״̬��
	Ep1WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//��ʼ���˵�1������ʱ�õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	Ep1ReadOverlapped.Offset=0;
	Ep1ReadOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ��ReadFileʹ�ã���ReadFile���ʱ��
	//�����ø��¼�Ϊ����״̬��
	Ep1ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//��ʼ���˵�2д�����õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	Ep2WriteOverlapped.Offset=0;
	Ep2WriteOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ�������˵�2��IoControlʹ�ã���IoControl���ʱ��
	//�����ø��¼�Ϊ����״̬��
	Ep2WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//��ʼ���˵�2������ʱ�õ�Overlapped�ṹ��
	//ƫ��������Ϊ0
	Ep2ReadOverlapped.Offset=0;
	Ep2ReadOverlapped.OffsetHigh=0;
	//����һ���¼����ṩ�������˵�2��IoControlʹ�ã���IoControl���ʱ��
	//�����ø��¼�Ϊ����״̬��
	Ep2ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//�����˵�1д���ݵ��̣߳����ڹ���״̬��
	pEp1WriteThread=AfxBeginThread(Ep1WriteThread,
		                              this,
																																THREAD_PRIORITY_NORMAL,
																																0,
																																CREATE_SUSPENDED,
																																NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pEp1WriteThread!=NULL)
	{
		pEp1WriteThread->ResumeThread();
	}

	//�����˵�1�����ݵ��̣߳����ڹ���״̬��
 pEp1ReadThread=AfxBeginThread(Ep1ReadThread,
		                             this,
																															THREAD_PRIORITY_NORMAL,
																															0,
																															CREATE_SUSPENDED,
																										 				NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pEp1ReadThread!=NULL)
	{
		pEp1ReadThread->ResumeThread();
	}

	//�����˵�2д���ݵ��̣߳����ڹ���״̬��
	pEp2WriteThread=AfxBeginThread(Ep2WriteThread,
		                              this,
																																THREAD_PRIORITY_NORMAL,
																																0,
																																CREATE_SUSPENDED,
																																NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pEp2WriteThread!=NULL)
	{
		pEp2WriteThread->ResumeThread();
	}

	//�����˵�2�����ݵ��̣߳����ڹ���״̬��
 pEp2ReadThread=AfxBeginThread(Ep2ReadThread,
		                             this,
																															THREAD_PRIORITY_NORMAL,
																															0,
																															CREATE_SUSPENDED,
																															NULL);
	//��������ɹ�����ָ����̵߳�����
	if(pEp2ReadThread!=NULL)
	{
		pEp2ReadThread->ResumeThread();
	}

	//����DevBroadcastDeviceInterface�ṹ�壬����ע���豸�ı�ʱ��֪ͨ
	DevBroadcastDeviceInterface.dbcc_size=sizeof(DevBroadcastDeviceInterface);
 DevBroadcastDeviceInterface.dbcc_devicetype=DBT_DEVTYP_DEVICEINTERFACE;
 DevBroadcastDeviceInterface.dbcc_classguid=MyGuid;
	//ע���豸�ı�ʱ�յ�֪ͨ
	RegisterDeviceNotification(m_hWnd,
							                    &DevBroadcastDeviceInterface,
																											DEVICE_NOTIFY_WINDOW_HANDLE);
	FindMyDevice(); //����һ���Ƿ�������ָ�����豸�Ѿ�������
	if(MyDevFound==TRUE) //����豸�Ѿ��ҵ�����ô��ʾ
	{
		AddToInfOut("�豸������");
		SetDlgItemText(IDC_DS,"�豸������");
	}
	else
	{
		AddToInfOut("�豸δ����");
		SetDlgItemText(IDC_DS,"�豸δ����");
	}
	MyDevFound=FALSE; //���ǲ���һ�£�����Ҫ��MyDevFound���û�FALSE��
}
//////////////////////////////End of function//////////////////////

//����Ϣ��ӵ���Ϣ��ʾ��ĺ���
void CMyUsbDeviceTestAppDlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
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
void CMyUsbDeviceTestAppDlg::AddToInfOut(char *p, BOOL AddTime, BOOL NewLine)
{
	CString str=p;
	AddToInfOut(str,AddTime,NewLine);
}
//////////////////////////////End of function//////////////////////

void CMyUsbDeviceTestAppDlg::OnClearInf() 
{
	// TODO: Add your control notification handler code here
	TotalSent=0;
	TotalReceived=0;
	SetDlgItemText(IDC_TOTAL_SENT, itos(TotalSent)+"�ֽ�");
 SetDlgItemText(IDC_TOTAL_RECEIVED, itos(TotalReceived)+"�ֽ�");
	SetDlgItemText(IDC_INF_OUT,"");
}
//////////////////////////////End of function//////////////////////

//����LED��״̬��
BOOL CMyUsbDeviceTestAppDlg::SendLedStatus()
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
	if(hMyDevHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("��Ч���豸����������Ǵ��豸ʱʧ��");
		return FALSE;
	}

	//����˵�1�������ڷ����У��򷵻�ʧ��
	if(Ep1DataInSending==TRUE)
	{
		AddToInfOut("�˵�1�������ڷ����У���ʱ���ܷ���");
		return FALSE;
	}

	//����Ҫ���͵�����
	Ep1WriteBuffer[0]=LedStatus;	//��LED״̬�ŵ���������

	//��ʾ�������ݵ���Ϣ
	AddToInfOut("�˵�1��������8�ֽ�");
	Str="";
	for(i=0;i<8;i++)
	{
		Str+=itos(Ep1WriteBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//�������ڷ��ͱ�־
 Ep1DataInSending=TRUE;

	//����WriteFile������������
	Result=WriteFile(hMyDevHandle,
			               Ep1WriteBuffer,
											       8,
											       NULL,
										       	&Ep1WriteOverlapped);

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
			Ep1DataInSending=FALSE;
			AddToInfOut("�˵�1����ʧ�ܣ�������룺"+itos(LastError));
			return FALSE;
		}
	}
	//���򣬺������سɹ�
	else
	{
		Ep1DataInSending=FALSE;
	 return TRUE;
	}
}
//////////////////////////////End of function//////////////////////

void CMyUsbDeviceTestAppDlg::SetLedStatus(INT nID, INT nLed)
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
void CMyUsbDeviceTestAppDlg::OnLed1()
{
	SetLedStatus(IDC_LED1,1);
	return;
}
//////////////////////////////End of function//////////////////////

//LED2��������
void CMyUsbDeviceTestAppDlg::OnLed2() 
{
	SetLedStatus(IDC_LED2,2);
}
//////////////////////////////End of function//////////////////////

//LED3��������
void CMyUsbDeviceTestAppDlg::OnLed3() 
{
	SetLedStatus(IDC_LED3,3);
}
//////////////////////////////End of function//////////////////////

//LED4��������
void CMyUsbDeviceTestAppDlg::OnLed4() 
{
	SetLedStatus(IDC_LED4,4);
}
//////////////////////////////End of function//////////////////////

//LED5��������
void CMyUsbDeviceTestAppDlg::OnLed5() 
{
	SetLedStatus(IDC_LED5,5);
}
//////////////////////////////End of function//////////////////////

//LED6��������
void CMyUsbDeviceTestAppDlg::OnLed6() 
{
	SetLedStatus(IDC_LED6,6);
}
//////////////////////////////End of function//////////////////////

//LED7��������
void CMyUsbDeviceTestAppDlg::OnLed7() 
{
	SetLedStatus(IDC_LED7,7);
}
//////////////////////////////End of function//////////////////////

//LED8��������
void CMyUsbDeviceTestAppDlg::OnLed8() 
{
	SetLedStatus(IDC_LED8,8);
}
//////////////////////////////End of function//////////////////////

//��ʾ�������
void CMyUsbDeviceTestAppDlg::SetKeyStatus()
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

//�����豸�ĺ��������ָ�����豸�ҵ���������MyDevFoundΪTRUE��
//�����豸��·��������MyDevPathName�С����ָ�����豸δ�ҵ�����
//����MyDevFoundΪFALSE��
void CMyUsbDeviceTestAppDlg::FindMyDevice()
{
 //����һ��GUID�Ľṹ��MyGuid����ʼ��Ϊ�����Զ���USB�豸�Ľӿ���GUID��
 GUID MyGuid=GUID_DEVINTERFACE_COMPUTER00USB; //��interface.h�ļ��ж�����
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

 //��ʼ���豸δ�ҵ�
	MyDevFound=FALSE;

	//��DevInterfaceData�ṹ���cbSize��ʼ��Ϊ�ṹ���С
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);

	//����MyGuid����ȡ�豸��Ϣ���ϡ�����Flags��������Ϊ
	//DIGCF_DEVICEINTERFACE|DIGCF_PRESENT��ǰ�߱�ʾʹ�õ�GUIDΪ
	//�ӿ���GUID�����߱�ʾֻ�о�����ʹ�õ��豸����Ϊ��������ֻ
	//�����Ѿ������ϵ��豸�����صľ��������hDevinfo�С�ע���豸
	//��Ϣ������ʹ����Ϻ�Ҫʹ�ú���SetupDiDestroyDeviceInfoList
	//���٣���Ȼ������ڴ�й©��
	hDevInfoSet=SetupDiGetClassDevs(&MyGuid,
		                               NULL,
																			            		NULL,
																					            DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
 
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
			 																							       			&MyGuid,
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

		//����˵�����óɹ����豸�Ѿ��ҵ�
		MyDevFound=TRUE; //�����豸�Ѿ��ҵ�

		//�ҵ��豸���˳�ѭ����������ֻ���һ��Ŀ���豸�����ҵ�����˳�
		//�����ˡ��������Ҫ�����е�Ŀ���豸���г����Ļ�����������һ��
		//���飬�ҵ���ͱ����������У�ֱ�������豸��������ϲ��˳�����
		break;
	}
	//����SetupDiDestroyDeviceInfoList���������豸��Ϣ����
	SetupDiDestroyDeviceInfoList(hDevInfoSet);
}
//////////////////////////////End of function//////////////////////

//������豸��ť�Ĵ�����
void CMyUsbDeviceTestAppDlg::OnOpenDevice()
{
	//��ʼ���豸���Ϊ��Ч�����
	hMyDevHandle=INVALID_HANDLE_VALUE;
 
 //��ʾ��ʼ�����豸
	AddToInfOut("��ʼ�����豸");

	//����FindMyDevice���������豸������ҵ���������
	//MyDevFoundΪTRUE������·��������MyDevPathName�С�
	//���δ�ҵ���������MyDevFoundΪFALSE��
	FindMyDevice();

	if(MyDevFound==TRUE) //����豸�ҵ�
	{
  AddToInfOut("�豸�Ѿ��ҵ�");
		//���Զ�д��ʽ���豸��ѡ��Ϊ�첽���ʷ�ʽ����������������
  hMyDevHandle=CreateFile(MyDevPathName, 
			                      GENERIC_READ|GENERIC_WRITE,
                         FILE_SHARE_READ|FILE_SHARE_WRITE, 
                         NULL,
																								 OPEN_EXISTING,
																								 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
																								 NULL);
		
		if(hMyDevHandle!=INVALID_HANDLE_VALUE)
		{
			AddToInfOut("���豸�ɹ�");
		}
		else 
		{
			if(GetLastError()==0x20)AddToInfOut("���豸ʧ�ܣ��豸�Ѿ�����");
			else AddToInfOut("���豸ʧ��");
			MyDevFound=FALSE; //����Ϊδ�ҵ�״̬
  }
	}
	else //�豸δ�ҵ�
	{
		AddToInfOut("�豸δ�ҵ�");
	}

	if(MyDevFound==TRUE) //����ҵ����򿪳ɹ�
	{
		Ep1DataInSending=FALSE; //���Է�������
		Ep2DataInSending=FALSE;

  //�ֶ������¼����ö��˵�1��2�ָ̻߳����С���Ϊ����֮ǰ��û�е���
  //�����ݵĺ�����Ҳ�Ͳ��������¼��Ĳ�����������Ҫ���ֶ�����һ
  //���¼����ö��˵�1��2�ָ̻߳����С�
		SetEvent(Ep1ReadOverlapped.hEvent);
  SetEvent(Ep2ReadOverlapped.hEvent);

		//��ʾ�豸��״̬��
		SetDlgItemText(IDC_DS,"�豸�Ѵ�");

  //ʹ�ܸ�������ť����ͬʱ��ֹ���豸��ť
		//��ֹ���豸������ʹ�ܹر��豸�������������
		GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(TRUE);
  GetDlgItem(IDC_SEND_DATA)->EnableWindow(TRUE);

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
}
//////////////////////////////End of function//////////////////////

//������ת��ΪCString
CString CMyUsbDeviceTestAppDlg::itos(INT value, INT radix)
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
BOOL CMyUsbDeviceTestAppDlg::GetInputData(INT nID, CString ErrorMsg, UCHAR &Value)
{
	CString InText;

	GetDlgItemText(nID,InText);	//��ȡ������ı�

	if((InText.GetLength()<=2)&&(sscanf(InText,"%x",&Value)))	//�ж��Ƿ�������ȷ
	{
		SetDlgItemText(nID,itos(Value,16).Right(2));	//�����ı�
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

//�����ر��豸��ť����Ӧ����
void CMyUsbDeviceTestAppDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	HICON hIcon;

	LedStatus=0;  //����LED�ر�
 KeyStatus=0;  //���а���̧��

	//����豸���������Ч�ģ�����������LEDΪ�ر�״̬��Ȼ��رվ����
	if(hMyDevHandle!=INVALID_HANDLE_VALUE)
	{
		SendLedStatus();
		CloseHandle(hMyDevHandle);
		hMyDevHandle=INVALID_HANDLE_VALUE;
	}
 
	Ep1DataInSending=TRUE; //���ܷ�������
	Ep2DataInSending=TRUE;

	//�����豸״̬Ϊδ�ҵ�
	MyDevFound=FALSE;

	//�޸İ���ʹ�����
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND_DATA)->EnableWindow(FALSE);
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
void CMyUsbDeviceTestAppDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	OnCloseDevice(); //�˳�����ǰ�ȹر��豸
 DestroyWindow(); //���ٴ���
}
//////////////////////////////End of function//////////////////////

//���ڹر�ʱ�Ĵ���
void CMyUsbDeviceTestAppDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnCloseDevice();  //�رմ���ǰ�ȹر��豸
	CDialog::OnClose();
}
//////////////////////////////End of function//////////////////////

//��ʾ��������ֵ
void CMyUsbDeviceTestAppDlg::SetCounterNumber(DWORD Counter)
{
	SetDlgItemText(IDC_COUNTER, "������ֵ��" + itos(Counter));
}
//////////////////////////////End of function//////////////////////

//��������������ťʱ����Ӧ����
void CMyUsbDeviceTestAppDlg::OnClearCounter() 
{
 //���˵�1������ݵĵڶ��ֽ�Ϊ��0ֵʱ�����������ֵ��
	Ep1WriteBuffer[1]=0xFF;
	if(SendLedStatus()==TRUE)
	{
		SetCounterNumber(0);
	}
}
//////////////////////////////End of function//////////////////////

//��ʱ���¼�����
void CMyUsbDeviceTestAppDlg::OnTimer(UINT nIDEvent) 
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
LRESULT CMyUsbDeviceTestAppDlg::OnDeviceChange(WPARAM nEventType, LPARAM dwData)
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
			FindMyDevice(); //�����豸����ʱ������һ���Ƿ�������ָ�����豸������
			MyDevFound=FALSE; //���ǲ���һ�£�Ϊ�˻�ȡ�豸·��������Ҫ��MyDevFound����ΪFALSE��
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //���淢��״̬�ı���豸��·����
				//�Ƚ��Ƿ�������ָ�����豸
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
			 	AddToInfOut("�豸������");
					SetDlgItemText(IDC_DS,"�豸������");
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
					SetDlgItemText(IDC_DS,"�豸�Ѱγ�");
				}
			}
			return TRUE;

		default:
			return TRUE;
	} 
}
//////////////////////////////End of function//////////////////////

void CMyUsbDeviceTestAppDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	BOOL Result;
	UINT LastError;
	UINT i;
	CString Str;

	//�����������ı����ڻ�ȡ���ݣ���ȡʧ���򷵻�
	if(!GetInputData(IDC_DATA0, "���������ʽ�������������롣", Ep2WriteBuffer[0]))return;
	if(!GetInputData(IDC_DATA1, "���������ʽ�������������롣", Ep2WriteBuffer[1]))return;
	if(!GetInputData(IDC_DATA2, "���������ʽ�������������롣", Ep2WriteBuffer[2]))return;
	if(!GetInputData(IDC_DATA3, "���������ʽ�������������롣", Ep2WriteBuffer[3]))return;
	if(!GetInputData(IDC_DATA4, "���������ʽ�������������롣", Ep2WriteBuffer[4]))return;
	if(!GetInputData(IDC_DATA5, "���������ʽ�������������롣", Ep2WriteBuffer[5]))return;
	if(!GetInputData(IDC_DATA6, "���������ʽ�������������롣", Ep2WriteBuffer[6]))return;
	if(!GetInputData(IDC_DATA7, "���������ʽ�������������롣", Ep2WriteBuffer[7]))return;

 
	//����豸û���ҵ����򷵻�ʧ��
	if(MyDevFound==FALSE)
	{
  AddToInfOut("�豸δ�ҵ�");
		return;
	}

	//��������Ч����˵�����豸ʧ��
	if(hMyDevHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("��Ч���豸����������Ǵ��豸ʱʧ��");
		return;
	}

	//����˵�2�������ڷ����У��򷵻�ʧ��
	if(Ep2DataInSending==TRUE)
	{
		AddToInfOut("�˵�2�������ڷ����У���ʱ���ܷ���");
		return;
	}

	//��ʾ�������ݵ���Ϣ
	AddToInfOut("�˵�2��������8�ֽ�");
	Str="";
	for(i=0;i<8;i++)
	{
		Str+=itos(Ep2WriteBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//�������ڷ��ͱ�־
 Ep2DataInSending=TRUE;

	//ʹ��DeviceIoControl���˵�2����8�ֽ�����
 Result=DeviceIoControl(hMyDevHandle,	//���ǵ��豸
                 EP2_WRITE,		  //�����������IoControl�Ĺ��ܴ���
                 Ep2WriteBuffer,	//���뻺��
                 8,		            //�����ֽ���
                 NULL,	          //������壬��
                 0,		            //����ֽ���
                 NULL,    //ʹ��Overlappedʱ�������ﱣ�洫���ֽ���
                 &Ep2WriteOverlapped);	//ָ����Overlapped�ṹ

	//�����������ʧ�ܣ�����������ʧ�ܣ�Ҳ������IO������
	if(Result==FALSE)
	{
  //��ȡ���������
		LastError=GetLastError();
		//���Ƿ������IO����
		if((LastError==ERROR_IO_PENDING)||(LastError==ERROR_SUCCESS))
		{
			return;
		}
		//�����Ǻ�������ʱ����������ʾ�������
		else
		{
			Ep2DataInSending=FALSE;
			AddToInfOut("�˵�2����ʧ�ܣ�������룺"+itos(LastError));
			return;
		}
	}
	//���򣬺������سɹ�
	else
	{
		Ep2DataInSending=FALSE;
	 return;
	}
}

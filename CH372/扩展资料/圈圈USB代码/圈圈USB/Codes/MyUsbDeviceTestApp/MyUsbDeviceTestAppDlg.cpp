/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

作者：电脑圈圈
建立日期: 2008.08.22
修改日期: 2008.08.27
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
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

UCHAR LedStatus=0;	//LED状态
UCHAR KeyStatus=0;	//按键状态

//用来保存找到的设备路径
CString MyDevPathName="";
//用来保存设备是否已经找到
BOOL MyDevFound=FALSE;

//定义变量用来保存VID、PID、版本号
DWORD MyVid,MyPid,MyPvn;

//用来保存打开设备的句柄
HANDLE hMyDevHandle=INVALID_HANDLE_VALUE;

//用来保存读数据的设备句柄
//HANDLE hReadHandle=INVALID_HANDLE_VALUE;
//用来保存写数据的设备句柄
//HANDLE hWriteHandle=INVALID_HANDLE_VALUE;

//端点1正在发送数据的标志
BOOL Ep1DataInSending=FALSE;
//端点2正在发送数据的标志
BOOL Ep2DataInSending=FALSE;

//端点1发送数据的缓冲区，16字节数据。
UCHAR Ep1WriteBuffer[16];

//端点1接收数据的缓冲区，16字节数据。
UCHAR Ep1ReadBuffer[16];

//端点2发送数据的缓冲区，64字节数据。
UCHAR Ep2WriteBuffer[64];

//端点2接收数据的缓冲区，64字节数据。
UCHAR Ep2ReadBuffer[64];

//写端点1用的OVERLAPPED。
OVERLAPPED Ep1WriteOverlapped;
//读端点1用的OVERLAPPED。
OVERLAPPED Ep1ReadOverlapped;

//写端点2用的OVERLAPPED。
OVERLAPPED Ep2WriteOverlapped;
//读端点2用的OVERLAPPED。
OVERLAPPED Ep2ReadOverlapped;

//指向读端点1线程的指针
CWinThread * pEp1ReadThread;
//指向写端点1线程的指针
CWinThread * pEp1WriteThread;

//指向读端点2线程的指针
CWinThread * pEp2ReadThread;
//指向写端点2线程的指针
CWinThread * pEp2WriteThread;

//总发送字节数和总接收字节数
UINT TotalSent;
UINT TotalReceived;

//用来注册设备通知事件用的广播接口。
//要使用该结构体，需要在StdAfx.h中将增加语句#define WINVER 0x0500
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

//读端点1数据的线程。由于使用的是异步调用，因而在调用ReadFile
//函数时提供一个Overlapped的结构，该结构中含有一个事件的
//句柄。平时该事件是处于无信号状态的，因而等待事件的函数
//就会被挂起，从而该线程被阻塞。当数据正确返回后，事件被
//触发，线程恢复运行。然后检查返回的数据量是否正
//确，从而设置界面上各开关的状态。由于该函数并不是
//CMyUsbDeviceTestAppDlg类（就是该工程中主窗口类）中的成员函数，
//所以无法直接调用CMyUsbDeviceTestAppDlg类中的成员函数。
//在创建该线程时，通过pParam参数传递了一个this指针，将参数pParam
//强制转化为CMyUsbDeviceTestAppDlg类的指针即可访问CMyUsbDeviceTestAppDlg类
//中的成员函数。
UINT Ep1ReadThread(LPVOID pParam)
{
	CMyUsbDeviceTestAppDlg *pAppDlg;
	DWORD Length, Counter;
	UINT i;
	CString Str;

 //将参数pParam取出，并转换为CMyUsbDeviceTestAppDlg型指针，
	//以供下面调用其成员函数。
 pAppDlg=(CMyUsbDeviceTestAppDlg*)pParam;

	//该线程是个死循环，直到程序退出时，它才退出
	while(1)
	{
		//设置事件为无效状态
		ResetEvent(Ep1ReadOverlapped.hEvent);

  //如果设备已经找到
		if(MyDevFound==TRUE)
		{
			if(hMyDevHandle==INVALID_HANDLE_VALUE) //如果读句柄无效
			{
				pAppDlg->AddToInfOut("无效的读句柄，可能是打开设备时失败");
			}
			else  //否则，句柄有效
			{
				//则调用ReadFile函数请求16字节的数据
				ReadFile(hMyDevHandle,
													Ep1ReadBuffer,
													16,
													NULL,
													&Ep1ReadOverlapped);
			}

			//等待事件触发
			WaitForSingleObject(Ep1ReadOverlapped.hEvent,INFINITE);

			//如果等待过程中设备被拔出，也会导致事件触发，但此时MyDevFound
			//被设置为假，因此在这里判断MyDevFound为假的话就进入下一轮循环。
			if(MyDevFound==FALSE) continue;

			//如果设备没有被拔下，则是ReadFile函数正常操作完成。
			//通过GetOverlappedResult函数来获取实际读取到的字节数。
			GetOverlappedResult(hMyDevHandle,&Ep1ReadOverlapped,&Length,TRUE);

			//如果字节数不为0，则将读到的数据显示到信息框中
			if(Length!=0)
			{
			 pAppDlg->AddToInfOut("端点1读取数据"+pAppDlg->itos(Length)+"字节");
			 Str="";
			 for(i=0;i<Length;i++)
			 {
			 	Str+=pAppDlg->itos(Ep1ReadBuffer[i],16).Right(2)+" ";
			 }
			 pAppDlg->AddToInfOut(Str, FALSE);
			}

			//如果字节数为8，则说明获取到了正确的8字节输入数据
			if(Length==8)
			{
				//第一字节为按键状态，将其保存到KeyStatus中。
				KeyStatus=Ep1ReadBuffer[0];

				//刷新按键的情况。
				pAppDlg->SetKeyStatus();

				//第2、3、4、5字节为设备返回的发送次数值。计算出值后并显示。
				Counter=Ep1ReadBuffer[4];
				Counter=(Counter<<8)+Ep1ReadBuffer[3];
				Counter=(Counter<<8)+Ep1ReadBuffer[2];
				Counter=(Counter<<8)+Ep1ReadBuffer[1];
				pAppDlg->SetCounterNumber(Counter);
			}
		}
		else
		{
			//阻塞线程，直到下次事件被触发
		 WaitForSingleObject(Ep1ReadOverlapped.hEvent,INFINITE);
  }
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//端点1等待写数据的线程，该线程比较简单，只是简单地等待事件被触发，
//然后清除数据正在发送的标志。
UINT Ep1WriteThread(LPVOID pParam)
{
	while(1)
	{
		//设置事件为无效状态
  ResetEvent(Ep1WriteOverlapped.hEvent);

		//等待事件触发
	 WaitForSingleObject(Ep1WriteOverlapped.hEvent,INFINITE);

		//清除数据正在发送标志
		Ep1DataInSending=FALSE;

		//EP1WriteBuffer[1]为非0值时将让设备清除它的计数值，
		//当点击清除计数器按键时，将会设置该值为非0，等数据发送
		//完毕后，将它改回0。这样在发送LED状态时，就可以不用去设置
		//EP1WriteBuffer[1]的值了。
		Ep1WriteBuffer[1]=0;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//端点2读数据的线程
UINT Ep2ReadThread(LPVOID pParam)
{
	CMyUsbDeviceTestAppDlg *pAppDlg;
	DWORD Length;
	UINT i;
	CString Str;

 //将参数pParam取出，并转换为CMyUsbDeviceTestAppDlg型指针，
	//以供下面调用其成员函数。
 pAppDlg=(CMyUsbDeviceTestAppDlg*)pParam;

	//该线程是个死循环，直到程序退出时，它才退出
	while(1)
	{
		//设置事件为无效状态
		ResetEvent(Ep2ReadOverlapped.hEvent);

  //如果设备已经找到
		if(MyDevFound==TRUE)
		{
			if(hMyDevHandle==INVALID_HANDLE_VALUE) //如果读句柄无效
			{
				pAppDlg->AddToInfOut("无效的读句柄，可能是打开设备时失败");
			}
			else  //否则，句柄有效
			{
   	//使用DeviceIoControl从端点2读取64字节数据
    DeviceIoControl(hMyDevHandle,	//我们的设备
                 EP2_READ,		  //驱动程序定义的IoControl的功能代码
                 NULL,	          //输入缓冲，无
                 0,		            //输入字节数
                 Ep2ReadBuffer,  //输出缓冲
                 64,		           //输出字节数
                 NULL,           //使用Overlapped时不在这里保存传输字节数
                 &Ep2ReadOverlapped);	 //指定的Overlapped结构
			}

			//等待事件触发
			WaitForSingleObject(Ep2ReadOverlapped.hEvent,INFINITE);

			//如果等待过程中设备被拔出，也会导致事件触发，但此时MyDevFound
			//被设置为假，因此在这里判断MyDevFound为假的话就进入下一轮循环。
			if(MyDevFound==FALSE) continue;

			//如果设备没有被拔下，则是ReadFile函数正常操作完成。
			//通过GetOverlappedResult函数来获取实际读取到的字节数。
			GetOverlappedResult(hMyDevHandle,&Ep2ReadOverlapped,&Length,TRUE);

			//如果字节数不为0，则将读到的数据显示到信息框中
			if(Length!=0)
			{
				TotalReceived+=Length;
    pAppDlg->SetDlgItemText(IDC_TOTAL_RECEIVED, pAppDlg->itos(TotalReceived)+"字节");
			 pAppDlg->AddToInfOut("端点2读取数据"+pAppDlg->itos(Length)+"字节");
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
			//阻塞线程，直到下次事件被触发
		 WaitForSingleObject(Ep2ReadOverlapped.hEvent,INFINITE);
  }
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//端点2等待写数据的线程，该线程比较简单，只是简单地等待事件被触发，
//然后清除数据正在发送的标志。
UINT Ep2WriteThread(LPVOID pParam)
{
	ULONG Length;
 CMyUsbDeviceTestAppDlg * pAppDlg;

	pAppDlg=(CMyUsbDeviceTestAppDlg *)pParam;

	while(1)
	{
		//设置事件为无效状态
  ResetEvent(Ep2WriteOverlapped.hEvent);

		//等待事件触发
	 WaitForSingleObject(Ep2WriteOverlapped.hEvent,INFINITE);

  //通过GetOverlappedResult函数来获取实际发送的字节数。
		GetOverlappedResult(hMyDevHandle,&Ep2WriteOverlapped,&Length,TRUE);
		//累计总发送字节数

		TotalSent+=Length;
  //设置总发送字节数
  pAppDlg->SetDlgItemText(IDC_TOTAL_SENT, pAppDlg->itos(TotalSent)+"字节");

		//清除数据正在发送标志
		Ep2DataInSending=FALSE;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//窗口将要显示时的函数
void CMyUsbDeviceTestAppDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	HICON hIcon;
 //获取我们自定义USB设备的接口类GUDI
 GUID MyGuid=GUID_DEVINTERFACE_COMPUTER00USB; //在interface.h文件中定义了

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	//初始化
	//LED加载熄灭态图标
	hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
	GetDlgItem(IDC_LED1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);

	//设置按键使能情况
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND_DATA)->EnableWindow(FALSE);

	//禁止LED按钮，打开设备后才能使用
	GetDlgItem(IDC_LED1)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED3)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED5)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED6)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED7)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED8)->EnableWindow(FALSE);

	//开关加载弹起状态图标
	GetDlgItem(IDC_KEY1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_KEY8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);	

	//初始化发送数据区
	SetDlgItemText(IDC_DATA0,"01");
	SetDlgItemText(IDC_DATA1,"02");
	SetDlgItemText(IDC_DATA2,"03");
	SetDlgItemText(IDC_DATA3,"04");
	SetDlgItemText(IDC_DATA4,"05");
	SetDlgItemText(IDC_DATA5,"06");
	SetDlgItemText(IDC_DATA6,"07");
	SetDlgItemText(IDC_DATA7,"08");

	//设置发送数据区文本宽度
	((CEdit*)GetDlgItem(IDC_DATA0))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA1))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA2))->SetLimitText(2);
 ((CEdit*)GetDlgItem(IDC_DATA3))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA4))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA5))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA6))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_DATA7))->SetLimitText(2);

	SetTimer(1,600,NULL);	//这个定时器用来刷新图标

	//总发送和总接收字节数为0
	TotalSent=0;
	TotalReceived=0;

	//初始化端点1写数据用的Overlapped结构体
	//偏移量设置为0
	Ep1WriteOverlapped.Offset=0;
	Ep1WriteOverlapped.OffsetHigh=0;
	//创建一个事件，提供给WriteFile使用，当WriteFile完成时，
	//会设置该事件为触发状态。
	Ep1WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//初始化端点1读数据时用的Overlapped结构体
	//偏移量设置为0
	Ep1ReadOverlapped.Offset=0;
	Ep1ReadOverlapped.OffsetHigh=0;
	//创建一个事件，提供给ReadFile使用，当ReadFile完成时，
	//会设置该事件为触发状态。
	Ep1ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//初始化端点2写数据用的Overlapped结构体
	//偏移量设置为0
	Ep2WriteOverlapped.Offset=0;
	Ep2WriteOverlapped.OffsetHigh=0;
	//创建一个事件，提供给操作端点2的IoControl使用，当IoControl完成时，
	//会设置该事件为触发状态。
	Ep2WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//初始化端点2读数据时用的Overlapped结构体
	//偏移量设置为0
	Ep2ReadOverlapped.Offset=0;
	Ep2ReadOverlapped.OffsetHigh=0;
	//创建一个事件，提供给操作端点2的IoControl使用，当IoControl完成时，
	//会设置该事件为触发状态。
	Ep2ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//创建端点1写数据的线程（处于挂起状态）
	pEp1WriteThread=AfxBeginThread(Ep1WriteThread,
		                              this,
																																THREAD_PRIORITY_NORMAL,
																																0,
																																CREATE_SUSPENDED,
																																NULL);
	//如果创建成功，则恢复该线程的运行
	if(pEp1WriteThread!=NULL)
	{
		pEp1WriteThread->ResumeThread();
	}

	//创建端点1读数据的线程（处于挂起状态）
 pEp1ReadThread=AfxBeginThread(Ep1ReadThread,
		                             this,
																															THREAD_PRIORITY_NORMAL,
																															0,
																															CREATE_SUSPENDED,
																										 				NULL);
	//如果创建成功，则恢复该线程的运行
	if(pEp1ReadThread!=NULL)
	{
		pEp1ReadThread->ResumeThread();
	}

	//创建端点2写数据的线程（处于挂起状态）
	pEp2WriteThread=AfxBeginThread(Ep2WriteThread,
		                              this,
																																THREAD_PRIORITY_NORMAL,
																																0,
																																CREATE_SUSPENDED,
																																NULL);
	//如果创建成功，则恢复该线程的运行
	if(pEp2WriteThread!=NULL)
	{
		pEp2WriteThread->ResumeThread();
	}

	//创建端点2读数据的线程（处于挂起状态）
 pEp2ReadThread=AfxBeginThread(Ep2ReadThread,
		                             this,
																															THREAD_PRIORITY_NORMAL,
																															0,
																															CREATE_SUSPENDED,
																															NULL);
	//如果创建成功，则恢复该线程的运行
	if(pEp2ReadThread!=NULL)
	{
		pEp2ReadThread->ResumeThread();
	}

	//设置DevBroadcastDeviceInterface结构体，用来注册设备改变时的通知
	DevBroadcastDeviceInterface.dbcc_size=sizeof(DevBroadcastDeviceInterface);
 DevBroadcastDeviceInterface.dbcc_devicetype=DBT_DEVTYP_DEVICEINTERFACE;
 DevBroadcastDeviceInterface.dbcc_classguid=MyGuid;
	//注册设备改变时收到通知
	RegisterDeviceNotification(m_hWnd,
							                    &DevBroadcastDeviceInterface,
																											DEVICE_NOTIFY_WINDOW_HANDLE);
	FindMyDevice(); //查找一下是否是我们指定的设备已经连接了
	if(MyDevFound==TRUE) //如果设备已经找到，那么显示
	{
		AddToInfOut("设备已连接");
		SetDlgItemText(IDC_DS,"设备已连接");
	}
	else
	{
		AddToInfOut("设备未连接");
		SetDlgItemText(IDC_DS,"设备未连接");
	}
	MyDevFound=FALSE; //仅是查找一下，所以要将MyDevFound设置回FALSE。
}
//////////////////////////////End of function//////////////////////

//将信息添加到信息显示框的函数
void CMyUsbDeviceTestAppDlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
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
}
//////////////////////////////End of function//////////////////////

//重载上述函数，入口参数不一样。
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
	SetDlgItemText(IDC_TOTAL_SENT, itos(TotalSent)+"字节");
 SetDlgItemText(IDC_TOTAL_RECEIVED, itos(TotalReceived)+"字节");
	SetDlgItemText(IDC_INF_OUT,"");
}
//////////////////////////////End of function//////////////////////

//发送LED的状态。
BOOL CMyUsbDeviceTestAppDlg::SendLedStatus()
{
	BOOL Result;
	UINT LastError;
	UINT i;
	CString Str;
 
	//如果设备没有找到，则返回失败
	if(MyDevFound==FALSE)
	{
  AddToInfOut("设备未找到");
		return FALSE;
	}

	//如果句柄无效，则说明打开设备失败
	if(hMyDevHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("无效的设备句柄，可能是打开设备时失败");
		return FALSE;
	}

	//如果端点1数据仍在发送中，则返回失败
	if(Ep1DataInSending==TRUE)
	{
		AddToInfOut("端点1数据正在发送中，暂时不能发送");
		return FALSE;
	}

	//设置要发送的数据
	Ep1WriteBuffer[0]=LedStatus;	//将LED状态放到缓冲区中

	//显示发送数据的信息
	AddToInfOut("端点1发送数据8字节");
	Str="";
	for(i=0;i<8;i++)
	{
		Str+=itos(Ep1WriteBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//设置正在发送标志
 Ep1DataInSending=TRUE;

	//调用WriteFile函数发送数据
	Result=WriteFile(hMyDevHandle,
			               Ep1WriteBuffer,
											       8,
											       NULL,
										       	&Ep1WriteOverlapped);

	//如果函数返回失败，则可能是真的失败，也可能是IO挂起了
	if(Result==FALSE)
	{
  //获取最后错误代码
		LastError=GetLastError();
		//看是否是真的IO挂起
		if((LastError==ERROR_IO_PENDING)||(LastError==ERROR_SUCCESS))
		{
			return TRUE;
		}
		//否则，是函数调用时发生错误，显示错误代码
		else
		{
			Ep1DataInSending=FALSE;
			AddToInfOut("端点1发送失败，错误代码："+itos(LastError));
			return FALSE;
		}
	}
	//否则，函数返回成功
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

	//当处理某位LED时，对应的bit为1
	Mask=(1<<(nLed-1));
 
	//翻转对应的位
	LedStatus^=Mask;

	//设置LED状态成功
	if(SendLedStatus()==TRUE)
	{
		//LED处于亮状态，使用亮的图片
		if(LedStatus&Mask)
		{
			hIcon=AfxGetApp()->LoadIcon(IDI_LED_ON);
			AddToInfOut("LED" + itos(nLed) + " 点亮");
		}
		//LED处于灭状态，使用灭的图片
		else
		{
			hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
   AddToInfOut("LED" + itos(nLed) + " 熄灭");
		}
  GetDlgItem(nID)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	}
	else
	{
		LedStatus^=Mask;  //没有设置成功，改回原来的值
	}
}
//////////////////////////////End of function//////////////////////

//LED1按键单击
void CMyUsbDeviceTestAppDlg::OnLed1()
{
	SetLedStatus(IDC_LED1,1);
	return;
}
//////////////////////////////End of function//////////////////////

//LED2按键单击
void CMyUsbDeviceTestAppDlg::OnLed2() 
{
	SetLedStatus(IDC_LED2,2);
}
//////////////////////////////End of function//////////////////////

//LED3按键单击
void CMyUsbDeviceTestAppDlg::OnLed3() 
{
	SetLedStatus(IDC_LED3,3);
}
//////////////////////////////End of function//////////////////////

//LED4按键单击
void CMyUsbDeviceTestAppDlg::OnLed4() 
{
	SetLedStatus(IDC_LED4,4);
}
//////////////////////////////End of function//////////////////////

//LED5按键单击
void CMyUsbDeviceTestAppDlg::OnLed5() 
{
	SetLedStatus(IDC_LED5,5);
}
//////////////////////////////End of function//////////////////////

//LED6按键单击
void CMyUsbDeviceTestAppDlg::OnLed6() 
{
	SetLedStatus(IDC_LED6,6);
}
//////////////////////////////End of function//////////////////////

//LED7按键单击
void CMyUsbDeviceTestAppDlg::OnLed7() 
{
	SetLedStatus(IDC_LED7,7);
}
//////////////////////////////End of function//////////////////////

//LED8按键单击
void CMyUsbDeviceTestAppDlg::OnLed8() 
{
	SetLedStatus(IDC_LED8,8);
}
//////////////////////////////End of function//////////////////////

//显示按键情况
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

	//保存上一次按键的情况
	static UCHAR LastKeyStatus=0x00;

	if(LastKeyStatus==KeyStatus)
	{
		return;		//按键状态没有改变，不需要更新按键状态
	}

	//KEYi按键情况改变
	for(i=0;i<8;i++)
	{
		Mask=(1<<i);
		if((LastKeyStatus^KeyStatus)&Mask)
		{
			if(KeyStatus&Mask)
			{
				hIcon=AfxGetApp()->LoadIcon(IDI_LED_ON);
				AddToInfOut("KEY" + itos(i+1) + "按下");
			}
			else
			{
				hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
				AddToInfOut("KEY" + itos(i+1) +"弹起");
			}
			GetDlgItem(nIDs[i])->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
		}
 }

	//保存最后按键的情况
	LastKeyStatus=KeyStatus;
}
//////////////////////////////End of function//////////////////////

//查找设备的函数。如果指定的设备找到，则设置MyDevFound为TRUE，
//并将设备的路径保存在MyDevPathName中。如果指定的设备未找到，则
//设置MyDevFound为FALSE。
void CMyUsbDeviceTestAppDlg::FindMyDevice()
{
 //定义一个GUID的结构体MyGuid并初始化为我们自定义USB设备的接口类GUID。
 GUID MyGuid=GUID_DEVINTERFACE_COMPUTER00USB; //在interface.h文件中定义了
 //定义一个DEVINFO的句柄hDevInfoSet来保存获取到的设备信息集合句柄。
	HDEVINFO hDevInfoSet;
 //定义MemberIndex，表示当前搜索到第几个设备，0表示第一个设备。
	DWORD MemberIndex;
	//DevInterfaceData，用来保存设备的驱动接口信息
	SP_DEVICE_INTERFACE_DATA DevInterfaceData;
 //定义一个BOOL变量，保存函数调用是否返回成功
	BOOL Result;
 //定义一个RequiredSize的变量，用来接收需要保存详细信息的缓冲长度。
	DWORD RequiredSize;
 //定义一个指向设备详细信息的结构体指针。
 PSP_DEVICE_INTERFACE_DETAIL_DATA	pDevDetailData;

 //初始化设备未找到
	MyDevFound=FALSE;

	//对DevInterfaceData结构体的cbSize初始化为结构体大小
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);

	//根据MyGuid来获取设备信息集合。其中Flags参数设置为
	//DIGCF_DEVICEINTERFACE|DIGCF_PRESENT，前者表示使用的GUID为
	//接口类GUID，后者表示只列举正在使用的设备，因为我们这里只
	//查找已经连接上的设备。返回的句柄保存在hDevinfo中。注意设备
	//信息集合在使用完毕后，要使用函数SetupDiDestroyDeviceInfoList
	//销毁，不然会造成内存泄漏。
	hDevInfoSet=SetupDiGetClassDevs(&MyGuid,
		                               NULL,
																			            		NULL,
																					            DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
 
	//然后对设备集合中每个设备进行列举，检查是否是我们要找的设备
	//当找到我们指定的设备，或者设备已经查找完毕时，就退出查找。
	//首先指向第一个设备，即将MemberIndex置为0。
	MemberIndex=0;
	while(1)
	{
		//调用SetupDiEnumDeviceInterfaces在设备信息集合中获取编号为
		//MemberIndex的设备信息。
	 Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
		                                   NULL,
			 																							       			&MyGuid,
				 																									       MemberIndex,
					 																							       	&DevInterfaceData);

		//如果获取信息失败，则说明设备已经查找完毕，退出循环。
		if(Result==FALSE) break;

  //将MemberIndex指向下一个设备
		MemberIndex++;

		//如果获取信息成功，则继续获取该设备的详细信息。在获取设备
		//详细信息时，需要先知道保存详细信息需要多大的缓冲区，这通过
		//第一次调用函数SetupDiGetDeviceInterfaceDetail来获取。这时
		//提供缓冲区和长度都为NULL的参数，并提供一个用来保存需要多大
		//缓冲区的变量RequiredSize。
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			                                      &DevInterfaceData,
																																									NULL,
																																									NULL,
																																									&RequiredSize,
																																									NULL);

		//然后，分配一个大小为RequiredSize缓冲区，用来保存设备详细信息。
		pDevDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
		if(pDevDetailData==NULL) //如果内存不足，则直接返回。
		{
			MessageBox("内存不足!");
			SetupDiDestroyDeviceInfoList(hDevInfoSet);
			return;
		}

		//并设置pDevDetailData的cbSize为结构体的大小（注意只是结构体大小，
		//不包括后面缓冲区）。
		pDevDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//然后再次调用SetupDiGetDeviceInterfaceDetail函数来获取设备的
		//详细信息。这次调用设置使用的缓冲区以及缓冲区大小。
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			                                      &DevInterfaceData,
																																									pDevDetailData,
																																									RequiredSize,
																																									NULL,
																																									NULL);

		//将设备路径复制出来，然后销毁刚刚申请的内存。
		MyDevPathName=pDevDetailData->DevicePath;
		free(pDevDetailData);

		//如果调用失败，则查找下一个设备。
		if(Result==FALSE) continue;

		//否则，说明调用成功，设备已经找到
		MyDevFound=TRUE; //设置设备已经找到

		//找到设备，退出循环。本程序只检测一个目标设备，查找到后就退出
		//查找了。如果你需要将所有的目标设备都列出来的话，可以设置一个
		//数组，找到后就保存在数组中，直到所有设备都查找完毕才退出查找
		break;
	}
	//调用SetupDiDestroyDeviceInfoList函数销毁设备信息集合
	SetupDiDestroyDeviceInfoList(hDevInfoSet);
}
//////////////////////////////End of function//////////////////////

//点击打开设备按钮的处理函数
void CMyUsbDeviceTestAppDlg::OnOpenDevice()
{
	//初始化设备句柄为无效句柄。
	hMyDevHandle=INVALID_HANDLE_VALUE;
 
 //显示开始查找设备
	AddToInfOut("开始查找设备");

	//调用FindMyDevice函数查找设备，如果找到，则设置
	//MyDevFound为TRUE，并将路径保存在MyDevPathName中。
	//如果未找到，则设置MyDevFound为FALSE。
	FindMyDevice();

	if(MyDevFound==TRUE) //如果设备找到
	{
  AddToInfOut("设备已经找到");
		//则以读写方式打开设备（选择为异步访问方式），并保存其句柄。
  hMyDevHandle=CreateFile(MyDevPathName, 
			                      GENERIC_READ|GENERIC_WRITE,
                         FILE_SHARE_READ|FILE_SHARE_WRITE, 
                         NULL,
																								 OPEN_EXISTING,
																								 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
																								 NULL);
		
		if(hMyDevHandle!=INVALID_HANDLE_VALUE)
		{
			AddToInfOut("打开设备成功");
		}
		else 
		{
			if(GetLastError()==0x20)AddToInfOut("打开设备失败：设备已经被打开");
			else AddToInfOut("打开设备失败");
			MyDevFound=FALSE; //设置为未找到状态
  }
	}
	else //设备未找到
	{
		AddToInfOut("设备未找到");
	}

	if(MyDevFound==TRUE) //如果找到并打开成功
	{
		Ep1DataInSending=FALSE; //可以发送数据
		Ep2DataInSending=FALSE;

  //手动触发事件，让读端点1、2线程恢复运行。因为在这之前并没有调用
  //读数据的函数，也就不会引起事件的产生，所以需要先手动触发一
  //次事件，让读端点1、2线程恢复运行。
		SetEvent(Ep1ReadOverlapped.hEvent);
  SetEvent(Ep2ReadOverlapped.hEvent);

		//显示设备的状态。
		SetDlgItemText(IDC_DS,"设备已打开");

  //使能各操作按钮，并同时禁止打开设备按钮
		//禁止打开设备按键，使能关闭设备、清计数器按键
		GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(TRUE);
  GetDlgItem(IDC_SEND_DATA)->EnableWindow(TRUE);

  //使能LED控制按键。
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

//将整型转化为CString
CString CMyUsbDeviceTestAppDlg::itos(INT value, INT radix)
{
	static CString Str;
	UCHAR strBuffer[20];

	Str=itoa(value,(char *)strBuffer,radix);
	if(radix==16)
	{
		if(Str.GetLength()==1)	//如果只有1位数据，则在前面加3个0
		{
			Str="000"+Str;
		}
		if(Str.GetLength()==2)	//如果只有2位数据，则在前面加2个0
		{
			Str="00"+Str;
		}
			if(Str.GetLength()==3)	//如果只有3位数据，则在前面加1个0
		{
			Str="0"+Str;
		}
	}
	Str.MakeUpper();
	return Str;
}
//////////////////////////////End of function//////////////////////

//获取文本框中的内容并转换为数值
BOOL CMyUsbDeviceTestAppDlg::GetInputData(INT nID, CString ErrorMsg, UCHAR &Value)
{
	CString InText;

	GetDlgItemText(nID,InText);	//获取输入的文本

	if((InText.GetLength()<=2)&&(sscanf(InText,"%x",&Value)))	//判断是否输入正确
	{
		SetDlgItemText(nID,itos(Value,16).Right(2));	//设置文本
		return TRUE;
	}
	else	//输入错误，提示重新输入，并聚焦到错误的输入框
	{
		MessageBox(ErrorMsg,NULL,MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(nID)->SetFocus();
		((CEdit*)GetDlgItem(nID))->SetSel(0,-1);
		return FALSE;
	}
}
//////////////////////////////End of function//////////////////////

//单击关闭设备按钮的响应函数
void CMyUsbDeviceTestAppDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	HICON hIcon;

	LedStatus=0;  //所有LED关闭
 KeyStatus=0;  //所有按键抬起

	//如果设备句柄不是无效的，先设置所有LED为关闭状态，然后关闭句柄。
	if(hMyDevHandle!=INVALID_HANDLE_VALUE)
	{
		SendLedStatus();
		CloseHandle(hMyDevHandle);
		hMyDevHandle=INVALID_HANDLE_VALUE;
	}
 
	Ep1DataInSending=TRUE; //不能发送数据
	Ep2DataInSending=TRUE;

	//设置设备状态为未找到
	MyDevFound=FALSE;

	//修改按键使能情况
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND_DATA)->EnableWindow(FALSE);
	//设置LED为关闭状态
	hIcon=AfxGetApp()->LoadIcon(IDI_LED_OFF);
	GetDlgItem(IDC_LED1)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED2)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED3)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED4)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED5)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED6)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED7)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	GetDlgItem(IDC_LED8)->SendMessage(BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
	//禁止LED
	GetDlgItem(IDC_LED1)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED2)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED3)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED4)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED5)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED6)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED7)->EnableWindow(FALSE);
	GetDlgItem(IDC_LED8)->EnableWindow(FALSE);

 //设置按键及显示的状态
	KeyStatus=0x00;
	SetKeyStatus();

	//显示关闭信息
	AddToInfOut("关闭设备");
 //显示设备的状态
	SetDlgItemText(IDC_DS,"设备已关闭");
}
//////////////////////////////End of function//////////////////////

//单击退出程序的按钮响应
void CMyUsbDeviceTestAppDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	OnCloseDevice(); //退出程序前先关闭设备
 DestroyWindow(); //销毁窗口
}
//////////////////////////////End of function//////////////////////

//窗口关闭时的处理
void CMyUsbDeviceTestAppDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnCloseDevice();  //关闭窗口前先关闭设备
	CDialog::OnClose();
}
//////////////////////////////End of function//////////////////////

//显示计数器的值
void CMyUsbDeviceTestAppDlg::SetCounterNumber(DWORD Counter)
{
	SetDlgItemText(IDC_COUNTER, "计数器值：" + itos(Counter));
}
//////////////////////////////End of function//////////////////////

//点击清除计数器按钮时的响应函数
void CMyUsbDeviceTestAppDlg::OnClearCounter() 
{
 //当端点1输出数据的第二字节为非0值时，清除计数器值。
	Ep1WriteBuffer[1]=0xFF;
	if(SendLedStatus()==TRUE)
	{
		SetCounterNumber(0);
	}
}
//////////////////////////////End of function//////////////////////

//定时器事件处理
void CMyUsbDeviceTestAppDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1)	//刷新图标时间到，用来产生动作的小图标
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

//设备状态改变时的处理函数
LRESULT CMyUsbDeviceTestAppDlg::OnDeviceChange(WPARAM nEventType, LPARAM dwData)
{
 PDEV_BROADCAST_DEVICEINTERFACE pdbi;
	CString DevPathName;

	//dwData是一个指向DEV_BROADCAST_DEVICEINTERFACE结构体的指针，
	//在该结构体中保存了设备的类型、路径名等参数。通过跟我们指定设备
	//的路径名比较，即可以判断是否是我们指定的设备拔下或者插入了。
	pdbi=(PDEV_BROADCAST_DEVICEINTERFACE)dwData;

	switch(nEventType) //参数nEventType中保存着事件的类型
	{
  //设备连接事件
		case DBT_DEVICEARRIVAL:
			FindMyDevice(); //有新设备连接时，查找一下是否是我们指定的设备连接了
			MyDevFound=FALSE; //仅是查找一下，为了获取设备路径，所以要将MyDevFound设置为FALSE。
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //保存发生状态改变的设备的路径名
				//比较是否是我们指定的设备
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
			 	AddToInfOut("设备被插入");
					SetDlgItemText(IDC_DS,"设备已连接");
			 }
			}
			return TRUE;

  //设备拔出事件
		case DBT_DEVICEREMOVECOMPLETE: 
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //保存发生状态改变的设备的路径名
				//比较是否是我们指定的设备
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
				 AddToInfOut("设备被拔出");
					//设备被拔出，应该关闭设备（如果处于打开状态的话），停止操作
					if(MyDevFound==TRUE)
					{
					 MyDevFound=FALSE;
			 	 OnCloseDevice();
					}
					SetDlgItemText(IDC_DS,"设备已拔出");
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

	//从数据输入文本框内获取数据，获取失败则返回
	if(!GetInputData(IDC_DATA0, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[0]))return;
	if(!GetInputData(IDC_DATA1, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[1]))return;
	if(!GetInputData(IDC_DATA2, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[2]))return;
	if(!GetInputData(IDC_DATA3, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[3]))return;
	if(!GetInputData(IDC_DATA4, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[4]))return;
	if(!GetInputData(IDC_DATA5, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[5]))return;
	if(!GetInputData(IDC_DATA6, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[6]))return;
	if(!GetInputData(IDC_DATA7, "数据输入格式错误，请重新输入。", Ep2WriteBuffer[7]))return;

 
	//如果设备没有找到，则返回失败
	if(MyDevFound==FALSE)
	{
  AddToInfOut("设备未找到");
		return;
	}

	//如果句柄无效，则说明打开设备失败
	if(hMyDevHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("无效的设备句柄，可能是打开设备时失败");
		return;
	}

	//如果端点2数据仍在发送中，则返回失败
	if(Ep2DataInSending==TRUE)
	{
		AddToInfOut("端点2数据正在发送中，暂时不能发送");
		return;
	}

	//显示发送数据的信息
	AddToInfOut("端点2发送数据8字节");
	Str="";
	for(i=0;i<8;i++)
	{
		Str+=itos(Ep2WriteBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//设置正在发送标志
 Ep2DataInSending=TRUE;

	//使用DeviceIoControl往端点2发送8字节数据
 Result=DeviceIoControl(hMyDevHandle,	//我们的设备
                 EP2_WRITE,		  //驱动程序定义的IoControl的功能代码
                 Ep2WriteBuffer,	//输入缓冲
                 8,		            //输入字节数
                 NULL,	          //输出缓冲，无
                 0,		            //输出字节数
                 NULL,    //使用Overlapped时不在这里保存传输字节数
                 &Ep2WriteOverlapped);	//指定的Overlapped结构

	//如果函数返回失败，则可能是真的失败，也可能是IO挂起了
	if(Result==FALSE)
	{
  //获取最后错误代码
		LastError=GetLastError();
		//看是否是真的IO挂起
		if((LastError==ERROR_IO_PENDING)||(LastError==ERROR_SUCCESS))
		{
			return;
		}
		//否则，是函数调用时发生错误，显示错误代码
		else
		{
			Ep2DataInSending=FALSE;
			AddToInfOut("端点2发送失败，错误代码："+itos(LastError));
			return;
		}
	}
	//否则，函数返回成功
	else
	{
		Ep2DataInSending=FALSE;
	 return;
	}
}

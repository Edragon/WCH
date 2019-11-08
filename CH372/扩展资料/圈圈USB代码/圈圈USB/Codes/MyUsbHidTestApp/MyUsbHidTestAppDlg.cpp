/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

作者：电脑圈圈
建立日期: 2008.07.22
修改日期: 2008.07.23
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
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

UCHAR LedStatus=0;	//LED状态
UCHAR KeyStatus=0;	//按键状态

//用来保存找到的设备路径
CString MyDevPathName="";
//用来保存设备是否已经找到
BOOL MyDevFound=FALSE;

//定义变量用来保存VID、PID、版本号
DWORD MyVid,MyPid,MyPvn;

//用来保存读数据的设备句柄
HANDLE hReadHandle=INVALID_HANDLE_VALUE;
//用来保存写数据的设备句柄
HANDLE hWriteHandle=INVALID_HANDLE_VALUE;

//正在发送数据的标志
BOOL DataInSending=FALSE;

//发送报告的缓冲区，1字节报告ID+8字节报告数据。
UCHAR WriteReportBuffer[9];

//接收报告的缓冲区，1字节报告ID+8字节报告数据。
UCHAR ReadReportBuffer[9];

//发送报告用的OVERLAPPED。
OVERLAPPED WriteOverlapped;
//接收报告用的OVERLAPPED。
OVERLAPPED ReadOverlapped;

//指向读报告线程的指针
CWinThread * pReadReportThread;
//指向写报告线程的指针
CWinThread * pWriteReportThread;

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

//读报告的线程。由于使用的是异步调用，因而在调用ReadFile
//函数时提供一个Overlapped的结构，该结构中含有一个事件的
//句柄。平时该事件是处于无信号状态的，因而等待事件的函数
//就会被挂起，从而该线程被阻塞。当数据正确返回后，事件被
//触发，线程恢复运行。并检查返回的数据量以及报告ID是否正
//确，从而设置界面上各开关的状态。由于该函数并不是
//CMyUsbHidTestAppDlg类（就是该工程中主窗口类）中的成员函数，
//所以无法直接调用CMyUsbHidTestAppDlg类中的成员函数。
//在创建该线程时，通过pParam参数传递了一个this指针，将参数pParam
//强制转化为CMyUsbHidTestAppDlg类的指针即可访问CMyUsbHidTestAppDlg类
//中的成员函数。
UINT ReadReportThread(LPVOID pParam)
{
	CMyUsbHidTestAppDlg *pAppDlg;
	DWORD Length, Counter;
	UINT i;
	CString Str;

 //将参数pParam取出，并转换为CMyUsbHidTestAppDlg型指针，
	//以供下面调用其成员函数。
 pAppDlg=(CMyUsbHidTestAppDlg*)pParam;

	//该线程是个死循环，直到程序退出时，它才退出
	while(1)
	{
		//设置事件为无效状态
		ResetEvent(ReadOverlapped.hEvent);

  //如果设备已经找到
		if(MyDevFound==TRUE)
		{
			if(hReadHandle==INVALID_HANDLE_VALUE) //如果读句柄无效
			{
				pAppDlg->AddToInfOut("无效的读报告句柄，可能是打开设备时失败");
			}
			else  //否则，句柄有效
			{
				//则调用ReadFile函数请求9字节的报告数据
				ReadFile(hReadHandle,
													ReadReportBuffer,
													9,
													NULL,
													&ReadOverlapped);
			}

			//等待事件触发
			WaitForSingleObject(ReadOverlapped.hEvent,INFINITE);

			//如果等待过程中设备被拔出，也会导致事件触发，但此时MyDevFound
			//被设置为假，因此在这里判断MyDevFound为假的话就进入下一轮循环。
			if(MyDevFound==FALSE) continue;

			//如果设备没有被拔下，则是ReadFile函数正常操作完成。
			//通过GetOverlappedResult函数来获取实际读取到的字节数。
			GetOverlappedResult(hReadHandle,&ReadOverlapped,&Length,TRUE);

			//如果字节数不为0，则将读到的数据显示到信息框中
			if(Length!=0)
			{
			 pAppDlg->AddToInfOut("读取报告"+pAppDlg->itos(Length)+"字节");
			 Str="";
			 for(i=0;i<Length;i++)
			 {
			 	Str+=pAppDlg->itos(ReadReportBuffer[i],16).Right(2)+" ";
			 }
			 pAppDlg->AddToInfOut(Str, FALSE);
			}

			//如果字节数为9，则说明获取到了正确的9字节报告
			if(Length==9)
			{
				//第一字节为报告ID，应该为0
				if(ReadReportBuffer[0]==0)
				{
					//第二字节为按键状态，将其保存到KeyStatus中。
					KeyStatus=ReadReportBuffer[1];

					//刷新按键的情况。
					pAppDlg->SetKeyStatus();

					//第3、4、5、6字节为设备返回的发送次数值。计算出值后并显示。
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
			//阻塞线程，直到下次事件被触发
		 WaitForSingleObject(ReadOverlapped.hEvent,INFINITE);
  }
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//写报告的线程，该线程比较简单，只是简单地等待事件被触发，
//然后清除数据正在发送的标志。
UINT WriteReportThread(LPVOID pParam)
{
	while(1)
	{
		//设置事件为无效状态
  ResetEvent(WriteOverlapped.hEvent);

		//等待事件触发
	 WaitForSingleObject(WriteOverlapped.hEvent,INFINITE);

		//清除数据正在发送标志
		DataInSending=FALSE;

		//WriteReportBuffer[2]为非0值时将让设备清除它的计数值，
		//当点击清除计数器按键时，将会设置该值为非0，等数据发送
		//完毕后，将它改回0。这样在发送LED状态时，就可以不用去设置
		//WriteReportBuffer[2]的值了。
		WriteReportBuffer[2]=0;
	}
	return 0;
}
//////////////////////////////End of function//////////////////////

//窗口将要显示时的函数
void CMyUsbHidTestAppDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	HICON hIcon;
 GUID HidGuid;

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

	//初始化VID、PID、PVN数据区
	SetDlgItemText(IDC_VID_EDIT,"8888");
	SetDlgItemText(IDC_PID_EDIT,"0006");
	SetDlgItemText(IDC_PVN_EDIT,"0100");

	//设置VID、PID、PVN数据区文本宽度
	((CEdit*)GetDlgItem(IDC_VID_EDIT))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_PID_EDIT))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_PVN_EDIT))->SetLimitText(4);

	SetTimer(1,600,NULL);	//这个定时器用来刷新图标

	//初始化写报告时用的Overlapped结构体
	//偏移量设置为0
	WriteOverlapped.Offset=0;
	WriteOverlapped.OffsetHigh=0;
	//创建一个事件，提供给WriteFile使用，当WriteFile完成时，
	//会设置该事件为触发状态。
	WriteOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//初始化读报告时用的Overlapped结构体
	//偏移量设置为0
	ReadOverlapped.Offset=0;
	ReadOverlapped.OffsetHigh=0;
	//创建一个事件，提供给ReadFile使用，当ReadFile完成时，
	//会设置该事件为触发状态。
	ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	//创建写报告的线程（处于挂起状态）
	pWriteReportThread=AfxBeginThread(WriteReportThread,
		                                this,
																																		THREAD_PRIORITY_NORMAL,
																																		0,
																																		CREATE_SUSPENDED,
																																		NULL);
	//如果创建成功，则恢复该线程的运行
	if(pWriteReportThread!=NULL)
	{
		pWriteReportThread->ResumeThread();
	}

	//创建一个读报告的线程（处于挂起状态）
 pReadReportThread=AfxBeginThread(ReadReportThread,
		                                this,
																																		THREAD_PRIORITY_NORMAL,
																																		0,
																																		CREATE_SUSPENDED,
																																		NULL);
	//如果创建成功，则恢复该线程的运行
	if(pReadReportThread!=NULL)
	{
		pReadReportThread->ResumeThread();
	}
 
	//获取HID设备的接口类GUDI
 HidD_GetHidGuid(&HidGuid);
	//设置DevBroadcastDeviceInterface结构体，用来注册设备改变时的通知
	DevBroadcastDeviceInterface.dbcc_size=sizeof(DevBroadcastDeviceInterface);
 DevBroadcastDeviceInterface.dbcc_devicetype=DBT_DEVTYP_DEVICEINTERFACE;
 DevBroadcastDeviceInterface.dbcc_classguid=HidGuid;
	//注册设备改变时收到通知
	RegisterDeviceNotification(m_hWnd,
							                    &DevBroadcastDeviceInterface,
																											DEVICE_NOTIFY_WINDOW_HANDLE);
}
//////////////////////////////End of function//////////////////////

//将信息添加到信息显示框的函数
void CMyUsbHidTestAppDlg::AddToInfOut(CString InStr, BOOL AddTime, BOOL NewLine)
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

//发送LED的状态。
BOOL CMyUsbHidTestAppDlg::SendLedStatus()
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
	if(hWriteHandle==INVALID_HANDLE_VALUE)
	{
		AddToInfOut("无效的写报告句柄，可能是打开设备时失败");
		return FALSE;
	}

	//如果数据仍在发送中，则返回失败
	if(DataInSending==TRUE)
	{
		AddToInfOut("数据正在发送中，暂时不能发送");
		return FALSE;
	}

	//设置要发送报告的数据
	WriteReportBuffer[0]=0x00;   //报告ID为0
	WriteReportBuffer[1]=LedStatus;	//将LED状态放到缓冲区中

	//显示发送数据的信息
	AddToInfOut("发送输出报告9字节");
	Str="";
	for(i=0;i<9;i++)
	{
		Str+=itos(WriteReportBuffer[i],16).Right(2)+" ";
	}
 AddToInfOut(Str,FALSE);

	//设置正在发送标志
 DataInSending=TRUE;

	//调用WriteFile函数发送数据
	Result=WriteFile(hWriteHandle,
			               WriteReportBuffer,
											       9,
											       NULL,
										       	&WriteOverlapped);

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
			DataInSending=FALSE;
			AddToInfOut("发送失败，错误代码："+itos(LastError));
   //如果最后错误为1，说明该设备不支持该函数。
			if(LastError==1)
			{
				AddToInfOut("该设备不支持WriteFile函数。", FALSE);
			}
			return FALSE;
		}
	}
	//否则，函数返回成功
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
void CMyUsbHidTestAppDlg::OnLed1()
{
	SetLedStatus(IDC_LED1,1);
	return;
}
//////////////////////////////End of function//////////////////////

//LED2按键单击
void CMyUsbHidTestAppDlg::OnLed2() 
{
	SetLedStatus(IDC_LED2,2);
}
//////////////////////////////End of function//////////////////////

//LED3按键单击
void CMyUsbHidTestAppDlg::OnLed3() 
{
	SetLedStatus(IDC_LED3,3);
}
//////////////////////////////End of function//////////////////////

//LED4按键单击
void CMyUsbHidTestAppDlg::OnLed4() 
{
	SetLedStatus(IDC_LED4,4);
}
//////////////////////////////End of function//////////////////////

//LED5按键单击
void CMyUsbHidTestAppDlg::OnLed5() 
{
	SetLedStatus(IDC_LED5,5);
}
//////////////////////////////End of function//////////////////////

//LED6按键单击
void CMyUsbHidTestAppDlg::OnLed6() 
{
	SetLedStatus(IDC_LED6,6);
}
//////////////////////////////End of function//////////////////////

//LED7按键单击
void CMyUsbHidTestAppDlg::OnLed7() 
{
	SetLedStatus(IDC_LED7,7);
}
//////////////////////////////End of function//////////////////////

//LED8按键单击
void CMyUsbHidTestAppDlg::OnLed8() 
{
	SetLedStatus(IDC_LED8,8);
}
//////////////////////////////End of function//////////////////////

//显示按键情况
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

//点击打开设备按钮的处理函数
void CMyUsbHidTestAppDlg::OnOpenDevice()
{
 //定义一个GUID的结构体HidGuid来保存HID设备的接口类GUID。
 GUID HidGuid;
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
 //定义一个用来保存打开设备的句柄。
	HANDLE hDevHandle;
 //定义一个HIDD_ATTRIBUTES的结构体变量，保存设备的属性。
	HIDD_ATTRIBUTES DevAttributes;

 //初始化设备未找到
	MyDevFound=FALSE;

	//获取在文本框中设置的VID、PID、PVN。
	GetMyIDs();

	//初始化读、写句柄为无效句柄。
	hReadHandle=INVALID_HANDLE_VALUE;
	hWriteHandle=INVALID_HANDLE_VALUE;

	//对DevInterfaceData结构体的cbSize初始化为结构体大小
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);
	//对DevAttributes结构体的Size初始化为结构体大小
	DevAttributes.Size=sizeof(DevAttributes);

	//调用HidD_GetHidGuid函数获取HID设备的GUID，并保存在HidGuid中。
	HidD_GetHidGuid(&HidGuid);

	//根据HidGuid来获取设备信息集合。其中Flags参数设置为
	//DIGCF_DEVICEINTERFACE|DIGCF_PRESENT，前者表示使用的GUID为
	//接口类GUID，后者表示只列举正在使用的设备，因为我们这里只
	//查找已经连接上的设备。返回的句柄保存在hDevinfo中。注意设备
	//信息集合在使用完毕后，要使用函数SetupDiDestroyDeviceInfoList
	//销毁，不然会造成内存泄漏。
	hDevInfoSet=SetupDiGetClassDevs(&HidGuid,
		                               NULL,
																			            		NULL,
																					            DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);
 
	AddToInfOut("开始查找设备");
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
			 																							       			&HidGuid,
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

		//如果调用成功，则使用不带读写访问的CreateFile函数
		//来获取设备的属性，包括VID、PID、版本号等。
		//对于一些独占设备（例如USB键盘），使用读访问方式是无法打开的，
		//而使用不带读写访问的格式才可以打开这些设备，从而获取设备的属性。
  hDevHandle=CreateFile(MyDevPathName, 
			                     NULL,
                        FILE_SHARE_READ|FILE_SHARE_WRITE, 
                        NULL,
																								OPEN_EXISTING,
																								FILE_ATTRIBUTE_NORMAL,
																								NULL);

		//如果打开成功，则获取设备属性。
		if(hDevHandle!=INVALID_HANDLE_VALUE)
		{
			//获取设备的属性并保存在DevAttributes结构体中
			Result=HidD_GetAttributes(hDevHandle,
				                         &DevAttributes);

			//关闭刚刚打开的设备
			CloseHandle(hDevHandle);

   //获取失败，查找下一个
   if(Result==FALSE) continue;

			//如果获取成功，则将属性中的VID、PID以及设备版本号与我们需要的
			//进行比较，如果都一致的话，则说明它就是我们要找的设备。
			if(DevAttributes.VendorID==MyVid) //如果VID相等
				if(DevAttributes.ProductID==MyPid) //并且PID相等
					if(DevAttributes.VersionNumber==MyPvn) //并且设备版本号相等
					{
						MyDevFound=TRUE; //设置设备已经找到
						AddToInfOut("设备已经找到");

						//那么就是我们要找的设备，分别使用读写方式打开之，并保存其句柄
						//并且选择为异步访问方式。
      //读方式打开设备
      hReadHandle=CreateFile(MyDevPathName, 
			                          GENERIC_READ,
                             FILE_SHARE_READ|FILE_SHARE_WRITE, 
                             NULL,
																								     OPEN_EXISTING,
																								     FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
																								     NULL);
						if(hReadHandle!=INVALID_HANDLE_VALUE)AddToInfOut("读访问打开设备成功");
						else AddToInfOut("读访问打开设备失败");

						//写方式打开设备
      hWriteHandle=CreateFile(MyDevPathName, 
			                          GENERIC_WRITE,
                             FILE_SHARE_READ|FILE_SHARE_WRITE, 
                             NULL,
																								     OPEN_EXISTING,
																								     FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
																								     NULL);
						if(hWriteHandle!=INVALID_HANDLE_VALUE)AddToInfOut("写访问打开设备成功");
						else AddToInfOut("写访问打开设备失败");

						DataInSending=FALSE; //可以发送数据

      //手动触发事件，让读报告线程恢复运行。因为在这之前并没有调用
      //读数据的函数，也就不会引起事件的产生，所以需要先手动触发一
      //次事件，让读报告线程恢复运行。
						SetEvent(ReadOverlapped.hEvent);
      
						//显示设备的状态。
						SetDlgItemText(IDC_DS,"设备已打开");

						//找到设备，退出循环。本程序只检测一个目标设备，查找到后就退出
						//查找了。如果你需要将所有的目标设备都列出来的话，可以设置一个
						//数组，找到后就保存在数组中，直到所有设备都查找完毕才退出查找
						break;
					}
		}
		//如果打开失败，则查找下一个设备
		else continue;
	}

	//调用SetupDiDestroyDeviceInfoList函数销毁设备信息集合
	SetupDiDestroyDeviceInfoList(hDevInfoSet);

 //如果设备已经找到，那么应该使能各操作按钮，并同时禁止打开设备按钮
	if(MyDevFound)
	{
		//禁止打开设备按键，使能关闭设备、清计数器按键
		GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(TRUE);

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
	else
	{
		AddToInfOut("设备未找到");
	}
}
//////////////////////////////End of function//////////////////////

//将整型转化为CString
CString CMyUsbHidTestAppDlg::itos(INT value, INT radix)
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
BOOL CMyUsbHidTestAppDlg::GetInputData(INT nID, CString ErrorMsg, DWORD &Value)
{
	CString InText;

	GetDlgItemText(nID,InText);	//获取输入的文本

	if((InText.GetLength()<=4)&&(sscanf(InText,"%x",&Value)))	//判断是否输入正确
	{
		SetDlgItemText(nID,itos(Value,16));	//设置文本
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

//从文本框中获取输入的VID、PID、PVN
void CMyUsbHidTestAppDlg::GetMyIDs()
{
 //从文本框中获取VID号，保存在MyVid中。
	GetInputData(IDC_VID_EDIT,"厂商ID(VID)输入格式错误。请重新输入。",MyVid);
 //从文本框中获取PID号，保存在MyPid中。
 GetInputData(IDC_PID_EDIT,"产品ID(PID)输入格式错误。请重新输入。",MyPid);
	//从文本框中获取PVN号，保存在MyPvn中。
	GetInputData(IDC_PVN_EDIT,"产品版本号(PVN)输入格式错误。请重新输入。",MyPvn);
}
//////////////////////////////End of function//////////////////////

//单击关闭设备按钮的响应函数
void CMyUsbHidTestAppDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	HICON hIcon;

	LedStatus=0;  //所有LED关闭
 KeyStatus=0;  //所有按键抬起

	//如果读数据的句柄不是无效句柄，则关闭之
	if(hReadHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReadHandle);
		hReadHandle=INVALID_HANDLE_VALUE;
	}
	//如果写数据的句柄不是无效句柄，先设置所有LED为关闭状态，
	//然后关闭句柄。
	if(hWriteHandle!=INVALID_HANDLE_VALUE)
	{
		SendLedStatus();
		CloseHandle(hWriteHandle);
		hWriteHandle=INVALID_HANDLE_VALUE;
	}

	//设置设备状态为未找到
	MyDevFound=FALSE;

	//修改按键使能情况
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEAR_COUNTER)->EnableWindow(FALSE);
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
void CMyUsbHidTestAppDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	OnCloseDevice(); //退出程序前先关闭设备
 DestroyWindow(); //销毁窗口
}
//////////////////////////////End of function//////////////////////

//窗口关闭时的处理
void CMyUsbHidTestAppDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnCloseDevice();  //关闭窗口前先关闭设备
	CDialog::OnClose();
}
//////////////////////////////End of function//////////////////////

//显示计数器的值
void CMyUsbHidTestAppDlg::SetCounterNumber(DWORD Counter)
{
	SetDlgItemText(IDC_COUNTER, "计数器值：" + itos(Counter));
}
//////////////////////////////End of function//////////////////////

//点击清除计数器按钮时的响应函数
void CMyUsbHidTestAppDlg::OnClearCounter() 
{
 //当输出报告的第二字节为非0值时，清除计数器值。
	WriteReportBuffer[2]=0xFF;
	if(SendLedStatus()==TRUE)
	{
		SetCounterNumber(0);
	}
}
//////////////////////////////End of function//////////////////////

//定时器事件处理
void CMyUsbHidTestAppDlg::OnTimer(UINT nIDEvent) 
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
LRESULT CMyUsbHidTestAppDlg::OnDeviceChange(WPARAM nEventType, LPARAM dwData)
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
			if(pdbi->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
   {
				DevPathName=pdbi->dbcc_name; //保存发生状态改变的设备的路径名
				//比较是否是我们指定的设备
			 if(MyDevPathName.CompareNoCase(DevPathName)==0)
			 {
			 	AddToInfOut("设备已连接");
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
				}
			}
			return TRUE;

		default:
			return TRUE;
	} 
}
//////////////////////////////End of function//////////////////////

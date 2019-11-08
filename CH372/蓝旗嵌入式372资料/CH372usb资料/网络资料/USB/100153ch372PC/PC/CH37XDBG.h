// CH37XDBG.h : main header file for the CH37XDBG application
//

#if !defined(AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_)
#define AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "total.h"
/////////////////////////////////////////////////////////////////////////////
// CCH37XDBGApp:
// See CH37XDBG.cpp for the implementation of this class
//


class CCH37XDBGApp : public CWinApp
{
public:
	CCH37XDBGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCH37XDBGApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCH37XDBGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/* **************************************************************************************************** */
// 提供给主程序调用的子程序


// 应用程序启动后,在所有操作之前,必须首先调用一次下面的CH375DBG_GetFirmwareInfo子程序
BOOL	CH375DBG_GetFirmwareInfo( PUCHAR ver );  //获取调试固件程序的版本,并取消未完成的上传数据块
// 输入参数: ver 指向一个字节变量单元,用于存放获取的版本号


// 应用程序退出前,在所有操作之后,应该调用一次下面的CH375DBG_CloseExit子程序
VOID	CH375DBG_CloseExit( VOID );  //关闭CH372/CH375设备


BOOL	CH375DBG_GetAppSysInfo( PUCHAR ver, PUCHAR StringBuffer );  //获取当前应用系统的版本和说明字符串
// 输入参数: ver 指向一个字节变量单元,用于存放获取的版本号
//           StringBuffer 指向一个足够大的字符串缓冲区,用于存放获取的说明字符串


BOOL	CH375DBG_ReadSFR( ULONG addr, PUCHAR data );  //从SFR一个单元读取数据
// 输入参数: addr 指定SFR的地址
//           data 指向一个字节变量单元,用于存放读出的数据


BOOL	CH375DBG_WriteSFR( ULONG addr, UCHAR data );  //向SFR一个单元写入数据
// 输入参数: addr 指定SFR的地址
//           data 指定准备写入的数据


BOOL	CH375DBG_ReadIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //从内部RAM读取数据块
// 输入参数: StartAddr 指定内部RAM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数


BOOL	CH375DBG_WriteIRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //向内部RAM写入数据块
// 输入参数: StartAddr 指定内部RAM的起始地址
//           buffer 指向一个数据缓冲区,存放准备写入的数据块
//           count 指定写入的字节数


BOOL	CH375DBG_ReadXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //从外部RAM读取数据块
// 输入参数: StartAddr 指定外部RAM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数


BOOL	CH375DBG_WriteXRAM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //向外部RAM写入数据块
// 输入参数: StartAddr 指定外部RAM的起始地址
//           buffer 指向一个数据缓冲区,存放准备写入的数据块
//           count 指定写入的字节数


BOOL	CH375DBG_ReadROM( ULONG StartAddr, PUCHAR buffer, UCHAR count );  //从程序ROM读取数据块
// 输入参数: StartAddr 指定程序ROM的起始地址
//           buffer 指向一个足够大的数据缓冲区,用于存放读出的数据块
//           count 指定读取的字节数


UCHAR	CH375DBG_GetPortP1( VOID );  //获取MCS51单片机端口P1的状态
// 输出结果: 返回当前端口状态值


VOID	CH375DBG_PutPortP1( UCHAR d );  //设置MCS51单片机端口P1的状态
// 输入参数: d 指定新的端口状态值

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH37XDBG_H__AD41B0B6_A521_441F_AF68_B690253F6919__INCLUDED_)

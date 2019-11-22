; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSFRPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CH37XDBG.h"

ClassCount=9
Class1=CCH37XDBGApp
Class2=CCH37XDBGDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_DLGEXTERN
Resource2=IDR_MAINFRAME
Resource3=IDD_DLGSFR
Resource4=IDD_DLGIRAM
Resource5=IDD_CH37XDBG_DIALOG
Resource6=IDD_ABOUTBOX
Class4=CSFRPage
Class5=CIRAMPage
Class6=CXRAMPage
Class7=CExternPage
Class8=CAllControlsSheet
Resource7=IDD_DLGXRAM
Class9=CENTER
Resource8=IDD_DLGENTER

[CLS:CCH37XDBGApp]
Type=0
HeaderFile=CH37XDBG.h
ImplementationFile=CH37XDBG.cpp
Filter=N

[CLS:CCH37XDBGDlg]
Type=0
HeaderFile=CH37XDBGDlg.h
ImplementationFile=CH37XDBGDlg.cpp
Filter=D
LastObject=CCH37XDBGDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=CH37XDBGDlg.h
ImplementationFile=CH37XDBGDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CH37XDBG_DIALOG]
Type=1
Class=CCH37XDBGDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DLGSFR]
Type=1
Class=CSFRPage
ControlCount=37
Control1=IDC_EDIT_NAME1,edit,1350633600
Control2=IDC_EDIT_VALUE1,edit,1350631552
Control3=IDC_EDIT_NAME2,edit,1350633600
Control4=IDC_EDIT_VALUE2,edit,1350631552
Control5=IDC_EDIT_NAME3,edit,1350633600
Control6=IDC_EDIT_VALUE3,edit,1350631552
Control7=IDC_EDIT_NAME4,edit,1350633600
Control8=IDC_EDIT_VALUE4,edit,1350631552
Control9=IDC_EDIT_NAME5,edit,1350633600
Control10=IDC_EDIT_VALUE5,edit,1350631552
Control11=IDC_EDIT_NAME6,edit,1350633600
Control12=IDC_EDIT_VALUE6,edit,1350631552
Control13=IDC_EDIT_NAME7,edit,1350633600
Control14=IDC_EDIT_VALUE7,edit,1350631552
Control15=IDC_EDIT_NAME8,edit,1350633600
Control16=IDC_EDIT_VALUE8,edit,1350631552
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC_VER,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_BUTTON_REFRESH,button,1342242816
Control21=IDC_EDIT_NAME9,edit,1350633600
Control22=IDC_EDIT_VALUE9,edit,1350631552
Control23=IDC_EDIT_NAME10,edit,1350633600
Control24=IDC_EDIT_VALUE10,edit,1350631552
Control25=IDC_EDIT_NAME11,edit,1350633600
Control26=IDC_EDIT_VALUE11,edit,1350631552
Control27=IDC_EDIT_NAME12,edit,1350633600
Control28=IDC_EDIT_VALUE12,edit,1350631552
Control29=IDC_EDIT_NAME13,edit,1350633600
Control30=IDC_EDIT_VALUE13,edit,1350631552
Control31=IDC_EDIT_NAME14,edit,1350633600
Control32=IDC_EDIT_VALUE14,edit,1350631552
Control33=IDC_EDIT_NAME15,edit,1350633600
Control34=IDC_EDIT_VALUE15,edit,1350631552
Control35=IDC_EDIT_NAME16,edit,1350633600
Control36=IDC_EDIT_VALUE16,edit,1350631552
Control37=IDC_STATIC,static,1342308352

[DLG:IDD_DLGIRAM]
Type=1
Class=CIRAMPage
ControlCount=6
Control1=IDC_IGRID,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON_READ,button,1342242816
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DLGXRAM]
Type=1
Class=CXRAMPage
ControlCount=9
Control1=IDC_EDIT_ADDR,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_XGRID,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON_READ,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_DLGEXTERN]
Type=1
Class=CExternPage
ControlCount=15
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_COMM,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_LEN,edit,1350631552
Control5=IDC_EDIT_PARA1,edit,1350631552
Control6=IDC_EDIT_PARA2,edit,1350631552
Control7=IDC_EDIT_PARA3,edit,1350631552
Control8=IDC_EDIT_PARA4,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_BUTTON_SEND,button,1342242816
Control14=IDC_BUTTON_RECEIVE,button,1342242816
Control15=IDC_EDIT_BUF,edit,1350631556

[CLS:CSFRPage]
Type=0
HeaderFile=SFRPage.h
ImplementationFile=SFRPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CSFRPage
VirtualFilter=idWC

[CLS:CIRAMPage]
Type=0
HeaderFile=IRAMPage.h
ImplementationFile=IRAMPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CIRAMPage
VirtualFilter=idWC

[CLS:CXRAMPage]
Type=0
HeaderFile=XRAMPage.h
ImplementationFile=XRAMPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_EDIT_ADDR
VirtualFilter=idWC

[CLS:CExternPage]
Type=0
HeaderFile=ExternPage.h
ImplementationFile=ExternPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CExternPage
VirtualFilter=idWC

[CLS:CAllControlsSheet]
Type=0
HeaderFile=AllControlsSheet.h
ImplementationFile=AllControlsSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CAllControlsSheet
VirtualFilter=hWC

[DLG:IDD_DLGENTER]
Type=1
Class=CENTER
ControlCount=2
Control1=IDC_EDIT_DATA,edit,1350631552
Control2=IDC_BUTTON_ENTER,button,1342242816

[CLS:CENTER]
Type=0
HeaderFile=ENTER.h
ImplementationFile=ENTER.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC


VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "CH372/CH375和CH451/CH452演示程序 V1.3"
   ClientHeight    =   6255
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5745
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6255
   ScaleWidth      =   5745
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton pnpproce 
      Caption         =   "设备插拔事件处理控件"
      Height          =   420
      Left            =   3135
      TabIndex        =   76
      Top             =   5760
      Visible         =   0   'False
      Width           =   2055
   End
   Begin VB.CommandButton Command3 
      Caption         =   "命令"
      Height          =   495
      Left            =   495
      TabIndex        =   74
      Top             =   5280
      Width           =   735
   End
   Begin VB.CommandButton Command2 
      Caption         =   "63"
      Height          =   495
      Index           =   63
      Left            =   4695
      TabIndex        =   73
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "62"
      Height          =   495
      Index           =   62
      Left            =   4215
      TabIndex        =   72
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "61"
      Height          =   495
      Index           =   61
      Left            =   3735
      TabIndex        =   71
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "60"
      Height          =   495
      Index           =   60
      Left            =   3255
      TabIndex        =   70
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "59"
      Height          =   495
      Index           =   59
      Left            =   2775
      TabIndex        =   69
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "58"
      Height          =   495
      Index           =   58
      Left            =   2295
      TabIndex        =   68
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "57"
      Height          =   495
      Index           =   57
      Left            =   1815
      TabIndex        =   67
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "56"
      Height          =   495
      Index           =   56
      Left            =   1335
      TabIndex        =   66
      Top             =   5280
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "55"
      Height          =   495
      Index           =   55
      Left            =   4695
      TabIndex        =   65
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "54"
      Height          =   495
      Index           =   54
      Left            =   4215
      TabIndex        =   64
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "53"
      Height          =   495
      Index           =   53
      Left            =   3735
      TabIndex        =   63
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "52"
      Height          =   495
      Index           =   52
      Left            =   3255
      TabIndex        =   62
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "51"
      Height          =   495
      Index           =   51
      Left            =   2775
      TabIndex        =   61
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "50"
      Height          =   495
      Index           =   50
      Left            =   2295
      TabIndex        =   60
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "49"
      Height          =   495
      Index           =   49
      Left            =   1815
      TabIndex        =   59
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "48"
      Height          =   495
      Index           =   48
      Left            =   1335
      TabIndex        =   58
      Top             =   4800
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "47"
      Height          =   495
      Index           =   47
      Left            =   4695
      TabIndex        =   57
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "46"
      Height          =   495
      Index           =   46
      Left            =   4215
      TabIndex        =   56
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "45"
      Height          =   495
      Index           =   45
      Left            =   3735
      TabIndex        =   55
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "44"
      Height          =   495
      Index           =   44
      Left            =   3255
      TabIndex        =   54
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "43"
      Height          =   495
      Index           =   43
      Left            =   2775
      TabIndex        =   53
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "42"
      Height          =   495
      Index           =   42
      Left            =   2295
      TabIndex        =   52
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "41"
      Height          =   495
      Index           =   41
      Left            =   1815
      TabIndex        =   51
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "40"
      Height          =   495
      Index           =   40
      Left            =   1335
      TabIndex        =   50
      Top             =   4320
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "39"
      Height          =   495
      Index           =   39
      Left            =   4695
      TabIndex        =   49
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "38"
      Height          =   495
      Index           =   38
      Left            =   4215
      TabIndex        =   48
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "37"
      Height          =   495
      Index           =   37
      Left            =   3735
      TabIndex        =   47
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "36"
      Height          =   495
      Index           =   36
      Left            =   3255
      TabIndex        =   46
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "35"
      Height          =   495
      Index           =   35
      Left            =   2775
      TabIndex        =   45
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "34"
      Height          =   495
      Index           =   34
      Left            =   2295
      TabIndex        =   44
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "33"
      Height          =   495
      Index           =   33
      Left            =   1815
      TabIndex        =   43
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "32"
      Height          =   495
      Index           =   32
      Left            =   1335
      TabIndex        =   42
      Top             =   3840
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "31"
      Height          =   495
      Index           =   31
      Left            =   4695
      TabIndex        =   41
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "30"
      Height          =   495
      Index           =   30
      Left            =   4215
      TabIndex        =   40
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "29"
      Height          =   495
      Index           =   29
      Left            =   3735
      TabIndex        =   39
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "28"
      Height          =   495
      Index           =   28
      Left            =   3255
      TabIndex        =   38
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "27"
      Height          =   495
      Index           =   27
      Left            =   2775
      TabIndex        =   37
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "26"
      Height          =   495
      Index           =   26
      Left            =   2295
      TabIndex        =   36
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "25"
      Height          =   495
      Index           =   25
      Left            =   1815
      TabIndex        =   35
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "24"
      Height          =   495
      Index           =   24
      Left            =   1335
      TabIndex        =   34
      Top             =   3360
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "23"
      Height          =   495
      Index           =   23
      Left            =   4695
      TabIndex        =   33
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "22"
      Height          =   495
      Index           =   22
      Left            =   4215
      TabIndex        =   32
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "21"
      Height          =   495
      Index           =   21
      Left            =   3735
      TabIndex        =   31
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "20"
      Height          =   495
      Index           =   20
      Left            =   3255
      TabIndex        =   30
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "19"
      Height          =   495
      Index           =   19
      Left            =   2775
      TabIndex        =   29
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "18"
      Height          =   495
      Index           =   18
      Left            =   2295
      TabIndex        =   28
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "17"
      Height          =   495
      Index           =   17
      Left            =   1815
      TabIndex        =   27
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "16"
      Height          =   495
      Index           =   16
      Left            =   1335
      TabIndex        =   26
      Top             =   2880
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "15"
      Height          =   495
      Index           =   15
      Left            =   4695
      TabIndex        =   25
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "14"
      Height          =   495
      Index           =   14
      Left            =   4215
      TabIndex        =   24
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "13"
      Height          =   495
      Index           =   13
      Left            =   3735
      TabIndex        =   23
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "12"
      Height          =   495
      Index           =   12
      Left            =   3255
      TabIndex        =   22
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "11"
      Height          =   495
      Index           =   11
      Left            =   2775
      TabIndex        =   21
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "10"
      Height          =   495
      Index           =   10
      Left            =   2295
      TabIndex        =   20
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "9"
      Height          =   495
      Index           =   9
      Left            =   1815
      TabIndex        =   19
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "8"
      Height          =   495
      Index           =   8
      Left            =   1335
      TabIndex        =   18
      Top             =   2400
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "7"
      Height          =   495
      Index           =   7
      Left            =   4695
      TabIndex        =   17
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "6"
      Height          =   495
      Index           =   6
      Left            =   4215
      TabIndex        =   16
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "5"
      Height          =   495
      Index           =   5
      Left            =   3735
      TabIndex        =   15
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "4"
      Height          =   495
      Index           =   4
      Left            =   3255
      TabIndex        =   14
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "3"
      Height          =   495
      Index           =   3
      Left            =   2775
      TabIndex        =   13
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "2"
      Height          =   495
      Index           =   2
      Left            =   2295
      TabIndex        =   12
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "1"
      Height          =   495
      Index           =   1
      Left            =   1815
      TabIndex        =   11
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command2 
      Caption         =   "0"
      Height          =   495
      Index           =   0
      Left            =   1335
      TabIndex        =   8
      Top             =   1920
      Width           =   495
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   7
      Left            =   4590
      TabIndex        =   7
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   6
      Left            =   4005
      TabIndex        =   6
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   5
      Left            =   3420
      TabIndex        =   5
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   4
      Left            =   2835
      TabIndex        =   4
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   3
      Left            =   2250
      TabIndex        =   3
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   2
      Left            =   1665
      TabIndex        =   2
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   1
      Left            =   1080
      TabIndex        =   1
      Top             =   120
      Width           =   595
   End
   Begin VB.CommandButton Command1 
      Caption         =   "0"
      Height          =   855
      Index           =   0
      Left            =   495
      TabIndex        =   0
      Top             =   120
      Width           =   595
   End
   Begin VB.Label devstatue 
      AutoSize        =   -1  'True
      Caption         =   "**"
      Height          =   540
      Left            =   1455
      TabIndex        =   75
      Top             =   5880
      Width           =   180
   End
   Begin VB.Label Label2 
      Caption         =   "下面的每个按钮对应按键，按下时同步显示"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9.75
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   615
      TabIndex        =   10
      Top             =   1560
      Width           =   3855
   End
   Begin VB.Label Label1 
      Caption         =   "上面的每个按钮对应数码管，双击输入数字"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9.75
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   615
      TabIndex        =   9
      Top             =   1200
      Width           =   3975
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim mCaptionInform As String
Dim mDemoReq As COMMAND_PACKET
Dim mOpen As Long
Const DEMO_GET_KEY = &H34
Const DEMO_RET_OK = &H0

'以下测试PC机与单片机之间的USB通讯
Public Sub Testcommunication()
    Dim mLength As Long

    mDemoReq.mCommandCode = DEF_CMD_TEST_DATA    ' 测试命令,将PC机发来的所有数据取反后返回
    mDemoReq.mCommandCodeNot = &HFF - DEF_CMD_TEST_DATA
    mDemoReq.mParameter(0) = &H5A  ' 任意的测试数据,返回后将按位取反
    mDemoReq.mParameter(1) = &H96  ' 任意的测试数据,返回后将按位取反
    mDemoReq.mParameter(2) = &HF3  ' 任意的测试数据,返回后将按位取反
    mDemoReq.mParameter(3) = &H4C  ' 任意的测试数据,返回后将按位取反
    mDemoReq.mParameter(4) = &H39  ' 任意的测试数据,返回后将按位取反

    mLength = CONST_CMD_LEN    ' 命令包的长度
    If (CH375Writedata(mIndex, mDemoReq, mLength)) Then       ' 通过CH375发送命令数据,成功
        mLength = mCH375_PACKET_LENGTH
        If (CH375ReadData(mIndex, mDemoReq, mLength)) Then        ' 通过CH375接收应答数据,成功
            If (mLength = CONST_CMD_LEN) Then
                If ((mDemoReq.mCommandCode <> (&HFF - DEF_CMD_TEST_DATA)) Or (mDemoReq.mParameter(0) <> (&HFF - &H5A)) Or (mDemoReq.mParameter(1) <> (&HFF - &H96))) Then
                    MsgBox "通过USB传输的数据有错误", vbExclamation, mCaptionInform
                End If
            Else
                MsgBox "CH375数据测试返回的长度错误", vbExclamation, mCaptionInform
            End If
                  
        Else
            MsgBox "CH375ReadData 失败", vbExclamation, mCaptionInform
        End If
    Else
        MsgBox "CH375WriteData 失败", vbExclamation, mCaptionInform
    End If
        
' 下面是下传数据块的例子

 '             Dim mBuffer(4095) As Byte
 '             mBuffer(0) = Data  ' 准备下传的数据
 '             mLength = mDownloadData(mBuffer, 4096)   ' 将数据块从计算机下传给单片机,返回实际传输长度
 '             mLength = mUploadData(mBuffer, 4096)     ' 从单片机上传数据块到计算机,返回实际传输长度

End Sub

'/*设备插拔事件处理*/
Private Sub pnpproce_KeyUp(KeyCode As Integer, Shift As Integer)
    Dim i As Byte
    Dim iEventStatus As Long
    iEventStatus = KeyCode   '插拔事件代码

    If (iEventStatus = CH375_DEVICE_ARRIVAL) Then
        If (mOpen = -1) Then  '设备没打开
            mOpen = CH375OpenDevice(mIndex)                  '设备插入后打开设备
            If mOpen = -1 Then
                MsgBox "无法打开CH375设备", vbCritical, "信息提示"
                Exit Sub
            End If
            CH375SetTimeout mIndex, 3000, 3000
            Call Testcommunication  '测试PC机与单片机之间的USB通讯
            Call mCallInt  '设置中断
        End If
        For i = 0 To 7
            Command1(i).Enabled = True 'LED
        Next i
        Command3.Enabled = True
        devstatue.Caption = "**CH372/CH375设备已插上"
        
    ElseIf (iEventStatus = CH375_DEVICE_REMOVE) Then
        If (mOpen <> -1) Then
            CH375CloseDevice (mIndex)     '设备拔出,关闭设备句柄
            mOpen = -1
        End If
        For i = 0 To 7
            Command1(i).Enabled = False
        Next i
        Command3.Enabled = False
        devstatue.Caption = "**CH372/CH375设备已拔出"
    End If
End Sub
Private Sub Command1_Click(index As Integer)
    Select Case index       '按钮标识
        Case 0
            frmEnter.index = 0                          '按钮1
        Case 1
            frmEnter.index = 1                          '按钮2
        Case 2
            frmEnter.index = 2                          '按钮3
        Case 3
            frmEnter.index = 3                          '按钮4
        Case 4
            frmEnter.index = 4                          '按钮5
        Case 5
            frmEnter.index = 5                          '按钮6
        Case 6
            frmEnter.index = 6                          '按钮7
        Case 7
            frmEnter.index = 7                          '按钮8
    End Select
    frmEnter.Show
End Sub

Private Sub Command3_Click()               '命令按钮
    frmEnter.index = -1                    '识别命令按钮
    frmEnter.Show vbModal, Me
 
End Sub



'模拟调用窗体的按键按下事件
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    Dim mLength As Long
    Dim mDemoReq As COMMAND_PACKET
    If KeyCode = 1 Then     ' 键被按下,以下先释放原按键再读取新按键值
        Call mSyncKeyboard(&HFF)        ' 释放上次的按键
        Call mSyncKeyboard(mKeyCode)         '根据键值作同步显示
    End If
End Sub
'模拟调用窗体按键松开事件
Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    If KeyCode = 1 Then
        Call mSyncKeyboard(&HFF)   ' 键盘按键松开
    End If
End Sub

Private Sub Form_Load()
    mIndex = 0  '第一个ch375设备
    mShowLED = 0
    mKeyCode = &HFF
    mCaptionInform = " 信息提示 "
    mOpen = -1
    
    '设置设备插拔监视
    If (CH375SetDeviceNotify(mIndex, vbNullString, AddressOf iNotifyRoutine) = False) Then
       MsgBox "设置监视CH372/CH375设备插拔失败"
    End If
        
    mOpen = CH375OpenDevice(mIndex)                  '窗体加载时打开设备
    If mOpen = -1 Then
        Call pnpproce_KeyUp(CH375_DEVICE_REMOVE, 0)  '作设备拔出处理
        Exit Sub
    Else
        Call pnpproce_KeyUp(CH375_DEVICE_ARRIVAL, 0) '作设备插入处理
    End If
    
    ' 设置USB数据读写的超时,超过3000mS未完成读写将强制返回,避免一直等待下去
    CH375SetTimeout mIndex, 3000, 3000
    
    ' 以下测试PC机与单片机之间的USB通讯,仅作演示,本身没有意义
    Call Testcommunication
    
    Call mCallInt  '设置中断
 '下面加载中断服务程序 , 中断服务程序是在应用层执行的, 其线程优先级是THREAD_PRIORITY_TIME_CRITICAL
 '当单片机有事需要通知计算机时 , 可以用CMD_WR_USB_DATA5命令写入中断特征数据, 计算机的mInterruptEvent线程将会收到该中断特征数据
 '然后mInterruptEvent线程向主程序发出消息进行处理,mInterruptEvent线程相当于中断服务程序,代替主程序定时查询单片机
  
       
End Sub

'同步按键显示,键被按下时显示◎,否则显示键号
Sub mSyncKeyboard(ByVal iKeyCode As Long)     ' 输入的按键值,00H-3FH则键被按下,0FFH则按下的键被释放
' 以下方法只适用于IDC_K0至IDC_K63完全有序的情况
    Static mKeyNo As Long
    If iKeyCode = &HFF Then      ' 释放刚按下的键
         Command2(mKeyNo).Caption = CStr(mKeyNo)     '恢复显示键号
         
    Else                         ' 键被按下
        mKeyNo = iKeyCode And &H3F   ' 键号0-63
        Command2(mKeyNo).Caption = "◎"   ' 00H-3FH键被按下则显示◎
        
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)     '退出窗体
    CH375SetDeviceNotify mIndex, vbNullString, 0&    '取设备插拔通知
    CH375SetIntRoutine mIndex, 0&                    '取消中断上传
    CH375CloseDevice (mIndex)     '程序退出关闭设备
End Sub

Private Function mDownloadData(ByRef iBuffer As String, ByVal iLength As Long) As Long

    Dim mDemoReq   As COMMAND_PACKET
    Dim mLength As Long
    If (iLength > 4096) Then
    
        MsgBox "单次下传数据长度超过4096字节", vbExclamation, mCaptionInform
        mDownloadData = 0
        Exit Function
    End If
    mDemoReq.mCommandCode = DEF_CMD_DOWN_DATA  ' 连续下传数据块
    mDemoReq.mCommandCodeNot = &HFF - DEF_CMD_DOWN_DATA
    mDemoReq.mParameter(0) = ACCESS_MCS51_XRAM  ' 读写51单片机的外部RAM(本演示板的单片机没有外部RAM,所以无法演示)
    mDemoReq.mParameter(1) = &H0   ' 指定外部RAM缓冲区起始地址,该例是将数据下传到起始地址为0X8200的外部RAM
    mDemoReq.mParameter(2) = &H82
    mDemoReq.mParameter(3) = iLength  ' 传输数据总长度
    mLength = CONST_CMD_LEN    ' 命令包的长度
    If (CH375Writedata(mIndex, mDemoReq, mLength)) Then      ' 通过CH375发送命令包,成功
    
        mLength = iLength       ' 数据块的长度,一次下传不超过4096字节
        If ((mLength Mod 64) = CONST_CMD_LEN) Then
            mLength = mLength + 1  ' 防止数据包的长度与命令包的长度相同,如果相同,则多发送一个无效数据
        End If
        If (CH375Writedata(mIndex, iBuffer, mLength)) Then       ' 通过CH375发送数据,成功
            mDownloadData = mLength
            Return
        Else
            MsgBox "CH375WriteData 下传数据失败", vbExclamation, mCaptionInform
        End If
    Else
        MsgBox "CH375WriteData 发送命令失败,DEF_CMD_DOWN_DATA", vbExclamation, mCaptionInform
    End If
    mDownloadData = 0
    Return
End Function

Private Function mUploadData(ByRef iBuffer As String, ByVal iLength As Long) As Long
    Dim mDemoReq As COMMAND_PACKET
    Dim mLength As Long
    If (iLength > 4096) Then
    
        MsgBox "单次上传数据长度超过4096字节", vbQuestion, mCaptionInform
        mUploadData = 0
        Return
    End If
    mDemoReq.mCommandCode = DEF_CMD_CLEAR_UP    ' 连续上传数据块之前进行同步,实际是让单片机清除上传缓冲区的已有内容
    mDemoReq.mCommandCodeNot = &HFF - DEF_CMD_CLEAR_UP
    mLength = CONST_CMD_LEN    ' 命令包的长度
    If (CH375Writedata(mIndex, mDemoReq, mLength)) Then       ' 通过CH375发送命令包,成功
    
        mDemoReq.mCommandCode = DEF_CMD_UP_DATA     ' 连续上传数据块
        mDemoReq.mCommandCodeNot = &HFF - DEF_CMD_UP_DATA
        mDemoReq.mParameter(0) = ACCESS_MCS51_XRAM      ' 读写51单片机的外部RAM(本演示板的单片机没有外部RAM,所以无法演示)
        mDemoReq.mParameter(1) = &H8200         ' 指定外部RAM缓冲区起始地址,该例是将从起始地址为0X8200的外部RAM上传数据
        mDemoReq.mParameter(3) = iLength  ' 传输数据总长度
        mLength = CONST_CMD_LEN     '命令包的长度
        If (CH375Writedata(mIndex, mDemoReq, mLength)) Then        ' 通过CH375发送命令包,成功
        
            mLength = iLength   ' 数据块的长度,一次上传不超过4096字节
            If (CH375ReadData(mIndex, iBuffer, mLength)) Then        ' 通过CH375接收数据,成功
                mUploadData = mLength
                Return
            Else
                MsgBox "CH375ReadData 上传数据失败", vbExclamation, mCaptionInform
            End If
        
        Else
            MsgBox "CH375WriteData 发送命令失败,DEF_CMD_UP_DATA", vbExclamation, mCaptionInform
        End If
            
    
    Else
        MsgBox "CH375WriteData 发送命令失败,DEF_CMD_CLEAR_UP", vbExclamation, mCaptionInform
    End If
    mUploadData = 0
    Return
End Function




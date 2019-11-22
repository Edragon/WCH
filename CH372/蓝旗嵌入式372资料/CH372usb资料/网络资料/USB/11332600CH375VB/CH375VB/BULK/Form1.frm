VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7740
   ClientLeft      =   165
   ClientTop       =   450
   ClientWidth     =   8970
   LinkTopic       =   "Form1"
   ScaleHeight     =   7740
   ScaleWidth      =   8970
   StartUpPosition =   2  '屏幕中心
   Begin ComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   375
      Left            =   0
      TabIndex        =   6
      Top             =   7365
      Width           =   8970
      _ExtentX        =   15822
      _ExtentY        =   661
      SimpleText      =   ""
      _Version        =   327682
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   2
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            Text            =   "ErrorCount=0"
            TextSave        =   "ErrorCount=0"
            Key             =   ""
            Object.Tag             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.TextBox TxtErr 
      Height          =   375
      Left            =   7560
      TabIndex        =   4
      Top             =   6840
      Width           =   975
   End
   Begin VB.ListBox List1 
      Height          =   6000
      Left            =   240
      TabIndex        =   3
      Top             =   720
      Width           =   8295
   End
   Begin VB.CommandButton CmdClear 
      Caption         =   "Clear"
      Height          =   375
      Left            =   7200
      TabIndex        =   2
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton CmdClose 
      Caption         =   "Close"
      Height          =   375
      Left            =   6000
      TabIndex        =   1
      Top             =   240
      Width           =   1095
   End
   Begin VB.CheckBox ChkTest 
      Caption         =   "Test"
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   735
   End
   Begin VB.Label Label3 
      Caption         =   "ErrorCount"
      Height          =   255
      Left            =   6240
      TabIndex        =   5
      Top             =   6960
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim i, mLength, mTestCount, mErrCnt, mStep, mTotal As Long
Dim speed As Double
Dim IsOpen As Byte

'测试
Private Sub ChkTest_Click()
    Dim mBuffer As mBuf
    Dim mReadBuf As mBuf
    
    If (ChkTest.Value = 1) Then
        If (Not IsOpen) Then
            IsOpen = mOpenDevice()
            If (Not IsOpen) Then
                Exit Sub
            End If
        End If
            mErrCnt = 0
            StatusBar1.Panels.Item(1).Text = "Testing..."
            TxtErr.Text = mErrCnt
            List1.AddItem ("*** Step-0: notice MCU start test ")
            Call SetCurSel
            
            mBuffer.mBuff(0) = TEST_START ' 我们在计算机和单片机的应用程序之间约定下传数据的首字节是命令码
            mTotal = 1
        On Error Resume Next
            If (CH375WriteData(0, mBuffer, mTotal)) Then '发送成功
                List1.AddItem ("start test now")
                Call SetCurSel
            Else           ' 写操作失败
                mErrCnt = mErrCnt + 1
                List1.AddItem ("S0-T0 CH375WriteData return error, length=1")
                Call SetCurSel
            End If
            Call Wait
            
            mBuffer.mBuff(0) = TEST_START
            mTotal = 1
        On Error Resume Next
            If (CH375WriteData(0, mBuffer, mTotal)) Then       ' 发送成功
            
            Else        ' 写操作失败
                mErrCnt = mErrCnt + 1
                List1.AddItem ("S0-T1 CH375WriteData return error, length=1")
            End If
            Call Wait
        
            '************************correctness test*********************************
            List1.AddItem ("*** Step-1: test data correctness: 2000 times, random length and data")
            Call SetCurSel
        On Error Resume Next
            For mTestCount = 0 To 1999    ' 循环测试
                mStep = mTestCount And &H3
                Select Case mStep
                    Case 0
                        For i = 0 To 63         ' 发送全00H/返回全FFH
                            mBuffer.mBuff(i) = &H0
                        Next i
                    Case 1
                        For i = 0 To 63
                            mBuffer.mBuff(i) = &HFF    ' 发送全FFH/返回全00H
                        Next i
                    Case Else
                        For i = 0 To 63
                            mBuffer.mBuff(i) = Int((255 * Rnd) + 1)  ' 发送随机数/返回位反码
                        Next i
                End Select
                mBuffer.mBuff(0) = TEST_DATA
                mTotal = Int((100 * Rnd) + 1) ' 随机数长度
                mTotal = mTotal Mod 100
                If (mTotal = 0 Or mTotal > 64) Then
                    mTotal = (mTotal And &H1)
                    If (mTotal >= 1) Then
                        mTotal = 64
                    Else
                        mTotal = 63
                    End If
                End If
                    
                Dim strtempcrr As String
                strtempcrr = "Cnt=" & mTestCount & ",Len=" & mTotal & ",Data: " & Hex(mBuffer.mBuff(0)) & "," & Hex(mBuffer.mBuff(1)) & "," & Hex(mBuffer.mBuff(2)) & "," & Hex(mBuffer.mBuff(3))
                List1.AddItem (strtempcrr)
            On Error Resume Next
                If (CH375WriteData(0, mBuffer, mTotal)) Then      ' 发送成功
                
                    mLength = 64
                    If (CH375ReadData(0, mReadBuf, mLength)) Then         ' 接收成功
                    
                        If (mLength <> mTotal Or mLength = 0) Then
                            mErrCnt = mErrCnt + 1
                            TxtErr.Text = mErrCnt
                            Dim strtemplerr As String
                            strtemplerr = "S1-T" & mStep & "-C" & mTestCount & " return length error:" & mLength & " (" & mTotal & ")"
                            List1.AddItem (strtemplerr)
                        Else
                            For i = 0 To mLength - 1
                                If (&HFF - mReadBuf.mBuff(i)) <> mBuffer.mBuff(i) Then
                                    mErrCnt = mErrCnt + 1
                                    TxtErr.Text = mErrCnt
                                    Dim strtempderr As String
                                    strtempderr = "S1-T" & mStep & "-C" & mTestCount & " return data error at " & i & ":" & mReadBuf.mBuff(i) & " (" & mBuffer.mBuff(i) & ")"
                                    List1.AddItem (strtempderr)
                                End If
                            Next i
                        End If
                    
                    Else   ' 读操作失败
                        mErrCnt = mErrCnt + 1
                        TxtErr.Text = mErrCnt
                        Dim strtemprerr As String
                        strtemprerr = "S1-T" & mStep & "-C" & mTestCount & " CH375ReadData return error, length=" & mTotal
                        List1.AddItem (strtemprerr)
                    End If
                
                Else   ' 写操作失败
                    mErrCnt = mErrCnt + 1
                    TxtErr.Text = mErrCnt
                    Dim strtempwerr As String
                    strtempwerr = "S1-T" & mStep & "-C" & mTestCount & " CH375WriteData return error, length=" & mTotal
                    List1.AddItem (strtempwerr)
                End If
            Next mTestCount
            
            If (mErrCnt = 0) Then
                List1.AddItem ("*** passed                                      ")
            End If
            Call SetCurSel
            Call Wait
            
            '************************************下传速度测试********************************
            List1.AddItem ("*** Step-2: test speed of download data: 2048KB data ")
            Call SetCurSel
            
            On Error Resume Next
            For i = 0 To 4095
                mBuffer.mBuff(i) = Int((255 * Rnd) + 1)   ' 产生随机数以发送
            Next i
            For i = 0 To 4095 Step 64
                mBuffer.mBuff(i) = TEST_DOWNLOAD  ' 每64字节为一个数据包,首字节为命令码
            Next i
            mTotal = 0
            mStep = GetTickCount()
            For mTestCount = 0 To 499   ' 循环测试
            On Error Resume Next
                mBuffer.mBuff(5) = (mTestCount And &HFF00) \ &H100
                mBuffer.mBuff(4) = mTestCount And &HFF
                mLength = 4096
            On Error Resume Next
                If (CH375WriteData(0, mBuffer, mLength)) Then        ' 发送成功
                
                    If (mLength <> 4096) Then
                        Dim strtempdlerr As String
                        strtempdlerr = "S2-C" & mTestCount & " CH375WriteData actual length short " & mLength & "(4096)"
                        List1.AddItem (strtempdlerr)
                    End If
                        
                    mTotal = mTotal + mLength ' 累计长度
                
                Else   ' 写操作失败
                    mErrCnt = mErrCnt + 1
                    Dim strtempdwerr As String
                    strtempdwerr = "S2-C" & mTestCount & " CH375WriteData return error"
                    List1.AddItem (strtempdwerr)
                End If
            Next mTestCount
            mLength = GetTickCount()
            mLength = mLength - mStep
            speed = 1000
            If (mLength <> 0) Then
                speed = speed * mTotal / mLength
            Else
                speed = 9999999
            End If
            Dim strtempdspeed As String
            strtempdspeed = "*** download speed = " & speed & " Bytes/Sec, total=" & mTotal & " bytes, time=" & mLength & "mS"
            List1.AddItem (strtempdspeed)
            Call SetCurSel
            Call Wait
        
            '*********************************上传速度测试**************************
            List1.AddItem ("*** Step-3: test speed of upload data: 2048KB data")
            Call SetCurSel
            mBuffer.mBuff(0) = TEST_UPLOAD
            mLength = 1
        On Error Resume Next
            If (CH375WriteData(0, mBuffer, mLength)) Then     ' 发送成功
            
                mTotal = 0
                mStep = GetTickCount()
                For mTestCount = 0 To 499  ' 循环测试
                
                    mLength = 4096
                On Error Resume Next
                    If (CH375ReadData(0, mBuffer, mLength)) Then         ' 接收成功
                    
                        If (mLength <> 4096) Then
                            Dim strtempulerr As String
                            strtempulerr = "S3-C" & mTestCount & " CH375ReadData actual length short " & mLength & " (4096)"
                            List1.AddItem (strtempulerr)
                        End If
                        mTotal = mTotal + mLength   ' 累计长度
                    Else   ' 读操作失败
                        mErrCnt = mErrCnt + 1
                        TxtErr.Text = mErrCnt
                        Dim strtempurerr As String
                        strtempurerr = "S3-C" & mTestCount & " CH375ReadData return error"
                        List1.AddItem (strtempurerr)
                    End If
                Next mTestCount
                mLength = GetTickCount()
                mLength = mLength - mStep
                speed = 1000
                If (mLength <> 0) Then
                    speed = speed * mTotal / mLength
                Else
                    speed = 9999999
                End If
                Dim strtempuspeed As String
                strtempuspeed = "*** upload speed = " & speed & " Bytes/Sec, total=" & mTotal & " bytes, time=" & mLength & " mS"
                List1.AddItem (strtempuspeed)
                Call SetCurSel
            Else        ' 写操作失败
              
                mErrCnt = mErrCnt + 1
                TxtErr.Text = mErrCnt
                List1.AddItem ("S3 CH375WriteData return error")
                Call SetCurSel
            End If
           Call Wait
        
        ' 关闭CH375设备,如果不继续使用则必须关闭设备,就象读写完硬盘中的数据文件后要关闭一样
            Dim strtemp As String
            strtemp = "*** Total error = " & mErrCnt
            List1.AddItem (strtemp)
            List1.AddItem ("*** CH375CloseDevice: 0 ")
        On Error Resume Next
            CH375CloseDevice (0)
            IsOpen = False
            List1.AddItem ("Exit.")
            ChkTest.Value = False
            Call SetCurSel
            StatusBar1.Panels.Item(1).Text = "Test Over"
            StatusBar1.Panels.Item(2).Text = "ErrorCount = " & mErrCnt
       
    Else
        Call Wait
    On Error Resume Next
        CH375CloseDevice (0)
            IsOpen = False
        List1.AddItem ("设备已关闭。")
        ChkTest.Value = False
    End If
    Call SetCurSel
End Sub

'清空列表框
Private Sub CmdClear_Click()
    List1.Clear
End Sub

Private Sub CmdClose_Click()
On Error Resume Next
    CH375CloseDevice (0)
    IsOpen = False
    List1.AddItem ("设备已关闭")
    Unload Me
End Sub

'初始化，加载DLL
Private Sub Form_Load()
    ChkTest.Value = 0
    List1.AddItem ("CH372/CH375 Bulk Data Test Program V1.0 ,   Copyright (C) W.ch 2003.12")
    List1.AddItem ("test data correctness and USB speed")
On Error Resume Next
    '需要使用DLL则需要先加载
    List1.AddItem ("*** Load DLL: CH375DLL.DLL ")
    
    If (LoadLibrary("CH375DLL.DLL") = Null) Then
        Exit Sub                      ' 加载DLL失败,可能未安装到系统中
    End If
    
    IsOpen = mOpenDevice()
    
End Sub

'等待子程序
Private Sub Wait()
    Dim x As Integer
    x = 1000
    While x
        x = x - 1
    Wend
End Sub

'设置当前选项在最底端
Private Sub SetCurSel()
    Dim lstid As Integer
    If (List1.ListCount > 1) Then
        lstid = List1.ListCount - 1
   Else
        lstid = List1.ListCount
   End If
   List1.Selected(lstid) = True
End Sub

'关闭窗口前先关闭设备
Private Sub Form_Unload(Cancel As Integer)
    If (IsOpen) Then
        Call Wait
    On Error Resume Next
        CH375CloseDevice (0)
        IsOpen = False
    End If
    
End Sub

Private Function mOpenDevice() As Boolean
    On Error Resume Next
     If (CH375OpenDevice(0) = -1) Then   ' 打开设备
        List1.AddItem ("设备打开失败，请检查设备和链接库")
        IsOpen = False
        mOpenDevice = False
    Else
        IsOpen = True
        List1.AddItem ("*** CH375OpenDevice: 0# ")
        mOpenDevice = True
    End If
End Function

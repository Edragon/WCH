VERSION 5.00
Begin VB.Form frmEnter 
   Caption         =   "Form2"
   ClientHeight    =   1245
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4080
   LinkTopic       =   "Form2"
   ScaleHeight     =   1245
   ScaleWidth      =   4080
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2400
      TabIndex        =   3
      Top             =   720
      Width           =   975
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   2400
      TabIndex        =   2
      Top             =   240
      Width           =   975
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   600
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "输入数据"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "frmEnter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public index As Integer   '按下的按钮序号
Const DEMO_CH451_CMD = &H56

Private Sub cmdCancel_Click()
    mEnterBuffer = ""          '放弃按钮
    Unload Me
End Sub

Private Sub cmdOK_Click()
    Dim ilen As Integer
    Dim mDemoReq As COMMAND_PACKET
    mEnterBuffer = Text1.Text
    If index <> -1 Then              '数码管对应的按键
        Dim mNumber As Byte
        If Text1.Text <> "" Then
            Dim mBcdCode1 As Byte
            Dim mLength As Long
            
            If mEnterBuffer = " " Then
                mBcdCode1 = &H10                 ' 空格
            ElseIf mEnterBuffer = "=" Then
                mBcdCode1 = &H13
            ElseIf mEnterBuffer = "." Then
                mBcdCode1 = &H1A
            Else
                mBcdCode1 = mCharToBcd(Left(mEnterBuffer, 1))
            End If
            ilen = Len(mEnterBuffer)
            If ilen >= 2 And Right(Left(mEnterBuffer, 2), 1) = "." Then
                mBcdCode1 = mBcdCode1 Or &H80
            End If
            If mBcdCode1 <> &HFF Then            '输入的字符有效
                mNumber = index    ' 如果IDC_N1至IDC_N8有序,则结果为0-7,分别对应于各个数码管按钮
                mNumber = 7 - mNumber
                mDemoReq.mCommandCode = DEMO_CH451_CMD
                mDemoReq.mCommandCodeNot = &HFF - DEMO_CH451_CMD
                mDemoReq.mParameter(2) = &H8 Or mNumber      ' CH451命令:加载字数据到指定数码管
                mDemoReq.mParameter(1) = mBcdCode1            ' 字数据,对应于BCD译码方式
                mLength = CONST_CMD_LEN    ' 命令包的长度
                On Error Resume Next
                If CH375WriteData(mIndex, mDemoReq, mLength) Then    ' 通过CH371发送数据,成功
                ' 如果输入时mLength大于8,则成功返回时,为了更加可靠,可以检查实际发送的长度是否符合
                     mEnterBuffer = Left(mEnterBuffer, 1) ' 字符串结束符,只要一个字符
                    frmMain.Command1(index).Caption = Left(mEnterBuffer, 1)  '设置按钮上的字符
                Else
                   MsgBox "CH375WriteData 失败！", vbCritical, "信息提示"
                End If
            Else
                MsgBox "请输入字符0-9,A-F或者空格", vbExclamation, "信息提示"
            End If
        Else
            MsgBox "请至少输入一个字符0-9，A-F或者空格", vbExclamation, "信息提示"
        End If
        index = 0
        Unload Me
    ElseIf index = -1 Then       '命令按钮触发
        If Len(mEnterBuffer) >= 3 Then    ' 已经输入数据,并且长度超过3位
           ReDim mBcdCode(2) As Byte
            mBcdCode(0) = mCharToBcd(Left(mEnterBuffer, 1))
            mBcdCode(1) = mCharToBcd(Right(Left(mEnterBuffer, 2), 1))
            mBcdCode(2) = mCharToBcd(Right(Left(mEnterBuffer, 3), 1))
            If mBcdCode(0) <> &HFF And mBcdCode(1) <> &HFF And mBcdCode(2) <> &HFF Then    ' 输入的三个字符都有效
                mDemoReq.mCommandCode = DEMO_CH451_CMD
                mDemoReq.mCommandCodeNot = &HFF - DEMO_CH451_CMD
                mDemoReq.mParameter(2) = mBcdCode(0)  ' CH451命令:高4位
                mDemoReq.mParameter(1) = mBcdCode(1) * &H10 Or mBcdCode(2) ' 低8位的字节数据
                mLength = CONST_CMD_LEN    ' 命令包的长度
                If CH375WriteData(mIndex, mDemoReq, mLength) Then    ' 通过CH375发送数据,成功
                    MsgBox "命令成功发送到CH451", vbExclamation, "信息提示"
                Else
                    MsgBox "CH375WriteData 失败", vbCritical, "信息提示"
                End If
            Else
                MsgBox "请输入3个字符0-9,A-F", vbExclamation, "信息提示"
            End If
        Else
            MsgBox "请至少输入3个字符0-9,A-F", vbExclamation, "信息提示"
        End If
        Unload Me
    End If
End Sub

Private Function mCharToBcd(ByVal iChar As String) As Byte ' 输入的ASCII字符
    Dim mBCD As Byte
    If iChar >= "0" And iChar <= "9" Then
        mBCD = iChar - "0"
    ElseIf iChar >= "A" And iChar <= "F" Then
        mBCD = Asc(iChar) - Asc("A") + &HA
    ElseIf iChar >= "a" And iChar <= "f" Then
        mBCD = Asc(iChar) - Asc("a") + &HA
    Else
        mBCD = &HFF
    End If
    mCharToBcd = mBCD
End Function


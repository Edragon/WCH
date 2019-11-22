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
   StartUpPosition =   2  '��Ļ����
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
      Caption         =   "��������"
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
Public index As Integer   '���µİ�ť���
Const DEMO_CH451_CMD = &H56

Private Sub cmdCancel_Click()
    mEnterBuffer = ""          '������ť
    Unload Me
End Sub

Private Sub cmdOK_Click()
    Dim ilen As Integer
    Dim mDemoReq As COMMAND_PACKET
    mEnterBuffer = Text1.Text
    If index <> -1 Then              '����ܶ�Ӧ�İ���
        Dim mNumber As Byte
        If Text1.Text <> "" Then
            Dim mBcdCode1 As Byte
            Dim mLength As Long
            
            If mEnterBuffer = " " Then
                mBcdCode1 = &H10                 ' �ո�
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
            If mBcdCode1 <> &HFF Then            '������ַ���Ч
                mNumber = index    ' ���IDC_N1��IDC_N8����,����Ϊ0-7,�ֱ��Ӧ�ڸ�������ܰ�ť
                mNumber = 7 - mNumber
                mDemoReq.mCommandCode = DEMO_CH451_CMD
                mDemoReq.mCommandCodeNot = &HFF - DEMO_CH451_CMD
                mDemoReq.mParameter(2) = &H8 Or mNumber      ' CH451����:���������ݵ�ָ�������
                mDemoReq.mParameter(1) = mBcdCode1            ' ������,��Ӧ��BCD���뷽ʽ
                mLength = CONST_CMD_LEN    ' ������ĳ���
                On Error Resume Next
                If CH375WriteData(mIndex, mDemoReq, mLength) Then    ' ͨ��CH371��������,�ɹ�
                ' �������ʱmLength����8,��ɹ�����ʱ,Ϊ�˸��ӿɿ�,���Լ��ʵ�ʷ��͵ĳ����Ƿ����
                     mEnterBuffer = Left(mEnterBuffer, 1) ' �ַ���������,ֻҪһ���ַ�
                    frmMain.Command1(index).Caption = Left(mEnterBuffer, 1)  '���ð�ť�ϵ��ַ�
                Else
                   MsgBox "CH375WriteData ʧ�ܣ�", vbCritical, "��Ϣ��ʾ"
                End If
            Else
                MsgBox "�������ַ�0-9,A-F���߿ո�", vbExclamation, "��Ϣ��ʾ"
            End If
        Else
            MsgBox "����������һ���ַ�0-9��A-F���߿ո�", vbExclamation, "��Ϣ��ʾ"
        End If
        index = 0
        Unload Me
    ElseIf index = -1 Then       '���ť����
        If Len(mEnterBuffer) >= 3 Then    ' �Ѿ���������,���ҳ��ȳ���3λ
           ReDim mBcdCode(2) As Byte
            mBcdCode(0) = mCharToBcd(Left(mEnterBuffer, 1))
            mBcdCode(1) = mCharToBcd(Right(Left(mEnterBuffer, 2), 1))
            mBcdCode(2) = mCharToBcd(Right(Left(mEnterBuffer, 3), 1))
            If mBcdCode(0) <> &HFF And mBcdCode(1) <> &HFF And mBcdCode(2) <> &HFF Then    ' ����������ַ�����Ч
                mDemoReq.mCommandCode = DEMO_CH451_CMD
                mDemoReq.mCommandCodeNot = &HFF - DEMO_CH451_CMD
                mDemoReq.mParameter(2) = mBcdCode(0)  ' CH451����:��4λ
                mDemoReq.mParameter(1) = mBcdCode(1) * &H10 Or mBcdCode(2) ' ��8λ���ֽ�����
                mLength = CONST_CMD_LEN    ' ������ĳ���
                If CH375WriteData(mIndex, mDemoReq, mLength) Then    ' ͨ��CH375��������,�ɹ�
                    MsgBox "����ɹ����͵�CH451", vbExclamation, "��Ϣ��ʾ"
                Else
                    MsgBox "CH375WriteData ʧ��", vbCritical, "��Ϣ��ʾ"
                End If
            Else
                MsgBox "������3���ַ�0-9,A-F", vbExclamation, "��Ϣ��ʾ"
            End If
        Else
            MsgBox "����������3���ַ�0-9,A-F", vbExclamation, "��Ϣ��ʾ"
        End If
        Unload Me
    End If
End Sub

Private Function mCharToBcd(ByVal iChar As String) As Byte ' �����ASCII�ַ�
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


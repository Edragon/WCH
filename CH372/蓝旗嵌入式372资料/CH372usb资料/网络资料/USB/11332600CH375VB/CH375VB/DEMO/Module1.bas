Attribute VB_Name = "Module1"

Option Explicit

Public mEnterBuffer As String
Public mBufferLength As Long       ' �������ڵ���Ч���ݳ���
Public mShowLED As Integer
Public mIndex As Long
Public mKeyCode As Integer

Type INT_PARA  '�ж϶˵��ϴ�����������
   buf(8) As Byte
End Type

Type COMMAND_PACKET     ' �Զ����������ṹ
    mCommandCode As Byte      ' ����������,������Ķ���
    mCommandCodeNot As Byte    ' ������ķ���,����У�������
    mParameter(4) As Byte   ' ����
            '  mParamter(4)�൱�� UCHAR   mBufferID;  // ������ʶ����,���������MCS51��Ƭ������: 1-ר�ù��ܼĴ���SFR, 2-�ڲ�RAM, 3-�ⲿRAM, ����������ʵ��ֻ��ʾ�ڲ�RAM
            '  USHORT  mBufferAddr;    // ��д��������ʼ��ַ,Ѱַ��Χ��0000H-0FFFFH,���ֽ���ǰ
            '  USHORT  mLength;    // ���ݿ��ܳ���,���ֽ���ǰ
End Type
Dim mCOMMAND_PACKET As COMMAND_PACKET

Public Const CONST_CMD_LEN = &H7           ' �����ĳ���
' �������������ݶ���ͨ�������´��ܵ�(USB�˵�2��OUT)�´�, Ϊ�˷�ֹ���߻���,
' ���ǿ����ڼ����Ӧ�ó����뵥Ƭ������֮��Լ��, �����ĳ�������7, �����ݿ�ĳ��ȿ϶�����7, ����64,32��
' ����, ����Լ��, ���������ֽ���������, �ȵ�
' ������Լ��������: 80H-0FFH��ͨ������,�����ڸ���Ӧ��
'                   00H-7FH��ר������,��Ը���Ӧ���ر���
' ͨ������
Public Const DEF_CMD_GET_INFORM = &H90          ' ��ȡ��λ����˵����Ϣ,���Ȳ�����64���ַ�,�ַ�����00H����
Public Const DEF_CMD_TEST_DATA = &H91            ' ��������,��λ����PC�����������������������ȡ���󷵻�
Public Const DEF_CMD_CLEAR_UP = &HA0              ' ���ϴ����ݿ�֮ǰ����ͬ��,ʵ��������λ������ϴ�����������������
Public Const DEF_CMD_UP_DATA = &HA1             ' ����λ����ָ����ַ�Ļ������ж�ȡ���ݿ�(�ϴ����ݿ�)
Public Const DEF_CMD_DOWN_DATA = &HA2           ' ����λ����ָ����ַ�Ļ�������д�����ݿ�(�´����ݿ�)
' ר������
Public Const DEMO_CH451_CMD = &H56               ' PC���������CH451,������ʾCH451�Ĺ���
' ����MCS51��Ƭ����ʹ��ͨ������ʱ,����Ҫָ��������ʶ����
Public Const ACCESS_MCS51_SFR = 1                 ' ��д51��Ƭ����SFR
Public Const ACCESS_MCS51_IRAM = 2               ' ��д51��Ƭ�����ڲ�RAM
Public Const ACCESS_MCS51_XRAM = 3               ' ��д51��Ƭ�����ⲿRAM
Public Const WM_KEYUP = &H101
Public Const WM_KEYDOWN = &H100

'�жϷ��������������ж���Ϣ
Public Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


Public Sub mCallInt()
    Dim x As Boolean
    x = CH375SetIntRoutine(mIndex, AddressOf mInterruptEvent)    '�����ж��ӳ���
    If x = 0 Then
        MsgBox "�޷������ж�" & x, vbCritical, "��Ϣ��ʾ"
        Exit Sub
    End If
End Sub



'iBuffer ָ��һ��������,�ṩ��ǰ���ж���������
'******����˳���Ҫ���öϵ�******
Public Sub mInterruptEvent(ByRef mBuffer As INT_PARA)                '�жϷ������
   Dim bool As Boolean
     ' CH371�����жϺ�,����һ����Ϣ֪ͨ������
    If mBuffer.buf(0) = 1 Then        ' ����ж���������
        mKeyCode = mBuffer.buf(1) And &H3F
        bool = PostMessage(frmMain.hwnd, WM_KEYDOWN, 1, 0)                  ' �ж���������1���������
    Else
        If mBuffer.buf(0) = 2 Then
           bool = PostMessage(frmMain.hwnd, WM_KEYUP, 1, 0)                ' �ж���������2������ͷ�
        End If
    End If
End Sub

Public Sub iNotifyRoutine(ByVal iEventStatus As Long)     ' �豸�¼��͵�ǰ״̬(�����ж���): 0=�豸�γ��¼�, 3=�豸�����¼�
   PostMessage frmMain.pnpproce.hwnd, WM_KEYUP, iEventStatus, 0
End Sub




Attribute VB_Name = "Module1"

Option Explicit

Public mEnterBuffer As String
Public mBufferLength As Long       ' 缓冲区内的有效数据长度
Public mShowLED As Integer
Public mIndex As Long
Public mKeyCode As Integer

Type INT_PARA  '中断端点上传的数据类型
   buf(8) As Byte
End Type

Type COMMAND_PACKET     ' 自定义的命令包结构
    mCommandCode As Byte      ' 命令请求码,见下面的定义
    mCommandCodeNot As Byte    ' 命令码的反码,用于校验命令包
    mParameter(4) As Byte   ' 参数
            '  mParamter(4)相当于 UCHAR   mBufferID;  // 缓冲区识别码,本程序针对MCS51单片机定义: 1-专用功能寄存器SFR, 2-内部RAM, 3-外部RAM, 不过本程序实际只演示内部RAM
            '  USHORT  mBufferAddr;    // 读写操作的起始地址,寻址范围是0000H-0FFFFH,低字节在前
            '  USHORT  mLength;    // 数据块总长度,低字节在前
End Type
Dim mCOMMAND_PACKET As COMMAND_PACKET

Public Const CONST_CMD_LEN = &H7           ' 命令块的长度
' 由于命令与数据都是通过数据下传管道(USB端点2的OUT)下传, 为了防止两者混淆,
' 我们可以在计算机应用程序与单片机程序之间约定, 命令块的长度总是7, 而数据块的长度肯定不是7, 例如64,32等
' 另外, 可以约定, 命令块的首字节是命令码, 等等
' 本程序约定命令码: 80H-0FFH是通用命令,适用于各种应用
'                   00H-7FH是专用命令,针对各种应用特别定义
' 通用命令
Public Const DEF_CMD_GET_INFORM = &H90          ' 获取下位机的说明信息,长度不超过64个字符,字符串以00H结束
Public Const DEF_CMD_TEST_DATA = &H91            ' 测试命令,下位机将PC机发来的命令包的所有数据取反后返回
Public Const DEF_CMD_CLEAR_UP = &HA0              ' 在上传数据块之前进行同步,实际是让下位机清除上传缓冲区的已有内容
Public Const DEF_CMD_UP_DATA = &HA1             ' 从下位机的指定地址的缓冲区中读取数据块(上传数据块)
Public Const DEF_CMD_DOWN_DATA = &HA2           ' 向下位机的指定地址的缓冲区中写入数据块(下传数据块)
' 专用命令
Public Const DEMO_CH451_CMD = &H56               ' PC发送命令给CH451,用于演示CH451的功能
' 对于MCS51单片机在使用通用命令时,还需要指定缓冲区识别码
Public Const ACCESS_MCS51_SFR = 1                 ' 读写51单片机的SFR
Public Const ACCESS_MCS51_IRAM = 2               ' 读写51单片机的内部RAM
Public Const ACCESS_MCS51_XRAM = 3               ' 读写51单片机的外部RAM
Public Const WM_KEYUP = &H101
Public Const WM_KEYDOWN = &H100

'中断发生给主程序发送中断消息
Public Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


Public Sub mCallInt()
    Dim x As Boolean
    x = CH375SetIntRoutine(mIndex, AddressOf mInterruptEvent)    '调用中断子程序
    If x = 0 Then
        MsgBox "无法调用中断" & x, vbCritical, "信息提示"
        Exit Sub
    End If
End Sub



'iBuffer 指向一个缓冲区,提供当前的中断特征数据
'******建议此程序不要设置断点******
Public Sub mInterruptEvent(ByRef mBuffer As INT_PARA)                '中断服务程序
   Dim bool As Boolean
     ' CH371产生中断后,产生一个消息通知主程序
    If mBuffer.buf(0) = 1 Then        ' 检查中断特征数据
        mKeyCode = mBuffer.buf(1) And &H3F
        bool = PostMessage(frmMain.hwnd, WM_KEYDOWN, 1, 0)                  ' 中断特征数据1则键被按下
    Else
        If mBuffer.buf(0) = 2 Then
           bool = PostMessage(frmMain.hwnd, WM_KEYUP, 1, 0)                ' 中断特征数据2则键被释放
        End If
    End If
End Sub

Public Sub iNotifyRoutine(ByVal iEventStatus As Long)     ' 设备事件和当前状态(在下行定义): 0=设备拔出事件, 3=设备插入事件
   PostMessage frmMain.pnpproce.hwnd, WM_KEYUP, iEventStatus, 0
End Sub




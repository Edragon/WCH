Attribute VB_Name = "Module2"
Option Explicit

Public Const TEST_START = &H20                ' ���Թ��̿�ʼ
Public Const TEST_DATA = &H21                 ' ����������ȷ��
Public Const TEST_UPLOAD = &H22               ' �����ϴ����ݿ�
Public Const TEST_DOWNLOAD = &H23             ' �����´����ݿ�

Type mBuf
    mBuff(4099) As Byte
End Type

Public Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long


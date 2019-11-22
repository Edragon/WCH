Attribute VB_Name = "Module2"
Option Explicit

Public Const TEST_START = &H20                ' 测试过程开始
Public Const TEST_DATA = &H21                 ' 测试数据正确性
Public Const TEST_UPLOAD = &H22               ' 测试上传数据块
Public Const TEST_DOWNLOAD = &H23             ' 测试下传数据块

Type mBuf
    mBuff(4099) As Byte
End Type

Public Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long


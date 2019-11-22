VERSION 5.00
Begin VB.Form FrmBored 
   Caption         =   "Bored_USB_Printer"
   ClientHeight    =   7035
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10035
   LinkTopic       =   "Form1"
   ScaleHeight     =   7035
   ScaleWidth      =   10035
   StartUpPosition =   3  '����ȱʡ
   Begin VB.Frame Frame3 
      Caption         =   "����ַ�"
      Height          =   735
      Left            =   2040
      TabIndex        =   8
      Top             =   120
      Width           =   1215
      Begin VB.TextBox txtBytesReceived 
         Alignment       =   2  'Center
         Height          =   375
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   9
         Text            =   "FrmBored.frx":0000
         Top             =   240
         Width           =   975
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "д���ַ�"
      Height          =   735
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   1215
      Begin VB.TextBox txtBytesSend 
         Alignment       =   2  'Center
         Height          =   390
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   6
         Text            =   "FrmBored.frx":0006
         Top             =   240
         Width           =   975
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "���ݵ����������"
      Height          =   735
      Left            =   4320
      TabIndex        =   2
      Top             =   120
      Width           =   4575
      Begin VB.CommandButton CmdWR 
         Caption         =   "Write_Read"
         Height          =   375
         Left            =   1740
         TabIndex        =   7
         Top             =   240
         Width           =   1095
      End
      Begin VB.CommandButton CmdEnd 
         Caption         =   "End"
         Height          =   375
         Left            =   3360
         TabIndex        =   4
         Top             =   240
         Width           =   1095
      End
      Begin VB.CommandButton CmdPrint 
         Caption         =   "Print"
         Height          =   375
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   1095
      End
   End
   Begin VB.TextBox Text1 
      Height          =   1455
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   1
      Text            =   "FrmBored.frx":000E
      Top             =   5280
      Width           =   9735
   End
   Begin VB.ListBox lstResults 
      Height          =   3840
      Left            =   120
      TabIndex        =   0
      Top             =   1080
      Width           =   9735
   End
   Begin VB.Timer tmrDelay 
      Left            =   9240
      Top             =   720
   End
End
Attribute VB_Name = "FrmBored"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim Capabilities As HIDP_CAPS
Dim DataString As String
Dim DetailData As Long
Dim DetailDataBuffer() As Byte
Dim DeviceAttributes As HIDD_ATTRIBUTES
Dim DevicePathName As String
Dim DeviceInfoSet As Long
Dim ErrorString As String
Dim HidDevice As Long
Dim LastDevice As Boolean
Dim MyDeviceDetected As Boolean
Dim MyDeviceInfoData As SP_DEVINFO_DATA
Dim MyDeviceInterfaceDetailData As SP_DEVICE_INTERFACE_DETAIL_DATA
Dim MyDeviceInterfaceData As SP_DEVICE_INTERFACE_DATA
Dim Needed As Long
Dim OutputReportData(7) As Byte
Dim PreparsedData As Long
Dim Result As Long
Dim Timeout As Boolean

Dim boredtime As Long

Const MyVendorID = &H1234
Const MyProductID = &H5678
Function FindBoredHID() As Boolean

Dim Count As Integer
Dim GUIDString As String
Dim HidGuid As GUID
Dim MemberIndex As Long

LastDevice = False
MyDeviceDetected = False

Result = HidD_GetHidGuid(HidGuid)
Call DisplayResultOfAPICall("GetHidGuid")

GUIDString = _
    Hex$(HidGuid.Data1) & "-" & _
    Hex$(HidGuid.Data2) & "-" & _
    Hex$(HidGuid.Data3) & "-"

For Count = 0 To 7
  
    If HidGuid.Data4(Count) >= &H10 Then
        GUIDString = GUIDString & Hex$(HidGuid.Data4(Count)) & " "
    Else
        GUIDString = GUIDString & "0" & Hex$(HidGuid.Data4(Count)) & " "
    End If
Next Count

lstResults.AddItem "  GUID for system HIDs: "
lstResults.AddItem "  " & GUIDString
Text1.Text = vbCrLf & "    " + GUIDString
                                            ''''''''''''''''''''''''''''''''''''''''''''''''''''''''
DeviceInfoSet = SetupDiGetClassDevs(HidGuid, vbNullString, 0, _
    (DIGCF_PRESENT Or DIGCF_DEVICEINTERFACE))
    
Call DisplayResultOfAPICall("SetupDiClassDevs")
DataString = GetDataString(DeviceInfoSet, 32)
lstResults.AddItem "  DeviceInfoSet:" & DeviceInfoSet
                                            '''''''''''''''''''''''''''''''''''''''''''''''''''''
MemberIndex = 0
'��0��ʼ������ֱ��û���豸����⵽ΪֹMemberIndex=0
Do
        '��������MyDeviceInterfaceData�ṹ�е�cbSizeԪ��
        '����ṹ���ֽڼ�����ֵ����СΪ28�ֽ�
    MyDeviceInterfaceData.cbSize = LenB(MyDeviceInterfaceData)
    '����ָ��ʶ��ӿڽṹ��ָ�루�豸�����
    Result = SetupDiEnumDeviceInterfaces _
        (DeviceInfoSet, _
        0, _
        HidGuid, _
        MemberIndex, _
        MyDeviceInterfaceData)
    
    Call DisplayResultOfAPICall("SetupDiEnumDeviceInterfaces")
    
        '����豸�����ڣ���ʾ���ص���Ϣ
    If Result = 0 Then
        LastDevice = True
        BoredDisplay ("Result = 0; This is LastDevice")
    End If
    
        '����豸���ڣ���ʾ���صĽ��
    If Result <> 0 Then
        'BoredDisplay ("Result is not zero!")
        lstResults.AddItem "  DeviceInfoSet for device #" & CStr(MemberIndex) & ": "
        lstResults.AddItem "  cbSize = " & CStr(MyDeviceInterfaceData.cbSize)   '=28�ֽ�
        lstResults.AddItem _
            "  InterfaceClassGuid.Data1 = " & Hex$(MyDeviceInterfaceData.InterfaceClassGuid.Data1)
        lstResults.AddItem _
            "  InterfaceClassGuid.Data2 = " & Hex$(MyDeviceInterfaceData.InterfaceClassGuid.Data2)
        lstResults.AddItem _
            "  InterfaceClassGuid.Data3 = " & Hex$(MyDeviceInterfaceData.InterfaceClassGuid.Data3)
        lstResults.AddItem _
            "  Flags = " & Hex$(MyDeviceInterfaceData.Flags)
        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        
        MyDeviceInfoData.cbSize = Len(MyDeviceInfoData)
                                                '�����豸��·��
        Result = SetupDiGetDeviceInterfaceDetail _
           (DeviceInfoSet, _
           MyDeviceInterfaceData, _
           0, _
           0, _
           Needed, _
           0)
        
        DetailData = Needed
        
        Call DisplayResultOfAPICall("Result of First call SetupDiGetDeviceInterfaceDetail:")
        Call BoredDisplay("First call fuction SetupDiGetDeviceInterfaceDetail")
        lstResults.AddItem "  (OK to say too small)"
        lstResults.AddItem "  Required buffer size for the data:" & Needed
                
            '��������ṹ�Ĵ�Сֵ
        MyDeviceInterfaceDetailData.cbSize = Len(MyDeviceInterfaceDetailData)
        
        ReDim DetailDataBuffer(Needed)  'ʹ���ֽ����������ṹMyDeviceInterfaceDetailData���ڴ�
        
            '�����������ǰ4���ֽڣ�cbSize
        Call RtlMoveMemory(DetailDataBuffer(0), MyDeviceInterfaceDetailData, 4)
        
'��һ�ε���MyDeviceInterfaceDetailData()����������DetailDataBufer�е�ÿһ��Ԫ�صĵ�ַ
'���ﷵ�ص���DetailData����Ļ�������С
        Result = SetupDiGetDeviceInterfaceDetail _
           (DeviceInfoSet, _
           MyDeviceInterfaceData, _
           VarPtr(DetailDataBuffer(0)), _
           DetailData, _
           Needed, _
           0)
           
        Call DisplayResultOfAPICall("Result of Second call SetupDiGetDeviceInterfaceDetail:")
        lstResults.AddItem "  MyDeviceInterfaceDetailData.cbSize:" & _
            CStr(MyDeviceInterfaceDetailData.cbSize)
        
        DevicePathName = CStr(DetailDataBuffer())   '���ֽ�����ת�����ַ���
        DevicePathName = StrConv(DevicePathName, vbUnicode) 'ת����Unicode��ʽ
       
        DevicePathName = Right$(DevicePathName, Len(DevicePathName) - 4) '�ӿ�ʼ�����cbSize��4�ֽڣ�
        lstResults.AddItem "  Device pathname: "    '��ʾ�豸��·������
        lstResults.AddItem "    " & DevicePathName
           
            '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                                 '�����豸�ľ��(����豸�������������֧��CreateFile����) 5
        HidDevice = CreateFile _
            (DevicePathName, _
            GENERIC_READ Or GENERIC_WRITE, _
            (FILE_SHARE_READ Or FILE_SHARE_WRITE), _
            0, _
            OPEN_EXISTING, _
            0, _
            0)
            
        Call DisplayResultOfAPICall("CreateFile")
        lstResults.AddItem "  Returned handle: " & Hex$(HidDevice) & "h"
                
                                        ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        DeviceAttributes.Size = LenB(DeviceAttributes) '���á�Size������Ϊ�ṹ�е��ֽ���ֵ
                                                            '���س��̺Ͳ�ƷID  6
        Result = HidD_GetAttributes(HidDevice, DeviceAttributes)
            
        Call DisplayResultOfAPICall("HidD_GetAttributes")
        If Result <> 0 Then
            lstResults.AddItem "  HIDD_ATTRIBUTES structure filled without error."
        Else
            lstResults.AddItem "  Error in filling HIDD_ATTRIBUTES structure."
        End If
    
        lstResults.AddItem "  Structure size: " & DeviceAttributes.Size
        lstResults.AddItem "  Vendor ID: " & Hex$(DeviceAttributes.VendorID)
        lstResults.AddItem "  Product ID: " & Hex$(DeviceAttributes.ProductID)
        lstResults.AddItem "  Version Number: " & Hex$(DeviceAttributes.VersionNumber)
                                                            
                                                            '�ҳ�������豸�Ƿ�������Ѱ�ҵ��豸���
        If (DeviceAttributes.VendorID = MyVendorID) And _
            (DeviceAttributes.ProductID = MyProductID) Then
                lstResults.AddItem "  My device detected"
                MyDeviceDetected = True
        Else
                MyDeviceDetected = False    '������Ҫ���豸����رձ�ʶ���
                Result = CloseHandle(HidDevice)
                DisplayResultOfAPICall ("CloseHandle")
        End If
    End If
        '����Ѱ�ң�ֱ�����ִ��豸�����Ѿ�û��ʣ�µ��豸Ҫ���
    MemberIndex = MemberIndex + 1
    
Loop Until (LastDevice = True) Or (MyDeviceDetected = True)

If MyDeviceDetected = True Then
    FindBoredHID = True
Else
    lstResults.AddItem " Device not found."
End If

End Function
Private Function BoredDisplay(BoredStr As String)
lstResults.AddItem "  bored's message =    " & "#" & BoredStr & "#"
End Function
Private Function GetDataString(Address As Long, Bytes As Long) As String

Dim Offset As Integer
Dim Result$
Dim ThisByte As Byte

For Offset = 0 To Bytes - 1
    Call RtlMoveMemory(ByVal VarPtr(ThisByte), ByVal Address + Offset, 1)
    If (ThisByte And &HF0) = 0 Then
        Result$ = Result$ & "0"
    End If
    Result$ = Result$ & Hex$(ThisByte) & " "
Next Offset

GetDataString = Result$

End Function
Private Function GetErrorString(ByVal LastError As Long) As String

Dim Bytes As Long
Dim ErrorString As String

ErrorString = String$(129, 0)
Bytes = FormatMessage _
    (FORMAT_MESSAGE_FROM_SYSTEM, _
    0&, _
    LastError, _
    0, _
    ErrorString$, _
    128, _
    0)

If Bytes > 2 Then
    GetErrorString = Left$(ErrorString, Bytes - 2)
End If

End Function
Private Sub DisplayResultOfAPICall(FunctionName As String)

Dim ErrorString As String

lstResults.AddItem " ==================================================================== " & boredtime
ErrorString = GetErrorString(Err.LastDllError)
lstResults.AddItem FunctionName
lstResults.AddItem "  Result = " & ErrorString

boredtime = boredtime + 1

End Sub
Private Sub CmdEnd_Click()

End

End Sub
Private Sub CmdPrint_Click()

boredtime = 1
lstResults.Clear
Call FindBoredHID

End Sub

Private Sub CmdWR_Click()
lstResults.Clear
Call ReadAndWriteToDevice
End Sub

Private Sub Form_Load()

txtBytesReceived.Text = ""
txtBytesSend.Text = ""
Text1.Text = ""
FrmBored.Show
tmrDelay.Enabled = False
boredtime = 1

End Sub
Private Sub GetDeviceCapabilities()

Dim ppData(29) As Byte
Dim ppDataString As Variant
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                                '���ذ����豸�����Ļ�����ָ��HidD_GetPreparsedData()���� 7
                                '���������ݣ�PreparsedData��ָ��ָ���������Ļ�����
Result = HidD_GetPreparsedData(HidDevice, PreparsedData)
Call DisplayResultOfAPICall("HidD_GetPreparsedData")
                                                    
                                        '��PreparsedData�е����ݸ��Ƶ��ֽ�������
Result = RtlMoveMemory(ppData(0), PreparsedData, 30)

Call DisplayResultOfAPICall("RtlMoveMemory")
ppDataString = ppData()
ppDataString = StrConv(ppDataString, vbUnicode) '������ת����Unicode��ʽ
        ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Result = HidP_GetCaps(PreparsedData, Capabilities)  '�����豸������ 8

Call DisplayResultOfAPICall("HidP_GetCaps")
lstResults.AddItem "  Last error: " & ErrorString
lstResults.AddItem "  Usage: " & Hex$(Capabilities.Usage)
lstResults.AddItem "  Usage Page: " & Hex$(Capabilities.UsagePage)
lstResults.AddItem "  Input Report Byte Length: " & Capabilities.InputReportByteLength
lstResults.AddItem "  Output Report Byte Length: " & Capabilities.OutputReportByteLength
lstResults.AddItem "  Feature Report Byte Length: " & Capabilities.FeatureReportByteLength
lstResults.AddItem "  Number of Link Collection Nodes: " & Capabilities.NumberLinkCollectionNodes
lstResults.AddItem "  Number of Input Button Caps: " & Capabilities.NumberInputButtonCaps
lstResults.AddItem "  Number of Input Value Caps: " & Capabilities.NumberInputValueCaps
lstResults.AddItem "  Number of Input Data Indices: " & Capabilities.NumberInputDataIndices
lstResults.AddItem "  Number of Output Button Caps: " & Capabilities.NumberOutputButtonCaps
lstResults.AddItem "  Number of Output Value Caps: " & Capabilities.NumberOutputValueCaps
lstResults.AddItem "  Number of Output Data Indices: " & Capabilities.NumberOutputDataIndices
lstResults.AddItem "  Number of Feature Button Caps: " & Capabilities.NumberFeatureButtonCaps
lstResults.AddItem "  Number of Feature Value Caps: " & Capabilities.NumberFeatureValueCaps
lstResults.AddItem "  Number of Feature Data Indices: " & Capabilities.NumberFeatureDataIndices

        ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Dim ValueCaps(1023) As Byte

        '�����豸����ֵ����HidP_GetValueCaps()
Result = HidP_GetValueCaps(HidP_Input, ValueCaps(0), Capabilities.NumberInputValueCaps, PreparsedData)
   
Call DisplayResultOfAPICall("HidP_GetValueCaps")

End Sub
Private Sub ReadAndWriteToDevice()

Dim DeviceDetected As Boolean
Dim temp As String

If Len(Trim(txtBytesSend.Text)) < 2 Then
    txtBytesSend.Text = "0" & txtBytesSend.Text
ElseIf Len(Trim(txtBytesSend.Text)) > 2 Then
    txtBytesSend.Text = Right(txtBytesSend.Text, 2)
End If

txtBytesSend.Text = UCase(txtBytesSend.Text)
OutputReportData(0) = AHex(txtBytesSend.Text)
'OutputReportData(1) = cboByte1.ListIndex

DeviceDetected = FindBoredHID
If DeviceDetected = True Then
    Call GetDeviceCapabilities
    Call WriteReport
    
    Timeout = False
    tmrDelay.Interval = 100
    tmrDelay.Enabled = True
    Do
        DoEvents
    Loop Until Timeout = True
   
    Call ReadReport
Else
End If

'Scroll to the bottom of the list box.
'lstResults.ListIndex = lstResults.ListCount - 1

End Sub
Private Sub WriteReport()   '�ӳ���  ���豸�ͳ�����

Dim Count As Integer
Dim NumberOfBytesRead As Long
Dim NumberOfBytesToSend As Long
Dim NumberOfBytesWritten As Long
Dim ReadBuffer() As Byte
Dim SendBuffer() As Byte

                    'SendBuffer�����0��ʼ���������ȥһ���ֽ�
ReDim SendBuffer(Capabilities.OutputReportByteLength - 1)

                '��һ���ֽ��Ǳ���ID
SendBuffer(0) = 0

                '��һ���ֽ�Ϊ������������
For Count = 1 To Capabilities.OutputReportByteLength - 1
    SendBuffer(Count) = OutputReportData(Count - 1)
Next Count

NumberOfBytesWritten = 0
                                
                                'WriteFile()���豸�����������
Result = WriteFile(HidDevice, SendBuffer(0), _
    CLng(Capabilities.OutputReportByteLength), _
    NumberOfBytesWritten, 0)
    
Call DisplayResultOfAPICall("WriteFile")

lstResults.AddItem " OutputReportByteLength = " & Capabilities.OutputReportByteLength
lstResults.AddItem " NumberOfBytesWritten = " & NumberOfBytesWritten
lstResults.AddItem " Report ID: " & SendBuffer(0)
lstResults.AddItem " Report Data:"

For Count = 1 To UBound(SendBuffer)
    lstResults.AddItem " " & Hex$(SendBuffer(Count))
Next Count

End Sub
Private Function AHex(a) '
    Dim sum, i
    For i = Len(a) To 1 Step -1
        If Asc(Mid(a, i, 1)) < 58 And Asc(Mid(a, i, 1)) > 47 Then
            sum = sum + Val(Mid(a, i, 1)) * 16 ^ (Len(a) - i)
        End If
        If Asc(Mid(a, i, 1)) < 71 And Asc(Mid(a, i, 1)) > 64 Then
            sum = sum + (Asc(Mid(a, i, 1)) - 55) * 16 ^ (Len(a) - i)
        End If
    Next i
    AHex = sum
End Function
Private Sub ReadReport()    '���豸��ȡ���뱨��
                    'Read data from the device.
Dim Count
Dim NumberOfBytesRead As Long
    'Allocate a buffer for the report. ���������������Ļ�����
    'Byte 0 is the report ID.          �ֽ�0Ϊ����
Dim ReadBuffer() As Byte
Dim UBoundReadBuffer As Integer

    'ReadFile Ϊ�ж�ʽ�ĵ��÷�ʽ
    '���Ӧ�ó���ᡰ����ֱ���豸�ͳ����������Ϊֹ
    '��Ϊ�˱�������������ִ��ʱ���ҡ������Ǳ��뱣֤�豸�����������ͳ�
Dim ByteValue As String

    'ReadBuffer������0��ʼ�������Ҫ���ֽ�����1
ReDim ReadBuffer(Capabilities.InputReportByteLength - 1)

                        '���豸��ȡ���뱨�桪��ReadFile()����
                        '���ض�ȡ�������ĵ�һ���ֽڵĵ�ַ
Result = ReadFile(HidDevice, ReadBuffer(0), _
    CLng(Capabilities.InputReportByteLength), _
    NumberOfBytesRead, 0)
    
Call DisplayResultOfAPICall("ReadFile")

lstResults.AddItem " Report ID: " & ReadBuffer(0)
lstResults.AddItem " Report Data:"

txtBytesReceived.Text = ""
For Count = 1 To UBound(ReadBuffer)
     'Add a leading 0 to values 0 - Fh.
    If Len(Hex$(ReadBuffer(Count))) < 2 Then
        ByteValue = "0" & Hex$(ReadBuffer(Count))
    Else
        ByteValue = Hex$(ReadBuffer(Count))
    End If
    lstResults.AddItem " " & ByteValue
    
    '�����յ����ֽڷ����ı�����
    'txtBytesReceived.SelStart = Len(txtBytesReceived.Text)
    'txtBytesReceived.SelText = ByteValue & vbCrLf
    
    txtBytesReceived.Text = ByteValue
    
Next Count
End Sub
Private Sub Shutdown()  '����USBͨ��

Result = CloseHandle(HidDevice) '�ر�
Call DisplayResultOfAPICall("CloseHandle (HidDevice)")

Result = SetupDiDestroyDeviceInfoList(DeviceInfoSet)
Call DisplayResultOfAPICall("DestroyDeviceInfoList")

Result = HidD_FreePreparsedData(PreparsedData)
Call DisplayResultOfAPICall("HidD_FreePreparsedData")

End Sub

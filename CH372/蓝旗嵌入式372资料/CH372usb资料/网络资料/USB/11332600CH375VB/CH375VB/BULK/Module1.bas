Attribute VB_Name = "Module1"
Option Explicit

Type mUspValue
        mUspValueLow As Byte
        mUspValueHigh As Byte
End Type
Type mUspIndex
        mUspIndexLow As Byte
        mUspIndexHigh  As Byte
End Type
Type USB_SETUP_PKT
    mUspReqType As Byte
    mUspRequest As Byte
    mUspValue As mUspValue
    mUspIndex As mUspIndex
    mLength As Integer
End Type
Public mUSB_SETUP_PKT As USB_SETUP_PKT

Public Const mCH375_PACKET_LENGTH = 64        ' CH375支持的数据包的长度

Type WIN32_COMMAND                            '定义WIN32命令接口结构
   mFunction As Long                          '输入时指定功能代码或者管道号
                                              '输出时返回操作状态
   mLength As Long                            '存取长度,返回后续数据的长度
   mBuffer(mCH375_PACKET_LENGTH) As Byte      '数据缓冲区,长度为0至255B                                           '数据缓冲区,长度为0至255B
End Type

Public mWIN32_COMMAND As WIN32_COMMAND

Public Const mCH375_MAX_NUMBER = 16                ' 最多同时连接的CH375数

Public Const mMAX_BUFFER_LENGTH = &H1000           ' 数据缓冲区最大长度

Public Const mDEFAULT_BUFFER_LEN = &H400           ' 数据缓冲区默认长度1024

' CH375端点地址

Public Const mCH375_ENDP_INTER_UP = &H81        ' CH375的中断数据上传端点的地址
Public Const mCH375_ENDP_AUX_DOWN = &H1 	    ' CH375的辅助数据下传端点的地址
Public Const mCH375_ENDP_DATA_UP = &H82         ' CH375的数据块上传端点的地址
Public Const mCH375_ENDP_DATA_DOWN = &H2        ' CH375的数据块下传端点的地址

' 设备层接口提供的管道操作命令
Public Const mPipeDeviceCtrl = &H4              ' CH375的综合控制管道
Public Const mPipeInterUp = &H5                 ' CH375的中断数据上传管道
Public Const mPipeDataUp = &H6                  ' CH375的数据块上传管道
Public Const mPipeDataDown = &H7                ' CH375的数据块下传管道
Public Const mPipeAuxDown = &H8					' CH375的辅助数据下传管道

' 应用层接口的功能代码
Public Const mFuncNoOperation = &H0              ' 无操作
Public Const mFuncGetVersion = &H1               ' 获取驱动程序版本号
Public Const mFuncGetConfig = &H2                ' 获取USB设备配置描述符
Public Const mFuncSetExclusive = &HB			 ' 设置独占使用
Public Const mFuncResetDevice = &HC              ' 复位USB设备
Public Const mFuncResetPipe = &HD                ' 复位USB管道
Public Const mFuncAbortPipe = &HE                ' 取消USB管道的数据请求
Public Const mFuncSetTimeout = &HF				 ' 设置USB通讯超时

' USB设备标准请求代码
Public Const mUSB_CLR_FEATURE = &H1
Public Const mUSB_SET_FEATURE = &H3
Public Const mUSB_GET_STATUS = &H0
Public Const mUSB_SET_ADDRESS = &H5
Public Const mUSB_GET_DESCR = &H6
Public Const mUSB_SET_DESCR = &H7
Public Const mUSB_GET_CONFIG = &H8
Public Const mUSB_SET_CONFIG = &H9
Public Const mUSB_GET_INTERF = &HA
Public Const mUSB_SET_INTERF = &HB
Public Const mUSB_SYNC_FRAME = &HC

' CH375控制传输的供应商专用请求类型
Public Const mCH375_VENDOR_READ = &HC0             '通过控制传输实现的CH375供应商专用读操作
Public Const mCH375_VENDOR_WRITE = &H40            '通过控制传输实现的CH375供应商专用写操作

' CH375控制传输的供应商专用请求代码
Public Const mCH375_SET_CONTROL = &H51             ' 输出控制信号
Public Const mCH375_GET_STATUS = &H52              ' 输入状态信号

' 寄存器的位定义
Public Const mBitInputRxd = &H2                    ' 只读,RXD#引脚输入状态,1:高电平,0:低电平
Public Const mBitInputReq = &H4                    ' 只读,REQ#引脚输入状态,1:高电平,0:低电平

' 直接输入的状态信号的位定义
Public Const mStateRXD = &H200                     ' RXD#引脚输入状态,1:高电平,0:低电平
Public Const mStateREQ = &H400                     ' REQ#引脚输入状态,1:高电平,0:低电平



Declare Function CH375OpenDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Long
'打开CH375设备,返回句柄,出错则无效
' iIndex 指定CH375设备序号,0对应第一个设备

Declare Sub CH375CloseDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long)
'关闭CH375设备
' iIndex 指定CH375设备序号

Declare Function CH375GetVersion Lib "CH375DLL.DLL" () As Long
'获得DLL版本号,返回版本号

Declare Function CH375DriverCommand Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef ioCommand As WIN32_COMMAND) As Long
' 直接传递命令给驱动程序,出错则返回0,否则返回数据长度
' iIndex 指定CH375设备序号
' ioCommand 命令结构的指针
' 该程序在调用后返回数据长度,并且仍然返回命令结构,如果是读操作,则数据返回在命令结构中,
' 返回的数据长度在操作失败时为0,操作成功时为整个命令结构的长度,例如读一个字节,则返回mWIN32_COMMAND_HEAD+1,
' 命令结构在调用前,分别提供:管道号或者命令功能代码,存取数据的长度(可选),数据(可选)
' 命令结构在调用后,分别返回:操作状态代码,后续数据的长度(可选),
'   操作状态代码是由WINDOWS定义的代码,可以参考NTSTATUS.H,
'   后续数据的长度是指读操作返回的数据长度,数据存放在随后的缓冲区中,对于写操作一般为0

Declare Function CH375GetDrvVersion Lib "CH375DLL.DLL" () As Long
'获得驱动程序版本号,返回版本号,出错则返回0

Declare Function CH375ResetDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' 复位USB设备
' iIndex 指定CH375设备序号

Declare Function CH375GetDeviceDescr Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' 读取设备描述符
' iIndex 指定CH375设备序号
' oBuffer 指向一个足够大的缓冲区,用于保存描述符
' ioLength 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

Declare Function CH375GetConfigDescr Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' 读取配置描述符
' iIndex 指定CH375设备序号
' oBuffer 指向一个足够大的缓冲区,用于保存描述符
' ioLength 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

Declare Function CH375SetIntRoutine Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iIntRoutine As Long) As Boolean
' 设定中断服务程序
' iIndex 指定CH375设备序号
' iIntRoutine 指定中断服务程序,为NULL则取消中断服务,否则在中断时调用该程序

Declare Function CH375ReadInter Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' 读取中断数据
' iIndex 指定CH375设备序号
' oBuffer 指向一个足够大的缓冲区,用于保存读取的中断数据
' ioLength 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

Declare Function CH375AbortInter Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' 放弃中断数据读操作
' iIndex  指定CH375设备序号

Declare Function CH375ReadData Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' 读取数据块
' iIndex 指定CH375设备序号
' oBuffer 指向一个足够大的缓冲区,用于保存读取的数据
' ioLength 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

Declare Function CH375AbortRead Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' 放弃数据块读操作
' iIndex  指定CH375设备序号

Declare Function CH375WriteData Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef ioLength As Long) As Boolean
' 写出数据块
' iIndex 指定CH375设备序号
' iBuffer 指向一个缓冲区,放置准备写出的数据
' ioLength  指向长度单元,输入时为准备写出的长度,返回后为实际写出的长度

Declare Function CH375AbortWrite Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' 放弃数据块写操作
' iIndex  指定CH375设备序号

Declare Function CH375WriteRead Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' 先写出标准的数据块(命令),再读取标准的数据块(应答)
' iIndex  指定CH375设备序号
' iBuffer 指向一个缓冲区,放置准备写出的数据,长度不大于mCH375_PACKET_LENGTH
' oBuffer 指向一个足够大的缓冲区,长度不小于mCH375_PACKET_LENGTH,用于保存读取的数据
' ioLength 指向长度单元,不大于mCH375_PACKET_LENGTH,输入时为准备写出的长度,返回后为实际读取的长度

Declare Function CH375GetStatus Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iStatus As Long) As Boolean
' 通过CH375直接输入数据和状态
' iIndex   指定CH375设备序号
' iStatus  指向一个双字单元,用于保存状态数据
' 位7-位0对应CH375的D7-D0引脚,位9对应CH375的RXD#引脚,位10对应CH375的REQ#引脚

Declare Function CH375SetTimeout Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iWriteTimeout As Long, ByVal iReadTimeout As Long) As Boolean
' 设置USB数据读写的超时
' iIndex    指定CH375设备序号
' iWriteTimeout  指定USB写出数据块的超时时间,以毫秒mS为单位,0xFFFFFFFF指定不超时(默认值)
' iReadTimeout   指定USB读取数据块的超时时间,以毫秒mS为单位,0xFFFFFFFF指定不超时(默认值)

Declare Function CH375WriteAuxData Lib (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef ioLength As Long) As Boolean
' 写出辅助数据
' iIndex    指定CH375设备序号
' iBuffer   指向一个缓冲区,放置准备写出的数据
' ioLength  指向长度单元,输入时为准备写出的长度,返回后为实际写出的长度

Declare Function CH375SetExclusive Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iExclusive As Long) As Boolean
' 设置独占使用当前CH375设备
' iIndex   指定CH375设备序号
' iExclusive 为0则设备可以共享使用,非0则独占使用

Declare Function CH375GetUsbID Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Long
' 获取USB设备ID,返回数据中,低16位为厂商ID,高16位为产品ID,错误时返回全0(无效ID)
' iIndex   指定CH375设备序号

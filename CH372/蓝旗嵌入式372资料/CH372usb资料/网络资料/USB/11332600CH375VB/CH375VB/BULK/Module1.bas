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

Public Const mCH375_PACKET_LENGTH = 64        ' CH375֧�ֵ����ݰ��ĳ���

Type WIN32_COMMAND                            '����WIN32����ӿڽṹ
   mFunction As Long                          '����ʱָ�����ܴ�����߹ܵ���
                                              '���ʱ���ز���״̬
   mLength As Long                            '��ȡ����,���غ������ݵĳ���
   mBuffer(mCH375_PACKET_LENGTH) As Byte      '���ݻ�����,����Ϊ0��255B                                           '���ݻ�����,����Ϊ0��255B
End Type

Public mWIN32_COMMAND As WIN32_COMMAND

Public Const mCH375_MAX_NUMBER = 16                ' ���ͬʱ���ӵ�CH375��

Public Const mMAX_BUFFER_LENGTH = &H1000           ' ���ݻ�������󳤶�

Public Const mDEFAULT_BUFFER_LEN = &H400           ' ���ݻ�����Ĭ�ϳ���1024

' CH375�˵��ַ

Public Const mCH375_ENDP_INTER_UP = &H81        ' CH375���ж������ϴ��˵�ĵ�ַ
Public Const mCH375_ENDP_AUX_DOWN = &H1 	    ' CH375�ĸ��������´��˵�ĵ�ַ
Public Const mCH375_ENDP_DATA_UP = &H82         ' CH375�����ݿ��ϴ��˵�ĵ�ַ
Public Const mCH375_ENDP_DATA_DOWN = &H2        ' CH375�����ݿ��´��˵�ĵ�ַ

' �豸��ӿ��ṩ�Ĺܵ���������
Public Const mPipeDeviceCtrl = &H4              ' CH375���ۺϿ��ƹܵ�
Public Const mPipeInterUp = &H5                 ' CH375���ж������ϴ��ܵ�
Public Const mPipeDataUp = &H6                  ' CH375�����ݿ��ϴ��ܵ�
Public Const mPipeDataDown = &H7                ' CH375�����ݿ��´��ܵ�
Public Const mPipeAuxDown = &H8					' CH375�ĸ��������´��ܵ�

' Ӧ�ò�ӿڵĹ��ܴ���
Public Const mFuncNoOperation = &H0              ' �޲���
Public Const mFuncGetVersion = &H1               ' ��ȡ��������汾��
Public Const mFuncGetConfig = &H2                ' ��ȡUSB�豸����������
Public Const mFuncSetExclusive = &HB			 ' ���ö�ռʹ��
Public Const mFuncResetDevice = &HC              ' ��λUSB�豸
Public Const mFuncResetPipe = &HD                ' ��λUSB�ܵ�
Public Const mFuncAbortPipe = &HE                ' ȡ��USB�ܵ�����������
Public Const mFuncSetTimeout = &HF				 ' ����USBͨѶ��ʱ

' USB�豸��׼�������
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

' CH375���ƴ���Ĺ�Ӧ��ר����������
Public Const mCH375_VENDOR_READ = &HC0             'ͨ�����ƴ���ʵ�ֵ�CH375��Ӧ��ר�ö�����
Public Const mCH375_VENDOR_WRITE = &H40            'ͨ�����ƴ���ʵ�ֵ�CH375��Ӧ��ר��д����

' CH375���ƴ���Ĺ�Ӧ��ר���������
Public Const mCH375_SET_CONTROL = &H51             ' ��������ź�
Public Const mCH375_GET_STATUS = &H52              ' ����״̬�ź�

' �Ĵ�����λ����
Public Const mBitInputRxd = &H2                    ' ֻ��,RXD#��������״̬,1:�ߵ�ƽ,0:�͵�ƽ
Public Const mBitInputReq = &H4                    ' ֻ��,REQ#��������״̬,1:�ߵ�ƽ,0:�͵�ƽ

' ֱ�������״̬�źŵ�λ����
Public Const mStateRXD = &H200                     ' RXD#��������״̬,1:�ߵ�ƽ,0:�͵�ƽ
Public Const mStateREQ = &H400                     ' REQ#��������״̬,1:�ߵ�ƽ,0:�͵�ƽ



Declare Function CH375OpenDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Long
'��CH375�豸,���ؾ��,��������Ч
' iIndex ָ��CH375�豸���,0��Ӧ��һ���豸

Declare Sub CH375CloseDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long)
'�ر�CH375�豸
' iIndex ָ��CH375�豸���

Declare Function CH375GetVersion Lib "CH375DLL.DLL" () As Long
'���DLL�汾��,���ذ汾��

Declare Function CH375DriverCommand Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef ioCommand As WIN32_COMMAND) As Long
' ֱ�Ӵ����������������,�����򷵻�0,���򷵻����ݳ���
' iIndex ָ��CH375�豸���
' ioCommand ����ṹ��ָ��
' �ó����ڵ��ú󷵻����ݳ���,������Ȼ��������ṹ,����Ƕ�����,�����ݷ���������ṹ��,
' ���ص����ݳ����ڲ���ʧ��ʱΪ0,�����ɹ�ʱΪ��������ṹ�ĳ���,�����һ���ֽ�,�򷵻�mWIN32_COMMAND_HEAD+1,
' ����ṹ�ڵ���ǰ,�ֱ��ṩ:�ܵ��Ż�������ܴ���,��ȡ���ݵĳ���(��ѡ),����(��ѡ)
' ����ṹ�ڵ��ú�,�ֱ𷵻�:����״̬����,�������ݵĳ���(��ѡ),
'   ����״̬��������WINDOWS����Ĵ���,���Բο�NTSTATUS.H,
'   �������ݵĳ�����ָ���������ص����ݳ���,���ݴ�������Ļ�������,����д����һ��Ϊ0

Declare Function CH375GetDrvVersion Lib "CH375DLL.DLL" () As Long
'�����������汾��,���ذ汾��,�����򷵻�0

Declare Function CH375ResetDevice Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' ��λUSB�豸
' iIndex ָ��CH375�豸���

Declare Function CH375GetDeviceDescr Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' ��ȡ�豸������
' iIndex ָ��CH375�豸���
' oBuffer ָ��һ���㹻��Ļ�����,���ڱ���������
' ioLength ָ�򳤶ȵ�Ԫ,����ʱΪ׼����ȡ�ĳ���,���غ�Ϊʵ�ʶ�ȡ�ĳ���

Declare Function CH375GetConfigDescr Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' ��ȡ����������
' iIndex ָ��CH375�豸���
' oBuffer ָ��һ���㹻��Ļ�����,���ڱ���������
' ioLength ָ�򳤶ȵ�Ԫ,����ʱΪ׼����ȡ�ĳ���,���غ�Ϊʵ�ʶ�ȡ�ĳ���

Declare Function CH375SetIntRoutine Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iIntRoutine As Long) As Boolean
' �趨�жϷ������
' iIndex ָ��CH375�豸���
' iIntRoutine ָ���жϷ������,ΪNULL��ȡ���жϷ���,�������ж�ʱ���øó���

Declare Function CH375ReadInter Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' ��ȡ�ж�����
' iIndex ָ��CH375�豸���
' oBuffer ָ��һ���㹻��Ļ�����,���ڱ����ȡ���ж�����
' ioLength ָ�򳤶ȵ�Ԫ,����ʱΪ׼����ȡ�ĳ���,���غ�Ϊʵ�ʶ�ȡ�ĳ���

Declare Function CH375AbortInter Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' �����ж����ݶ�����
' iIndex  ָ��CH375�豸���

Declare Function CH375ReadData Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' ��ȡ���ݿ�
' iIndex ָ��CH375�豸���
' oBuffer ָ��һ���㹻��Ļ�����,���ڱ����ȡ������
' ioLength ָ�򳤶ȵ�Ԫ,����ʱΪ׼����ȡ�ĳ���,���غ�Ϊʵ�ʶ�ȡ�ĳ���

Declare Function CH375AbortRead Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' �������ݿ������
' iIndex  ָ��CH375�豸���

Declare Function CH375WriteData Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef ioLength As Long) As Boolean
' д�����ݿ�
' iIndex ָ��CH375�豸���
' iBuffer ָ��һ��������,����׼��д��������
' ioLength  ָ�򳤶ȵ�Ԫ,����ʱΪ׼��д���ĳ���,���غ�Ϊʵ��д���ĳ���

Declare Function CH375AbortWrite Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Boolean
' �������ݿ�д����
' iIndex  ָ��CH375�豸���

Declare Function CH375WriteRead Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef oBuffer As Any, ByRef ioLength As Long) As Boolean
' ��д����׼�����ݿ�(����),�ٶ�ȡ��׼�����ݿ�(Ӧ��)
' iIndex  ָ��CH375�豸���
' iBuffer ָ��һ��������,����׼��д��������,���Ȳ�����mCH375_PACKET_LENGTH
' oBuffer ָ��һ���㹻��Ļ�����,���Ȳ�С��mCH375_PACKET_LENGTH,���ڱ����ȡ������
' ioLength ָ�򳤶ȵ�Ԫ,������mCH375_PACKET_LENGTH,����ʱΪ׼��д���ĳ���,���غ�Ϊʵ�ʶ�ȡ�ĳ���

Declare Function CH375GetStatus Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByRef iStatus As Long) As Boolean
' ͨ��CH375ֱ���������ݺ�״̬
' iIndex   ָ��CH375�豸���
' iStatus  ָ��һ��˫�ֵ�Ԫ,���ڱ���״̬����
' λ7-λ0��ӦCH375��D7-D0����,λ9��ӦCH375��RXD#����,λ10��ӦCH375��REQ#����

Declare Function CH375SetTimeout Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iWriteTimeout As Long, ByVal iReadTimeout As Long) As Boolean
' ����USB���ݶ�д�ĳ�ʱ
' iIndex    ָ��CH375�豸���
' iWriteTimeout  ָ��USBд�����ݿ�ĳ�ʱʱ��,�Ժ���mSΪ��λ,0xFFFFFFFFָ������ʱ(Ĭ��ֵ)
' iReadTimeout   ָ��USB��ȡ���ݿ�ĳ�ʱʱ��,�Ժ���mSΪ��λ,0xFFFFFFFFָ������ʱ(Ĭ��ֵ)

Declare Function CH375WriteAuxData Lib (ByVal iIndex As Long, ByRef iBuffer As Any, ByRef ioLength As Long) As Boolean
' д����������
' iIndex    ָ��CH375�豸���
' iBuffer   ָ��һ��������,����׼��д��������
' ioLength  ָ�򳤶ȵ�Ԫ,����ʱΪ׼��д���ĳ���,���غ�Ϊʵ��д���ĳ���

Declare Function CH375SetExclusive Lib "CH375DLL.DLL" (ByVal iIndex As Long, ByVal iExclusive As Long) As Boolean
' ���ö�ռʹ�õ�ǰCH375�豸
' iIndex   ָ��CH375�豸���
' iExclusive Ϊ0���豸���Թ���ʹ��,��0���ռʹ��

Declare Function CH375GetUsbID Lib "CH375DLL.DLL" (ByVal iIndex As Long) As Long
' ��ȡUSB�豸ID,����������,��16λΪ����ID,��16λΪ��ƷID,����ʱ����ȫ0(��ЧID)
' iIndex   ָ��CH375�豸���

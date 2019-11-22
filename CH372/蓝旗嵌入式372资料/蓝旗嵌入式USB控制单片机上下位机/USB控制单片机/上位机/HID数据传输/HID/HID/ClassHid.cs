using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using Microsoft.Win32.SafeHandles;
using System.Windows;
using System.Windows.Forms;
namespace HID
{
    class ClassHid
    {
        [DllImport("hid.dll")]
        public static extern void HidD_GetHidGuid(ref Guid HidGuid);

        [DllImport("setupapi.dll", SetLastError = true)]
        public static extern IntPtr SetupDiGetClassDevs(ref Guid ClassGuid, uint Enumerator, IntPtr HwndParent, DIGCF Flags);

        [DllImport("setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern Boolean SetupDiDestroyDeviceInfoList(IntPtr deviceInfoSet);

        [DllImport("setupapi.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern Boolean SetupDiEnumDeviceInterfaces(IntPtr deviceInfoSet, IntPtr deviceInfoData, ref Guid interfaceClassGuid, UInt32 memberIndex, ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData);

        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        private static extern bool SetupDiGetDeviceInterfaceDetail(IntPtr deviceInfoSet, ref SP_DEVICE_INTERFACE_DATA deviceInterfaceData, IntPtr deviceInterfaceDetailData, int deviceInterfaceDetailDataSize, ref int requiredSize, SP_DEVINFO_DATA deviceInfoData);

        [DllImport("hid.dll")]
        private static extern Boolean HidD_GetAttributes(IntPtr hidDeviceObject, out HIDD_ATTRIBUTES attributes);

        [DllImport("hid.dll")]
        private static extern Boolean HidD_GetSerialNumberString(IntPtr hidDeviceObject, IntPtr buffer, int bufferLength);

        [DllImport("hid.dll")]
        private static extern Boolean HidD_GetPreparsedData(IntPtr hidDeviceObject, out IntPtr PreparsedData);

        [DllImport("hid.dll")]
        private static extern Boolean HidD_FreePreparsedData(IntPtr PreparsedData);

        [DllImport("hid.dll")]
        private static extern uint HidP_GetCaps(IntPtr PreparsedData, out HIDP_CAPS Capabilities);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr CreateFile(string fileName, uint desiredAccess, uint shareMode, uint securityAttributes, uint creationDisposition, uint flagsAndAttributes, uint templateFile);
       
        [DllImport("kernel32.dll")]
        private static extern int CloseHandle(IntPtr hObject);
       
        [DllImport("Kernel32.dll", SetLastError = true)]
        private static extern bool ReadFile(IntPtr file, byte[] buffer, uint numberOfBytesToRead, out uint numberOfBytesRead, IntPtr lpOverlapped);
       
        [DllImport("Kernel32.dll", SetLastError = true)]
        private static extern bool WriteFile(IntPtr file, byte[] buffer, uint numberOfBytesToWrite, out uint numberOfBytesWritten, IntPtr lpOverlapped);
      
        [DllImport("User32.dll", SetLastError = true)]
        private static extern IntPtr RegisterDeviceNotification(IntPtr recipient, IntPtr notificationFilter, int flags);
       
        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool UnregisterDeviceNotification(IntPtr handle);
        /*************************************************************************************************
        * ****全局变量
        * ****
        * ****
        * ***********************************************************************************************/

        public static Form1 fm1 = new Form1();
        private const int MAX_USB_DEVICES = 64;
        private IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
        public bool deviceOpened = false;
        public FileStream hidDevice = null;
        public string deviceStr;
        
        public  byte[] reportData;
        report rr;
        public report RP
        {
            get
            {
                return rr;
            }
        }
        public HIDD_ATTRIBUTES attributes;
        int outputReportLength;//输出报告长度,包刮一个字节的报告ID
        public int OutputReportLength { get { return outputReportLength; } }
        int inputReportLength;//输入报告长度,包刮一个字节的报告ID   
        public int InputReportLength { get { return inputReportLength; } }
      

        public class report : EventArgs
        {

            public readonly byte reportID;
            public readonly byte[] reportBuff;
            public report(byte id, byte[] arrayBuff)
            {
                reportID = id;
                reportBuff = arrayBuff;
            }
        }
        /*************************************************************************************************
        * ****返回值
        * ****
        * ****
       * ***********************************************************************************************/
        public enum HID_RETURN
        {
            打开成功 = 0,
            没有设备接入,
            没有找到设备,
            设备已经打开,
            发送失败,
            接收失败,
            发送成功

        }
        /*************************************************************************************************
         * ****获取HID设备列表
         * ****
         * ****
         * ***********************************************************************************************/
        public  void GetHidDeviceList(ref List<string> devicelist)
        {
            Guid hUSB = Guid.Empty;
            uint index = 0;

            devicelist.Clear();
            HidD_GetHidGuid(ref hUSB);

            IntPtr hidInfoSet = SetupDiGetClassDevs(ref hUSB, 0, IntPtr.Zero, DIGCF.DIGCF_PRESENT | DIGCF.DIGCF_DEVICEINTERFACE);
            if (hidInfoSet != IntPtr.Zero)
            {
                SP_DEVICE_INTERFACE_DATA interfaceInfo = new SP_DEVICE_INTERFACE_DATA();
                interfaceInfo.cbSize = Marshal.SizeOf(interfaceInfo);
                //查询集合中每一个接口
                for (index = 0; index < MAX_USB_DEVICES; index++)
                {
                    //得到第index个接口信息
                    if (SetupDiEnumDeviceInterfaces(hidInfoSet, IntPtr.Zero, ref hUSB, index, ref interfaceInfo))
                    {
                        int buffsize = 0;
                        // 取得接口详细信息:第一次读取错误,但可以取得信息缓冲区的大小
                        SetupDiGetDeviceInterfaceDetail(hidInfoSet, ref interfaceInfo, IntPtr.Zero, buffsize, ref buffsize, null);
                        //构建接收缓冲
                        IntPtr pDetail = Marshal.AllocHGlobal(buffsize);
                        SP_DEVICE_INTERFACE_DETAIL_DATA detail = new SP_DEVICE_INTERFACE_DETAIL_DATA();
                        detail.cbSize = Marshal.SizeOf(typeof(SP_DEVICE_INTERFACE_DETAIL_DATA));
                        Marshal.StructureToPtr(detail, pDetail, false);
                        if (SetupDiGetDeviceInterfaceDetail(hidInfoSet, ref interfaceInfo, pDetail, buffsize, ref buffsize, null))
                        {
                            devicelist.Add(Marshal.PtrToStringAuto((IntPtr)((int)pDetail + 4)));
                        }
                        Marshal.FreeHGlobal(pDetail);
                    }
                }
            }
            SetupDiDestroyDeviceInfoList(hidInfoSet);
          //  MessageBox.Show(hidInfoSet.ToString());
            //MessageBox.Show();
        }
        /*************************************************************************************************
         * ****打开HID设备
         * ****vID:hid设备的VID厂商ID，pID:hid设备的PID产品ID，均为16进制
         * ****serrial：设备的字符串描述符
         * ***********************************************************************************************/
        public HID_RETURN OpenHidDevice(UInt16 vID,UInt16 pID,string serial)
        {
            if (deviceOpened == false)
            {
                //获取连接的HID列表
                List<string> deviceList = new List<string>();
                GetHidDeviceList(ref deviceList);
                //判断已经接入的HID设备的数目
                if (deviceList.Count == 0)
                    return HID_RETURN.没有设备接入;//如果没有则返回没有设备连接提示
                //对接入的每个HID设备进行操作
                for (int i = 0; i < deviceList.Count; i++)
                {
                    IntPtr device = CreateFile(deviceList[i], DESIREDACCESS.GENERIC_READ | DESIREDACCESS.GENERIC_WRITE, 0, 0, CREATIONDISPOSITION.OPEN_EXISTING, 0x40000000, 0);
                    if (device != INVALID_HANDLE_VALUE)
                    {
                        
                        IntPtr serialBuff = Marshal.AllocHGlobal(512);
                        HidD_GetAttributes(device, out attributes);    //获取设备的VID、PID等信息
                        HidD_GetSerialNumberString(device, serialBuff, 512);//获取设备的字符串描述符
                        deviceStr = Marshal.PtrToStringAuto(serialBuff);
                        Marshal.FreeHGlobal(serialBuff);
                        if (attributes.VendorID == vID && attributes.ProductID == pID && deviceStr == serial)//根据输入参数进行判断
                        {
                            IntPtr preparseData;
                            HIDP_CAPS caps;
                            HidD_GetPreparsedData(device, out preparseData);//请求获得与设备能力信息相关的缓冲区的代号
                            HidP_GetCaps(preparseData, out caps);           //请求获得用于描述设备能力的结构的指针
                            HidD_FreePreparsedData(preparseData);           //释放HidD_GetPreparsedData使用的资源 
                            outputReportLength = caps.OutputReportByteLength;//输出缓冲区的长度
                            inputReportLength = caps.InputReportByteLength;  //输入缓冲区的长度
                            //MessageBox.Show(outputReportLength.ToString());
                            hidDevice = new FileStream(new SafeFileHandle(device, false), FileAccess.ReadWrite, inputReportLength, true);//使用指定的读/写权限、缓冲区大小和同步或异步状态为指定的文件句柄初始化 FileStream 类的新实例
                            deviceOpened = true;
                            BeginAsyncRead();

                            return HID_RETURN.打开成功;
                        }
                    }
                }
                return HID_RETURN.没有找到设备;
            }
            else
                return HID_RETURN.设备已经打开;
        }

        /*************************************************************************************************
         * ****异步读
         * ****
         * ****
         * ***********************************************************************************************/
        public void BeginAsyncRead()
        {
            byte[] inputBuff = new byte[InputReportLength];
            hidDevice.BeginRead(inputBuff, 0, InputReportLength, new AsyncCallback(ReadCompleted), inputBuff);
          //  hidDevice.b
           // MessageBox.Show(inputBuff[0].ToString());
        }
        /*************************************************************************************************
         * ****读完成
         * ****参数:输入报告的数组
         * ****
         * ***********************************************************************************************/
        public void ReadCompleted(IAsyncResult iResult)
        {
           // public static IAsyncResult iResult;
            byte[] readBuff=(byte[])(iResult.AsyncState);
            try
            {
                hidDevice.EndRead(iResult);//读取结束,如果读取错误就会产生一个异常
                reportData = new byte[readBuff.Length - 1];
                for (int i = 1; i < readBuff.Length; i++)
                    reportData[i - 1] = readBuff[i];
                rr = new report(readBuff[0], reportData);
                OnDataReceived(rr); //发出数据到达消息
               // MessageBox.Show(rr.reportBuff[2].ToString());
                BeginAsyncRead();//启动下一次读操作
            }
            catch (IOException e)//读写错误,设备已经被移除
            {
                EventArgs ex = new EventArgs();
                OnDeviceRemoved(ex);//发出设备移除消息
                CloseDevice();
            }
        }
        /*************************************************************************************************
         * ****数据接收事件
         * ****
         * ****
         * ***********************************************************************************************/
        public event EventHandler<report> DataReceived;
        public virtual void OnDataReceived(report e)
        {
            if (DataReceived != null) 
                DataReceived(this, e);
          //  MessageBox.Show(e.reportBuff[5].ToString());
        }

        /*************************************************************************************************
         * ****设备断开
         * ****
         * ****
         * ***********************************************************************************************/
        public event EventHandler DeviceRemoved;
        protected virtual void OnDeviceRemoved(EventArgs e)
        {
            if (DeviceRemoved != null) 
                DeviceRemoved(this, e);
        }
        /*************************************************************************************************
         * ****关闭打开的设备
         * ****
         * ****
         * ***********************************************************************************************/
        public void CloseDevice()
        {
            if (deviceOpened == true)
            {
                hidDevice.Close();
                deviceOpened = false;
            }
        }
        /*************************************************************************************************
         * ****写数据/发送数据
         * ****
         * ****
         * ***********************************************************************************************/
        public HID_RETURN Write(report r)
        {
            if (deviceOpened)
            {
                try
                {
                    byte[] buffer = new byte[outputReportLength];
                    buffer[0] = r.reportID;
                   
                    int maxBufferLength = 0;
                    if (r.reportBuff.Length < outputReportLength - 1)
                        maxBufferLength = r.reportBuff.Length;
                    else
                        maxBufferLength = outputReportLength - 1;
                    for (int i = 1; i < maxBufferLength; i++)
                        buffer[i] = r.reportBuff[i - 1];
                  
                    hidDevice.Write(buffer, 0, OutputReportLength);
                    
                }
                catch(IOException e)
                {

                    EventArgs ex = new EventArgs();
                    OnDeviceRemoved(ex);//发出设备移除消息
                    CloseDevice();
                 
                    return HID_RETURN.发送失败;
                }
            }
            return HID_RETURN.发送成功;
            
        }

        /*************************************************************************************************
         * ****以下块请不要删除
         * ****
         * ****
         * ***********************************************************************************************/

        #region
        /// <summary>
        /// SP_DEVICE_INTERFACE_DATA structure defines a device interface in a device information set.
        /// </summary>
        public struct SP_DEVICE_INTERFACE_DATA
        {
            public int cbSize;
            public Guid interfaceClassGuid;
            public int flags;
            public int reserved;
        }

        /// <summary>
        /// SP_DEVICE_INTERFACE_DETAIL_DATA structure contains the path for a device interface.
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 2)]
        internal struct SP_DEVICE_INTERFACE_DETAIL_DATA
        {
            internal int cbSize;
            internal short devicePath;
        }

        /// <summary>
        /// SP_DEVINFO_DATA structure defines a device instance that is a member of a device information set.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public class SP_DEVINFO_DATA
        {
            public int cbSize = Marshal.SizeOf(typeof(SP_DEVINFO_DATA));
            public Guid classGuid = Guid.Empty; // temp
            public int devInst = 0; // dumy
            public int reserved = 0;
        }
        /// <summary>
        /// Flags controlling what is included in the device information set built by SetupDiGetClassDevs
        /// </summary>
        public enum DIGCF
        {
            DIGCF_DEFAULT = 0x00000001, // only valid with DIGCF_DEVICEINTERFACE                 
            DIGCF_PRESENT = 0x00000002,
            DIGCF_ALLCLASSES = 0x00000004,
            DIGCF_PROFILE = 0x00000008,
            DIGCF_DEVICEINTERFACE = 0x00000010
        }
        /// <summary>
        /// The HIDD_ATTRIBUTES structure contains vendor information about a HIDClass device
        /// </summary>
        public struct HIDD_ATTRIBUTES
        {
            public int Size;
            public ushort VendorID;
            public ushort ProductID;
            public ushort VersionNumber;
        }

        public struct HIDP_CAPS
        {
            public ushort Usage;
            public ushort UsagePage;
            public ushort InputReportByteLength;
            public ushort OutputReportByteLength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 17)]
            public ushort[] Reserved;
            public ushort NumberLinkCollectionNodes;
            public ushort NumberInputButtonCaps;
            public ushort NumberInputValueCaps;
            public ushort NumberInputDataIndices;
            public ushort NumberOutputButtonCaps;
            public ushort NumberOutputValueCaps;
            public ushort NumberOutputDataIndices;
            public ushort NumberFeatureButtonCaps;
            public ushort NumberFeatureValueCaps;
            public ushort NumberFeatureDataIndices;
        }
        /// <summary>
        /// Type of access to the object. 
        ///</summary>
        static class DESIREDACCESS
        {
            public const uint GENERIC_READ = 0x80000000;
            public const uint GENERIC_WRITE = 0x40000000;
            public const uint GENERIC_EXECUTE = 0x20000000;
            public const uint GENERIC_ALL = 0x10000000;
        }
        /// <summary>
        /// Action to take on files that exist, and which action to take when files do not exist. 
        /// </summary>
        static class CREATIONDISPOSITION
        {
            public const uint CREATE_NEW = 1;
            public const uint CREATE_ALWAYS = 2;
            public const uint OPEN_EXISTING = 3;
            public const uint OPEN_ALWAYS = 4;
            public const uint TRUNCATE_EXISTING = 5;
        }
        /// <summary>
        /// File attributes and flags for the file. 
        /// </summary>
        static class FLAGSANDATTRIBUTES
        {
            public const uint FILE_FLAG_WRITE_THROUGH = 0x80000000;
            public const uint FILE_FLAG_OVERLAPPED = 0x40000000;
            public const uint FILE_FLAG_NO_BUFFERING = 0x20000000;
            public const uint FILE_FLAG_RANDOM_ACCESS = 0x10000000;
            public const uint FILE_FLAG_SEQUENTIAL_SCAN = 0x08000000;
            public const uint FILE_FLAG_DELETE_ON_CLOSE = 0x04000000;
            public const uint FILE_FLAG_BACKUP_SEMANTICS = 0x02000000;
            public const uint FILE_FLAG_POSIX_SEMANTICS = 0x01000000;
            public const uint FILE_FLAG_OPEN_REPARSE_POINT = 0x00200000;
            public const uint FILE_FLAG_OPEN_NO_RECALL = 0x00100000;
            public const uint FILE_FLAG_FIRST_PIPE_INSTANCE = 0x00080000;
        }
        /// <summary>
        /// Serves as a standard header for information related to a device event reported through the WM_DEVICECHANGE message.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct DEV_BROADCAST_HDR
        {
            public int dbcc_size;
            public int dbcc_devicetype;
            public int dbcc_reserved;
        }
        /// <summary>
        /// Contains information about a class of devices
        /// </summary>
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct DEV_BROADCAST_DEVICEINTERFACE
        {
            public int dbcc_size;
            public int dbcc_devicetype;
            public int dbcc_reserved;
            public Guid dbcc_classguid;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 255)]
            public string dbcc_name;
        }
        #endregion
    }
}

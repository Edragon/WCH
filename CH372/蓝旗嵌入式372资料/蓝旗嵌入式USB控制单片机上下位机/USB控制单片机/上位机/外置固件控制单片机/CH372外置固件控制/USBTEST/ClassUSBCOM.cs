using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;


namespace USBTEST
{
   
    class ClassUSBCOM
    {
        public static int[] data = new int[200];
        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375WriteData(int iIndex,byte[] iBuffer, int[] ioLength);
        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375ReadData(int iIndex, [MarshalAs(UnmanagedType.LPArray)] byte[] oBuffer, [MarshalAs(UnmanagedType.LPArray)]  int[] ioLength);
        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375SetTimeout(int iIndex, int iWriteTimeout, int iReadTimeout);
        [DllImport("CH375DLL.DLL")]
        public static extern IntPtr CH375OpenDevice(int iIndex);
        [DllImport("CH375DLL.DLL")]
        public static extern IntPtr CH375CloseDevice(int iIndex);
        [DllImport("CH375DLL.DLL")]
        public static extern int CH375GetVersion();
        [DllImport("CH375DLL.DLL")]
        public static extern int CH375GetDrvVersion( );
        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375ResetDevice(int iIndex);

        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375GetDeviceDescr(int iIndex, [MarshalAs(UnmanagedType.LPArray)] byte[] oBuffer, [MarshalAs(UnmanagedType.LPArray)] int[] ioLength);
        [DllImport("CH375DLL.DLL")]
        public static extern int CH375GetUsbID(int iIndex);
       
        public delegate void NotifyCallBack(long iEventStatus);        
        [DllImport("CH375DLL.DLL")]
        public static extern bool CH375SetDeviceNotify(int iIndex, string iDeviceID, NotifyCallBack iNotifyRoutine);

       
        //属性字段
        public static bool usbstatus = false;//USB不可用
        public static bool error = false;//读写出错字段
       // public static IntPtr ch375Handle;

        public static byte[] TxBuf= new byte[64];//发送缓冲区
        public static byte[] ReBuf = new byte[64];//接收缓冲区

        public static Form1 fm1 = new Form1();



        
        /**************************************************************************
        * ****函数名称：打开USB设备
        * ****入口参数：deviceNum-设备号
        * ****出口参数：无
        * ************************************************************************/
        /*  public static void USBCOMM()
          {
              try
              {
                  ch375Handle = CH375OpenDevice(0);  //打开USB设备
                  string hvalue = ch375Handle.ToString();
                  if (ch375Handle.ToInt32() == -1)
                  {
                      enable = false;
                     // MessageBox.Show("NOT OK");
                     // fm1.label1.Text = "打开失败！";
                  }else
                  {
                      enable = true;
                    //  MessageBox.Show("OK");
                  }
              }
              catch (System.Exception ex)
              {
                  MessageBox.Show(ex.Message);
              }
            
          }
          //////////////////////////////////////////////////////////////////////////
          //////////////////////////////////////////////////////////////////////////
          ~ClassUSBCOM()
          {
              if (enable==true)
              {//成功加载WinIO
                  try
                  {
                      CH375CloseDevice(0);
                  }
                  catch (System.Exception ex)
                  {
                      MessageBox.Show(ex.Message);
                  }
                
              }
          }
          public bool Enable
          {
              //Enable只读属性
              get
              {
                  return enable;
              }
          }
          public bool Error
          {
              //Error只读属性
              get
              {
                  return error;
              }
          }*/
        public static void OpenUSB(int deviceNum)
        {
            try
            {
                if (CH375OpenDevice(deviceNum).ToInt32() == -1)
                {
                    usbstatus= false;
                }
                else
                {
                   // MessageBox.Show("USB设备" + deviceNum.ToString() + "打开成功!", "提示信息");
                    usbstatus = true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                usbstatus= false;
            }
        }
        /**************************************************************************
         * ****函数名称：关闭USB设备
         * ****入口参数：deviceNum-设备号
         * ****出口参数：无
         * ************************************************************************/
        public static void CloseUSB(int deviceNum)
        {
            try
            {
                CH375CloseDevice(deviceNum);
                usbstatus = false;
             }
            catch( Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        /**************************************************************************
        * ****函数名称：获取DLL版本号
        * ****入口参数：无
        * ****出口参数：无
        * ************************************************************************/
        public static int GetDLLVer()
        {
            int ver = CH375GetVersion();
            //MessageBox.Show("DLL版本信息：" + ver.ToString(), "提示信息");
            return ver;
        }
        /**************************************************************************
        * ****函数名称：获取驱动版本号
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        public static int GetDrvVer()
        {

            int ver = CH375GetDrvVersion();
            if (ver == 0)
            {
                MessageBox.Show("获取驱动版本时有错误发生！", "提示信息");
                return 0;
            }
            else
                // MessageBox.Show("驱动版本信息：" + ver.ToString(), "提示信息");
                return ver;

        }
        /**************************************************************************
        * ****函数名称：复位USB设备
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        public static void ResetDevice(int deviceNum)
        {

            bool bak = CH375ResetDevice(deviceNum);
            if (bak == false)
                MessageBox.Show("复位CH372USB设备"+deviceNum.ToString()+"失败！","提示信息");
            else
                MessageBox.Show("复位CH372USB设备" + deviceNum.ToString() + "成功！", "提示信息");

        }
        /**************************************************************************
        * ****函数名称：获取设备描述符
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        public static void GetDeDesc(int deviceNum)
        {
             byte[] DescBuff=new  byte[100];
             int[] len = new int[1];
             len[0] = 64;
             Encoding enc = Encoding.UTF8;
             try
             {
                 bool bak = CH375GetDeviceDescr(deviceNum, DescBuff, len);
                 string s = enc.GetString(DescBuff);
                 MessageBox.Show(s, "提示信息");
                
             }
             catch(Exception ex)
             {
                 MessageBox.Show(ex.Message);
            }
        }
        /**************************************************************************
        * ****函数名称：读取数据
        * ****入口参数：deviceNum-设备编号,num接收数据的个数
        * ****出口参数：无
        * * ************************************************************************/
        public static void ReadData(int deviceNum,int num)
        {
            int[] oLen=new int[1];
            oLen[0] = num;
            try
            {
                CH375ReadData(deviceNum, ReBuf, oLen);
            }
            catch(Exception ex )
            {
              MessageBox.Show(ex.Message);
            }
        }
        /**************************************************************************
        * ****函数名称：写数据
        * ****入口参数：deviceNum-设备编号,num发送数据的个数
        * ****出口参数：
        * ************************************************************************/
        public static void WriteData(int deviceNum,int num)
        {
            int[] iLen = new int[1];
            iLen[0] = num;
        
            try
            {
                CH375WriteData(deviceNum, TxBuf, iLen);
            }
            catch(Exception ex  )
            {
                MessageBox.Show(ex.Message);
            }
        }

        /**************************************************************************
        * ****函数名称：检测是否插入PC
        * ****入口参数：设备事件回调状态值3--插入/0--拔出
        * ****出口参数：无
        * ************************************************************************/
        public static void CheckCon(long iEventStatus)
        {

            if (iEventStatus == 3)
            {
                fm1.label1.Text = "CH372设备已经插入！";
            }
            else
                if (iEventStatus == 0)
                {
                   fm1.label1.Text = "CH372设备已经拔出！";
                }
            //MessageBox.Show(iEventStatus.ToString());
     }
        /**************************************************************************
        * ****函数名称：检测CH372设备是否插入计算机
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：
        * ************************************************************************/
        public static void Check(int deviceNum)
        {
            NotifyCallBack myback = new NotifyCallBack(CheckCon);
            CH375SetDeviceNotify(0, "", myback);
           // MessageBox.Show(myback.ToString());
         }
         /**************************************************************************
        * ****函数名称：获取USB设备的ID，
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：
        * ************************************************************************/
        public static int GetUSBID(int deviceNum)
        {
           int deviceID=0;
           try
           {
               deviceID = CH375GetUsbID(0);
               return deviceID;
           }
            catch(Exception ex)
           {
               MessageBox.Show(ex.Message);
           }
           return deviceID;
        }





        
    }

    }


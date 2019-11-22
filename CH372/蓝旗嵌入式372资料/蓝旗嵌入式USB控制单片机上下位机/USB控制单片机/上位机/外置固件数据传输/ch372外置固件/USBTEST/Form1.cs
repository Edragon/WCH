using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using　System.Runtime.InteropServices;

namespace USBTEST
{
    public partial class Form1 : Form
    {
        ClassUSBCOM uscom = new ClassUSBCOM();
        bool TxFlag = false;
        int TxLen = 0;
        
        public Form1()
        {
            InitializeComponent();
            ClassUSBCOM.fm1 = this;
            //初始化时检测是否有CH372USB设备已经插入计算机
            int i;
            for (i = 0; i < 15; i++)
            {
                ClassUSBCOM.OpenUSB(i);
                if(ClassUSBCOM.usbstatus)
                    this.label1.Text="CH372设备已经插入！";
                ClassUSBCOM.CloseUSB(i);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ClassUSBCOM.Check(0); //启动拔插检测
        }
        /**************************************************************************
        * ****函数名称：打开USB设备
        * ****入口参数：deviceNum-设备号
        * ****出口参数：无
        * ************************************************************************/
        private void button1_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.OpenUSB(0);
            if (ClassUSBCOM.usbstatus)
            {
                this.label1.Text = "CH372USB设备已经打开!";
                DispDeviceNum(0);
                this.label11.Text=ClassUSBCOM.GetDLLVer().ToString();
                this.label10.Text=ClassUSBCOM.GetDrvVer().ToString();
            }
            else
            {
                this.label1.ForeColor = Color.Red;
                this.label1.Text = "CH372USB设备没有打开!";
            }
          
        }
        /**************************************************************************
        * ****函数名称：关闭USB设备
        * ****入口参数：deviceNum-设备号
        * ****出口参数：无
        * ************************************************************************/
        private void button2_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.CloseUSB(0);
            this.label1.Text = "CH372USB设备没有打开!";
            this.label7.Text = "0000";
            this.label5.Text = "0000";
            this.label10.Text = "...";
            this.label11.Text = "...";
        }
       /**************************************************************************
       * ****函数名称：获取DLL版本号
       * ****入口参数：无
       * ****出口参数：无
       * ************************************************************************/
        private void button4_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.GetDLLVer();
        }
        /**************************************************************************
        * ****函数名称：获取驱动版本号
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        private void button5_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.GetDrvVer();
        }
        /**************************************************************************
        * ****函数名称：复位USB设备
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        private void button6_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.ResetDevice(0);
        }
        /**************************************************************************
        * ****函数名称：获取设备描述符
        * ****入口参数：deviceNum-设备编号
        * ****出口参数：无
        * ************************************************************************/
        private void button7_Click(object sender, EventArgs e)
        {
            ClassUSBCOM.GetDeDesc(0);
        }
        /*****************************************************************************
         * ****函数名称：接收数据据
         * ****函数作用：将下位机发送来的数据放到文本框中
         * ****函数描述：
         * ****************************************************************************/
        private void button8_Click(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备并发送数据！","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
                return;
            }
            if(TxFlag==false)
            {
                MessageBox.Show("请先发送符合格式的数据！","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
                return;
            }
            try
            {
               // MessageBox.Show(TxLen.ToString());
                ClassUSBCOM.ReadData(0, TxLen);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            TxFlag = false;
            this.textBox2.Clear();
            foreach(byte b in ClassUSBCOM.ReBuf)
            {
                this.textBox2.Text += b.ToString()+",";
            }
           
        }
        /*****************************************************************************
         * ****函数名称：发送数据
         * ****函数作用：向下位机发送文本框中的数据
         * ****函数描述：
         * ****************************************************************************/
        private void button9_Click(object sender, EventArgs e)
        {
            if(ClassUSBCOM.usbstatus==false)
            {
                MessageBox.Show("请先打开USB设备！","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
                return;
            }
            if (this.textBox1.Text== "")
            {
                MessageBox.Show("发送数据不能为空！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            string[] s= this.textBox1.Text.ToString().Split(',','\0');
            int b;
            b = s.Length;
            TxLen = b;
           // MessageBox.Show(b.ToString());
            for (int i = 0; i < s.Length; i++)
            {
                try
                {
                    ClassUSBCOM.TxBuf[i] = byte.Parse(s[i]);
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message,"提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
                }
            }
            ClassUSBCOM.WriteData(0,TxLen);
            TxFlag = true;
        }
        /*****************************************************************************
         * ****函数名称：显示设备ID
         * ****入口参数：deviceNum-设备编号
         * ****出口参数：无
         * ****说    明：高16位为产品ID，低16位为厂商ID
         * ****************************************************************************/
        private void DispDeviceNum(int deviceNum)
        {
            int id;
            id = ClassUSBCOM.GetUSBID(deviceNum);
            this.label7.Text = GetHighword(id).ToString("X");//产品ID
            this.label5.Text = GetLowword(id).ToString("X");//厂商ID
            

        }
        /*****************************************************************************
         * ****函数名称：获取高8位
         * ****入口参数：
         * ****出口参数：无
         * ****************************************************************************/
        public short GetHighword(long tmp)
        {
            return (short)(tmp>>16);
        }
        /*****************************************************************************
         * ****函数名称：获取低8位
         * ****入口参数：
         * ****出口参数：无
         * ****************************************************************************/
        public short GetLowword(long tmp)
        {
            return (short)tmp;
        }
        

       
    

       
    }
}
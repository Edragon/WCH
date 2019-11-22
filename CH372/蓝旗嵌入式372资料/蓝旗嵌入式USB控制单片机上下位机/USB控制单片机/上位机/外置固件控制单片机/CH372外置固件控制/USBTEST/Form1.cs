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
      //  bool TxFlag = false;
       // int TxLen = 0;
        
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
  
        /*****************************************************************************
         * ****函数名称：发送数据
         * ****函数作用：向下位机发送文本框中的数据
         * ****函数描述：
         * ****************************************************************************/
       
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
        /*****************************************************************************
        * ****函数名称：以下为勾选框的响应函数，用于控制LED
        * ****入口参数：
        * ****出口参数：无
        * ****************************************************************************/
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            //MessageBox.Show("changed!");
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x00;
            if (checkBox1.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0,ClassUSBCOM.TxBuf[3]);

        }

        private void button1_Click_1(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x01;
            if (checkBox2.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x02;
            if (checkBox3.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x03;
            if (checkBox4.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x04;
            if (checkBox5.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x05;
            if (checkBox6.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox7_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x06;
            if (checkBox7.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void checkBox8_CheckedChanged(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x07;
            if (checkBox8.Checked)
                ClassUSBCOM.TxBuf[6] = 0x00;
            else
                ClassUSBCOM.TxBuf[6] = 0x01;

            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x06;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x08;
            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
            checkBox1.Checked = true;
            checkBox2.Checked = true;
            checkBox3.Checked = true;
            checkBox4.Checked = true;
            checkBox5.Checked = true;
            checkBox6.Checked = true;
            checkBox7.Checked = true;
            checkBox8.Checked = true;
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            if (ClassUSBCOM.usbstatus == false)
            {
                MessageBox.Show("请先打开USB设备！", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x06;
            ClassUSBCOM.TxBuf[4] = 0x00;
            ClassUSBCOM.TxBuf[5] = 0x09;
            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox4.Checked = false;
            checkBox5.Checked = false;
            checkBox6.Checked = false;
            checkBox7.Checked = false;
            checkBox8.Checked = false;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
           // MessageBox.Show("OK");
            ClassUSBCOM.TxBuf[0] = 0x00;
            ClassUSBCOM.TxBuf[1] = 0x01;
            ClassUSBCOM.TxBuf[2] = 0x00;
            ClassUSBCOM.TxBuf[3] = 0x07;
            ClassUSBCOM.TxBuf[4] = 0x01;
           // ClassUSBCOM.TxBuf[5] = 0x09;
            if (!radioButton1.Checked)
            {
                trackBar1.Enabled = false;
                ClassUSBCOM.TxBuf[5] = 0x00;
            }
            else
            {
                trackBar1.Enabled = true;
                ClassUSBCOM.TxBuf[5] = 0x01;
            }
            ClassUSBCOM.TxBuf[6] =Convert.ToByte(trackBar1.Value);
            ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            label14.Text ="频率值：" +trackBar1.Value.ToString();
            if (radioButton1.Checked)
            {
                ClassUSBCOM.TxBuf[0] = 0x00;
                ClassUSBCOM.TxBuf[1] = 0x01;
                ClassUSBCOM.TxBuf[2] = 0x00;
                ClassUSBCOM.TxBuf[3] = 0x07;
                ClassUSBCOM.TxBuf[4] = 0x01;
                ClassUSBCOM.TxBuf[5] = 0x01;
                ClassUSBCOM.TxBuf[6] = Convert.ToByte(trackBar1.Value);
                ClassUSBCOM.WriteData(0, ClassUSBCOM.TxBuf[3]);
            }
        }

       
    

       
    }
}
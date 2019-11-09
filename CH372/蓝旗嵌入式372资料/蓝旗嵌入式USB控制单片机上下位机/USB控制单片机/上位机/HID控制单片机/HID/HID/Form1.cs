using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HID
{
    public partial class Form1 : Form
    {
        /****************************************************************************************
          * ***全局变量及对象
          * ***
          * **************************************************************************************/
        ClassHid h2 = new ClassHid();       //实例化
        static byte[] TXBuff = new byte[64];//发送数据缓冲区
        ClassHid.report r = new ClassHid.report(0x00, TXBuff);//实例化
        /****************************************************************************************
         * ***窗体加载函数
         * ***
         * **************************************************************************************/     
        public Form1()
        {
            InitializeComponent();
        }
        /****************************************************************************************
         * ***打开设备按钮的响应函数
         * ***
         * **************************************************************************************/
        private void button2_Click(object sender, EventArgs e)
        {
            List<string> deviceList = new List<string>();
            this.label1.Text= "设备状态:"+h2.OpenHidDevice(0x4348,0x5537,"CH372USB").ToString();
            this.label2.Text ="厂商ID:0x"+Convert.ToString(h2.attributes.VendorID,16);
            this.label3.Text = "设备ID:0x" + Convert.ToString(h2.attributes.ProductID, 16);
            this.label4.Text = "产品版本:Ver"+Convert.ToString(h2.attributes.VersionNumber/256,16)+"."+Convert.ToString(h2.attributes.VersionNumber%256,16);
           // this.label5.Text = h2.attributes.Size.ToString();
        }
        /****************************************************************************************
         * ***弹出设备按钮的响应函数
         * ***
         * **************************************************************************************/
        private void button3_Click(object sender, EventArgs e)
        {
            if (h2.deviceOpened)
            {
                h2.CloseDevice();
                this.label1.Text = "设备状态:设备已经弹出";
            }
            else
                this.label1.Text = "设备状态:设备没有打开";
            
        }
        
        
        //以下为LED控制相关控件的响应函数
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x00;
            if (checkBox1.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x01;
            if (checkBox2.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x02;
            if (checkBox3.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x03;
            if (checkBox4.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x04;
            if (checkBox5.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x05;
            if (checkBox6.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox7_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x06;
            if (checkBox7.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void checkBox8_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x07;
            if (checkBox8.Checked)
                TXBuff[6] = 0x00;
            else
                TXBuff[6] = 0x01;
            h2.Write(r);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x08;
            TXBuff[6] = 0x00;
            h2.Write(r);
            checkBox8.Checked = true;
            checkBox7.Checked = true;
            checkBox6.Checked = true;
            checkBox5.Checked = true;
            checkBox4.Checked = true;
            checkBox3.Checked = true;
            checkBox2.Checked = true;
            checkBox1.Checked = true;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x00;
            TXBuff[5] = 0x09;
            TXBuff[6] = 0x00;
            h2.Write(r);
            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox4.Checked = false;
            checkBox5.Checked = false;
            checkBox6.Checked = false;
            checkBox7.Checked = false;
            checkBox8.Checked = false;
        }
        //以下为控制蜂鸣器控件的响应函数

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x01;
            if (this.radioButton1.Checked)
            {
                TXBuff[5] = 0x01;
                TXBuff[6] = Convert.ToByte(this.trackBar1.Value);
            }
            else
                TXBuff[5] = 0x00;
            h2.Write(r);
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            this.label8.Text = "频率值:"+this.trackBar1.Value.ToString();
            TXBuff[0] = 0x00;
            TXBuff[1] = 0x01;
            TXBuff[2] = 0x00;
            TXBuff[3] = 0x07;
            TXBuff[4] = 0x01;
            TXBuff[5] = 0x01;
            TXBuff[6] = Convert.ToByte(this.trackBar1.Value);
            h2.Write(r);
        }

    }
}
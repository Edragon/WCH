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
        ClassHid h2 = new ClassHid();
        static byte[] tb = new byte[64];
        static byte[] rb = new byte[66];
        ClassHid.report r = new ClassHid.report(0x00,tb);
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
        /****************************************************************************************
         * ***发送数据按钮的响应函数
         * ***
         * **************************************************************************************/
        private void button4_Click(object sender, EventArgs e)
        {
            if (h2.deviceOpened)
            {
                if (this.textBox1.Text == "")
                {
                    MessageBox.Show("发送数据为空", "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                string[] s = this.textBox1.Text.ToString().Split(',', '\0');
                int b;
                b = s.Length;
                //TxLen = b;
                for (int i = 0; i < b; i++)
                {
                    try
                    {
                        tb[i] = byte.Parse(s[i]);
                    }
                    catch(Exception ex)
                    {
                        MessageBox.Show(ex.Message,"提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
                    }
                }
                if (h2.Write(r).ToString() == "发送成功")
                {
                    this.label1.Text = "设备状态:发送成功";
                    this.textBox2.Clear();
                   
                }
            }
            else
                MessageBox.Show("请先打开设备","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
       
        }
        /****************************************************************************************
         * ***接收数据按钮的响应函数
         * ***
         * **************************************************************************************/
        private void button1_Click_1(object sender, EventArgs e)
        {
            if (h2.deviceOpened)
            {
                if (h2.RP != null)
                {
                    foreach (byte b in h2.RP.reportBuff)
                    {
                        this.textBox2.Text += b.ToString() + ",";
                    }
                }
                else
                    MessageBox.Show("没有数据接收","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
            }
            else
                MessageBox.Show("请先打开设备","提示信息",MessageBoxButtons.OK,MessageBoxIcon.Warning);
         }
    }
}
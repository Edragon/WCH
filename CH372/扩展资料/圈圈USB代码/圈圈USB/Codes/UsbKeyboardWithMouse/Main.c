/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.07.15
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#include <AT89X52.H>  //头文件
#include "Key.h"
#include "Led.h"
#include "UART.h"
#include "pdiusbd12.h"
#include "UsbCore.h"
#include "config.h"

code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******           《圈圈教你玩USB》之 带鼠标的USB键盘          ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  建立日期：",__DATE__,"                 ******\r\n",
"******                   建立时间：",__TIME__,"                   ******\r\n",
"******                    作者：电脑圈圈                      ******\r\n",
"******                    欢迎访问作者的                      ******\r\n",
"******           USB专区：http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1：http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2：http://computer00.21ic.org       ******\r\n",
"******                  请按K1-K8分别进行测试                 ******\r\n",
"******   K1:功能选择键：按下时K2~K8为键盘，松开时K2~K8为鼠标  ******\r\n",
"******                      键盘功能时                        ******\r\n",
"******             K2:左Shift键  K3:左Alt键  K4:1键           ******\r\n",
"******       K5:2键  K6:3键  K7:Caps Lock键 K8:Num Lock键     ******\r\n",
"******  注意：数字键是数字小键盘，必须打开Num Lock才是数字键  ******\r\n",
"******                      鼠标功能时                        ******\r\n",
"******     K2:光标左移  K3:光标右移  K4:光标上移 K5:光标下移  ******\r\n",
"******            K6:鼠标左键  K7:鼠标中键  K8:鼠标右键       ******\r\n",
"********************************************************************\r\n",
};

/********************************************************************
函数功能：根据按键情况返回报告的函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void SendReport(void)
{
 //需要返回的9字节报告的缓冲（一字节报告ID加键盘8字节报告）
 //而鼠标报告只有4字节，加上一字节报告ID总共5字节，9字节够了。
 
 uint8 Buf[9]={0,0,0,0,0,0,0,0,0}; 
 
 //由于需要返回多个按键，所以需要增加一个变量来保存当前的位置。
 //由于报告ID占用第一字节，所以普通按键从第四字节开始存放。
 uint8 i=3; 
 
 //我们用KEY1键来选择剩余的7个键是键盘功能还是鼠标功能。
 //当KEY1按住时，剩余7个键为键盘功能，这7个键的功能跟键盘
 //实例的一样。当KEY1松开时，剩余7个键为鼠标功能，功能分别为：
 //KEY2：光标左移，KEY3：光标右移，KEY4：光标上移，KEY5：光标下移。
 //KEY6：鼠标左键，KEY7：鼠标中键，KEY8：鼠标右键。
 
 //根据不同的按键设置输入报告
 
 if(KeyPress & KEY1) //如果KEY1按住，则为键盘功能
 {
  Buf[0]=0x01;  //第一字节为报告ID，键盘报告ID为1。
  if(KeyPress & KEY2) //如果KEY2按住
  {
   Buf[1]|=0x02;  //KEY2为左Shift键。
  }
  if(KeyPress & KEY3) //如果KEY3按住
  {
   Buf[1]|=0x04;  //KEY3为左Alt键
  }
  if(KeyPress & KEY4) //如果KEY4按住
  {
   Buf[i]=0x59;  //KEY4为数字小键盘1键。
   i++;  //切换到下个位置。
  }
  if(KeyPress & KEY5)  //如果KEY5按住
  {
   Buf[i]=0x5A;  //KEY5数字小键盘2键。
   i++;  //切换到下个位置。
  }
  if(KeyPress & KEY6)  //如果KEY6按住
  {
   Buf[i]=0x5B;  //KEY6为数字小键盘3键。
   i++;  //切换到下个位置。
  }
  if(KeyPress & KEY7)  //如果KEY7按住
  {
   Buf[i]=0x39;  //KEY7为大/小写切换键。
   i++;  //切换到下个位置。
  }
  if(KeyPress & KEY8)  //如果KEY8按住
  {
   Buf[i]=0x53;  //KEY8为数字小键盘功能切换键。
  }
  //报告准备好了，通过端点1返回，长度为9字节。
  D12WriteEndpointBuffer(3,9,Buf);
 }
 else  //KEY1松开，为鼠标功能
 {
  Buf[0]=0x02;  //第一字节为报告ID，鼠标报告ID为2。
  if(KeyDown & KEY2) //如果KEY2按下
  {
   Buf[2]=-10;  //KEY2为鼠标左移，按一次移动10个单位。
  }
  if(KeyDown & KEY3) //如果KEY3按下
  {
   Buf[2]=10;  //KEY3为鼠标右移，按一次移动10个单位。
  }
  if(KeyDown & KEY4) //如果KEY4按下
  {
   Buf[3]=-10;  //KEY4为鼠标上移，按一次移动10个单位。
  }
  if(KeyDown & KEY5)  //如果KEY5按下
  {
   Buf[3]=10;  //KEY5为鼠标下移，按一次移动10个单位。
  }
  if(KeyPress & KEY6)  //如果KEY6按住
  {
   Buf[1]|=0x01;  //KEY6为鼠标左键。
  }
  if(KeyPress & KEY7)  //如果KEY7按住
  {
   Buf[1]|=0x04;  //KEY7为鼠标中键。
  }
  if(KeyPress & KEY8)  //如果KEY8按住
  {
   Buf[1]|=0x02;  //KEY8为鼠标右键。
  }
  //报告准备好了，通过端点1返回，长度为5字节。
  D12WriteEndpointBuffer(3,5,Buf);
 }
 Ep1InIsBusy=1;  //设置端点忙标志。
 //记得清除KeyUp和KeyDown
 KeyUp=0;
 KeyDown=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：主函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void main(void)  //主函数
{
 uint8 i;
 uint16 id;
 uint8 InterruptSource;
 
 EA=1; //打开中断
 InitKeyboard(); //初始化按键
 InitUART();  //初始化串口
 
 for(i=0;i<24;i++)	  //显示信息
 {
  Prints(HeadTable[i]);
 }
 
 id=D12ReadID();

 Prints("Your D12 chip\'s ID is: ");
 PrintShortIntHex(id);

 if(id==0x1012)
 {
  Prints(". ID is correct! Congratulations!\r\n\r\n");
 }
 else
 {
  Prints(". ID is incorrect! What a pity!\r\n\r\n");
 }
 
 UsbDisconnect();  //先断开USB连接
 UsbConnect();  //将USB连接上
 ConfigValue=0; //配置值初始化为0
 
 while(1)  //死循环
 {
  if(D12GetIntPin()==0) //如果有中断发生
  {
   D12WriteCommand(READ_INTERRUPT_REGISTER);  //写读中断寄存器的命令
   InterruptSource=D12ReadByte(); //读回第一字节的中断寄存器
   if(InterruptSource&0x80)UsbBusSuspend(); //总线挂起中断处理
   if(InterruptSource&0x40)UsbBusReset();   //总线复位中断处理
   if(InterruptSource&0x01)UsbEp0Out();     //端点0输出中断处理
   if(InterruptSource&0x02)UsbEp0In();      //端点0输入中断处理
   if(InterruptSource&0x04)UsbEp1Out();     //端点1输出中断处理
   if(InterruptSource&0x08)UsbEp1In();      //端点1输入中断处理
   if(InterruptSource&0x10)UsbEp2Out();     //端点2输出中断处理
   if(InterruptSource&0x20)UsbEp2In();      //端点2输入中断处理
  }
  if(ConfigValue!=0) //如果已经设置为非0的配置，则可以返回报告数据
  {
   //为显示KEY1选择功能，当KEY1按住时，将LED8点亮，表示键盘功能状态
   if(KeyPress & KEY1)
   {
    OnLed8();
   }
   else
   {
    OffLed8();
   }
   if(!Ep1InIsBusy)  //如果端点1输入没有处于忙状态，则可以发送数据
   {
    KeyCanChange=0;  //禁止按键扫描
    if(KeyUp||KeyDown) //如果有按键事件发生
    {
     SendReport();  //则返回报告
    }
    KeyCanChange=1;  //允许按键扫描
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

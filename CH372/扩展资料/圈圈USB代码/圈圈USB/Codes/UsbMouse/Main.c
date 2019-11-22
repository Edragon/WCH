/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.07.10
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
"******             《圈圈教你玩USB》之 USB鼠标                ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  建立日期：",__DATE__,"                 ******\r\n",
"******                   建立时间：",__TIME__,"                   ******\r\n",
"******                    作者：电脑圈圈                      ******\r\n",
"******                    欢迎访问作者的                      ******\r\n",
"******           USB专区：http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1：http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2：http://computer00.21ic.org       ******\r\n",
"******                  请按K1-K8分别进行测试                 ******\r\n",
"******    K1:光标左移  K2:光标右移  K3:光标上移 K4:光标下移   ******\r\n",
"******    K5:滚轮下滚  K6:滚轮上滚  K7:鼠标左键 K8:鼠标右键   ******\r\n",
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
 //需要返回的4字节报告的缓冲
 //Buf[0]的D0就是左键，D1就是右键，D2就是中键（这里没有）
 //Buf[1]为X轴，Buf[2]为Y轴，Buf[3]为滚轮
 uint8 Buf[4]={0,0,0,0};
 
 //我们不需要KEY1~KEY6按键改变的信息，所以先将它们清0
 KeyUp &=~(KEY1|KEY2|KEY3|KEY4|KEY5|KEY6);
 KeyDown &=~(KEY1|KEY2|KEY3|KEY4|KEY5|KEY6);
 
 //如果有按键按住，并且不是KEY7、KEY8（左、右键）
 //或者KEY7、KEY8任何一个键有变动的话，则需要返回报告
 if((KeyPress&(~(KEY7|KEY8)))||KeyUp||KeyDown)
 {
  if(KeyPress & KEY1) //如果KEY1按住，则光标需要左移，即X轴为负值。
  {
   Buf[1]=-1;  //这里一次往左移动一个单位。
  }
  if(KeyPress & KEY2) //如果KEY2按住，则光标需要右移，即X轴为正值。
  {
   Buf[1]=1;   //这里一次往右移动一个单位。
  }
  if(KeyPress & KEY3) //如果KEY3按住，则光标需要上移，即Y轴为负值。
  {
   Buf[2]=-1;   //这里一次往上移动一个单位。
  }
  if(KeyPress & KEY4)  //如果KEY4按住，则光标需要下移，即Y轴为正值。
  {
   Buf[2]=1;  //这里一次往下移动一个单位。
  }
  if(KeyPress & KEY5)  //如果KEY5按住，则滚轮下滚，即滚轮值为负。
  {
   Buf[3]=-1;  //这里一次往下滚动一个单位。
  }
  if(KeyPress & KEY6)  //如果KEY6按住，则滚轮上滚，既滚轮值为正
  {
   Buf[3]=1;   //这里一次往上滚动一个单位。
  }
  if(KeyPress & KEY7)  //鼠标左键
  {
   Buf[0]|=0x01;  //D0为鼠标左键
  }
  if(KeyPress & KEY8)  //鼠标右键
  {
   Buf[0]|=0x02;  //D1为鼠标右键
  }
  //报告准备好了，通过端点1返回，长度为4字节。
  D12WriteEndpointBuffer(3,4,Buf);
  Ep1InIsBusy=1;  //设置端点忙标志。
 }
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
 
 for(i=0;i<18;i++)	  //显示信息
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
   LEDs=~KeyPress;  //利用板上8个LED显示按键状态，按下时亮
   if(!Ep1InIsBusy)  //如果端点1输入没有处于忙状态，则可以发送数据
   {
    KeyCanChange=0;  //禁止按键扫描
    if(KeyUp||KeyDown||KeyPress) //如果有按键事件发生
    {
     SendReport();  //则返回报告
    }
    KeyCanChange=1;  //允许按键扫描
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

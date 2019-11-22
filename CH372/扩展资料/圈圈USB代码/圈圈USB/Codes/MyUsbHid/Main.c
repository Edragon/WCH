/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.07.14
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
"******       《圈圈教你玩USB》之 用户自定义的USB HID设备      ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  建立日期：",__DATE__,"                 ******\r\n",
"******                   建立时间：",__TIME__,"                   ******\r\n",
"******                    作者：电脑圈圈                      ******\r\n",
"******                    欢迎访问作者的                      ******\r\n",
"******           USB专区：http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1：http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2：http://computer00.21ic.org       ******\r\n",
"******          打开上位机应用软件，然后按K1-K8分别进行测试   ******\r\n",
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
 //需要返回的8字节报告的缓冲。但在本测试程序中，只使用前5字节。
 uint8 Buf[8]={0,0,0,0,0,0,0,0};
 
 //每发送一次数据，则将Count增加一。
 Count++;
 
 //根据不同的按键设置输入报告。这里将8个按键情况放在第一字节。
 Buf[0]=KeyPress;
 
 //根据Count的值设置报告的第二到第五字节。
 Buf[1]=(Count&0xFF);       //最低字节
 Buf[2]=((Count>>8)&0xFF);  //次低字节
 Buf[3]=((Count>>16)&0xFF); //次高字节
 Buf[4]=((Count>>24)&0xFF); //最高字节
 
 //报告准备好了，通过端点1返回，长度为8字节。
 D12WriteEndpointBuffer(3,8,Buf);
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
 
 for(i=0;i<16;i++)	  //显示信息
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
   if(Ep1InIsBusy==0)  //如果端点1输入没有处于忙状态，则可以发送数据
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




/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.08.22
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
"******         《圈圈教你玩USB》之 用户自定义的USB设备        ******\r\n",
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
函数功能：根据按键情况返回数据的函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void SendKeyData(void)
{
 //需要返回8字节数据的缓冲区，这里仅使用前5字节
 uint8 Buf[8]={0,0,0,0,0,0,0,0};
 
 //每发送一次数据，则将Count增加一。
 Count++;
 
 //根据不同的按键设置输入数据。这里将8个按键情况放在第一字节。
 Buf[0]=KeyPress;
 
 //根据Count的值设置缓冲的第二到第四字节。
 Buf[1]=(Count&0xFF);       //最低字节
 Buf[2]=((Count>>8)&0xFF);  //次低字节
 Buf[3]=((Count>>16)&0xFF); //次高字节
 Buf[4]=((Count>>24)&0xFF); //最高字节
 
 //数据准备好了，通过端点1返回，长度为8字节。
 D12WriteEndpointBuffer(3,8,Buf);
 Ep1InIsBusy=1;  //设置端点忙标志。
 
 //记得清除KeyUp和KeyDown
 KeyUp=0;
 KeyDown=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：将串口缓冲区中的数据发送到端点2的函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void SendUartDataToEp2(void)
{
 uint8 Len;
 
 //暂时禁止串行中断，防止UartByteCount在中断中修改而导致不同步
 ES=0;
 //将串口缓冲区接收到的字节数复制出来
 Len=UartByteCount;
 //检查长度是否为0，如果没有收到，数据，则不需要处理，直接返回
 if(Len==0)
 {
  ES=1; //记得打开串口中断
  return;
 }
 //检查Len字节个数据是否跨越了缓冲区边界，如果跨越了，那么本次只发送
 //跨越边界之前的数据，剩余的数据留待下次发送。否则，可以一次发送全部。
 if((Len+UartBufferOutputPoint)>BUF_LEN)
 {
  Len=BUF_LEN-UartBufferOutputPoint;
 }
 //修改缓冲区数据字节数
 UartByteCount-=Len;
 
 //到这里可以打开串口中断了
 ES=1;
 
 //将数据写入到端点2输入缓冲区
 D12WriteEndpointBuffer(5,Len,UartBuffer+UartBufferOutputPoint);
 //修改输出数据的位置
 UartBufferOutputPoint+=Len;
 //如果已经到达缓冲区末尾，则设置回开头
 if(UartBufferOutputPoint>=BUF_LEN)
 {
  UartBufferOutputPoint=0;
 }
 //只有两个缓冲区都满时，才设置端点2输入忙
 if((D12ReadEndpointStatus(5)&0x60)==0x60)
 {
  Ep2InIsBusy=1;
 }
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
  if(ConfigValue!=0) //如果已经设置为非0的配置，则可以返回数据
  {
   //如果端点1输入没有处于忙状态，则可以发送数据
   if(Ep1InIsBusy==0)
   {
    KeyCanChange=0;  //禁止按键扫描
    if(KeyUp||KeyDown) //如果有按键事件发生
    {
     SendKeyData();  //则返回键盘数据
    }
    KeyCanChange=1;  //允许按键扫描
   }
   
   //如果端点2空闲，则发送串口数据到端点2
   if(Ep2InIsBusy==0)
   {
    SendUartDataToEp2(); //调用函数将缓冲区数据发送到端点2
   }
   
   //端点2接收缓冲区中还有数据未发送，则发送到串口
   if(UsbEp2ByteCount!=0) 
   {
    //发送一字节到串口
    UartPutChar(UsbEp2Buffer[UsbEp2BufferOutputPoint]);
    UsbEp2BufferOutputPoint++; //发送位置后移1
    UsbEp2ByteCount--;   //计数值减1
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途

        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

UART.C  file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.08.05
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#include <at89x52.H>

#include "UART.h"
#include "MyType.h"
#include "config.h"

idata uint8 UartBuffer[BUF_LEN];
idata uint8 UsbEp2Buffer[BUF_LEN];

uint8 UartBufferOutputPoint;
uint8 UartBufferInputPoint;
uint8 UsbEp2BufferOutputPoint;

uint8 UartByteCount;
uint8 UsbEp2ByteCount;

volatile uint8 Sending;

//使用T2做波特率发生器可以获得更多的波特率设置。
//删除本行将使用T1作为波特率发生器，最低波特率为300bps，最高为115200bps。
#define USE_T2

/********************************************************************
函数功能：设置串口波特率。
入口参数：BitRate: 需要设置的波特率。
返    回：实际设置的拨特率。
备    注：无。
********************************************************************/
uint32 UartSetBitRate(uint32 BitRate)
{ 
#ifdef USE_T2
 if(BitRate<=230400)
 {
  RCAP2L=0x10000-(Fclk/(BitRate*32));
  RCAP2H=(0x10000-(Fclk/(BitRate*32)))>>8;
 }
 BitRate=(Fclk/32)/(0x10000-((((uint32)RCAP2H)<<8)+RCAP2L));
#else
 if(BitRate<225)
 {
  BitRate=225;
  PCON&=~0x80;  //波特率不加倍
  TH1=256-Fclk/(BitRate*12*16*2);
  TL1=256-Fclk/(BitRate*12*16*2);
  BitRate=(Fclk/12/32)/(0x100-((uint32)TH1));
 }
 else if(BitRate<1200)
 {
  PCON&=~0x80;  //波特率不加倍
  TH1=256-Fclk/(BitRate*12*16*2);
  TL1=256-Fclk/(BitRate*12*16*2);
  BitRate=(Fclk/12/32)/(0x100-((uint32)TH1));
 }
 else if(BitRate<=115200)
 {
  PCON|=0x80;  //波特率加倍
  TH1=256-Fclk/(BitRate*12*16);
  TL1=256-Fclk/(BitRate*12*16);
  BitRate=(Fclk/12/16)/(0x100-((uint32)TH1));
 }
 else
 {
  BitRate=115200;
  PCON|=0x80;  //波特率加倍
  TH1=256-Fclk/(BitRate*12*16);
  TL1=256-Fclk/(BitRate*12*16);
  BitRate=(Fclk/12/16)/(0x100-((uint32)TH1));
 }
#endif
 return BitRate;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：串口初始化。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void InitUART(void)
{
 EA=0;
 
#ifndef USE_T2
 TMOD&=0x0F;
 TMOD|=0x20;    //定时器1工作在模式2
 TCON=0x05;
#endif

 SCON=0x50;       //串口工作在模式1
 
 UartSetBitRate(9600);  //波特率初始化为9600

#ifdef USE_T2
 T2CON=0x34;     //使用T2作为波特率发生器
#endif

 ES=1;         //串行中断允许

#ifndef USE_T2
 TR1=1;        //启动定时器1
#endif

 REN=1;        //允许接收 
 EA=1;         //允许中断
 Sending=0;    //允许发送
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：串口中断处理。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UartISR(void) interrupt 4
{
 if(RI)    //收到数据
 {
  RI=0;   //清中断请求
  //从SBUF读回一字节数据保存在缓冲区中
  UartBuffer[UartBufferInputPoint]=SBUF;
  //将输入位置下移
  UartBufferInputPoint++;
  //如果已经到达缓冲区末尾，则切换到缓冲区开头
  if(UartBufferInputPoint>=BUF_LEN)
  {
   UartBufferInputPoint=0;
  }
  //接收字节数加1
  UartByteCount++;
 }
 else      //发送完一字节数据
 {
  TI=0;
  Sending=0;  //清正在发送标志
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：往串口发送一字节数据。
入口参数：d: 要发送的字节数据。
返    回：无。
备    注：无。
********************************************************************/
void UartPutChar(uint8 d)
{
 SBUF=d; //将数据写入到串口缓冲
 Sending=1;	 //设置发送标志
 while(Sending); //等待发送完毕
}
////////////////////////End of function//////////////////////////////

#if (defined DEBUG0)||(defined DEBUG1)

code uint8 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/********************************************************************
函数功能：发送一个byte的数据。
入口参数：待发送的数据。
返    回：无。
备    注：无。
********************************************************************/
void Printc(uint8 x)
{
 Sending=1;
 SBUF=x;
 while(Sending);
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：以HEX格式发送一个byte的数据。
入口参数：待发送的数据
返    回：无。
备    注：无。
********************************************************************/
void PrintHex(uint8 x)
{
 Printc('0');
 Printc('x');
 Printc(HexTable[x>>4]);
 Printc(HexTable[x&0xf]);
 Printc(' ');
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：发送一个字符串。
入口参数：pd：要发送的字符串指针。
返    回：无。
备    注：无。
********************************************************************/
void Prints(uint8 * pd)
{
 while((*pd)!='\0') //发送字符串，直到遇到0才结束
 {
  UartPutChar(*pd); //发送一个字符
  pd++;  //移动到下一个字符
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：将整数转按十进制字符串发送。
入口参数：x：待显示的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintLongInt(uint32 x)
{
 int8 i;
 uint8 display_buffer[10];

 for(i=9;i>=0;i--)
 {
  display_buffer[i]='0'+x%10;
  x/=10;
 }
 for(i=0;i<9;i++)
 {
  if(display_buffer[i]!='0')break;
 }
 for(;i<10;i++)UartPutChar(display_buffer[i]);
}
////////////////////////End of function//////////////////////////////

#endif

#ifdef DEBUG0
/********************************************************************
函数功能：将短整数按十六进制发送。
入口参数：待发送的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintShortIntHex(uint16 x)
{
 uint8 i;
 uint8 display_buffer[7];
 display_buffer[6]=0;
 display_buffer[0]='0';
 display_buffer[1]='x';
 for(i=5;i>=2;i--) //将整数转换为4个字节的HEX值
 {
  display_buffer[i]=HexTable[(x&0xf)];
  x>>=4;
 }
 Prints(display_buffer);
}
////////////////////////End of function//////////////////////////////
#endif


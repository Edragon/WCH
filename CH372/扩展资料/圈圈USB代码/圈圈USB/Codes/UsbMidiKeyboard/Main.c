/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.08.08
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

#ifdef DEBUG0
code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******        《圈圈教你玩USB》之 USB MIDI 键盘               ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  建立日期：",__DATE__,"                 ******\r\n",
"******                   建立时间：",__TIME__,"                   ******\r\n",
"******                    作者：电脑圈圈                      ******\r\n",
"******                    欢迎访问作者的                      ******\r\n",
"******           USB专区：http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1：http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2：http://computer00.21ic.org       ******\r\n",
"******                  请按K1-K8分别进行测试                 ******\r\n",
"********************************************************************\r\n",
};
#endif
/********************************************************************
函数功能：根据按键情况返回Note On消息的函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void SendNoteOnMsg(void)
{
 //4字节的缓冲区
 uint8 Buf[4];
 
 //Note On消息第一字节固定为0x09，第二字节为0x9n（n为通道号）
 //第三字节为0xKK（K为音高），第四字节为0xVV（V为力度）。
 
 Buf[0]=0x09; //Note On消息的包头
 Buf[1]=0x90; //在通道0上发送Note On消息
 Buf[3]=0x7F; //音量设置为最大
 
 if(KeyDown&KEY1)
 {
  Buf[2]=55;  //C调的5（绝对音高为G音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY1; //清除对应的按键
  return;  //发送一个音符后就返回
 }

 if(KeyDown&KEY2)
 {
  Buf[2]=57;  //C调的6（绝对音高为A音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY2; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY3)
 {
  Buf[2]=60;  //C调的1（绝对音高为C音，即中央C）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY3; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY4)
 {
  Buf[2]=62;  //C调的2（绝对音高为D音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY4; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY5)
 {
  Buf[2]=64;  //C调的3（绝对音高为E音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY5; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY6)
 {
  Buf[2]=67;  //C调的5（绝对音高为G音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY6; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY7)
 {
  Buf[2]=69;  //C调的6（绝对音高为A音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY7; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyDown&KEY8)
 {
  Buf[2]=72;  //C调的1（绝对音高为C音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyDown&=~KEY8; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 //如果有按键弹起，则关闭对应的音
 Buf[3]=0x00;  //音量设置为0
 
 if(KeyUp&KEY1)
 {
  Buf[2]=55;  //C调的5（绝对音高为G音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY1; //清除对应的按键
  return;  //发送一个音符后就返回
 }

 if(KeyUp&KEY2)
 {
  Buf[2]=57;  //C调的6（绝对音高为A音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY2; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY3)
 {
  Buf[2]=60;  //C调的1（绝对音高为C音，即中央C）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY3; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY4)
 {
  Buf[2]=62;  //C调的2（绝对音高为D音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY4; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY5)
 {
  Buf[2]=64;  //C调的3（绝对音高为E音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY5; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY6)
 {
  Buf[2]=67;  //C调的5（绝对音高为G音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY6; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY7)
 {
  Buf[2]=69;  //C调的6（绝对音高为A音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY7; //清除对应的按键
  return;  //发送一个音符后就返回
 }
 
 if(KeyUp&KEY8)
 {
  Buf[2]=72;  //C调的1（绝对音高为C音）
  //通过端点2返回4字节MIDI事件包。
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //设置端点忙标志。
  KeyUp&=~KEY8; //清除对应的按键
  return;  //发送一个音符后就返回
 }
}
////////////////////////End of function//////////////////////////////

void PlaySong(void);

/********************************************************************
函数功能：主函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void main(void)  //主函数
{
#ifdef DEBUG0
 uint8 i;
#endif

 uint16 id;
 uint8 InterruptSource;
 
 EA=1; //打开中断
 InitKeyboard(); //初始化按键
 InitUART();  //初始化串口

 id=D12ReadID();
 
#ifdef DEBUG0
 for(i=0;i<16;i++)	  //显示信息
 {
  Prints(HeadTable[i]);
 }

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
#endif 

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
   if(Ep2InIsBusy==0)  //如果端点2输入没有处于忙状态，则可以发送数据
   {
    KeyCanChange=0;  //禁止按键扫描
    //如果KEY1和KEY8同时按住，则自动播放歌曲
    if(KeyPress==(KEY1|KEY8))
    {
     KeyCanChange=1; //允许按键扫描
     PlaySong(); //播放歌曲
    }
    if(KeyUp||KeyDown) //如果有按键事件发生
    {
     SendNoteOnMsg();  //则返回MIDI Note On消息
    }
    KeyCanChange=1;  //允许按键扫描
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

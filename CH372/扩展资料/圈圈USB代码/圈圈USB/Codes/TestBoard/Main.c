/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

main.c file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.06.27
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

code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******          《圈圈教你玩USB》之 学习板测试程序            ******\r\n",
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
 
 while(1)  //死循环
	{
	 LEDs=~KeyPress;  //将按键结果取反后控制LED
  if(KeyDown)		  //有键按下
  {              //处理按下的键
   if(KeyDown&KEY1)
   {
    Prints("KEY1 down\r\n");
    KeyDown&=~KEY1;
   }
   if(KeyDown&KEY2)
   {
    Prints("KEY2 down\r\n");
    KeyDown&=~KEY2;
   }
   if(KeyDown&KEY3)
   {
    Prints("KEY3 down\r\n");
    KeyDown&=~KEY3;
   }
   if(KeyDown&KEY4)
   {
    Prints("KEY4 down\r\n");
    KeyDown&=~KEY4;
   }
   if(KeyDown&KEY5)
   {
    Prints("KEY5 down\r\n");
    KeyDown&=~KEY5;
   }
   if(KeyDown&KEY6)
   {
    Prints("KEY6 down\r\n");
    KeyDown&=~KEY6;
   }
   if(KeyDown&KEY7)
   {
    Prints("KEY7 down\r\n");
    KeyDown&=~KEY7;
   }
   if(KeyDown&KEY8)
   {
    Prints("KEY8 down\r\n");
    KeyDown&=~KEY8;
   }
  }

  if(KeyUp)//有键释放
  {//处理释放的键
   if(KeyUp&KEY1)
   {
    Prints("KEY1 up\r\n");
    KeyUp&=~KEY1;
   }
   if(KeyUp&KEY2)
   {
    Prints("KEY2 up\r\n");
    KeyUp&=~KEY2;
   }
   if(KeyUp&KEY3)
   {
    Prints("KEY3 up\r\n");
    KeyUp&=~KEY3;
   }
   if(KeyUp&KEY4)
   {
    Prints("KEY4 up\r\n");
    KeyUp&=~KEY4;
   }
   if(KeyUp&KEY5)
   {
    Prints("KEY5 up\r\n");
    KeyUp&=~KEY5;
   }
   if(KeyUp&KEY6)
   {
    Prints("KEY6 up\r\n");
    KeyUp&=~KEY6;
   }
   if(KeyUp&KEY7)
   {
    Prints("KEY7 up\r\n");
    KeyUp&=~KEY7;
   }
   if(KeyUp&KEY8)
   {
    Prints("KEY8 up\r\n");
    KeyUp&=~KEY8;
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

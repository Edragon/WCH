/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

main.c file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.06.27
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#include <AT89X52.H>  //ͷ�ļ�
#include "Key.h"
#include "Led.h"
#include "UART.h"
#include "pdiusbd12.h"

code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******          ��ȦȦ������USB��֮ ѧϰ����Գ���            ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                 ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"                   ******\r\n",
"******                    ���ߣ�����ȦȦ                      ******\r\n",
"******                    ��ӭ�������ߵ�                      ******\r\n",
"******           USBר����http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1��http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2��http://computer00.21ic.org       ******\r\n",
"******                  �밴K1-K8�ֱ���в���                 ******\r\n",
"********************************************************************\r\n",
};

/********************************************************************
�������ܣ���������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void main(void)  //������
{
 uint8 i;
 uint16 id;
 
 EA=1; //���ж�
 InitKeyboard(); //��ʼ������
 InitUART();  //��ʼ������
 
 for(i=0;i<16;i++)	  //��ʾ��Ϣ
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
 
 while(1)  //��ѭ��
	{
	 LEDs=~KeyPress;  //���������ȡ�������LED
  if(KeyDown)		  //�м�����
  {              //�����µļ�
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

  if(KeyUp)//�м��ͷ�
  {//�����ͷŵļ�
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

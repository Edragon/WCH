/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

Led.h file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.06.27
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#ifndef __LED_H__
#define __LED_H__
//ȫ��LED
#define LEDs  P2
//����LED��LED1~LED7
//ע�⡰^�������������ֻ�и�sbit����ʱ�ű�ʾ����һ��λ��
//C�����У���^����ʾ����������Ҫ�ڳ�����ֱ��ʹ�� ��^������ʾĳһλ��
sbit LED1  =     LEDs^0;
sbit LED2  =     LEDs^1;
sbit LED3  =     LEDs^2;
sbit LED4  =     LEDs^3;
sbit LED5  =     LEDs^4;
sbit LED6  =     LEDs^5;
sbit LED7  =     LEDs^6;
sbit LED8  =     LEDs^7;
//����ĳ��LED
#define OnLed1()  LED1=0
#define OnLed2()  LED2=0
#define OnLed3()  LED3=0
#define OnLed4()  LED4=0
#define OnLed5()  LED5=0
#define OnLed6()  LED6=0
#define OnLed7()  LED7=0
#define OnLed8()  LED8=0
//�ر�ĳ��LED
#define OffLed1() LED1=1
#define OffLed2() LED2=1
#define OffLed3() LED3=1
#define OffLed4() LED4=1
#define OffLed5() LED5=1
#define OffLed6() LED6=1
#define OffLed7() LED7=1
#define OffLed8() LED8=1

#endif


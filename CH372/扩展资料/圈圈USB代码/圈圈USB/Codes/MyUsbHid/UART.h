/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

UART.H  file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.07.10
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#ifndef __UART_C__
#define __UART_C__

#include "MyType.h"

void InitUART(void);
void UartPutChar(uint8);
void Prints(uint8 *);
void PrintLongInt(uint32);
void PrintShortIntHex(uint16 x);
void Printc(uint8);
void PrintHex(uint8 x);

#endif

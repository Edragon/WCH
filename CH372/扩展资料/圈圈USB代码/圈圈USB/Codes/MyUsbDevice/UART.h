/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途

        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

UART.H  file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.07.10
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
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

//定义缓冲区的大小，这里使用64字节。
//对于RAM小的单片机，可以修改该定义来减少RAM的使用量。
//但是还要记得修改端点最大长度，否则接收数据可能会溢出。
#define BUF_LEN 64

extern idata uint8 UartBuffer[BUF_LEN];
extern idata uint8 UsbEp2Buffer[BUF_LEN];
extern uint8 UartBufferOutputPoint;
extern uint8 UartBufferInputPoint;
extern uint8 UartByteCount;
extern uint8 UsbEp2ByteCount;
extern uint8 UsbEp2BufferOutputPoint;

#endif

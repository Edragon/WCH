/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

main.c file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.08.08
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#include <AT89X52.H>  //ͷ�ļ�
#include "Led.h"
#include "UART.h"
#include "pdiusbd12.h"
#include "UsbCore.h"
#include "config.h"

#ifdef DEBUG0
code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******                ��ȦȦ������USB��֮ ��U��               ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                 ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"                   ******\r\n",
"******                    ���ߣ�����ȦȦ                      ******\r\n",
"******                    ��ӭ�������ߵ�                      ******\r\n",
"******           USBר����http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1��http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2��http://computer00.21ic.org       ******\r\n",
"********************************************************************\r\n",
};
#endif


/********************************************************************
�������ܣ���������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void main(void)  //������
{
#ifdef DEBUG0
 uint8 i;
#endif

 uint16 id;
 uint8 InterruptSource;
 
 EA=1; //���ж�

 InitUART();  //��ʼ������

 id=D12ReadID();
 
#ifdef DEBUG0
 for(i=0;i<15;i++)	  //��ʾ��Ϣ
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

 UsbDisconnect();  //�ȶϿ�USB����
 UsbConnect();  //��USB������
 ConfigValue=0; //����ֵ��ʼ��Ϊ0
 
 while(1)  //��ѭ��
 {
  if(D12GetIntPin()==0) //������жϷ���
  {
   D12WriteCommand(READ_INTERRUPT_REGISTER);  //д���жϼĴ���������
   InterruptSource=D12ReadByte(); //���ص�һ�ֽڵ��жϼĴ���
   if(InterruptSource&0x80)UsbBusSuspend(); //���߹����жϴ���
   if(InterruptSource&0x40)UsbBusReset();   //���߸�λ�жϴ���
   if(InterruptSource&0x01)UsbEp0Out();     //�˵�0����жϴ���
   if(InterruptSource&0x02)UsbEp0In();      //�˵�0�����жϴ���
   if(InterruptSource&0x04)UsbEp1Out();     //�˵�1����жϴ���
   if(InterruptSource&0x08)UsbEp1In();      //�˵�1�����жϴ���
   if(InterruptSource&0x10)UsbEp2Out();     //�˵�2����жϴ���
   if(InterruptSource&0x20)UsbEp2In();      //�˵�2�����жϴ���
  }
  if(ConfigValue!=0) //����Ѿ�����Ϊ��0�����ã�����Է�������
  {
  }
 }
}
////////////////////////End of function//////////////////////////////

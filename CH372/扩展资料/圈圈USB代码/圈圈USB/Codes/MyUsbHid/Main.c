/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

main.c file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.07.14
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
#include "UsbCore.h"
#include "config.h"

code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******       ��ȦȦ������USB��֮ �û��Զ����USB HID�豸      ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                 ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"                   ******\r\n",
"******                    ���ߣ�����ȦȦ                      ******\r\n",
"******                    ��ӭ�������ߵ�                      ******\r\n",
"******           USBר����http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1��http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2��http://computer00.21ic.org       ******\r\n",
"******          ����λ��Ӧ�������Ȼ��K1-K8�ֱ���в���   ******\r\n",
"********************************************************************\r\n",
};

/********************************************************************
�������ܣ����ݰ���������ر���ĺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void SendReport(void)
{
 //��Ҫ���ص�8�ֽڱ���Ļ��塣���ڱ����Գ����У�ֻʹ��ǰ5�ֽڡ�
 uint8 Buf[8]={0,0,0,0,0,0,0,0};
 
 //ÿ����һ�����ݣ���Count����һ��
 Count++;
 
 //���ݲ�ͬ�İ����������뱨�档���ｫ8������������ڵ�һ�ֽڡ�
 Buf[0]=KeyPress;
 
 //����Count��ֵ���ñ���ĵڶ��������ֽڡ�
 Buf[1]=(Count&0xFF);       //����ֽ�
 Buf[2]=((Count>>8)&0xFF);  //�ε��ֽ�
 Buf[3]=((Count>>16)&0xFF); //�θ��ֽ�
 Buf[4]=((Count>>24)&0xFF); //����ֽ�
 
 //����׼�����ˣ�ͨ���˵�1���أ�����Ϊ8�ֽڡ�
 D12WriteEndpointBuffer(3,8,Buf);
 Ep1InIsBusy=1;  //���ö˵�æ��־��
 
 //�ǵ����KeyUp��KeyDown
 KeyUp=0;
 KeyDown=0;
}
////////////////////////End of function//////////////////////////////

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
 uint8 InterruptSource;
 
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
  if(ConfigValue!=0) //����Ѿ�����Ϊ��0�����ã�����Է��ر�������
  {
   if(Ep1InIsBusy==0)  //����˵�1����û�д���æ״̬������Է�������
   {
    KeyCanChange=0;  //��ֹ����ɨ��
    if(KeyUp||KeyDown) //����а����¼�����
    {
     SendReport();  //�򷵻ر���
    }
    KeyCanChange=1;  //������ɨ��
   }
  }
 }
}
////////////////////////End of function//////////////////////////////




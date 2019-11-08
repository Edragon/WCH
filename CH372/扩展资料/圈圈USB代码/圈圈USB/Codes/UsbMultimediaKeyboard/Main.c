/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

main.c file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.07.15
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
"******          ��ȦȦ������USB��֮ ��ý��USB����             ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                 ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"                   ******\r\n",
"******                    ���ߣ�����ȦȦ                      ******\r\n",
"******                    ��ӭ�������ߵ�                      ******\r\n",
"******           USBר����http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1��http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2��http://computer00.21ic.org       ******\r\n",
"******                  �밴K1-K8�ֱ���в���                 ******\r\n",
"******                      K1:����ѡ���                     ******\r\n",
"******                KE1��סʱ����ͨ���̹���                 ******\r\n",
"******             K2:��Shift��  K3:��Alt��  K4:1��           ******\r\n",
"******       K5:2��  K6:3��  K7:Caps Lock�� K8:Num Lock��     ******\r\n",
"******  ע�⣺���ּ�������С���̣������Num Lock�������ּ�  ******\r\n",
"******                K1�ɿ�ʱ����ý����̹���                ******\r\n",
"******    K2:��������  K3:��������  K4:�������� K5:����/��ͣ  ******\r\n",
"******            K6:һ������  K7:ϵͳ����  K8:ϵͳ�ػ�       ******\r\n",
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
 //��Ҫ���ص�9�ֽڱ���Ļ��壨һ�ֽڱ���ID�Ӽ���8�ֽڱ��棩
 //���û������豸�ı���ֻ��1�ֽڣ�����һ�ֽڱ���ID�ܹ�2�ֽڣ�9�ֽڹ��ˡ�
 
 uint8 Buf[9]={0,0,0,0,0,0,0,0,0}; 
 
 //������Ҫ���ض��������������Ҫ����һ�����������浱ǰ��λ�á�
 //���ڱ���IDռ�õ�һ�ֽڣ�������ͨ�����ӵ����ֽڿ�ʼ��š�
 uint8 i=3; 
 
 //������KEY1����ѡ��ʣ���7��������ͨ���̹��ܻ��Ƕ�ý������ܡ�
 //��KEY1��סʱ��ʣ��7����Ϊ��ͨ���̹��ܣ���7�����Ĺ��ܸ�����
 //ʵ����һ������KEY1�ɿ�ʱ��ʣ��7����Ϊ��ý����̣����ֱܷ�Ϊ��
 //KEY2���������ͣ�KEY3������������KEY4���������أ�KEY5��ֹͣ/���ţ�
 //KEY6������ҳ��KEY7��ϵͳ������KEY8��ϵͳ�ػ���
 
 //���ݲ�ͬ�İ����������뱨��
 
 if(KeyPress & KEY1) //���KEY1��ס����Ϊ���̹���
 {
  Buf[0]=0x01;  //��һ�ֽ�Ϊ����ID�����̱���IDΪ1��
  if(KeyPress & KEY2) //���KEY2��ס
  {
   Buf[1]|=0x02;  //KEY2Ϊ��Shift����
  }
  if(KeyPress & KEY3) //���KEY3��ס
  {
   Buf[1]|=0x04;  //KEY3Ϊ��Alt��
  }
  if(KeyPress & KEY4) //���KEY4��ס
  {
   Buf[i]=0x59;  //KEY4Ϊ����С����1����
   i++;  //�л����¸�λ�á�
  }
  if(KeyPress & KEY5)  //���KEY5��ס
  {
   Buf[i]=0x5A;  //KEY5����С����2����
   i++;  //�л����¸�λ�á�
  }
  if(KeyPress & KEY6)  //���KEY6��ס
  {
   Buf[i]=0x5B;  //KEY6Ϊ����С����3����
   i++;  //�л����¸�λ�á�
  }
  if(KeyPress & KEY7)  //���KEY7��ס
  {
   Buf[i]=0x39;  //KEY7Ϊ��/Сд�л�����
   i++;  //�л����¸�λ�á�
  }
  if(KeyPress & KEY8)  //���KEY8��ס
  {
   Buf[i]=0x53;  //KEY8Ϊ����С���̹����л�����
  }
  //����׼�����ˣ�ͨ���˵�1���أ�����Ϊ9�ֽڡ�
  D12WriteEndpointBuffer(3,9,Buf);
 }
 else  //KEY1�ɿ����û������豸
 {
  Buf[0]=0x02;  //��һ�ֽ�Ϊ����ID���û������豸����IDΪ2��
  if(KeyPress & KEY2) //���KEY2����
  {
   Buf[1]|=0x01;  //KEY2Ϊ�������͡�
  }
  if(KeyPress & KEY3) //���KEY3����
  {
   Buf[1]|=0x02;  //KEY3Ϊ����������
  }
  if(KeyPress & KEY4) //���KEY4����
  {
   Buf[1]|=0x04;  //KEY4Ϊ�������ء�
  }
  if(KeyPress & KEY5)  //���KEY5����
  {
   Buf[1]=0x08;  //KEY5Ϊ����/ֹͣ��
  }
  if(KeyPress & KEY6)  //���KEY6��ס
  {
   Buf[1]|=0x10;  //KEY6Ϊ����ҳ��
  }
  if(KeyPress & KEY7)  //���KEY7��ס
  {
   Buf[1]|=0x20;  //KEY7Ϊϵͳ������
  }
  if(KeyPress & KEY8)  //���KEY8��ס
  {
   Buf[1]|=0x40;  //KEY8Ϊϵͳ�ػ���
  }
  //����׼�����ˣ�ͨ���˵�1���أ�����Ϊ3�ֽڡ�
  D12WriteEndpointBuffer(3,2,Buf);
 }
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
 
 for(i=0;i<24;i++)	  //��ʾ��Ϣ
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
   //Ϊ��ʾKEY1ѡ���ܣ���KEY1��סʱ����LED8��������ʾ���̹���״̬
   if(KeyPress & KEY1)
   {
    OnLed8();
   }
   else
   {
    OffLed8();
   }
   if(!Ep1InIsBusy)  //����˵�1����û�д���æ״̬������Է�������
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

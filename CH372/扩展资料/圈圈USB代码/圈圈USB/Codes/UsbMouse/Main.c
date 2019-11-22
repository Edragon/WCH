/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

main.c file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.07.10
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
"******             ��ȦȦ������USB��֮ USB���                ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                 ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"                   ******\r\n",
"******                    ���ߣ�����ȦȦ                      ******\r\n",
"******                    ��ӭ�������ߵ�                      ******\r\n",
"******           USBר����http://group.ednchina.com/93/       ******\r\n",
"******      BLOG1��http://www.ednchina.com/blog/computer00    ******\r\n",
"******                BLOG2��http://computer00.21ic.org       ******\r\n",
"******                  �밴K1-K8�ֱ���в���                 ******\r\n",
"******    K1:�������  K2:�������  K3:������� K4:�������   ******\r\n",
"******    K5:�����¹�  K6:�����Ϲ�  K7:������ K8:����Ҽ�   ******\r\n",
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
 //��Ҫ���ص�4�ֽڱ���Ļ���
 //Buf[0]��D0���������D1�����Ҽ���D2�����м�������û�У�
 //Buf[1]ΪX�ᣬBuf[2]ΪY�ᣬBuf[3]Ϊ����
 uint8 Buf[4]={0,0,0,0};
 
 //���ǲ���ҪKEY1~KEY6�����ı����Ϣ�������Ƚ�������0
 KeyUp &=~(KEY1|KEY2|KEY3|KEY4|KEY5|KEY6);
 KeyDown &=~(KEY1|KEY2|KEY3|KEY4|KEY5|KEY6);
 
 //����а�����ס�����Ҳ���KEY7��KEY8�����Ҽ���
 //����KEY7��KEY8�κ�һ�����б䶯�Ļ�������Ҫ���ر���
 if((KeyPress&(~(KEY7|KEY8)))||KeyUp||KeyDown)
 {
  if(KeyPress & KEY1) //���KEY1��ס��������Ҫ���ƣ���X��Ϊ��ֵ��
  {
   Buf[1]=-1;  //����һ�������ƶ�һ����λ��
  }
  if(KeyPress & KEY2) //���KEY2��ס��������Ҫ���ƣ���X��Ϊ��ֵ��
  {
   Buf[1]=1;   //����һ�������ƶ�һ����λ��
  }
  if(KeyPress & KEY3) //���KEY3��ס��������Ҫ���ƣ���Y��Ϊ��ֵ��
  {
   Buf[2]=-1;   //����һ�������ƶ�һ����λ��
  }
  if(KeyPress & KEY4)  //���KEY4��ס��������Ҫ���ƣ���Y��Ϊ��ֵ��
  {
   Buf[2]=1;  //����һ�������ƶ�һ����λ��
  }
  if(KeyPress & KEY5)  //���KEY5��ס��������¹���������ֵΪ����
  {
   Buf[3]=-1;  //����һ�����¹���һ����λ��
  }
  if(KeyPress & KEY6)  //���KEY6��ס��������Ϲ����ȹ���ֵΪ��
  {
   Buf[3]=1;   //����һ�����Ϲ���һ����λ��
  }
  if(KeyPress & KEY7)  //������
  {
   Buf[0]|=0x01;  //D0Ϊ������
  }
  if(KeyPress & KEY8)  //����Ҽ�
  {
   Buf[0]|=0x02;  //D1Ϊ����Ҽ�
  }
  //����׼�����ˣ�ͨ���˵�1���أ�����Ϊ4�ֽڡ�
  D12WriteEndpointBuffer(3,4,Buf);
  Ep1InIsBusy=1;  //���ö˵�æ��־��
 }
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
 
 for(i=0;i<18;i++)	  //��ʾ��Ϣ
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
   LEDs=~KeyPress;  //���ð���8��LED��ʾ����״̬������ʱ��
   if(!Ep1InIsBusy)  //����˵�1����û�д���æ״̬������Է�������
   {
    KeyCanChange=0;  //��ֹ����ɨ��
    if(KeyUp||KeyDown||KeyPress) //����а����¼�����
    {
     SendReport();  //�򷵻ر���
    }
    KeyCanChange=1;  //������ɨ��
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

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
#include "Key.h"
#include "Led.h"
#include "UART.h"
#include "pdiusbd12.h"
#include "UsbCore.h"
#include "config.h"

#ifdef DEBUG0
code uint8 HeadTable[][74]={
"********************************************************************\r\n",
"******        ��ȦȦ������USB��֮ USB MIDI ����               ******\r\n",
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
#endif
/********************************************************************
�������ܣ����ݰ����������Note On��Ϣ�ĺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void SendNoteOnMsg(void)
{
 //4�ֽڵĻ�����
 uint8 Buf[4];
 
 //Note On��Ϣ��һ�ֽڹ̶�Ϊ0x09���ڶ��ֽ�Ϊ0x9n��nΪͨ���ţ�
 //�����ֽ�Ϊ0xKK��KΪ���ߣ��������ֽ�Ϊ0xVV��VΪ���ȣ���
 
 Buf[0]=0x09; //Note On��Ϣ�İ�ͷ
 Buf[1]=0x90; //��ͨ��0�Ϸ���Note On��Ϣ
 Buf[3]=0x7F; //��������Ϊ���
 
 if(KeyDown&KEY1)
 {
  Buf[2]=55;  //C����5����������ΪG����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY1; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }

 if(KeyDown&KEY2)
 {
  Buf[2]=57;  //C����6����������ΪA����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY2; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY3)
 {
  Buf[2]=60;  //C����1����������ΪC����������C��
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY3; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY4)
 {
  Buf[2]=62;  //C����2����������ΪD����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY4; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY5)
 {
  Buf[2]=64;  //C����3����������ΪE����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY5; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY6)
 {
  Buf[2]=67;  //C����5����������ΪG����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY6; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY7)
 {
  Buf[2]=69;  //C����6����������ΪA����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY7; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyDown&KEY8)
 {
  Buf[2]=72;  //C����1����������ΪC����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyDown&=~KEY8; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 //����а���������رն�Ӧ����
 Buf[3]=0x00;  //��������Ϊ0
 
 if(KeyUp&KEY1)
 {
  Buf[2]=55;  //C����5����������ΪG����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY1; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }

 if(KeyUp&KEY2)
 {
  Buf[2]=57;  //C����6����������ΪA����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY2; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY3)
 {
  Buf[2]=60;  //C����1����������ΪC����������C��
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY3; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY4)
 {
  Buf[2]=62;  //C����2����������ΪD����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY4; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY5)
 {
  Buf[2]=64;  //C����3����������ΪE����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY5; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY6)
 {
  Buf[2]=67;  //C����5����������ΪG����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY6; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY7)
 {
  Buf[2]=69;  //C����6����������ΪA����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY7; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
 
 if(KeyUp&KEY8)
 {
  Buf[2]=72;  //C����1����������ΪC����
  //ͨ���˵�2����4�ֽ�MIDI�¼�����
  D12WriteEndpointBuffer(5,4,Buf);
  Ep2InIsBusy=1;  //���ö˵�æ��־��
  KeyUp&=~KEY8; //�����Ӧ�İ���
  return;  //����һ��������ͷ���
 }
}
////////////////////////End of function//////////////////////////////

void PlaySong(void);

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
 InitKeyboard(); //��ʼ������
 InitUART();  //��ʼ������

 id=D12ReadID();
 
#ifdef DEBUG0
 for(i=0;i<16;i++)	  //��ʾ��Ϣ
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
   if(Ep2InIsBusy==0)  //����˵�2����û�д���æ״̬������Է�������
   {
    KeyCanChange=0;  //��ֹ����ɨ��
    //���KEY1��KEY8ͬʱ��ס�����Զ����Ÿ���
    if(KeyPress==(KEY1|KEY8))
    {
     KeyCanChange=1; //������ɨ��
     PlaySong(); //���Ÿ���
    }
    if(KeyUp||KeyDown) //����а����¼�����
    {
     SendNoteOnMsg();  //�򷵻�MIDI Note On��Ϣ
    }
    KeyCanChange=1;  //������ɨ��
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

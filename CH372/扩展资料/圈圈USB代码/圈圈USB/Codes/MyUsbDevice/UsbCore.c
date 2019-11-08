/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

UsbCore.c file

���ߣ�����ȦȦ
��������: 2008.06.29
�޸�����: 2008.08.22
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#include "config.h"
#include "pdiusbd12.h"
#include "uart.h"
#include "usbcore.h"
#include "led.h"

idata uint8 Buffer[16];  //���˵�0�õĻ�����

//USB�豸����ĸ��ֶ�
uint8  bmRequestType;
uint8  bRequest;
uint16 wValue;
uint16 wIndex;
uint16 wLength;
//��ǰ�������ݵ�λ��
uint8 * pSendData;
//��Ҫ�������ݵĳ���
uint16 SendLength;
//�Ƿ���Ҫ����0���ݰ��ı�־����USB���ƴ�������ݹ����У�
//�����ص����ݰ��ֽ�������������ʱ������Ϊ���ݹ��̽�����
//��������ֽ�����ʵ����Ҫ���ص��ֽ���������ʵ�ʷ��ص��ֽ�
//���ָպ��Ƕ˵�0��С��������ʱ������Ҫ����һ��0���ȵ����ݰ�
//���������ݹ��̡������������һ����־������������Ƿ���Ҫ����
//һ��0���ȵ����ݰ���
uint8 NeedZeroPacket;

//��ǰ������ֵ��ֻ�������÷�0���ú�
uint8 ConfigValue;

//�˵�1�����Ƿ�æ�ı�־������������������ʱ���ñ�־Ϊ�档
//���������п���ʱ���ñ�־Ϊ�١�
uint8 Ep1InIsBusy;

//�˵�2�����Ƿ�æ�ı�־������������������ʱ���ñ�־Ϊ�档
//���������п���ʱ���ñ�־Ϊ�١�
uint8 Ep2InIsBusy;

//�����˶��ٴα���ļ�����
uint32 Count=0;

//USB�豸�������Ķ���
code uint8 DeviceDescriptor[0x12]=  //�豸������Ϊ18�ֽ�
{
//bLength�ֶΡ��豸�������ĳ���Ϊ18(0x12)�ֽ�
 0x12,
 
//bDescriptorType�ֶΡ��豸�������ı��Ϊ0x01
 0x01,
 
//bcdUSB�ֶΡ��������ð汾ΪUSB1.1����0x0110��
//������С�˽ṹ�����Ե��ֽ����ȣ���0x10��0x01��
 0x10,
 0x01,
 
//bDeviceClass�ֶΡ��û��Զ���USB�豸������Ϊ0xFF
 0xFF,
 
//bDeviceSubClass�ֶΡ�Ϊ0��û�й涨���ࡣ
 0x00,
 
//bDeviceProtocol�ֶΡ�Ϊ0��û�й涨Э�顣
 0x00,
 
//bMaxPacketSize0�ֶΡ�PDIUSBD12�Ķ˵�0��С��16�ֽڡ�
 0x10,
 
//idVender�ֶΡ�����ID�ţ���������ȡ0x8888������ʵ���á�
//ʵ�ʲ�Ʒ�������ʹ�ó���ID�ţ������USBЭ�����볧��ID�š�
//ע��С��ģʽ�����ֽ����ȡ�
 0x88,
 0x88,
 
//idProduct�ֶΡ���ƷID�ţ������ǵ�11��ʵ�飬��������ȡ0x000B��
//ע��С��ģʽ�����ֽ�Ӧ����ǰ��
 0x0B,
 0x00,
 
//bcdDevice�ֶΡ��豸�汾�ţ�ȡ1.0�棬��0x0100��
//С��ģʽ�����ֽ����ȡ�
 0x00,
 0x01,
 
//iManufacturer�ֶΡ������ַ���������ֵ��Ϊ�˷������͹���
//�ַ��������ʹ�1��ʼ�ɡ�
 0x01,

//iProduct�ֶΡ���Ʒ�ַ���������ֵ���ո�����1�������ȡ2�ɡ�
//ע���ַ�������ֵ��Ҫʹ����ͬ��ֵ��
 0x02,
 
//iSerialNumber�ֶΡ��豸�����к��ַ�������ֵ��
//����ȡ3�Ϳ����ˡ�
 0x03,

//bNumConfigurations�ֶΡ����豸�����е���������
//����ֻ��Ҫһ�����þ����ˣ���˸�ֵ����Ϊ1��
 0x01
};
//////////////////////////�豸���������//////////////////////////////

//USB�������������ϵĶ���
//�����������ܳ���Ϊ9+9+7+7+7+7�ֽ�
code uint8 ConfigurationDescriptor[9+9+7+7+7+7]=
{
 /***************����������***********************/
 //bLength�ֶΡ������������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��������������Ϊ0x02��
 0x02,
 
 //wTotalLength�ֶΡ��������������ϵ��ܳ��ȣ�
 //�������������������ӿ��������������������˵��������ȡ�
 sizeof(ConfigurationDescriptor)&0xFF, //���ֽ�
 (sizeof(ConfigurationDescriptor)>>8)&0xFF, //���ֽ�
 
 //bNumInterfaces�ֶΡ������ð����Ľӿ�����ֻ��һ���ӿڡ�
 0x01,
 
 //bConfiguration�ֶΡ������õ�ֵΪ1��
 0x01,
 
 //iConfigurationz�ֶΣ������õ��ַ�������������û�У�Ϊ0��
 0x00,
 
 //bmAttributes�ֶΣ����豸�����ԡ��������ǵİ��������߹���ģ�
 //�������ǲ���ʵ��Զ�̻��ѵĹ��ܣ����Ը��ֶε�ֵΪ0x80��
 0x80,
 
 //bMaxPower�ֶΣ����豸��Ҫ�������������������ǵİ���
 //��Ҫ�ĵ�������100mA�����������������Ϊ100mA������ÿ��λ
 //����Ϊ2mA��������������Ϊ50(0x32)��
 0x32,
 
 /*******************�ӿ�������*********************/
 //bLength�ֶΡ��ӿ��������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��ӿ��������ı��Ϊ0x04��
 0x04,
 
 //bInterfaceNumber�ֶΡ��ýӿڵı�ţ���һ���ӿڣ����Ϊ0��
 0x00,
 
 //bAlternateSetting�ֶΡ��ýӿڵı��ñ�ţ�Ϊ0��
 0x00,
 
 //bNumEndpoints�ֶΡ���0�˵����Ŀ����ʵ����Ҫ�ĸ��˵㣬��˸�ֵΪ4��
 0x04,
 
 //bInterfaceClass�ֶΡ��ýӿ���ʹ�õ��࣬δָ�������豸�����豸����������ָ����
 0x00,
 
 //bInterfaceSubClass�ֶΡ��ýӿ���ʹ�õ����࣬δָ��
 0x00,
 
 //bInterfaceProtocol�ֶΡ��ýӿ���ʹ�õ�Э�飬δָ����
 0x00,
 
 //iConfiguration�ֶΡ��ýӿڵ��ַ�������ֵ������û�У�Ϊ0��
 0x00,
 
 /**********************�ж�����˵�������***********************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�1��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ1����������˵�1�ĵ�ַΪ0x81��
 0x81,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ�ж϶˵㡣�ж϶˵�ı��Ϊ3������λ����Ϊ0��
 0x03,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�1��������Ϊ16�ֽڡ�
 //ע����ֽ����ȡ�
 0x10,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬��������Ϊ10��֡ʱ�䣬��10ms��
 0x0A,
 
 /**********************�ж�����˵�������***********************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�1��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ0����������˵�1�ĵ�ַΪ0x01��
 0x01,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ�ж϶˵㡣�ж϶˵�ı��Ϊ3������λ����Ϊ0��
 0x03,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�1��������Ϊ16�ֽڡ�
 //ע����ֽ����ȡ�
 0x10,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬��������Ϊ10��֡ʱ�䣬��10ms��
 0x0A,
 
  /*************** ��������˵�2������ ******************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�2��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ1����������˵�2�ĵ�ַΪ0x82��
 0x82,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ�����˵㣬�����˵�ı��Ϊ0x02������λ����Ϊ0��
 0x02,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�2��������Ϊ64�ֽڡ�
 //ע����ֽ����ȡ�
 0x40,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬����������˵���Ч��
 0x00,
 
 /*************** ��������˵�2������ ******************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�2��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ0����������˵�2�ĵ�ַΪ0x02��
 0x02,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ�����˵㣬�����˵�ı��Ϊ0x02������λ����Ϊ0��
 0x02,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�2��������Ϊ64�ֽڡ�
 //ע����ֽ����ȡ�
 0x40,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬����������˵���Ч��
 0x00
};
////////////////////////�����������������//////////////////////////

/************************����ID�Ķ���********************/
code uint8 LanguageId[4]=
{
 0x04, //���������ĳ���
 0x03, //�ַ���������
 //0x0409Ϊ��ʽӢ���ID
 0x09,
 0x04
};
////////////////////////����ID���//////////////////////////////////

/**************************************************/
/*********        ��ת���������         **********/
/********* Http://computer00.21ic.org    **********/
/*********        ����: ����ȦȦ         **********/
/*********         ��ӭ���ʹ��          **********/
/*********    ��Ȩ���У�������д������   **********/
/**************************************************/

//http://computer00.21ic.org/user1/2198/archives/2007/42769.html
//�ַ���������ȦȦ��USBר�� Http://group.ednchina.com/93/����Unicode����
//8λС�˸�ʽ
code uint8 ManufacturerStringDescriptor[82]={
82,         //���������ĳ���Ϊ82�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x35, 0x75, //��
0x11, 0x81, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x84, 0x76, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x13, 0x4e, //ר
0x3a, 0x53, //��
0x20, 0x00, // 
0x48, 0x00, //H
0x74, 0x00, //t
0x74, 0x00, //t
0x70, 0x00, //p
0x3a, 0x00, //:
0x2f, 0x00, ///
0x2f, 0x00, ///
0x67, 0x00, //g
0x72, 0x00, //r
0x6f, 0x00, //o
0x75, 0x00, //u
0x70, 0x00, //p
0x2e, 0x00, //.
0x65, 0x00, //e
0x64, 0x00, //d
0x6e, 0x00, //n
0x63, 0x00, //c
0x68, 0x00, //h
0x69, 0x00, //i
0x6e, 0x00, //n
0x61, 0x00, //a
0x2e, 0x00, //.
0x63, 0x00, //c
0x6f, 0x00, //o
0x6d, 0x00, //m
0x2f, 0x00, ///
0x39, 0x00, //9
0x33, 0x00, //3
0x2f, 0x00  ///
};
/////////////////////////�����ַ�������/////////////////////////////

//�ַ�������ȦȦ������USB��֮�û��Զ����USB�豸����Unicode����
//8λС�˸�ʽ
code uint8 ProductStringDescriptor[46]={
46,         //���������ĳ���Ϊ46�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x0a, 0x30, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x59, 0x65, //��
0x60, 0x4f, //��
0xa9, 0x73, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x0b, 0x30, //��
0x4b, 0x4e, //֮
0x28, 0x75, //��
0x37, 0x62, //��
0xea, 0x81, //��
0x9a, 0x5b, //��
0x49, 0x4e, //��
0x84, 0x76, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0xbe, 0x8b, //��
0x07, 0x59, //��

};
////////////////////////��Ʒ�ַ�������////////////////////////////

//�ַ�����2008-08-22����Unicode����
//8λС�˸�ʽ
code uint8 SerialNumberStringDescriptor[22]={
22,         //���������ĳ���Ϊ22�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x32, 0x00, //2
0x30, 0x00, //0
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x32, 0x00, //2
0x32, 0x00  //2
};
//////////////////////��Ʒ���к��ַ�������/////////////////////////

/********************************************************************
�������ܣ���ʱx���뺯����
��ڲ�����x����ʱ�ĺ�������
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void DelayXms(uint16 x)                
{
 uint16 i;
 uint16 j;
 for(i=0;i<x;i++)
 for(j=0;j<227;j++); //ѭ�������ʱ
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�USB�Ͽ����Ӻ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbDisconnect(void)
{
#ifdef DEBUG0
 Prints("�Ͽ�USB���ӡ�\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //д����ģʽ����
 D12WriteByte(0x06); //����ģʽ�ĵ�һ�ֽ�
 D12WriteByte(0x47); //����ģʽ�ĵڶ��ֽ�
 DelayXms(1000);  //�ӳ�1��
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�USB���Ӻ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbConnect(void)
{
#ifdef DEBUG0
 Prints("����USB��\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //д����ģʽ����
 D12WriteByte(0x16); //����ģʽ�ĵ�һ�ֽ�
 D12WriteByte(0x47); //����ģʽ�ĵڶ��ֽ�
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ����߹����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbBusSuspend(void)
{
#ifdef DEBUG0
 Prints("USB���߹���\r\n");
#endif
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ����߸�λ�жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbBusReset(void)
{
#ifdef DEBUG0
 Prints("USB���߸�λ��\r\n");
#endif
 Ep1InIsBusy=0; //��λ��˵�1���뻺�������С�
 Ep2InIsBusy=0; //��λ��˵�2���뻺�������С�
 UartBufferOutputPoint=0;
 UartBufferInputPoint=0;
 UartByteCount=0;
 UsbEp2ByteCount=0;
 UsbEp2BufferOutputPoint=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�����pData��SendLength�����ݷ��͵��˵�0�ĺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0SendData(void)
{
 //������д���˵���ȥ׼������
 //д֮ǰҪ���ж�һ����Ҫ���͵������Ƿ�ȶ˵�0
 //��󳤶ȴ���������˵��С����һ��ֻ�ܷ���
 //�����������ݡ��˵�0����������DeviceDescriptor[7]
 if(SendLength>DeviceDescriptor[7])
 {
  //���������ȷ���
  D12WriteEndpointBuffer(1,DeviceDescriptor[7],pSendData);
  //���ͺ�ʣ���ֽ�������������
  SendLength-=DeviceDescriptor[7];
  //����һ�κ�ָ��λ��Ҫ����
  pSendData+= DeviceDescriptor[7];
 }
 else
 {
  if(SendLength!=0)
  {
   //����������������ֱ�ӷ���
   D12WriteEndpointBuffer(1,SendLength,pSendData);
   //������Ϻ�SendLength���ȱ�Ϊ0
   SendLength=0;
  }
  else //���Ҫ���͵����ݰ�����Ϊ0
  {
   if(NeedZeroPacket==1) //�����Ҫ����0��������
   {
    D12WriteEndpointBuffer(1,0,pSendData); //����0�������ݰ�
    NeedZeroPacket=0; //����Ҫ����0�������ݰ���־
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�0����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0Out(void)
{
#ifdef DEBUG0
 Prints("USB�˵�0����жϡ�\r\n");
#endif
 //��ȡ�˵�0��������״̬���ò�������жϱ�־
 //���жϵ�5λ�Ƿ�Ϊ1������ǣ���˵���ǽ�����
 if(D12ReadEndpointLastStatus(0)&0x20)
 {
  D12ReadEndpointBuffer(0,16,Buffer); //��������������
  D12AcknowledgeSetup(); //Ӧ������
  D12ClearBuffer(); //�建����
  //������������豸����ĸ��ֶ���
  bmRequestType=Buffer[0];
  bRequest=Buffer[1];
  wValue=Buffer[2]+(((uint16)Buffer[3])<<8);
  wIndex=Buffer[4]+(((uint16)Buffer[5])<<8);
  wLength=Buffer[6]+(((uint16)Buffer[7])<<8);
  //����Ĵ����жϾ�������󣬲����ݲ�ͬ�����������ز���
  //���D7λΪ1����˵������������
  if((bmRequestType&0x80)==0x80)
  {
   //����bmRequestType��D6~5λɢת��D6~5λ��ʾ���������
   //0Ϊ��׼����1Ϊ������2Ϊ��������
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //��׼����
     #ifdef DEBUG0
      Prints("USB��׼��������");
     #endif
     //USBЭ�鶨���˼�����׼������������ʵ����Щ��׼���󼴿�
     //����Ĵ�����bRequest�У��Բ�ͬ������������ɢת
     //��ʵ�ϣ����ǻ���Ҫ�Խ����߽���ɢת����Ϊ��ͬ�����������
     //�ǲ�һ���ġ���������bmRequestType��D4~D0λ�ж��塣
     //��������Ϊ�˼򻯲�������Щ��ʡ���˶Խ����ߵ��жϡ�
     //�����ȡ������������ֻ����������������������
     switch(bRequest)
     {
      case GET_CONFIGURATION: //��ȡ����
       #ifdef DEBUG0
        Prints("��ȡ���á�\r\n");
       #endif
      break;
      
      case GET_DESCRIPTOR:  //��ȡ������
       #ifdef DEBUG0
        Prints("��ȡ����������");
       #endif
       //�����������ͽ���ɢת������ȫ���豸��
       //��׼����ֻ֧�ַ��͵��豸���豸�����á��ַ�������������
       switch((wValue>>8)&0xFF)
        {
         case DEVICE_DESCRIPTOR: //�豸������
          #ifdef DEBUG0
           Prints("�豸��������\r\n");
          #endif
          pSendData=DeviceDescriptor;  //��Ҫ���͵�����
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //������������豸��������������ݳ��Ⱦ���
          //DeviceDescriptor[0]���������ı�ʵ�ʵĳ���
          //��ôֻ����ʵ�ʳ��ȵ�����
          if(wLength>DeviceDescriptor[0])
          {
           SendLength=DeviceDescriptor[0];
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();
         break;
         
         case CONFIGURATION_DESCRIPTOR:  //����������
          #ifdef DEBUG0
           Prints("������������\r\n");
          #endif
          pSendData=ConfigurationDescriptor; //��Ҫ���͵�����Ϊ����������
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //������������������������ϣ�������ݳ��Ⱦ���
          //ConfigurationDescriptor[3]*256+ConfigurationDescriptor[2]��
          //�������ı�ʵ�ʵĳ�����ôֻ����ʵ�ʳ��ȵ�����
          SendLength=ConfigurationDescriptor[3];
          SendLength=SendLength*256+ConfigurationDescriptor[2];
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();
         break;
         
         case STRING_DESCRIPTOR:  //�ַ���������
          #ifdef DEBUG0
           Prints("�ַ���������");
          #endif
          switch(wValue&0xFF)  //����wValue�ĵ��ֽڣ�����ֵ��ɢת
          {
           case 0:  //��ȡ����ID
            #ifdef DEBUG0
             Prints("(����ID)��\r\n");
            #endif
            pSendData=LanguageId;
            SendLength=LanguageId[0];
           break;
           
           case 1:  //�����ַ���������ֵΪ1����������Ϊ�����ַ���
           #ifdef DEBUG0
             Prints("(��������)��\r\n");
            #endif
            pSendData=ManufacturerStringDescriptor;
            SendLength=ManufacturerStringDescriptor[0];
           break;
           
           case 2:  //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
           #ifdef DEBUG0
             Prints("(��Ʒ����)��\r\n");
            #endif
            pSendData=ProductStringDescriptor;
            SendLength=ProductStringDescriptor[0];
           break;
           
           case 3:  //��Ʒ���кŵ�����ֵΪ3����������Ϊ���к�
           #ifdef DEBUG0
             Prints("(��Ʒ���к�)��\r\n");
            #endif
            pSendData=SerialNumberStringDescriptor;
            SendLength=SerialNumberStringDescriptor[0];
           break;
           
           default :
            #ifdef DEBUG0
             Prints("(δ֪������ֵ)��\r\n");
            #endif
            //����δ֪����ֵ�����󣬷���һ��0���ȵİ�
            SendLength=0;
            NeedZeroPacket=1;
           break;
          }
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //�������ı�ʵ�ʵĳ�����ôֻ����ʵ�ʳ��ȵ�����
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();         
         break;

         case REPORT_DESCRIPTOR:  //����������
          #ifdef DEBUG0
           Prints("������������\r\n");
          #endif
         break;
                 
         default:  //����������
          #ifdef DEBUG0
           Prints("���������������������룺");
           PrintHex((wValue>>8)&0xFF);
           Prints("\r\n");
          #endif
         break;
        }
       break;
      
      case GET_INTERFACE: //��ȡ�ӿ�
       #ifdef DEBUG0
        Prints("��ȡ�ӿڡ�\r\n");
       #endif
      break;
      
      case GET_STATUS: //��ȡ״̬
       #ifdef DEBUG0
        Prints("��ȡ״̬��\r\n");
       #endif
      break;
      
      case SYNCH_FRAME: //ͬ��֡
       #ifdef DEBUG0
        Prints("ͬ��֡��\r\n");
       #endif
      break;
      
      default:  //δ����ı�׼����
       #ifdef DEBUG0
        Prints("����δ����ı�׼��������\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //������
     #ifdef DEBUG0
      Prints("USB����������\r\n");
     #endif
    break;
    
    case 2:  //��������
     #ifdef DEBUG0
      Prints("USB������������\r\n");
     #endif
    break;
    
    default: //δ�������������ֻ��ʾһ��������Ϣ��
     #ifdef DEBUG0
      Prints("����δ�������������\r\n");
     #endif
    break;
   }
  }
  //����˵�����������
  else //if(bmRequestType&0x80==0x80)֮else
  {
   //����bmRequestType��D6~5λɢת��D6~5λ��ʾ���������
   //0Ϊ��׼����1Ϊ������2Ϊ��������
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //��׼����
     #ifdef DEBUG0
      Prints("USB��׼�������");
     #endif
     //USBЭ�鶨���˼�����׼�����������ʵ����Щ��׼���󼴿�
     //����Ĵ�����bRequest�У��Բ�ͬ������������ɢת
     switch(bRequest)
     {
      case CLEAR_FEATURE: //�������
       #ifdef DEBUG0
        Prints("������ԡ�\r\n");
       #endif
      break;
      
      case SET_ADDRESS:  //���õ�ַ
       #ifdef DEBUG0
        Prints("���õ�ַ����ַΪ��");
        PrintHex(wValue&0xFF); //��ʾ�����õĵ�ַ
        Prints("\r\n");
       #endif
       D12SetAddress(wValue&0xFF); //wValue�еĵ��ֽ������õĵ�ֵַ
       //���õ�ַû�����ݹ��̣�ֱ�ӽ��뵽״̬���̣�����һ��0���ȵ����ݰ�
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      case SET_CONFIGURATION: //��������
       #ifdef DEBUG0
        Prints("�������á�\r\n");
       #endif
       //ʹ�ܷ�0�˵㡣��0�˵�ֻ��������Ϊ��0�����ú����ʹ�ܡ�
       //wValue�ĵ��ֽ�Ϊ���õ�ֵ�������ֵΪ��0������ʹ�ܷ�0�˵㡣
       //���浱ǰ����ֵ
       ConfigValue=wValue&0xFF;
       D12SetEndpointEnable(ConfigValue);
       //����һ��0���ȵ�״̬���ݰ�
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      case SET_DESCRIPTOR: //����������
       #ifdef DEBUG0
        Prints("������������\r\n");
       #endif
      break;
      
      case SET_FEATURE: //��������
       #ifdef DEBUG0
        Prints("�������ԡ�\r\n");
       #endif
      break;

      case SET_INTERFACE: //���ýӿ�
       #ifdef DEBUG0
        Prints("���ýӿڡ�\r\n");
       #endif
      break;      
      
      default:  //δ����ı�׼����
       #ifdef DEBUG0
        Prints("����δ����ı�׼�������\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //������
     #ifdef DEBUG0
      Prints("USB���������");
     #endif
     switch(bRequest)
     {
      case SET_IDLE:
       #ifdef DEBUG0
        Prints("���ÿ��С�\r\n");
       #endif
       //ֻ��Ҫ����һ��0���ȵ����ݰ�����
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      default:
       #ifdef DEBUG0
        Prints("δ֪����\r\n");
       #endif
      break;
     }
    break;
    
    case 2:  //��������
     #ifdef DEBUG0
      Prints("USB�����������\r\n");
     #endif
    break;
    
    default: //δ�������������ֻ��ʾһ��������Ϣ��
     #ifdef DEBUG0
      Prints("����δ������������\r\n");
     #endif
    break;
   }
  }
 }
 //��ͨ�������
 else //if(D12ReadEndpointLastStatus(0)&0x20)֮else
 {
  D12ReadEndpointBuffer(0,16,Buffer);
  D12ClearBuffer();
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�0�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0In(void)
{
#ifdef DEBUG0
 Prints("USB�˵�0�����жϡ�\r\n");
#endif
 //�������״̬���⽫����˵�0���жϱ�־λ
 D12ReadEndpointLastStatus(1);
 //����ʣ����ֽ���
 UsbEp0SendData();
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�1����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp1Out(void)
{
 uint8 Buf[8]; //��������8�ֽڵ���������
#ifdef DEBUG0
 Prints("USB�˵�1����жϡ�\r\n");
#endif
 //���˵����״̬���⽫����˵�1������жϱ�־λ
 D12ReadEndpointLastStatus(2);
 //�Ӷ˵�1����������8�ֽ�����
 D12ReadEndpointBuffer(2,8,Buf);
 //����˵㻺����
 D12ClearBuffer();
 
 //������ݵ�һ�ֽ�ΪLED״̬��ĳλΪ1ʱ����ʾLED����
 LEDs=~Buf[0];
 
 //������ݵĵڶ��ֽڷ�0ʱ��������ͼ�����Count��
 if(Buf[1]!=0)
 {
  Count=0;
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�1�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp1In(void)
{
#ifdef DEBUG0
 Prints("USB�˵�1�����жϡ�\r\n");
#endif
 //�������״̬���⽫����˵�1������жϱ�־λ
 D12ReadEndpointLastStatus(3);
 //�˵�1���봦�ڿ���״̬
 Ep1InIsBusy=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�2����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp2Out(void)
{
#ifdef DEBUG0
 Prints("USB�˵�2����жϡ�\r\n");
#endif
 //����������е����ݻ�δͨ�����ڷ�����ϣ�����ʱ��������жϣ�ֱ�ӷ��ء�
 if(UsbEp2ByteCount!=0)return;

/* �ɰ���룬��BUG���޸��ں�������ʱ����ж�
 //��������״̬���⽫����˵�2������жϱ�־λ��
 //ע��˵�2�и�˫������ƣ�������ж�֮ǰ���ȼ���Ƿ�����������
 //�Ƿ�ȫ���ˣ��������������ȫ���Ļ����Ͳ�������жϱ�־��ֻ�е�
 //������������ȫ����ʱ�����Ҫ����жϱ�־��
 if((D12ReadEndpointStatus(4)&0x60)!=0x60)
 {
  D12ReadEndpointLastStatus(4);
 }
*/
  
 //�����˵�2�����ݡ�����ֵΪʵ�ʶ����������ֽ���
 UsbEp2ByteCount=D12ReadEndpointBuffer(4,BUF_LEN,UsbEp2Buffer);
 //����˵㻺����
 D12ClearBuffer();
 
 //���λ����Ϊ0
 UsbEp2BufferOutputPoint=0;

 //�������������ж�û������ʱ����������жϱ�־
 if(!(D12ReadEndpointStatus(4)&0x60))
 {
  //�������״̬���⽫����˵�2������жϱ�־λ
  D12ReadEndpointLastStatus(4);
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�2�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp2In(void)
{
#ifdef DEBUG0
 Prints("USB�˵�2�����жϡ�\r\n");
#endif
 //�������״̬���⽫����˵�2������жϱ�־λ
 D12ReadEndpointLastStatus(5);
 //�˵�2���봦�ڿ���״̬
 Ep2InIsBusy=0;
}
////////////////////////End of function//////////////////////////////

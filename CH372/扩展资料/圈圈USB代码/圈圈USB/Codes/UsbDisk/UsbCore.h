/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

UsbCore.h file

���ߣ�����ȦȦ
��������: 2008.06.29
�޸�����: 2008.07.10
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/

#ifndef __USBCORE_H__
#define __USBCORE_H__

#define GET_STATUS         0
#define CLEAR_FEATURE      1
#define SET_FEATURE        3
#define SET_ADDRESS        5
#define GET_DESCRIPTOR     6
#define SET_DESCRIPTOR     7
#define GET_CONFIGURATION  8
#define SET_CONFIGURATION  9
#define GET_INTERFACE      10
#define SET_INTERFACE      11
#define SYNCH_FRAME        12

#define DEVICE_DESCRIPTOR         1
#define CONFIGURATION_DESCRIPTOR  2
#define STRING_DESCRIPTOR         3
#define INTERFACE_DESCRIPTOR      4
#define ENDPOINT_DESCRIPTOR       5
#define REPORT_DESCRIPTOR         0x22

#define SET_IDLE 0x0A


#define GET_MAX_LUN  0xFE
#define MASS_STORAGE_RESET 0xFF

#define COMMAND_STAGE 0x00
#define DATA_STAGE    0x01
#define STATUS_STAGE  0x02

void UsbDisconnect(void); //USB�Ͽ�����
void UsbConnect(void);    //USB����
void UsbBusSuspend(void); //���߹����жϴ���
void UsbBusReset(void);   //���߸�λ�жϴ���
void UsbEp0Out(void);     //�˵�0����жϴ���
void UsbEp0In(void);      //�˵�0�����жϴ���
void UsbEp1Out(void);     //�˵�1����жϴ���
void UsbEp1In(void);      //�˵�1�����жϴ���
void UsbEp2Out(void);     //�˵�2����жϴ���
void UsbEp2In(void);      //�˵�2�����жϴ���

extern uint8 ConfigValue;  //��ǰ����ֵ
extern uint8 Ep1InIsBusy;  //�˵�1�����Ƿ�æ
extern uint8 Ep2InIsBusy;  //�˵�2�����Ƿ�æ

extern idata uint8 CBW[31];
extern idata uint8 CSW[13];
extern uint8 TransportStage;

#endif
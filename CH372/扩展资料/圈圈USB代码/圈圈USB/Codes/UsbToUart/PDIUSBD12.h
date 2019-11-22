/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

PDIUSBD12.H  file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.07.10
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/


#ifndef __PDIUSBD12_H__
#define __PDIUSBD12_H__

#include <at89x52.h>
#include "MyType.h"

//�����ַ�����ݵ�ַ
#define D12_COMMAND_ADD           1
#define D12_DATA_ADD              0

//PDIUSBD12оƬ��������
#define D12_DATA                  P0
#define D12_A0                    P3_5
#define D12_WR                    P3_6
#define D12_RD                    P3_7
#define D12_INT                   P3_2

//ѡ����������ݵ�ַ
#define D12SetCommandAddr() D12_A0=D12_COMMAND_ADD
#define D12SetDataAddr()    D12_A0=D12_DATA_ADD
//WR����
#define D12SetWr() D12_WR=1
#define D12ClrWr() D12_WR=0
//RD����
#define D12SetRd() D12_RD=1
#define D12ClrRd() D12_RD=0
//��ȡ�ж����ŵ�ƽ״̬
#define D12GetIntPin()  D12_INT
//��д����
#define D12GetData() D12_DATA
#define D12SetData(Value) D12_DATA=(Value)

//�����ݿ�����Ϊ����״̬��51��Ƭ���˿�д1����Ϊ����״̬
#define D12SetPortIn() D12_DATA=0xFF

//�����ݿ�����Ϊ���״̬������51��Ƭ����׼˫��IO�ڣ����Բ����л���Ϊ�պ�
#define D12SetPortOut()

//D12�Ķ�ID����
#define Read_ID  0xFD

//D12������ģʽ����
#define D12_SET_MODE  0xF3

//D12�Ķ��жϼĴ�������
#define READ_INTERRUPT_REGISTER  0xF4

//D12���˵㻺����������
#define D12_READ_BUFFER 0xF0

//D12д�˵㻺����������
#define D12_WRITE_BUFFER 0xF0

//D12������ն˵㻺����������
#define D12_CLEAR_BUFFER    0xF2

//D12ʹ�ܷ��Ͷ˵㻺����������
#define D12_VALIDATE_BUFFER 0xFA

//D12��Ӧ�����ð�����
#define D12_ACKNOWLEDGE_SETUP 0xF1

//D12�����õ�ַ/ʹ������
#define D12_SET_ADDRESS_ENABLE 0xD0

//D12��ʹ�ܶ˵�����
#define D12_SET_ENDPOINT_ENABLE 0xD8

//��������
void D12WriteCommand(uint8);
uint8 D12ReadByte(void);
uint16 D12ReadID(void);
void D12WriteByte(uint8);
void DelayXms(uint16);
uint8 D12ReadEndpointBuffer(uint8 Endp, uint8 Len, uint8 *Buf);
uint8 D12WriteEndpointBuffer(uint8 Endp, uint8 Len, uint8 *Buf);
void D12ClearBuffer(void);
uint8 D12ReadEndpointLastStatus(uint8 Endp);
void D12AcknowledgeSetup(void);
void D12SetAddress(uint8 Addr);
void D12SetEndpointEnable(uint8 Enable);
uint8 D12ReadEndpointStatus(uint8 Endp);

#endif

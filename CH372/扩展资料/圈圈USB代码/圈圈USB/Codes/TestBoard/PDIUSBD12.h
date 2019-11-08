/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

PDIUSBD12.H  file

���ߣ�����ȦȦ
��������: 2008.06.27
�޸�����: 2008.06.27
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
//��ȡ�ж�״̬
#define D12GetIntPin()  D12_INT
//��д����
#define D12GetData() D12_DATA
#define D12SetData(Value) D12_DATA=(Value)

//�����ݿ�����Ϊ����״̬��51��Ƭ���˿�д1����Ϊ����״̬
#define D12SetPortIn() D12_DATA=0xFF

//�����ݿ�����Ϊ���״̬������51��Ƭ����׼˫��IO�ڣ����Բ����л���Ϊ�պ�
#define D12SetPortOut()

//D12�Ķ�ID����
#define Read_ID                  0xFD

//��������
void D12WriteCommand(uint8);
uint8 D12ReadByte(void);
uint16 D12ReadID(void);

#endif

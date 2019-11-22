/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
			
        ��ӭ�����ҵ�USBר����http://group.ednchina.com/93/
        ��ӭ�����ҵ�blog��   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        ��лPCB�����̡�������԰�� http://bbs.cepark.com/

SCSI.c file

���ߣ�����ȦȦ
��������: 2008.08.15
�޸�����: 2008.08.20
�汾��V1.1
��Ȩ���У�����ؾ���
Copyright(C) ����ȦȦ 2008-2018
All rights reserved            
*******************************************************************/


#include "MyType.H"
#include "UsbCore.h"
#include "PDIUSBD12.h"
#include "SCSI.h"
#include "Uart.h"
#include "config.h"
#include "FAT.h"

//����˵�2��������Ϊ64�ֽ�
#define EP2_SIZE 64

//����˵�2���ݵĻ�����
idata uint8 Ep2Buffer[EP2_SIZE];

uint32 ByteAddr;  //�ֽڵ�ַ

//INQUIRY������Ҫ���ص�����
//���������INQUIRY������Ӧ���ݸ�ʽ
code uint8 DiskInf[36]=
{
 0x00, //�����豸
 0x00, //�������λD7ΪRMB��RMB=0����ʾ�����Ƴ��豸�����RMB=1����Ϊ���Ƴ��豸��
 0x00, //���ְ汾��0
 0x01, //������Ӧ��ʽ
 0x1F, //�������ݳ��ȣ�Ϊ31�ֽ�
 0x00, //����
 0x00, //����
 0x00, //����
 0xB5,0xE7,0XC4,0xD4,0xC8,0xA6,0xC8,0xA6, //���̱�ʶ��Ϊ�ַ���������ȦȦ��
 
 //��Ʒ��ʶ��Ϊ�ַ������Լ����ļ�U�̡�
 0xD7,0xD4,0xBC,0xBA,0xD7,0xF6,0xB5,0xC4,0xBC,0xD9,0x55,0xC5,0xCC,0x00,0x00,0x00,
 0x31,0x2E,0x30,0x31 //��Ʒ�汾�ţ�Ϊ1.01
};

//READ_FORMAT_CAPACITIES������Ҫ���ص�����
//���������READ_FORMAT_CAPACITIES������Ӧ���ݸ�ʽ
code uint8 MaximumCapacity[12]=
{
 0x00, 0x00, 0x00, //����
 0x08,  //�����б���
 0x01, 0x00, 0x00, 0x00,  //����(���֧��8GB)
 0x03, //����������Ϊ3����ʾ���֧�ֵĸ�ʽ������
 0x00, 0x02, 0x00 //ÿ���СΪ512�ֽ�
};

//READ_CAPACITY������Ҫ���ص�����
code uint8 DiskCapacity[8]=
{
 0x00,0x03,0xFF,0xFF, //�ܹ����ʵ�����߼����ַ
 0x00,0x00,0x02,0x00  //��ĳ���
 //���Ըô��̵�����Ϊ
 //(0x3FFFF+1)*0x200 = 0x8000000 = 128*1024*1024 = 128MB.
};

//REQUEST SENSE������Ҫ���ص����ݣ�����̶�Ϊ��Ч����
//��ο��������ݽṹ�Ľ���
code uint8 SenseData[18]=
{
 0x70, //������룬�̶�Ϊ0x70
 0x00, //����
 0x05, //Sense KeyΪ0x05����ʾ��Ч����ILLEGAL REQUEST��
 0x00, 0x00, 0x00, 0x00, //InformationΪ0
 0x0A, //�������ݳ���Ϊ10�ֽ�
 0x00, 0x00, 0x00, 0x00, //����
 0x20, //Additional Sense Code(ASC)Ϊ0x20����ʾ��Ч��������루INVALID COMMAND OPERATION CODE��
 0x00, //Additional Sense Code Qualifier(ASCQ)Ϊ0
 0x00, 0x00, 0x00, 0x00 //����
};

uint8 * pEp2SendData;
uint32 Ep2DataLength;

/********************************************************************
�������ܣ���CBW�л�ȡ�������ݵ��ֽ�����
��ڲ������ޡ�
��    �أ���Ҫ������ֽ�����
��    ע���ޡ�
********************************************************************/
uint32 GetDataTransferLength(void)
{
 uint32 Len;
 
 //CBW[8]~CBW[11]Ϊ���䳤�ȣ�С�˽ṹ��
 
 Len=CBW[11];
 Len=Len*256+CBW[10];
 Len=Len*256+CBW[9];
 Len=Len*256+CBW[8];
 
 return Len;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ���CBW�л�ȡ�߼����ַLBA���ֽ�����
��ڲ������ޡ�
��    �أ��߼����ַLBA��
��    ע���ޡ�
********************************************************************/
uint32 GetLba(void)
{
 uint32 Lba;
 
 //����д����ʱ��CBW[17]~CBW[20]Ϊ�߼����ַ����˽ṹ��
 
 Lba=CBW[17];
 Lba=Lba*256+CBW[18];
 Lba=Lba*256+CBW[19];
 Lba=Lba*256+CBW[20];
 
 return Lba;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ����CSW��
��ڲ�����Residue��ʣ���ֽ�����Status������ִ�е�״̬��
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void SetCsw(uint32 Residue, uint8 Status)
{
 //����CSW��ǩ������ʵ���Բ���ÿ�ζ����õģ�
 //��ʼ��ʼ������һ�ξ����ˣ�����ÿ�ζ�����
 CSW[0]='U';
 CSW[1]='S';
 CSW[2]='B';
 CSW[3]='S';
 
 //����dCBWTag��CSW��dCSWTag��ȥ
 CSW[4]=CBW[4];
 CSW[5]=CBW[5];
 CSW[6]=CBW[6];
 CSW[7]=CBW[7];
    
 //ʣ���ֽ���
 CSW[8]=Residue&0xFF;
 CSW[9]=(Residue>>8)&0xFF;
 CSW[10]=(Residue>>16)&0xFF;
 CSW[11]=(Residue>>24)&0xFF;
 
 //����ִ�е�״̬��0��ʾ�ɹ���1��ʾʧ�ܡ�
 CSW[12]=Status;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ���ȡ�������ݺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void GetDiskData(void)
{
 //�жϸ÷���ʲô����
 if(ByteAddr==0) pEp2SendData=Dbr; //����DBR
 if(ByteAddr==512) pEp2SendData=Fat; //����FAT
 if((ByteAddr>=576)&&(ByteAddr<16896)) pEp2SendData=Zeros;
 if(ByteAddr==16896) pEp2SendData=Fat; //����FAT������FAT��
 if((ByteAddr>=16960)&&(ByteAddr<33280)) pEp2SendData=Zeros;
 if(ByteAddr==33280) pEp2SendData=RootDir; //���ظ�Ŀ¼
 if((ByteAddr>=33344)&&(ByteAddr<49664)) pEp2SendData=Zeros;
 if(ByteAddr==49664) pEp2SendData=TestFileData; //�����ļ�����
 if(ByteAddr>50175) pEp2SendData=Zeros;
 
 ByteAddr+=EP2_SIZE; //�����ֽڵ�ַ��ÿ�η����������ȵ�����
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�������ͨ���˵�2���͡�
��ڲ������ޡ�
��    �أ��ޡ�
��    ע�����������ݳ���Ϊ0�����Ҵ������ݽ׶�ʱ�����Զ�����CSW��
********************************************************************/
void Ep2SendData(void)
{
 if(Ep2DataLength==0) //�����Ҫ���͵����ݳ���Ϊ0
 {
  if(TransportStage==DATA_STAGE) //���Ҵ������ݽ׶�
  {
   //��ֱ�ӽ���״̬�׶�
   TransportStage=STATUS_STAGE;
   Ep2DataLength=sizeof(CSW); //���ݳ���ΪCSW�Ĵ�С
   pEp2SendData=CSW; //���ص�����ΪCSW
  }
  else
  {
   return; //�����״̬�׶ε����ݷ�����ϣ��򷵻�
  }
 }
 
#ifdef DEBUG0
 if(TransportStage==STATUS_STAGE)
 {
  Prints("״̬�׶Ρ�\r\n");
 }
#endif

 //���Ҫ���͵ĳ��ȱȶ˵�2������Ҫ�࣬��ֶ��������
 if(Ep2DataLength>EP2_SIZE)
 {
  //���Ͷ˵�2��󳤶��ֽ�
  D12WriteEndpointBuffer(5,EP2_SIZE,pEp2SendData);
  //ָ���ƶ�EP2_SIZE�ֽ�
  pEp2SendData+=EP2_SIZE;
  Ep2DataLength-=EP2_SIZE;
  //�����READ(10)������������ݽ׶Σ�����Ҫ��ȡ��������
  if((CBW[15]==READ_10)&&(TransportStage==DATA_STAGE))
  {
   GetDiskData(); //��ȡ��������
  }
 }
 else
 {
  //����ȫ��������
  D12WriteEndpointBuffer(5,(uint8)Ep2DataLength,pEp2SendData);
  Ep2DataLength=0;  //���䳤��Ϊ0  
  //��������ݷ�����ϣ���������������Э���״̬�׶�
  if(TransportStage==DATA_STAGE)
  {
   TransportStage=STATUS_STAGE;
   Ep2DataLength=sizeof(CSW); //���ݳ���ΪCSW�Ĵ�С
   pEp2SendData=CSW; //���ص�����ΪCSW
  }
  else if(TransportStage==STATUS_STAGE) //�����״̬�׶���ϣ�����뵽����׶�
  {
   TransportStage=COMMAND_STAGE;  //���뵽����׶�
  }
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�����SCSI����ĺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע����Ȼ��SCSI�������ʵ��ʹ�õ���UFI���
********************************************************************/
void ProcScsiCommand(void)
{
 TransportStage=DATA_STAGE; //���뵽���ݽ׶�
 
 //CBW��ƫ����Ϊ15���ֶ�Ϊ���������
 switch(CBW[15])
 {
  case INQUIRY:  //INQUIRY����
  #ifdef DEBUG0
   Prints("��ѯ����������ݣ�\r\n");
  #endif
   pEp2SendData=DiskInf; //���ش�����Ϣ
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   SetCsw(Ep2DataLength-sizeof(DiskInf),0); //����ʣ���ֽ����Լ�״̬�ɹ�
   if(Ep2DataLength>sizeof(DiskInf)) //�����������ݱ�ʵ�ʵ�Ҫ��
   {
    Ep2DataLength=sizeof(DiskInf); //��ֻ����ʵ�ʵĳ���
   }
   Ep2SendData(); //��������
  break;
  
  case READ_FORMAT_CAPACITIES: //����ʽ������
  #ifdef DEBUG0
   Prints("����ʽ����������������ݣ�\r\n");
  #endif
   pEp2SendData=MaximumCapacity; //��������ʽ��������Ϣ
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   SetCsw(Ep2DataLength-sizeof(MaximumCapacity),0); //����ʣ���ֽ����Լ�״̬�ɹ�
   if(Ep2DataLength>sizeof(MaximumCapacity)) //�����������ݱ�ʵ�ʵ�Ҫ��
   {
    Ep2DataLength=sizeof(MaximumCapacity); //��ֻ����ʵ�ʵĳ���
   }
   Ep2SendData(); //��������
  break;
  
  case READ_CAPACITY: //����������
  #ifdef DEBUG0
   Prints("����������������ݣ�\r\n");
  #endif
   pEp2SendData=DiskCapacity; //���ش�������
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   SetCsw(Ep2DataLength-sizeof(DiskCapacity),0); //����ʣ���ֽ����Լ�״̬�ɹ�
   if(Ep2DataLength>sizeof(DiskCapacity)) //�����������ݱ�ʵ�ʵ�Ҫ��
   {
    Ep2DataLength=sizeof(DiskCapacity); //��ֻ����ʵ�ʵĳ���
   }
   Ep2SendData(); //��������
  break;
  
  case READ_10: //READ(10)����
  #ifdef DEBUG0
   Prints("READ(10)����������ݣ�\r\n");
  #endif
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   ByteAddr=GetLba()*512; //��ȡ�ֽڵ�ַ���ֽڵ�ַΪ�߼����ַ����ÿ���С
   SetCsw(0,0); //����ʣ���ֽ���Ϊ0��״̬�ɹ�
   GetDiskData(); //��ȡ��Ҫ���ص�����
   Ep2SendData(); //��������
  break;
  
  case WRITE_10: //WRITE(10)����
  #ifdef DEBUG0
   Prints("WRITE(10)���������ݣ�\r\n");
  #endif
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   SetCsw(0,0); //����ʣ���ֽ���Ϊ0��״̬�ɹ�
  break;
  
  case REQUEST_SENSE: //������ѯ��ǰһ������ִ��ʧ�ܵ�ԭ��
  #ifdef DEBUG0
   Prints("REQUEST SENSE�������SENSE���ݣ���Ч�����\r\n");
  #endif
   pEp2SendData=SenseData; //����̽������
   Ep2DataLength=GetDataTransferLength(); //��ȡ��Ҫ���صĳ���
   SetCsw(Ep2DataLength-sizeof(SenseData),0); //����ʣ���ֽ����Լ�״̬�ɹ�
   if(Ep2DataLength>sizeof(SenseData)) //�����������ݱ�ʵ�ʵ�Ҫ��
   {
    Ep2DataLength=sizeof(SenseData); //��ֻ����ʵ�ʵĳ���
   }
   Ep2SendData(); //��������
  break;
  
  case TEST_UNIT_READY: //���Դ����Ƿ�׼����
   Ep2DataLength=0; //���ó���Ϊ0���������ݽ�����CSW
   SetCsw(0,0); //����CSWΪ�ɹ�
   Ep2SendData(); //����CSW
  break;
  
  default: //��������ϣ�����ִ��ʧ��
   if(CBW[12]&0x80) Ep2DataLength=1; //���Ϊ������������㷵��1�ֽ�
   else Ep2DataLength=0; //����Ϊ������������ó���Ϊ0��ֱ�ӷ���CSW
   SetCsw(GetDataTransferLength()-Ep2DataLength,1); //����CSWΪʧ��
   Ep2SendData(); //����CSW
  break;
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�����������ݡ�
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void ProcScsiOutData(void)
{
 uint8 Len;
 //���˵�2����
 Len=D12ReadEndpointBuffer(4,EP2_SIZE,Ep2Buffer);
 Ep2DataLength-=Len;
 //����˵㻺����
 D12ClearBuffer();
 //����û�д洢�������ｫ��������0ģ�����ݴ���
 while(Len)
 {
  Ep2Buffer[Len]=0; //��������0
  Len--;
 }
 
 //���ݴ�����ϣ����뵽״̬�׶�
 if(Ep2DataLength==0)
 {
  //��ʱEp2DataLengthΪ0�����Ҵ������ݽ׶Σ����÷������ݺ���������CSW
  Ep2SendData();
 }
}
////////////////////////End of function//////////////////////////////
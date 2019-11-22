/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

SCSI.c file

作者：电脑圈圈
建立日期: 2008.08.15
修改日期: 2008.08.20
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/


#include "MyType.H"
#include "UsbCore.h"
#include "PDIUSBD12.h"
#include "SCSI.h"
#include "Uart.h"
#include "config.h"
#include "FAT.h"

//定义端点2最大包长度为64字节
#define EP2_SIZE 64

//处理端点2数据的缓冲区
idata uint8 Ep2Buffer[EP2_SIZE];

uint32 ByteAddr;  //字节地址

//INQUIRY命令需要返回的数据
//请对照书中INQUIRY命令响应数据格式
code uint8 DiskInf[36]=
{
 0x00, //磁盘设备
 0x00, //其中最高位D7为RMB。RMB=0，表示不可移除设备。如果RMB=1，则为可移除设备。
 0x00, //各种版本号0
 0x01, //数据响应格式
 0x1F, //附加数据长度，为31字节
 0x00, //保留
 0x00, //保留
 0x00, //保留
 0xB5,0xE7,0XC4,0xD4,0xC8,0xA6,0xC8,0xA6, //厂商标识，为字符串“电脑圈圈”
 
 //产品标识，为字符串“自己做的假U盘”
 0xD7,0xD4,0xBC,0xBA,0xD7,0xF6,0xB5,0xC4,0xBC,0xD9,0x55,0xC5,0xCC,0x00,0x00,0x00,
 0x31,0x2E,0x30,0x31 //产品版本号，为1.01
};

//READ_FORMAT_CAPACITIES命令需要返回的数据
//请对照书中READ_FORMAT_CAPACITIES命令响应数据格式
code uint8 MaximumCapacity[12]=
{
 0x00, 0x00, 0x00, //保留
 0x08,  //容量列表长度
 0x01, 0x00, 0x00, 0x00,  //块数(最大支持8GB)
 0x03, //描述符代码为3，表示最大支持的格式化容量
 0x00, 0x02, 0x00 //每块大小为512字节
};

//READ_CAPACITY命令需要返回的数据
code uint8 DiskCapacity[8]=
{
 0x00,0x03,0xFF,0xFF, //能够访问的最大逻辑块地址
 0x00,0x00,0x02,0x00  //块的长度
 //所以该磁盘的容量为
 //(0x3FFFF+1)*0x200 = 0x8000000 = 128*1024*1024 = 128MB.
};

//REQUEST SENSE命令需要返回的数据，这里固定为无效命令
//请参看书总数据结构的解释
code uint8 SenseData[18]=
{
 0x70, //错误代码，固定为0x70
 0x00, //保留
 0x05, //Sense Key为0x05，表示无效请求（ILLEGAL REQUEST）
 0x00, 0x00, 0x00, 0x00, //Information为0
 0x0A, //附加数据长度为10字节
 0x00, 0x00, 0x00, 0x00, //保留
 0x20, //Additional Sense Code(ASC)为0x20，表示无效命令操作码（INVALID COMMAND OPERATION CODE）
 0x00, //Additional Sense Code Qualifier(ASCQ)为0
 0x00, 0x00, 0x00, 0x00 //保留
};

uint8 * pEp2SendData;
uint32 Ep2DataLength;

/********************************************************************
函数功能：从CBW中获取传输数据的字节数。
入口参数：无。
返    回：需要传输的字节数。
备    注：无。
********************************************************************/
uint32 GetDataTransferLength(void)
{
 uint32 Len;
 
 //CBW[8]~CBW[11]为传输长度（小端结构）
 
 Len=CBW[11];
 Len=Len*256+CBW[10];
 Len=Len*256+CBW[9];
 Len=Len*256+CBW[8];
 
 return Len;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：从CBW中获取逻辑块地址LBA的字节数。
入口参数：无。
返    回：逻辑块地址LBA。
备    注：无。
********************************************************************/
uint32 GetLba(void)
{
 uint32 Lba;
 
 //读和写命令时，CBW[17]~CBW[20]为逻辑块地址（大端结构）
 
 Lba=CBW[17];
 Lba=Lba*256+CBW[18];
 Lba=Lba*256+CBW[19];
 Lba=Lba*256+CBW[20];
 
 return Lba;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：填充CSW。
入口参数：Residue：剩余字节数；Status：命令执行的状态。
返    回：无。
备    注：无。
********************************************************************/
void SetCsw(uint32 Residue, uint8 Status)
{
 //设置CSW的签名，其实可以不用每次都设置的，
 //开始初始化设置一次就行了，这里每次都设置
 CSW[0]='U';
 CSW[1]='S';
 CSW[2]='B';
 CSW[3]='S';
 
 //复制dCBWTag到CSW的dCSWTag中去
 CSW[4]=CBW[4];
 CSW[5]=CBW[5];
 CSW[6]=CBW[6];
 CSW[7]=CBW[7];
    
 //剩余字节数
 CSW[8]=Residue&0xFF;
 CSW[9]=(Residue>>8)&0xFF;
 CSW[10]=(Residue>>16)&0xFF;
 CSW[11]=(Residue>>24)&0xFF;
 
 //命令执行的状态，0表示成功，1表示失败。
 CSW[12]=Status;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：获取磁盘数据函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void GetDiskData(void)
{
 //判断该返回什么数据
 if(ByteAddr==0) pEp2SendData=Dbr; //返回DBR
 if(ByteAddr==512) pEp2SendData=Fat; //返回FAT
 if((ByteAddr>=576)&&(ByteAddr<16896)) pEp2SendData=Zeros;
 if(ByteAddr==16896) pEp2SendData=Fat; //返回FAT（备份FAT）
 if((ByteAddr>=16960)&&(ByteAddr<33280)) pEp2SendData=Zeros;
 if(ByteAddr==33280) pEp2SendData=RootDir; //返回根目录
 if((ByteAddr>=33344)&&(ByteAddr<49664)) pEp2SendData=Zeros;
 if(ByteAddr==49664) pEp2SendData=TestFileData; //返回文件数据
 if(ByteAddr>50175) pEp2SendData=Zeros;
 
 ByteAddr+=EP2_SIZE; //调整字节地址，每次发送最大包长度的数据
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：将数据通过端点2发送。
入口参数：无。
返    回：无。
备    注：当发送数据长度为0，并且处于数据阶段时，将自动发送CSW。
********************************************************************/
void Ep2SendData(void)
{
 if(Ep2DataLength==0) //如果需要发送的数据长度为0
 {
  if(TransportStage==DATA_STAGE) //并且处于数据阶段
  {
   //则直接进入状态阶段
   TransportStage=STATUS_STAGE;
   Ep2DataLength=sizeof(CSW); //数据长度为CSW的大小
   pEp2SendData=CSW; //返回的数据为CSW
  }
  else
  {
   return; //如果是状态阶段的数据发送完毕，则返回
  }
 }
 
#ifdef DEBUG0
 if(TransportStage==STATUS_STAGE)
 {
  Prints("状态阶段。\r\n");
 }
#endif

 //如果要发送的长度比端点2最大包长要多，则分多个包发送
 if(Ep2DataLength>EP2_SIZE)
 {
  //发送端点2最大长度字节
  D12WriteEndpointBuffer(5,EP2_SIZE,pEp2SendData);
  //指针移动EP2_SIZE字节
  pEp2SendData+=EP2_SIZE;
  Ep2DataLength-=EP2_SIZE;
  //如果是READ(10)命令，并且是数据阶段，则需要获取磁盘数据
  if((CBW[15]==READ_10)&&(TransportStage==DATA_STAGE))
  {
   GetDiskData(); //获取磁盘数据
  }
 }
 else
 {
  //可以全部发送完
  D12WriteEndpointBuffer(5,(uint8)Ep2DataLength,pEp2SendData);
  Ep2DataLength=0;  //传输长度为0  
  //如果是数据发送完毕，则进入仅批量传输协议的状态阶段
  if(TransportStage==DATA_STAGE)
  {
   TransportStage=STATUS_STAGE;
   Ep2DataLength=sizeof(CSW); //数据长度为CSW的大小
   pEp2SendData=CSW; //返回的数据为CSW
  }
  else if(TransportStage==STATUS_STAGE) //如果是状态阶段完毕，则进入到命令阶段
  {
   TransportStage=COMMAND_STAGE;  //进入到命令阶段
  }
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：处理SCSI命令的函数。
入口参数：无。
返    回：无。
备    注：虽然叫SCSI命令，但是实际使用的是UFI命令。
********************************************************************/
void ProcScsiCommand(void)
{
 TransportStage=DATA_STAGE; //进入到数据阶段
 
 //CBW中偏移量为15的字段为命令的类型
 switch(CBW[15])
 {
  case INQUIRY:  //INQUIRY命令
  #ifdef DEBUG0
   Prints("查询命令。返回数据：\r\n");
  #endif
   pEp2SendData=DiskInf; //返回磁盘信息
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   SetCsw(Ep2DataLength-sizeof(DiskInf),0); //设置剩余字节数以及状态成功
   if(Ep2DataLength>sizeof(DiskInf)) //如果请求的数据比实际的要长
   {
    Ep2DataLength=sizeof(DiskInf); //则只返回实际的长度
   }
   Ep2SendData(); //返回数据
  break;
  
  case READ_FORMAT_CAPACITIES: //读格式化容量
  #ifdef DEBUG0
   Prints("读格式化容量命令。返回数据：\r\n");
  #endif
   pEp2SendData=MaximumCapacity; //返回最大格式化容量信息
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   SetCsw(Ep2DataLength-sizeof(MaximumCapacity),0); //设置剩余字节数以及状态成功
   if(Ep2DataLength>sizeof(MaximumCapacity)) //如果请求的数据比实际的要长
   {
    Ep2DataLength=sizeof(MaximumCapacity); //则只返回实际的长度
   }
   Ep2SendData(); //返回数据
  break;
  
  case READ_CAPACITY: //读容量命令
  #ifdef DEBUG0
   Prints("读容量命令。返回数据：\r\n");
  #endif
   pEp2SendData=DiskCapacity; //返回磁盘容量
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   SetCsw(Ep2DataLength-sizeof(DiskCapacity),0); //设置剩余字节数以及状态成功
   if(Ep2DataLength>sizeof(DiskCapacity)) //如果请求的数据比实际的要长
   {
    Ep2DataLength=sizeof(DiskCapacity); //则只返回实际的长度
   }
   Ep2SendData(); //返回数据
  break;
  
  case READ_10: //READ(10)命令
  #ifdef DEBUG0
   Prints("READ(10)命令。返回数据：\r\n");
  #endif
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   ByteAddr=GetLba()*512; //获取字节地址，字节地址为逻辑块地址乘以每块大小
   SetCsw(0,0); //设置剩余字节数为0，状态成功
   GetDiskData(); //获取需要返回的数据
   Ep2SendData(); //返回数据
  break;
  
  case WRITE_10: //WRITE(10)命令
  #ifdef DEBUG0
   Prints("WRITE(10)命令。输出数据：\r\n");
  #endif
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   SetCsw(0,0); //设置剩余字节数为0，状态成功
  break;
  
  case REQUEST_SENSE: //该命令询问前一个命令执行失败的原因
  #ifdef DEBUG0
   Prints("REQUEST SENSE命令。返回SENSE数据（无效命令）：\r\n");
  #endif
   pEp2SendData=SenseData; //返回探测数据
   Ep2DataLength=GetDataTransferLength(); //获取需要返回的长度
   SetCsw(Ep2DataLength-sizeof(SenseData),0); //设置剩余字节数以及状态成功
   if(Ep2DataLength>sizeof(SenseData)) //如果请求的数据比实际的要长
   {
    Ep2DataLength=sizeof(SenseData); //则只返回实际的长度
   }
   Ep2SendData(); //返回数据
  break;
  
  case TEST_UNIT_READY: //测试磁盘是否准备好
   Ep2DataLength=0; //设置长度为0，发送数据将返回CSW
   SetCsw(0,0); //设置CSW为成功
   Ep2SendData(); //返回CSW
  break;
  
  default: //其它命令不认，返回执行失败
   if(CBW[12]&0x80) Ep2DataLength=1; //如果为输入请求，则随便返回1字节
   else Ep2DataLength=0; //否则为输出请求，则设置长度为0，直接返回CSW
   SetCsw(GetDataTransferLength()-Ep2DataLength,1); //设置CSW为失败
   Ep2SendData(); //返回CSW
  break;
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：处理输出数据。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void ProcScsiOutData(void)
{
 uint8 Len;
 //读端点2数据
 Len=D12ReadEndpointBuffer(4,EP2_SIZE,Ep2Buffer);
 Ep2DataLength-=Len;
 //清除端点缓冲区
 D12ClearBuffer();
 //由于没有存储器，这里将缓冲区清0模拟数据处理
 while(Len)
 {
  Ep2Buffer[Len]=0; //缓冲区清0
  Len--;
 }
 
 //数据传输完毕，进入到状态阶段
 if(Ep2DataLength==0)
 {
  //此时Ep2DataLength为0，并且处于数据阶段，调用发送数据函数将返回CSW
  Ep2SendData();
 }
}
////////////////////////End of function//////////////////////////////
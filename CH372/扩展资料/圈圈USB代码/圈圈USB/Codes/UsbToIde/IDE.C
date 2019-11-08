/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

IDE.c file

作者：电脑圈圈
建立日期: 2008.08.21
修改日期: 2008.08.21
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#include <at89X52.h>
#include "MyType.h"
#include "IDE.H"
#include "UsbCore.h"
#include "SCSI.h"

/********************************************************************
函数功能：设置IDE地址的函数。其实是将P1口数据通过573锁存输出。
入口参数：Address: 要锁存的数据。
返    回：无。
备    注：请参看原理图，P1口通过573锁存后所连接的引脚。
********************************************************************/
void IdeSetAddress(uint8 Address)
{
 uint8 i;
 IDE_DATA_L=0xFF; //设置为高电平
 IDE_DATA_H=0xFF;
 i=5;
 while(--i); //IO口上拉能力弱，延时等待电平稳定
 IDE_ADDRESS=Address; //设置地址（通过573锁存）
 IDE_ALE=1;  //将P1口数据锁存到573中
 IDE_ALE=0;
 //由于学习板上IO口未接上拉电阻，仅使用内部上拉电阻，
 //所以驱动能力较弱，需要驱动较长的IDE数据线，因而
 //高电平的建立时间较长，所以需要事先设置高电平状态
 IDE_DATA_L=0xFF;
 IDE_DATA_H=0xFF;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：IDE硬盘复位函数。
入口参数：无。
返    回：无。
备    注：调用此函数对硬盘复位，进入默认状态。
********************************************************************/
void IdeReset(void)
{
 IdeSetAddress(IDE_RESET);  //设置复位，即RESET脚置低
 DelayXms(200);             //延时等待
 IdeSetAddress(IDE_BUS_NOT_USE);  //释放复位脚（即RESET脚为高）
 DelayXms(200);             //延时等待
 while(!IDE_IORDY);         //等待IO口空闲
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：从IDE硬盘读取16位数据的函数。
入口参数：Address: 要访问的寄存器地址。
返    回：读取到的数据。
备    注：无。
********************************************************************/
uint16 IdeRead16(uint8 Address)
{
 uint16 Value;
 while(!IDE_IORDY);  //等待IO口空闲
 IdeSetAddress(Address); //设置要访问的寄存器地址
 IDE_DATA_L=0xFF;    //设置为高电平状态，准备读取数据
 IDE_DATA_H=0xFF;
 IdeClrDior();       //DIOR置低
 Value=(((uint16)(IDE_DATA_H))*256)+IDE_DATA_L; //读回16位数据
 IdeSetDior();       //DIOR置高
 IdeSetAddress(IDE_BUS_NOT_USE); //设置总线为高阻态
 return Value;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：从IDE硬盘读取8位数据的函数。
入口参数：Address: 要访问的寄存器地址。
返    回：读取到的数据。
备    注：无。
********************************************************************/
uint8 IdeRead8(uint8 Address)
{
 uint8 Value;
 while(!IDE_IORDY);  //等待IO空闲
 IdeSetAddress(Address); //设置要访问的寄存器地址
 IDE_DATA_L=0xFF;  //设置为高电平状态，准备读取数据
 IDE_DATA_H=0xFF;
 IdeClrDior();   //DIOR置低
 Value=IDE_DATA_L;  //读回8位数据
 IdeSetDior();   //DIOR置高
 IdeSetAddress(IDE_BUS_NOT_USE);  //设置总线为高阻态
 return Value;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：写8位数据的函数。
入口参数：Address: 需要访问的寄存器地址；Value: 要写入的数据。
返    回：无。
备    注：无。
********************************************************************/
void IdeWrite8(uint8 Address,uint8 Value)
{
 while(!IDE_IORDY);  //等待IO口空闲
 IdeSetAddress(Address);  //设置需要访问的寄存器地址
 IdeClrDiow();   //DIOW置低
 IDE_DATA_L=Value;  //发出数据
 IdeSetDiow();   //DIOW置高
 IdeSetAddress(IDE_BUS_NOT_USE);  //设置总线为高阻态
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：读取IDE硬盘状态寄存器的函数。
入口参数：无。
返    回：状寄存器的值。
备    注：无。
********************************************************************/
uint8 IdeReadStatus(void)
{
 return IdeRead8(IDE_STATUS);  //读IDE STATUS寄存器
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：等待IDE硬盘忙碌函数。
入口参数：无。
返    回：无。
备    注：当硬盘内部执行操作时，会进入忙碌状态，这时不能进行其它操作。
********************************************************************/
void IdeWaitBusy(void)
{
 while(IdeReadStatus()&0x80); //如果硬盘忙碌，则等待
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：IDE硬盘写命令的函数。
入口参数：Command: 要写入的命令。
返    回：无。
备    注：无。
********************************************************************/
void IdeWriteCommand(uint8 Command)
{
 IdeWrite8(IDE_COMMAND,Command); //往IDE COMMAND寄存器写入命令
 IdeWaitBusy();  //等待IDE硬盘内部操作完成
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：获取磁盘信息的函数。
入口参数：无。
返    回：无。
备    注：这里主要获取磁盘容量。当然你也可以修改代码查看其它需要的信息。
********************************************************************/
void IdeGetInformation(void)
{
 uint16 Value;
 uint16 i;
 //使用IDE_IDENTIFY_DRIVE命令获取磁盘信息
 IdeWriteCommand(IDE_Identify_Drive);
 for(i=0;i<57;i++) //前面57字信息这里未用
 {
  IdeRead16(IDE_DATA);
 }
 
 Value=IdeRead16(IDE_DATA); //当前容量低字(扇区数)
 DiskCapacity[3]=Value;
 DiskCapacity[2]=Value>>8;
 Value=IdeRead16(IDE_DATA); //当前容量高字(扇区数)
 DiskCapacity[1]=Value;
 DiskCapacity[0]=Value>>8;
 
 for(i=59;i<=255;i++) //读剩余字节
 {
  IdeRead16(IDE_DATA);
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：设置LBA寄存器的函数。
入口参数：LBA: 需要设置的LBA地址。
返    回：无。
备    注：无。
********************************************************************/
void IdeWriteLba(uint32 LBA)
{
 LBA&=0x0FFFFFFF; //注意LBA跟Drive/Head寄存器实际上是同一个
 LBA|=0xE0000000;
 IdeWrite8(IDE_LBA_BITS_24_27,(uint8)(LBA>>24));  //写对应的LBA寄存器
 IdeWrite8(IDE_LBA_BITS_16_23,(uint8)(LBA>>16));
 IdeWrite8(IDE_LBA_BITS_8_15, (uint8)(LBA>>8));
 IdeWrite8(IDE_LBA_BITS_0_7,  (uint8)(LBA));
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：设置操作扇区数的函数。
入口参数：Count: 需要操作的扇区数（0表示256个）。
返    回：无。
备    注：无。
********************************************************************/
void IdeWriteSectorCount(uint8 Count)
{
 IdeWrite8(IDE_SECTOR_COUNT,Count); //写IDE SECTOR COUNT寄存器
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：IDE硬盘初始化函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void IdeInit(void)
{
 IdeReset(); //复位
 IdeWaitBusy(); //等待忙
 IdeWrite8(IDE_DRIVE_HEAD,0xE0);  //设置为LBA模式
 IdeWaitBusy();  //等待忙
 IdeWrite8(IDE_DEVICE_CONTROL,0xFA);//没有中断模式
 IdeGetInformation(); //获取硬盘信息
}
////////////////////////End of function//////////////////////////////
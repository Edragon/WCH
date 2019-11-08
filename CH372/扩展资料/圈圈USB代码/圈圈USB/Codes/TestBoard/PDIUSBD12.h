/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途

        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

PDIUSBD12.H  file

作者：电脑圈圈
建立日期: 2008.06.27
修改日期: 2008.06.27
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/


#ifndef __PDIUSBD12_H__
#define __PDIUSBD12_H__

#include <at89x52.h>
#include "MyType.h"

//命令地址和数据地址
#define D12_COMMAND_ADD           1
#define D12_DATA_ADD              0

//PDIUSBD12芯片连接引脚
#define D12_DATA                  P0
#define D12_A0                    P3_5
#define D12_WR                    P3_6
#define D12_RD                    P3_7
#define D12_INT                   P3_2

//选择命令或数据地址
#define D12SetCommandAddr() D12_A0=D12_COMMAND_ADD
#define D12SetDataAddr()    D12_A0=D12_DATA_ADD
//WR控制
#define D12SetWr() D12_WR=1
#define D12ClrWr() D12_WR=0
//RD控制
#define D12SetRd() D12_RD=1
#define D12ClrRd() D12_RD=0
//获取中断状态
#define D12GetIntPin()  D12_INT
//读写数据
#define D12GetData() D12_DATA
#define D12SetData(Value) D12_DATA=(Value)

//将数据口设置为输入状态，51单片机端口写1就是为输入状态
#define D12SetPortIn() D12_DATA=0xFF

//将数据口设置为输出状态，由于51单片机是准双向IO口，所以不用切换，为空宏
#define D12SetPortOut()

//D12的读ID命令
#define Read_ID                  0xFD

//函数声明
void D12WriteCommand(uint8);
uint8 D12ReadByte(void);
uint16 D12ReadID(void);

#endif

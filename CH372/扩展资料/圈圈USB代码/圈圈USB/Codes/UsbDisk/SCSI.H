/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

SCSI.h file

作者：电脑圈圈
建立日期: 2008.08.15
修改日期: 2008.08.15
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#ifndef __SCSI_H__
#define __SCSI_H__

//以下为UFI命令
#define INQUIRY                0x12
#define READ_FORMAT_CAPACITIES 0x23
#define READ_CAPACITY          0x25
#define READ_10                0x28
#define WRITE_10               0x2A
#define REQUEST_SENSE          0x03
#define TEST_UNIT_READY        0x00

void ProcScsiCommand(void);
void ProcScsiOutData(void);
void Ep2SendData(void);

#endif

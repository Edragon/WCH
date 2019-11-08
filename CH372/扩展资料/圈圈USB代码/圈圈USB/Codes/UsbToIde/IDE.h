/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

IDE.h file

作者：电脑圈圈
建立日期: 2008.08.21
修改日期: 2008.08.21
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#ifndef __IDE_H__
#define __IDE_H__

#include <at89X52.h>
#include "MyType.h"

/*                       TABLE 6-1:  INTERFACE SIGNALS 
 +----------------------------------+                         +-----------+
 |         HOST  I/O                |                         | DRIVE I/O | 
 |         CONNECTOR                |                         | CONNECTOR | 
 |                                  |                         |           | 
 | HOST RESET                     1 | -----  RESET-  -------->| 1         | 
 |                                2 | -----  Ground  -------- | 2         | 
 | HOST DATA BUS BIT 7            3 |<-----  DD7  ----------->| 3         | 
 | HOST DATA BUS BIT 8            4 |<-----  DD8  ----------->| 4         | 
 | HOST DATA BUS BIT 6            5 |<-----  DD6  ----------->| 5         | 
 | HOST DATA BUS BIT 9            6 |<-----  DD9  ----------->| 6         | 
 | HOST DATA BUS BIT 5            7 |<-----  DD5  ----------->| 7         | 
 | HOST DATA BUS BIT 10           8 |<-----  DD10  ---------->| 8         | 
 | HOST DATA BUS BIT 4            9 |<-----  DD4  ----------->| 9         | 
 | HOST DATA BUS BIT 11          10 |<-----  DD11  ---------->| 10        | 
 | HOST DATA BUS BIT 3           11 |<-----  DD3  ----------->| 11        | 
 | HOST DATA BUS BIT 12          12 |<-----  DD12  ---------->| 12        | 
 | HOST DATA BUS BIT 2           13 |<-----  DD2  ----------->| 13        | 
 | HOST DATA BUS BIT 13          14 |<-----  DD13  ---------->| 14        | 
 | HOST DATA BUS BIT 1           15 |<-----  DD1  ----------->| 15        | 
 | HOST DATA BUS BIT 14          16 |<-----  DD14  ---------->| 16        | 
 | HOST DATA BUS BIT 0           17 |<-----  DD0  ----------->| 17        | 
 | HOST DATA BUS BIT 15          18 |<-----  DD15  ---------->| 18        | 
 |                               19 | -----  Ground  -------- | 19        | 
 |                               20 | -----  (keypin)  ------ | 20        | 
 | DMA REQUEST                   21 |<-----  DMARQ  --------- | 21        | 
 |                               22 | -----  Ground  -------- | 22        | 
 | HOST I/O WRITE                23 | -----  DIOW-  --------->| 23        | 
 |                               24 | -----  Ground  -------- | 24        | 
 | HOST I/O READ                 25 | -----  DIOR-  --------->| 25        | 
 |                               26 | -----  Ground  -------- | 26        | 
 | I/O CHANNEL READY             27 |<-----  IORDY  --------- | 27        | 
 | SPINDLE SYNC or CABLE SELECT  28 |*---- SPSYNC:CSEL  -----*| 28        | 
 | DMA ACKNOWLEDGE               29 | -----  DMACK-  -------->| 29        | 
 |                               30 | -----  Ground  -------- | 30        | 
 | HOST INTERRUPT REQUEST        31 |<-----  INTRQ  --------- | 31        | 
 | HOST 16 BIT I/O               32 |<-----  IOCS16-  ------- | 32        | 
 | HOST ADDRESS BUS BIT 1        33 | -----  DA1  ----------->| 33        | 
 | PASSED DIAGNOSTICS            34 |*-----  PDIAG-  --------*| 34        | 
 | HOST ADDRESS BUS BIT 0        35 | -----  DAO  ----------->| 35        | 
 | HOST ADDRESS BUS BIT 2        36 | -----  DA2  ----------->| 36        | 
 | HOST CHIP SELECT 0            37 | -----  CS1FX-  -------->| 37        | 
 | HOST CHIP SELECT 1            38 | -----  CS3FX-  -------->| 38        | 
 | DRIVE ACTIVE/DRIVE 1 PRESENT  39 |<-----  DASP-  ---------*| 39        | 
 |                               40 | -----  Ground  -------- | 40        | 
 +----------------------------------+                         +-----------+

                     * Drive Intercommunication Signals
*/

/*
DATA BUS 0-7   <====>  P1
DATA BUS 8-16  <====>  P2
IORDY  ---->P34
ALE   <-----P33
DIOW  <-----P30
DIOR  <-----P31
*/


#define IDE_DATA_L P1
#define IDE_DATA_H P2
#define IDE_IORDY  P3_4
#define IDE_DIOW   P3_0
#define IDE_DIOR   P3_1
#define IDE_ALE    P3_3

#define IdeClrDior() IDE_DIOR=0
#define IdeSetDior() IDE_DIOR=1
#define IdeClrDiow() IDE_DIOW=0
#define IdeSetDiow() IDE_DIOW=1

/*
Logic conventions are:   A = signal asserted (0)
                         N = signal negated  (1)
                         x = does not matter which it is

            TABLE 7-1:  I/O PORT FUNCTIONS/SELECTION ADDRESSES 
 +-------------------------------+-----------------------------------------+
 |         Addresses             |                 Functions               |
 |CS1FX-|CS3FX-| DA2 | DA1 | DA0 |    READ (DIOR-)     |   WRITE (DIOW-)   |
 +------+------+-----+-----+-----+---------------------+-------------------+
 |                                        Control Block Registers          |
 +------+------+-----+-----+-----+---------------------+-------------------+
 |  N   |  N   |  x  |  x  |  x  | Data Bus High Imped | Not used          |
 |  N   |  A   |  0  |  x  |  X  | Data Bus High Imped | Not used          |
 |  N   |  A   |  1  |  0  |  x  | Data Bus High Imped | Not used          |
 |  N   |  A   |  1  |  1  |  0  | Alternate Status    | Device Control    | 
 |  N   |  A   |  1  |  1  |  1  | Drive Address       | Not used          |
 +------+------+-----+-----+-----+---------------------+-------------------+
 |                                        Command Block Registers          |
 +------+------+-----+-----+-----+---------------------+-------------------+
 |  A   |  N   |  0  |  0  |  0  | Data                | Data              |
 |  A   |  N   |  0  |  0  |  1  | Error Register      | Features          |
 |  A   |  N   |  0  |  1  |  0  | Sector Count        | Sector Count      |
 |  A   |  N   |  0  |  1  |  1  | Sector Number       | Sector Number     | 
 |  A   |  N   |  0  |  1  |  1  | * LBA Bits  0- 7    | * LBA Bits  0- 7  |
 |  A   |  N   |  1  |  0  |  0  | Cylinder Low        | Cylinder Low      |
 |  A   |  N   |  1  |  0  |  0  | * LBA Bits  8-15    | * LBA Bits  8-15  |
 |  A   |  N   |  1  |  0  |  1  | Cylinder High       | Cylinder High     | 
 |  A   |  N   |  1  |  0  |  1  | * LBA Bits 16-23    | * LBA Bits 16-23  |
 |  A   |  N   |  1  |  1  |  0  | Drive/Head          | Drive/Head        |
 |  A   |  N   |  1  |  1  |  0  | * LBA Bits 24-27    | * LBA Bits 24-27  |
 |  A   |  N   |  1  |  1  |  1  | Status              | Command           |
 |  A   |  A   |  x  |  x  |  x  | Invalid Address     | Invalid Address   |
 +------+------+-----+-----+-----+---------------------+-------------------+
                      * Mapping of registers in LBA Mode
*/

/*
DA0   ----->   P10
DA1   ----->   P11
DA3   ----->   P12
CS1FX ----->   P13
CS3FX ----->   P14
CSEL  ----->   P15
RESET ----->   P17
*/

#define IDE_ADDRESS                 P1

#define IDE_BUS_NOT_USE             0x98
#define IDE_ALTERNATE_STATUS        0x8E
#define IDE_DEVICE_CONTROL          0x8E
#define IDE_DRIVER_ADDRESS          0x8F

#define IDE_DATA                    0x90
#define IDE_ERROR_REGISTER          0x91
#define IDE_FEATURES                0x91
#define IDE_SECTOR_COUNT            0x92
#define IDE_SECTOR_NUMBER           0x93
#define IDE_LBA_BITS_0_7            0x93
#define IDE_CYLINDER_LOW            0x94
#define IDE_LBA_BITS_8_15           0x94
#define IDE_CYLINDER_HIGH           0x95
#define IDE_LBA_BITS_16_23          0x95
#define IDE_DRIVE_HEAD              0x96
#define IDE_LBA_BITS_24_27          0x96
#define IDE_STATUS                  0x97
#define IDE_COMMAND                 0x97

#define IDE_RESET                   0x18

/*
 Alternate Status Register
         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     |  BSY  | DRDY  |  DWF  |  DSC  |  DRQ  | CORR  |  IDX  |  ERR  | 
     +-------+-------+-------+-------+-------+-------+-------+-------+
*/

/*
7.2.2  Command Register 

This register contains the command code being sent to the drive. Command 
execution begins immediately after this register is written. The executable 
commands, the command codes, and the necessary parameters for each command are 
listed in Table 9-1.
*/

/*
7.2.3  Cylinder High Register

This register contains the high order bits of the starting cylinder address 
for any disk access. At the end of the command, this register is updated to 
reflect the current cylinder number. The most significant bits of the cylinder 
address shall be loaded into the cylinder high Register. 

In LBA Mode this register contains Bits 16-23. At the end of the command, this 
register is updated to reflect the current LBA Bits 16-23.

  NOTE: Prior to the introduction of this standard, only the lower 2 bits of 
        this register were valid, limiting cylinder address to 10 bits i.e. 
        1,024 cylinders.
*/

/*
7.2.4  Cylinder Low Register

This register contains the low order 8 bits of the starting cylinder address 
for any disk access. At the end of the command, this register is updated to 
reflect the current cylinder number. 

In LBA Mode this register contains Bits 8-15. At the end of the command, this 
register is updated to reflect the current LBA Bits 8-15.

7.2.5  Data Register

This 16-bit register is used to transfer data blocks between the device data 
buffer and the host. It is also the register through which sector information 
is transferred on a Format Track command. Data transfers may be either PIO or 
DMA. 
*/

/*
7.2.6  Device Control Register

The bits in this register are as follows: 

         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     |   x   |   x   |   x   |   x   |   1   | SRST  |  nIEN |   0   | 
     +-------+-------+-------+-------+-------+-------+-------+-------+

 - SRST is the host software reset bit. The drive is held reset when this bit 
   is set. If two disk drives are daisy chained on the interface, this bit 
   resets both simultaneously. Drive 1 is not required to execute the DASP- 
   handshake procedure.
 - nIEN is the enable bit for the drive interrupt to the host. When nIEN=0, 
   and the drive is selected, INTRQ shall be enabled through a tri-state 
   buffer. When nIEN=1, or the drive is not selected, the INTRQ signal shall 
   be in a high impedance state. 

*/


/*
7.2.7  Drive Address Register

This register contains the inverted drive select and head select addresses of 
the currently selected drive. The bits in this register are as follows: 

         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     |  HiZ  |  nWTG |  nHS3 |  nHS2 |  nHS1 |  nHS0 |  nDS1 |  nDS0 | 
     +-------+-------+-------+-------+-------+-------+-------+-------+

 - HiZ shall always be in a high impedance state. 
 - nWTG is the Write Gate bit. When writing to the disk drive is in progress, 
   nWTG=0.
 - nHS3 through nHS0 are the one's complement of the binary coded address of 
   the currently selected head. For example, if nHS3 through nHS0 are 1100b, 
   respectively, head 3 is selected. nHS3 is the most significant bit. 
 - nDS1 is the drive select bit for drive 1. When drive 1 is selected and 
   active, nDS1=0. 
 - nDS0 is the drive select bit for drive 0. When drive 0 is selected and 
   active, nDS0=0.

  NOTE: Care should be used when interpreting these bits, as they do not 
        always represent the expected status of drive operations at the 
        instant the status was put into this register. This is because of the 
        use of cacheing, translate mode and the Drive 0/Drive 1 concept with 
        each drive having its own embedded controller. 

*/


/*
7.2.8  Drive/Head Register

This register contains the drive and head numbers. The contents of this 
register define the number of heads minus 1, when executing an Initialize 
Drive Parameters command. 

         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     |   1   |   L   |   1   |  DRV  |  HS3  |  HS2  |  HS1  |  HS0  | 
     +-------+-------+-------+-------+-------+-------+-------+-------+

 - L is the binary encoded address mode select. When L=0, addressing is by CHS 
   mode. When L=1, addressing is by LBA mode. 
 - DRV is the binary encoded drive select number. When DRV=0, Drive 0 is 
   selected. When DRV=1, Drive 1 is selected.
 - If L=0, HS3 through HS0 contain the binary coded address of the head to be 
   selected e.g. if HS3 through HS0 are 0011b, respectively, head 3 will be 
   selected. HS3 is the most significant bit. At command completion, these 
   bits are updated to reflect the currently selected head. 
   If L=1, HS3 through HS0 contain bits 24-27 of the LBA. At command 
   completion, these bits are updated to reflect the current LBA bits 24-27. 
*/


/*
7.2.9  Error Register 

This register contains status from the last command executed by the drive or a 
Diagnostic Code. 

At the completion of any command except Execute Drive Diagnostic, the contents 
of this register are valid when ERR=1 in the Status Register. 

Following a power on, a reset, or completion of an Execute Drive Diagnostic 
command, this register contains a Diagnostic Code (see Table 9-2). 

         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     | BBK   |  UNC  |  MC   |  IDNF |  MCR  | ABRT  | TK0NF |  AMNF |
     +-------+-------+-------+-------+-------+-------+-------+-------+

 - BBK (Bad Block Detected) indicates a bad block mark was detected in the 
   requested sector's ID field. 
 - UNC (Uncorrectable Data Error) indicates an uncorrectable data error has 
   been encountered. 
 - MC (Media Changed) indicates that the removable media has been changed i.e. 
   there has been a change in the ability to access the media.
 - IDNF (ID Not Found) indicates the requested sector's ID field could not be 
   found. 
 - ABRT (Aborted Command) indicates the requested command has been aborted due 
   to a drive status error (Not Ready, Write Fault, etc.) or because the 
   command code is invalid. 
 - MCR (Media Change Requested) indicates that the release latch on a 
   removable media drive has been pressed. This means that the user wishes to 
   remove the media and requires an action of some kind e.g. have software 
   issue a Media Eject or Door Unlock command. 
 - TK0NF (Track 0 Not Found) indicates track 0 has not been found during a 
   Recalibrate command. 
 - AMNF (Address Mark Not Found) indicates the data address mark has not been 
   found after finding the correct ID field. 
*/


/*
7.2.10  Features Register

This register is command specific and may be used to enable and disable 
features of the interface e.g. by the Set Features Command to enable and 
disable cacheing. 

This register may be ignored by some drives. 

Some hosts, based on definitions prior to the completion of this standard, set 
values in this register to designate a recommended Write Precompensation 
Cylinder value. 
*/


/*
7.2.11  Sector Count Register

This register contains the number of sectors of data requested to be 
transferred on a read or write operation between the host and the drive. If 
the value in this register is zero, a count of 256 sectors is specified. 

If this register is zero at command completion, the command was successful. If 
not successfully completed, the register contains the number of sectors which 
need to be transferred in order to complete the request.

The contents of this register may be defined otherwise on some commands e.g. 
Initialize Drive Parameters, Format Track or Write Same commands. 
*/


/*
7.2.12  Sector Number Register

This register contains the starting sector number for any disk data access for 
the subsequent command. The sector number may be from 1 to the maximum number 
of sectors per track. 

In LBA Mode this register contains Bits 0-7. At the end of the command, this 
register is updated to reflect the current LBA Bits 0-7.

See the command descriptions for contents of the register at command 
completion (whether successful or unsuccessful).
*/


/*
7.2.13  Status Register

This register contains the drive status. The contents of this register are 
updated at the completion of each command. When BSY is cleared, the other bits 
in this register shall be valid within 400 nsec. If BSY=1, no other bits in 
this register are valid. If the host reads this register when an interrupt is 
pending, it is considered to be the interrupt acknowledge. Any pending 
interrupt is cleared whenever this register is read. 

  NOTE: If Drive 1 is not detected as being present, Drive 0 clears the Drive 
        1 Status Register to 00h (indicating that the drive is Not Ready).

         7       6       5       4       3       2       1       0 
     +-------+-------+-------+-------+-------+-------+-------+-------+
     |  BSY  | DRDY  |  DWF  |  DSC  |  DRQ  | CORR  |  IDX  |  ERR  | 
     +-------+-------+-------+-------+-------+-------+-------+-------+

  NOTE: Prior to the definition of this standard, DRDY and DSC were unlatched 
        real time signals. 

 - BSY (Busy) is set whenever the drive has access to the Command Block 
   Registers. The host should not access the Command Block Register when 
   BSY=1. When BSY=1, a read of any Command Block Register shall return the 
   contents of the Status Register. This bit is set by the drive (which may be 
   able to respond at times when the media cannot be accessed) under the 
   following circumstances: 
   a) within 400 nsec after the negation of RESET- or after SRST has been set 
      in the Device Control Register. Following acceptance of a reset it is 
      recommended that BSY be set for no longer than 30 seconds by Drive 1 and 
      no longer than 31 seconds by Drive 0.
   b) within 400 nsec of a host write of the Command Register with a Read, 
      Read Long, Read Buffer, Seek, Recalibrate, Initialize Drive Parameters, 
      Read Verify, Identify Drive, or Execute Drive Diagnostic command. 
   c) within 5 usecs following transfer of 512 bytes of data during execution 
      of a Write, Format Track, or Write Buffer command, or 512 bytes of data 
      and the appropriate number of ECC bytes during the execution of a Write 
      Long command. 
 - DRDY (Drive Ready) indicates that the drive is capable of responding to a 
   command. When there is an error, this bit is not changed until the Status 
   Register is read by the host, at which time the bit again indicates the 
   current readiness of the drive. This bit shall be cleared at power on and 
   remain cleared until the drive is ready to accept a command. 
 - DWF (Drive Write Fault) indicates the current write fault status. When an 
   error occurs, this bit shall not be changed until the Status Register is 
   read by the host, at which time the bit again indicates the current write 
   fault status. 
 - DSC (Drive Seek Complete) indicates that the drive heads are settled over a 
   track. When an error occurs, this bit shall not be changed until the Status 
   Register is read by the host, at which time the bit again indicates the 
   current Seek Complete status. 
 - DRQ (Data Request) indicates that the drive is ready to transfer a word or 
   byte of data between the host and the drive.
 - CORR (Corrected Data) indicates that a correctable data error was 
   encountered and the data has been corrected. This condition does not 
   terminate a data transfer. 
 - IDX (Index) is set once per disk revolution. 
 - ERR (Error) indicates that an error occurred during execution of the 
   previous command. The bits in the Error Register have additional 
   information regarding the cause of the error. 
*/


/*
            TABLE 9-1:  COMMAND CODES AND PARAMETERS - Part 1 of 2
                                               +-------+-------------------+
  +-----+                                      |Command|  Parameters Used  |
  |Class|                                      | Code  |FR  SC  SN  CY  DH |
  +-----+----------------------------------+---+-------+---+---+---+---+---+
  |  1  | Acknowledge Media Chge           | O |  DBh  |   |   |   |   | D |
  |  1  | Boot - Post-Boot                 | O |  DCh  |   |   |   |   | D |
  |  1  | Boot - Pre-Boot                  | O |  DDh  |   |   |   |   | D |
  |  1  | Check Power Mode                 | O |98h E5h|   | y |   |   | D |
  |  1  | Door Lock                        | O |  DEh  |   |   |   |   | D |
  |  1  | Door Unlock                      | O |  DFh  |   |   |   |   | D |
  |  1  | Execute Drive Diagnostic         | M |  90h  |   |   |   |   | D*|
  |  2  | Format Track                     | M |  50h  | * | y |   | y | y |
  |  1  | Identify Drive                   | O |  ECh  |   |   |   |   | D |
  |  1  | Idle                             | O |97h E3h|   | y |   |   | D |
  |  1  | Idle Immediate                   | O |95h E1h|   |   |   |   | D |
  |  1  | Initialize Drive Parameters      | M |  91h  |   | y |   |   | y |
  |  1  | NOP                              | O |  00h  |   |   |   |   | y |
  |  1  | Read Buffer                      | O |  E4h  |   |   |   |   | D |
  |  1  | Read DMA (w/retry)               | O |  C8h  |   | y | y | y | y |
  |  1  | Read DMA (w/o retry)             | O |  C9h  |   | y | y | y | y |
  |  1  | Read Long (w/retry)              | M |  22   |   | y | y | y | y |
  |  1  | Read Long (w/o retry)            | M |  23   |   | y | y | y | y |
  |  1  | Read Multiple                    | O |  C4h  |   | y | y | y | y |
  |  1  | Read Sector(s) (w/retry)         | M |  20   |   | y | y | y | y |
  |  1  | Read Sector(s) (w/o retry)       | M |  21   |   | y | y | y | y |
  |  1  | Read Verify Sector(s) (w/retry)  | M |  40   |   | y | y | y | y |
  |  1  | Read Verify Sector(s) (w/o retry)| M |  41   |   | y | y | y | y |
  |  1  | Recalibrate                      | M |  1xh  |   |   |   |   | D |
  +-----+----------------------------------+---+-------+---+---+---+---+---+
            TABLE 9-1:  COMMAND CODES AND PARAMETERS - Part 2 of 2
                                               +-------+-------------------+
  +-----+                                      |Command|  Parameters Used  |
  |Class|                                      | Code  |FR  SC  SN  CY  DH |
  +-----+----------------------------------+---+-------+---+---+---+---+---+
  |  1  | Seek                             | M |  7xh  |   |   | y | y | y |
  |  1  | Set Features                     | O |  EFh  | y |   |   |   | D |
  |  1  | Set Multiple Mode                | O |  C6h  |   | y |   |   | D |
  |  1  | Sleep                            | O |99h E6h|   |   |   |   | D |
  |  1  | Standby                          | O |96h E2h|   | y |   |   | D |
  |  1  | Standby Immediate                | O |94h E0h|   |   |   |   | D |
  |  2  | Write Buffer                     | O |  E8h  |   |   |   |   | D |
  |  3  | Write DMA (w/retry)              | O |  CAh  |   | y | y | y | y |
  |  3  | Write DMA (w/o retry)            | O |  CBh  |   | y | y | y | y |
  |  2  | Write Long (w/retry)             | M |  32   | * | y | y | y | y |
  |  2  | Write Long (w/o retry)           | M |  33   | * | y | y | y | y |
  |  3  | Write Multiple                   | O |  C5h  | * | y | y | y | y |
  |  3  | Write Same                       | O |  E9h  | y | y | y | y | y |
  |  2  | Write Sector(s) (w/retry)        | M |  30   | * | y | y | y | y |
  |  2  | Write Sector(s) (w/o retry)      | M |  31   | * | y | y | y | y |
  |  3  | Write Verify                     | O |  3Ch  | * | y | y | y | y |
  |     | Vendor Unique                    | V |  9Ah  |   |   |   |   |   |
  |     | Vendor Unique                    | V | C0-C3h|   |   |   |   |   |
  |     | Vendor Unique                    | V |  8xh  |   |   |   |   |   |
  |     | Vendor Unique                    | V |F0h-FFh|   |   |   |   |   |
  |     | Reserved: All remaining codes    |   |       |   |   |   |   |   |
  +-----+----------------------------------+---+-------+---+---+---+---+---+
  |     | CY = Cylinder Registers         SC = Sector Count Register       |
  |     | DH = Drive/Head Register        SN = Sector Number Register      |
  |     | FR = Features Register (see command descriptions for use)        |
  |     | y - the register contains a valid parameter for this command.    |
  |     |     For the Drive/Head Register, y means both the drive and      |
  |     |     head parameters are used.                                    |
  |     | D - only the drive parameter is valid and not the head parameter.|
  |     | D* - Addressed to Drive 0 but both drives execute it.            |
  |     | * - Maintained for compatibility (see 7.2.9)                     |
  +-----+------------------------------------------------------------------+
*/
#define IDE_Acknowledge_Media_Change    0xDB
#define IDE_Boot_Post_Boot              0xDC
#define IDE_Boot_Pre_Boot               0xDD
#define IDE_Check_Power_Mode            0x98E5
#define IDE_Door_Lock                   0xDE
#define IDE_Door_Unlock                 0xDF
#define IDE_Execute_Drive_Diagnostic    0x90
#define IDE_Format_Track                0x50
#define IDE_Identify_Drive              0xEC
#define IDE_Idle                        0x97E3
#define IDE_Idle_Immediate              0x95E1
#define IDE_Initialize_Drive_parameters 0x91
#define IDE_NOP                         0x00
#define IDE_Read_Buffer                 0xE4
#define IDE_Read_DMA                    0xC8
#define IDE_Read_DMA_o                  0xC9
#define IDE_Read_Long                   0x22
#define IDE_Read_Long_o                 0x23
#define IDE_Read_Multiple               0xC4
#define IDE_Read_Sectors                0x20
#define IDE_Read_Sectors_o              0x21
#define IDE_Read_Verify_Sectors         0x40
#define IDE_Read_Verify_Sectors_o       0x41
#define IDE_Recalibrate                 0x10
#define IDE_Seek                        0x70
#define IDE_Set_Features                0xEF
#define IDE_Set_Multiple_Mode           0x96
#define IDE_Sleep                       0x99E6
#define IDE_Standby                     0x96E2
#define IDE_Standby_Immediate           0x94E0
#define IDE_Write_Buffer                0xE8
#define IDE_Write_DMA                   0xCA
#define IDE_Write_DMA_o                 0xCB
#define IDE_Write_Long                  0x32
#define IDE_Write_Long_o                0x33
#define IDE_Write_Multiple              0xC5
#define IDE_Write_Same                  0xE9
#define IDE_Write_Sectors               0x30
#define IDE_Write_Sectors_o             0x31
#define IDE_Write_Verify                0x3C

void IdeSetAddress(uint8 Address);
void IdeReset(void);
uint16 IdeRead16(uint8 Address);
uint8 IdeRead8(uint8 Address);
void IdeWrite8(uint8 Address, uint8 Value);
uint8 IdeReadStatus(void);
void IdeWaitBusy(void);
void IdeWriteCommand(uint8);
void IdeGetInfromation(void);
void IdeWriteLba(uint32 LBA);
void IdeWriteSectorCount(uint8 Count);
void IdeInit(void);

#endif


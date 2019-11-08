/********************************** (C) COPYRIGHT *******************************
* File Name          : HID.H
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : USB主机操作HID类键盘、鼠标相关头文件
*******************************************************************************/



/******************************************************************************/
/* 头文件包含 */
#include "CH563BAS.H"

#ifndef __HID_H__
#define __HID_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* HID类设备相关宏定义 */
#define HID_TYPE_UNKNOW					0x00								   /* HID类未知设备 */
#define HID_TYPE_KEYBOARD				0x01								   /* HID类键盘设备 */
#define HID_TYPE_MOUSE					0x02								   /* HID鼠标设备 */

/******************************************************************************/
/* HID类设备信息结构体定义 */
typedef struct _MIDIInfo 
{ 
	ATTACH_DEVICE_INFO  Device;													
	qHD_Structure *qHD_BLK_IN; 													/* 批量上传端点EHCI相关结构体指针定义 */
	qHD_Structure *qHD_BLK_OUT; 												/* 批量下传端点EHCI相关结构体指针定义 */
	UINT8  AudioType;															/* Audio类设备的类型：0:None,1:Audio control, 2:Audio streaming, 3:MIDI streaming */
	UINT8  IntfacNum;															/* MIDI设备的接口号 */   
	UINT8  BulkInEp;															/* MIDI设备的IN端点地址 */
	UINT8  BulkOutEp;															/* MIDI设备的OUT端点地址 */
	UINT16 BulkInEpSize;  	    												/* MIDI设备的IN端点的最大包大小 */	
	UINT16 BulkOutEpSize;  														/* MIDI设备的OUT端点的最大包大小 */	
}MIDI_INFO;																		/* HID类信息结构体,存储当前HID类必要参数 */

/******************************************************************************/
/* 变量外扩 */	
extern Periodic_Frame_List_Structure  *pHID_FramList;
extern UINT32 gEHCI_Preiodic_Frame_List_BaseAddr;  								/* EHCI周期列表数据结构基地址 */

/******************************************************************************/
/* 函数外扩 */
extern UINT8 MIDI_CofDescrAnalyse( MIDI_INFO *midiinfo, UINT8 *pbuf );				/* 分析HID类配置描述符 */

#ifdef __cplusplus
}
#endif

#endif

/********************************** (C) COPYRIGHT *******************************
* File Name          : UDISK_FAT.H
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : USB�������洢�豸�����ļ�ϵͳ�����ͷ�ļ�
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include "CH563BAS.H"

#ifndef __UDISK_FAT_H__
#define __UDISK_FAT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* �������� */
extern UINT8  gDiskMaxLun;				    									/* ��������߼���Ԫ�� */
extern UINT8  gDiskCurLun;	    												/* ���̵�ǰ�����߼���Ԫ�� */
extern DISK_INFO    	UDisk;													/* U����ؽṹ�嶨�� */	

/*******************************************************************************/
/* �������� */
/* ����ΪEHCI�ײ���ط�װ���� */
/*******************************************************************************/
extern UINT8 USBHOST_BusReset( void );
extern UINT8 USBHOST_EP0_Init( void );
extern UINT8 USBHOST_CtrlTransfer( UINT8 *DatBuf, UINT16 *RetLen );
extern UINT8 USBHOST_GetDeviceDescr( UINT8 *buf );
extern UINT8 USBHOST_GetConfigDescr( UINT8 *buf );
extern UINT8 USBHOST_SetAddress( UINT8 addr );
extern UINT8 USBHOST_SetConfig( UINT8 cfg );
extern UINT8 USBHOST_ClearEndpStall( UINT8 endp );
extern void USBHOST_SetTargetAddr( UINT8 addr );
extern UINT8 USBHOST_Detect_Connect( void );
extern void USBHOST_Parameter_Init( void );
extern UINT8 USBHOST_Enumerate( UINT8 *pbuf );

/******************************************************************************/
/* ����ΪU�̵ײ���صڷ�װ���� */
extern UINT8 USBHOST_MS_CofDescrAnalyse( UINT8 *pbuf );
extern UINT8 MS_GetMaxLun( void );
extern UINT8 MS_ResetErrorBOC( void );
extern UINT8 MS_ScsiCmd_Process( UINT8 *DataBuf );
extern UINT8 MS_Init( UINT8 *pbuf );
extern UINT8 MS_RequestSense( UINT8 *pbuf );
extern UINT8 MS_DiskInquiry( UINT8 *pbuf );
extern UINT8 MS_DiskCapacity( UINT8 *pbuf );
extern UINT8 MS_DiskTestReady( UINT8 *pbuf );
extern UINT8 MS_ReadSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf );
extern UINT8 MS_WriteSector( UINT32 StartLba, UINT8 SectCount, PUINT8 DataBuf );

#ifdef __cplusplus
}
#endif

#endif
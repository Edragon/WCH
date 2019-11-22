/********************************** (C) COPYRIGHT *******************************
* File Name          : HID.H
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : USB��������HID����̡�������ͷ�ļ�
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include "CH563BAS.H"

#ifndef __HID_H__
#define __HID_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* HID���豸��غ궨�� */
#define HID_TYPE_UNKNOW					0x00								   /* HID��δ֪�豸 */
#define HID_TYPE_KEYBOARD				0x01								   /* HID������豸 */
#define HID_TYPE_MOUSE					0x02								   /* HID����豸 */

/******************************************************************************/
/* HID���豸��Ϣ�ṹ�嶨�� */
typedef struct _HidInfo 
{ 
	ATTACH_DEVICE_INFO  Device;													
	qHD_Structure *qHD_INT_In1; 												/* �ж��ϴ��˵�1 EHCI��ؽṹ��ָ�붨�� */
	qHD_Structure *qHD_INT_In2; 												/* �ж��ϴ��˵�2 EHCI��ؽṹ��ָ�붨�� */
	qTD_Structure *IntInEp1qTD;
	qTD_Structure *IntInEp2qTD;
	UINT8  HidType;																/* HID���豸�����ͣ�0:None,1:Keyboard,2:Mouse */
	UINT8  IntfacNum;															/* HID���豸�Ľӿں� */   
	UINT8  IntInEp1;															/* HID���豸��IN�˵�1��ַ */
	UINT8  IntInEp2;															/* HID���豸��IN�˵�2��ַ */
	UINT16 IntInEp1Size;  	    												/* HID���豸��IN�˵�1��������С */	
	UINT16 IntInEp2Size;  														/* HID���豸��IN�˵�2��������С */	
	UINT16 ReportDes_Len1;														/* HID�౨���������ܳ���1 */
	UINT16 ReportDes_Len2;														/* HID�౨���������ܳ���2 */
}HID_INFO;																		/* HID����Ϣ�ṹ��,�洢��ǰHID���Ҫ���� */

/******************************************************************************/
/* �������� */	
extern Periodic_Frame_List_Structure  *pHID_FramList;
extern UINT32 gEHCI_Preiodic_Frame_List_BaseAddr;  								/* EHCI�����б����ݽṹ����ַ */

/******************************************************************************/
/* �������� */
extern UINT8 HID_CofDescrAnalyse( HID_INFO *Hidinfo, UINT8 *pbuf );				/* ����HID������������ */
extern UINT8 HID_SetIdle( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf );
extern UINT8 HID_SetReport( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf );
extern UINT8 HID_GetReportDescr( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf, UINT16 len );
extern UINT8 HID_ClassRequest_Deal( HID_INFO *Hidinfo, UINT8 *pbuf );
extern UINT8 HID_Issue_GetData( HID_INFO *Hidinfo, qHD_Structure *spTempqHD, UINT8 *pbuf, UINT16 len );
extern UINT8 HID_Check_Data( HID_INFO *Hidinfo, qHD_Structure *spTempqHD, UINT16 *len );
extern UINT8 HID_Stop_GetData( HID_INFO *Hidinfo, qHD_Structure *spTempqHD );

#ifdef __cplusplus
}
#endif

#endif
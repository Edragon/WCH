/********************************** (C) COPYRIGHT *******************************
* File Name          : UDISK_FAT.C
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : USB�������洢�豸�����ļ�ϵͳ������ļ�
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CH563BAS.H"
#include "EHCI_HCD.H"                                                           /* �����������Ͳ�������ͷ�ļ� */
#include "CH563SFR.H"                                                           /* CH563�Ĵ����������ͷ�ļ� */
#include "CH563USBSFR.H"                                                        /* оƬUSB�Ĵ������ͷ�ļ� */    
#include "CH563USB.H"                                                           /* USB���ö������ͷ�ļ� */    
#include "Udisk.H"                                                              /* Udisk�������ͷ�ļ� */
#include "Udisk_FAT.H"                                                          /* Udisk��װ�������ͷ�ļ� */

/******************************************************************************/
/* ������������ */
UINT8  gDiskMaxLun;                                                             /* ��������߼���Ԫ�� */
UINT8  gDiskCurLun;                                                             /* ���̵�ǰ�����߼���Ԫ�� */

/******************************************************************************/ 
/******************************************************************************/
/* ����ΪEHCI�ײ���ط�װ���� */
/******************************************************************************/
/******************************************************************************/ 

/*******************************************************************************
* Function Name  : USBHOST_BusReset
* Description    : USB�����������߸�λ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 USBHOST_BusReset( void )
{
    return( USBHOST_EHCI_BusReset( &UDisk.Device ) );
}                             

/*******************************************************************************
* Function Name  : USBHOST_EP0_Init
* Description    : USB�������ƶ˵�0��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 USBHOST_EP0_Init( void )
{
       return( USBHOST_EHCI_EP0_Init( &UDisk.Device) );
}

/*******************************************************************************
* Function Name  : USBHOST_CtrlTransfer
* Description    : USB��������һ�����ƴ���
* Input          : *pDatBuf----�շ����ݻ�����
*                   *pRetLen----�շ�����ʵ�ʳ���
* Output         : None   
* Return         : None
*******************************************************************************/
UINT8 USBHOST_CtrlTransfer( UINT8 *DatBuf, UINT16 *RetLen )
{
       return( USBHOST_Issue_Control( &UDisk.Device, Ctl_Setup, DatBuf, RetLen ) );
}

/*******************************************************************************
* Function Name  : USBHOST_GetDeviceDescr
* Description    : USB������ȡ�豸������
* Input          : *buf------���ݻ�����                 
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 USBHOST_GetDeviceDescr( UINT8 *buf )
{
    UINT16  len;    

       return( USBHOST_EHCI_GetDeviceDescr( &UDisk.Device, buf, &len ) );    
}

/*******************************************************************************
* Function Name  : USBHOST_GetConfigDescr
* Description    : USB������ȡ����������
* Input          : *buf------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 USBHOST_GetConfigDescr( UINT8 *buf )
{
    UINT16  len;

       return( USBHOST_EHCI_GetConfigDescr( &UDisk.Device, buf, &len ) );
}


/*******************************************************************************
* Function Name  : USBHOST_SetAddress
* Description    : USB�������õ�ַ
* Input          : addr------Ҫ���õĵ�ַ
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 USBHOST_SetAddress( UINT8 addr )
{
       return( USBHOST_EHCI_SetAddress( &UDisk.Device, addr ) );
}

/*******************************************************************************
* Function Name  : USBHOST_SetConfig
* Description    : USB������������ֵ
* Input          : cfg-------Ҫ���õ�����ֵ
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 USBHOST_SetConfig( UINT8 cfg )
{
       return( USBHOST_EHCI_SetConfig( &UDisk.Device, cfg ) );
}

/*******************************************************************************
* Function Name  : USBHOST_ClearEndpStall
* Description    : USB��������˵�
* Input          : endp------Ҫ����Ķ˵��
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 USBHOST_ClearEndpStall( UINT8 endp )
{
    return( USBHOST_EHCI_ClearEndpStall( &UDisk.Device, endp ) );
}

/*******************************************************************************
* Function Name  : USBHOST_SetTargetAddr
* Description    : USB��������Ŀ���ַ
* Input          : addr------Ҫ���õĵ�ַ
* Output         : None
* Return         : None
*******************************************************************************/
void USBHOST_SetTargetAddr( UINT8 addr )
{
}

/*******************************************************************************
* Function Name  : USBHOST_Detect_Connect
* Description    : USB��������豸���
*                  ע���˿�ʹ��λ�����߸�λʱ�������������Զ���1
* Input          : None
* Output         : None
* Return         : 0x00-----------------��USB�豸
*                  0x01-----------------��USB�豸����δʹ��
*                  0x02-----------------��USB�豸���Ѿ�ʹ��
*                  USB_INT_CONNECT------��⵽USB�豸�����¼�
*                  USB_INT_DISCONNECT---��⵽USB�豸�Ͽ��¼�
*******************************************************************************/
UINT8 USBHOST_Detect_Connect( void )
{
    return( USBHOST_EHCI_Detect_Connect( &UDisk.Device ) );
}

/*******************************************************************************
* Function Name  : USBHOST_Parameter_Init
* Description    : ��ʼ���豸��ز���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBHOST_Parameter_Init( void )
{                                                                
    USBHOST_DevicePara_Init( &UDisk.Device );
}

/*******************************************************************************
* Function Name  : USBHOST_Enumerate
* Description    : USB�������豸����ö��
* Input          : *pbuf-----���ݻ�����
* Output         : None
* Return         : USB_OPERATE_SUCCESS---�豸ö�ٳɹ�;
*                  USB_OPERATE_ERROR-----�豸ö��ʧ��;
*******************************************************************************/
UINT8 USBHOST_Enumerate( UINT8 *pbuf )
{
    return( USBHOST_EHCI_Enumerate( &UDisk.Device, pbuf ) );
}

/******************************************************************************/
/******************************************************************************/
/* ����ΪU�̵ײ���صڷ�װ���� */
/******************************************************************************/
/******************************************************************************/
/*******************************************************************************
* Function Name  : USBHOST_MS_CofDescrAnalyse
* Description    : ����Mass Storage Device����������
* Input          : *pbuf--------��ǰ��Ҫ����������������������
* Output         : None
* Return         : USB_OPERATE_SUCCESS----Mass Storage Device; 
*                  USB_OPERATE_ERROR---�����豸
*******************************************************************************/
UINT8 USBHOST_MS_CofDescrAnalyse( UINT8 *pbuf )
{
    return( MS_CofDescrAnalyse( &UDisk, pbuf ) );    
}

/*******************************************************************************
* Function Name  : MS_GetMaxLun
* Description    : USB������ȡ����߼���Ԫ��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_GetMaxLun( void )
{
    return( MS_GetMaxLun_EX( &UDisk ) );    
}

/*******************************************************************************
* Function Name  : MS_ResetErrorBOC
* Description    : USB������λU��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_ResetErrorBOC( void )
{
    return( MS_ResetErrorBOC_EX( &UDisk ) );    
}

/*******************************************************************************
* Function Name  : MS_ScsiCmd_Process
* Description    : ִ�л���BulkOnlyЭ��������
*                  ����һ�ο��Դ������20K����,��δ����ѭ������
*                  ע��: ���ݻ����������4K�߽�����,���δ����,�򻺳���������С����
*                        ��ʵ��Ҫ��д�Ĵ�СҪ��4K�� 
* Input          : *DataBuf-----���롢������ݻ�����
* Output         : None
* Return         : ����ִ��״̬
*******************************************************************************/
UINT8 MS_ScsiCmd_Process( UINT8 *DataBuf )
{
    return( MS_ScsiCmd_Process_EX( &UDisk, DataBuf ) );    
}

/*******************************************************************************
* Function Name  : MS_Init
* Description    : USB�������洢�豸��ʼ�� 
* Input          : *pbuf------���ݻ����� 
* Output         : None
* Return         : USB_OPERATE_SUCCESS---��ʼ���ɹ�;
*                  USB_OPERATE_ERROR--��ʼ��ʧ��;
*******************************************************************************/
UINT8 MS_Init( UINT8 *pbuf )
{
    return( MS_Init_EX( &UDisk, pbuf ) );
}

/*******************************************************************************
* Function Name  : MS_RequestSense
* Description    : USB���������̴���״̬
* Input          : *pbuf------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_RequestSense( UINT8 *pbuf )
{
    return( MS_RequestSense_EX( &UDisk, pbuf ) );
}

/*******************************************************************************
* Function Name  : MS_DiskInquiry
* Description    : USB������ȡ��������
* Input          : *pbuf------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_DiskInquiry( UINT8 *pbuf )
{
    return( MS_DiskInquiry_EX( &UDisk, pbuf ) );
}

/*******************************************************************************
* Function Name  : MS_DiskCapacity
* Description    : USB������ȡ��������
* Input          : *pbuf------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_DiskCapacity( UINT8 *pbuf )
{
    return( MS_DiskCapacity_EX( &UDisk, pbuf ) );
}

/*******************************************************************************
* Function Name  : MS_DiskTestReady
* Description    : USB�������Դ����Ƿ����
* Input          : *pbuf------���ݻ����� 
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 MS_DiskTestReady( UINT8 *pbuf )
{
    return( MS_DiskTestReady_EX( &UDisk, pbuf ) );
}

/*******************************************************************************
* Function Name  : MS_ReadSector
* Description    : ������Ϊ��λ�Ӵ��̶�ȡ����
* Input          : StartLba----��ʼ������
*                  SectCount---��Ҫ��ȡ��������Ŀ
*                  DataBuf-----���ݻ����� 
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 MS_ReadSector( UINT32 StartLba, UINT32 SectCount, PUINT8 DataBuf )
{
    return( MS_ReadSector_EX( &UDisk, StartLba, SectCount, DataBuf ) );
}

/*******************************************************************************
* Function Name  : MS_WriteSector
* Description    : ������Ϊ��λ������д�����
* Input          : StartLba----��ʼ������
*                  SectCount---��Ҫд���������Ŀ
*                  DataBuf-----���ݻ�����
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 MS_WriteSector( UINT32 StartLba, UINT8 SectCount, PUINT8 DataBuf )
{
    return( MS_WriteSector_EX( &UDisk, StartLba, SectCount, DataBuf ) );
}


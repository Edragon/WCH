/********************************** (C) COPYRIGHT *******************************
* File Name          : HID.C
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : CH563 USB Host Controller HID Operate
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CH563BAS.H"
#include "CH563SFR.H"
#include "CH563USBSFR.H"                                                        /* оƬUSB�Ĵ������ͷ�ļ� */    
#include "CH563USB.H"                                                           /* USB���ö������ͷ�ļ� */    
#include "EHCI_HCD.H"                                                           /* �����������Ͳ�������ͷ�ļ� */
#include "PRINTF.H"                                                             /* ���ڴ�ӡ�������ͷ�ļ� */
#include "HID.H"                                                                /* HID�ඨ�����ͷ�ļ� */

/*******************************************************************************
* Function Name  : HID_CofDescrAnalyse
* Description    : ����HID������������
* Input          : *Hidinfo-----��ǰ������HID���豸��ؽṹ��
*                  *pbuf--------��ǰ��Ҫ����������������������
* Output         : None
* Return         : USB_OPERATE_SUCCESS----HID���豸; 
*                  USB_OPERATE_ERROR------�����豸
*******************************************************************************/
UINT8 HID_CofDescrAnalyse( HID_INFO *Hidinfo, UINT8 *pbuf )
{
    UINT16 i, j;
    UINT32 temp;

    /* �򵥷�������������,������������Ϣ */    
    /* ���ڷ����豸,ֻ�������̡����Ľӿ�,�����������ӿ� */
    Hidinfo->IntfacNum = pbuf[ 4 ];                                             /* ����ӿں� */
    Hidinfo->ReportDes_Len1 = 0x00;
    Hidinfo->ReportDes_Len2 = 0x00;    
    for( i = 0; i < pbuf[ 2 ]; i++ )
    {
        if( ( pbuf[ i ] == 0x09 ) && ( pbuf[ i + 1 ] == 0x04 ) && ( pbuf[ i + 2 ] == 0x00 ) )
        {
            for( j = 0 ; j < pbuf[ 2 ] - i; j++ ) 
            {
                if( ( pbuf[ i + j ] == 0x09 ) && ( pbuf[ i + j + 1 ] == 0x21 ) && 
                    ( pbuf[ i + j + 6 ] == 0x22 ) )                             /* �ж��Ƿ�ΪHID��˵� */    
                {    
                    Hidinfo->ReportDes_Len1 = pbuf[ i + j + 7 ];                /* ���汨���������ĳ��� */    
                    Hidinfo->HidType = pbuf[ i + 7 ];                    
                    break;
                }
            }
            if( Hidinfo->IntfacNum == 0x01 )
            {
                break;
            }                    
        }
        else if( ( pbuf[ i ] == 0x09 ) && ( pbuf[ i + 1 ] == 0x04 ) && ( pbuf[ i + 2 ] == 0x01 ) )
        {
            for( j = 0 ; j < pbuf[ 2 ] - i; j++ ) 
            {
                if( ( pbuf[ i + j ] == 0x09 ) && ( pbuf[ i + j + 1 ] == 0x21 ) && 
                    ( pbuf[ i + j + 6 ] == 0x22 ) )                             /* �ж��Ƿ�ΪHID��˵� */    
                {    
                    Hidinfo->ReportDes_Len2 = pbuf[ i + j + 7 ];                /* ���汨���������ĳ��� */    
                    break;
                }
            }
            if( Hidinfo->ReportDes_Len2 != 0x00 )
            {
                break;
            }                    
        }                                
    }

    for( i = 0; i < pbuf[ 2 ]; i++ )
    {
        if( ( pbuf[ i ] == 0x09 ) && ( pbuf[ i + 1 ] == 0x04 ) && 
            ( pbuf[ i + 2 ] == 0x00 ) && ( pbuf[ i + 5 ] == 0x03 ) )            /* �ӿ�������ΪHID����ꡢ���� */   
        { 
            for( j = 0 ; j < pbuf[ 2 ] - i; j++ ) 
            {
                if( ( pbuf[ i + j ] == 0x07 ) && ( pbuf[ i + j + 1 ] == 0x05 ) && 
                    ( pbuf[ i + j + 3 ] == 0x03 ) )                             /* �ж��Ƿ�Ϊ�ж϶˵� */    
                {    
                    if( pbuf[ i + j + 2 ] & 0x80 )                              /* IN�˵�ĵ�ַ */
                    {
                        Hidinfo->IntInEp1 = pbuf[ i + j + 2 ];                  /* ����IN�˵�1�˵�� */
                        Hidinfo->IntInEp1Size = pbuf[ i + j + 4 ];              /* ����IN�˵�1�˵��С */
                    }
                    break;                    
                }       
            }
            if( Hidinfo->IntfacNum == 0x01 )
            {
                break;
            }                    
        }
        else if( ( pbuf[ i ] == 0x09 ) && ( pbuf[ i + 1 ] == 0x04 ) && 
               ( pbuf[ i + 2 ] == 0x01 ) && ( pbuf[ i + 5 ] == 0x03 ) )         /* �ӿ�������ΪHID����ꡢ���� */   
        { 
            for( j = 0 ; j < pbuf[ 2 ] - i; j++ ) 
            {
                if( ( pbuf[ i + j ] == 0x07 ) && ( pbuf[ i + j + 1 ] == 0x05 ) && 
                    ( pbuf[ i + j + 3 ] == 0x03 ) )                             /* �ж��Ƿ�Ϊ�ж϶˵� */    
                {    
                    if( pbuf[ i + j + 2 ] & 0x80 )                              /* IN�˵�ĵ�ַ */
                    {
                        Hidinfo->IntInEp2 = pbuf[ i + j + 2 ];                  /* ����IN�˵�2�˵�� */
                        Hidinfo->IntInEp2Size = pbuf[ i + j + 4 ];              /* ����IN�˵�2�˵��С */                                        
                    }
                    break;                     
                }       
            }                
        }                
    }
    if( ( Hidinfo->IntInEp1 == 0x00 ) && ( Hidinfo->IntInEp2 == 0x00 ) )
    {
        return( USB_OPERATE_ERROR );                                            /* ��ǰ����HID���豸,ֱ�ӷ��� */
    }

#ifdef  MY_DEBUG_PRINTF 
    printf(" Hidinfo->HidType:%x\n ",(UINT16)Hidinfo->HidType);            
    printf("Hidinfo->IntfacNum:%x\n",(UINT16)Hidinfo->IntfacNum);        
    printf("Hidinfo->IntInEp1:%x\n",(UINT16)Hidinfo->IntInEp1);
    printf("Hidinfo->IntInEp1Size:%x\n",(UINT16)Hidinfo->IntInEp1Size);
    printf("Hidinfo->ReportDes_Len1:%x\n",(UINT16)Hidinfo->ReportDes_Len1);
    printf("Hidinfo->IntInEp2:%x\n",(UINT16)Hidinfo->IntInEp2);
    printf("Hidinfo->IntInEp2Size:%x\n",(UINT16)Hidinfo->IntInEp2Size);
    printf("Hidinfo->ReportDes_Len2:%x\n",(UINT16)Hidinfo->ReportDes_Len2);
#endif

    /*******************************����HID���豸����ڴ漰QH******************/
    if( Hidinfo->IntInEp1 )
    {    
        /* �����жϴ������QH(qHD_INT_In1) */
        temp = USBHOST_GetStructure( EHCI_MEM_TYPE_QHD );
        if( temp == USB_MEM_ALLOCATE_ERROR )
        {
            return( USB_MEM_ALLOCATE_ERROR );
        }    
        Hidinfo->qHD_INT_In1 = ( qHD_Structure *)temp;                          /* ����qHD_Structure */
    
        /* ��ʼ�������IN�˵����QHD */
        USBHOST_Allocate_QHD( &Hidinfo->Device, 
                              Hidinfo->qHD_INT_In1, 
                              EHCI_HD_TYPE_QH, 
                              EHCI_EP_TYPE_NO_CONTROL, 
                              0, 
                              Hidinfo->IntInEp1, 
                              Hidinfo->IntInEp1Size );    
    
           /* �����Դ������QHDָ����� */
        Hidinfo->qHD_INT_In1->bTerminate = 0x01;                                /* ��һ��ָ����Ч */
    }
    
    if( Hidinfo->IntInEp2 )
    {    
        /* �����жϴ������QH(qHD_INT_In2) */
        temp = USBHOST_GetStructure( EHCI_MEM_TYPE_QHD );
        if( temp == USB_MEM_ALLOCATE_ERROR )
        {
            return( USB_MEM_ALLOCATE_ERROR );
        }    
        Hidinfo->qHD_INT_In2 = ( qHD_Structure *)temp;                          /* ����qHD_Structure */
    
        /* ��ʼ�������IN�˵����QHD */
        USBHOST_Allocate_QHD( &Hidinfo->Device, 
                              Hidinfo->qHD_INT_In2, 
                              EHCI_HD_TYPE_QH, 
                              EHCI_EP_TYPE_NO_CONTROL, 
                              0, 
                              Hidinfo->IntInEp2, 
                              Hidinfo->IntInEp2Size );    

           /* �����Դ������QHDָ����� */
        Hidinfo->qHD_INT_In2->bTerminate = 0x01;                                /* ��һ��ָ����Ч */        
    }    
    return( USB_OPERATE_SUCCESS );
}

/*******************************************************************************
* Function Name  : HID_SetIdle
* Description    : HID���豸SET IDLE����
* Input          : *Hidinfo---��ǰHID��ؽṹ��
*                   intface----��ǰ�ӿں�
                   *buf-------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 HID_SetIdle( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf ) 
{
    UINT16 l;
    UINT8  status;
    
    /* ���SETUP����� */
    Ctl_Setup->bReqType = 0x21;
    Ctl_Setup->bRequest = 0x0A;
    Ctl_Setup->wValueL  = 0x00;
    Ctl_Setup->wValueH  = 0x00;
    Ctl_Setup->wIndexL  = intface;
    Ctl_Setup->wIndexH  = 0x00;
    Ctl_Setup->wLengthL = 0x00;
    Ctl_Setup->wLengthH = 0x00;
    
    /* SET_IDLE */
    l = 0x00;
    status = USBHOST_Issue_Control( &Hidinfo->Device, Ctl_Setup, buf, &l );     /* ִ�п��ƴ��� */    
    return( status );
}

/*******************************************************************************
* Function Name  : HID_GetReportDescr
* Description    : ��ȡHID�౨��������
* Input          : *Hidinfo---��ǰHID��ؽṹ��
*                   intface----��ǰ�ӿں�
                   *buf-------���ݻ�����
                   *len-------���ݳ���
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 HID_GetReportDescr( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf, UINT16 len ) 
{
    UINT16 l;
    UINT8  status;
    
    /* ���SETUP����� */
    Ctl_Setup->bReqType = 0x81;
    Ctl_Setup->bRequest = 0x06;
    Ctl_Setup->wValueL  = 0x00;
    Ctl_Setup->wValueH  = 0x22;
    Ctl_Setup->wIndexL  = intface;
    Ctl_Setup->wIndexH  = 0x00;
    Ctl_Setup->wLengthL = ( UINT8 )len;
    Ctl_Setup->wLengthH = ( UINT8)( len >> 8 );
    
    /* ��ȡHID�౨�������� */
    l = len;
    status = USBHOST_Issue_Control( &Hidinfo->Device, Ctl_Setup, buf, &l );     /* ִ�п��ƴ��� */    
    return( status );
}

/*******************************************************************************
* Function Name  : HID_SetReport
* Description    : HID���豸SET REPORT����
* Input          : *Hidinfo---��ǰHID��ؽṹ��
*                   intface----��ǰ�ӿں�
                   *buf-------���ݻ�����
* Output         : None
* Return         : ���ص�ǰ����ִ��״̬
*******************************************************************************/
UINT8 HID_SetReport( HID_INFO *Hidinfo, UINT8 intface, UINT8 *buf ) 
{
    UINT16 l;
    UINT8  status;
    
    /* ���SETUP����� */
    Ctl_Setup->bReqType = 0x21;
    Ctl_Setup->bRequest = 0x09;
    Ctl_Setup->wValueL  = 0x00;
    Ctl_Setup->wValueH  = 0x02;
    Ctl_Setup->wIndexL  = intface;
    Ctl_Setup->wIndexH  = 0x00;
    Ctl_Setup->wLengthL = 0x01;
    Ctl_Setup->wLengthH = 0x00;
    *buf = 0x00;                                                                /* Ϩ������LEDָʾ�� */
    
    /* SET REPORT */
    l = 0x01;
    status = USBHOST_Issue_Control( &Hidinfo->Device, Ctl_Setup, buf, &l );     /* ִ�п��ƴ��� */    
    return( status );
}

/*******************************************************************************
* Function Name  : HID_ClassRequest_Deal
* Description    : HID��������
* Input          : *Hidinfo-----��ǰ������HID���豸��ؽṹ��
*                  *pbuf--------������
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 HID_ClassRequest_Deal( HID_INFO *Hidinfo, UINT8 *pbuf )
{
    UINT8  status;
    UINT16 i;

    /**************************************************************************/    
    /* Set Idle */ 
#ifdef  MY_DEBUG_PRINTF            
    printf("Set Idle\n");        
#endif
    status = HID_SetIdle( Hidinfo, 0x00, pbuf );
    if( status )
    {
#ifdef  MY_DEBUG_PRINTF            
    printf("Set Idle Error\n");        
#endif        
    }
    
    /* ����еڶ���HID��ӿ�,���������ýӿ� */
    if( ( Hidinfo->IntfacNum > 1 ) && ( Hidinfo->IntInEp2 != 0x00 ) )
    {
#ifdef  MY_DEBUG_PRINTF            
        printf("Set Idle2\n");        
#endif
        status = HID_SetIdle( Hidinfo, 0x01, pbuf );
        if( status )
        {
#ifdef  MY_DEBUG_PRINTF            
        printf("Set Idle2 Error\n");        
#endif        
        }        
    }

    /***********************************************************************************/      
    /* Get Report Desc */ 
#ifdef  MY_DEBUG_PRINTF            
    printf("Get Report Desc\n");        
#endif
    status = HID_GetReportDescr( Hidinfo, 0x00, pbuf, Hidinfo->ReportDes_Len1 + 0x40 );
    if( status )
    {
#ifdef  MY_DEBUG_PRINTF            
        printf("Get Report Desc Error\n");    
#endif        
    }
    else
    {
#ifdef  MY_DEBUG_PRINTF
        for( i = 0; i < Hidinfo->ReportDes_Len1; i++ )
        {
            printf("%02x ",pbuf[ i ]);        
        } 
        printf("\n"); 
#endif
    }

    /* ����еڶ���HID��ӿ�,���������ýӿ� */
    if( ( Hidinfo->IntfacNum > 1 ) && ( Hidinfo->IntInEp2 != 0x00 ) )
    {
#ifdef  MY_DEBUG_PRINTF            
        printf("Get Report Desc2\n");        
#endif
        status = HID_GetReportDescr( Hidinfo, 0x01, pbuf, Hidinfo->ReportDes_Len2 + 0x40 );
        if( status )
        {
#ifdef  MY_DEBUG_PRINTF            
        printf("Get Report Desc2 Error\n");    
#endif        
        }
        else
        {
#ifdef  MY_DEBUG_PRINTF    
            for( i = 0; i < Hidinfo->ReportDes_Len2; i++ )
            {
                printf("%02x ",pbuf[ i ]);        
            } 
            printf("\n"); 
#endif
        }        
    }

    /***********************************************************************************/      
    /* SET REPORT */ 
    /* ������ֻ�����������豸 */
    if( Hidinfo->HidType == HID_TYPE_KEYBOARD )
    {
#ifdef  MY_DEBUG_PRINTF            
        printf("Set Report\n");        
#endif
        status = HID_SetReport( Hidinfo, 0x00, pbuf );
        if( status )
        {
#ifdef  MY_DEBUG_PRINTF            
            printf("Set Report Error\n");    
#endif        
        }
    }
    return( status );        
}

/*******************************************************************************
* Function Name  : HID_Issue_GetData
* Description    : �����ȡ�˵���������
* Input          : *Hidinfo-----��ǰ������HID���豸��ؽṹ��
*                  *spTempqHD---��ǰҪִ�е�QHD
*                  *pbuf--------��ǰ���ݻ�����
*                  len----------��ǰ��ȡ���� 
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 HID_Issue_GetData( HID_INFO *Hidinfo, qHD_Structure *spTempqHD, UINT8 *pbuf, UINT16 len )
{
    UINT32 temp;
    qTD_Structure *spTempqTD;

    /* ��ʼ���жϴ���ķ���qTD */    
    spTempqTD = ( qTD_Structure *)USBHOST_GetStructure( EHCI_MEM_TYPE_QTD ); 
    spTempqTD->bTotalBytes = len;      
//    spTempqTD->bDataToggle = 0;  
    spTempqTD->ArrayBufferPointer_Word[ 0 ] = (UINT32)pbuf;
    temp = ( ( ( UINT32 )( pbuf ) >> 12 ) + 1 ) << 12;
    spTempqTD->ArrayBufferPointer_Word[ 1 ] = (UINT32)temp; 
    spTempqTD->ArrayBufferPointer_Word[ 2 ] = (UINT32)temp + 4096;                    
    spTempqTD->ArrayBufferPointer_Word[ 3 ] = (UINT32)temp + 2 * 4096;                    
    spTempqTD->ArrayBufferPointer_Word[ 4 ] = (UINT32)temp + 3 * 4096;                       
    spTempqTD->bPID = EHCI_QTD_PID_IN;             

    /* USB��������һ��QTD */
    temp = USBHOST_Issue_qTD( &Hidinfo->Device, spTempqTD, spTempqHD );

    if( spTempqHD == Hidinfo->qHD_INT_In1 )
    {    
        Hidinfo->IntInEp1qTD = ( qTD_Structure *)temp;                          /* �ݴ�����ִ�е�qTD */
    }
    else if( spTempqHD == Hidinfo->qHD_INT_In2 )
    {
        Hidinfo->IntInEp2qTD = ( qTD_Structure *)temp;                          /* �ݴ�����ִ�е�qTD */        
    } 
    return( USB_OPERATE_SUCCESS );
}

/*******************************************************************************
* Function Name  : HID_Check_Data
* Description    : ��ѯ�˵��Ƿ��ȡ������
* Input          : *Hidinfo-----��ǰ������HID���豸��ؽṹ��
*                  *spTempqHD---��ǰҪִ�е�QHD
*                  len----------��ǰ��ȡ���� 
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 HID_Check_Data( HID_INFO *Hidinfo, qHD_Structure *spTempqHD, UINT16 *len )
{
    UINT8  status = 0xFF;

    if( spTempqHD == Hidinfo->qHD_INT_In1 )
    {    
        status = USBHOST_Check_qTD( &Hidinfo->Device, Hidinfo->IntInEp1qTD, Hidinfo->qHD_INT_In1, len );    
    }
    else if( spTempqHD == Hidinfo->qHD_INT_In2 )
    {
        status = USBHOST_Check_qTD( &Hidinfo->Device, Hidinfo->IntInEp2qTD, Hidinfo->qHD_INT_In2, len );        
    } 
    return( status );
}

/*******************************************************************************
* Function Name  : HID_Stop_GetData
* Description    : ����ǿ�Ƴ����ö˵��ȡ��������
* Input          : *Hidinfo-----��ǰ������HID���豸��ؽṹ��
*                  *spTempqHD---��ǰҪִ�е�QHD
*                  len----------��ǰ��ȡ���� 
* Output         : None
* Return         : None
*******************************************************************************/

UINT8 HID_Stop_GetData( HID_INFO *Hidinfo, qHD_Structure *spTempqHD )
{
    UINT8  status = 0xFF;

    if( spTempqHD == Hidinfo->qHD_INT_In1 )
    {    
        status = USBHOST_Remove_qTD( &Hidinfo->Device, Hidinfo->IntInEp1qTD, Hidinfo->qHD_INT_In1 );    
    }
    else if( spTempqHD == Hidinfo->qHD_INT_In2 )
    {
        status = USBHOST_Remove_qTD( &Hidinfo->Device, Hidinfo->IntInEp2qTD, Hidinfo->qHD_INT_In2 );        
    }
    return( status );        
}

/*********************************** endfile **********************************/
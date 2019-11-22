/********************************** (C) COPYRIGHT *******************************
* File Name          : HID.C
* Author             : WCH
* Version            : V1.20
* Date               : 2013/11/15
* Description        : CH563 USB Host Controller HID Operate
*******************************************************************************/



/******************************************************************************/
/* 头文件包含 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CH563BAS.H"
#include "CH563SFR.H"
#include "CH563USBSFR.H"                                                        /* 芯片USB寄存器相关头文件 */    
#include "CH563USB.H"                                                           /* USB常用定义相关头文件 */    
#include "EHCI_HCD.H"                                                           /* 主机控制器低层操作相关头文件 */
#include "PRINTF.H"                                                             /* 串口打印输出控制头文件 */
#include "MIDI.H"                                                                /* HID类定义相关头文件 */

/*******************************************************************************
* Function Name  : MIDI_CofDescrAnalyse
* Description    : 分析Audio类配置描述符
* Input          : *midiinfo-----当前操作的Audio类设备相关结构体
*                  *pbuf--------当前需要分析的配置描述符缓冲区
* Output         : None
* Return         : USB_OPERATE_SUCCESS----MIDI类设备; 
*                  USB_OPERATE_ERROR------其它设备
*******************************************************************************/
UINT8 MIDI_CofDescrAnalyse( MIDI_INFO *midiinfo, UINT8 *pbuf )
{
    MIDI_INFO* pmidi = midiinfo;
	PUINT8 p;
	UINT16 i,len;
    UINT32 temp;

    /* 简单分析配置描述符,并保存有用信息 */    
    /* 对于符合设备,只处理MIDI接口,而忽略其它接口 */
	p = pbuf;
	len = ((UINT16)pbuf[3])<<8 | pbuf[2];	
	for( i=0; i<len;)
	{
		if( *(p+1)==0x04 )		//接口描述符
		{
			if((*(p+5)==0x01)&&(*(p+6)==0x03))		//MIDI Streaming
			{
				pmidi->AudioType = 0x03; 
				pmidi->IntfacNum = *(p+2);
			}
		}
		if ((pmidi->AudioType==0x03)&&(*(p+1)==0x05))	//端点描述符
		{
			if(*(p+3)==0x02) //bulk
			{
				if((*(p+2))&0x80)		//IN端点
				{
					pmidi->BulkInEp = (*(p+2))&0x0f;
					pmidi->BulkInEpSize = ((UINT16)*(p+5))<<8 | (*(p+4));
				}
				else{					//OUT端点
					pmidi->BulkOutEp = (*(p+2))&0x0f;
					pmidi->BulkOutEpSize = ((UINT16)*(p+5))<<8 | (*(p+4));
				}
			}
		}		
		i += *p;
		p = pbuf+i;
	}

#ifdef  MY_DEBUG_PRINTF 
    printf("pmidi->AudioType:    %x\n ",  (UINT16)pmidi->AudioType);            
    printf("pmidi->IntfacNum:    %x\n",   (UINT16)pmidi->IntfacNum);        
    printf("pmidi->BulkInEp:     %x\n",   (UINT16)pmidi->BulkInEp);
    printf("pmidi->BulkInEpSize: %x\n",   (UINT16)pmidi->BulkInEpSize);
    printf("pmidi->BulkOutEp:    %x\n",   (UINT16)pmidi->BulkOutEp);
    printf("pmidi->BulkOutEpSize:%x\n",   (UINT16)pmidi->BulkOutEpSize);
#endif
	
    if((pmidi->AudioType!=0x03)||(pmidi->BulkInEp==0)||(pmidi->BulkOutEp==0))
    {
        return( USB_OPERATE_ERROR );                                            /* 当前不是MIDI设备,直接返回 */
    }
	
    /*******************************分配MIDI设备相关内存及QH******************/
	/* 分配批量传输相关QH(qHD_Bulk_In) */
    if( pmidi->BulkInEp )
    {    
        /* 分配中断传输相关QH(qHD_INT_In1) */
        temp = USBHOST_GetStructure( EHCI_MEM_TYPE_QHD );
        if( temp == USB_MEM_ALLOCATE_ERROR )
        {
            return( USB_MEM_ALLOCATE_ERROR );
        }    
        pmidi->qHD_BLK_IN = ( qHD_Structure *)temp;                          /* 分配qHD_Structure */
    
        /* 初始化分配的IN端点相关QHD */
        USBHOST_Allocate_QHD( &pmidi->Device, 
                              pmidi->qHD_BLK_IN, 
                              EHCI_HD_TYPE_QH, 
                              EHCI_EP_TYPE_NO_CONTROL, 
                              0, 
                              pmidi->BulkInEp, 
                              pmidi->BulkInEpSize );    
                              
		/* 指针关联 */ 
		USBHOST_qHD_Link_Insert( pmidi->qHD_BLK_IN );
    }
    
	/* 分配批量传输相关QH(qHD_Bulk_Out) */
    if( pmidi->BulkOutEp )
    {    
        temp = USBHOST_GetStructure( EHCI_MEM_TYPE_QHD );
        if( temp == USB_MEM_ALLOCATE_ERROR )
        {
            return( USB_MEM_ALLOCATE_ERROR );
        }    
        pmidi->qHD_BLK_OUT = ( qHD_Structure *)temp;                          /* 分配qHD_Structure */
    
        /* 初始化分配的IN端点相关QHD */
        USBHOST_Allocate_QHD( &pmidi->Device, 
                              pmidi->qHD_BLK_OUT, 
                              EHCI_HD_TYPE_QH, 
                              EHCI_EP_TYPE_NO_CONTROL, 
                              0, 
                              pmidi->BulkOutEp, 
                              pmidi->BulkOutEpSize );    

        USBHOST_qHD_Link_Insert( pmidi->qHD_BLK_OUT );   
	}    
    return( USB_OPERATE_SUCCESS );
}


/*********************************** endfile **********************************/

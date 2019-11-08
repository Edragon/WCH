
#include "dataflash.h"

UINT8 ReadDataFlash(UINT8 Addr,UINT8 len,PUINT8 buf)
{
    UINT8 i;
	E_DIS = 1;   //��ֹ�ж�ʹ��
	
    ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
    Addr <<= 1;
    for(i=0;i<len;i++)
	{
	  ROM_ADDR_L = Addr + i*2;                                                   //Addr����Ϊż��ַ
	  ROM_CTRL = ROM_CMD_READ;
     if ( ROM_STATUS & bROM_CMD_ERR ) return( 0xFF );                        // unknown command
	  *(buf+i) = ROM_DATA_L;
	}
	E_DIS = 0;   //��ֹ�ж�ʹ��
    return i;
}


/*******************************************************************************
* Function Name  : WriteDataFlash(UINT8 Addr,PUINT8 buf,UINT8 len)
* Description    : DataFlashд
* Input          : UINT8 Addr��PUINT16 buf,UINT8 len
* Output         : None
* Return         : UINT8 i ����д�볤��
*******************************************************************************/
UINT8 FLASH_Write(UINT8 Addr,PUINT8 buf,UINT8 len)
{
    UINT8 i;
	UINT16 add;
    E_DIS = 1;   //��ֹ�ж�ʹ��
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;                                                           //���밲ȫģʽ
    GLOBAL_CFG |= bDATA_WE;                                                    //ʹ��DataFlashд
    SAFE_MOD = 0;                                                          //�˳���ȫģʽ	
    add = 0xC000+(Addr*2);
    for(i=0;i<len;i++)
    {
		ROM_ADDR = add;
		add+=2;
		ROM_DATA = *(buf+i);	
		if ( ROM_STATUS & bROM_ADDR_OK )                                     // ������ַ��Ч
	    { 
			ROM_CTRL = ROM_CMD_WRITE;                                           // д��

        }
    }
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;                                                           //���밲ȫģʽ
    GLOBAL_CFG &= ~(bCODE_WE|bDATA_WE);                                                   //����DataFlashд����
    SAFE_MOD = 0;                                                              //�˳���ȫģʽ	
    E_DIS = 0;   //��ֹ�жϹر�
    return i;		
}

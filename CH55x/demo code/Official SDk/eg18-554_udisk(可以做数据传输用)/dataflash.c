
#include "dataflash.h"

UINT8 ReadDataFlash(UINT8 Addr,UINT8 len,PUINT8 buf)
{
    UINT8 i;
	E_DIS = 1;   //禁止中断使能
	
    ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
    Addr <<= 1;
    for(i=0;i<len;i++)
	{
	  ROM_ADDR_L = Addr + i*2;                                                   //Addr必须为偶地址
	  ROM_CTRL = ROM_CMD_READ;
     if ( ROM_STATUS & bROM_CMD_ERR ) return( 0xFF );                        // unknown command
	  *(buf+i) = ROM_DATA_L;
	}
	E_DIS = 0;   //禁止中断使能
    return i;
}


/*******************************************************************************
* Function Name  : WriteDataFlash(UINT8 Addr,PUINT8 buf,UINT8 len)
* Description    : DataFlash写
* Input          : UINT8 Addr，PUINT16 buf,UINT8 len
* Output         : None
* Return         : UINT8 i 返回写入长度
*******************************************************************************/
UINT8 FLASH_Write(UINT8 Addr,PUINT8 buf,UINT8 len)
{
    UINT8 i;
	UINT16 add;
    E_DIS = 1;   //禁止中断使能
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;                                                           //进入安全模式
    GLOBAL_CFG |= bDATA_WE;                                                    //使能DataFlash写
    SAFE_MOD = 0;                                                          //退出安全模式	
    add = 0xC000+(Addr*2);
    for(i=0;i<len;i++)
    {
		ROM_ADDR = add;
		add+=2;
		ROM_DATA = *(buf+i);	
		if ( ROM_STATUS & bROM_ADDR_OK )                                     // 操作地址有效
	    { 
			ROM_CTRL = ROM_CMD_WRITE;                                           // 写入

        }
    }
    SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;                                                           //进入安全模式
    GLOBAL_CFG &= ~(bCODE_WE|bDATA_WE);                                                   //开启DataFlash写保护
    SAFE_MOD = 0;                                                              //退出安全模式	
    E_DIS = 0;   //禁止中断关闭
    return i;		
}

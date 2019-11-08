#ifndef	__DATAFLASH_H__
#define __DATAFLASH_H__

#include "CH554.H"
#include "DEBUG.H"

UINT8 ReadDataFlash(UINT8 Addr,UINT8 len,PUINT8 buf);
UINT8 FLASH_Write(UINT8 Addr,PUINT8 buf,UINT8 len);


#endif


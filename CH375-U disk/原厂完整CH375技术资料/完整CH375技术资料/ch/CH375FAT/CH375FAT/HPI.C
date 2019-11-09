#include "common.h"

extern XXGFLAGS bdata bXXGFlags;
extern unsigned char xdata DBUF[BUFFER_LENGTH];
//////////////////////////////////////////
//CH_CMD_BLOCK xdata inblock;
unsigned char xdata CHBUF[CHBUF_LENGTH];
SYS_INFO_BLOCK xdata DeviceInfo;
CH_CMD_BLOCK xdata CHCmdBlock;
CH_RSP_BLOCK xdata CHRspBlock;
FILE_INFO xdata ThisFile;
extern FREE_FAT_INFO xdata FreeFat;


unsigned char EnumMassDev(void)
{
	PMBR_BLOCK pMBR;
	PBPB_BLOCK pBPB;
	if(!mInitDisk( ))
	    return FALSE;  /* 初始化USB存储器 */
	pMBR=(PMBR_BLOCK)DBUF;
	DeviceInfo.BPB_BytesPerSec=512; //暂假设为512
	if(!SPC_LockMedia())
		return FALSE;
	if(!SPC_RequestSense())
		return FALSE;
	if(!SPC_TestUnit())
		return FALSE;
	if(!ReadCapacity())
		return FALSE;
	if(!RBC_Read(0x0,1,DBUF))
		return FALSE;
	if(DBUF[0]==0xeb||DBUF[0]==0xe9){
		DeviceInfo.StartSector=0;
		//DeviceInfo.TotalSector=SwapINT32(pMBR->TotalSector);
		}
	else	{
		DeviceInfo.StartSector=SwapINT32(pMBR->StartSector);
		//DeviceInfo.TotalSector=SwapINT32(pMBR->TotalSector);
		}
	pBPB=(PBPB_BLOCK)DBUF;
	if(!RBC_Read(DeviceInfo.StartSector,1,DBUF))
		return FALSE;
	DeviceInfo.BPB_BytesPerSec=SwapINT16(pBPB->BPB_BytesPerSec);
	DeviceInfo.BPB_SecPerClus=pBPB->BPB_SecPerClus;
	DeviceInfo.BPB_NumFATs=pBPB->BPB_NumFATs;
	DeviceInfo.BPB_RootEntCnt=SwapINT16(pBPB->BPB_RootEntCnt);
	DeviceInfo.BPB_TotSec16=SwapINT16(pBPB->BPB_TotSec16);
	DeviceInfo.BPB_FATSz16=SwapINT16(pBPB->BPB_FATSz16);
	DeviceInfo.BPB_TotSec32=SwapINT32(pBPB->BPB_TotSec32);
	DeviceInfo.FatStartSector=DeviceInfo.StartSector+pBPB->BPB_RsvdSecCn[0];
	DeviceInfo.RootStartSector=DeviceInfo.StartSector+2*DeviceInfo.BPB_FATSz16+pBPB->BPB_RsvdSecCn[0];
	DeviceInfo.FirstDataSector=DeviceInfo.FatStartSector+2*DeviceInfo.BPB_FATSz16+32;
	ThisFile.bFileOpen=0;
	bXXGFlags.bits.SLAVE_IS_ATTACHED=1;
	return TRUE;
}

//***********************************************
unsigned char DetectDevice(void)      //检测设备
{
#define RspBlockDetectDevice CHRspBlock.RspBlock.Rsp_DetectDevice
	return bXXGFlags.bits.SLAVE_IS_ATTACHED;

#undef RspBlockDetectDevice
}

unsigned char List(void)     //列表文件
{
#define RspBlockList CHRspBlock.RspBlock.Rsp_List
	unsigned int item,i;
	unsigned char j,k,bstop,sector;
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	item=0;
	bstop=0;
	//RspBlockList.result=0x1;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt>>4;sector++)
	    {
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			CHRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;
			}
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{bstop=1;
				break;}
			else if(DBUF[i]==0xE5)
				continue;
			else if((DBUF[i]&0x40==0x40)&&(DBUF[i+11]==0xff))
				{
				j=DBUF[i]&0x0F;
				j=j+1;
				for(k=0;k<j*32;k++)
					CHBUF[item*32+k]=DBUF[i+k];
				i=i+(j-1)*32;
				item=item+j;
				}
			else
				{
				for(k=0;k<32;k++)
					CHBUF[item*32+k]=DBUF[i+k];
				item=item+1;
				}
			}
		if(bstop==1)break;
	    }
	//pBuf=(PREAD_CAPACITY_RSP)DBUF;
	RspBlockList.len=SwapINT16(item*32);
	CHRspBlock.len=item*32;
	return TRUE;

#undef RspBlockList

}
unsigned char OpenFile(unsigned char *pBuffer)
{
#define RspBlockOpenFile CHRspBlock.RspBlock.Rsp_OpenFile
	unsigned int i;
	unsigned char j,bstop,sector;
	PDIR_INFO pDirInfo;

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	ThisFile.bFileOpen=0;
	//RspBlockOpenFile.result=0x0;
	//RspBlockOpenFile.errcode=ERC_OK;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt>>4;sector++)
	    {
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			CHRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;

			}
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{
				CHRspBlock.errcode=ERC_FILENOTFOUND;
				return FALSE;
				}
			j=0;
			while(DBUF[i+j]==*(pBuffer+j))
				{
				 j=j+1;
				 if(j>10)
				 	break;
				}

			if(j>10)
			    {
			    for(j=0;j<32;j++)
			    	RspBlockOpenFile.item[j]=DBUF[i+j];
			    //RspBlockOpenFile.result=0x1;
			    ThisFile.bFileOpen=1;
			    bstop=1;
			     break;}

			}
		if(bstop==1)break;
	    }

	    if(sector>=DeviceInfo.BPB_RootEntCnt>>4)
	    	{
	    	CHRspBlock.errcode=ERC_FILENOTFOUND;
		return FALSE;
	    	}
	pDirInfo=(PDIR_INFO)RspBlockOpenFile.item;
	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
	ThisFile.LengthInByte=SwapINT32(pDirInfo->length);
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.StartCluster);
	ThisFile.OffsetofSector=0;
	ThisFile.SectorofCluster=0;
	ThisFile.FatSectorPointer=0;
	//ThisFile.bFileOpen=1;
	ThisFile.pointer=0;
	FreeFat.SectorNum=DeviceInfo.FatStartSector;
	FreeFat.OldSectorNum=DeviceInfo.FatStartSector;
	FreeFat.OffsetofSector=2;

	return TRUE;
#undef RspBlockOpenFile
}

unsigned char ReadFile(unsigned int readLength,unsigned char *pBuffer)
{
#define RspBlockReadFile CHRspBlock.RspBlock.Rsp_ReadFile
	unsigned int idata len,i;
	unsigned char bSuccess;
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	if(!ThisFile.bFileOpen)
	{
	CHRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;
	}
	ThisFile.bFileOpen=0;
	bSuccess=1;
	CHRspBlock.len=0;
	if(readLength>MAX_READ_LENGTH)
		{
		CHRspBlock.errcode=ERC_LENGTHEXCEED;
		return FALSE;
		}
	if(readLength+ThisFile.pointer>ThisFile.LengthInByte)
		{
		CHRspBlock.errcode=ERC_LENGTHEXCEED;
		return FALSE;
		}
		while(readLength>0)
		{
		   if(readLength+ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec)
		   	len=DeviceInfo.BPB_BytesPerSec;
		   else
		   	len=readLength+ThisFile.OffsetofSector;
		   if(ThisFile.OffsetofSector>0)
		   	{
		   	if(RBC_Read(ThisFile.SectorPointer,1,DBUF))
		   		{
		   		len=len-ThisFile.OffsetofSector;
		   		for(i=0;i<len;i++)
		   			*(pBuffer+i)=DBUF[ThisFile.OffsetofSector+i];
		   		ThisFile.OffsetofSector=ThisFile.OffsetofSector+len;
		   		}
		   	else
		   		{
		   		CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
		   		}
		   	}
		   else
		   	{
		   		if(!RBC_Read(ThisFile.SectorPointer,1,pBuffer+CHRspBlock.len))
		   		{
		   		CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
		   		}
		   		ThisFile.OffsetofSector=len;
		   	}
		  // if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		  // 	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   readLength-=len;
		   CHRspBlock.len+=len;
		   if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		   {
		   	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   	ThisFile.SectorofCluster+=1;
		   	if(ThisFile.SectorofCluster>DeviceInfo.BPB_SecPerClus-1)
		   	{
		   		ThisFile.SectorofCluster=0;
		 		 ThisFile.ClusterPointer=GetNextClusterNum(ThisFile.ClusterPointer);
		 		 if(ThisFile.ClusterPointer>0xffef)
		 		 	{
		 		 	   //RspBlockReadFile.errcode=ERC_REACHEND;
		   			   //RspBlockReadFile.result=0x0;
		   			   CHRspBlock.errcode=ERC_REACHEND;
					   return FALSE;
		 		 	}
		 		 ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer);
		   	}
		   	else
		   		ThisFile.SectorPointer=ThisFile.SectorPointer+1;
		    }
		}
	ThisFile.bFileOpen=1;
	ThisFile.pointer+=CHRspBlock.len;
	RspBlockReadFile.readLength=SwapINT16(CHRspBlock.len);
	return TRUE;
#undef RspBlockReadFile
}

unsigned char CreateFile(unsigned char *pBuffer)		//创建文件
{
#define RspBlockCreateFile CHRspBlock.RspBlock.Rsp_CreateFile
	//unsigned long sectorNum;
	unsigned int sector,i,j;
	unsigned char bstop;
	PDIR_INFO pDirInfo;

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)      //检测设备是否连入
	{
	CHRspBlock.errcode=ERC_NODEVICE;			//否则返回错误
	return FALSE;
	}
	if(OpenFile( pBuffer))RemoveFile(pBuffer);
	//RspBlockCreateFile.result=0x1;
	//RspBlockCreateFile.errcode=ERC_OK;
	pDirInfo=(PDIR_INFO)pBuffer;
	//if(!SPC_TestUnit())
	//	return FALSE;
	//////// Search the fat for a free cluster  ////////////
	pDirInfo->startCluster=SwapINT16(GetFreeCusterNum());

	if(pDirInfo->startCluster<0x2)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	pDirInfo->length=0;
	/////// Search a free space in the root dir space and build the item ///
	ThisFile.bFileOpen=0;
	bstop=0;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt>>4;sector++)
	    {
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{

			CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
			}
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if((DBUF[i]==0x00)||(DBUF[i]==0xE5))
				{
				for(j=0;j<32;j++)
					DBUF[i+j]=*(pBuffer+j);
				if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
		  	 		{
		  	 		CHRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;
		  	 		}
				bstop=1;
				break;
				}
			}
		if(bstop==1)break;
	    }
	//pDirInfo=(PDIR_INFO)RspBlockOpenFile.item;
	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
	ThisFile.LengthInByte=0;
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.StartCluster);
	ThisFile.OffsetofSector=0;
	ThisFile.SectorofCluster=0;
	ThisFile.bFileOpen=1;
	ThisFile.pointer=0;
	ThisFile.FatSectorPointer=0;
	FreeFat.SectorNum=DeviceInfo.FatStartSector;
	FreeFat.OldSectorNum=DeviceInfo.FatStartSector;
	FreeFat.OffsetofSector=2;

	return TRUE;
#undef RspBlockCreateFile
}

unsigned char WriteFile(unsigned int writeLength,unsigned char *pBuffer)
{
#define RspBlockWriteFile CHRspBlock.RspBlock.Rsp_WriteFile
	unsigned int len,sector,i;
	PDIR_INFO pDirInfo;
	unsigned char bSuccess,bStop,step;

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)	//U盘是否已经连接
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	if(!ThisFile.bFileOpen)			//文件是否已经打开
	{
	CHRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;
	}
	ThisFile.bFileOpen=0;
	bSuccess=1;
	bStop=0;

	CHRspBlock.len=0;
	//if(CHCmdBlock.CmdBlock.Cmd_WriteFile.writeLength)
	while(writeLength>0)
	{
		 if(ThisFile.OffsetofSector>0)
		 	{
		 	if(writeLength+ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec)
		   		len=DeviceInfo.BPB_BytesPerSec;
			else
		   		len=writeLength+ThisFile.OffsetofSector;
		 	if(!RBC_Read(ThisFile.SectorPointer,1,DBUF))
				{
		   		CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
		   		}
			len=len-ThisFile.OffsetofSector;
		   	for(i=0;i<len;i++)
		   		DBUF[ThisFile.OffsetofSector+i]=*(pBuffer+i);
		   	if(!RBC_Write(ThisFile.SectorPointer,1,DBUF))
		   			{
		   			CHRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;
		   			}
		  	ThisFile.OffsetofSector=ThisFile.OffsetofSector+len;
		   	}
		 else
		 	{
		 	if(writeLength>DeviceInfo.BPB_BytesPerSec)
		   		{
		   		step=writeLength/DeviceInfo.BPB_BytesPerSec;
		   		if(step>DeviceInfo.BPB_SecPerClus)
		   			{
		   			step=DeviceInfo.BPB_SecPerClus-ThisFile.SectorofCluster;
		   			len=step*DeviceInfo.BPB_BytesPerSec;
		   			}
		   		else
		   			{
		   			step=step-ThisFile.SectorofCluster;
		   			len=step*DeviceInfo.BPB_BytesPerSec;
		   			}
		   		}
			else
		   		{
		   		step=1;
		   		len=writeLength;
		   		}
		 	if(!RBC_Write(ThisFile.SectorPointer,step,pBuffer+CHRspBlock.len))
		   		{
		   		CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
		   		}
		   	ThisFile.OffsetofSector=len;
		 	}
		 //if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		 //  	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   writeLength-=len;
		   CHRspBlock.len+=len;
		   //ThisFile.LengthInByte+=len;
		/////////////更新文件指针 //////////////////////////////
		//改变参数：OffsetofSector,SectorofCluster,SectorPointer,ClusterPointer
		  step=ThisFile.OffsetofSector/DeviceInfo.BPB_BytesPerSec;

		  if(step>0)
		   {
		   	ThisFile.OffsetofSector-=step*DeviceInfo.BPB_BytesPerSec;
		   	ThisFile.SectorofCluster+=step;
		   	if(ThisFile.SectorofCluster>DeviceInfo.BPB_SecPerClus-1)
		   	{
		   		ThisFile.SectorofCluster=0;
		   		if(ThisFile.pointer+CHRspBlock.len<ThisFile.LengthInByte)
		   			ThisFile.ClusterPointer=GetNextClusterNum(ThisFile.ClusterPointer);
		   		else
		 		 	ThisFile.ClusterPointer=CreateClusterLink(ThisFile.ClusterPointer);//GetNextClusterNum(ThisFile.ClusterPointer);
		 		 if(ThisFile.ClusterPointer==0x00)
		 		 	{
		 		 //	   RspBlockReadFile.errcode=ERC_REACHEND;
		   			//   RspBlockReadFile.result=0x0;
		   			CHRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;
		 			}
		 		 ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer);
		   	}
		   	else
		   		ThisFile.SectorPointer=ThisFile.SectorPointer+step;
		    }


	}//end while
	ThisFile.pointer+=CHRspBlock.len;
	UpdateFat(FreeFat.SectorNum);
	///////////更新文件目录信息/////////////////////////////
	if(bSuccess==1)
	{

		for(sector=0;sector<DeviceInfo.BPB_RootEntCnt>>4;sector++)
	    	{
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			CHRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;
			}
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			pDirInfo=(PDIR_INFO)(DBUF+i);

			if(pDirInfo->startCluster==SwapINT16(ThisFile.StartCluster))
				{
				if(ThisFile.pointer>ThisFile.LengthInByte)
					ThisFile.LengthInByte=ThisFile.pointer;
				//else ?
				pDirInfo->length=SwapINT32(ThisFile.LengthInByte);
				if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
		   		{
		   		CHRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;
		   		}
				 bStop=1;
				 break;
				}
			}
		if(bStop==1)
			break;
	       }
	}
	CHRspBlock.len=0;
	ThisFile.bFileOpen=1;
	return TRUE;
#undef RspBlockWriteFile
}

unsigned char RemoveFile(unsigned char *pBuffer)   //删除文件
{
#define RspBlockRemoveFile CHRspBlock.RspBlock.Rsp_RemoveFile
	unsigned int sector,i;
	unsigned char bStop,j;
	PDIR_INFO pDirInfo;

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	//RspBlockRemoveFile.result=0x1;
	////////////// 清除目录/////////////////////////////////////
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt>>4;sector++)
	    	{
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			CHRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;
			}
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{
				CHRspBlock.errcode=ERC_FILENOTFOUND;
				return FALSE;
				}
			j=0;
			while(DBUF[i+j]==*(pBuffer+j))
				{
				 j=j+1;
				 if(j>10) break;
				 }
			if(j>10)
			 	{
			 	DBUF[i]=0xE5;
			 	pDirInfo=(PDIR_INFO)(DBUF+i);
			 	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
			 	//DelayMs(15);
			 	if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
					{
					CHRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;
					}
				//////////////////// 清除FAT中的纪录////////////////////////
				//DelayMs(10);
				if(!DeleteClusterLink(ThisFile.StartCluster))
						{
						CHRspBlock.errcode=ERC_DEVICEERR;
						return FALSE;
						}
			 	bStop=1;
			 	break;
			 	}

			}//end for
		if(bStop==1)
			break;
	       }//end search
	if(sector>=DeviceInfo.BPB_RootEntCnt>>4)
		{
		CHRspBlock.errcode=ERC_FILENOTFOUND;
			return FALSE;
		}
	return TRUE;
#undef RspBlockRemoveFile
}

unsigned char GetCapacity(void)
{
	unsigned int sectorNum,freesectorcnt,i;

#define RspBlockGetCapacity CHRspBlock.RspBlock.Rsp_GetCapacity
	PREAD_CAPACITY_RSP pBuf;

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	if(!ReadCapacity())
	{
	CHRspBlock.errcode=ERC_DEVICEERR;
	return FALSE;
	}
	pBuf=(PREAD_CAPACITY_RSP)DBUF;
	RspBlockGetCapacity.disksize=SwapINT32((pBuf->LastLBA+1)*pBuf->BlockSize);
	sectorNum=DeviceInfo.FatStartSector;
	freesectorcnt=0;
	while(sectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{

		if(RBC_Read(sectorNum,1,DBUF))
		{
		  for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+2)
		  	{
		  	 if((DBUF[i]==0xff)&&(DBUF[i+1]==0xff))
		  	 	{
		  	 	freesectorcnt++;
		  	 	}
		  	}
		}
		else
			{
			CHRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;
			}
		sectorNum++;
	}
	RspBlockGetCapacity.freedisksize=DeviceInfo.BPB_BytesPerSec*DeviceInfo.BPB_SecPerClus;
	RspBlockGetCapacity.freedisksize=freesectorcnt*RspBlockGetCapacity.freedisksize;
	RspBlockGetCapacity.freedisksize=SwapINT32(RspBlockGetCapacity.disksize)-RspBlockGetCapacity.freedisksize;
	RspBlockGetCapacity.freedisksize=SwapINT32(RspBlockGetCapacity.freedisksize);
	return TRUE;
#undef RspBlockGetCapacity
}

unsigned char GetFreeCapacity(void)
{
#define RspBlockGetCapacity CHRspBlock.RspBlock.Rsp_GetFreeCapacity
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	return TRUE;
#undef RspBlockGetFreeCapacity
}

unsigned char SetFilePointer(unsigned long pointer)
{
#define RspBlockSetFilePointer CHRspBlock.RspBlock.Rsp_SetFilePointer

	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	CHRspBlock.errcode=ERC_NODEVICE;
	return FALSE;
	}
	if(!ThisFile.bFileOpen)
	{
	CHRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;
	}
	///////////////////////////////////////////////////////////
	ThisFile.pointer=pointer;
	if(ThisFile.pointer>ThisFile.LengthInByte)
	{
	CHRspBlock.errcode=ERC_LENGTHEXCEED;
	return FALSE;
	}

	if(!GoToPointer(ThisFile.pointer))
	{
	ThisFile.bFileOpen=0;
	CHRspBlock.errcode=ERC_DEVICEERR;
	return FALSE;
	}
	return TRUE;

#undef RspBlockSetFilePointer
}

unsigned char GetFirmwareVersion(void)
{
   #define RspBlockGetVersion CHRspBlock.RspBlock.Rsp_GetVersion
   RspBlockGetVersion.version=0x0101;
   return TRUE;
   #undef RspBlockGetVersion
}


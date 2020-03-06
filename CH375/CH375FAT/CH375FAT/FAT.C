#include "common.h"

////////////////////////////////////////
extern XXGFLAGS bdata bXXGFlags;
extern SYS_INFO_BLOCK xdata DeviceInfo;
extern FILE_INFO xdata ThisFile;
extern unsigned char xdata DBUF[BUFFER_LENGTH];
unsigned char xdata FATBUF[512];
unsigned char xdata CurFatSector[512];
FREE_FAT_INFO xdata FreeFat;
////////////////////////////////////////
unsigned long FirstSectorofCluster(unsigned int clusterNum)
{
	unsigned long temp;
	temp=clusterNum-2;
	temp=temp*DeviceInfo.BPB_SecPerClus;
	temp=temp+DeviceInfo.FirstDataSector;
	return temp;
}

unsigned int ThisFatSecNum(unsigned int clusterNum)
{
   unsigned int temp;
   temp=clusterNum*2;
   temp=temp/DeviceInfo.BPB_BytesPerSec;
   temp=temp+DeviceInfo.FatStartSector;
   return temp;
}

unsigned int ThisFatEntOffset(unsigned int clusterNum)
{
	unsigned int temp1,temp2;
	temp1=2*clusterNum;
	temp2=temp1/DeviceInfo.BPB_BytesPerSec;
	temp1=temp1-temp2*DeviceInfo.BPB_BytesPerSec;
	return temp1;
}

unsigned int GetNextClusterNum(unsigned int clusterNum)
{
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	
	xxgFatSecNum=ThisFatSecNum(clusterNum);
	xxgFatEntOffset=ThisFatEntOffset(clusterNum);
	if(ThisFile.FatSectorPointer!=xxgFatSecNum)
	{	
		
		if(!RBC_Read(xxgFatSecNum,1,FATBUF))
			return 0xFFFF;
		ThisFile.FatSectorPointer=xxgFatSecNum;
	}
	clusterNum=FATBUF[xxgFatEntOffset+1];
	clusterNum=clusterNum<<8;
	clusterNum+=FATBUF[xxgFatEntOffset];	
	return clusterNum;
}

unsigned char DeleteClusterLink(unsigned int clusterNum)
{
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	while((clusterNum>1)&&(clusterNum<0xfff0))
	{
	xxgFatSecNum=ThisFatSecNum(clusterNum);
	xxgFatEntOffset=ThisFatEntOffset(clusterNum);
	if(RBC_Read(xxgFatSecNum,1,DBUF))
		{
		clusterNum=DBUF[xxgFatEntOffset+1];
		clusterNum=clusterNum<<8;
		clusterNum+=DBUF[xxgFatEntOffset];	
		}
	else
		return FALSE;
	DBUF[xxgFatEntOffset]=0x00;
	DBUF[xxgFatEntOffset+1]=0x00;	
	//DelayMs(5);
	if(!RBC_Write(xxgFatSecNum,1,DBUF))
		return FALSE;
	//DelayMs(5);
	if(!RBC_Write(xxgFatSecNum+DeviceInfo.BPB_FATSz16,1,DBUF))
		return FALSE;
	////////////////////////////////////////////
	}
	return TRUE;
}

unsigned char GoToPointer(unsigned long pointer)
{
	unsigned int clusterSize;
	clusterSize=DeviceInfo.BPB_SecPerClus*DeviceInfo.BPB_BytesPerSec;
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	while(pointer>clusterSize)
	{
		pointer-=clusterSize;	
		ThisFile.ClusterPointer=GetNextClusterNum(ThisFile.ClusterPointer);
		if(ThisFile.ClusterPointer==0xffff)
		{
		return FALSE;
		}
	}
	ThisFile.SectorofCluster=pointer/DeviceInfo.BPB_BytesPerSec;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer)+ThisFile.SectorofCluster;
	ThisFile.OffsetofSector=pointer-ThisFile.SectorofCluster*DeviceInfo.BPB_BytesPerSec;
	ThisFile.FatSectorPointer=0;
	return TRUE;
	
}

unsigned int GetFreeCusterNum(void)
{
	unsigned int clusterNum,i;
	unsigned long sectorNum;
	clusterNum=0;
	sectorNum=DeviceInfo.FatStartSector;
	while(sectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{
		
		if(!RBC_Read(sectorNum,1,DBUF))
			return 0x0;
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+2)
		  	{
		  	 if((DBUF[i]==0)&&(DBUF[i+1]==0))
		  	 	{	
		  	 	DBUF[i]=0xff;
		  	 	DBUF[i+1]=0xff;
		  	 	//DelayMs(10);
		  	 	if(!RBC_Write(sectorNum,1,DBUF))
		  	 		return 0x00;
		  	 	//DelayMs(10);
		  	 	if(!RBC_Write(sectorNum+DeviceInfo.BPB_FATSz16,1,DBUF))
		  	 		return 0x00;
		  	 	
		  	 	return	clusterNum; 
		  	 	}
		  	 clusterNum++;
		  	}	
				
		sectorNum=2*clusterNum/DeviceInfo.BPB_BytesPerSec+DeviceInfo.FatStartSector;	
		//DelayMs(10);
	}
	
	return 0x0;
}

unsigned int CreateClusterLink(unsigned int currentCluster)
{
	unsigned char bFound;
	unsigned int clusterNum;
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	unsigned long temp;
	bFound=0;
	//µÚÒ»´Î¶ÁFAT
	if((FreeFat.SectorNum==DeviceInfo.FatStartSector)&&(FreeFat.OffsetofSector<3))
		{	
		if(!RBC_Read(FreeFat.SectorNum,1,CurFatSector))
			return 0x0;	
		}
	temp=FreeFat.SectorNum-DeviceInfo.FatStartSector;
	temp=temp*DeviceInfo.BPB_BytesPerSec;
	temp=temp/2;
	clusterNum=temp+FreeFat.OffsetofSector/2;
	while(FreeFat.SectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{
		while(FreeFat.OffsetofSector<DeviceInfo.BPB_BytesPerSec)
		  	{
		  	 if((CurFatSector[FreeFat.OffsetofSector]==0)&&(CurFatSector[FreeFat.OffsetofSector+1]==0))
		  	 	{	
		  	 	CurFatSector[FreeFat.OffsetofSector]=0xff;
		  	 	CurFatSector[FreeFat.OffsetofSector+1]=0xff;
		  	 	
		  	 	FreeFat.OffsetofSector=FreeFat.OffsetofSector+2;
		  	 	bXXGFlags.bits.bFatChanged=1;
		  	 	bFound=1;
		  	 	break;
		  	 	}
		  	 FreeFat.OffsetofSector=FreeFat.OffsetofSector+2;
		  	 clusterNum++;
		  	}	
		if(bFound==1)
			break;	
		//FreeFat.SectorNum=2*clusterNum/DeviceInfo.BPB_BytesPerSec+DeviceInfo.FatStartSector;	
		//FreeFat.OldSectorNum=FreeFat.SectorNum;
		//FreeFat.SectorNum++;
		//FreeFat.OffsetofSector=0;
		UpdateFat(FreeFat.SectorNum);
		
		FreeFat.SectorNum++;
		FreeFat.OffsetofSector=0;
		
		if(!RBC_Read(FreeFat.SectorNum,1,CurFatSector))
			return 0x0;	
		//DelayMs(10);
	}
	if(bFound==0)
		return 0x00;
	xxgFatSecNum=ThisFatSecNum(currentCluster);
	xxgFatEntOffset=ThisFatEntOffset(currentCluster);
	
	if(xxgFatSecNum!=FreeFat.SectorNum)
	  {
		RBC_Read(xxgFatSecNum,1,DBUF);
	//FreeFat.OffsetofSector=FreeFat.OffsetofSector+2;
		DBUF[xxgFatEntOffset]=clusterNum;
		DBUF[xxgFatEntOffset+1]=clusterNum>>8;
		//DelayMs(5);
		if(!RBC_Write(xxgFatSecNum,1,DBUF))
			return 0x00;
		//DelayMs(10);
		if(!RBC_Write(xxgFatSecNum+DeviceInfo.BPB_FATSz16,1,DBUF))
			return 0x00;
	   }
	 else
	   {
	   	CurFatSector[xxgFatEntOffset]=clusterNum;
		CurFatSector[xxgFatEntOffset+1]=clusterNum>>8;
		bXXGFlags.bits.bFatChanged=1;
	   }	
	return clusterNum;
}

void UpdateFat(unsigned long sectorNum)
{
	if(bXXGFlags.bits.bFatChanged==1)
			{
			if(!RBC_Write(sectorNum,1,CurFatSector))
				return ;
			//DelayMs(10);
			if(!RBC_Write(sectorNum+DeviceInfo.BPB_FATSz16,1,CurFatSector))
				return ;
			bXXGFlags.bits.bFatChanged=0;
			}
}


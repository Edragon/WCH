typedef struct _MBR_BLOCK {
  INT8 Res[454];
  unsigned long StartSector;
  unsigned long TotalSector;
  INT8 Res1[50];
} MBR_BLOCK,* PMBR_BLOCK;

typedef struct _BPB_BLOCK {
  INT8 BS_jmpBoo[3];
  INT8 BS_OEMName[8];
  INT16 BPB_BytesPerSec;
  INT8 BPB_SecPerClus;
  INT8 BPB_RsvdSecCn[2];
  INT8 BPB_NumFATs;
  INT16 BPB_RootEntCnt;
  INT16 BPB_TotSec16;
  INT8 BPB_Media;
  INT16 BPB_FATSz16;
  INT16 BPB_SecPerTrk;
  INT16 BPB_NumHeads;
  unsigned long BPB_HiddSec;
  unsigned long BPB_TotSec32;
  INT8 BS_DrvNum;
  INT8 BS_Reserved1;
  INT8 BS_BootSig;
  INT8 BS_VolID[4];
  INT8 BS_VolLab[11];
  INT8 BS_FilSysType[8];
  INT8 ExecutableCode[448];
  INT8 Marker[2];
} BPB_BLOCK,* PBPB_BLOCK;

typedef struct _SYS_INFO_BLOCK{
  unsigned long StartSector;
  unsigned long TotalSector;
  //INT8 BS_jmpBoo[3];
  //INT8 BS_OEMName[8];
  INT16 BPB_BytesPerSec;
  INT8 BPB_SecPerClus;
  //INT8 BPB_RsvdSecCn[2];
  INT8 BPB_NumFATs;
  INT16 BPB_RootEntCnt;
  INT16 BPB_TotSec16;
  INT8 BPB_Media;
  INT16 BPB_FATSz16;
  INT16 BPB_SecPerTrk;
  INT16 BPB_NumHeads;
  unsigned long BPB_HiddSec;
  unsigned long BPB_TotSec32;
  INT8 BS_DrvNum;
  //INT8 BS_Reserved1;
  INT8 BS_BootSig;
  INT8 BS_VolID[4];
  INT8 BS_VolLab[11];
  INT8 BS_FilSysType[8];
  //INT8 ExecutableCode[448];
  //INT8 Marker[2];
  ///////////////////////////////
  unsigned long FatStartSector;
  unsigned long RootStartSector;
  //unsigned long DataStartSector;
  unsigned long FirstDataSector;
  //unsigned long FirstSectorofCluster;
} SYS_INFO_BLOCK,* PSYS_INFO_BLOCK;

typedef struct _FILE_INFO{
  unsigned char bFileOpen;
  unsigned int StartCluster;
  unsigned long LengthInByte;
  unsigned int ClusterPointer;
  unsigned long SectorPointer;
  unsigned int OffsetofSector;
  unsigned char SectorofCluster;
  unsigned long pointer;
  unsigned int	FatSectorPointer;
} FILE_INFO, * PFILE_INFO;

typedef struct _FREE_FAT_INFO{
  unsigned long SectorNum;
  unsigned int OffsetofSector;
  unsigned long OldSectorNum;
} FREE_FAT_INFO, * PFREE_FAT_INFO;

typedef struct _DIR_INFO{
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attribute;
	unsigned char Reserved[10];
	unsigned int lastUpdateDate;
	unsigned int lastUpdateTime;
	unsigned int startCluster;
	unsigned long length;
} DIR_INFO,* PDIR_INFO;

unsigned long FirstSectorofCluster(unsigned int n);
unsigned int ThisFatSecNum(unsigned int clusterNum);
unsigned int ThisFatEntOffset(unsigned int clusterNum);
unsigned int GetNextClusterNum(unsigned int clusterNum);
unsigned int GetClusterNumFromSectorNum(unsigned long sectorNum);
//unsigned long GetSecNumFromPointer(void);
unsigned char GoToPointer(unsigned long pointer);
unsigned int GetFreeCusterNum(void);
unsigned int CreateClusterLink(unsigned int currentCluster);
unsigned char DeleteClusterLink(unsigned int clusterNum);
void UpdateFat(unsigned long sectorNum);

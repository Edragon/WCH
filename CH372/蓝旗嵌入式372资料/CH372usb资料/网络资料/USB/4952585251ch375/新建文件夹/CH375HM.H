/*
****************************************
**  Copyright  (C)  W.ch  1999-2007   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Module      @CH375  **
****************************************
*/
/* CH375 主机文件系统接口模块 */
/* 支持: FAT12/FAT16/FAT32 */
/* U盘文件读写模块C语言头文件 */

#ifndef __CH375HM_H__
#define __CH375HM_H__

#define CH375HM_VER				0x30	/* 当前模块版本 */

/* ********************************************************************************************************************* */
/* 返回的操作状态码 */
/* 以下是事件通知状态码,检测到当前U盘已经连接或者已经断开,可以作为最终状态码 */
#define	ERR_USB_CONNECT			0x15	/* 检测到USB设备连接事件,磁盘已经连接 */
#define ERR_DISK_DISCON			0x82	/* 检测到USB设备连接事件,磁盘已经断开,或者磁盘尚未连接 */
/* 代码0XH用于USB设备方式的操作状态代码,由CH375返回,是USB设备模式的中断状态 */
#define	ERR_USB_DAT_DOWN		0x02	/* USB设备模式: 数据下传成功,上位机下传的数据已经在模块中 */
#define	ERR_USB_DAT_UP			0x0A	/* USB设备模式: 数据上传成功,模块中的上传数据已经被上位机取走 */
/* 以下是阶段性的状态码,用于请求数据交换,不是最终的操作状态 */
#define	USB_INT_DISK_READ		0x1D	/* USB存储器读数据块,只用于CMD_FileRead命令,请求数据读出 */
#define	USB_INT_DISK_WRITE		0x1E	/* USB存储器写数据块,只用于CMD_FileWrite命令,请求数据写入 */
#define	USB_INT_DISK_RETRY		0xEE	/* USB存储器读写数据块失败重试,只用于CMD_FileRead命令和CMD_FileRead命令,操作失败重试 */
/* USB_INT_DISK_RETRY重试状态码要求用户端程序回改文件数据缓冲区指针,以便重新发送或者接收文件数据,
   在USB_INT_DISK_RETRY重试状态码之后用户端程序将收到两个字节的数据,也就是一个16位的数据,指定需要回改指针的字节数,大端时高字节在前,小端时低字节在前 */
/* 最终状态码 */
#define ERR_SUCCESS				0x00	/* 操作成功 */
#define ERR_CH375_ERROR			0x81	/* CH375硬件错误,可能需要复位CH375 */
#define ERR_STATUS_ERR			0x83	/* 磁盘状态错误,可能正在连接或者断开磁盘 */
#define ERR_MBR_ERROR			0x91	/* 磁盘的主引导记录无效,可能磁盘尚未分区或者尚未格式化 */
#define ERR_TYPE_ERROR			0x92	/* 磁盘分区类型不支持,只支持FAT12/FAT16/BigDOS/FAT32,需要由磁盘管理工具重新分区 */
#define ERR_BPB_ERROR			0xA1	/* 磁盘尚未格式化,或者参数错误,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_TOO_LARGE			0xA2	/* 磁盘非正常格式化并且容量大于4GB,或者容量大于250GB,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_FAT_ERROR			0xA3	/* 磁盘的文件系统不支持,只支持FAT12/FAT16/FAT32,需要由WINDOWS采用默认参数重新格式化 */
#define ERR_DISK_FULL			0xB1	/* 磁盘文件太满,剩余空间太少或者已经没有,需要磁盘整理 */
#define ERR_FDT_OVER			0xB2	/* 目录内文件太多,没有空闲的目录项,FAT12/FAT16根目录下的文件数应该少于500个,需要磁盘整理 */
#define ERR_MISS_DIR			0xB3	/* 指定路径的某个子目录没有找到,可能是目录名称错误 */
#define ERR_FILE_CLOSE			0xB4	/* 文件已经关闭,如果需要使用,应该重新打开文件 */
#define ERR_OPEN_DIR			0x41	/* 指定路径的目录被打开 */
#define ERR_MISS_FILE			0x42	/* 指定路径的文件没有找到,可能是文件名称错误 */
#define ERR_FOUND_NAME			0x43	/* 搜索到与通配符相匹配的文件名,文件名及其完整路径在命令缓冲区中,如果需要使用,应该打开该文件 */
#define	ERR_USB_DISK_ERR		0x1F	/* USB存储器操作失败,在初始化时可能是USB存储器不支持,在读写操作中可能是磁盘损坏或者已经断开 */
/* 其余错误代码未定义,可以发出命令CMD_QueryStatus查询模块状态并分析 */

/* ********************************************************************************************************************* */
/* 磁盘及文件状态,适用于CMD_PARAM.Status.mDiskStatus */
#define DISK_UNKNOWN			0x00	/* 尚未初始化,未知状态 */
#define DISK_DISCONNECT			0x01	/* 磁盘没有连接或者已经断开 */
#define DISK_CONNECT			0x02	/* 磁盘已经连接,但是尚未初始化或者无法识别该磁盘 */
#define DISK_MOUNTED			0x03	/* 磁盘已经初始化成功,但是尚未分析文件系统或者文件系统不支持 */
#define DISK_READY				0x10	/* 已经分析磁盘的文件系统并且能够支持 */
#define DISK_OPEN_ROOT			0x12	/* 已经打开根目录,扇区模式,只能以扇区为单位读写目录的内容,使用后必须关闭,注意FAT12/FAT16根目录是固定长度 */
#define DISK_OPEN_DIR			0x13	/* 已经打开子目录,扇区模式,只能以扇区为单位读写目录的内容 */
#define DISK_OPEN_FILE			0x14	/* 已经打开文件,扇区模式,可以以扇区为单位进行数据读写 */
#define DISK_OPEN_FILE_B		0x15	/* 已经打开文件,字节模式,可以以字节为单位进行数据读写 */

/* ********************************************************************************************************************* */
/* FAT类型标志,适用于CMD_PARAM.Query.mDiskFat */
#define DISK_FS_UNKNOWN			0		/* 未知的文件系统 */
#define DISK_FAT12				1		/* FAT12文件系统 */
#define DISK_FAT16				2		/* FAT16文件系统 */
#define DISK_FAT32				3		/* FAT32文件系统 */

/* ********************************************************************************************************************* */
/* 文件属性,适用于CMD_PARAM.Modify.mFileAttr */
#define ATTR_READ_ONLY			0x01	/* 文件为只读属性 */
#define ATTR_HIDDEN				0x02	/* 文件为隐含属性 */
#define ATTR_SYSTEM				0x04	/* 文件为系统属性 */
#define ATTR_VOLUME_ID			0x08	/* 卷标 */
#define ATTR_DIRECTORY			0x10	/* 子目录 */
#define ATTR_ARCHIVE			0x20	/* 文件为存档属性 */
#define ATTR_LONG_NAME			( ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID )
/* 文件属性 unsigned char */
/* bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7 */
/*  只   隐   系   卷   目   存   未定义   */
/*  读   藏   统   标   录   档            */
/* 文件时间 unsigned short,适用于CMD_PARAM.Modify.mFileTime */
/* Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	/* 生成指定时分秒的文件时间数据 */
/* 文件日期 unsigned short,适用于CMD_PARAM.Modify.mFileDate */
/* Date = ((Year-1980)<<9) + (Month<<5) + Day */
#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	/* 生成指定年月日的文件日期数据 */

/* ********************************************************************************************************************* */
/* 文件名,适用于CMD_PARAM.?.mPathName */
#define PATH_WILDCARD_CHAR		0x2A	/* 路径名的通配符 '*' */
#define PATH_SEPAR_CHAR1		0x5C	/* 路径名的分隔符 '\' */
#define PATH_SEPAR_CHAR2		0x2F	/* 路径名的分隔符 '/' */
#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN			65		/* 最大路径长度,含所有斜杠分隔符和小数点间隔符以及路径结束符00H,并口方式下最大值为62 */
#endif
#if		MAX_PATH_LEN		>	65
#undef	MAX_PATH_LEN
#define MAX_PATH_LEN			65
#endif
#ifndef MAX_BYTE_IO
#define MAX_BYTE_IO				( MAX_PATH_LEN - 1 )	/* 以字节为单位单次读写文件时的最大长度,超过该长度可以分多次读写,并口方式下最大值为61 */
#endif
#if		MAX_BYTE_IO			>	64
#undef	MAX_BYTE_IO
#define	MAX_BYTE_IO				64
#endif

/* ********************************************************************************************************************* */
/* 外部命令码 */
#define	CMD_QueryStatus			0x60	/* 查询当前模块的状态 */
#define	CMD_DiskQuery			0x61	/* 查询磁盘信息 */
#define	CMD_FileEnumer			0x63	/* 枚举文件,返回文件名 */
#define	CMD_FileOpen			0x64	/* 打开文件 */
#define	CMD_FileCreate			0x65	/* 新建文件并打开,如果文件已经存在则先删除后再新建 */
#define	CMD_FileErase			0x66	/* 删除文件并关闭 */
#define	CMD_FileClose			0x67	/* 关闭当前文件 */
#define	CMD_FileQuery			0x68	/* 查询当前文件的信息 */
#define	CMD_FileModify			0x69	/* 查询或者修改当前文件的信息 */
#define	CMD_FileLocate			0x6A	/* 移动当前文件指针 */
#define	CMD_FileRead			0x6B	/* 从当前文件读取数据 */
#define	CMD_FileWrite			0x6C	/* 向当前文件写入数据 */
#define	CMD_ByteLocate			0x7A	/* 以字节为单位移动当前文件指针 */
#define	CMD_ByteRead			0x7B	/* 以字节为单位从当前文件读取数据块 */
#define	CMD_ByteWrite			0x7C	/* 以字节为单位向当前文件写入数据块 */
#define	CMD_BulkOnlyCmd			0x70	/* 执行基于BulkOnly协议的命令 */
#define	CMD_DiskReady			0x71	/* 查询磁盘是否准备好 */
#define	CMD_DiskSize			0x72	/* 查询磁盘容量 */
#define	CMD_FileDirInfo			0x75	/* 存取当前已打开文件的目录信息 */
#define	CMD_DirCreate			0x76	/* 新建目录并打开,如果目录已经存在则直接打开 */
#define	CMD_FileReadLast		0x77	/* 从当前文件的尾部读取不足一个扇区长度的零碎数据 */
#define	CMD_SetFileSize			0x78	/* 修改模块系统内子程序库的文件长度变量 */
#define	CMD_StreamRead			0x7E	/* 数据流模式读文件,只支持串口,只支持文本 */
#define	CMD_StreamWrite			0x7F	/* 数据流模式写文件,只支持串口,只支持文本 */
/* USB设备方式命令码 */
#define	CMD_SetUsbId			0x12	/* USB设备模式: 设置USB设备的厂商VID和产品PID */
#define	CMD_SetUsbMode			0x15	/* USB设备模式: 设置USB主机/设备工作模式,只支持串口 */
#define	CMD_ReadUsbData			0x28	/* USB设备模式: 从模块的数据下传端点读取数据块 */
#define	CMD_WriteUsbData		0x2B	/* USB设备模式: 向模块的数据上传端点写入数据块 */
/* 辅助命令码 */
#define	CMD_GetVer				0x0A	/* 获取当前模块的版本号 */
#define	CMD_ResetInit			0x0B	/* 复位并重新初始化CH375以及模块 */
#define	CMD_BaudRate			0xA5	/* 设置串口通讯波特率 */
#define	CMD_SetupModule			0xA6	/* 设置模块配置 */
#define	CMD_DirectWrCmd			0xB9	/* 直接传递给CH375,写命令 */
#define	CMD_DirectRdDat			0xB5	/* 直接传递给CH375,读数据 */
#define	CMD_DirectWrDat			0xB6	/* 直接传递给CH375,写数据 */
#define	CMD_GetStringSN			0xA0	/* 获取产品序列号字符串 */

/* ********************************************************************************************************************* */
/* 外部命令参数 */
typedef union _CMD_PARAM {
	struct {
		unsigned char	mBuffer[ MAX_PATH_LEN ];
	} Other;
	struct {
		unsigned long	mDiskSizeSec;			/* 返回: 整个物理磁盘的总扇区数 */
		unsigned long	mTotalSector;			/* 返回: 当前逻辑盘的总扇区数 */
		unsigned long	mFreeSector;			/* 返回: 当前逻辑盘的剩余扇区数 */
		unsigned char	mDiskFat;				/* 返回: 当前逻辑盘的FAT类型 */
	} Query;									/* CMD_DiskQuery, 查询磁盘信息 */
	struct {
		unsigned char	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Open;										/* CMD_FileOpen, 打开文件 */
	struct {
		unsigned char	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名(含通配符*)...,枚举序号], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILE*",00H */
	} Enumer;									/* CMD_FileEnumer, 枚举文件,返回文件名 */
	struct {
		unsigned char	mUpdateLen;				/* 输入参数: 是否允许更新长度: 0禁止,1允许 */
	} Close;									/* CMD_FileClose, 关闭当前文件 */
	struct {
		unsigned char	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Create;									/* CMD_FileCreate, 新建文件并打开,如果文件已经存在则先删除后再新建 */
	struct {
		unsigned char	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} Erase;									/* CMD_FileErase, 删除文件并关闭 */
	struct {
		unsigned long	mFileSize;				/* 输入参数: 新的文件长度,为0FFFFFFFFH则不修改, 返回: 原长度 */
		unsigned short	mFileDate;				/* 输入参数: 新的文件日期,为0FFFFH则不修改, 返回: 原日期 */
		unsigned short	mFileTime;				/* 输入参数: 新的文件时间,为0FFFFH则不修改, 返回: 原时间 */
		unsigned char	mFileAttr;				/* 输入参数: 新的文件属性,为0FFH则不修改, 返回: 原属性 */
	} Modify;									/* CMD_FileQuery, 查询当前文件的信息; CMD_FileModify, 查询或者修改当前文件的信息 */
	struct {
		unsigned long	mSectorOffset;			/* 输入参数: 扇区偏移,0则移动到文件头,0FFFFFFFFH则移动到文件尾, 返回: 当前文件指针对应的绝对线性扇区号, 0FFFFFFFFH则已到文件尾 */
	} Locate;									/* CMD_FileLocate, 移动当前文件指针 */
	struct {
		unsigned char	mSectorCount;			/* 输入参数: 读取扇区数, 返回: 实际读取扇区数 */
	} Read;										/* CMD_FileRead, 从当前文件读取数据 */
	struct {
		unsigned char	mSectorCount;			/* 输入参数: 写入扇区数, 返回: 实际写入扇区数 */
	} Write;									/* CMD_FileWrite, 向当前文件写入数据 */
	struct {
		unsigned long	mDiskSizeSec;			/* 返回: 整个物理磁盘的总扇区数 */
	} DiskSize;									/* CMD_DiskSize, 查询磁盘容量 */
	struct {
		unsigned long	mByteOffset;			/* 输入参数: 以字节为单位的偏移量, 以字节为单位的文件指针, 返回: 当前文件指针对应的绝对线性扇区号, 0FFFFFFFFH则已到文件尾 */
	} ByteLocate;								/* CMD_ByteLocate, 以字节为单位移动当前文件指针 */
	struct {
		unsigned char	mByteCount;				/* 输入参数: 准备读取的字节数,不得大于MAX_BYTE_IO, 返回: 实际读出的字节数 */
		unsigned char	mByteBuffer[ MAX_BYTE_IO ];	/* 返回: 读出的数据块 */
	} ByteRead;									/* CMD_ByteRead, 以字节为单位从当前文件读取数据块 */
	struct {
		unsigned char	mByteCount;				/* 输入参数: 准备写入的字节数,不得大于MAX_BYTE_IO,如果为0则刷新文件长度而不写入,如果不为0则写入数据但不刷新文件长度, 返回: 实际写入的字节数 */
		unsigned char	mByteBuffer[ MAX_BYTE_IO ];	/* 输入参数: 准备写入的数据块 */
	} ByteWrite;								/* CMD_ByteWrite, 以字节为单位向当前文件写入数据块 */
	union {
		struct {
			unsigned long	mCBW_Sig;
			unsigned long	mCBW_Tag;
			unsigned char	mCBW_DataLen;		/* 输入: 数据传输长度,有效值是0到255 */
			unsigned char	mCBW_DataLen1;
			unsigned char	mCBW_DataLen2;
			unsigned char	mCBW_DataLen3;
			unsigned char	mCBW_Flag;			/* 输入: 传输方向等标志 */
			unsigned char	mCBW_LUN;
			unsigned char	mCBW_CB_Len;		/* 输入: 命令块的长度,有效值是1到16 */
			unsigned char	mCBW_CB_Buf[1];		/* 输入: 命令块,该缓冲区最多为16个字节 */
		} mCBW;									/* BulkOnly协议的命令块, 输入CBW结构 */
		struct {
			unsigned long	mCSW_Sig;
			unsigned long	mCSW_Tag;
			unsigned long	mCSW_Residue;		/* 返回: 剩余数据长度 */
			unsigned char	mCSW_Status;		/* 返回: 命令执行结果状态 */
			unsigned char	mReserved;
		} mCSW;									/* BulkOnly协议的命令状态块, 输出CSW结构 */
	} BOC;										/* CMD_BulkOnlyCmd, 执行基于BulkOnly协议的命令, 如果有数据传输那么数据在DISK_BASE_BUF中 */
	struct {
		unsigned char	mPathName[ MAX_PATH_LEN ];	/* 输入参数: 路径: [盘符,冒号,斜杠,目录名或者文件名及扩展名...,结束符00H], 其中盘符和冒号可以省略, 例如"C:\DIR1.EXT\DIR2\FILENAME.EXT",00H */
	} DirCreate;								/* CMD_DirCreate, 新建目录并打开,如果目录已经存在则直接打开 */
	struct {
		unsigned char	mSectorCount;			/* 返回: 实际读取扇区数,为1则已经读取最后扇区,为0则没有零碎数据(文件长度是512的倍数) */
	} ReadLast;									/* CMD_FileReadLast, 从当前文件的尾部读取不足一个扇区长度的零碎数据 */
	struct {
		unsigned char	mAccessMode;			/* 输入: 存取方式, 0=读取文件目录信息,0F0H=写入/更新文件目录信息,其它值无效 */
		unsigned char	mReserved[3];
		struct {								/* 该结构中的成员输入输出必须都是小端格式,如果外部单片机是大端格式,那么需要自行处理大小端格式转换 */
			unsigned char	DIR_Name[11];		/* 00H,文件名,共11字节,不足处填空格 */
			unsigned char	DIR_Attr;			/* 0BH,文件属性,参考下面的说明 */
/* bit0  bit1  bit2  bit3  bit4  bit5  bit6 bit7 */
/* 只读  隐藏  系统  卷标  目录  存档  未定义    */
			unsigned char	DIR_NTRes;			/* 0CH */
			unsigned char	DIR_CrtTimeTenth;	/* 0DH,文件创建的时间,以0.1秒单位计数 */
			unsigned short	DIR_CrtTime;		/* 0EH,文件创建的时间,参考前面的宏MAKE_FILE_TIME,小端格式 */
			unsigned short	DIR_CrtDate;		/* 10H,文件创建的日期,参考前面的宏MAKE_FILE_DATE,小端格式 */
			unsigned short	DIR_LstAccDate;		/* 12H,最近一次存取操作的日期,参考前面的宏MAKE_FILE_DATE,小端格式 */
			unsigned short	DIR_FstClusHI;		/* 14H */
			unsigned short	DIR_WrtTime;		/* 16H,文件修改时间,参考下面的说明和前面的宏MAKE_FILE_TIME,小端格式 */
/* 文件时间 UINT16: Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
			unsigned short	DIR_WrtDate;		/* 18H,文件修改日期,参考下面的说明和前面的宏MAKE_FILE_DATE,小端格式 */
/* 文件日期 UINT16: Date = ((Year-1980)<<9) + (Month<<5) + Day */
			unsigned short	DIR_FstClusLO;		/* 1AH */
			unsigned long	DIR_FileSize;		/* 1CH,文件长度,小端格式 */
		} mDir;									/* 20H */
	} FileDirInfo;								/* CMD_FileDirInfo, 存取当前已打开文件的目录信息 */
	struct {
		unsigned char	mLastStatus;			/* 返回: 上次的操作状态 */
		unsigned char	mDiskStatus;			/* 返回: 磁盘及文件状态 */
		unsigned char	mIntStatus;				/* 返回: CH375操作的中断状态 */
		unsigned char	reserved;
		unsigned long	mFileSize;				/* 返回: 当前文件的长度 */
		unsigned long	mCurrentOffset;			/* 返回: 当前文件指针,当前读写位置的字节偏移 */
	} Status;									/* CMD_QueryStatus, 查询当前模块的状态 */
	struct {
		unsigned long	mFileSize;				/* 输入: 指定新的文件长度变量,只修改子程序库的变量,只在更新文件长度后影响实际文件长度 */
	} SetFileSize;								/* CMD_SetFileSize, 修改模块系统内子程序库的文件长度变量 */
	struct {
		unsigned char	mFileLibVer;			/* 返回: 子程序库的版本号 */
		unsigned char	mModuleVer;				/* 返回: 模块的版本号 */
		unsigned char	mUsbIcVer;				/* 返回: USB芯片版本:10H-CH375S,2xH-CH375A */
	} GetVer;									/* CMD_GetVer, 获取当前模块的版本号 */
	struct {
		unsigned char	mDivisor;				/* 输入参数: 通讯波特率除数 */
	} BaudRate;									/* CMD_BaudRate, 设置串口通讯波特率 */
	struct {
		unsigned char	mSetup;					/* 输入参数: 模块配置值 */
/* 位7位6: 数据字节顺序: 00-保持当前格式, 01-设置小端格式LITTLE_ENDIAN, 10-设置大端格式BIG_ENDIAN, 11-保留/保持当前格式 */
/* 位4: 向文件写入数据后自动更新文件长度(设置CH375LibConfig的位4):0禁止,1允许 */
/* 位0: 空闲时查询U盘连接状态并自动中断通知:0禁止,1允许 */
	} Setup;									/* CMD_SetupModule, 设置模块配置 */
	struct {
		unsigned char	mData;					/* 输入参数或者返回: 字节数据 */
	} Direct;									/* CMD_DirectWrCmd, 直接传递给CH375,写命令; CMD_DirectRdDat, 直接传递给CH375,读数据; CMD_DirectWrDat, 直接传递给CH375,写数据 */
	struct {
		unsigned char	mStringSN[16];			/* 返回: 产品序列号字符串 */
	} GetSN;									/* CMD_GetStringSN, 获取产品序列号字符串 */
	struct {
		unsigned char	mVidLow;				/* 输入参数: 厂商ID低字节 */
		unsigned char	mVidHigh;				/* 输入参数: 厂商ID高字节 */
		unsigned char	mPidLow;				/* 输入参数: 产品ID低字节 */
		unsigned char	mPidHigh;				/* 输入参数: 产品ID高字节 */
	} SetUsbId;									/* CMD_SetUsbId, USB设备模式: 设置USB设备的厂商VID和产品PID */
	struct {
		unsigned char	mUsbMode;				/* 输入参数: 工作模式代码, 0=空闲, 2=USB设备模式, 6=USB主机模式 */
	} SetUsbMode;								/* CMD_SetUsbMode, USB设备模式: 设置USB主机/设备工作模式,只支持串口 */
	struct {
		unsigned char	mByteCount;				/* 返回: 读出的字节数 */
		unsigned char	mByteBuffer[ MAX_BYTE_IO ];	/* 返回: 读出的数据块 */
	} ReadUsbData;								/* CMD_ReadUsbData, USB设备模式: 从模块的数据下传端点读取数据块 */
	struct {
		unsigned char	mByteCount;				/* 输入参数: 准备写入的字节数,不得大于MAX_BYTE_IO,不得大于CH375_MAX_DATA_LEN */
		unsigned char	mByteBuffer[ MAX_BYTE_IO ];	/* 输入参数: 准备写入的数据块 */
	} WriteUsbData;								/* CMD_WriteUsbData, USB设备模式: 向模块的数据上传端点写入数据块 */
} CMD_PARAM;

/* ********************************************************************************************************************* */
/* 并口CH421A芯片的空间定义 */
#define	PARA_COMMAND_ADDR		0x40	/* 命令码的单元地址,该单元为操作命令码,该单元由单片机在操作前设置,由模块在操作完成后清除,查询该单元可代替查询INT#引脚 */
#define	PARA_STATUS_ADDR		0x41	/* 状态码的单元地址,该单元为操作结果状态码 */
#define	PARA_CMD_LEN_ADDR		0x42	/* 命令包长度的单元地址,该单元低7位为后续参数的长度,最高位用于通知模块,为1则激活/继续,为0则结束/应答 */
#define	PARA_STS_LEN_ADDR		0x43	/* 状态包长度的单元地址,该单元低7位为后续参数的长度 */
#define	PARA_CMD_BIT_ACT		0x80	/* 命令包长度的最高位,用于通知模块,激活/继续 */
#define	PARA_CMD_BIT_INACT		0x00	/* 命令包长度的最高位,用于通知模块,结束/应答 */
#define	PARA_BUFFER_ADDR		0x00	/* 后续参数的起始单元地址,从该单元开始的64个字节为数据缓冲区 */

/* ********************************************************************************************************************* */
/* 三线串口的命令同步,用两个串口同步码代替STA#引脚的下降沿进行命令码同步 */
#define	SER_SYNC_CODE1			0x57	/* 启动操作的第1个串口同步码 */
#define	SER_SYNC_CODE2			0xAB	/* 启动操作的第2个串口同步码 */

/* ********************************************************************************************************************* */
/* 数据流模式读写文件,只支持串口,只支持文本,默认文件名是"/模块数据.TXT" */
#define	SER_STREAM_END			0x1A	/* 数据流模式文件读写:结束 */
#define	SER_STREAM_ERROR		0x15	/* 数据流模式文件读写:错误 */
#define	SER_STREAM_ACK			0x06	/* 数据流模式文件读写:应答 */
#define	SER_STREAM_FLUSH		0x00	/* 数据流模式文件读写:刷新 */

/* ********************************************************************************************************************* */
/* 模块的预设说明,用于功能配置 */
/* 程序空间3F00H开始的4个字节必须是"$HMS",说明下面的预设参数有效 */
/* 程序空间3F08H字节是工作模式: */
/*                              位7: 数据字节顺序: 0-小端LITTLE_ENDIAN,1-大端BIG_ENDIAN */
/*                              位6: 保留,必须为0 */
/*                              位5: 空闲时查询U盘连接状态并自动中断通知:0禁止,1允许 */
/*                              位4: 向文件写入数据后自动更新文件长度:0禁止,1允许 */
/*                              位3: 保留,必须为0 */
/*                              位2: 看门狗使能/串口超时使能: 0-禁止超时检查, 1-允许超时检查 */
/*                              位1位0: 接口方式: 00-8位并口, 01-专用接口/定制I2C或SPI等,10-4+1线制串口,11-3线制串口 */
/* 程序空间3F09H字节是串口通讯波特率除数 */
/* 程序空间3F10H开始的16个字节是产品序列号字符串 */

/* ********************************************************************************************************************* */

#endif

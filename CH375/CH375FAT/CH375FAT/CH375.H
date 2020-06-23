
/* �������ݸ�����CH375INC.H�ļ� */

/* Define for CH372 & CH375          */
/* Website:  http://winchiphead.com  */
/* Email:    tech@winchiphead.com    */
/* Author:   W.ch 2003.09            */

/* ********************************************************************************************************************* */
/* Ӳ������ */

#define	CH375_MAX_DATA_LEN	0x40			/* ������ݰ��ĳ���, �ڲ��������ĳ��� */

/* ********************************************************************************************************************* */
/* ������� */

#define	CMD_RESET_ALL		0x05			/* ִ��Ӳ����λ */

#define	CMD_CHECK_EXIST		0x06			/* ���Թ���״̬ */
/* ����: �������� */
/* ���: �������ݵİ�λȡ�� */

#define	CMD_SET_USB_ID		0x12			/* �豸��ʽ: ����USB����VID�Ͳ�ƷPID */
/* ����: ����ID���ֽ�, ����ID���ֽ�, ��ƷID���ֽ�, ��ƷID���ֽ� */

#define	CMD_SET_USB_ADDR	0x13			/* ����USB��ַ */
/* ����: ��ֵַ */

#define	CMD_SET_USB_MODE	0x15			/* ����USB����ģʽ */
/* ����: ģʽ���� */
/*       00H=δ���õ��豸��ʽ, 01H=�����õ��豸��ʽ����ʹ���ⲿ�̼�ģʽ, 02H=�����õ��豸��ʽ����ʹ�����ù̼�ģʽ, 03H=�����õ��豸��ʽ����ʹ���ж϶˵�����ù̼�ģʽ */
/*       04H=δ���õ�������ʽ, 05H=�����õ�������ʽ, 06H=�����õ�������ʽ�����Զ�����SOF��, 07H=�����õ�������ʽ���Ҹ�λUSB���� */
/* ���: ����״̬( CMD_RET_SUCCESS��CMD_RET_ABORT, ����ֵ˵������δ��� ) */

#define	CMD_SET_ENDP2		0x18			/* �豸��ʽ: ����USB�˵�0�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP3		0x19			/* �豸��ʽ: ����USB�˵�0�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000~1000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP4		0x1A			/* �豸��ʽ: ����USB�˵�1�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP5		0x1B			/* �豸��ʽ: ����USB�˵�1�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000~1000-����ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP6		0x1C			/* ����USB�˵�2/�����˵�Ľ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1101-������������ACK, 1110-��æNAK, 1111-����STALL */

#define	CMD_SET_ENDP7		0x1D			/* ����USB�˵�2/�����˵�ķ����� */
/* ����: ������ʽ */
/*           λ7Ϊ1��λ6Ϊͬ������λ, ����ͬ������λ���� */
/*           λ3~λ0Ϊ������Ӧ��ʽ:  0000-����ACK, 1101-����������Ӧ��, 1110-��æNAK, 1111-����STALL */

#define	CMD_GET_TOGGLE		0x0A			/* ��ȡOUT�����ͬ��״̬ */
/* ����: ����1AH */
/* ���: ͬ��״̬ */
/*           λ4Ϊ1��OUT����ͬ��, ����OUT����ͬ�� */

#define	CMD_GET_STATUS		0x22			/* ��ȡ�ж�״̬��ȡ���ж����� */
/* ���: �ж�״̬ */

#define	CMD_UNLOCK_USB		0x23			/* �豸��ʽ: �ͷŵ�ǰUSB������ */

#define	CMD_RD_USB_DATA		0x28			/* �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ�, ���ͷŻ����� */
/* ���: ����, ������ */

#define	CMD_WR_USB_DATA3	0x29			/* �豸��ʽ: ��USB�˵�0�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

#define	CMD_WR_USB_DATA5	0x2A			/* �豸��ʽ: ��USB�˵�1�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

#define	CMD_WR_USB_DATA7	0x2B			/* ��USB�˵�2�ķ��ͻ�����д�����ݿ� */
/* ����: ����, ������ */

/* ************************************************************************** */
/* ������������USB������ʽ, ֻ��CH375֧�� */

#define	CMD_SET_BAUDRATE	0x02			/* ���ڷ�ʽ: ���ô���ͨѶ������ */
/* ����: �����ʷ�Ƶϵ��, �����ʷ�Ƶ���� */
/* ���: ����״̬( CMD_RET_SUCCESS��CMD_RET_ABORT, ����ֵ˵������δ��� ) */

#define	CMD_ABORT_NAK		0x17			/* ������ʽ: ������ǰNAK������ */

#define	CMD_SET_RETRY		0x0B			/* ������ʽ: ����USB������������Դ��� */
/* ����: ����25H, ���Դ��� */
/*                    λ7Ϊ1���յ�NAKʱ��������, λ3~λ0Ϊ��ʱ������Դ��� */

#define	CMD_ISSUE_TOKEN		0x4F			/* ������ʽ: ��������, ִ������ */
/* ����: �������� */
/*           ��4λ������, ��4λ�Ƕ˵�� */
/* ����ж� */

#define	CMD_CLR_STALL		0x41			/* ������ʽ: ���ƴ���-����˵���� */
/* ����: �˵�� */
/* ����ж� */

#define	CMD_SET_ADDRESS		0x45			/* ������ʽ: ���ƴ���-����USB��ַ */
/* ����: ��ֵַ */
/* ����ж� */

#define	CMD_GET_DESCR		0x46			/* ������ʽ: ���ƴ���-��ȡ������ */
/* ����: ���������� */
/* ����ж� */

#define	CMD_SET_CONFIG		0x49			/* ������ʽ: ���ƴ���-����USB���� */
/* ����: ����ֵ */
/* ����ж� */

#define	CMD_DISK_INIT		0x51			/* ������ʽ: ��ʼ��USB�洢�� */
/* ����ж� */

#define	CMD_DISK_RESET		0x52			/* ������ʽ: ��λUSB�洢�� */
/* ����ж� */

#define	CMD_DISK_SIZE		0x53			/* ������ʽ: ��ȡUSB�洢�������� */
/* ����ж� */

#define	CMD_DISK_READ		0x54			/* ������ʽ: ��USB�洢�������ݿ�(������512�ֽ�Ϊ��λ) */
/* ����: LBA������ַ(�ܳ���32λ, ���ֽ���ǰ), ������(01H~FFH) */
/* ����ж� */

#define	CMD_DISK_RD_GO		0x55			/* ������ʽ: ����ִ��USB�洢���Ķ����� */
/* ����ж� */

#define	CMD_DISK_WRITE		0x56			/* ������ʽ: ��USB�洢��д���ݿ�(������512�ֽ�Ϊ��λ) */
/* ����: LBA������ַ(�ܳ���32λ, ���ֽ���ǰ), ������(01H~FFH) */
/* ����ж� */

#define	CMD_DISK_WR_GO		0x57			/* ������ʽ: ����ִ��USB�洢����д���� */
/* ����ж� */

/* ************************************************************************** */
/* ����������V2.0������, ��CH372A/CH375A֧�� */

#define	CMD_GET_IC_VER		0x01			/* ��ȡоƬ���̼��汾 */
/* ���: �汾��( λ7Ϊ1, λ6Ϊ0, λ5~λ0Ϊ�汾�� ) */
/*           CH375������ЧֵΪ5FH, CH375A���ذ汾�ŵ�ֵΪ0A2H */

#define	CMD_ENTER_SLEEP		0x03			/* ����˯��״̬ */

#define	CMD_RD_USB_DATA0	0x27			/* �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ� */
/* ���: ����, ������ */

#define	CMD_DELAY_100US		0x0F			/* ���ڷ�ʽ: ��ʱ100uS */
/* ���: ��ʱ�ڼ����0, ��ʱ���������0 */

#define	CMD_CHK_SUSPEND		0x0B			/* �豸��ʽ: ���ü��USB���߹���״̬�ķ�ʽ */
/* ����: ����10H, ��鷽ʽ */
/*                    00H=�����USB����, 04H=��50mSΪ������USB����, 05H=��10mSΪ������USB���� */

#define	CMD_SET_SYS_FREQ	0x04			/* ����ϵͳ����Ƶ�� */
/* ����: Ƶ�� */
/*           00H=12MHz, 01H=1.5MHz */

/* ************************************************************************** */
/* ���¸Ľ���V2.0������, ����USB������ʽ, ��CH375A֧�� */

/*#define	CMD_SET_RETRY		0x0B*/			/* ������ʽ: ����USB������������Դ��� */
/* ����: ����25H, ���Դ��� */
/*                    λ7Ϊ0���յ�NAKʱ������, λ7Ϊ1λ6Ϊ0���յ�NAKʱ��������, λ7Ϊ1λ6Ϊ1���յ�NAKʱ����200mS, λ5~λ0Ϊ��ʱ������Դ��� */

/* ************************************************************************** */
/* ����������V2.0������, ����USB������ʽ, ��CH375A֧�� */

#define	CMD_TEST_CONNECT	0x16			/* ������ʽ: ���USB�豸����״̬ */
/* ���: ״̬( USB_INT_CONNECT��USB_INT_DISCONNECT, ����ֵ˵������δ��� ) */

#define	CMD_AUTO_SETUP		0x4D			/* ������ʽ: �Զ�����USB�豸 */
/* ����ж� */

#define	CMD_ISSUE_TKN_X		0x4E			/* ������ʽ: ����ͬ������, ִ������ */
/* ����: ͬ����־, �������� */
/*           ͬ����־��λ7Ϊ�����˵�IN��ͬ������λ, λ6Ϊ�����˵�OUT��ͬ������λ, λ5~λ0����Ϊ0 */
/*           �������Եĵ�4λ������, ��4λ�Ƕ˵�� */
/* ����ж� */

#define	CMD_SET_DISK_LUN	0x0B			/* ������ʽ: ����USB�洢���ĵ�ǰ�߼���Ԫ�� */
/* ����: ����34H, �µĵ�ǰ�߼���Ԫ��(00H-0FH) */

#define	CMD_DISK_BOC_CMD	0x50			/* ������ʽ: ��USB�洢��ִ��BulkOnly����Э������� */
/* ����ж� */

#define	CMD_DISK_INQUIRY	0x58			/* ������ʽ: ��ѯUSB�洢������ */
/* ����ж� */

#define	CMD_DISK_READY		0x59			/* ������ʽ: ���USB�洢������ */
/* ����ж� */

#define	CMD_DISK_R_SENSE	0x5A			/* ������ʽ: ���USB�洢������ */
/* ����ж� */

#define	CMD_DISK_MAX_LUN	0x5D			/* ������ʽ: ��ȡUSB�洢������߼���Ԫ�� */
/* ����ж� */

/* ********************************************************************************************************************* */
/* ����״̬ */

#define	CMD_RET_SUCCESS		0x51			/* ��������ɹ� */
#define	CMD_RET_ABORT		0x5F			/* �������ʧ�� */

/* ********************************************************************************************************************* */
/* USB�ж�״̬ */

/* ����״̬����Ϊ�����¼��ж�, ��CH372A/CH375A֧��, ���ͨ��CMD_CHK_SUSPEND����USB���߹�����, ��ô���봦��USB���߹����˯�߻��ѵ��ж�״̬ */
#define	USB_INT_USB_SUSPEND	0x05			/* USB���߹����¼� */
#define	USB_INT_WAKE_UP		0x06			/* ��˯���б������¼� */

/* ����״̬����0XH����USB�豸��ʽ */
/*   ���ù̼�ģʽ��ֻ��Ҫ����: USB_INT_EP1_IN, USB_INT_EP2_OUT, USB_INT_EP2_IN, ����CH372A/CH375A����Ҫ����: USB_INT_EP1_OUT */
/*   λ7-λ4Ϊ0000 */
/*   λ3-λ2ָʾ��ǰ����, 00=OUT, 10=IN, 11=SETUP */
/*   λ1-λ0ָʾ��ǰ�˵�, 00=�˵�0, 01=�˵�1, 10=�˵�2, 11=USB���߸�λ */
#define	USB_INT_EP0_SETUP	0x0C			/* USB�˵�0��SETUP */
#define	USB_INT_EP0_OUT		0x00			/* USB�˵�0��OUT */
#define	USB_INT_EP0_IN		0x08			/* USB�˵�0��IN */
#define	USB_INT_EP1_OUT		0x01			/* USB�˵�1��OUT */
#define	USB_INT_EP1_IN		0x09			/* USB�˵�1��IN */
#define	USB_INT_EP2_OUT		0x02			/* USB�˵�2��OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB�˵�2��IN */
/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB���߸�λ */
#define	USB_INT_BUS_RESET1	0x03			/* USB���߸�λ */
#define	USB_INT_BUS_RESET2	0x07			/* USB���߸�λ */
#define	USB_INT_BUS_RESET3	0x0B			/* USB���߸�λ */
#define	USB_INT_BUS_RESET4	0x0F			/* USB���߸�λ */

/* ����״̬����2XH-3XH����USB������ʽ��ͨѶʧ�ܴ���, ��CH375/CH375A֧�� */
/*   λ7-λ6Ϊ00 */
/*   λ5Ϊ1 */
/*   λ4ָʾ��ǰ���յ����ݰ��Ƿ�ͬ�� */
/*   λ3-λ0ָʾ����ͨѶʧ��ʱUSB�豸��Ӧ��: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=��ʱ */
/* USB_INT_RET_ACK	0x001X0010B */			/* ����:����IN���񷵻�ACK */
/* USB_INT_RET_NAK	0x001X1010B */			/* ����:����NAK */
/* USB_INT_RET_STALL	0x001X1110B */		/* ����:����STALL */
/* USB_INT_RET_DATA0	0x001X0011B */		/* ����:����OUT/SETUP���񷵻�DATA0 */
/* USB_INT_RET_DATA1	0x001X1011B */		/* ����:����OUT/SETUP���񷵻�DATA1 */
/* USB_INT_RET_TOUT	0x001XXX00B */			/* ����:���س�ʱ */
/* USB_INT_RET_TOGX	0x0010X011B */			/* ����:����IN���񷵻����ݲ�ͬ�� */
/* USB_INT_RET_PID	0x001XXXXXB */			/* ����:δ���� */

/* ����״̬����1XH����USB������ʽ�Ĳ���״̬����, ��CH375/CH375A֧�� */

#define	USB_INT_SUCCESS		0x14			/* USB������ߴ�������ɹ� */
#define	USB_INT_CONNECT		0x15			/* ��⵽USB�豸�����¼� */
#define	USB_INT_DISCONNECT	0x16			/* ��⵽USB�豸�Ͽ��¼� */
#define	USB_INT_BUF_OVER	0x17			/* USB���ƴ��������̫��, ��������� */
#define	USB_INT_DISK_READ	0x1D			/* USB�洢�������ݿ�, �������ݶ��� */
#define	USB_INT_DISK_WRITE	0x1E			/* USB�洢��д���ݿ�, ��������д�� */
#define	USB_INT_DISK_ERR	0x1F			/* USB�洢������ʧ�� */

/* ********************************************************************************************************************* */
/* ����USB���� */

/* USB�İ���ʶPID, ������ʽ�����õ� */
#define	DEF_USB_PID_NULL	0x00			/* ����PID, δ���� */
#define	DEF_USB_PID_SOF		0x05
#define	DEF_USB_PID_SETUP	0x0D
#define	DEF_USB_PID_IN		0x09
#define	DEF_USB_PID_OUT		0x01
#define	DEF_USB_PID_ACK		0x02
#define	DEF_USB_PID_NAK		0x0A
#define	DEF_USB_PID_STALL	0x0E
#define	DEF_USB_PID_DATA0	0x03
#define	DEF_USB_PID_DATA1	0x0B
#define	DEF_USB_PID_PRE		0x0C

/* USB��������, ���ù̼�ģʽ�����õ� */
#define	DEF_USB_REQ_READ	0x80			/* ���ƶ����� */
#define	DEF_USB_REQ_WRITE	0x00			/* ����д���� */
#define	DEF_USB_REQ_TYPE	0x60			/* ������������ */
#define	DEF_USB_REQ_STAND	0x00			/* ��׼���� */
#define	DEF_USB_REQ_CLASS	0x20			/* �豸������ */
#define	DEF_USB_REQ_VENDOR	0x40			/* �������� */
#define	DEF_USB_REQ_RESERVE	0x60			/* �������� */

/* USB��׼�豸����, RequestType��λ6λ5=00(Standard), ���ù̼�ģʽ�����õ� */
#define	DEF_USB_CLR_FEATURE	0x01
#define	DEF_USB_SET_FEATURE	0x03
#define	DEF_USB_GET_STATUS	0x00
#define	DEF_USB_SET_ADDRESS	0x05
#define	DEF_USB_GET_DESCR	0x06
#define	DEF_USB_SET_DESCR	0x07
#define	DEF_USB_GET_CONFIG	0x08
#define	DEF_USB_SET_CONFIG	0x09
#define	DEF_USB_GET_INTERF	0x0A
#define	DEF_USB_SET_INTERF	0x0B
#define	DEF_USB_SYNC_FRAME	0x0C

/* ********************************************************************************************************************* */

// ����Ϊ�µĶ���,ԭ��������CH375INC.H�ļ� *******************************************

#define CH375_BLOCK_SIZE		64		/* CH375 maximum data block size */
#define CH375_BLK_PER_SEC		8		/* CH375 block per sector, SECTOR_SIZE/CH375_BLOCK_SIZE */

/* ********************************************************************************************************************* */
#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	/* ����ָ��ʱ������ļ�ʱ������ */
#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	/* ����ָ�������յ��ļ��������� */

/*
// RBC commands
*/
#define RBC_CMD_FORMAT					0x04
#define RBC_CMD_READCAPACITY			0x25
#define RBC_CMD_STARTSTOPUNIT			0x1B
#define RBC_CMD_SYNCCACHE				0x35

/*
// SPC-2 commands
*/
#define SPC_CMD_MODESELECT6				0x15
#define SPC_CMD_MODESENSE6				0x1A
#define SPC_CMD_PERSISTANTRESERVIN		0x5E
#define SPC_CMD_PERSISTANTRESERVOUT		0x5F
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL	0x1E
#define SPC_CMD_RELEASE6				0x17
#define SPC_CMD_REQUESTSENSE			0x03
#define SPC_CMD_RESERVE6				0x16
#define SPC_CMD_TESTUNITREADY			0x00
#define SPC_CMD_WRITEBUFFER				0x3B
#define SPC_CMD_READLONG				0x23

typedef struct _MODE_SENSE_10 {
		INT8 OperationCode;
		INT8 Reserved1;
		INT8 PageCode : 6;
		INT8 Pc : 2;
		INT8 Reserved2[4];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved3[3];
} MODE_SENSE_10, *PMODE_SENSE_10;

typedef struct _MODE_SELECT_10 {
		INT8 OperationCode;
		INT8 Reserved1 : 4;
		INT8 PFBit : 1;
		INT8 Reserved2 : 3;
		INT8 Reserved3[5];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved4[3];
} MODE_SELECT_10, *PMODE_SELECT_10;
/*
////////////////////////////////////////////////////////////////////////////////////
// Command Descriptor Block
//      _RBC : Reduced Block Command
//      _SPC : SPC-2 SCSI primary Command - 2
////////////////////////////////////////////////////////////////////////////////////
*/

typedef struct _GENERIC_CDB {
	INT8 OperationCode;
    INT8 Reserved[15];
} GENERIC_CDB,*PGENERIC_CDB;

typedef struct _GENERIC_RBC {
	INT8 OperationCode;
    INT8 Reserved[8];
	INT8 Control;
} GENERIC_RBC,*PGENERIC_RBC;

/*
// format unit
*/
typedef struct _FORMAT_RBC {
	INT8 OperationCode;	/* 04H */
	INT8 VendorSpecific;
	INT8 Increment : 1;
	INT8 PercentorTime : 1;
	INT8 Progress : 1;
    INT8 Immediate : 1;
	INT8 VendorSpecific1 : 4;
	INT8 Reserved2[2];
	INT8 Control;
} FORMAT_RBC, *PFORMAT_RBC;

//////////////////////////////////////////////////
typedef struct _READ_LONG_CMD{
    INT8 OperationCode;
    INT8 LogicalUnitNum	:3;
    INT8 RES_1		:5;
    INT8 RES_2;
    INT8 RES_3;
    INT8 RES_4;
    INT8 RES_5;
    INT8 RES_6;
    INT16 AllocationLen;
    INT8 RES_7;
    INT8 RES_8;
    INT8 RES_9;
}READ_LONG_CMD, *PREAD_LONG_CMD;

typedef struct _READ_LONG {
    INT8 RES_1;
    INT8 RES_2;
    INT8 RES_3;
    INT8 CAP_LIST_LEN;
    INT8 LBA_3;
    INT8 LBA_2;
    INT8 LBA_1;
    INT8 LBA_0;
    INT8 Descripter;
    INT8 BlockLen_2;
    INT8 BlockLen_1;
    INT8 BlockLen_0;
} READ_LONG, *PREAD_LONG;
/*
// START_STOP_UNIT
*/
typedef struct _START_STOP_RBC {
    INT8 OperationCode;    /*1BH*/
    INT8 Immediate: 1;
    INT8 Reserved1 : 7;
    INT8 Reserved2[2];
	union _START_STOP_FLAGS
    {
        struct
        {
            INT8 Start          : 1;
            INT8 LoadEject      : 1;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits0;

        struct
        {
            INT8 MediumState    : 2;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits1;
    } Flags;
    INT8 Control;
} START_STOP_RBC, *PSTART_STOP_RBC;

/*
// Synchronize Cache
*/
typedef struct _SYNCHRONIZE_CACHE_RBC {

	INT8 OperationCode;    /* 0x35 */
	INT8 Reserved[8];
	INT8 Control;

} SYNCHRONIZE_CACHE_RBC, *PSYNCHRONIZE_CACHE_RBC;


/*
//***********************************************************************************
// SPC-2 of SCSI-3 commands
//***********************************************************************************
*/

typedef struct _STD_INQUIRYDATA {
    INT8 DeviceType : 5;
    INT8 Reserved0 : 3;

    INT8 Reserved1 : 7;
    INT8 RemovableMedia : 1;

    INT8 Reserved2;

    INT8 Reserved3 : 5;
    INT8 NormACA : 1;
    INT8 Obsolete0 : 1;
    INT8 AERC : 1;

    INT8 Reserved4[3];

    INT8 SoftReset : 1;
    INT8 CommandQueue : 1;
	INT8 Reserved5 : 1;
	INT8 LinkedCommands : 1;
	INT8 Synchronous : 1;
	INT8 Wide16Bit : 1;
	INT8 Wide32Bit : 1;
	INT8 RelativeAddressing : 1;

	INT8 VendorId[8];

	INT8 ProductId[16];

	INT8 ProductRevisionLevel[4];

/*//  Above is 36 bytes//  can be tranmitted by Bulk*/
    INT8 VendorSpecific[20];
    INT8 InfoUnitSupport : 1;
    INT8 QuickArbitSupport : 1;
    INT8 Clocking : 2;
    INT8 Reserved6 : 4;

    INT8  Reserved7 ;
    INT16 VersionDescriptor[8] ;

    INT8 Reserved8[22];
} STD_INQUIRYDATA, *PSTD_INQUIRYDATA;

typedef struct _SERIALNUMBER_PAGE {
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;
    INT8 SerialNumber[24] ;

}VPD_SERIAL_PAGE,* PVPD_SERIAL_PAGE;

#define ASCII_ID_STRING 32
typedef struct _ID_DESCRIPTOR {
	INT8   CodeSet : 4;
	INT8   Reserved0 : 4;

	INT8   IDType : 4;
    INT8   Association : 2;
    INT8   Reserved1 : 2;

    INT8   Reserved2;

	INT8   IDLength ;
	INT8   AsciiID[ASCII_ID_STRING];
} ASCII_ID_DESCRIPTOR,* PASCII_ID_DESCRIPTOR;

typedef struct _DEVICE_ID_PAGE
{
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;

    ASCII_ID_DESCRIPTOR   AsciiIdDescriptor[1];
} VPD_DEVICE_ID_PAGE, * PVPD_DEVICE_ID_PAGE;

typedef struct _READ_CAPACITY_RSP {
	INT32 LastLBA;
	INT32 BlockSize;

} READ_CAPACITY_RSP, *PREAD_CAPACITY_RSP;
/*
// Mode Select
*/
typedef struct _MODE_SELECT_SPC {
	INT8 OperationCode;	/* 15H */
	INT8 SavePage : 1 ;
	INT8 Reseved0 : 3 ;
	INT8 PageFormat : 1 ;
	INT8 Reserved1 : 3 ;
	INT8 Reserved2[2];
	INT8 ParameterLen;
	INT8 Control;
} MODE_SELECT_SPC, * PMODE_SELECT_SPC;

/*
// Mode Sense
*/
typedef struct _MODE_SENSE_SPC {
    INT8 OperationCode;	/* 1AH */
    INT8 Reseved0 : 3 ;
    INT8 DisableBlockDescriptor : 1 ;
    INT8 Reserved0 : 4 ;
    INT8 PageCode:6 ;
    INT8 PageControl : 2 ;
    INT8 Reserved1;
    INT8 ParameterLen;
    INT8 Control;
} MODE_SENSE_SPC, * PMODE_SENSE_SPC;

typedef struct _MODE_PARAMETER_HEAD {
    INT8 DataLen;
    INT8 MediumType;
    INT8 DeviceParameter;
    INT8 BlockDescriptorLen;
} MODE_PARAMETER_HEAD, * PMODE_PARAMETER_HEAD;

/*
// Define Device Capabilities page.
*/
typedef struct _MODE_RBC_DEVICE_PARAMETERS_PAGE {
    INT8 PageCode : 6;
	INT8 Reserved : 1;
    INT8 PageSavable : 1;
    INT8 PageLength;
    INT8 WriteCacheDisable : 1;
    INT8 Reserved1 : 7;
    INT8 LogicalBlockSize[2];
    INT8 NumberOfLogicalBlocks[5];
    INT8 PowerPerformance;
    INT8 Lockable : 1;
    INT8 Formattable : 1;
    INT8 Writable : 1;
    INT8 Readable : 1;
    INT8 Reserved2 : 4;
    INT8 Reserved3;
}MODE_RBC_DEVICE_PARAMETERS_PAGE, *PMODE_RBC_DEVICE_PARAMETERS_PAGE;

/*
// prevent/allow medium removal
*/
typedef struct _MEDIA_REMOVAL_SPC {
	INT8 OperationCode;    /* 1EH */
	INT8 Reserved0[3];
	INT8 Prevent;
	//INT8 Reserved1:6 ;
	//INT8 Control;
} MEDIA_REMOVAL_SPC, *PMEDIA_REMOVAL_SPC;

/*
// Request Sense
*/
typedef struct _REQUEST_SENSE_SPC {
    INT8 OperationCode;    /* 03H */
    INT8 Reserved[3];
    INT8 AllocationLen;
    INT8 Control;
} REQUEST_SENSE_SPC, *PREQUEST_SENSE_SPC;

typedef struct _REQUEST_SENSE_DATA {
    INT8 ResponseCode : 7;
    INT8 Valid : 1;

    INT8 SegmentNum;

    INT8 SenseKey : 4;
    INT8 Reserved0 : 1;
    INT8 WrongLenIndicator : 1;
    INT8 EndofMedium : 1;
    INT8 FileMark : 1;

    INT8 Info_0;
    INT8 Info_1;
    INT8 Info_2;
    INT8 Info_3;

    INT8 AdditionalSenseLen;

    INT8 CommandSpecInfo_0;
    INT8 CommandSpecInfo_1;
    INT8 CommandSpecInfo_2;
    INT8 CommandSpecInfo_3;

    INT8 ASC;
    INT8 ASCQ;
    INT8 FieldReplacableUnitCode;
    INT8 SenseKeySpec_0 : 7;
    INT8 SenseKeySpecValid : 1;
    INT8 SenseKeySpec_1;
    INT8 SenseKeySpec_2;

} REQUEST_SENSE_DATA, *PREQUEST_SENSE_DATA;

/*
// Test Unit Ready
*/
typedef struct _TEST_UNIT_SPC {
	INT8 OperationCode;    /* 00H */
	INT8 Reserved[4];
	INT8 Control;
} TEST_UNIT_SPC, *PTEST_UNIT_SPC;

/*
// Write Buffer
*/
typedef struct _WRITE_BUFFER_SPC {
    INT8 OperationCode;    /* 3BH */
    INT8 Mode:4 ;
    INT8 Reserved0:4 ;
	INT8 BufferID;
    INT8 BufferOff_2;
    INT8 BufferOff_1;
    INT8 BufferOff_0;
    INT8 ParameterLen_2;
    INT8 ParameterLen_1;
	INT8 ParameterLen_0;
    INT8 Control;
} WRITE_BUFFER_SPC, *PWRITE_BUFFER_SPC;

typedef union _CDB_RBC {
    GENERIC_CDB             Cdb_Generic;
     // RBC commands
    GENERIC_RBC             RbcCdb_Generic;
    FORMAT_RBC              RbcCdb_Format;
    START_STOP_RBC          RbcCdb_OnOffUnit;
    SYNCHRONIZE_CACHE_RBC   RbcCdb_SyncCache;

    // SPC-2 commands
    MODE_SELECT_SPC         SpcCdb_ModeSelect;
    MODE_SENSE_SPC          SpcCdb_ModeSense;
    MEDIA_REMOVAL_SPC       SpcCdb_Remove;
    REQUEST_SENSE_SPC       SpcCdb_RequestSense;
    TEST_UNIT_SPC           SpcCdb_TestUnit;
    WRITE_BUFFER_SPC        SpcCdb_WriteBuffer;

    // ATAPI Commands
    MODE_SELECT_10  CmdModeSel10;
    MODE_SENSE_10   CmdModeSen10;
    //////////////////////////////////////
    READ_LONG_CMD	SpcCdb_ReadLong;

} CDB_RBC, *PCDB_RBC;
//***********************************************

//***********************************************////
#define CBW_SIGNATURE   0x55534243
#define CSW_SIGNATURE   0x55534253

typedef struct _COMMAND_BLOCK_WRAPPER{
    INT32   dCBW_Signature;
    INT32   dCBW_Tag;
    INT32   dCBW_DataXferLen;
    INT8    bCBW_Flag;
    INT8    bCBW_LUN;
    INT8    bCBW_CDBLen;
    CDB_RBC cdbRBC;
} CBW, *PCBW;

typedef struct _COMMAND_STATUS_WRAPPER{
    INT32   dCSW_Signature;
    INT32   dCSW_Tag;
    INT32   dCSW_DataResidue;
    INT8    bCSW_Status;
} CSW, *PCSW;

typedef union _TPBULK_STRUC {
    CBW     TPBulk_CommandBlock;
    CSW     TPBulk_CommandStatus;
}TPBLK_STRUC, * PTPBLK_STRUC;

//unsigned char TPBulk_GetMaxLUN(void);

unsigned char	mCH375Init(void );
unsigned char	mWaitInterrupt(void );
void	mDelay2uS(void );
void	DelayMs(unsigned char nFactor);
unsigned long SwapINT32(unsigned long dData);
unsigned int SwapINT16(unsigned int dData);
unsigned char mInitDisk( void );
unsigned char ReadCapacity(void);
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer);
unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer);
unsigned char SPC_RequestSense(void);
unsigned char SPC_TestUnit(void);
unsigned char SPC_LockMedia(void);
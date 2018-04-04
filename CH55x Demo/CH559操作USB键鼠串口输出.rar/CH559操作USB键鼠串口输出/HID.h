
//-------------------------------------------------------------------------
//主项目标签(Main item tag) 
//The default data value for all Main items is zero  
#define	Main_INPUT				   0x80
#define	Main_OUTPUT				   0x90
#define	Main_FEATURE			   0xB0
#define	Main_COLLECTION			 0xA0
#define	Main_END_COLLECTION	 0xC0
//Collection Value
#define	ColleVal_PHYSICAL		 0x00
#define	ColleVal_APPLICATION 0x01
#define	ColleVal_LOGICAL		 0x02
#define	ColleVal_REPORT			 0x03
#define	ColleVal_NAMED_ARREY 0x04
#define	ColleVal_USAGE_SWITCH 0x05
#define	ColleVal_USAGE_MODIFIER 0x06
//---------------------------------------------------------------
//全局项(Global Items)
#define	Global_USAGE_PAGE			    0x04
#define	Global_LOGICAL_MINIMUM		0x14
#define	Global_LOGICAL_MAXIMUM		0x24
#define	Global_PHYSICAL_MINIMUM		0X34
#define	Global_PHYSICAL_MAXIMUM		0X44
#define	Global_UNIT_EXPOLENT		  0X54
#define	Global_UNIT					      0x64
#define	Global_REPORT_SIZE			  0x74
#define	Global_REPORT_ID			    0x84
#define	Global_REPORT_COUNT		    0x94
#define	Global_PUSH					      0xA4
#define	Global_POP					      0xB4
//
#define	UsagePage_GENERIC_DESKTOP	0x01
#define UsagePage_LEDS	       		0x08
#define UsagePage_BUTTON			    0x09

//---------------------------------------------------------------------------
//Local Items
#define	Local_USAGE					    0x08
#define	Local_USAGE_MINIMUM			0x18
#define	Local_USAGE_MAXMUM			0x28
#define	Local_DESIGNATOR_INDEX		0X38
#define	Local_DESIGNATOR_MINIMUM	0X48
#define	Local_DESIGNATOR_MAXIMUM	0X58
//
#define	Usage_POINTER		  0x01
#define	Usage_MOUSE				0x02
#define Game_Pad          0x05//游戏柄
#define	Usage_KEYBOARD		0x06
#define	Usage_X						0x30
#define	Usage_Y						0x31
#define	Usage_WHEEL				0x38
#define Reserved          0x94//大于94都是保留项

#define Success           0x00
#define ID_Len_Err        0x01
#define Not_Mouse         0x02
#define Len_Err           0x03
#define Unknow            0x0F


typedef struct  _HID_Device_Button{
 unsigned char Usage_min;
 unsigned char Usage_max;
 unsigned char Logical_min;
 unsigned char Logical_max;
 unsigned char Size_bit;
 unsigned char Size_reserved;
}BUTTON;

typedef struct  _HID_Device_XY{
 unsigned short Logical_min;
 unsigned short Logical_max;
 unsigned char Size_bit;
 unsigned char Size_reserved;
}XY;

typedef struct  _HID_Device_Wheel{
 unsigned char Logical_min;
 unsigned char Logical_max;
 unsigned char Size_bit;
 unsigned char Size_reserved;
}WHEEL;

typedef struct  _HID_Device_Mouse{
	unsigned short  X;
	unsigned short  Y;
	unsigned char  Left;
	unsigned char  Right;
	unsigned char  Middle;
	char  Wheel;
	//------------------------------
  BUTTON Button;
  XY X_Y;
  WHEEL Wheel_;
} MOUSE;
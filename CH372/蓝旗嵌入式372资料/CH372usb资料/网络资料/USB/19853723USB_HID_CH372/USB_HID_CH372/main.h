//sbit    SPEAKER     =           P1^1;
//sbit    LED         =           P3^3;

//sbit    OUT         =           P2^3;           //接收IO口定义
sbit    OUT         =           P2^7;  //接收IO口定义
//sbit    OUT         =           P1^0;  //接收IO口定义

//sbit    LED2        =           P2^4;  

//sbit    SPK_P         =           P1^0;
sbit    SPK_P         =           P2^4;

sbit    RED_LED         =           P2^2;
sbit    GREEN_LED       =           P2^3;

#define TRUE                    1
#define FALSE                   0
#define OSC_FREQ                22118400//11059200L
                                
#define BAUD_115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BAUD_57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BAUD_38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BAUD_28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BAUD_19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BAUD_14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BAUD_9600               256 - (OSC_FREQ/192L)/9600L     // 244
#define RCAP2_50us              65536L - OSC_FREQ/240417L
#define RCAP2_1ms               65536L - OSC_FREQ/12021L
#define RCAP2_10ms              65536L - OSC_FREQ/1200L
#define RCAP2_1s                65536L - OSC_FREQ/12L

extern  bit         PA;
extern  void        RDCARD(void);
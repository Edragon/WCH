#include <reg52.h>

//#define NULL		0
#define TRUE   		1
#define FALSE  		0

#define CHBUF_LENGTH  28672
#define BUFFER_LENGTH	512
#define MAX_READ_LENGTH 28672
#define MAX_WRITE_LENGTH 28672
// Disable all interrupts
#define DISABLE_INTERRUPTS		EA = 0
// Enable all interrupts
#define ENABLE_INTERRUPTS		EA = 1

typedef unsigned char       INT8;
typedef unsigned short      INT16;
typedef unsigned long       INT32;

typedef union {
    struct 
    {
        INT8    Type;
        INT8    Index;
    } Descriptor;

    struct 
    {
        INT8 tx0;  // MSB for 8051 Keil C
        INT8 tx1;   
        INT8 endp;   
        INT8 c0;   // LSB for 8051 Keil C
    } chars;

    struct 
    {
        INT8 c3;  // MSB for 8051 Keil C
        INT8 c2;   
        INT8 c1;   
        INT8 c0;   // LSB for 8051 Keil C
    } chars0;

    struct
    {
        INT16 i1;    // MSW for 8051 keil C
        INT16 i0;    // LSW for 8051 Keil C
    } ints;

    INT32 u0;

} FLEXI_INT32;

typedef union  {
    struct 
    {
        INT8 c1;   // MSB for 8051 Keil C
        INT8 c0;   // LSB for 8051 Keil C
    } chars;

    INT16 i0;
} FLEXI_INT16, * PFLEXI_INT16;


typedef union _XXG_FLAGS
{
	struct _FLAGS
	{
		unsigned char bTimer           	:1;
		unsigned char bIN_ISR		    :1;
		unsigned char bCOM_ERR			:1;
		unsigned char bTimeout			:1;
		unsigned char SLAVE_IS_ATTACHED	:1;
		unsigned char SLAVE_REMOVED		:1;
		unsigned char SLAVE_FOUND		:1;	// Slave USB device found
		unsigned char SLAVE_ENUMERATED	:1;	// slave USB device enumeration done
		unsigned char SLAVE_ONLINE		:1;
		unsigned char TIMEOUT_ERR		:1;	// timeout error during data endpoint transfer
		unsigned char DATA_STOP			:1;	// device unplugged during data transfer
		unsigned char bData1			:1;
		unsigned char bCHInDone			:1;
		unsigned char bMassDevice		:1;
		unsigned char bFatChanged		:1;
	} bits;
} XXGFLAGS;

#include "CH375.H"
#include "FAT.H"
#include "HPI.H"

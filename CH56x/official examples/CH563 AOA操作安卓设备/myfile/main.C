/********************************** (C) COPYRIGHT *******************************
* File Name          : CH563.C
* Author             : 
* Version            : V1.20
* Date               : 2014/8/4
* Description        : CH563 USB Host Examples
* 					   (1)、CH563 USB Examples by KEIL3_V3.53；	
*                      (2)、串口0输出监控信息,115200bps；
*					   (3)、USB HOST以查询方式处理；
*******************************************************************************/



/******************************************************************************/
/* 头文件包含 */
#include <stdio.h>
#include <string.h>
#include "absacc.h"
#include "SYSFREQ.H"								  			    			/* 系统定义相关头文件 */
#include "CH563SFR.H"
#include "EHCI_HCD.H"															/* 主机控制器低层操作相关头文件 */
#include "CH563USBSFR.H"								  			    		/* 芯片USB寄存器相关头文件 */
#include "PRINTF.H"																/* 串口打印输出控制头文件 */
#include "android.h"
/******************************************************************************/
/* 常、变量定义 */
UINT8 EHCI_Allocate_Buf[ EHCI_ALLOCATE_BUF_LEN ];								/* 内存分配缓冲区(由于QH、QTD等结构提需要32字节对齐, 仅用来分配QH、QTD相关内存使用) */
__align( 4096 ) UINT8 EHCI_Data_Buf[ EHCI_DATA_BUF_LEN ];						/* 内存数据缓冲区(需要4K对齐) */								
__align( 4096 ) UINT8 EHCI_PERIODIC_LIST_Buf[ EHCI_PER_LISE_BUF_LEN ];			/* 周期性帧列表缓冲区(需要4K对齐) */								

/* 定义HUB相关结构体 */
AOA_INFO1  AOA_INFO;																/* HUB相关结构体信息 */
Periodic_Frame_List_Structure  *pHID_FramList;	   								/* 周期性帧列表结构体 */

/******************************************************************************/

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ中断处理函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void IRQ_Handler( void )
{
	while(1);
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : FIQ中断处理函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void FIQ_Handler( void )
{
    while( 1 );
}


/*******************************************************************************
* Function Name  : Bluetooth_Init
* Description    : 蓝牙串口初始化
*         
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Init( UINT32 Baud_rate )
{  
	UINT32	x, x2;
	x = 10 * FREQ_SYS * 2 / 16 / Baud_rate;  // 115200bps
	x2 = x % 10;
	x /= 10;
	if ( x2 >= 5 ) x ++;  // 四舍五入
	R8_UART1_LCR = 0x80;  // DLAB位置1
	R8_UART1_DIV = 1;  // 预分频
	R8_UART1_DLM = x>>8;
	R8_UART1_DLL = x&0xff;

	R8_UART1_LCR = 0x03;
	R8_UART1_FCR = 0xc7;
	R8_UART1_IER = RB_IER_TXD_EN;  // TXD enable
	R8_UART1_MCR = 0x08;  // interrupt enable

	R32_PB_SMT |= RXD1 | TXD1;  // RXD0 schmitt input, TXD0 slow rate
	R32_PB_PD &= ~ RXD1;  // disable pulldown for RXD0, keep pullup
	R32_PB_DIR |= TXD1;  // TXD0 output enable
	R8_UART1_IER |= RB_IER_RECV_RDY;										/* 使能UART接收中断 */
//	R8_INT_EN_IRQ_0 |= RB_IE_IRQ_UART1;										/* 使能IRQ中的UART1中断 */
}



/*******************************************************************************
* Function Name  : fputc
* Description    : 通过串口输出监控信息
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int	fputc( int c, FILE *f )
{
	R8_UART1_THR = c;                            								/* 发送数据 */
	while( ( R8_UART1_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );       					/* 等待数据发送 */
	return( c );
}


__align( 4096 ) UINT8 data_buf[4096];
UINT8 data_buf1[100];
/*******************************************************************************
* Function Name  : main
* Description    : 主程序
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int	main( void ) 
{
	UINT16  status,i,s;
	UINT32 len;	
	Delay_ms( 500 );
	Uart_Init( 115200 ); 													/* 为了让计算机通过串口监控演示过程 */
	USBHOST_ModeSet( );															/* 设置USB主机模式 */
	USBHOST_EHCI_Init( );														/* USB主机EHCI复位及初始化 */
	USBHOST_Structure_DeInit( );												/* USB主机数据结构初始化 */
	USBHOST_DevicePara_Init( &AOA_INFO.Device );									/* USB主机相关参数初始化 */
	while( 1 )
	{				
		/***********************************************************************/
		/* 查询等待设备插入 */
#ifdef  MY_DEBUG_PRINTF 
		printf("Wait USB Device In:\n" );
#endif
HOST_WAIT_DEVICE_IN:
		while( 1 )
		{
			if( USBHOST_EHCI_Detect_Connect( &AOA_INFO.Device ) == USB_INT_CONNECT )/* 检测设备连接 */
			{
				break;
			}
		   	Delay_ms( 50 );
		}
		Delay_ms( 200 );														/* 延时 */
		if( USBHOST_EHCI_Detect_Connect( &AOA_INFO.Device ) == USB_INT_DISCONNECT )/* 检测设备连接 */
		{
			goto HOST_WAIT_DEVICE_IN;											/* 设备断开跳转到前面继续等待 */
		}
#ifdef  MY_DEBUG_PRINTF			
		printf("USB Device In:\n" ); 
		printf("Device Speed is: %02x\n",(UINT16)AOA_INFO.Device.bSpeed ); 		/* 0x01:低速设备, 0x00:全速设备, 0x02:高速设备, 0xFF:未知 */			
#endif
		/***********************************************************************/
		/* 对USB设备进行枚举 */	
		status = USBHOST_EHCI_EP0_Init( &AOA_INFO.Device );						/* 初始化端点0 */
		if( status != USB_OPERATE_SUCCESS )
		{
#ifdef  MY_DEBUG_PRINTF			
			printf("USBHOST_EP0_Init Error\n");
#endif				
			return( status );
		}
		status = USBHOST_EHCI_Enumerate( &AOA_INFO.Device, EHCI_Data_Buf );		/* USB主机对设备进行枚举 */
		printf("AOA_Device_Ststus=%02x\n",status);
		if( status )goto device_out;
		MS_CofDescrAnalyse( &AOA_INFO,EHCI_Data_Buf );							/*分析描述符*/
    Delay_ms( 5000 );
	while(1)
	{	
//		data_buf[0] = 0x31;data_buf[1] = 0x32;
//		data_buf[2] = 0x33;data_buf[3] = 0x34;
//		status = AOA_OUT_Data( &AOA_INFO,data_buf,4);
	   	Delay_ms( 500 );
		memset( data_buf,0x00,100 );
		memset( data_buf1,0x00,100 );
		status = AOA_IN_Data( &AOA_INFO,data_buf,100 );
		if( status == 0x00 )	
		{
		 	s = memcmp( data_buf,data_buf1,100 );		//说明为相同数据，为非取到的数据,第一次接收一定要做
			if( s == 0x00 )continue;	

			printf("in_status=%02x\n",status);
			for( i=0;i!=100;i++ )
				printf("%02x ",data_buf[i]);
			printf("\n");
		}
		if( USBHOST_EHCI_Detect_Connect( &AOA_INFO.Device ) == USB_INT_DISCONNECT )
		{
			printf("USB Device Out\n");
			USBHOST_EHCI_Init( );
			USBHOST_DevicePara_Init( &AOA_INFO.Device );						/* USB主机相关参数初始化 */
//				USBHOST_DevicePara_Init( &UDisk.Device );						/* USB主机相关参数初始化 */
			USBHOST_Structure_DeInit( );									/* USB主机数据结构初始化 */
#ifdef  MY_DEBUG_PRINTF	
			printf("USB Device Out\n");
#endif
			goto HOST_WAIT_DEVICE_IN;
		}	
	}			
device_out:
	while(1){
		if( USBHOST_EHCI_Detect_Connect( &AOA_INFO.Device ) == USB_INT_DISCONNECT )
		{
			printf("USB Device Out\n");
			USBHOST_EHCI_Init( );
			USBHOST_DevicePara_Init( &AOA_INFO.Device );						/* USB主机相关参数初始化 */
			USBHOST_Structure_DeInit( );									/* USB主机数据结构初始化 */
#ifdef  MY_DEBUG_PRINTF	
			printf("USB Device Out\n");
#endif
			goto HOST_WAIT_DEVICE_IN;
		}	
	 }	
	}
}


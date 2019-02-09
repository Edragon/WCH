
/********************************** (C) COPYRIGHT ******************************
* File Name          : CH559_DEMO.C
* Author             : WCH
* Version            : V1.3
* Date               : 2016/06/24
* Description        : IAP下载主程序
                       演示用户程序运行，当P13输入低电平时，程序跳转至IAP程序区，进行用户程序升级
*******************************************************************************/

#include "CH559.H"
#include <string.h>
#include <intrins.h>

#define IAP_ProgrameStartAddr    0xE000                    //IAP程序存放的起始地址，该地址至少要比实际的IAP地址小4字节
sbit EnableIAP  = P1^3;                                    //IAP跳转检测引脚

typedef void( *pTaskFn)( void );
pTaskFn tasksArr[1]; 

#pragma NOAREGS

/*******************************************************************************
* Function Name  : mDelay20us(UNIT16 n)
* Description    : 20us延时函数，主频12MHz，延时不准，其他主频参考DEBUG.C的延时函数
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void mDelay20us( UINT16 n )
{
	for( n <<= 3;n;--n ){
		_nop_( );
	}
}

/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void main( void ) 
{
	UINT16 i=0;

	P1_DIR = 0x00;
	P1_DIR |= 0xff;       		                                 //P40-P43输出使能
 P2_DIR |= 0xff; 
	P3_DIR |= 0xff; 
	P4_DIR |= 0xff; 
	tasksArr[0] = (pTaskFn)(IAP_ProgrameStartAddr+0x00);       //IAP程序地址
	while(1)
  {
		/*
		if( EnableIAP == 0 )                                     //检测P13引脚是否为低
    {	  
			P4_OUT =  0x0f;
			P4_OUT =  ~(1<<1);		                                 //P4连接的LED用于状态指示
			mDelay20us(60000);
			P4_OUT =  0x0f;
	 		(tasksArr[0])( );                                      //跳转至IAP程序区
		}*/
		
		
	  
		
	 
		P1 =  0xFF;
		P2 =  0xFF;
		P3 =  0xFF;
		P4_OUT =  0xFF;
		mDelay20us(3000);
		mDelay20us(3000);
		mDelay20us(3000);
		mDelay20us(3000);
		P1 =  0x00;
		P2 =  0x00;
		P3 =  0x00;
		P4_OUT =  0x00;
		mDelay20us(3000);
		mDelay20us(3000);
		mDelay20us(3000);
		mDelay20us(3000);
		mDelay20us(3000);
		 
		
	}
}

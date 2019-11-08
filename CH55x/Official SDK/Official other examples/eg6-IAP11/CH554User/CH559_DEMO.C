
/********************************** (C) COPYRIGHT ******************************
* File Name          : CH559_DEMO.C
* Author             : WCH
* Version            : V1.3
* Date               : 2016/06/24
* Description        : IAP下载主程序
                       演示用户程序运行，当P13输入低电平时，程序跳转至IAP程序区，进行用户程序升级
*******************************************************************************/

#include "../../public/CH554.H"
#include <string.h>
#include <intrins.h>

#define IAP_ProgrameStartAddr    (0x3000)             //IAP程序存放的起始地址，该地址至少要比实际的IAP地址小4字节
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

	tasksArr[0] = (pTaskFn)(IAP_ProgrameStartAddr+0x00);       //IAP程序地址
	while(1)
  {
		if( EnableIAP == 0 )                                     //检测P13引脚是否为低
    {	  
			mDelay20us(60000);
	 		(tasksArr[0])( );                                      //跳转至IAP程序区
		}
	  i++;
		if( i == 200 ) i = 0;                                    //用户程序可以干其他事情，此处就只是闪灯
		mDelay20us(1000);
	}
}

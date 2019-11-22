
#ifndef _GPIO_H_
#define _GPIO_H_

//gpio mode
typedef enum
{
	GPIO_Mode_IN_Floating = 0,
	GPIO_Mode_PP_Out      = 1,
	GPIO_Mode_OD_Out      = 2,
	GPIO_Mode_IPU         = 3
} GPIO_Mode;

//#define GPIO_INTERRUPT   1
  
/*******************************************************************************
* Function Name  : Port1Cfg()
* Description    : �˿�1����
* Input          : UINT8 Pin	(0-7),	
				   Mode  0 = �������룬������
                         1 = �����������
                         2 = ��©���������������
                         3 = ��51ģʽ����©������������������ڲ���·���Լ����ɵ͵��ߵĵ�ƽ����											 
* Output         : None
* Return         : None
*******************************************************************************/
extern void Port1Cfg(UINT8 Pin, GPIO_Mode Mode);

extern void Port3Cfg(UINT8 Pin, GPIO_Mode Mode);

/*******************************************************************************
* Function Name  : GPIOInterruptCfg()
* Description    : GPIO�ж�����
* Input          : None									 
* Output         : None
* Return         : None
*******************************************************************************/
void GPIOInterruptCfg();

#endif


/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.0
* Date               : 2015/02/05
* Description        : CH563 UART0 UCOS
*                      (1)��CH563 Examples by KEIL4 V4.23
*                      (2)��CH563 UCOS����
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "includes.h"
#include "OS_CPU.h"
#include "BSP.H"


/******************************************************************************/
/* �궨�� */


/******************************************************************************/
/* �ڲ��������� */
void TIM0_INIT( UINT32 );                                                       /* TIM0��ʱ����ʼ�� */
void TaskStart(void *data);                                                     /* ���������� */
void Task1(void *data);                                                         /* ����1���� */
void Task2(void *data);                                                         /* ����2���� */


/******************************************************************************/
/* �ⲿ������������չ */
extern OS_STK  TaskStartStk[TASK_STK_SIZE];                                     /* ���������ջ */


/*******************************************************************************
* @function     IRQ_C_Handler
* @param        None
* @retval       None
* @brief        IRQ�жϺ�������Ҫ��__irq�������Σ��ڻ������б���ӵ���
*******************************************************************************/
void IRQ_C_Handler( void )
{
    UINT8 i;
    
    if(R8_INT_FLAG_0&RB_IF_TMR1)
    {
        i= R8_TMR1_INT_FLAG;
        if(i & RB_TMR_IF_CYC_END)                                               /* ����ʱ�ж� */
        {                                                                       
            R8_INT_EN_IRQ_0 &= ~RB_IE_IRQ_TMR1;                                 /* �رն�ʱ���ж� */
            R8_TMR1_CTRL_MOD |= RB_TMR_ALL_CLEAR;
            R8_TMR1_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
            printf("TIM1 IRQ\r\n");         /* ����Ϊ�����ã��ж�������ʹ�ô�ӡ���ķ�ʱ�� */
            R8_TMR1_INT_FLAG |= 0xFF;
            R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR1;                                  /* ������ʱ���ж� */
        }
    }
}
  

/*******************************************************************************
* @function     FIQ_C_Handler
* @param        None
* @retval       None
* @brief        FIQ�жϺ�������Ҫ��__irq�������Σ��ڻ������б���ӵ���
*******************************************************************************/
void FIQ_C_Handler( void )
{
    while(1);
}


/*******************************************************************************
* @Function     main
* @Input        None
* @Output       None
* @retVal       None
* @brief        ������
*******************************************************************************/
int main( void ) 
{
    /* ��ʱ�����������ȶ� */
    Delay_ms( 100 );                                                 

    /* ��CPU����Ȩ��������ϵͳ */
    OSInit();                                                                   /* ����ϵͳ��ʼ�� */
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);    /* ������ʼ��������ʼ�����г�ʼ��BSP�Լ������������� */
    OSStart();                                                                  /* ����ʼ���� */
}


#if 0
/******************************************************************************/
/* ���º���Ϊ����ʱʹ�ú�������ʱ��Ҫɾ��������bug����ʱ��ʹ�� */
void UndefineInstruction_P( void )
{
    //while(1)
        printf("UndefineInstruction_P\r\n");
}
void PrefetchAbort_P( void )
{
    //while(1)
        printf("PrefetchAbort_P\r\n");
}
void SoftwareInterrupt_P( void )
{
    //while(1)
        printf("SoftwareInterrupt_P\r\n");
}
void DataAbort_P( void )
{
   //while(1)
        printf("DataAbort_P\r\n");
}

#endif

/*********************************** endfile **********************************/
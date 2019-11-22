/********************************** (C) COPYRIGHT *******************************
* File Name          : BSP.C
* Author             : WCH
* Version            : V1.0
* Date               : 2015/02/05
* Description        : Board support package of ucos on CH563.
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "BSP.H"
#include "includes.h"
#include "OS_CPU.h"
#include <stdio.h>


/******************************************************************************/
/* ȫ�ֱ������� */
OS_STK  TaskStartStk[TASK_STK_SIZE];                                            /* ���������ջ */
OS_STK  Task1Stk[TASK_STK_SIZE];                                                /* ����1��ջ */
OS_STK  Task2Stk[TASK_STK_SIZE];                                                /* ����2��ջ */


/******************************************************************************/
/* �ڲ��������� */
void TIM0_INIT( UINT32 ticksPerSec );
void IRQCTimer0( void );
void Task1 (void *pdata);
void Task2( void *pdata );
void TaskStart (void *pdata);
void BSP_init(void);
void TIM1_INIT( void );
void Uart0_Init( UINT32  baud );


/*******************************************************************************
* @function     TIM0_INIT
* @param        ticksPerSec ÿ�����жϵĴ�������
* @retval       None
* @brief        TIM0��ʱ���ĳ�ʼ���������ö�ʱ��������ΪUCOS��ϵͳʱ��
*******************************************************************************/
void TIM0_INIT( UINT32 ticksPerSec ) 
{
    /* �ⲿ������������չ */
    extern void IRQASMTimer0(void);                                             

    /* �ж�ӳ�����ã���TIM0��IRQ�ж�ӳ��������ж� */
    R8_SAFE_ACCESS_SIG = 0x57 ;  /* unlock step 1 */
    R8_SAFE_ACCESS_SIG = 0xA8 ;  /* unlock step 2 */
    
    R32_INT_VEC_TMR0 = MAKE_INT_VEC_IRQ( IRQASMTimer0 );                        /* ����TIM0 IRQ�жϴ�����ת��ַ */
    R8_INT_VEC_IRQ |= (RB_IV_IRQ_TMR0 );		                                /* ����ӳ��TIM0��IRQΪ�����ж� */
    
    R8_SAFE_ACCESS_SIG = 0x00 ;  /* lock, to prevent unexpected writing */

    /* ����TIM0�����ô���ʱ�䣬�ж����� */
    R32_TMR0_CNT_END = 100000000/ticksPerSec;	            				    /* ���ö�ʱ������ʱ�� */
    R8_TMR0_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
    R8_TMR0_INT_FLAG = 0xff;                                                    /* ������е��жϱ�־ */                                                               
    R8_TMR0_INTER_EN = RB_TMR_IE_CYC_END;      				                    /* ��ʱ��ʱ�����ڽ����ж�ʹ�ܣ�PWM �����ж�ʹ�� */

    /* ʹ���ж� */
    R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR0;                                          /* ����ʱ��0����жϲ��� */
    R8_INT_EN_IRQ_GLOB |= RB_IE_IRQ_GLOB;                                       /* ����FIQȫ���ж� */

    /* ϵͳʱ�ӿ�ʼ���� */
    R8_TMR0_CTRL_MOD |= RB_TMR_COUNT_EN;										/* ��ʼ���� */    
	
}

/*******************************************************************************
* @function     IRQ_C_Timer0
* @param        None
* @retval       None
* @brief        TIM0�����жϷ��������ú�����Ҫʹ��__irq����ָ��Ϊ�жϺ�����
*               Ҳ��Ҫ��Ӧ�ó����е��øú�����
*******************************************************************************/
void IRQ_C_Timer0( void )
{
	#if OS_CRITICAL_METHOD == 3                                
    OS_CPU_SR  cpu_sr;
	#endif

    OS_ENTER_CRITICAL();
    
    /* �����ʱ���жϱ�־���Լ�������ֵ���� */
    R8_INT_EN_IRQ_0 &= ~RB_IE_IRQ_TMR0;		                                    /* �رն�ʱ���ж� */
    R8_TMR0_CTRL_MOD |= RB_TMR_ALL_CLEAR;
    R8_TMR0_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
    R8_TMR0_INT_FLAG |= 0xFF;
    R8_INT_EN_IRQ_0 |= RB_IE_IRQ_TMR0;		                                    /* ������ʱ���ж� */

    OS_EXIT_CRITICAL();

    OSTimeTick();
}

/*******************************************************************************
* @function     Task1
* @param        pdata
* @retval       None
* @brief        ����1�ĺ���
*******************************************************************************/
void  Task1 (void *pdata) 
{
	pdata = pdata;                                                              /* �������� */

	for(;;) 
    {
        printf("task1 print CPU Usage:%d\r\n",OSCPUUsage);                      /* ��ӡCPUʹ������Ϣ */
        
        OSTimeDlyHMSM(0,0,4,0);                                                 /* ��ʱ4���� */
    }
}


/*******************************************************************************
* @function     Task2
* @param        pdata
* @retval       None
* @brief        ����2�ĺ���
*******************************************************************************/
void Task2( void *pdata )
{   
    pdata = pdata;                                                              /* �������� */
    
    OSTimeSet(  1000000000 );                                                   /* ����ϵͳʱ�� */
        
    for(;;)
    {    
        printf("task2 OSTime:%d\r\n",OSTimeGet());                              /* ��ӡϵͳʱ�� */
        
        OSTimeDlyHMSM(0,0,2,0);                                                 /* ��ʱ2���� */
    }
}

/*******************************************************************************
* @function     TaskStart
* @param        pdata
* @retval       None
* @brief        ��������ĺ���
*******************************************************************************/
void TaskStart(void *pdata)
{
    pdata = pdata;                                                              /* Prevent compiler warning                 */
    
    BSP_init();  
    TIM1_INIT();    

    /* ��ʼ��cpuʹ����ͳ�����������Ҫʹ�ø�ͳ�ƹ��ܣ��ú�������Ҫ���� */
    OSStatInit();                                                               /* cpuʹ����ͳ�������ʼ�� */

    /* �����û����� */
    OSTaskCreate(Task1, (void *)0, &Task1Stk[TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Task2, (void *)0, &Task2Stk[TASK_STK_SIZE - 1], 5);
    
    OSTaskDel(0);                                                               /* ����ʼ����ɾ���Լ� */  
}


/*******************************************************************************
* @function     BSP_init
* @param        None
* @retval       None
* @brief        BSP��ʼ������
*******************************************************************************/
void BSP_init(void) 
{    
    ///�ɴ����������Ҫ��������Ҳ�������û�������

    Uart0_Init( 115200 );                                                       
    printf("stdio init ok.\r\n");

    /* TIM0��ʼ����TIIM0��ΪUCOS��ϵͳʱ�ӣ��ú���������� */
    TIM0_INIT( OS_TICKS_PER_SEC );
}

/*******************************************************************************
* @function     TIM1_INIT
* @param        None
* @retval       None
* @brief        TIM1�ĳ�ʼ������������Ϊÿ�봥��һ���ж�
*******************************************************************************/
void TIM1_INIT( void )
{
    /* ����TIM1�����ô���ʱ�䣬�ж����� */
    R32_TMR1_CNT_END = 100000000/1;	            				                /* ���ö�ʱ������ʱ�� */
    R8_TMR1_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
    R8_TMR1_INT_FLAG = 0xff;                                                    /* ������е��жϱ�־ */                                                               
    R8_TMR1_INTER_EN = RB_TMR_IE_CYC_END;      				                    /* ��ʱ��ʱ�����ڽ����ж�ʹ�ܣ�PWM �����ж�ʹ�� */

    /* ʹ���ж� */
    R8_INT_EN_FIQ_0 |= RB_IE_FIQ_TMR1;                                          /* ����ʱ��1���IRQ�жϲ��� */
    R8_INT_EN_FIQ_GLOB |= RB_IE_FIQ_GLOB;                                       /* ����FIQȫ���ж� */

    /* ϵͳʱ�ӿ�ʼ���� */
    R8_TMR1_CTRL_MOD |= RB_TMR_COUNT_EN;										/* ��ʼ���� */    

}

/*******************************************************************************
* @function     Uart0_Init
* @param        baud-���ڲ����ʣ����Ϊ��Ƶ1/8
* @retval       None
* @brief        ����0��ʼ��
*******************************************************************************/
void Uart0_Init( UINT32  baud )
{
    UINT32 x;

    x = 10 * FREQ_SYS/ 8 / baud;                                                /* ���ô���0������ */
    x += 5;                                                                     /* �������� */
    x /= 10;
    R8_UART0_LCR = RB_LCR_DLAB;                                                 /* DLABλ��1 */
    R8_UART0_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;              
    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R8_UART0_MCR = RB_MCR_OUT1;
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PU  |= RXD0;                                                         /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 ������Ϊ��� */
}


/*******************************************************************************
* @function     fputc
* @param        c  -writes the character specified by c 
* @param        *f -the output stream pointed to by *f
* @retval       None
* @brief        ͨ��������������Ϣ
*******************************************************************************/
int fputc( int c, FILE *f )
{
    R8_UART0_THR = c;                                                           /* �������� */
    while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* �ȴ����ݷ��� */
    return( c );
}

/***************************End of file.***************************************/



/********************************** (C) COPYRIGHT *******************************
* File Name          : PWM_WAVE.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH56X ADC_QUERY DEMO
*                      (1)��CH56X Examples by KEIL;    
*                      (2)������0��������Ϣ,115200bps;
*                      (3)����������ʾ��ʱ��0���PWM����.����һ����Ƶ���ݡ�
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "WAVE.H"

/******************************************************************************/
/* �������� */
extern UINT8 sound[];                                                           /* ԭ����wave.c�ļ������� */
extern UINT8 *pdata_addr;                                                       /* ���������ݿ�ʼ��λ�õĵ�ַ���ⲿ������ԭ����wave.c�ļ������� */
extern UINT32 datasize;                                                         /* ������ʵ�����ݴӳ��ȣ��ⲿ������ԭ����wave.c�ļ������� */
extern UINT32 CountEnd;
UINT8 PWM_Flag = 0;
UINT8 PlayTimes = 0;
UINT8 *pdata_now;
/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

__irq void IRQ_Handler( void )
{
    UINT8 i;

    if(R8_INT_FLAG_0 & RB_IF_TMR0){                                             /* �ж��ǲ��Ƕ�ʱ��0��ص��ж� */
        i= R8_TMR0_INT_FLAG;
        if(i&RB_TMR_IF_CYC_END){
            PlayTimes++;                                                        /* ���Ŵ������� */
            if(PlayTimes==4){                                                   /* ͬһ���������ݲ��Ŵ���������pwmƵ�ʣ�ʹ����Ч���� */
                R32_TMR0_FIFO = (UINT32)(CountEnd*((*pdata_now)/256.0));        /* ���ݲ���ֵ���Ӧ����������з������ֵֵ */
                pdata_now++;                                                    /* ָ���Լ� */
                if((pdata_now-pdata_addr)>datasize){                            /* ������ڲ��ŵ����ļ��Ľ�β�ˣ���ô���¿�ʼ���� */
                    R8_INT_EN_IRQ_0 &= ~RB_IE_IRQ_TMR0;                         /* ʧ�ܶ�ʱ��0����жϲ��� */
                    R8_INT_EN_IRQ_GLOB &= ~RB_IE_IRQ_GLOB;                      /* �ر�IRQȫ���ж� */
                    PRINT("Play End\n");                                        /* ��ӡ��Ϣ */
                    goto IRQ_End;                                               /* ��ת���жϺ��������� */
                }
                PlayTimes = 0;
            }
IRQ_End:
            R8_TMR0_INT_FLAG=RB_TMR_IF_CYC_END;                                 /* �����Ӧ�жϱ�־ */
            R8_INT_FLAG_0 |= RB_IF_TMR0;                                        /* �����Ӧ�жϱ�־ */
        }
    }
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : FIQ�жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void FIQ_Handler( void )
{
    while(1);
}

/*******************************************************************************
* Function Name  : IRQ_Init
* Description    : IRQ�жϳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IRQ_Init( void )
{
    R8_TMR0_INT_FLAG=0xff;                                                      /* ������е��жϱ�־ */
    R8_TMR0_INTER_EN=RB_TMR3_FORCE_EN|RB_TMR_IE_CYC_END|RB_TMR_IE_DATA_ACT;;    /* ��ʱ��ʱ�����ڽ����ж�ʹ�ܣ�PWM �����ж�ʹ�� */
    R8_INT_EN_IRQ_0=RB_IE_IRQ_TMR0;                                             /* ������ʱ��0����жϲ��� */
    R8_INT_EN_IRQ_GLOB= RB_IE_IRQ_GLOB;                                         /* ֻ����IRQȫ���ж� */
}

/*******************************************************************************
* Function Name  : Timer0_Init
* Description    : ��ʱ��0��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Timer0_Init( UINT32 end )
{
    R8_TMR0_CTRL_MOD=0x00;                                                      /* RB_TMR_MODE_COUNTλ��0 */
    R32_PB_DIR|=PWM0;                                                           /* PB0Ϊ��ʱ��0������ţ�����Ϊ��� */
    R32_TMR0_CNT_END=end;                                                       /* ����PWM���� */
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : Ϊprintf��getkey���������ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / 115200;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* �������� */
    R8_UART0_LCR = 0x80;                                                        /* DLABλ��1 */
    R8_UART0_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 output enable */
}

/*******************************************************************************
* Function Name  : fputc
* Description    : ͨ��������������Ϣ
* Input          : c-- writes the character specified by c 
*                  *f--the output stream pointed to by *f
* Output         : None
* Return         : None
*******************************************************************************/
int fputc( int c, FILE *f )
{
    R8_UART0_THR = c;                                                           /* �������� */
    while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );                        /* �ȴ����ݷ��� */
    return( c );
}

/*******************************************************************************
* Function Name  : main
* Description    : I������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main( void )
{
    mInitSTDIO( );                                                              /* Ϊ���ü����ͨ�����ڼ����ʾ���� */
    PRINT("System Init OK\n");
    IRQ_Init( );                                                                /* �жϳ�ʼ�� */
    filehead_load(sound);                                                       /* ���������е��������ݣ���ȡ�����в�������ص���Ϣ */
    Timer0_Init( CountEnd );                                                    /* �����ݲ��β���������ļ�����ֵд��Ĵ��� */
    pdata_now = pdata_addr;
    R32_TMR0_FIFO=(UINT32)(CountEnd*((*pdata_now)/256.0));                      /* �������еĵ�һ�����ݷ��� */
    R8_TMR0_CTRL_MOD|=(RB_TMR_COUNT_EN|RB_TMR_OUT_EN);                          /* ����������������� */
    while(1);
}

/*********************************** endfile **********************************/
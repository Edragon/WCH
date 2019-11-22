/********************************** (C) COPYRIGHT *******************************
* File Name          : FIQ.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH56X FIQ DEMO
*                      (1)��CH56X Examples by KEIL;    
*                      (2)������0��������Ϣ,115200bps;
*                      (3)��������ʵ��FIQ�жϹ���.
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#define DEBUG                  1                                                /* ���� */


#include <stdio.h>
#include <string.h> 
#include "CH563SFR.H"
#include "SYSFREQ.H"

/******************************************************************************/
/* �������� */
__packed union  _BUF {
    UINT8  buf[4096];
    UINT32 my_buffer[ 1024 ];
} pbuf;

/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED                     1<<3

#define LED_OUT_INIT(  )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }         /* LED �ߵ�ƽΪ������� */
#define LED_OUT_ACT(  )      { R32_PB_CLR |= LED; }                            /* LED �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT(  )    { R32_PB_OUT |= LED; }                            /* LED �ߵ�ƽ�ر�LED��ʾ */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϺ���
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
* Description    : FIQ�жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

__irq void FIQ_Handler( void )
{
    UINT16 i;
    /*�����ж�*/
    if( R8_INT_FLAG_0 & RB_IF_TMR0 ){                                           /* ��ʱ���ж� */
        PRINT("CAT_FIQ\n");
        if( R8_TMR0_INT_FLAG & RB_TMR_IF_DMA_END ){
            R8_INT_EN_FIQ_0 &= ~RB_IE_FIQ_TMR0;                                 /* �رն�ʱ���ж� */      
            R16_TMR0_DMA_BEG = (UINT32)&pbuf.my_buffer[0];                      /* �����ʹ��RB_TMR_DMA_LOOP��������Ҫ����װ��DMA��ʼ�ͽ��������� */
            R16_TMR0_DMA_END = (UINT32)&pbuf.my_buffer[64];
            /*����Ϊͨ�����ڼ�Ⲷ������*/
            PRINT("CAT_DATA:    \n");
            for( i=0;i!=64*4;i++ ){
                if(!(i%8)) PRINT("\n"); 
                PRINT("%8x",(UINT16)pbuf.buf[i]);
            }
            PRINT("\n\n");
            R8_TMR0_CTRL_DMA = ( RB_TMR_DMA_ENABLE | RB_TMR_DMA_BURST  );    
            /*������ʱ���Լ�PWM����*/ 
            R8_TMR0_CTRL_MOD |= ( RB_TMR_COUNT_EN );
            R8_TMR0_INT_FLAG = 0x7f;                                            /* ���ж�״̬ */
            R8_INT_EN_FIQ_0 |= RB_IE_FIQ_TMR0;                                  /* ������ʱ���ж� */
        }
    } 
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
* Function Name  : TIM0_INIT
* Description    : ����ģʽ��ʱ��0��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void TIM0_INIT( void ) 
{
    R32_PB_DIR &= ~CAT0;                                                        /* PB1ʹ������ */
    R8_TMR0_CTRL_MOD =  RB_TMR_MODE_IN ;                                        /* ��ʱ�����ģʽ���� */
    R8_TMR0_CTRL_MOD |= 1<<6;                                                   /* ����  00=disable, 01=edge change, 10=fall to fall, 11-rise to rise */
    R8_TMR0_CTRL_DMA &= 0x00;                                                   /* ��ֹDMA */
    R32_TMR0_CNT_END = 0xfffffff;                                               /* ��ʱ��  ������ʱ�� */
    /*����Ϊ�ж�����*/    
    R8_TMR0_INTER_EN |= RB_TMR_IE_DMA_END;                                      /* ��������DMA�����ж� */     
    R8_INT_EN_FIQ_0 |= RB_IE_FIQ_TMR0;                                          /* ������ʱ�� */
    /*����ΪDMA����*/     
    R16_TMR0_DMA_BEG = (UINT32)&pbuf.my_buffer[0];                              /* DMA��ʼ��������ַ */
    R16_TMR0_DMA_END = (UINT32)&pbuf.my_buffer[64];                             /* DMA������������ַ */

    PRINT("DMA_BEG =%8x\n",(UINT16)R16_TMR0_DMA_BEG);                           /* �����DMA��ʼ�ͽ�����ַ�������ַ�쳣��Ҫ�������� */
    PRINT("DMA_END =%8x\n",(UINT16)R16_TMR0_DMA_END);                           /* ��ַ����Ϊ4�������� */

    R8_TMR0_CTRL_DMA = ( RB_TMR_DMA_ENABLE | RB_TMR_DMA_BURST );                /* ��ʼDMA */    
    /*������ʱ������*/ 
    R8_TMR0_CTRL_MOD |= RB_TMR_COUNT_EN ;                                       /* ������ʱ���������� */
    R8_TMR0_INT_FLAG = 0x7f;                                                    /* ���ж� */
    R8_INT_EN_FIQ_GLOB |= RB_IE_FIQ_GLOB;                                       /* ����ȫ���ж� */
    R8_TMR0_INT_FLAG = 0x7f;                                                    /* ���ж� */
} 

/*******************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main( void ) 
{ 
    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* ������LED��һ����ʾ���� */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    mInitSTDIO( );                                                              /* Ϊ���ü����ͨ�����ڼ����ʾ���� */ 
    PRINT( "Start\n" );
    TIM0_INIT( );                                                               /* ����DMA��ʼ������ */
    while(1);                                                                   /* �ȵ�����DMA�ж� */
}

/*********************************** endfile **********************************/
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH563 TFT DEMO
*                      ����оƬ6804��֧�ֺ�����������Ӣ���ַ���ͼ�Σ����֣�ͼƬ����ʾ
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "..\PARA\PARA_TFT\LED\LED.H"
#include "..\PARA\PARA_TFT\LCD\LCD.H"
#include "..\PARA\PARA_TFT\KEY\KEY.H"
#include "..\PARA\PARA_TFT\CHAR\CHAR.H"
#include "..\PARA\PARA_TFT\CHINESE\CHINESE.H"
#include "..\PARA\PARA_TFT\PICTURE\PICTURE.H"
#include "..\PARA\PARA_TFT\EXTEST\EXTEST.H"
#include "..\PARA\PARA_TFT\TP\TP.H"

/******************************************************************************/
extern void show_pic(const unsigned short *p);

/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED_OUT_INIT( LED )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }       /* LED �ߵ�ƽΪ������� */
#define LED_OUT_ACT( LED )      { R32_PB_CLR |= LED; }                          /* LED �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT( LED )    { R32_PB_OUT |= LED; }                          /* LED �ߵ�ƽ�ر�LED��ʾ */

#define LED                     1<<3

/* ѡ��ͨѶ�ӿ� */    
#define  IC_OP_INTERFACE_MODE                 2                                 /* ����оƬ�����ӿ�ģʽ(1:Ӳ������;  2:����ģ�Ⲣ��; */ 
#if    ( IC_OP_INTERFACE_MODE == 1 )
#include "PARA_HW.C"                                                            /* Ӳ�����߲������ӷ�ʽ */
#elif  ( IC_OP_INTERFACE_MODE == 2 )
#include "PARA_SW.C"                                                            /* ����ģ�Ⲣ�����ӷ�ʽ */
#endif


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
    while(1);
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
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main( void )
{
    LED_OUT_INIT( LED );
    LED_OUT_ACT( LED );
    Delay_ms(300);
    LED_OUT_INACT( LED );
    mInitSTDIO( );
    PRINT("TFT DEMO...\r\n");
    CH563_PORT_INIT( );
    LED_Init( );
    LCD_Init( );
    KEY_INIT( );
    LCD_Clear(BLUE);                                                            /* ���� */
    Delay_ms(800);
    while(1){
#if 1
        LCD_Clear(WHITE);                                                       /* ���� */    
        TEST_CHAR( );                                                           /* �ַ�����ʾʾ�� */ 
        Delay_ms(3000);
        LCD_Clear(WHITE);                                                       /* ���� */     
        EX_TEST( );                                                             /* ��չ����,��ʾͼ�� */
        Delay_ms(3000);
        LCD_Clear(WHITE);                                                       /* ���� */
        TEST_FONT();                                                            /* ������ʾʾ�� */ 
        Delay_ms(3000);
        LCD_Clear(WHITE);                                                       /* ���� */
        show_pic(PIC_DATA);                                                     /* ͼƬ��ʾʾ�� */
        Delay_ms(5000);
#endif
#if 0
        LCD_Clear(BLACK);                                                       /* ���� */
        TP_DrawLine_TEST();
#endif
    }
}

/*********************************** endfile **********************************/
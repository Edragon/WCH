/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH563 USB WAKE_INT DEMO
*                      (1)��CH563 USB Device Examples by KEIL;    
*                      (2)������0��������Ϣ,115200bps;
*                      (3)���ó������жϷ�ʽ����,PB3����һ���ź�(�͸ߵ�ƽ�ź�)���ɲ��������ź�;
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "CH563USBSFR.H"                                                        /* оƬUSB�Ĵ������ͷ�ļ� */
#include "USB_DEVICE.H"                                                         /* USB�豸�������ͷ�ļ� */
#include "PRINTF.H"                                                             /* ���ڴ�ӡ�������ͷ�ļ� */
/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED                     1<<4

#define LED_OUT_INIT(  )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }          /* LED �ߵ�ƽΪ������� */
#define LED_OUT_ACT(  )      { R32_PB_CLR |= LED; }                             /* LED �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT(  )    { R32_PB_OUT |= LED; }                             /* LED �ߵ�ƽ�ر�LED��ʾ */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϴ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void IRQ_Handler( void )
{
    if( !(R32_PB_PIN & (1<<3)) ) {
        Delay_ms( 100 );
        if( !(R32_PB_PIN & (1<<3)) ) {
            R32_PA_DIR |= 1<<8;
            R32_PA_OUT &= ~( 1<<8 );
            while( !(R32_PB_PIN & (1<<3)) );
            R32_PA_OUT |= ( 1<<8 );
#if  MY_DEBUG_PRINTF    
            printf("GPIO PB input = %8x\n",(UINT32)R32_PB_PIN&(1<<3));
#endif
            R8_SAFE_ACCESS_SIG = 0x57;                                          /* unlock step 1 */ 
            R8_SAFE_ACCESS_SIG = 0xA8;                                          /* unlock step 2 */ 
//            R8_SLP_WAKE_CTRL |= RB_SLP_USB_WAKE;                              /* USB����ʹ�� */ 
            R8_SLP_WAKE_CTRL |= RB_SLP_AP_WAK_USB;                              /* Ӧ�ó�����USB���� */ 
            R8_SAFE_ACCESS_SIG = 0;
            Delay_ms( 20 );

            R8_SAFE_ACCESS_SIG = 0x57;        
            R8_SAFE_ACCESS_SIG = 0xA8;         
//            R8_SLP_WAKE_CTRL &= ~RB_SLP_USB_WAKE;                             /* ���ѽ�ֹ */
            R8_SLP_WAKE_CTRL &= ~RB_SLP_AP_WAK_USB;                             /* �رջ����ź� */ 
            R8_SAFE_ACCESS_SIG = 0;    
        }
    }
    if( R8_INT_FLAG_1 & RB_IF_USB ){                                            /* USB�ж� */
        if( USB_GL_INT_STATUS & RB_HC_INT ){
//            USBHOST_EHCI_IRQHandler( );                                       /* USB�����жϴ������� */
        }
        else if( USB_GL_INT_STATUS & RB_DEV_INT ){
            USBDev_IRQHandler( );                                               /* USB�豸�жϴ������� */
        }
    }
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : FIQ�жϴ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__irq void FIQ_Handler( void )
{
    while( 1 );
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
    LED_OUT_INIT( );
    LED_OUT_ACT( );                                                             /* ������LED��һ����ʾ���� */
    Delay_ms( 100 );
    LED_OUT_INACT( );
    R32_PB_DIR &= ~(1<<3);
    R32_PB_PU  |= (1<<3);
    mInitSTDIO( );                                                              /* Ϊ���ü����ͨ�����ڼ����ʾ���� */

#if  MY_DEBUG_PRINTF    
    printf( "CH563 Test\n" );
#endif
    /************************** �豸ģʽ���� **************************/       
    USBDev_ModeSet( );                                                          /* USB����Ϊ�豸ģʽ */
    USBDev_Init( );                                                             /* USB�豸��ʼ�� */
    USBDev_UsbInt_Enable( );                                                    /* USB�豸������ж�ʹ�� */
    while( 1 );
}

/*********************************** endfile **********************************/
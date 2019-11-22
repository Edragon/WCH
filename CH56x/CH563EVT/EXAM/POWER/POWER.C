/********************************** (C) COPYRIGHT *******************************
* File Name          : POWER.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH563 USB SLEEP DEMO
*                      (1)��CH563 Examples by KEIL;    
*                      (2)������0��������Ϣ,115200bps;
*                      (3)��ͨ������0������Ӧ������ʾ�رա��������ֹ��ܣ�˯�ߣ����ѵ�
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "POWER.H"                                                              /* оƬUSB�Ĵ������ͷ�ļ� */
#include "SYSFREQ.H"

UINT16    my_buffer[ 0x2000 ];

/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED                     1<<3

#define LED_OUT_INIT(  )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }         /* LED �ߵ�ƽΪ������� */
#define LED_OUT_ACT(  )      { R32_PB_CLR |= LED; }                            /* LED �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT(  )    { R32_PB_OUT |= LED; }                            /* LED �ߵ�ƽ�ر�LED��ʾ */

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϴ�������
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
* Description    : FIQ�жϴ�������
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
* Function Name  : USBHost_ModeSet
* Description    : ���ó�USB����ģʽ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBHost_ModeSet( void )
{ 
    R8_MISC_CTRL_USB = R8_MISC_CTRL_USB & ( ~RB_MISC_USB_ID_EN );
    R8_MISC_CTRL_USB = R8_MISC_CTRL_USB & ( ~RB_MISC_USB_ID_ST );
    OTG_CONTROL_STATUS = 0x000E0190;
}

/*******************************************************************************
* Function Name  : USBDev_ModeSet
* Description    : ���ó�USB�豸ģʽ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDev_ModeSet( void )
{ 
    R8_MISC_CTRL_USB = R8_MISC_CTRL_USB & ( ~RB_MISC_USB_ID_EN );               /* USB OTG ID pin enable */                                    
    R8_MISC_CTRL_USB = R8_MISC_CTRL_USB | RB_MISC_USB_ID_ST;                    /* ���ó�USB�豸 */
}

/*******************************************************************************
* Function Name  : WakeUP_PB
* Description    : PB[8]�������ó�ʼ�� 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void WakeUP_PB( void )
{
    R32_PB_DIR &= 0xffffff00;                                                   /* GPIO B��������Ϊ���� , direction: 0=in, 1=out */
    R32_PB_PU  |= 0x000000ff;                                                   /* GPIO B�������ã���1��ʾ���� */ 
    Delay_us(10);
    R32_INT_ENABLE_PB |= 0x000000ff;                                            /* GPIO B���λ�ж�ʹ�� �� 1-ʹ�ܣ�0-��ֹ */
    R32_INT_POLAR_PB  &= 0;                                                     /* PB[8]�͵�ƽ���� */      
}

/*******************************************************************************
* Function Name  : Test_Sleep
* Description    : ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Test_Sleep( void )
{
    UINT32    i;

    if ( R8_UART0_LSR & RB_LSR_DATA_RDY ) {                                     /* ͨ����������0x00001234���Ƶ�32λ��ֵַ */
        Delay_ms( 20 );
        for ( i=0; i<10; i++ ){
            if ( R8_UART0_LSR & RB_LSR_DATA_RDY ) {
                my_buffer[i] = R8_UART0_RBR;
                if ( my_buffer[i] >= 'a' && my_buffer[i] <= 'z' ) my_buffer[i] -= 'a' - 'A';
                if ( i >= 2 ){
                    if ( my_buffer[i] >= '0' && my_buffer[i] <= '9' ) my_buffer[i] -= '0';
                    else if ( my_buffer[i] >= 'A' && my_buffer[i] <= 'F' ) my_buffer[i] = my_buffer[i] - 'A' + 10;
                    else i = 99;
                }
            }
            else break;
        }
        if ( i == 10 && my_buffer[0] == '0' && my_buffer[1] == 'X' ) {
            i = ( my_buffer[2] << 12 ) | ( my_buffer[3] << 8 ) | ( my_buffer[4] << 4 ) | my_buffer[5];
            i <<= 16;
            i |= ( my_buffer[6] << 12 ) | ( my_buffer[7] << 8 ) | ( my_buffer[8] << 4 ) | my_buffer[9];
            printf("recv = %8lX\n",i);
            i = *((PUINT32)i);
            printf("Data = %8lX\n",i);
        }
        else { 
            printf("my_buffer=%x\n",my_buffer[0]);      
            switch (my_buffer[0]){
                case 1:                                                         /* �Ĵ���д��0x95����ʹ��PLL����ȫ������״̬(����CPU)����֧�ֲ���
                                                                                   GPIO ����USB ����ETH-WOL ���� */
                    printf("SLEEP\n");
                    R8_SAFE_ACCESS_SIG = 0x57;                                  /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8;                                  /* unlock step 2 */
                    R8_SLP_CTRL_PLL = 0x95;                                     /* д0x95 */
                    R8_SLP_WAKE_CTRL  |= RB_SLP_PB_WAKE;                        /* PB�˿ڻ���ʹ�� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;

                case 2:                                                         /* �Ĵ���д��0x6A����ʹ��CPU�������״̬����ʱPLL��Ȼ�ڹ�����CPU
                                                                                   ���Ա���ʹ�ܵĸ����жϿ��ٻ��� */
                    printf("CPU \n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */
                    R8_SLP_CTRL_PLL = 0x6A;                                     /* д0x6A */
                    R8_SLP_WAKE_CTRL  |= RB_SLP_PB_WAKE;                        /* PB�˿ڻ���ʹ�� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;

                case 3:                                                         /* ��̫��ģ���ϵ� */
                    printf("ETH PWR ON\n");
                    R8_SAFE_ACCESS_SIG = 0x57;                                  /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8;                                  /* unlock step 2 */
                    R8_SLP_WAKE_CTRL  &= 0x0b;                                  /* �ϵ� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;

                case 4:                                                         /* ��̫��ģ���µ� */
                    printf("ETH PWR OFF\n");
                    R8_SAFE_ACCESS_SIG = 0x57;                                  /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8;                                  /* unlock step 2 */
                    R8_SLP_WAKE_CTRL  |= 0x80;                                  /* �µ� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;

                case 5:                                                         /* ��ADC��PHY */
                    printf("open ADC PHY\n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */    
                    R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_ADC;                
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    R8_ADC_CTRL_MOD |= RB_ADC_POWER_ON;                         /* ADC����ģ��ʹ�� */
                    break;

                case 6:                                                         /* �ر�ADC��PHY */
                    printf("close ADC PHY\n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */    
                    R8_SLP_CLK_OFF1 |= RB_SLP_CLK_ADC;                
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    R8_ADC_CTRL_MOD &= ~RB_ADC_POWER_ON;                        /* ADC����ģ���ֹ */
                    break;

                case 7:                                                         /* �ر�����ʱ�� */
                    printf("close clk\n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */
                    R8_SLP_CLK_OFF0 |= ( RB_SLP_CLK_UART1 | RB_SLP_CLK_UART0 | 
                                       RB_SLP_CLK_TMR0 | RB_SLP_CLK_TMR1 |
                                       RB_SLP_CLK_TMR2 | RB_SLP_CLK_TMR3 | 
                                       RB_SLP_CLK_SPI0 | RB_SLP_CLK_SPI1 );     /* �ر�ʱ�ӿ��ƼĴ���0 */
                    R8_SLP_CLK_OFF1 |= ( RB_SLP_CLK_ADC | RB_SLP_CLK_GPIO | 
                                       RB_SLP_CLK_USB | RB_SLP_CLK_ETH );       /* �ر�ʱ�ӿ��ƼĴ���1 */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    Delay_ms( 1000 );
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */
                    R8_SLP_CLK_OFF0 &= ~RB_SLP_CLK_UART0;                       /* 1s��������0ʱ�� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    Delay_us( 10 );
                    printf("\nOK\n");
                   break;    

                case 8:                                                         /* ��������ʱ�� */
                    printf("open clk\n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */
                    R8_SLP_CLK_OFF0 &= 0x00;                                    /* ����ʱ�ӿ��ƼĴ���0 */
                    R8_SLP_CLK_OFF1 &= 0x0f;                                    /* ����ʱ�ӿ��ƼĴ���1 */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;    

                case 9:                                                         /* �ر�USB */
                    printf("close usb\n");
                    USBHost_ModeSet( );    
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */                     
//                  USB_DEV_CONTROL |= RB_GOSUSP;                               /* �豸ģʽPHY���� */
                    HC_MISCELLANEOUS |= (1<<6);                                 /* ����ģʽ�շ������� */
                    R8_SLP_CLK_OFF1 |= RB_SLP_CLK_USB;                          /* �ر�USBʱ�� */
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    break;

                 case 10:                                                       /* ����USB����ģʽ */
                    printf("USB Host\n");
                    R8_SAFE_ACCESS_SIG = 0x57 ;                                 /* unlock step 1 */
                    R8_SAFE_ACCESS_SIG = 0xA8 ;                                 /* unlock step 2 */                     
                    R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_USB;                         /* ����USBʱ�� */
                    HC_MISCELLANEOUS &= ~(1<<6);
                    R8_SAFE_ACCESS_SIG = 0;                                     /* ��������ֹ��д */
                    USBHost_ModeSet( );    
                    break;

                 default:
                    printf("!!!! input error !!!\n");
                    printf(" 1................PLL����ȫ������״̬,PB[8]�͵�ƽ����\r\n");
                    printf(" 2................CPU �������״̬��PB[8]�͵�ƽ����\r\n");
                    printf(" 3................��̫��ģ���ϵ�\r\n");
                    printf(" 4................��̫��ģ���µ�\r\n");
                    printf(" 5................��ADC��PHY\r\n");
                    printf(" 6................�ر�ADC��PHY\r\n");
                    printf(" 7................�ر�����ʱ��\r\n");
                    printf(" 8................��������ʱ��\r\n");
                    printf(" 9................�ر�USB��ʱ�Ӻ�PHY��\r\n");
                    printf(" A................����USB����ģʽ\r\n");
                    break;
            } 
        }
        while ( R8_UART0_LSR & RB_LSR_DATA_RDY ) i = R8_UART0_RBR;
    }
    for ( i=0; i!=5; i++ ) {
        LED_OUT_ACT( );
        Delay_ms( 100 );
        LED_OUT_INACT( );
        Delay_ms( 100 );
        if ( R8_UART0_LSR & RB_LSR_DATA_RDY ) break;
    }
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
    LED_OUT_ACT( );
    LED_OUT_INACT( );
    printf( "Start test\n" );
    printf(" 1................PLL����ȫ������״̬,PB[8]�͵�ƽ����\r\n");
    printf(" 2................CPU �������״̬��PB[8]�͵�ƽ����\r\n");
    printf(" 3................��̫��ģ���ϵ�\r\n");
    printf(" 4................��̫��ģ���µ�\r\n");
    printf(" 5................��ADC��PHY\r\n");
    printf(" 6................�ر�ADC��PHY\r\n");
    printf(" 7................�ر�����ʱ��\r\n");
    printf(" 8................��������ʱ��\r\n");
    printf(" 9................�ر�USB��ʱ�Ӻ�PHY��\r\n");
    printf(" A................����usb����ģʽ\r\n");
    WakeUP_PB( );
    while(1){
        Test_Sleep( );
    }
}

/*********************************** endfile **********************************/
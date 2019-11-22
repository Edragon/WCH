/********************************** (C) COPYRIGHT *******************************
* File Name          : EEPROM.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : CH56X EEPROM DEMO
*                      (1)��CH56X Examples by KEIL;    
*                      (2)������0��������Ϣ,115200bps;
*                      (3)��������ʵ��EEPROM��ȡ�����Լ���̹��̣�CH56XоƬ��28K��EEPROM
*                      (4)��д�����ݿ���ͨ����Ƭ����ȡ����Ҳ����ʹ��"WCHISPTool_Setup.exe"������ȡ
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "ISPXT56X.H"

/******************************************************************************/
/* �������� */
UINT8 my_buffer[ 0x2000 ];

/* ����һ��LED���ڼ����ʾ����Ľ���,�͵�ƽLED�� */
#define LED                     1<<3

#define LED_OUT_INIT(  )     { R32_PB_OUT |= LED; R32_PB_DIR |= LED; }          /* LED �ߵ�ƽΪ������� */
#define LED_OUT_ACT(  )      { R32_PB_CLR |= LED; }                             /* LED �͵�ƽ����LED��ʾ */
#define LED_OUT_INACT(  )    { R32_PB_OUT |= LED; }                             /* LED �ߵ�ƽ�ر�LED��ʾ */

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
* Function Name  : CH56X_EEPROM
* Description    : EEPROM�����ӳ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void CH56X_EEPROM( void ) 
{
    UINT32 i;
    
    PRINT("Eeprom Read:\n");
    EEPROM_READ( 0x0000,my_buffer,64 );                                         /* EEPROM��ȡ��ַ0x0000���ݣ���ȡ64�ֽ� */
    for( i=0;i!=64;i++ ){
        if(i%8 == 0) PRINT("\n");
        PRINT("%02x ",(UINT16)my_buffer[i]);
    }
    PRINT("\n");

    PRINT("Eeprom Erase:\n");
    i = EEPROM_ERASE(0x2000,0x1000);                                            /* ����0X2000��ַ��ʼ��4K���� */
    if( i ){
        PRINT("    ***Error***\n");
    }

    for( i=0;i!=64;i++ )my_buffer[i] = i;
    PRINT("Write_Eeprom:\n");
    i = EEPROM_WRITE( 0x2000,my_buffer,64 );                                    /* ����ַ0X2000д64�ֽڵ����� */
    if( i ){
        PRINT("    ***Error***\n");
    }

    PRINT("Eeprom Read:\n");
    EEPROM_READ( 0x2000,my_buffer,64 );                                         /* EEPROM��ȡ��ַ0x0000���ݣ���ȡ64�ֽ� */
    for( i=0;i!=64;i++ ){
        if(i%8 == 0) PRINT("\n");
        PRINT("%02x ",(UINT16)my_buffer[i]);
    }
    PRINT("\n");
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
    PRINT( "Start Eeprom Tset\xd\xa" );
    CH56X_EEPROM( );
    while(1);
}

/*********************************** endfile **********************************/
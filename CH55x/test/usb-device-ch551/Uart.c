
#include "Type.h"
#include "Ch552.h"
#include "System.h"
#include "Uart.h"

/*******************************************************************************
* Function Name  : CH554UART0Alter()
* Description    : CH554����0����ӳ��,����ӳ�䵽P0.2��P0.3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH554UART0Alter()
{
    PIN_FUNC |= bUART0_PIN_X;                                                  //����ӳ�䵽P1.2��P1.3
}

/*******************************************************************************
* Function Name  : InitUART0()
* Description    : CH554����0��ʼ��,Ĭ��ʹ��T1��UART0�Ĳ����ʷ�����,Ҳ����ʹ��T2
                   ��Ϊ�����ʷ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitUART0()
{
    UINT32 x;
    UINT8 x2; 

	//����0ʹ��ģʽ1
    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   
    
    //ʹ��Timer1��Ϊ�����ʷ�����                                                                          
    RCLK = 0;
    TCLK = 0;
    
    PCON |= SMOD;
    x = 10 * FREQ_SYS / UART0_BUAD / 16;                                       //���������Ƶ��ע��x��ֵ��Ҫ���                            
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //��������

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20��Timer1��Ϊ8λ�Զ����ض�ʱ��
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1ʱ��ѡ��
    TH1 = 0-x;                                                                 //12MHz����,buad/12Ϊʵ�������ò�����
    TR1 = 1;                                                                   //������ʱ��1
    TI = 0;
    REN = 1;                                                                   //����0����ʹ��

    ES = 1;
}

/*******************************************************************************
* Function Name  : CH554UART0RcvByte()
* Description    : CH554UART0����һ���ֽ�
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8  CH554UART0RcvByte( )
{
    while(RI == 0);                                                            //��ѯ���գ��жϷ�ʽ�ɲ���
    RI = 0;
    return SBUF;
}

static BOOL volatile s_sent = FALSE;

void SetUart0Sent(void)
{
	s_sent = TRUE;
}

/*******************************************************************************
* Function Name  : CH554UART0SendByte(UINT8 SendDat)
* Description    : CH554UART0����һ���ֽ�
* Input          : UINT8 SendDat��Ҫ���͵�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH554UART0SendByte(UINT8 SendDat)
{
	s_sent = FALSE;
	SBUF = SendDat;
	while (!s_sent);
}

void CH554UART0SendData(const UINT8 *pData, UINT8 len)
{
	while (len-- > 0)
	{
		s_sent = FALSE;
		SBUF = *pData++;
		while (!s_sent);
	}
}

/*******************************************************************************
* Function Name  : UART1Setup()
* Description    : CH554����1��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1Setup( )
{
   U1SM0 = 0;                                                                   //UART1ѡ��8λ����λ
   U1SMOD = 1;                                                                  //����ģʽ
   U1REN = 1;                                                                   //ʹ�ܽ���
   SBAUD1 = 0 - FREQ_SYS/16/UART1_BUAD;
}

/*******************************************************************************
* Function Name  : CH554UART1RcvByte()
* Description    : CH554UART1����һ���ֽ�
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8  CH554UART1RcvByte( )
{
    while(U1RI == 0);                                                           //��ѯ���գ��жϷ�ʽ�ɲ���
    U1RI = 0;
    return SBUF1;
}

/*******************************************************************************
* Function Name  : CH554UART1SendByte(UINT8 SendDat)
* Description    : CH554UART1����һ���ֽ�
* Input          : UINT8 SendDat��Ҫ���͵�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH554UART1SendByte(UINT8 SendDat)
{
	SBUF1 = SendDat;                                                             //��ѯ���ͣ��жϷ�ʽ�ɲ�������2�����,������ǰ��TI=0
	while(U1TI ==0);
	U1TI = 0;
}


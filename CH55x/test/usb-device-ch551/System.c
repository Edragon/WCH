/********************************** (C) COPYRIGHT *******************************
* File Name          : Debug.C
* Author             : WCH
* Version            : V1.1
* Date               : 2017/09/05
* Description        : CH554 DEBUG Interface
                     CH554��Ƶ�޸ġ���ʱ��������
                     ����0�ʹ���1��ʼ��
                     ����0�ʹ���1���շ��Ӻ���
                     ���Ź���ʼ��										 
*******************************************************************************/

#include "Type.h"
#include "Ch552.h"

#include "System.h"

/*******************************************************************************
* Function Name  : CfgFsys( )
* Description    : CH554ʱ��ѡ������ú���,Ĭ��ʹ��Fsys 6MHz��FREQ_SYS����ͨ��
                   CLOCK_CFG���õõ�����ʽ���£�
                   Fsys = (Fosc * 4/(CLOCK_CFG & MASK_SYS_CK_SEL);����ʱ����Ҫ�Լ����� 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void	CfgFsys( )  
{
#ifdef OSC_EN_XT	
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
    CLOCK_CFG |= bOSC_EN_XT;                          //ʹ���ⲿ����
    CLOCK_CFG &= ~bOSC_EN_INT;                        //�ر��ڲ����� 
#endif	  
		SAFE_MOD = 0x55;
		SAFE_MOD = 0xAA;

#if FREQ_SYS == 32000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 32MHz
#elif FREQ_SYS == 24000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 24MHz
#elif FREQ_SYS == 16000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 16MHz
#elif FREQ_SYS == 12000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 12MHz
#elif FREQ_SYS == 6000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 6MHz
#elif FREQ_SYS == 3000000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz
#elif FREQ_SYS == 750000ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz
#elif FREQ_SYS == 187500ul
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5MHz
#else
#error "invalid freq"
#endif

    SAFE_MOD = 0x00;
}

/*******************************************************************************
* Function Name  : mDelayus(UNIT16 n)
* Description    : us��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void	mDelayuS( UINT16 n )  // ��uSΪ��λ��ʱ
{
#ifdef	FREQ_SYS
#if		FREQ_SYS <= 6000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 3000000
		n >>= 2;
#endif
#if		FREQ_SYS <= 750000
		n >>= 4;
#endif
#endif
	while ( n ) {  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}

/*******************************************************************************
* Function Name  : mDelayms(UNIT16 n)
* Description    : ms��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	mDelaymS( UINT16 n )                                                  // ��mSΪ��λ��ʱ
{
	while ( n ) {
#ifdef	DELAY_MS_HW
		while ( ( TKEY_CTRL & bTKC_IF ) == 0 );
		while ( TKEY_CTRL & bTKC_IF );
#else
		mDelayuS( 1000 );
#endif
		-- n;
	}
}                                         

/*******************************************************************************
* Function Name  : CH554WDTModeSelect(UINT8 mode)
* Description    : CH554���Ź�ģʽѡ��
* Input          : UINT8 mode 
                   0  timer
                   1  watchDog
* Output         : None
* Return         : None
*******************************************************************************/
void CH554WDTModeSelect(UINT8 mode)
{
   SAFE_MOD = 0x55;
   SAFE_MOD = 0xaa;                                                             //���밲ȫģʽ
   if(mode){
     GLOBAL_CFG |= bWDOG_EN;                                                    //�������Ź���λ
   }
   else GLOBAL_CFG &= ~bWDOG_EN;	                                            //�������Ź�������Ϊ��ʱ��
   SAFE_MOD = 0x00;                                                             //�˳���ȫģʽ
   WDOG_COUNT = 0;                                                              //���Ź�����ֵ
}

/*******************************************************************************
* Function Name  : CH554WDTFeed(UINT8 tim)
* Description    : CH554���Ź���ʱʱ������
* Input          : UINT8 tim ���Ź���λʱ������
                   00H(6MHz)=2.8s
                   80H(6MHz)=1.4s
* Output         : None
* Return         : None
*******************************************************************************/
void CH554WDTFeed(UINT8 tim)
{
   WDOG_COUNT = tim;                                                             //���Ź���������ֵ	
}

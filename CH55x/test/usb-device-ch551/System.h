/* ���� */
/* �ṩprintf�ӳ������ʱ���� */
/* CH554��Ƶ�޸ġ���ʱ��������
   ����0�ʹ���1��ʼ��
   ����0�ʹ���1���շ��Ӻ���
   ���Ź���ʼ��	
*/

#ifndef	__DEBUG_H__
#define __DEBUG_H__

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS  0
#endif
#ifndef  FAIL
#define  FAIL    0xFF
#endif

//���嶨ʱ����ʼ
#ifndef  START
#define  START  1
#endif
#ifndef  STOP
#define  STOP    0
#endif

#ifndef  DE_PRINTF
#define  DE_PRINTF     0
#endif
#define	 FREQ_SYS	     16000000ul	         //ϵͳ��Ƶ12MHz
#ifndef  UART0_BUAD
#define  UART0_BUAD    100000ul
#define  UART1_BUAD    57600ul
#endif

void CfgFsys( );                        //CH554ʱ��ѡ�������
void mDelayuS( UINT16 n );              // ��uSΪ��λ��ʱ
void mDelaymS( UINT16 n );              // ��mSΪ��λ��ʱ

void CH554WDTModeSelect(UINT8 mode);     //CH554���Ź�ģʽ���� 
void CH554WDTFeed(UINT8 tim);            //CH554���Ź�ι��
#endif


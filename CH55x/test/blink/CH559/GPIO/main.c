#include "GPIO.C"

main( ) 
{
//  CfgFsys( );                                                                //CH559ʱ��ѡ������   
    mDelaymS(5);                                                              //�ȴ��ڲ������ȶ�
    P1_DIR = 0xff;	
    
    mInitSTDIO( );                                                             //����0,�������ڵ���
    printf("start ...\n");  
                                                                    
    XBUS_AUX |=  bALE_CLK_EN;                                                  //�ر�RS485ģʽ RS485_EN = 0;
    SER1_IER |= bIER_PIN_MOD1| bIER_PIN_MOD0;                                  //�ж�ʹ�ܼĴ�������λ����P2.6��P2.7,TNOW(P2.5)
    RXD1_ = 1;
    CH559GPIOInterruptInit();
    IE_GPIO = 1;                                                               //ʹ��GPIO�ж�
    EA  = 1;                                                                   //ʹ��ȫ���ж� 
	  
	  //CH559GPIODrivCap(1, 1);
	
	  
	
    while(1){
			CH559GPIOModeSelt(1, 2);
			
			mDelaymS(500);
			
      LED=1;
			mDelaymS(500);
			LED=0;
		}
}

#include "GPIO.C"

main( ) 
{
//  CfgFsys( );                                                                //CH559时钟选择配置   
    mDelaymS(5);                                                              //等待内部晶振稳定
    P1_DIR = 0xff;	
    
    mInitSTDIO( );                                                             //串口0,可以用于调试
    printf("start ...\n");  
                                                                    
    XBUS_AUX |=  bALE_CLK_EN;                                                  //关闭RS485模式 RS485_EN = 0;
    SER1_IER |= bIER_PIN_MOD1| bIER_PIN_MOD0;                                  //中断使能寄存器，脚位设置P2.6和P2.7,TNOW(P2.5)
    RXD1_ = 1;
    CH559GPIOInterruptInit();
    IE_GPIO = 1;                                                               //使能GPIO中断
    EA  = 1;                                                                   //使能全局中断 
	  
	  //CH559GPIODrivCap(1, 1);
	
	  
	
    while(1){
			CH559GPIOModeSelt(1, 2);
			
			mDelaymS(500);
			
      LED=1;
			mDelaymS(500);
			LED=0;
		}
}

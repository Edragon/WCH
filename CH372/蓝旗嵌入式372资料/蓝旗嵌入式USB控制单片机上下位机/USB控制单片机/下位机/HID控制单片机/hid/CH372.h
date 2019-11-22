#ifndef CH372_H_
#define CH372_H_

//以下为CH372的时序函数
void delay2us(void);
void delay50ms(void);
void CH375_WRCMD(unsigned char cmd);
void CH375_WRDAT(unsigned char dat);
unsigned char CH375_REDAT(void);

#endif
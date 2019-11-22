#ifndef ch372_h_
#define ch372_h_

extern void delay2us(void);
extern void delay50ms(void);
extern void CH375_WRCMD(unsigned char cmd);
extern void CH375_WRDAT(unsigned char dat);
extern unsigned char CH375_REDAT(void);

#endif
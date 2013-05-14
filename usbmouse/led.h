#ifndef __LED_H
#define __LED_H

#define LEDs    P2

sbit LED1   =       LEDs^0;
sbit LED2   =       LEDs^1;
sbit LED3   =       LEDs^2;
sbit LED4   =       LEDs^3;
sbit LED5   =       LEDs^4;
sbit LED6   =       LEDs^5;
sbit LED7   =       LEDs^6;
sbit LED8   =       LEDs^7;

/* turn on led */
#define on_led1()   LED1=0
#define on_led2()   LED2=0
#define on_led3()   LED3=0
#define on_led4()   LED4=0
#define on_led5()   LED5=0
#define on_led6()   LED6=0
#define on_led7()   LED7=0
#define on_led8()   LED8=0

/* turn off led */
#define off_led1()  LED1=1
#define off_led2()  LED2=1
#define off_led3()   LED3=1
#define off_led4()   LED4=1
#define off_led5()   LED5=1
#define off_led6()   LED6=1
#define off_led7()   LED7=1
#define off_led8()   LED8=1

#endif
#include <reg52.h>
#include "uart.h"
#include "config.h"

volatile uint8 sending;

/* uart init */
void uart_init(void)
{
    EA      = 0;
    TMOD    &= 0x0f;
    TMOD    |= 0x20;
    SCON    = 0x50;
    TH1     = 256-FCLK/(BAUTRATE*12*16);  
    TL1     = 256-FCLK/(BAUTRATE*12*16);
    PCON    |= 0x80;    
    ES      = 1;         
    TR1     = 1;        
    REN     = 1;        
    EA      = 1;           
}

/* uart isr */
void uartISR(void) interrupt 4
{
    if (RI) {
        RI = 0;
    }
    else {
        TI = 0;
        sending = 0;    /* clear uart is sending flag */
    }
}

/* put a char into the uart */
void uart_putchar(uint8 d)
{
    SBUF = d;
    sending = 1;    /* set the sending flag */
    while(sending); /* until ISR clear the flag */
}

/* send a string */
void prints(uint8 *pd)
{
    while ((*pd) != '\0') {
        uart_putchar(*pd);  
        pd ++;  /* move on to next char */
    }
}

code uint8 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void printshortIntHex(uint16 x)
{
    uint8 i;
    uint8 display_buffer[7];
    display_buffer[6] = 0;
    display_buffer[0] = '0';
    display_buffer[1] = 'x';
    for (i=5; i>=2; i--) {
        display_buffer[i] = HexTable[(x&0xf)];
        x >>= 4;
    }
    prints(display_buffer);
}
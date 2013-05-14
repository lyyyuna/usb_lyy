#ifndef __UART_H
#define __UART_H

#include "config.h"

extern void uart_init(void);
extern void uart_putchar(uint8);
extern void prints(uint8 *);
extern void printshortIntHex(uint16 x);

#endif
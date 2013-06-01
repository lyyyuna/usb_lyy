#ifndef __UART_H
#define __UART_H

#include "config.h"

extern void uart_init(void);
extern void uart_putchar(uint8);
extern void prints(uint8 *s);
extern void printshortIntHex(uint16 x);
extern void printLongInt(uint32 x);
extern void printHex(uint8 x);

#endif
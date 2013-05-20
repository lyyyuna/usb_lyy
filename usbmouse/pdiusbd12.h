#ifndef __PDIUSBD12_H
#define __PDIUSBD12_H

#include <reg52.h>
#include "config.h"

#define D12_COMMAND_ADDR    1
#define D12_DATA_ADDR       0

/* d12 chip */
#define D12_DATA        P0
sbit D12_A0     =       P3^5;
sbit D12_WR     =       P3^6;
sbit D12_RD     =       P3^7;
sbit D12_INT    =       P3^2;

#define d12_setcommandaddr()    D12_A0=D12_COMMAND_ADDR
#define d12_setdataaddr()       D12_A0=D12_DATA_ADDR

#define d12_setwr() D12_WR=1
#define d12_clrwr() D12_WR=0

#define d12_setrd() D12_RD=1
#define d12_clrrd() D12_RD=0

#define d12_getintpin() D12_INT

/* write and read data */
#define d12_getdata()   D12_DATA
#define d12_setdata(_VALUE)   D12_DATA=(_VALUE)

#define d12_setportin()     D12_DATA=0xff
#define d12_setportout()    


#define Read_ID  0xFD


#define D12_SET_MODE  0xF3


#define READ_INTERRUPT_REGISTER  0xF4


#define D12_READ_BUFFER 0xF0


#define D12_WRITE_BUFFER 0xF0


#define D12_CLEAR_BUFFER    0xF2


#define D12_VALIDATE_BUFFER 0xFA


#define D12_ACKNOWLEDGE_SETUP 0xF1


#define D12_SET_ADDRESS_ENABLE 0xD0


#define D12_SET_ENDPOINT_ENABLE 0xD8

extern void d12_writecommand(uint8);
extern uint8 d12_readbyte(void);
extern uint16 d12_readID(void);
extern void d12_writebyte(uint8);
extern uint8 d12_readendpbuffer(uint8 endp, uint8 len, uint8 *buf);
extern uint8 d12_writeendpbuffer(uint8 endp, uint8 len, uint8 *buf);
extern void d12_clearbuffer(void);
extern uint8 d12_readendpoint_laststatus(uint8 endp);
extern void d12_acksetup(void);
extern void d12_setaddr(uint8 addr);
extern void d12_setendpenable(uint8 enable);

#endif


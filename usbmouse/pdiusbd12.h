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

#define READ_ID    0xfd

extern void d12_writecommand(uint8);
uint8 d12_readbyte(void);
uint16 d12_readID(void);

#endif


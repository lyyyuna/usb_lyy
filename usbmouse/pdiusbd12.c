#include <reg52.h>
#include "config.h"
#include "pdiusbd12.h"

void d12_writecommand(uint8 command)
{
    d12_setcommandaddr();
    d12_clrwr();
    d12_setportout();
    d12_setdata(command);
    d12_setwr();
    d12_setportin();
}

uint8 d12_readbyte(void)
{
    uint8 temp;
    d12_setdataaddr();
    d12_clrrd();
    temp = d12_getdata();
    d12_setrd();
    return temp;
}

uint16 d12_readID(void)
{
    uint16 id;
    d12_writecommand(READ_ID);
    id = d12_readbyte();
    id |= ((uint16)d12_readbyte())<<8;
    return id;
}
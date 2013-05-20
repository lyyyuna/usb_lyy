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

void d12_writebyte(uint8 value)
{
    d12_setdataaddr();
    d12_clrwr();
    d12_setportout();
    d12_setdata(value);
    d12_setwr();
    d12_setportin();
}

/* read the last state of endpoint */
uint8 d12_readendpoint_laststatus(uint8 endp)
{
    d12_writecommand(0x40 + endp);
    return d12_readbyte();
}

/* select a endp */
void d12_selectendpoint(uint8 endp)
{
    d12_writecommand(0x00 + endp);
}

/* clear the buffer 
    otherwise, the next data will not be received */
void d12_clearbuffer(void)    
{
    d12_writecommand(D12_CLEAR_BUFFER);
}

void d12_acksetup(void)
{
    d12_selectendpoint(1);  /* endp 0 input */
    d12_writecommand(D12_ACKNOWLEDGE_SETUP);
    d12_selectendpoint(0);  /* endp 0 output */
    d12_writecommand(D12_ACKNOWLEDGE_SETUP);
}

uint8 d12_readendpbuffer(uint8 endp, uint8 len, uint8 *buf)
{
    uint8 i,j;
    d12_selectendpoint(endp);
    d12_writecommand(D12_READ_BUFFER);
    d12_readbyte();     /* this byte is reserved, not used */
    j = d12_readbyte(); /* the length of the received data */
    if (j > len) {
        j = len;    /* read the specified data length */
    }
    #ifdef DEBUG1
    prints("¶Á¶Ëµã");
    printLongInt(endp/2);
    prints("»º³åÇø")
    printLongInt(j);
    prints("×Ö½Ú¡£\r\n");
    #endif
    for (i = 0; i < j; i++) {
        d12_clrrd();
        *(buf+i) = d12_getdata();
        #ifdef DEBUG1
        printHex(*(buf+i));
        if (((i+1)%16)==0)
            prints("\r\n");
        #endif
    }
    #ifdef DEBUG1
    if ((j%16)!=0)
        prints("\r\n"); /* change line */
    #endif
    return j;
}

/* make the data in the buffer validate, only the data is valid, 
   it can be sent out */
void d12_validatebuffer(void)
{
    d12_writecommand(D12_VALIDATE_BUFFER);
}

uint8 d12_writeendpbuffer(uint8 endp, uint8 len, uint8 *buf)
{
    uint8 i;
    d12_selectendpoint(endp);
    d12_writecommand(D12_WRITE_BUFFER);
    d12_writebyte(0);   /* according to datasheet, mean nothing */
    d12_writebyte(len);
    
    #ifdef DEBUG1
    prints("Ð´¶Ëµã");
    printLongInt(endp/2);
    prints("»º³åÇø");
    printLongInt(len);
    prints("×Ö½Ú¡£\r\n");
    #endif
    d12_setportout();
    for (i = 0; i < len; i++) {
        d12_clrwr();
        d12_setdata(*(buf+i));
        d12_setwr();
        #ifdef DEBUG1
        printHex(*(buf+i));
        if ((i+1)%16 == 0)
            prints("\r\n");
        #endif
    }
    #ifdef DEBUG1
    if ((len%16) != 0)
        prints("\r\n");
    #endif
    d12_setportin();
    d12_validatebuffer();
    return len;
}

void d12_setaddr(uint8 addr)
{
    d12_writecommand(D12_SET_ADDRESS_ENABLE);
    d12_writebyte(0x80 | addr);
}


/* enable the endp */
void d12_setendpenable(uint8 enable)
{
    d12_writecommand(D12_SET_ENDPOINT_ENABLE);
    if (enable != 0)
        d12_writebyte(0x01);
    else 
        d12_writebyte(0x00);
}
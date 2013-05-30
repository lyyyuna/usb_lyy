#include <reg52.h>
#include "key.h"
#include "led.h"
#include "uart.h"
#include "pdiusbd12.h"
#include "config.h"
#include "usbcore.h"

code uint8 HeadTable[][74] = {
"********************************************************************\r\n",
"******                   lyy教你玩USB                         ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  建立日期：",__DATE__,"                ******\r\n",
"******                   建立时间：",__TIME__,"               ******\r\n",
"******                    作者：lyy                           ******\r\n",
"******                  请按K1-K8分别进行测试                 ******\r\n",
"********************************************************************\r\n",
};

void main(void)
{
    uint8 i;
    uint16 id;
    uint8 InterruptSource;

    EA = 1;
    InitKeyboard();
    uart_init();

    for (i=0; i<11; i++) {
        prints(HeadTable[i]);
    }

    id = d12_readID();
    prints("Your D12 chip\'s ID is: ");
    printshortIntHex(id);
    if (id==0x1012) {
        prints(". ID is correct! Congratulations!\r\n\r\n");
    }
    else {
        prints(". ID is incorrect! What a pity!\r\n\r\n");
    }

    usb_disconnect();
    usb_connect();
    ConfigValue = 0;

    while(1) {
        if ( d12_getintpin() == 0) {
            d12_writecommand(READ_INTERRUPT_REGISTER);
            InterruptSource = d12_readbyte();
            if (InterruptSource & 0x80)
                usb_bussuspend();
            if (InterruptSource & 0x40)
                usb_busreset();
            if (InterruptSource & 0x01)
                usb_ep0out();
            if (InterruptSource & 0x02)
                usb_ep0in();
            if (InterruptSource & 0x04)
                usb_ep1out();
            if (InterruptSource & 0x08)
                usb_ep1in();
            if (InterruptSource & 0x10)
                usb_ep2out();
            if (InterruptSource & 0x20)
                usb_ep2in();
        }
        if (configvalue != 0) {
            LEDs = ~KeyPress;
            if (!ep1in_isbusy) {
                KeyCanChange = 0;
                if (KeyUp || KeyDown || KeyPress) {
                    SendReport();
                }
                KeyCanChange = 1;
            }
        }
    }
}

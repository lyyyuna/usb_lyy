#include <reg52.h>
#include "key.h"
#include "led.h"
#include "uart.h"
#include "pdiusbd12.h"

code uint8 HeadTable[][74] = {
"********************************************************************\r\n",
"******                   lyy������USB                         ******\r\n",
"******                      AT89S52 CPU                       ******\r\n",
"******                  �������ڣ�",__DATE__,"                ******\r\n",
"******                   ����ʱ�䣺",__TIME__,"               ******\r\n",
"******                    ���ߣ�lyy                           ******\r\n",
"******                  �밴K1-K8�ֱ���в���                 ******\r\n",
"********************************************************************\r\n",
};

void main(void)
{
    uint8 i;
    uint16 id;
    
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
    
    while(1) {
    
    }
}
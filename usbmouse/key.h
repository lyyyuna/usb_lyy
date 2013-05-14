#ifndef __KEY_H
#define __KEY_H

#include <reg52.h>
#include "config.h"

extern volatile uint8 idata KeyCurrent,KeyOld,KeyNoChangedTime;
extern volatile uint8 idata KeyPress;
extern volatile uint8 idata KeyDown,KeyUp,KeyLast;

extern volatile uint8 KeyCanChange;

void InitKeyboard(void);

#define KeyIO P1
//获取按键值的宏。由于开关按下为0，所以对结果取反
#define GetKeyValue() (~KeyIO)

#define KEY1 0x01
#define KEY2 0x02
#define KEY3 0x04
#define KEY4 0x08
#define KEY5 0x10
#define KEY6 0x20
#define KEY7 0x40
#define KEY8 0x80

#endif
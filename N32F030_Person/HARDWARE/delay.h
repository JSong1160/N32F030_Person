#ifndef _DELAY_H_
#define _DELAY_H_
#include "n32g030.h"


void Delay(__IO uint32_t nCount);
void delay_init(u8 SYSCLK);
void delay_us(u32 us);
void delay_ms(u16 ms);



#endif




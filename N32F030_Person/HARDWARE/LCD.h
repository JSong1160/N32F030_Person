#ifndef _LCD_H_
#define _LCD_H_
#include "n32g030.h"

void SendBit_1621(u8 data,u8 cnt);
void SendDataBit_1621(u8 data,u8 cnt);
void SendDataBit_1621(u8 data,u8 cnt) ;
void SendCmd(u8 command);
void Write_1621(u8 addr,u8 data);
void WriteAll_1621(u8 addr,u8 *p,u8 cnt);
void LCD_Clear(void);
void LCD_Init(void);



#endif


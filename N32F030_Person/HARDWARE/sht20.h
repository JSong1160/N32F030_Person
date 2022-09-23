#ifndef _sht20_h
#define _sht20_h

#include "n32g030.h"
#include "include.h"

void scl3_h(void);
void scl3_l(void);
void sda_out(void);
void sda_in(void);
void sda3_h(void);
void sda3_l(void);


u8 I2C3_Start(void);
void I2C3_Stop(void);
void I2C3_Ack(void);
void I2C3_NoAck(void);
u8 I2C3_WaitAck(void);
u8 I2C3_Check(void);

void I2C3_SendByte(u8 SendByte);
u8 I2C3_ReceiveByte(void);

void I2cI2cI2cSoftReset(void);
void SET_Resolution(void);
u16 ReadSht20(char TempOrHumiCMD);
void ReadSht20Pro(void);
u16 ReadSht31(void);

void Single_Write_BH1750(u8 REG_Address);
void Set20Wlux(void);
void Set6WLux(void);
void Multiple_read_BH1750(void);
void  LMP_Init(void);




#endif


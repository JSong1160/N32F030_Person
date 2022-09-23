#ifndef _Proc_h
#define _Proc_h
#include "n32g030.h"


void GetSTM32MCUID(u8 type);
void GetLockCode(void);
void ResetInf(void);
void WriteInf(void);
void FlashCheck(void);
void ReadInf(void);
void RS485_SendBuf(u8 *data,u16 len);
void SetSDAPP(void);
void SetSDAOD(void);
void DAC7512(u16 DAData);
void DAC7512_2(u16 DAData);
void SendOneByte(u8 Byte);
void SendU2(u8 *data,u8 len);	
void WriteTFT(u8 sta);
void GetVolt(void);
void Zhongping(void);
void Waijieping(void);
void Alarm(void);
void Calibration(void);//Å¨¶ÈÐ£×¼

#endif



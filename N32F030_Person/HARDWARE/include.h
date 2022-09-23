#ifndef _include_h
#define _include_h

#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "modbus_slave.h"

#include "sht20.h"
#include "led.h"
#include "init.h"
#include "Proc.h"
#include "flash.h"
#include "n32g030.h"
#include "n32g030_conf.h"
#include "oled.h"

#define EN_LUX 1			

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t


/* �����豸�ṹ�� */
typedef struct
{
	USART_Module *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	__IO uint16_t usTxWrite;	/* ���ͻ�����дָ�� */
	__IO uint16_t usTxRead;		/* ���ͻ�������ָ�� */
	__IO uint16_t usTxCount;	/* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;	/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;		/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;	/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;



#ifdef __GLOBALS__
#define EXT
#else
#define EXT extern
#endif

EXT u8 cof_jixing,tim_flag;
EXT u8 hour,minutes,seconds;
EXT s32 raw_press, raw_temp;  
EXT s16 Tem,Hum;
EXT u16 ModelBaud,CO2,DACType,tvoc,LuxMult,SetGas,Standard,M_Dac1,M_Dac2;
EXT u32 DQQY,Lux;
EXT s16 LuxOff,CO2Off,ischange;
EXT u32 biaozhiwei;
EXT __IO uint16_t RegularConvData_Tab[160];
EXT  uint16_t RegularConvData_Tab2[10];
EXT int16_t Tem_K[2],TemOff[2],HumOff[2],Hum_K[2];
EXT u8 ModelID,LuxSelect;			//ModelID
EXT u16 Volt1,Volt2,NH3,NH3offset;
EXT u16 yurenowtianme,NH3offseton;
EXT u16 cof_yuretime;
EXT u32 CaliTime,CaliTime1;
typedef struct
{
	uint8_t Rxlen;
	char RxBuf[S_RX_BUF_SIZE];
	uint8_t Txlen;
	char TxBuf[S_TX_BUF_SIZE];
}PRINT_MODS_T;

EXT u16 Flash_Check;
EXT PRINT_MODS_T g_tPrint;

typedef struct
{
	s16 temperature;
	u32 pressure;
} defMS5611data;


EXT u8 TemSelect,cof_showstyle,cof_showstyle_last;
EXT u8 CpuID[12];
EXT u16 cpuID[6];
EXT u16 cof_NH3cheng,cof_NH3chu,DQSelect,DACSelect,cof_DACcheng,cof_DACchu,cof_5vcheng,cof_5vchu,DAC2_Style,cof_yurevalue;
EXT s16 cof_DACb;
EXT u32 cof_DACmax,temp1;
EXT u16 Shangxian[2],Xiaxian[2],HuiLuo[2];
EXT u8 EVENODD;


#endif


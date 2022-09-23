/*
*********************************************************************************************************
*
*	模块名称 : MODEBUS 通信模块 (从站）
*	文件名称 : modbus_slave.h
*	版    本 : V1.4
*	说    明 : 头文件
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MODBUY_SLAVE_H
#define __MODBUY_SLAVE_H
#include "n32g030.h"

#define SADDR485	1
#define SBAUD485	2400

/* 01H 读强制单线圈 */
/* 05H 写强制单线圈 */
#define REG_D01		0x0101
#define REG_D02		0x0102
#define REG_D03		0x0103
#define REG_D04		0x0104
#define REG_DXX 	REG_D04

/* 02H 读取输入状态 */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */
#define SLAVE_REG_P00		0x0000
#define SLAVE_REG_P01		0x0001
#define SLAVE_REG_P02		0x0002
#define SLAVE_REG_P03		0x0003
#define SLAVE_REG_P04		0x0004
#define SLAVE_REG_P05		0x0005
#define SLAVE_REG_P06		0x0006
#define SLAVE_REG_P07		0x0007
#define SLAVE_REG_P08		0x0008
#define SLAVE_REG_P09		0x0009
#define SLAVE_REG_P0a		0x000a
#define SLAVE_REG_P0b		0x000b
#define SLAVE_REG_P0c		0x000c
#define SLAVE_REG_P0d		0x000d
#define SLAVE_REG_P0e		0x000e
#define SLAVE_REG_P0f		0x000f

#define SLAVE_REG_P10		0x0010
#define SLAVE_REG_P11		0x0011
#define SLAVE_REG_P12		0x0012
#define SLAVE_REG_P13		0x0013
#define SLAVE_REG_P14		0x0014
#define SLAVE_REG_P15		0x0015
#define SLAVE_REG_P16		0x0016
#define SLAVE_REG_P17		0x0017
#define SLAVE_REG_P18		0x0018
#define SLAVE_REG_P19		0x0019
#define SLAVE_REG_P1a		0x001a
#define SLAVE_REG_P1b		0x001b
#define SLAVE_REG_P1c		0x001c
#define SLAVE_REG_P1d		0x001d
#define SLAVE_REG_P1e		0x001e
#define SLAVE_REG_P1f		0x001f


#define SLAVE_REG_P100		0x0100
#define SLAVE_REG_P101		0x0101
#define SLAVE_REG_P102		0x0102
#define SLAVE_REG_P103		0x0103
#define SLAVE_REG_P104		0x0104
#define SLAVE_REG_P105		0x0105
#define SLAVE_REG_P106		0x0106
#define SLAVE_REG_P107		0x0107
#define SLAVE_REG_P108		0x0108
#define SLAVE_REG_P109		0x0109
#define SLAVE_REG_P10a		0x010a
#define SLAVE_REG_P10b		0x010b
#define SLAVE_REG_P10c		0x010c
#define SLAVE_REG_P10d		0x010d
#define SLAVE_REG_P10e		0x010e
#define SLAVE_REG_P10f		0x010f
#define SLAVE_REG_P110		0x0110
#define SLAVE_REG_P111		0x0111
#define SLAVE_REG_P112		0x0112
#define SLAVE_REG_P113		0x0113
#define SLAVE_REG_P114		0x0114
#define SLAVE_REG_P115		0x0115
#define SLAVE_REG_P200		0x0200			
#define SLAVE_REG_P201		0x0201
#define SLAVE_REG_P202		0x0202
#define SLAVE_REG_P203		0x0203
#define SLAVE_REG_P204		0x0204
#define SLAVE_REG_P205		0x0205
#define SLAVE_REG_P206		0x0206
#define SLAVE_REG_P207		0x0207
#define SLAVE_REG_P208		0x0208
#define SLAVE_REG_P209		0x0209
#define SLAVE_REG_P20a		0x020a
#define SLAVE_REG_P20b		0x020b
#define SLAVE_REG_P20c		0x020c
#define SLAVE_REG_P20d		0x020d
#define SLAVE_REG_P800		0x0800			
#define SLAVE_REG_P801		0x0801
#define SLAVE_REG_P802		0x0802
#define SLAVE_REG_Pffa1      0xffa1
#define SLAVE_REG_Pffa2      0xffa2
#define SLAVE_REG_Pffa3      0xffa3
#define SLAVE_REG_Pffa4      0xffa4
#define SLAVE_REG_Pffa5      0xffa5
#define SLAVE_REG_Pffa6      0xffa6
#define SLAVE_REG_Pffa7      0xffa7
/* 04H 读取输入寄存器(模拟信号) */
#define REG_A01		0x0401
#define REG_AXX		REG_A01


/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128

typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H 读写保持寄存器 */
	uint16_t P01;
	uint16_t P02;
	


	/* 04H 读取模拟量寄存器 */
	uint16_t A01;

	/* 01H 05H 读写单个强制线圈 */
	uint16_t D01;
	uint16_t D02;
	uint16_t D03;
	uint16_t D04;

}VAR_T;

enum 
{
	MSG_NONE = 0,
	
	MSG_MODS_01H,
	MSG_MODS_02H,
	MSG_MODS_03H,
	MSG_MODS_04H,
	MSG_MODS_05H,
	MSG_MODS_06H,
	MSG_MODS_10H,
};

extern MODS_T g_tModS;
extern VAR_T g_tVar;

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
void MODS_Poll(void);
void MODS_ReciveNew(uint8_t _byte);
void MODS_RxTimeOut(void);
uint16_t BEBufToUint16(uint8_t *_pBuf);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

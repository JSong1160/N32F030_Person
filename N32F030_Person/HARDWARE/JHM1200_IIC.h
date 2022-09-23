/*
****************************************************************************
*
* JHM1200_IIC.h
* Date: 2015/06/26
* Revision: 1.0.0
*
* Usage: IIC read and write interface
*
**************************************************************************/
#ifndef __JHM1200_IIC_H_
#define __JHM1200_IIC_H_

#include "n32g030.h"

u8 BSP_IIC_Write(u8 IIC_Address, u8 *buffer, u8 count);
u8 BSP_IIC_Read(u8 IIC_Address, u8 *buffer, u8 count);

#endif

/*
****************************************************************************
*
* JHM1200_IIC.c
* Date: 2015/06/26
* Revision: 1.0.0
*
* Usage: IIC read and write interface
*
**************************************************************************/
#include "JHM1200_IIC.h"
#include "include.h"

////Delay function needs to be defined
void DelayUs(unsigned char i)
{
}



//Start signal
void Start(void)
{
	sda3_h();//SDA = 1;
  delay_us(5);
	scl3_h();//SCL = 1;
   delay_us(5);
	sda3_l();//SDA = 0;
  delay_us(5);
	scl3_l();//SCL = 0;
}

//Stop signal
void Stop(void)
{
	sda_out();
	sda3_l();//SDA = 0;
  delay_us(5);
	scl3_h();//SCL = 1;
  delay_us(5);
	sda3_h();//SDA = 1;
  delay_us(5);
}

//Read ACK signal
unsigned char Check_ACK(void)
{
	unsigned char ack;
	sda_in();
	scl3_h();//SCL = 1;
  delay_us(5);
  if(((GPIOA->PID)&0X00000020)==0)
	 {
		 ack = 0;
//		 break;
		}
	else
		{
			ack = 1;
		}
	scl3_l();//SCL = 0;
	sda_out();
	return ack;
}

//Send ACK signal
void Send_ACK(void)
{
	sda_out();
	sda3_l();//SDA = 0;
   delay_us(5);
	scl3_h();//SCL = 1;
  delay_us(5);
	scl3_l();//SCL = 0;
  delay_us(5);
}

//Send one byte
void SendByte(unsigned char byte)
{
	unsigned char i = 0;
	sda_out();
	do
	{
		if (byte & 0x80)
		{
		 sda3_h();	//SDA = 1;
		}
		else
		{
		 sda3_l();	//SDA = 0;
		}
	  delay_us(5);
		scl3_h();//SCL = 1;
	  delay_us(5);
		byte <<= 1;
		i++;
		scl3_l();//SCL = 0;
	} while (i < 8);
	  scl3_l();//SCL = 0;
}

//Receive one byte
unsigned char ReceiveByte(void)
{
	unsigned char i = 0, tmp = 0;
	sda_in();
	do
	{
		tmp <<= 1;
		scl3_h();//SCL = 1;
	  delay_us(5);
      if(((GPIOA->PID)&0X00000020))
      {
       tmp|=0x01;
      }
		scl3_l();//SCL = 0;
	  delay_us(5);
		i++;
	} while (i < 8);
	return tmp;
}

//Write a byte of data through IIC
u8 BSP_IIC_Write(u8 address, u8 *buf, u8 count)
{
	unsigned char timeout, ack;
	address &= 0xFE;
	Start();
  delay_us(5);
	SendByte(address);
	sda_in();
  delay_us(5);
	timeout = 0;
	do
	{
		ack = Check_ACK();
		timeout++;
		if (timeout == 10)
		{
			Stop();
			return 1;
		}
	} while (ack);
	while (count)
	{
		SendByte(*buf);
		sda_in();
	  delay_us(5);
		timeout = 0;
		do
		{
			ack = Check_ACK();
			timeout++;
			if (timeout == 10)
			{
				return 2; 
			}
		} while (0);
		buf++;
		count--;
	}
	Stop();
	return 0;
}

//Read a byte of data through IIC
u8 BSP_IIC_Read(u8 address, u8 *buf, u8 count)
{
	unsigned char timeout, ack;
	address |= 0x01;
	Start();
	SendByte(address);
	sda_in();
  delay_us(5);
	timeout = 0;
	do
	{
		ack = Check_ACK();
		timeout++;
		if (timeout == 4)
		{
			Stop();
			return 1; 
		}
	} while (ack);
  delay_us(5);
	while (count)
	{
		*buf = ReceiveByte();
		if (count != 1)
			Send_ACK();
		buf++;
		count--;
	}
	Stop();
	return 0;
}

#include "include.h"
#define BIAS 0x50
#define SYSEN 0x02
#define LCDON 0x06
#define LCDOFF 0x04
#define HT_RCOSC            0x30  

#define CLR_LCDCS    GPIOB->POD &= ~(1<<1)          //GPIO_ResetBits(GPIOB,GPIO_PIN_1)
#define SET_LCDCS 	 GPIOB->POD |=   1<<1					  //GPIO_SetBits(GPIOB,GPIO_PIN_1)

#define CLR_LCDWR    GPIOA->POD &= ~(1<<7)             //GPIO_ResetBits(GPIOA,GPIO_PIN_7)
#define SET_LCDWR    GPIOA->POD |=   1<<7               //GPIO_SetBits(GPIOA,GPIO_PIN_7)

#define CLR_LCDDATA  GPIOA->POD &= ~(1<<0)             //GPIO_ResetBits(GPIOA,GPIO_PIN_0)
#define SET_LCDDATA  GPIOA->POD |=   1<<0             //GPIO_SetBits(GPIOA,GPIO_PIN_0)



void SendBit_1621(u8 data,u8 cnt) //data的高cnt位写入HT1621，高位在前
{
	u8 i;
	for(i =0; i <cnt; i ++)
	{
		if((data&0x80)==0) 
			CLR_LCDDATA;
		else 
			SET_LCDDATA;
		CLR_LCDWR;
		__nop();
		SET_LCDWR;
		data<<=1;
	}
}
void SendDataBit_1621(u8 data,u8 cnt) //data的低cnt位写入HT1621，低位在前
{
	u8 i;
	for(i =0; i <cnt; i ++)
	{
		if((data&0x01)==0) CLR_LCDDATA;
		else SET_LCDDATA;
		CLR_LCDWR;
		__nop();
		SET_LCDWR;
		data>>=1;
	}
}
void SendCmd(u8 command)
{
	CLR_LCDCS;
	SendBit_1621(0x80,4); //写入标志位码”100”和9位command命令，由于
	SendBit_1621(command,8); //没有使有到更改时钟输出等命令，为了编程方便
	SET_LCDCS; //直接将command的最高位写”0”
}
void Write_1621(u8 addr,u8 data)
{
	CLR_LCDCS;
	SendBit_1621(0xa0,3); //写入标志位码”101”
	SendBit_1621(addr,6); //写入addr的高6位
	SendDataBit_1621(data,4); //写入data的低4位
	SET_LCDCS;
}
void WriteAll_1621(u8 addr,u8 *p,u8 cnt)
{
	u8 i;
	CLR_LCDCS;
	SendBit_1621(0xa0,3); //写入标志位码”101”
	SendBit_1621(addr,6); //写入addr的高6位
	for(i =0; i <cnt; i ++,p++) //连续写入数据
	{
		SendDataBit_1621(*p,8);
	}
	SET_LCDCS;
}

void LCD_Clear()
{
    u8 i;  
    u8 addr = 0;  
      
    for (i=0; i<32; i++)  
    {  
        Write_1621(addr*4, 0xff);  
        addr += 1;  
    }    	
}
void LCD_Init()
{
	u8 a[10],i;
	a[0]=0x21;a[1]=0x43;a[2]=0x65;a[3]=0x87;a[4]=0xa9;
	
	RCC->APB2PCLKEN  |= 1<<2;  //使能 GPIOA  时钟 
	RCC ->APB2PCLKEN |= 1<<3;  //使能 GPIOB  时钟

	GPIOA->PMODE     &= 0XFFFF3FFC;
	GPIOA->PMODE     |= 0X00004001;//PA 0 7 输出
  GPIOA->POTYPE    &= ~(1<<0);
  GPIOA->POTYPE    &= ~(1<<7);	 // PA 0 7 推挽模式
	
	GPIOA->PUPD      &= 0XFFFF3FFC;// PA 0 7 不上/下拉
	GPIOA->SR        &= ~(1<<0);
  GPIOA->SR        &= ~(1<<7);	 // PA 0 7 快速模式
	 
	GPIOB->PMODE   &= 0XFFFFFFF3;
	GPIOB->PMODE   |= 0X00000004;  //PB 1 输出
  GPIOB->POTYPE  &= ~(1<<1);     //PB 1 推挽模式
	GPIOB->PUPD    &= 0XFFFFFFF3;  //PB 1 不上/下拉
	GPIOB->SR      &= ~(1<<1); 	   //PB 1 快速模式
	delay_ms(10);
	SendCmd(HT_RCOSC);
	SendCmd(BIAS); //设置偏压和工作周期
	SendCmd(SYSEN); //打开系统振荡器
	SendCmd(LCDON); //打开LCD偏压发生器
	delay_ms(2);
	Write_1621(0x24,0x01); //0x24: (地址)的高6位有效，
	//0x01: (数据)的低4位有效
	LCD_Clear();
	for(i=0;i<32;i++)
	{Write_1621(i*4,0);
		delay_ms(500);
	}
	//WriteAll_1621(0,a,5); //0：(起始地址)高6位有效，
//a：(写入数据的起始地址)8位都有效，
//5：为写入的字节数。连续写入”123456789a”
}













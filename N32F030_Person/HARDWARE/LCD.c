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



void SendBit_1621(u8 data,u8 cnt) //data�ĸ�cntλд��HT1621����λ��ǰ
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
void SendDataBit_1621(u8 data,u8 cnt) //data�ĵ�cntλд��HT1621����λ��ǰ
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
	SendBit_1621(0x80,4); //д���־λ�롱100����9λcommand�������
	SendBit_1621(command,8); //û��ʹ�е�����ʱ����������Ϊ�˱�̷���
	SET_LCDCS; //ֱ�ӽ�command�����λд��0��
}
void Write_1621(u8 addr,u8 data)
{
	CLR_LCDCS;
	SendBit_1621(0xa0,3); //д���־λ�롱101��
	SendBit_1621(addr,6); //д��addr�ĸ�6λ
	SendDataBit_1621(data,4); //д��data�ĵ�4λ
	SET_LCDCS;
}
void WriteAll_1621(u8 addr,u8 *p,u8 cnt)
{
	u8 i;
	CLR_LCDCS;
	SendBit_1621(0xa0,3); //д���־λ�롱101��
	SendBit_1621(addr,6); //д��addr�ĸ�6λ
	for(i =0; i <cnt; i ++,p++) //����д������
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
	
	RCC->APB2PCLKEN  |= 1<<2;  //ʹ�� GPIOA  ʱ�� 
	RCC ->APB2PCLKEN |= 1<<3;  //ʹ�� GPIOB  ʱ��

	GPIOA->PMODE     &= 0XFFFF3FFC;
	GPIOA->PMODE     |= 0X00004001;//PA 0 7 ���
  GPIOA->POTYPE    &= ~(1<<0);
  GPIOA->POTYPE    &= ~(1<<7);	 // PA 0 7 ����ģʽ
	
	GPIOA->PUPD      &= 0XFFFF3FFC;// PA 0 7 ����/����
	GPIOA->SR        &= ~(1<<0);
  GPIOA->SR        &= ~(1<<7);	 // PA 0 7 ����ģʽ
	 
	GPIOB->PMODE   &= 0XFFFFFFF3;
	GPIOB->PMODE   |= 0X00000004;  //PB 1 ���
  GPIOB->POTYPE  &= ~(1<<1);     //PB 1 ����ģʽ
	GPIOB->PUPD    &= 0XFFFFFFF3;  //PB 1 ����/����
	GPIOB->SR      &= ~(1<<1); 	   //PB 1 ����ģʽ
	delay_ms(10);
	SendCmd(HT_RCOSC);
	SendCmd(BIAS); //����ƫѹ�͹�������
	SendCmd(SYSEN); //��ϵͳ����
	SendCmd(LCDON); //��LCDƫѹ������
	delay_ms(2);
	Write_1621(0x24,0x01); //0x24: (��ַ)�ĸ�6λ��Ч��
	//0x01: (����)�ĵ�4λ��Ч
	LCD_Clear();
	for(i=0;i<32;i++)
	{Write_1621(i*4,0);
		delay_ms(500);
	}
	//WriteAll_1621(0,a,5); //0��(��ʼ��ַ)��6λ��Ч��
//a��(д�����ݵ���ʼ��ַ)8λ����Ч��
//5��Ϊд����ֽ���������д�롱123456789a��
}













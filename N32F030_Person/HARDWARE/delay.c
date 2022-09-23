#include "delay.h"

/**
 * @brief  Delay funciton.
 */
static u8  fac_us  = 0;
static u16 fac_ms  = 0;

void Delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}

void SysTick_Init(void)
{
	 while(SysTick_Config(SystemCoreClock/1000));
}

void delay_init(u8 SYSCLK)
{
  SysTick->CTRL |= 1<<2;  //ѡ���ڲ�ʱ��
	SysTick->CTRL &= ~(1<<0);//��ʱ��ʧ��
	
  fac_us = SYSCLK/8;
  fac_ms = (u16)fac_us*3000;	
}


void delay_us(u32 us)
{
  u32 temp;
	
	SysTick->LOAD = fac_us* us;
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= 1<<0; //Systick��ʱ��ʹ��
  do
   {
     temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
   }
  while((temp&0x0001)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
 
  SysTick->CTRL  &= ~(1<<0);//��ʱ��ʧ��
  SysTick->VAL   =  0X00;     //��ռ�����
}

void delay_ms(u16 ms)
{
  u32 temp;
  SysTick->LOAD  = fac_ms*(u32)ms;
  SysTick->VAL   = 0X00;//��ռ�����
	SysTick->CTRL |= 1<<0; //Systick��ʱ��ʹ��
  do
   {    temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
   }
 	while((temp&0x0001)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��




	 SysTick->CTRL  &= ~(1<<0);//��ʱ��ʧ��
  SysTick->VAL =0X00; //��ռ�����
}


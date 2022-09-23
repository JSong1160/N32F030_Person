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
  SysTick->CTRL |= 1<<2;  //选用内部时钟
	SysTick->CTRL &= ~(1<<0);//定时器失能
	
  fac_us = SYSCLK/8;
  fac_ms = (u16)fac_us*3000;	
}


void delay_us(u32 us)
{
  u32 temp;
	
	SysTick->LOAD = fac_us* us;
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= 1<<0; //Systick定时器使能
  do
   {
     temp=SysTick->CTRL;//读取当前倒计数值
   }
  while((temp&0x0001)&&(!(temp&(1<<16))));//等待时间到达
 
  SysTick->CTRL  &= ~(1<<0);//定时器失能
  SysTick->VAL   =  0X00;     //清空计数器
}

void delay_ms(u16 ms)
{
  u32 temp;
  SysTick->LOAD  = fac_ms*(u32)ms;
  SysTick->VAL   = 0X00;//清空计数器
	SysTick->CTRL |= 1<<0; //Systick定时器使能
  do
   {    temp=SysTick->CTRL;//读取当前倒计数值
   }
 	while((temp&0x0001)&&(!(temp&(1<<16))));//等待时间到达




	 SysTick->CTRL  &= ~(1<<0);//定时器失能
  SysTick->VAL =0X00; //清空计数器
}


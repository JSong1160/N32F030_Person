#include "include.h"
void LED_Init(void)
{
   RCC ->APB2PCLKEN |= 1<<2;  //使能 GPIOA  时钟
   
	//PA10
	 GPIOA->PMODE  &= 0XFFDFFFFF;  //输出模式
	 GPIOA->POTYPE &= 0XFFFFFBFF;  //推挽模式
	 GPIOA->PUPD   |= 0x00C00000;  //nopull
   GPIOA->SR     &= 0XFFFFFBFF;  //  高速

}

void SetLed(u8 led,u8 onoff)
{
    //PA10
	
	 if(led)
	{
//		if(onoff)
//			GPIO_SetBits(GPIOA,GPIO_Pin_9);
//		else
//			GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	}
	 else
	{
		if(cof_showstyle!=2&&cof_showstyle!=3)	
	  {
		if(onoff)
			GPIOA->POD |= 1<<10;   //PA10输出高
		else
			GPIOA->POD &= 0XFFFFFBFF;   //PA10输出高		
	 }
	}
}


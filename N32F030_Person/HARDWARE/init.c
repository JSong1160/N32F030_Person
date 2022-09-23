#include "include.h"
#include "core_cm0.h"



#define ADC1_DR_Address 0x40020850

__IO uint16_t RegularConvData_Tab[160];
uint16_t RegularConvData_Tab2[10];
 
u16 yurenowtianme,NH3offseton;
u32 CaliTime,CaliTime1;

void  _sys_exit(int x)
{
 x=x;
}
int fputc(int ch,FILE *f)
{
  while((USART1->STS&(0x40))==0);
  USART1->DAT =(u8)ch;
	return ch;
}




 void GPIOInit()
 {
	/* GPIOC Periph clock enable */	 
	RCC ->APB2PCLKEN |= 1<<2;  //使能 GPIOA  时钟
	RCC ->APB2PCLKEN |= 1<<3;  //使能 GPIOB  时钟
	RCC ->APB2PCLKEN |= 1<<0;  //使能(AFIO)复用时钟
	
	/********************GPIO Configure*****************/
	/****************************************************
	*	PMODE说 明:
	*		00: 输入模式
	*		01: 通用输出模式
	*		10: 复用功能模式
	*		11: 模拟功能模式
	*
	*	POTYPE说明:
	*		0: 输出推挽模式（复位后的状态）
	*		1: 输出开漏模式
	*
	*	PUPD说明:
	*		00：无上/下拉
	*		01：上拉
	*		10：下拉
	*		11：保留
	*****************************************************
	*/

	/********************GPIO Configure*****************/
	//	PA 1
    GPIOA->PMODE  &= ~(3<<2); 
    GPIOA->PMODE  |= 1<<2;			// 输出 	 
 
	GPIOA->POTYPE &= ~(1<<2);		//推挽 

    GPIOA->PUPD  &= ~(3<<2);		//不上不下  
    GPIOA->PUPD  |= (1<<2);			//上拉		   				
			 
	GPIOA->SR      &= ~(1<<1);		//高速GPIO
	 

 }
 
void ADC_Config()
{
	/********************RCC Configure**********************/

	RCC ->AHBPCLKEN  &= ~(1<<12); //ADC时钟失能
    RCC ->AHBPCLKEN  |=   1<<12;  //ADC时钟使能
	RCC ->APB2PCLKEN |= 1<<2;    //使能 GPIOA  时钟 

  	RCC ->CFG2     &= ~(1<<10);  // HSI是ADC 1M 时钟源	
    RCC ->CFG2     &= ~(15<<11);
    RCC ->CFG2     &= ~(1<<15);
    RCC ->CFG2     |=  (7<<11);	//八分频    //实现内部时钟计时时钟1Mhz频率的配置
    
	RCC ->CFG2     &= ~(1<<8);//ADCPLL时钟 被关闭
	
	RCC ->CFG2     &= ~(15<<0);
    RCC ->CFG2     |=  (2<<0);	//4分频   48/4=12MHz     ADC的工作时钟

	
	/*****************GPIO Configure************************/	

	GPIOA->PMODE   |=  (3<<0);   // PA0模拟功能 
	GPIOA->PUPD    &= ~(3<<0);   //不上下拉
	GPIOA->SR      &= ~(1<<0);   //高速

	/*****************ADC Configure*************************/

	ADC->CTRL1     |=   1<<8;  //使用扫描模式	
    ADC->CTRL2     |= 1<<1;  //连续转换
    ADC->CTRL2     |=   7<<17; 
    ADC->CTRL2     &= ~(1<<15);//不用外部事件启动转换	
	ADC->CTRL2     &= ~(1<<11);//右对齐	
		
	ADC->RSEQ1     &= ~(15<<20);
    ADC->RSEQ1     |=  (0<<20); //通道数目一个
		
	ADC->RSEQ3     &=~(15<<0); 
	ADC->RSEQ3     |= (0<<0); 	// 通道0 第一个转换

		
	ADC->SAMPT3    &=~(15<<0); 
	ADC->SAMPT3    |= (6<<0);	 //CH0    56
		
	ADC->CTRL2     |=   1<<8;    //使用DMA模式
	ADC->CTRL2     |=   1<<0;    
		
	while(!(ADC->CTRL3 & 0X20));
	while(ADC->CTRL3 & 0X40);	
		
	ADC->CTRL2 |= 0X00500000;
}


void DMA_Config()
{
	/********************RCC Configure**********************/
	RCC->AHBPCLKEN |= 1<<0;  //使能DMA时钟

	/********************DMA Configure**********************/
	DMA_CH1->CHCFG = 0;
	DMA_CH1->TXNUM = 0;
	DMA_CH1->PADDR = 0;
	DMA_CH1->MADDR = 0;	
	DMA->INTCLR |= 0X0000000F;					//复位DMA
	
	DMA_CH1->PADDR  = (uint32_t)ADC1_DR_Address;
	DMA_CH1->MADDR  = (uint32_t)RegularConvData_Tab;
	DMA_CH1->CHCFG  &= ~(1<<4);					//从外设读出写入存储器
	DMA_CH1->TXNUM   =   160;
	DMA_CH1->CHCFG  &= ~(1<<6);					//外设固定地址模式
	DMA_CH1->CHCFG	|=   1<<7;					// 存储地址自增
	DMA_CH1->CHCFG  &= ~(3<<8);
	DMA_CH1->CHCFG  |=  (1<<8);					//外设数据宽度HalfWord	
	DMA_CH1->CHCFG  &= ~(3<<10);
	DMA_CH1->CHCFG  |=  (1<<10);				//存储器数据宽度HalfWord		
	DMA_CH1->CHCFG  |=   1<<5;					//循环模式
	DMA_CH1->CHCFG  &= ~(3<<12);  
	DMA_CH1->CHCFG  |=   2<<12;					//优先级高
	DMA_CH1->CHCFG  &= ~(1<<14);				//非存储器到存储器模式
	DMA_CH1->CHSEL   =   0;
	DMA_CH1->CHCFG  |= 1<<0;					//使能DMA
}
 
void TIM1_Conf(u16 pre,u16 per)
 {

	/********************RCC Configure**********************/
	RCC->APB2PCLKEN  |= 1<<12;					//使能TIM1时钟

	/********************TIM Configure**********************/

	TIM1->PSC = per;							//预分频值
	TIM1->AR  = pre;							//自动重装载值
	TIM1->CTRL1 &= 0xFFFFFC8F;					// 0分频  边沿对齐  向上计数
	TIM1->CTRL1 |= 0x00000081;					// 允许自动重装载   使能计数器 
	TIM1->DINTEN |= 1<<0;						//允许更新中断（TIM_IT_Update）
	 
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0x02); 
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
 	 
 }
 

 void TIM3_Conf(u16 pre,u16 per)
 {
  /* Time base configuration */
	RCC->APB1PCLKEN |= 1<<1;					//使能TIM3时钟
	 
	TIM3->PSC = per;							//预分频值
	TIM3->AR  = pre;							//自动重装载值

	TIM3->CTRL1  |= (1<<2);	 
	TIM3->CTRL1 &= 0xFFFFFC8F;					// 0分频  边沿对齐  向上计数
	TIM3->CTRL1 |= 0x00000081;					//允许自动重装载   使能计数器

	TIM3->DINTEN |= 1<<0;						//禁止更新中断（TIM_IT_Update）

	NVIC_SetPriority(TIM3_IRQn,0x01);	 	
	NVIC_EnableIRQ(TIM3_IRQn);
	 
 } 
 
 
 
 
void TIM6_Conf(u16 pre,u16 per)
 {
	 
   RCC->APB1PCLKEN  |= 1<<4;					//使能TIM6时钟
	TIM6->AR = pre;								//自动重装载值
	TIM6->PSC= per;								//预分频值
    TIM6->CTRL1 =0X0081;        	 
	TIM6->STS = 0;								//清除更新中断标志 // tim6无设置 分频系数  向上计数寄存器                      
	TIM6->DINTEN |= 1<<0;						//允许定时器更新中断 
	 
	NVIC_SetPriority(LPTIM_TIM6_IRQn,0x03);		 
	NVIC_EnableIRQ(LPTIM_TIM6_IRQn);
 
 }
 
 
  
void TIM8_Conf(u16 pre,u16 per)
 {
  /* Time base configuration */ 
	RCC->APB2PCLKEN  |= 1<<13;					//使能TIM8时钟
	TIM8->PSC     = per;						//预分频值
    TIM8->AR      = pre;						//自动重装载值
	TIM8->CTRL1 |= (1<<2);	
	TIM8->CTRL1  &= 0xFFFFFC8F;					// 0分频  边沿对齐  向上计数
	TIM8->CTRL1  |= 1<<7;						//允许自动重装载
	TIM8->CTRL1  |= 1<<0;						//使能计数器
	TIM8->REPCNT  = 0;
	TIM8->DINTEN |= ~(1<<0);					//允许更新中断（TIM_IT_Update）
	 
	NVIC_SetPriority(TIM8_BRK_UP_TRG_COM_IRQn,0x03); 
	NVIC_EnableIRQ(TIM8_BRK_UP_TRG_COM_IRQn);
 	 
 }
 
 void USART1_Config(void)
{ 
	u8 EVENODD=2;
	float temp;
	u16 mantissa;
	u16 fraction;	   
    u32 bound;
	
	RCC ->APB2PCLKEN |= 1<<2;					//使能 GPIOA  时钟
	RCC ->APB2PCLKEN |= 1<<14;					//使能 USART1 时钟
//	/* 配置PA2 ,PA3*/
	GPIOA->PMODE     &= 0XFFFFFF0F;				//先将PA 2 3 所在位清零
	GPIOA->PMODE     |= 0X000000A0;				//PA2 3 端口复用
	
	GPIOA->POTYPE    &= 0XFFFFFFF3;				//推挽输出
		
	GPIOA->PUPD      &= 0xFFFFFF0F;
	GPIOA->PUPD      |= 0X00000050;				//上拉
		
	GPIOA->SR        &= 0XFFFFFFF3;				//快速模式

	GPIOA->AFL       &= 0XFFFF00FF;				//先把将要改的位清零
	GPIOA->AFL       |= 0X00000100;				//PIN2 ->  AF_1
	GPIOA->AFL       |= 0X00001000;				// PIN3  -> AF_1		

	if(ModelBaud==1)
		bound = 4800;
	else if (ModelBaud==2)
		bound = 9600;
	else if (ModelBaud==3)
		bound =19200;
	else if (ModelBaud==4)
		bound =38400;
	else if (ModelBaud==5)
		bound =57600;
	else if (ModelBaud==6)
		bound =115200;
	else 
		bound =2400;
	
	temp=(float)(48*1000000)/(bound*16);		//得到USARTDIV
	mantissa=temp;								//得到整数部分
	fraction=(temp-mantissa)*16;				//得到小数部分
	mantissa<<=4;
	mantissa+=fraction;
	
	USART1->BRCF = mantissa;
	
	if(EVENODD==0||EVENODD==1)
		USART1->CTRL1 |= 1<<12;					// USART_WordLength_9b
	else
		USART1->CTRL1 &= 0XFFFFEF;				//USART_WordLength_8b
		USART1->CTRL2 &= 0XFFFFCFFF;			// USART_StopBits_1
	
	if(EVENODD==0)
	{
		USART1->CTRL1   |=1<<10;				//使能校验
		USART1->CTRL1   &=0XFFFFFDFF;			//偶校验
	}
	else if(EVENODD==1)
	{
		USART1->CTRL1   |=1<<10;				//使能校验
		USART1->CTRL1   |=1<<9;					//奇校验		
	}
	else
	{
	  	USART1->CTRL1   &=0xFFFFFBFF;			//校验禁用
	}
 
	USART1->CTRL3 &= 0xFFFFF8FF;				//USART_HardwareFlowControl_None
	
	USART1->CTRL1 |= 1<<3;						//发送器使能
	USART1->CTRL1 |= 1<<2;						//接收器使能
	USART1->CTRL1 |= 1<<5;						//接收中断使能 

 	NVIC_SetPriority(USART1_IRQn,0x00);	
	NVIC_EnableIRQ(USART1_IRQn);

	USART1->CTRL1 |= 1<<13;     //串口使能
}


void TIM1_SetAutoreload(uint32_t Autoload)
{
	TIM1->AR = Autoload;
}
void TIM1_SetCompare1(uint32_t Compare1)
{
	TIM1->CCDAT1 = Compare1;
}



void TIM3_SetAutoreload(uint32_t Autoload)
{
	TIM3->AR = Autoload;
}
void TIM3_SetCompare(uint32_t Compare1)
{
	TIM3->CCDAT2 = Compare1;
}


void TIM8_SetAutoreload(uint32_t Autoload)
{
	TIM8->AR = Autoload;
}
void TIM8_SetCompare(uint32_t Compare1)
{
	TIM8->CCDAT2 = Compare1;
}

//高级定时器
void TIM1_PWM_Init(void)
{
  	RCC ->APB2PCLKEN |= 1<<2;		//使能 GPIOA  时钟
	RCC ->APB2PCLKEN |= 1<<13;		//使能 TIM8 时钟	
	RCC ->APB2PCLKEN |= 1<<2;		//使能 GPIOA  时钟
	RCC ->APB2PCLKEN |= 1<<12;		//使能 TIM1 时钟
	// PA7   ->AF5	
	GPIOA->PMODE &= ~(3<<14);
	GPIOA->PMODE   |= (2<<14);
	GPIOA->POTYPE &= ~(1<<7);
	GPIOA->PUPD &= ~(3<<14);
	GPIOA->PUPD |= (1<<14);
	GPIOA->SR &= ~(1<<7);
	GPIOA->AFL &= ~(15<<28);
	GPIOA->AFL |= 5<<28;	    
		
	TIM1->AR =48000;				//自动重装载值
	TIM1->PSC= 0;					//预分频值
	  
  	TIM1->CTRL1  &= ~(3<<8);		//1分频
	TIM1->CTRL1  |=   1<<7 ;		//自动重装载
	TIM1->CTRL1  &= ~(3<<5);		//边沿对齐
	TIM1->CTRL1  &= ~(1<<4);		//向上计数
	TIM1->REPCNT  =0;
		
	TIM1->CCMOD1  |= (7<<4);		//PWM模式2                  CH1
	TIM1->CCDAT1  = 0;
	TIM1->CCMOD1  |= 1<<3;			//预装载使能
	TIM1->CCEN    &=~(1<<1);		//输出极性(高电平有效)	
	TIM1->CCEN    |= (1<<0);		//ch1 输出使能 		
	TIM1->CCEN    &= ~(1<<3);		//互补输出极性
    TIM1->CCEN    |=   1<<2;		//互补输出使能	
		
	TIM1->BKDT    &= ~(1<<11);
	TIM1->BKDT    &= ~(1<<10);		
	TIM1->BKDT    &= ~(3<<8);	
		
	TIM1->BKDT    &= ~(255<<0);
    TIM1->BKDT    |= 0x90;			
	TIM1->BKDT    &= ~(1<<12);
	TIM1->BKDT    |=  (1<<13);		
	TIM1->BKDT    |=  (1<<14);
		
    TIM1->CTRL1  |= 1<<0;			//使能计数器	
    TIM1->BKDT   |= (1<<15);		
	TIM1_SetAutoreload(48000); 
	
} 

//通用定时器
void TIM3_PWM_Init(int16_t  arr,int16_t  psc)
{
  	RCC ->APB2PCLKEN |= 1<<2;		//使能 GPIOA  时钟
	RCC ->APB2PCLKEN |= 1<<1;		//使能 TIM3 时钟
		
	/*******************GPIO Configure**********************/
	
	//PA6 -> TIM3_CH1 -> AF2
	GPIOA->PMODE &= ~(3<<12);
	GPIOA->PMODE   |= (2<<12);
	GPIOA->POTYPE &= ~(1<<6);
	GPIOA->PUPD &= ~(3<<12);
	GPIOA->PUPD |= (1<<12);
	GPIOA->SR &= ~(1<<6);
	GPIOA->AFL  &= ~(15<<24);
	GPIOA->AFL  |= 1<<25;			// PA6   ->AF2
	
	//PA7 -> TIM3_CH2 -> AF2
	GPIOA->PMODE &= ~(3<<14);
	GPIOA->PMODE   |= (2<<14);
	GPIOA->POTYPE &= ~(1<<7);
	GPIOA->PUPD &= ~(3<<14);
	GPIOA->PUPD |= (1<<14);
	GPIOA->SR &= ~(1<<7);
	GPIOA->AFL  &= ~(15<<28);
	GPIOA->AFL  |= 1<<29;			// PA7   ->AF2
	
	/********************TIM Configure**********************/
	TIM3->AR =arr;					//自动重装载值
	TIM3->PSC= psc;					//预分频值
	  
  	TIM3->CTRL1  &= ~(3<<8);		//1分频
	TIM3->CTRL1  |=   1<<7 ;		//自动重装载
	TIM3->CTRL1  &= ~(3<<5);		//边沿对齐
	TIM3->CTRL1  &= ~(1<<4);		//向上计数
	TIM3->REPCNT  =0;
		
	TIM3->CCMOD1  |= (7<<4);		//PWM模式              CH1
	TIM3->CCDAT1  = 0;
	TIM3->CCMOD1  |= 1<<3;			//预装载使能
	TIM3->CCEN    &=~(1<<1);		//输出极性(高电平有效)	
	TIM3->CCEN    |= (1<<0);		//ch1 输出使能 		
	TIM3->CCEN    &= ~(1<<3);		//互补输出极性
    TIM3->CCEN    |=   1<<2;		//互补输出使能	
		
    TIM3->CTRL1  |= 1<<0;			//使能计数器	
    TIM3->BKDT   |= (1<<15);		
	TIM3_SetAutoreload(48000); 
	
} 



void InitALL()
{
	SystemInit();
	delay_init(48); 
	GPIOInit();
	USART1_Config();
	ReadInf();
	delay_ms(500);
	GetVolt();	
	FlashCheck();
	GetLockCode();
}


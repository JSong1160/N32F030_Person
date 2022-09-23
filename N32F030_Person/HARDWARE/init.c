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
	RCC ->APB2PCLKEN |= 1<<2;  //ʹ�� GPIOA  ʱ��
	RCC ->APB2PCLKEN |= 1<<3;  //ʹ�� GPIOB  ʱ��
	RCC ->APB2PCLKEN |= 1<<0;  //ʹ��(AFIO)����ʱ��
	
	/********************GPIO Configure*****************/
	/****************************************************
	*	PMODE˵ ��:
	*		00: ����ģʽ
	*		01: ͨ�����ģʽ
	*		10: ���ù���ģʽ
	*		11: ģ�⹦��ģʽ
	*
	*	POTYPE˵��:
	*		0: �������ģʽ����λ���״̬��
	*		1: �����©ģʽ
	*
	*	PUPD˵��:
	*		00������/����
	*		01������
	*		10������
	*		11������
	*****************************************************
	*/

	/********************GPIO Configure*****************/
	//	PA 1
    GPIOA->PMODE  &= ~(3<<2); 
    GPIOA->PMODE  |= 1<<2;			// ��� 	 
 
	GPIOA->POTYPE &= ~(1<<2);		//���� 

    GPIOA->PUPD  &= ~(3<<2);		//���ϲ���  
    GPIOA->PUPD  |= (1<<2);			//����		   				
			 
	GPIOA->SR      &= ~(1<<1);		//����GPIO
	 

 }
 
void ADC_Config()
{
	/********************RCC Configure**********************/

	RCC ->AHBPCLKEN  &= ~(1<<12); //ADCʱ��ʧ��
    RCC ->AHBPCLKEN  |=   1<<12;  //ADCʱ��ʹ��
	RCC ->APB2PCLKEN |= 1<<2;    //ʹ�� GPIOA  ʱ�� 

  	RCC ->CFG2     &= ~(1<<10);  // HSI��ADC 1M ʱ��Դ	
    RCC ->CFG2     &= ~(15<<11);
    RCC ->CFG2     &= ~(1<<15);
    RCC ->CFG2     |=  (7<<11);	//�˷�Ƶ    //ʵ���ڲ�ʱ�Ӽ�ʱʱ��1MhzƵ�ʵ�����
    
	RCC ->CFG2     &= ~(1<<8);//ADCPLLʱ�� ���ر�
	
	RCC ->CFG2     &= ~(15<<0);
    RCC ->CFG2     |=  (2<<0);	//4��Ƶ   48/4=12MHz     ADC�Ĺ���ʱ��

	
	/*****************GPIO Configure************************/	

	GPIOA->PMODE   |=  (3<<0);   // PA0ģ�⹦�� 
	GPIOA->PUPD    &= ~(3<<0);   //��������
	GPIOA->SR      &= ~(1<<0);   //����

	/*****************ADC Configure*************************/

	ADC->CTRL1     |=   1<<8;  //ʹ��ɨ��ģʽ	
    ADC->CTRL2     |= 1<<1;  //����ת��
    ADC->CTRL2     |=   7<<17; 
    ADC->CTRL2     &= ~(1<<15);//�����ⲿ�¼�����ת��	
	ADC->CTRL2     &= ~(1<<11);//�Ҷ���	
		
	ADC->RSEQ1     &= ~(15<<20);
    ADC->RSEQ1     |=  (0<<20); //ͨ����Ŀһ��
		
	ADC->RSEQ3     &=~(15<<0); 
	ADC->RSEQ3     |= (0<<0); 	// ͨ��0 ��һ��ת��

		
	ADC->SAMPT3    &=~(15<<0); 
	ADC->SAMPT3    |= (6<<0);	 //CH0    56
		
	ADC->CTRL2     |=   1<<8;    //ʹ��DMAģʽ
	ADC->CTRL2     |=   1<<0;    
		
	while(!(ADC->CTRL3 & 0X20));
	while(ADC->CTRL3 & 0X40);	
		
	ADC->CTRL2 |= 0X00500000;
}


void DMA_Config()
{
	/********************RCC Configure**********************/
	RCC->AHBPCLKEN |= 1<<0;  //ʹ��DMAʱ��

	/********************DMA Configure**********************/
	DMA_CH1->CHCFG = 0;
	DMA_CH1->TXNUM = 0;
	DMA_CH1->PADDR = 0;
	DMA_CH1->MADDR = 0;	
	DMA->INTCLR |= 0X0000000F;					//��λDMA
	
	DMA_CH1->PADDR  = (uint32_t)ADC1_DR_Address;
	DMA_CH1->MADDR  = (uint32_t)RegularConvData_Tab;
	DMA_CH1->CHCFG  &= ~(1<<4);					//���������д��洢��
	DMA_CH1->TXNUM   =   160;
	DMA_CH1->CHCFG  &= ~(1<<6);					//����̶���ַģʽ
	DMA_CH1->CHCFG	|=   1<<7;					// �洢��ַ����
	DMA_CH1->CHCFG  &= ~(3<<8);
	DMA_CH1->CHCFG  |=  (1<<8);					//�������ݿ��HalfWord	
	DMA_CH1->CHCFG  &= ~(3<<10);
	DMA_CH1->CHCFG  |=  (1<<10);				//�洢�����ݿ��HalfWord		
	DMA_CH1->CHCFG  |=   1<<5;					//ѭ��ģʽ
	DMA_CH1->CHCFG  &= ~(3<<12);  
	DMA_CH1->CHCFG  |=   2<<12;					//���ȼ���
	DMA_CH1->CHCFG  &= ~(1<<14);				//�Ǵ洢�����洢��ģʽ
	DMA_CH1->CHSEL   =   0;
	DMA_CH1->CHCFG  |= 1<<0;					//ʹ��DMA
}
 
void TIM1_Conf(u16 pre,u16 per)
 {

	/********************RCC Configure**********************/
	RCC->APB2PCLKEN  |= 1<<12;					//ʹ��TIM1ʱ��

	/********************TIM Configure**********************/

	TIM1->PSC = per;							//Ԥ��Ƶֵ
	TIM1->AR  = pre;							//�Զ���װ��ֵ
	TIM1->CTRL1 &= 0xFFFFFC8F;					// 0��Ƶ  ���ض���  ���ϼ���
	TIM1->CTRL1 |= 0x00000081;					// �����Զ���װ��   ʹ�ܼ����� 
	TIM1->DINTEN |= 1<<0;						//��������жϣ�TIM_IT_Update��
	 
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0x02); 
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
 	 
 }
 

 void TIM3_Conf(u16 pre,u16 per)
 {
  /* Time base configuration */
	RCC->APB1PCLKEN |= 1<<1;					//ʹ��TIM3ʱ��
	 
	TIM3->PSC = per;							//Ԥ��Ƶֵ
	TIM3->AR  = pre;							//�Զ���װ��ֵ

	TIM3->CTRL1  |= (1<<2);	 
	TIM3->CTRL1 &= 0xFFFFFC8F;					// 0��Ƶ  ���ض���  ���ϼ���
	TIM3->CTRL1 |= 0x00000081;					//�����Զ���װ��   ʹ�ܼ�����

	TIM3->DINTEN |= 1<<0;						//��ֹ�����жϣ�TIM_IT_Update��

	NVIC_SetPriority(TIM3_IRQn,0x01);	 	
	NVIC_EnableIRQ(TIM3_IRQn);
	 
 } 
 
 
 
 
void TIM6_Conf(u16 pre,u16 per)
 {
	 
   RCC->APB1PCLKEN  |= 1<<4;					//ʹ��TIM6ʱ��
	TIM6->AR = pre;								//�Զ���װ��ֵ
	TIM6->PSC= per;								//Ԥ��Ƶֵ
    TIM6->CTRL1 =0X0081;        	 
	TIM6->STS = 0;								//��������жϱ�־ // tim6������ ��Ƶϵ��  ���ϼ����Ĵ���                      
	TIM6->DINTEN |= 1<<0;						//����ʱ�������ж� 
	 
	NVIC_SetPriority(LPTIM_TIM6_IRQn,0x03);		 
	NVIC_EnableIRQ(LPTIM_TIM6_IRQn);
 
 }
 
 
  
void TIM8_Conf(u16 pre,u16 per)
 {
  /* Time base configuration */ 
	RCC->APB2PCLKEN  |= 1<<13;					//ʹ��TIM8ʱ��
	TIM8->PSC     = per;						//Ԥ��Ƶֵ
    TIM8->AR      = pre;						//�Զ���װ��ֵ
	TIM8->CTRL1 |= (1<<2);	
	TIM8->CTRL1  &= 0xFFFFFC8F;					// 0��Ƶ  ���ض���  ���ϼ���
	TIM8->CTRL1  |= 1<<7;						//�����Զ���װ��
	TIM8->CTRL1  |= 1<<0;						//ʹ�ܼ�����
	TIM8->REPCNT  = 0;
	TIM8->DINTEN |= ~(1<<0);					//��������жϣ�TIM_IT_Update��
	 
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
	
	RCC ->APB2PCLKEN |= 1<<2;					//ʹ�� GPIOA  ʱ��
	RCC ->APB2PCLKEN |= 1<<14;					//ʹ�� USART1 ʱ��
//	/* ����PA2 ,PA3*/
	GPIOA->PMODE     &= 0XFFFFFF0F;				//�Ƚ�PA 2 3 ����λ����
	GPIOA->PMODE     |= 0X000000A0;				//PA2 3 �˿ڸ���
	
	GPIOA->POTYPE    &= 0XFFFFFFF3;				//�������
		
	GPIOA->PUPD      &= 0xFFFFFF0F;
	GPIOA->PUPD      |= 0X00000050;				//����
		
	GPIOA->SR        &= 0XFFFFFFF3;				//����ģʽ

	GPIOA->AFL       &= 0XFFFF00FF;				//�Ȱѽ�Ҫ�ĵ�λ����
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
	
	temp=(float)(48*1000000)/(bound*16);		//�õ�USARTDIV
	mantissa=temp;								//�õ���������
	fraction=(temp-mantissa)*16;				//�õ�С������
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
		USART1->CTRL1   |=1<<10;				//ʹ��У��
		USART1->CTRL1   &=0XFFFFFDFF;			//żУ��
	}
	else if(EVENODD==1)
	{
		USART1->CTRL1   |=1<<10;				//ʹ��У��
		USART1->CTRL1   |=1<<9;					//��У��		
	}
	else
	{
	  	USART1->CTRL1   &=0xFFFFFBFF;			//У�����
	}
 
	USART1->CTRL3 &= 0xFFFFF8FF;				//USART_HardwareFlowControl_None
	
	USART1->CTRL1 |= 1<<3;						//������ʹ��
	USART1->CTRL1 |= 1<<2;						//������ʹ��
	USART1->CTRL1 |= 1<<5;						//�����ж�ʹ�� 

 	NVIC_SetPriority(USART1_IRQn,0x00);	
	NVIC_EnableIRQ(USART1_IRQn);

	USART1->CTRL1 |= 1<<13;     //����ʹ��
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

//�߼���ʱ��
void TIM1_PWM_Init(void)
{
  	RCC ->APB2PCLKEN |= 1<<2;		//ʹ�� GPIOA  ʱ��
	RCC ->APB2PCLKEN |= 1<<13;		//ʹ�� TIM8 ʱ��	
	RCC ->APB2PCLKEN |= 1<<2;		//ʹ�� GPIOA  ʱ��
	RCC ->APB2PCLKEN |= 1<<12;		//ʹ�� TIM1 ʱ��
	// PA7   ->AF5	
	GPIOA->PMODE &= ~(3<<14);
	GPIOA->PMODE   |= (2<<14);
	GPIOA->POTYPE &= ~(1<<7);
	GPIOA->PUPD &= ~(3<<14);
	GPIOA->PUPD |= (1<<14);
	GPIOA->SR &= ~(1<<7);
	GPIOA->AFL &= ~(15<<28);
	GPIOA->AFL |= 5<<28;	    
		
	TIM1->AR =48000;				//�Զ���װ��ֵ
	TIM1->PSC= 0;					//Ԥ��Ƶֵ
	  
  	TIM1->CTRL1  &= ~(3<<8);		//1��Ƶ
	TIM1->CTRL1  |=   1<<7 ;		//�Զ���װ��
	TIM1->CTRL1  &= ~(3<<5);		//���ض���
	TIM1->CTRL1  &= ~(1<<4);		//���ϼ���
	TIM1->REPCNT  =0;
		
	TIM1->CCMOD1  |= (7<<4);		//PWMģʽ2                  CH1
	TIM1->CCDAT1  = 0;
	TIM1->CCMOD1  |= 1<<3;			//Ԥװ��ʹ��
	TIM1->CCEN    &=~(1<<1);		//�������(�ߵ�ƽ��Ч)	
	TIM1->CCEN    |= (1<<0);		//ch1 ���ʹ�� 		
	TIM1->CCEN    &= ~(1<<3);		//�����������
    TIM1->CCEN    |=   1<<2;		//�������ʹ��	
		
	TIM1->BKDT    &= ~(1<<11);
	TIM1->BKDT    &= ~(1<<10);		
	TIM1->BKDT    &= ~(3<<8);	
		
	TIM1->BKDT    &= ~(255<<0);
    TIM1->BKDT    |= 0x90;			
	TIM1->BKDT    &= ~(1<<12);
	TIM1->BKDT    |=  (1<<13);		
	TIM1->BKDT    |=  (1<<14);
		
    TIM1->CTRL1  |= 1<<0;			//ʹ�ܼ�����	
    TIM1->BKDT   |= (1<<15);		
	TIM1_SetAutoreload(48000); 
	
} 

//ͨ�ö�ʱ��
void TIM3_PWM_Init(int16_t  arr,int16_t  psc)
{
  	RCC ->APB2PCLKEN |= 1<<2;		//ʹ�� GPIOA  ʱ��
	RCC ->APB2PCLKEN |= 1<<1;		//ʹ�� TIM3 ʱ��
		
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
	TIM3->AR =arr;					//�Զ���װ��ֵ
	TIM3->PSC= psc;					//Ԥ��Ƶֵ
	  
  	TIM3->CTRL1  &= ~(3<<8);		//1��Ƶ
	TIM3->CTRL1  |=   1<<7 ;		//�Զ���װ��
	TIM3->CTRL1  &= ~(3<<5);		//���ض���
	TIM3->CTRL1  &= ~(1<<4);		//���ϼ���
	TIM3->REPCNT  =0;
		
	TIM3->CCMOD1  |= (7<<4);		//PWMģʽ              CH1
	TIM3->CCDAT1  = 0;
	TIM3->CCMOD1  |= 1<<3;			//Ԥװ��ʹ��
	TIM3->CCEN    &=~(1<<1);		//�������(�ߵ�ƽ��Ч)	
	TIM3->CCEN    |= (1<<0);		//ch1 ���ʹ�� 		
	TIM3->CCEN    &= ~(1<<3);		//�����������
    TIM3->CCEN    |=   1<<2;		//�������ʹ��	
		
    TIM3->CTRL1  |= 1<<0;			//ʹ�ܼ�����	
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


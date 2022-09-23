/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32g030_it.c
 * @author Nations 
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g030_it.h"
#include "main.h"
#include "include.h"
/** @addtogroup N32G030_StdPeriph_Template
 * @{
 */

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}


/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 N32G030 Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_n32g030.s).                                                 */
/******************************************************************************/


extern __IO uint8_t g_mods_timeout;
 u8 tim_flag=0;
void TIM3_IRQHandler(void)			
{
	if(TIM3->STS&0X00000001) 
	{ 
		if(tim_flag!=0)
		 {
	   g_mods_timeout=1;
	   TIM3->DINTEN &= ~(1<<0);         //  禁止更新中断（TIM_IT_Update）
		 }
    else		
		 tim_flag++;
	}
	TIM3->STS&=~(1<<0);//清除中断标志位 	 
}




u8 leda=0;
void USART1_IRQHandler(void)
{
  if(USART1->STS&(1<<5))
	{
	MODS_ReciveNew(USART1->DAT);
	}
}
u16 CO2_tem,CO2_last;
u8 tempdata[13],tickco2,n=1;
s16 CO2Off,ischange,CO2F,CO2Mult;
void USART1r(u8 byte)
{
	tempdata[0]=tempdata[1];
	tempdata[1]=tempdata[2];
	tempdata[2]=tempdata[3];
	tempdata[3]=tempdata[4];
	tempdata[4]=tempdata[5];
	tempdata[5]=tempdata[6];
	tempdata[6]=tempdata[7];
	tempdata[7]=tempdata[8];
	tempdata[8]=tempdata[9];
	tempdata[9]=tempdata[10];
	tempdata[10]=tempdata[11];
	tempdata[11]=byte;
	
  if(tempdata[1]==0x20&&tempdata[2]==0x20&&tempdata[6]==0x20&&tempdata[7]==0x70)
	{
		CO2=(tempdata[3]&0x0f)*100+(tempdata[4]&0x0f)*10+(tempdata[5]&0x0f);
		tempdata[0]=0;
		tickco2++;
	}
	else if (tempdata[1]==0x20&&(tempdata[2]&0xf0)==0x30&&tempdata[6]==0x20&&tempdata[7]==0x70)
	{
		CO2=(tempdata[2]&0x0f)*1000+(tempdata[3]&0x0f)*100+(tempdata[4]&0x0f)*10+(tempdata[5]&0x0f);
		tempdata[0]=0;
		tickco2++;
	}
	else if (tempdata[0]==0x20&&(tempdata[1]&0xf0)==0x30&&tempdata[6]==0x20&&tempdata[7]==0x70)
	{
		CO2=(tempdata[1]&0x0f)*10000+(tempdata[2]&0x0f)*1000+(tempdata[3]&0x0f)*100+(tempdata[4]&0x0f)*10+(tempdata[5]&0x0f);
		tempdata[0]=0;
		tickco2++;
	}	
	if(CO2>cof_DACmax)
		CO2=cof_DACmax;
	return ;
}


void openexit(void)
 {
	 
	AFIO->EXTI_CFG[0] |= 0X00000010;//PB1 -- EXTI1
  EXTI->IMASK    |= 0X00000002; //开放屏蔽线1上的中断请求
  EXTI->FT_CFG   |= 0X00000002; //下降沿触发  
 }

void closeexit(void)
 {
 	 AFIO->EXTI_CFG[0] &= 0XFFFFFF0F;
	 AFIO->EXTI_CFG[0] |= 0X00000010;//PB1 -- EXTI1
		
   EXTI->IMASK  &= ~(1<<1); //开放屏蔽线1上的中断请求
	 EXTI->EMASK  &= ~(1<<1);    //屏蔽事件请求	
	 EXTI->FT_CFG |=   1<<1; //下降沿触发
   EXTI->RT_CFG &= ~(1<<1);    //禁止上升沿触发	
 }
 
u8 co2tick=0;u16 co2history[5];
u8 fisrt=0;
u8 recbittick;
u8 recbit[10];
u8 outtimetick=0;
u8 recstyle=0;
u8 tim8_count; 
 
void LPTIM_TIM6_IRQHandler(void)			
{
	uint8_t tmp;
	if(TIM6->STS&0X00000001) 
	{	
	  if(recbittick<10)
		{
			recbit[recbittick]=(GPIOB->PID & 0x00000002)?1:0;
			if(recbittick<9)
			{
				recbittick++;
			}
			else		//最后一次已经采样完毕
			{
				if(recbit[0]==0&&recbit[9]==1)		//起始位和结束位都正常
				{
					TIM6->DINTEN &= ~(1<<0);         //  禁止更新中断（TIM_IT_Update）	
					openexit();					
					//执行串口中断
					tmp=recbit[1]+recbit[2]*2+recbit[3]*4+recbit[4]*8+recbit[5]*16+recbit[6]*32+recbit[7]*64+recbit[8]*128;
					recbittick=0;
					USART1r(tmp);				
					recstyle=0;					
				}
				else
				{
					if(outtimetick++>10)
					{
						TIM6->DINTEN &= ~(1<<0);         //  禁止更新中断（TIM_IT_Update）	
						openexit();					//打开串口
						recstyle=0;
						return ;
					}
					recbit[0]=recbit[1];
					recbit[1]=recbit[2];
					recbit[2]=recbit[3];
					recbit[3]=recbit[4];
					recbit[4]=recbit[5];
					recbit[5]=recbit[6];
					recbit[6]=recbit[7];
					recbit[7]=recbit[8];
					recbit[8]=recbit[9];
				}				
			}	
		}
		else
		 {
		    recbittick = 0; 
		 }
	}
	TIM6->STS&=~(1<<0);//清除中断标志位 	 
}

void EXTI0_1_IRQHandler(void)
{	
	if((EXTI->PEND&(1<<1)) != RESET)
  {  
   	EXTI->PEND |= (1<<1);	   //EXTI_ClearITPendingBit(EXTI_Line1);
	  TIM6->DINTEN |= 1<<0;         //  允许更新中断（TIM_IT_Update）
	  TIM6->CNT=0;
	while(TIM6->CNT>=52);		//延时
	if((GPIOB->PID&(1<<1))==0)		//PB1仍旧是低电平，有效信号
	{
		TIM6->CNT=0;
		closeexit();		//关外部中断
		recbittick=1;		
		recbit[0]=0;

		recstyle=1;
	}
	else
	{
		TIM6->DINTEN &= ~(1<<0);         //  禁止更新中断（TIM_IT_Update）	
		recstyle=0;
	}	  
  }
}


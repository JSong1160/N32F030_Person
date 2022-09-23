#include "include.h"
 
 //2KB
#define FLASH_USER_START_ADDR    ((uint32_t)0x08007000)  /* Start @ of user Flash area */

 u8 CpuID[12];
 u16 cpuID[6];
 u16 cof_NH3cheng,cof_NH3chu,DQSelect,DACSelect,cof_DACcheng,cof_DACchu,cof_5vcheng,cof_5vchu,DAC2_Style,cof_yurevalue,DACType;
 s16 cof_DACb;
 u32 cof_DACmax,temp1;
 u16 Shangxian[2],Xiaxian[2],HuiLuo[2];
 u16 Flash_Check;
 u8 EVENODD;
 u16 Volt1,Volt2,NH3,NH3offset,M_Dac1,M_Dac2;
 u16 act_send;
 
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
__IO FLASH_STS FLASHStatus ;

void GetLockCode(void)
{
 u32 CpuID[3];
 //获取CPU唯一ID
 CpuID[0]=*(vu32*)(0x1ffff4fc);
 CpuID[1]=*(vu32*)(0x1ffff500);
 CpuID[2]=*(vu32*)(0x1ffff504);
 cpuID[0]=CpuID[0]>>16;
 cpuID[1]=CpuID[0];
 cpuID[2]=CpuID[1]>>16;
 cpuID[3]=CpuID[1];
 cpuID[4]=CpuID[2]>>16;
 cpuID[5]=CpuID[2];
 //加密算法,很简单的加密算法
}

void ResetInf()
{
	  /* Unlock the Flash to enable the flash control register access *************/ 
  FLASH_Unlock();
    
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
  /* Erase the FLASH pages */

  if (FLASH_EraseOnePage(FLASH_USER_START_ADDR )!= FLASH_COMPL)
		{
      while (1)
      {
      }
		}
		
  Address = FLASH_USER_START_ADDR;

  while(FLASH_ProgramWord(FLASH_USER_START_ADDR, 0x12345678) != FLASH_COMPL);
  while(FLASH_ProgramWord(FLASH_USER_START_ADDR+4, 0x1) != FLASH_COMPL);
  Address = Address + 4;
   while(FLASH_ProgramWord(FLASH_USER_START_ADDR+8, 0x2) != FLASH_COMPL);		//0 2400， 1 4800 2 9600
  Address = Address + 4;
  while(FLASH_ProgramWord(FLASH_USER_START_ADDR+12, 0xffffffff) != FLASH_COMPL);
  Address = Address + 4;	  
}
void WriteInf()
{
	 FLASH_Unlock();  
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
  /* Erase the FLASH pages */
    if (FLASH_EraseOnePage(FLASH_USER_START_ADDR)!= FLASH_COMPL)
      while (1)
      {
		  
      }
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR, 0x12345678) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+4, ModelID) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+8, ModelBaud) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+12, NH3offset) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+16, cof_jixing) != FLASH_COMPL);  
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+20, cof_showstyle) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+24, cof_NH3cheng) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+28, cof_NH3chu) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+32, cof_DACmax) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+36, cof_DACcheng) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+40, cof_DACchu) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+44, cof_DACb) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+48, cof_5vcheng) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+52, cof_5vchu) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+56, cof_yuretime) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+60, cof_yurevalue) != FLASH_COMPL);
  while(FLASH_ProgramWord(FLASH_USER_START_ADDR+64, Shangxian[0]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+68, Shangxian[1]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+72, Xiaxian[0]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+76, Xiaxian[1]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+80, HuiLuo[0]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+84, HuiLuo[1]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+88, TemOff[0]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+96, HumOff[0]) != FLASH_COMPL);
	while(FLASH_ProgramWord(FLASH_USER_START_ADDR+100, act_send) != FLASH_COMPL);

  FLASH_Lock();
}



void FlashCheck()
{
	u16 i =0;
	Flash_Check=0;
	for(i=0;i<256;i++)
	Flash_Check+=*(__IO uint32_t*)(0x8000000+i*4);
	for(i=0;i<256;i++)
	Flash_Check+=*(__IO uint32_t*)(0x8001000+i*4);
	for(i=0;i<256;i++)
	Flash_Check+=*(__IO uint32_t*)(0x8002000+i*4);
	for(i=0;i<256;i++)
	Flash_Check+=*(__IO uint32_t*)(0x8003000+i*4);
}
void ReadInf()
{
  u32 dateread[4];		//4个字节的存储信息
  Address = FLASH_USER_START_ADDR;

	dateread[0] = *(__IO uint32_t *)(FLASH_USER_START_ADDR);     //  确认是否是所要用的地址
	dateread[1] = *(__IO uint32_t *)(FLASH_USER_START_ADDR+4);   //  ModelID
	dateread[2] = *(__IO uint32_t *)(FLASH_USER_START_ADDR+8);   //  ModelBaud
	dateread[3] = *(__IO uint32_t *)(FLASH_USER_START_ADDR+12);
	NH3offset=dateread[3];
	cof_jixing=*(__IO uint32_t *)(FLASH_USER_START_ADDR+16);
	cof_showstyle=*(__IO uint32_t *)(FLASH_USER_START_ADDR+20);
	cof_NH3cheng=*(__IO uint32_t *)(FLASH_USER_START_ADDR+24);
	cof_NH3chu=*(__IO uint32_t *)(FLASH_USER_START_ADDR+28);
	cof_DACmax=*(__IO uint32_t *)(FLASH_USER_START_ADDR+32);
	cof_DACcheng=*(__IO uint32_t *)(FLASH_USER_START_ADDR+36);
	cof_DACchu=*(__IO uint32_t *)(FLASH_USER_START_ADDR+40);
	cof_DACb=*(__IO uint32_t *)(FLASH_USER_START_ADDR+44);
	cof_5vcheng=*(__IO uint32_t *)(FLASH_USER_START_ADDR+48);
	cof_5vchu=*(__IO uint32_t *)(FLASH_USER_START_ADDR+52);
	cof_yuretime=*(__IO uint32_t *)(FLASH_USER_START_ADDR+56);
	cof_yurevalue=*(__IO uint32_t *)(FLASH_USER_START_ADDR+60);	
	Shangxian[0]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+64);
	Shangxian[1]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+68);
	Xiaxian[0]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+72);
	Xiaxian[1]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+76);
	HuiLuo[0]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+80);
	HuiLuo[1]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+84);
	TemOff[0]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+88);
	HumOff[0]=*(__IO uint32_t *)(FLASH_USER_START_ADDR+96);	
	act_send =*(__IO uint32_t *)(FLASH_USER_START_ADDR+100);
	
	ModelID=dateread[1];
	if(ModelID>253)
		ModelID=253;
	ModelBaud=dateread[2];
	if(ModelBaud>6)				//0 1 2 3 4 5 6 - 2400 4800 9600 19200 38400 57600 115200
		ModelBaud=2;
	
	NH3offset=dateread[3];
	
	if(TemOff[0]==-1&&HumOff[0]==-1)
	{
	 TemOff[0]=0;
	 HumOff[0]=0;
	}
	if(cof_showstyle>5)
		cof_showstyle=0;
  if(dateread[0]!=0x12345678)	//如果是新的内存块
  {
	 ResetInf();
	 ReadInf(); 
  }
	  FLASH_Lock();
}


void RS485_SendBuf(u8 *data,u16 len)
{
	u16  i;
	GPIOA->POD |= 1<<4;   //485使能端(PA4)置高
	
	for(i=0;i<len;i++)
	{
		USART1->DAT = (uint8_t)data[i];
		while((USART1->STS & (1<<7))==RESET);  
	}
	  while((USART1->STS & (1<<6))==RESET);  //判断是否完成
	
	GPIOA->POD &= ~(1<<4);   //485使能端(PA4)置低
}



void SetSDAPP()
{
   RCC->APB2PCLKEN |= 1<<2; //使能GPIOA时钟
	 //PA5
	 GPIOA->PMODE  &= 0XFFFFF3FF;
	 GPIOA->PMODE  |= 0X00000400;  //PA5 输出
   
	 GPIOA->POTYPE &= ~(1<<5);     //推挽输出

	 GPIOA->PUPD   &= 0XFFFFF3FF;
	 GPIOA->PUPD   |= 0X00000400;  //上拉
	
	 GPIOA->SR     &= ~(1<<5);     //高速
	
	 GPIOA->POD    |= 1<<5;   //输出高
}
void SetSDAOD()
{
	 RCC->APB2PCLKEN |= 1<<2; //使能GPIOA时钟
	 //PA5
	 GPIOA->PMODE  &= ~(3<<10);  //PA5 输入
	 GPIOA->POTYPE |= 1<<5;        //开漏模式

	 GPIOA->PUPD   &= 0XFFFFF3FF;
	 GPIOA->PUPD   |= 0X00000400;  //上拉
	
	 GPIOA->SR     &= ~(1<<5);     //高速	
}





void DAC7512(u16 DAData)			//满电压是5000mV  4096分度值
{
 u8 i;
 u32 temp=DAData;
 u32 temp1=NH3,temp2;
 if(DACType==1)
   temp2=temp1*50000/cof_DACmax/100;
 else if(DACType==2) 
	 temp2=temp1*100000/cof_DACmax/100;
 else
	 temp2=temp1*160000/cof_DACmax/100+400;
   M_Dac1=temp2;
	SetSDAPP();
	
	GPIOA->POD &= ~(1<<7);  //IO_SCLK(PA7)
	GPIOA->POD &= ~(1<<5);  //IO_CS(PA5)
	GPIOA->POD |=   1<<7;   //IO_SCLK(PA7)
	temp=temp*cof_5vcheng/cof_5vchu;//    /5000*4096
	if(temp>4095)
		temp=4095;
	DAData=temp;
	for(i=0;i<16;i++)//AD7888写入命令与获取数值一次。
	{  
		if((DAData&(0x8000>>i))>>(15-i))
			GPIOA->POD  |= 1<<6 ; //PA7置高
		else
		GPIOA->POD    &= ~(1<<6) ; //PA7置低

		GPIOA->POD    &= ~(1<<7);//PA1//下降写入
		GPIOA->POD    &= ~(1<<6);//PA7 启动转换;//每次数据移入后，DIN还需要置0真变态
    GPIOA->POD    |=  1<<7; 
	}
	GPIOA->POD &= ~(1<<7);  // 这里还需要SCLK置0，更变态,datasheet上却没提
	GPIOA->POD |=   1<<5;
	
	SetSDAOD();
}

u16 lstVolt1;
u8 votick=0;
u16 time=0,ledtime=1;
u8 ledon=0;
void GetVolt()
{
	u8 i;
	u32 temp=0;
	for(i=0;i<160;i++)
		temp+=RegularConvData_Tab[i];  
	temp=temp/160;
	temp=temp*825/1024;

	if(votick==9)
	{
		RegularConvData_Tab2[votick++]=temp;
		votick=0;
		temp=0;
		for(i=0;i<10;i++)
		temp+=RegularConvData_Tab2[i]; 
		Volt1=temp;
		Volt1=(Volt1+lstVolt1)/2;
		lstVolt1=Volt1; 
		temp=Volt1;
		if((GPIOA->PID&(1<<13))==0||NH3offseton==1)
		 {
			time++;
			if(time>6)
			 {
				if(cof_yuretime!=80)
				 {
				  NH3offset=Volt1+10;
				 }
				else
				 {
				  NH3offset=Volt1-10;
				 }
				WriteInf();
				ledon=1;
				time=0;
				NH3offseton=0;
			 }
			}
		 if(ledon)
			{
			 if(ledtime%2==1)
				{
					GPIOA->POD |=   1<<10;
//				 GPIO_SetBits(GPIOA,GPIO_Pin_10);
				}
			 else
				{
					GPIOA->POD |=   0<<10;
//				 GPIO_ResetBits(GPIOA,GPIO_Pin_10);
				}
			 ledtime++;
			 if(ledtime>=10)
				{
				 ledon=0;
				 ledtime=1;
				}
			}
			//if(1)		//都是正极性
		if(!(cof_yuretime&(1<<6)))		//正极性,BIAS 是1代表正极性
		{
			if(temp<=NH3offset)
			{
				NH3=0;
				return ;
			}
			else
				NH3=Volt1-NH3offset;
			}
		else				//反极性
		{
			if(temp>=NH3offset)
			{
				NH3=0;
				return ;
			}
			else  
				NH3=NH3offset-Volt1;	
		}	
		temp=NH3;
		temp=temp*cof_NH3cheng/cof_NH3chu;
		if(yurenowtianme<5000)		//预热时间恒定五秒钟
			NH3=cof_yurevalue;
		else	
			NH3=temp;		
				
		if(NH3>cof_DACmax)//设定上限
		 NH3=cof_DACmax;
	}
	else
	{
		RegularConvData_Tab2[votick++]=temp;
	}
			
			
			
		
		

//    if(cof_showstyle==0||cof_showstyle==2||cof_showstyle==4)		
//		 temp=temp/10;
//		else
//		 temp=temp;
//		
//		Volt1=temp;
//		Volt1=(Volt1+lstVolt1)/2;
//		
//		lstVolt1=Volt1;
//		temp=Volt1;
//		if((GPIOA->PID&(1<<13))==0||NH3offseton==1)
//		 {
//			time++;
//			if(time>6)
//			 {
//				if(cof_yuretime!=80)
//				 {
//				  NH3offset=Volt1+10;
//				 }
//				else
//				 {
//				  NH3offset=Volt1-10;
//				 }
//				WriteInf();
//				ledon=1;
//				time=0;
//				NH3offseton=0;
//			 }
//			}
//		 if(ledon)
//			{
//			 if(ledtime%2==1)
//				{
//					GPIOA->POD |=   1<<10;
//				}
//			 else
//				{
//					GPIOA->POD &= ~(1<<10);
//				}
//			 ledtime++;
//			 if(ledtime>=10)
//				{
//				 ledon=0;
//				 ledtime=1;
//				}
//			}
//				
//		//if(1)		//都是正极性
//		if(!(cof_yuretime&(1<<6)))		//正极性,BIAS 是1代表正极性
//		{
//			if(temp<=NH3offset)
//			{
//				NH3=0;
//				return ;
//			}
//			else
//				NH3=Volt1-NH3offset;
//		}
//		else				//反极性
//		{
//			if(temp>=NH3offset)
//			{
//				NH3=0;
//				return ;
//			}
//			else  
//				NH3=NH3offset-Volt1;	
//		}	
//		temp=NH3;
//		temp=temp*cof_NH3cheng/cof_NH3chu;
//		if(yurenowtianme<5000)		//预热时间恒定五秒钟
//			NH3=cof_yurevalue;
//		else	
//			NH3=temp;	

//		if(NH3>cof_DACmax)//设定上限
//		 NH3=cof_DACmax;
//	}
//	else
//	{
//		RegularConvData_Tab2[votick++]=temp;
//	}
}



u8 Alarm1=0,Alarm2=0;
void Alarm()
{
 if(NH3>Shangxian[0]||NH3<Xiaxian[0])//第一路
	 Alarm1=1;
	 
 if(Alarm1==1)
	{
	 if(NH3<(Shangxian[0]-HuiLuo[0])&&NH3>=(Xiaxian[0]+HuiLuo[0]))
			Alarm1=0;
	 else
			Alarm1=1;
	}		
 if(Alarm1==1)
	 	GPIOA->POD |= 1<<9;
 else 
  	GPIOA->POD &= ~(1<<9);

 if(cof_showstyle==2||cof_showstyle==3)	 
  {
	 if(NH3>Shangxian[1]||NH3<Xiaxian[1])//第二路
			Alarm2=1;			 
	 if(Alarm2==1)		 
		{
		 if(NH3<(Shangxian[1]-HuiLuo[1])&&NH3>=(Xiaxian[1]+HuiLuo[1]))
			 Alarm2=0;
		 else
			 Alarm2=1;
		}			
	 if(Alarm2==1)
		 		GPIOA->POD |= 1<<10;
	 else
		 		GPIOA->POD &= ~(1<<10);
  }
}







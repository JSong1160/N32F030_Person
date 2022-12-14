#include "include.h"


u8 TemSelect=1,cof_showstyle=0,cof_showstyle_last=0;
u8 hour,minutes,seconds;
s16 Tem,Hum,Tem_last,Hum_last,Tem_count=0,Hum_count=0;
u16 ModelBaud,CO2,tvoc,LuxMult;
u32 DQQY,Lux;
s16 LuxOff;
u32 biaozhiwei;
int16_t Tem_K[2],TemOff[2],HumOff[2],Hum_K[2];
u8 ModelID,LuxSelect;			//ModelID
u8 cof_jixing;
u16 cof_yuretime;
/*================================================================
                           IIC address
================================================================*/
#define SHT20ADDR      0x80
#define SHT31ADDR      0x88
/*================================================================
                 ACK  and  NACK  defination
================================================================*/
#define ACK            1
#define NACK           0

/*================================================================
                       HTU20D Command Code
================================================================*/
/*
  Command                         Code              Comment
  Trigger Temperature Measurement 0xe3              Hold master
  Trigger Humidity Measurement    0xe5              Hold master
  Trigger Temperature Measuremeng 0xf3              No Hold master
  Trigger Humidity Measurement    0xf5              No Hold master
  Write user register             0xe6
  Read user register              0xe7
  Soft Reset                      0xfe
*/
#define SOFT_RESET      0xfe
#define READ_REGISTER   0xe7
#define WRITE_REGISTER  0xe6
#define TRIGGER_TEMP    0xf3
#define TRIGGER_HUMI    0xf5

/*================================================================
                    HTU20 operate interface
================================================================*/
#define HTU20_SDA         ((GPIOA->PID)&0X00000020)              //GPIO_ReadInputDataBit(GPIOA,GPIO_PIN_5)
#define HTU20_SCL         ((GPIOA->PID)&0X00000040)              //GPIO_ReadInputDataBit(GPIOA,GPIO_PIN_6)


#define ADDR_24CXX       0xA0
#define SCLH        scl3_h()
#define SCLL        scl3_l()
   
#define SDAH        sda3_h()
#define SDAL        sda3_l()
#define SDA_O       sda_out()
#define SDA_I       sda_in()
#define SCLread     ((GPIOA->PID)&0X00000040)                     //GPIO_ReadInputDataBit(GPIOA,GPIO_PIN_6)
#define SDAread     ((GPIOA->PID)&0X00000020)                     //GPIO_ReadInputDataBit(GPIOA,GPIO_PIN_5)

#define  MS561101BA_SlaveAddress 0xee  //??????????IIC??????????????

#define  MS561101BA_D1 0x40 
#define  MS561101BA_D2 0x50 
#define  MS561101BA_RST 0x1E 

//#define  MS561101BA_D1_OSR_256 0x40 
//#define  MS561101BA_D1_OSR_512 0x42 
//#define  MS561101BA_D1_OSR_1024 0x44 
//#define  MS561101BA_D1_OSR_2048 0x46 
#define  MS561101BA_D1_OSR_4096 0x48 

//#define  MS561101BA_D2_OSR_256 0x50 
//#define  MS561101BA_D2_OSR_512 0x52 
//#define  MS561101BA_D2_OSR_1024 0x54 
//#define  MS561101BA_D2_OSR_2048 0x56 
#define  MS561101BA_D2_OSR_4096 0x58 

#define  MS561101BA_ADC_RD 0x00 
#define  MS561101BA_PROM_RD 0xA0 
#define  MS561101BA_PROM_CRC 0xAE 
u16 Cal_C[7];  //????????PROM????8??????

void scl3_h(void)
{	
    GPIOA->POD |=0X00000040;    //GPIO_SetBits(GPIOA,GPIO_PIN_6);	
}
void scl3_l(void)
{ 
    GPIOA->POD  &=0XFFFFFFBF;    //GPIO_ResetBits(GPIOA,GPIO_PIN_6);	
}

void sda_out(void)
{
	//PA5
	GPIOA->PMODE  &= 0XFFFFF3FF;
	GPIOA->PMODE  |= 0X00000400;   //????
	
//	GPIOA->POTYPE |= 1<<5;   //????
	GPIOA->POTYPE   &= ~(1<<5);   //????
  
	GPIOA->PUPD   &= 0XFFFFF3FF;    //???? ????
	GPIOA->PUPD   |= 0X00000400;    //????
	
//	  GPIOA->PUPD   &= 0XFFFFF3FF;   //???????? 
	
	GPIOA->SR     &= 0xFFFFFFBF;   //????????

}

void sda_in(void)
{
	//PA5
  GPIOA->PMODE  &= 0XFFFFF3FF;   //????????
	
//	GPIOA->POTYPE |= 1<<5;        //????
	
	GPIOA->POTYPE   &= ~(1<<5);   //????
	
	GPIOA->PUPD   &= 0XFFFFF3FF;    //???? ????
	GPIOA->PUPD   |= 0X00000400;    //????
//	  GPIOA->PUPD   &= 0XFFFFF3FF;   //???????? 
	
	GPIOA->SR     &= 0XFFFFFFDF;    //????????
}
void sda3_h(void)
{ 
  GPIOA->POD  |= 1<<5; 	
}
void sda3_l(void)
{
	//PA5
 GPIOA->POD &= ~(1<<5);
}
void I2C3_delay(void)
{ 
   int i=6; //???????????????? ??????????????5????????
   while(i--) 
   { 
  __nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();	   
   } 
}
u8 I2C3_Start(void)
{    
	   SDA_O;
     SCLH;
     delay_us(4);	
	   SDAH;
     delay_us(4);
     if(!SDAread)
			 return 0; //SDA??????????????????,????
     SDAL;
     delay_us(4);
     if(SDAread) 
			 return 0; //SDA????????????????????,????
     SCLL;
     delay_us(4);
     return 1;
}
void I2C3_Stop(void)
{
 SDA_O;
 SDAL;
 delay_us(4);
 SCLH;
 delay_us(4);
 SDAH;
 delay_us(4);
}

void I2C3_Ack(void)
{
 SDA_O;
 SDAL;
 delay_us(4);
 SCLH;
 delay_us(4);
 SCLL;
 delay_us(4);
 SDAH;
 delay_us(4);
}

void I2C3_NoAck(void)
{
 SDA_O; 	
 SDAH;
 delay_us(4);
 SCLH;
 delay_us(4);
 SCLL;
 delay_us(4);
}

u8 I2C3_WaitAck(void)  //??????:=1??ACK,=0??ACK
{
 SDA_I;
 SCLL;
 delay_us(4);
 SDAH;   
 delay_us(4);
 SCLH;
 delay_us(4);
 if(SDAread)
 {
     SCLL;
   return 0;
 }
 SCLL;
   return 1;
}

#define REPEAT_CHK_ACK_NUM		8

u8 I2C3_Check(void)
{
  u8 ack,i;
	SDA_I;	
	SCLH;
	for (i=0; i<REPEAT_CHK_ACK_NUM ; i++)
	 {
		if(SDAread==0)
	   {
		  ack = 0;
		  break;
		 }
		else
		 {
			ack = 1;
		 }
	  }	
   delay_us(5);
	 SCLL;
	 delay_us(5);
	 SDA_O;
	return ack;
}


void I2C3_SendByte(u8 SendByte) //????????????????//
{
    u8 i=8;
	  SDA_O;
    while(i--)
    {
       if(SendByte&0x80)
         SDAH;  
       else 
         SDAL;   
       SendByte<<=1;
			 
       delay_us(4);
			 SCLH;
       delay_us(4);
       SCLL;
       delay_us(4);
    }
}
u8 I2C3_ReceiveByte(void) //????????????????//
{ 
   u8 i=8;
   u8 ReceiveByte=0;
	 SDA_I;
   SDAH;    
    while(i--)
    {
			
      ReceiveByte<<=1;  
			delay_us(4);
      SCLH;    
      delay_us(4); 			
      if(SDAread)
      {
       ReceiveByte|=0x01;
      }
		  delay_us(4); 			
      SCLL;
    }
    return ReceiveByte;
}
/**
  * @brief  soft reset by transmit reset command
  * @retval None
  */
void I2cI2cI2cSoftReset(void)
{
  I2C3_Start();
  I2C3_SendByte(SHT20ADDR & 0xfe);  //I2C address + write
  I2C3_WaitAck();
  I2C3_SendByte(SOFT_RESET);        //soft reset
  I2C3_WaitAck(); 
  I2C3_Stop();
} 
/**
  * @brief  SET HTU20D resolution by write register
  * @retval None
  */
void SET_Resolution(void)
{
  I2C3_Start();      
  I2C3_SendByte(SHT20ADDR & 0xfe);
  if(I2C3_WaitAck() == ACK) //I2C address + write + ACK
  {
      I2C3_SendByte(WRITE_REGISTER);
    if(I2C3_WaitAck() == ACK)
    {
        I2C3_SendByte(0x83);
      if(I2C3_WaitAck() == ACK) ;       //??????????   11bit RH% ??????????12ms(typ.)
    }                                   //             11bit T?? ??????????9ms(typ.)
  }
  I2C3_Stop();
}

/**
  * @brief  read sht20's converted result
  * @param  TempOrHumiCMD: operation command for temperature or humidity
  * @retval None
  */
u16 ReadSht20(char TempOrHumiCMD)
{
  u16 temp;
  u8 MSB,LSB;
  u16 Humidity,Temperature;
  u8 outtime=0;
  SET_Resolution();  //??????????
  I2C3_Start();
  I2C3_SendByte(0x80) ;  //??????????????
  if(I2C3_WaitAck() != ACK)   
		   return 0;   //I2C address + write + ACK
  I2C3_SendByte(TempOrHumiCMD) ;  
  if(I2C3_WaitAck() != ACK)  
   		 return 0;           
	do
	{
		if(outtime++>200)     
			return 0xffff;
		I2C3_Start();
		I2C3_SendByte(0x81) ;
		delay_ms(6);          //????6ms
	} while(I2C3_WaitAck()== NACK); //I2C address + read + NACK

	MSB = I2C3_ReceiveByte(); 
  I2C3_Ack();
	LSB = I2C3_ReceiveByte(); 
  I2C3_Ack();
	I2C3_ReceiveByte();
  I2C3_NoAck();
    
	I2C3_Stop();                                //Checksum  + NACK      I2CStop();
  LSB &= 0xfc;                                //Data (LSB) ??????????????????????????????0
  temp = (MSB << 8) + LSB;

      if (TempOrHumiCMD == ((char)TRIGGER_HUMI))
      {
        /*-- calculate relative humidity [%RH] --*/
        //equation: RH% = -6 + 125 * SRH/2^16         
        Humidity = (temp * 1250) / 65536 ;
         // printf("H:%d\r\n",Humidity);
        return Humidity;
      }
      else
      {
        /*-- calculate temperature [??C] --*/
        //equation:T = -46.85 + 175.72 * ST/2^16
        Temperature = ((temp * 17572) / 65536 - 4685)/10;
		//  printf("T:%d\r\n",Temperature);
        return Temperature;
      }
}
/**
  * @brief  read sht20's converted result
  * @param  TempOrHumiCMD: operation command for temperature or humidity
  * @retval None
  */
//void ReadSht20Pro()          ////////////////////////////////////////////////////////////////////////////////////////////////sht20//////////////////////////////////////////////////////////////////
//{
//	Tem = ReadSht20(TRIGGER_TEMP);
//	
//	if(Tem==0xffff)
//	{		
//		SetLed(0);		//????led
//		return ;	
//	}
//	else
//	{		
//		SetLed(1);		
//	}
//	Hum=ReadSht20(TRIGGER_HUMI);
//	if(Hum==0xffff)
//	{
//		SetLed(0);		
//	}
//	else
//	{
//		SetLed(1);		
//	}
//	
//	Tem+= TemOff[0];
//	Hum+= HumOff[0];

//  if(Tem<-400)
//	{
//		Tem_count++;
//		if(Tem_count>=10)
//		{
//			Tem_count = 0 ; 
//			Tem =-400;
//		}
//		else
//		  Tem = Tem_last; 
//	}
//  if(Tem>800)
//	{
//		Tem_count++;
//		if(Tem_count>=10)
//		{
//			Tem_count = 0 ; 
//			Tem = 800;
//		}
//		else
//		  Tem = Tem_last; 
//	}
//	
//  if(Hum<0)
//	{
//		Hum_count++;
//		if(Hum_count>=10)
//		{
//			Hum_count = 0 ; 
//			Hum =0;
//		}
//		else
//		  Hum = Hum_last;
//  }
//	if(Hum>=1000)
//	{
//		Hum_count++;
//		if(Hum_count>=10)
//		{
//			Hum_count = 0 ; 
//			Hum =999;
//		}
//		else
//		  Hum = Hum_last;
//	}
//	Tem_last = Tem;
//	Hum_last = Hum;
//}
//????????????????????
u8 BUF[8];
void Single_Write_BH1750(u8 REG_Address)
{
	I2C3_Start();
	I2C3_SendByte(0x46);   //????????????+??????
	I2C3_WaitAck();   //I2C address + write + ACK
	I2C3_SendByte(REG_Address);    //??????????????????????????pdf22?? 
	I2C3_WaitAck();   //I2C address + write + ACK
	I2C3_Stop();                   //????????????
}
u8 hilux=0;
void Set20Wlux()
{
 	Single_Write_BH1750(0x41);   // ??????	 01000_xxx				??????3??			60%
	Single_Write_BH1750(0x7c);   // ??????	 011_XXXXX
	hilux=1;
}
void Set6WLux()
{
	Single_Write_BH1750(0x45);   // ??????			??????3??			182%
	Single_Write_BH1750(0x96);   // ??????
	hilux=0;
}

//*********************************************************//
//                                                         //
//????????BH1750????????                                   //
//                                                         //
//*********************************************************//
void Multiple_read_BH1750(void)
{  
   	u8 i;	
    I2C3_Start();                          //????????
    I2C3_SendByte(0x46+1);         //????????????+??????
	I2C3_WaitAck();	
	 for (i=0; i<3; i++)                      //?????? ??6??????????????????BUF
    {
        BUF[i] = I2C3_ReceiveByte();          //BUF[0]????0x32????????????
        if (i == 2)
        {
           I2C3_NoAck();				
        }
        else
        {						
           I2C3_Ack();					
       }
   }    
	I2C3_Stop();                          //????????
	delay_ms(5);                          
   
  Lux=BUF[0]*256+BUF[1];

  if(LuxSelect>1)				
	 {
	  if(hilux) 
		 Lux*=3;
	  if(Lux<30000&&hilux)		//????30000??????????
		 {
		  Set6WLux();
		 //Multiple_read_BH1750();
		 }
	  if(Lux>40000&&!hilux)		//????30000??????????
		 {
		  Set20Wlux();
		 // Multiple_read_BH1750();
		 }	
	 }
  else
	 Set6WLux();
	Lux=(Lux*LuxMult)/100;
  Lux+=LuxOff;

	if(LuxSelect==1)
	 {
		if(Lux>65535)Lux=65535; 
		if(Lux<0)
			    Lux=0; 
	 }
	else if(LuxSelect==2)
	 {
	  if(Lux>200000)Lux=200000; 
	  if(Lux<0)
			    Lux=0;
	 }
	else
		Lux=0;	
}


#define LMP_WRITE_ADDE    0x90              
#define LMP_READ_ADDE     0x91               
#define LMP_REG1_ADDE     0x00              
#define LMP_REG2_ADDE     0x01
#define LMP_REG3_ADDE     0x10             
#define LMP_REG4_ADDE     0x11                
#define LMP_REG5_ADDE     0x12               
void i2c_wirtebyte(u8 deviceaddr,u8 reg,u8 data)
{
	I2C3_Start();
	I2C3_SendByte(deviceaddr & 0xfe);
	if(I2C3_WaitAck() == ACK) //I2C address + write + ACK
	{
		I2C3_SendByte(reg);
		if(I2C3_WaitAck() == ACK)
		{
			I2C3_SendByte(data);
			if(I2C3_WaitAck() == ACK){}      //??????????   11bit RH% ??????????12ms(typ.)
		}                                         //             11bit T?? ??????????9ms(typ.)
	}
	I2C3_Stop();
}
//NH3OFFSET????????
//??????????LMP_REG3_ADDE????
//??????????LMP_REG4_ADDE????
void  LMP_Init()			//LMP????????0x90  ????????0x91
{
	i2c_wirtebyte(0x90,LMP_REG2_ADDE,0);		//????????
	i2c_wirtebyte(0x90,LMP_REG3_ADDE,cof_jixing);		//????1
	i2c_wirtebyte(0x90,LMP_REG4_ADDE,cof_yuretime);		//????2
	i2c_wirtebyte(0x90,LMP_REG5_ADDE,3);		//????3			3??????????????
	i2c_wirtebyte(0x90,LMP_REG2_ADDE,1);		//????????
}




 /*------------------------------------
 ------THÙNG RÁC THÔNG MINH------------
 compiler          : keil C u4 compiler
 Microcontrollers  : AT89c52
 programer         : Ngô Xuân Vinh
 date              : 25/05/2022
 ------------------------------------*/

#include <Servo_SG90.h>	// them thu vien cho VDK

/*--- Dinh nghia cac chan ket noi----*/	
#define PWM_Period 0xB7FE // khoi tao 
#define Stepper_Port P1
sbit Servo_Motor_Pin = P2^4;
sbit ledyellow1 = P2^0;
sbit ledred = P2^1;
sbit ledblue = P2^2;
sbit ledyellow2 = P2^3;
sbit sensor1 = P2^6;
sbit sensor2 = P2^7;
sbit nutan = P3^2;

unsigned int ON_Period, OFF_Period, DutyCycle;

 /*----- ham tre 1ms -------*/
void delay(unsigned int count)
{
	int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<112;j++);
}

void Timer_init()
{
	TMOD = 0x01;
	TH0 = (PWM_Period >> 8);
	TL0 = PWM_Period;
	TR0 = 1;
}

void Timer0_ISR() interrupt 1
{
	Servo_Motor_Pin = ! Servo_Motor_Pin;
	if(Servo_Motor_Pin)
	{
		TH0 = (ON_Period >> 8);
		TL0= ON_Period;
	}
	else
	{
		TH0 = (OFF_Period >> 8);
		TL0 = OFF_Period;
	}	
	
}

void Set_DutyCycle_To(float duty_cycle)
   {
	float period = 65535 - PWM_Period;
	ON_Period = ((period/100.0) * duty_cycle);
	OFF_Period = (period - ON_Period);
	ON_Period = 65535 - ON_Period;
	OFF_Period = 65535 - OFF_Period;
   } 

/*---ham dieu khien dong co buoc quay thuan---*/
void step_thuan()
{
  int i;		 
  for(i=0; i<6500; i++)	// So vong dong co
  {
   Stepper_Port = 0x09;	// 0000 1001
   delay(2);
   Stepper_Port = 0x03;	// 0000 0011
   delay(2);
   Stepper_Port = 0x06;	// 0000 0110
   delay(2);
   Stepper_Port = 0x0C;	// 0000 1100
   delay(2);
  }
}

/*---ham dieu khien dong co buoc quay nguoc---*/
 void step_nguoc()
{
  int j;		  
  for(j=0; j<5000; j++)
   {
   Stepper_Port = 0x09;
   delay(2);
   Stepper_Port = 0x0C;
   delay(2);
   Stepper_Port = 0x06;
   delay(2);
   Stepper_Port = 0x03;
   delay(2);
	}
 }
/* ----- chung trinh chinh -----------
chu y:					        ------
1) cac cam bien dang NPN        ------
   +) = 0 la dang nhan tin hieu ------
   +) = 1 la chua nhan tin hieu ------
2) led duoc noi chung Anot 	    ------
   +) = 0 led sang			    ------
   +) = 1 led tat               ----*/

int main()
{
	ET0 = 1;	  // cho phep ngat timer 0
	Timer_init();
	EA = 1;		  // cho phep ngat toan cuc

 do{
 if(nutan == 0)
 {delay(50);
 	if(nutan == 0)
 	{
		ledyellow1 = 0;
		ledyellow2 = 0;
		ledblue = 1;
		step_thuan();
		Set_DutyCycle_To(10.5);//2.1ms(10.5%) of 20ms(100%)(=151 do)  
		ledred = 0;
	 	delay(5000);
		step_nguoc();
		delay(2000);
		Set_DutyCycle_To(2.5);// 0.5ms(2.5%) of 20ms(100%)( =0 do) ;
		ledblue = 0;
		ledred = 1;
		ledyellow1 = 1;
		ledyellow2 = 1;
	} 
 }
if(sensor1 == 1)	
	{	
	ledblue = 0;
	ledred = 1;
	Set_DutyCycle_To(2.5);// 0.5ms(2.5%) of 20ms(100%)( =0 do) 
 	delay(500);

 	  if( sensor2 == 0)
		{
		ledblue = 1;
		ledyellow1 = 0;
		ledyellow2 = 0;
		step_thuan();
		delay(5000);
		step_nguoc();		
		} 	
	}

else
	{	
	ledblue = 1;
	ledred = 0;
	ledyellow1 = 1;
	ledyellow2 = 1;	
	Set_DutyCycle_To(10.5);//2.1ms(10.5%) of 20ms(100%)(=151 do)  
	delay(5000);
	} 
}
while(1);
}	

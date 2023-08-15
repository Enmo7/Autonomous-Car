/*
 * gradution_project.c
 *
 * Created: 12/15/2022 8:11:49 PM
 * Author : Lenovo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "BitMath.h"
#include "STD_Types.h"
#include "Clock.h"
#include "DIO.h"
#include "GINT.h"
#include "TIMR0.h"
#include "LCD.h"
#include "TIMR1.h"
#include "UltraSonic.h"
#include "servo.h"
#include "DC_MOTOR.h"


//global
uint64 Tick_Time = (1024.0/16000000);
uint64  totall_time = 1;
char buffer[30];
volatile uint64 distance = 1 ;



int main(void){
	
	Enable_GINT_AllInterrupts();
	DIO_Init();
	LCD_Init();
	ultrasonic_init();
	servo_init(64);
	LCD_String("Autonomous Car");
	LCD_SetPos(2,5);
	LCD_String("IS ON");
	_delay_ms(3000);
	LCD_Clear();
	servo_angle(0 , 64 , TIMR1_A);
	
	while (1)
	 {   
		  m_forward();
		  LCD_Clear();
		  LCD_String("moving forward..");
		  _delay_ms(500);
		  uint64 forward  = get_distance(DIO_ChannelB0);
		 if ( forward < 20)
		 {   
			 stop();
			 _delay_ms(1000);
			 servo_angle(90, 64 , TIMR1_A); //left
			 _delay_ms(1000);
			 uint64 left = get_distance(DIO_ChannelB0);
			 _delay_ms(1000);
			 itoa(left,buffer,10);
			 strcat(buffer, " cm ");
			 LCD_Clear();
			 LCD_String(buffer);
			 _delay_ms(1500);
			 //measure right 
			  servo_angle(-90, 64 , TIMR1_A); //right
			  _delay_ms(1000);
			  uint64 right = get_distance(DIO_ChannelB0);
			  _delay_ms(1000);
			  itoa(right,buffer,10);
			  strcat(buffer, " cm ");
			  LCD_Clear();
			  LCD_String(buffer);
			  _delay_ms(1500);
			  compare_distance(left , right);
}

}
}
		



ISR(INT1_vect){
	if (sensor_in_work == 1){
switch (interrupt)
{
   case 0:
     TIMR0_Start(1024 ,NONE);
     interrupt =1 ;
	break;
  case 1:
    TIMR0_Stop();
    totall_time = (((Timer_Overflow*256)+TIMR0_Get_Preload())*(Tick_Time*pow(10 , 6)));//totall time in us 
    distance = (34300 * (totall_time*pow(10 ,-6)))/2.0; //in cm
    sensor_in_work = 0 ;
	interrupt = 0 ;
	Timer_Overflow = 0 ;
    break;
  }
}


}

void __vector_11(void)__attribute__((signal));
void __vector_11(void){
	//call action
	Timer_Overflow++;
}
























































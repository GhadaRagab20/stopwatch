/*
 * APP.c
 *
 *  Created on: Feb 1, 2021
 *      Author: ghada
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Common_Macros.h"


#define MAX_SEVEN_SEG2_DISPLAY 6
#define MAX_SEVEN_SEG1_DISPLAY 10
#define NO_OF_SEVEN_SEG 6


unsigned char TIME[NO_OF_SEVEN_SEG]={0};
unsigned char Global_Timer1_Flag = 0;

ISR(TIMER1_COMPA_vect){
	Global_Timer1_Flag = 1;
}
/*RESET*/
ISR(INT0_vect)
{
	/* Set Timer Initial Value 0 */
	TCNT1 = 0;
	/* Set all 7-segs to Initial Value 0 */
	TIME[0]=0;
	TIME[1]=0;
	TIME[2]=0;
	TIME[3]=0;
	TIME[4]=0;
	TIME[5]=0;
	/*to return timer on if # reset is pressed  after pause */
	SET_BIT(TCCR1B,CS11);
	SET_BIT(TCCR1B,CS10);
}

/*PAUSE*/
ISR(INT1_vect)
{
	/* stop the timer */
	CLEAR_BIT(TCCR1B,CS11);
	CLEAR_BIT(TCCR1B,CS10);
}

/*RESUME*/
ISR(INT2_vect)
{   /* start the timer */
	SET_BIT(TCCR1B,CS11);
	SET_BIT(TCCR1B,CS10);

}

void init_INT0(void);
void init_INT1(void);
void init_INT2(void);
void init_Timer_1_CTC(void);
void init_seven_segmant(void);


int main(){
	unsigned char i;
	/* Enable interrupts by setting I-bit */
	SET_BIT(SREG,7);
	init_INT0();
	init_INT1();
	init_INT2();
	init_seven_segmant();
	init_Timer_1_CTC();

	while(1){
		for(i=0;i<6;i++){
			PORTA=(PORTA&0XC0)|(1<<i);
			PORTC = (PORTC & 0xF0) | (TIME[i] & 0x0F);
			_delay_ms(5);
			if(Global_Timer1_Flag){
				TIME[0]++;
				//SEC1
				if(TIME[0]== MAX_SEVEN_SEG1_DISPLAY){
					TIME[1]++;
					TIME[0]=0;

				}
				//SEC2
				if(TIME[1]== 6){
					TIME[1]=0;
					TIME[0]=0;
					TIME[2]++;
				}
				//MIN1
				if(TIME[2]== MAX_SEVEN_SEG1_DISPLAY){
					TIME[2]=0;
					TIME[3]++;
				}
				//MIN2
				if(TIME[3]== 6){
					TIME[3]=0;
					TIME[2]=0;
					TIME[4]++;
				}
				if(TIME[4]== MAX_SEVEN_SEG1_DISPLAY){
					TIME[4]=0;
					TIME[5]++;
				}
				if(TIME[5]== MAX_SEVEN_SEG1_DISPLAY){
					TIME[5]=0;
					//CLEAR_BIT(TCCR1B,CS11);
				}
				Global_Timer1_Flag =0;
			}
		}
	}
}

void init_INT0(void){
	/* Configure INT0/PD2 as input pin */
	CLEAR_BIT(DDRD,PD2);
	/* enable internal pull up resistor */
	SET_BIT(PORTD,PD2);
	/* Enable external interrupt pin INT0 */
	SET_BIT(GICR,INT0);
	/* Trigger INT0 with the falling edge*/
	/* bit ISC00 = 0 , ISC01 =1 in MCUCR */
	SET_BIT(MCUCR,ISC01);
	CLEAR_BIT(MCUCR,ISC00);

}
void init_INT1(void){
	/* Configure INT1/PD3 as input pin */
	CLEAR_BIT(DDRD,PD3);
	/* Enable external interrupt pin INT1 */
	SET_BIT(GICR,INT1);
	/* Trigger INT1 with the raising edge*/
	/* bit ISC00 = 1 , ISC01 =1 in MCUCR */
	SET_BIT(MCUCR,ISC11);
	SET_BIT(MCUCR,ISC10);

}
void init_INT2(void){
	/* Configure INT0/PD2 as input pin */
	CLEAR_BIT(DDRB,PB2);
	/* enable internal pull up resistor */
	SET_BIT(PORTB,PB2);
	/* Enable external interrupt pin INT2 */
	SET_BIT(GICR,INT2);
	/* Trigger INT2 with the falling edge*/
	/* bit ISC2 = 0 in MCUCSR */
	CLEAR_BIT(MCUCSR,ISC2);

}
void init_Timer_1_CTC(void){
	/* Set Timer Initial Value 0 */
	TCNT1 = 0;
	/* Set Compare Value to INT after 1 sec */
	OCR1A = 15624;
	/* Enable Timer1 Compare Interrupt */
	TIMSK |= (1<<OCIE1A);

	/* Configure timer1 control registers
	 * 1. Non PWM mode FOC1A=1 , FOC1B=1
	 * 2. CTC Mode WGM11=0 & WGM10=0 & WGM12=1 & WGM13=0
	 * 3. No need for OC0 in this example so COM00=0 & COM01=0
	 * 4. clock = F_CPU/64 CS10=1 CS11=1 CS12=0
	 */
	TCCR1A =  (1<<FOC1A)|(1<<FOC1B);
	TCCR1B =  (1<<CS11)|(1<<WGM12)|(1<<CS10);

}
void init_seven_segmant(void){
	DDRC  |= 0x0F;     // Configure the first four pins in PORTC as output pins.
	PORTC &= 0xF0;     // Initialize the 7-seg display zero at the beginning.
	DDRA  |= 0x3F;     // Configure the first six pins in PORTA as output pins(enable of 7-seg).
	PORTA &= 0xC0;     // Initialize the enable of 7-seg by zero at the beginning.

}

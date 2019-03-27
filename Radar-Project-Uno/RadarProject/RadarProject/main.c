/*
 * RadarProject.c
 *
 * Created: 27-3-2019 12:28:08
 * Author : Tom Martens
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "matrix.h"


int UltrSoneOn = 0;
int i = 0;
int pulse = 3000;
int distance = 50;
int sendBack = 1;
int safety = 0;

int main(void)
{
	twi_init();
	fill_board();
	
	 DDRD = 0b10000000;
	 DDRB = 0b00110000;
	 EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
	 EIMSK |= (1 << INT0);     // Turns on INT0
	 
	 sei();    
	 int i;
	for(i=0;i<12;i++){
		SetStep();
		_delay_ms(500);
	}
    /* Replace with your application code */
    while (1) 
    {
		PORTB |= 0b00010000;
		_delay_us(15);
		PORTB &= 0b11101111;
		distance = (pulse / 928); // Getting the distance in cm
		
		if(distance<=25 && distance >= 5){//&& distance <= 10){
			PORTB |= 0b00100000;
		}
		else{
			
			PORTB &= 0b11011111;
		}
	}
    
}

ISR(INT0_vect)
{
	if(UltrSoneOn == 1){
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		UltrSoneOn = 0;
	}
	else{
		TCCR1B |= 1<<CS10;
		UltrSoneOn = 1;
	}
}


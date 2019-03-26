/*
 * RadarProject.c
 *
 * Created: 13/03/2019 11:12:05
 * Author : Patrick
 */ 

#define F_CPU 20000000UL 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "matrix.h"

int UltrSoneOn = 0;
int i = 0;
int pulse = 3000;
int distance = 50;
int sendBack = 1;



int main(void)
{
	loadMatrix();
	DDRB = 0b00110000;
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
	EIMSK |= (1 << INT0);     // Turns on INT0

	sei();                    // turn on interrupts
	//PORTB = 0b00100000;
	while(1){
			PORTB |= 0b00010000;
			_delay_us(15);
			PORTB &= 0b11101111;
			distance = (pulse / 58)*2; // Getting the distance in cm
			if(distance<=12 ){//&& distance <= 10){
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
		pulse = TCNT1/58;
		TCNT1 = 0;
		UltrSoneOn = 0;
	}
	else{
		TCCR1B |= 1<<CS10;
		UltrSoneOn = 1;
	}
}

void setLED(int timeDistance){
	int time = timeDistance*1000000;
	int ix;
	PORTB |= 0b00100000;
	for(ix = 0; ix < 3; ix++){
		_delay_ms(1000);
	}
	PORTB &= 0b11011111;
	_delay_ms(1000);
}
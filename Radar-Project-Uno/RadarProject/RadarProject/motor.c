/*
 * motor.c
 *
 * Created: 27-3-2019 14:30:42
 *  Author: timde
 */ 
#define F_CPU 16000000UL
#define STEP_TRIGGER_VALUE 20


#include <avr/io.h>
#include <util/delay.h>

#include "motor.h"

int direction = 0;

int getDirection(){
	return direction;
}

void SetStep(){
	PORTD |= 0b10000000;
	_delay_us(STEP_TRIGGER_VALUE);
	PORTD &= 0b01111111;
}

void ChangeDirection(){
	if(direction == 0){
		direction = 1;
		PORTD |= 0b01000000;
	}
	else{
		direction = 0;
		PORTD &= 0b10111111;
	}
}

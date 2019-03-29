/*
 * motor.c
 *
 * Created: 27-3-2019 14:30:42
 *  Author: timde
 */ 

//	Defines
#define F_CPU 16000000UL
#define STEP_TRIGGER_VALUE 20

//	Includes
#include <avr/io.h>
#include <util/delay.h>

#include "motor.h"

//	Variables
int direction = 0;		//	Determines the direction of the motor 

//	Getter for the direction
int getDirection(){
	return direction;
}

//	Makes the motor set 1 step of 22,5 degrees in the selected direction
void SetStep(){
	PORTD |= 0b10000000;
	_delay_us(STEP_TRIGGER_VALUE);
	PORTD &= 0b01111111;
}

//	Changes the direction variable and port
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

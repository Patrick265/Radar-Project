/*
 * motor.c
 *
 * Created: 27-3-2019 14:30:42
 *  Author: timde
 */ 
#include <avr/io.h>
#include <util/delay.h>

void SetStep(){
	PORTD |= 0b10000000;
	_delay_us(20);
	PORTD &= 0b01111111;
}

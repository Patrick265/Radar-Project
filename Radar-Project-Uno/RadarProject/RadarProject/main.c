/*
* RadarProject.c
*
* Created: 27-3-2019 12:28:08
* Author : Tom Martens
*/

#define F_CPU 16000000UL
#define CM_CONVERT_VALUE 116
#define STEP_DELAY_VALUE 500
#define STEP_SIZE 3
#define MAX_TURN_VALUE 7
#define MIN_TURN_VALUE 0
#define MAX_DISTANCE 24
#define MIN_DISTANCE 3
#define MAX_COLUMNS 8

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "matrix.h"
#include "util.h"
#include "motor.h"


int UltrSoneOn = 0;
int i = 0;
int pulse = 3000;
int distance = 50;
int sendBack = 1;
int safety = 0;

int MotorIndex = 3;
int scanValue = 0;
int scanTimes = 0;


int main(void)
{
	twi_init();
	//fill_board();
	//fill_board();
	clear_board();
	DDRD = 0b11000000;
	DDRB = 0b00110000;
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	sei();
	
	/* Replace with your application code */
	
	//fill_column(4, 5);
	while (1)
	{
		//Checking if the direction of the motor needs to be changed (At 0 or 180 degrees)
		if(MotorIndex == MAX_TURN_VALUE || MotorIndex == MIN_TURN_VALUE ){
			ChangeDirection();
		}
		//Setting steps for 1/16th of 360 degrees
		for(i=0;i<STEP_SIZE;i++){
			SetStep();
			_delay_ms(STEP_DELAY_VALUE);
		}
		//Updating the motorindex
		if(getDirection() == 0){
			MotorIndex = MotorIndex + 1;
		}
		else{
			MotorIndex = MotorIndex - 1;
		}
		
		int scanAmount;
		for(scanAmount = 0; scanAmount < 100; scanAmount++){
			PORTB |= 0b00010000;
			_delay_us(15);
			PORTB &= 0b11101111;

			distance = (pulse / CM_CONVERT_VALUE); // Getting the distance in cm
			
			if(distance<=MAX_DISTANCE && distance >= MIN_DISTANCE){//&& distance <= 10){
				PORTB |= 0b00100000;
				scanValue = scanValue + distance;
				scanTimes += 1;
			}
			else{
				PORTB &= 0b11011111;
			}
		}

		if(scanTimes != 0){
			int averageDistance;
			averageDistance = scanValue/scanTimes;
			clear_column(MotorIndex);
			fill_column(MotorIndex, (int)averageDistance/3);
		}
		else{
			clear_column(MotorIndex);
		}
		scanTimes = 0;
		scanValue = 0;
		wait(1000);
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
		TCCR1B |= 1<<CS11;
		UltrSoneOn = 1;
	}
}


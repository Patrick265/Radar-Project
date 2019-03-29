/*
* RadarProject.c
*
* Created: 27-3-2019 12:28:08
* Author : Tom Martens
*/

//	Defines
#define F_CPU 16000000UL
#define CM_CONVERT_VALUE 116
#define STEP_DELAY_VALUE 500
#define STEP_SIZE 3
#define MAX_TURN_VALUE 7
#define MIN_TURN_VALUE 0
#define MAX_DISTANCE 24
#define MIN_DISTANCE 3
#define MAX_COLUMNS 8

//	Includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "matrix.h"
#include "util.h"
#include "motor.h"

//	Variables
int UltrSoneOn = 0;		//	Determines if the ultrasone is starting or ending the timer for measuring the echo length
int pulse = 0;			//	The raw pulse value returned by the ultrasone
int distance = 0;		//	The distance measured by the ultrasone
int MotorIndex = 3;		//	Determines were in the circle the motor is
int scanValue = 0;		//	Cumulative distance of every correct measurement
int scanTimes = 0;		//	Amount of correct measurements


int main(void)
{
	//	Initializing the dot matrix
	twi_init();
	clear_board();
	//	Setting the input and output
	DDRD = 0b11000000;
	DDRB = 0b00110000;
	
	//	set INT0 to trigger on ANY logic change
	EICRA |= (1 << ISC00); 
	//	Turns on INT0   
	EIMSK |= (1 << INT0);    
	//	Enable interupts
	sei();
	
	while (1)
	{
		//	Checking if the direction of the motor needs to be changed
		if(MotorIndex == MAX_TURN_VALUE || MotorIndex == MIN_TURN_VALUE ){
			ChangeDirection();
		}
		
		//	Setting steps for 1/16th of 360 degrees (22.5 degrees)
		int i;
		for(i=0;i<STEP_SIZE;i++){
			SetStep();
			_delay_ms(STEP_DELAY_VALUE);
		}
		
		//	Updating the motorindex
		if(getDirection() == 0){
			MotorIndex = MotorIndex + 1;
		}
		else{
			MotorIndex = MotorIndex - 1;
		}
		
		//	Scanning with the ultrasone
		int scanAmount;
		for(scanAmount = 0; scanAmount < 100; scanAmount++){
			//	Sending trigger
			PORTB |= 0b00010000;
			_delay_us(15);
			PORTB &= 0b11101111;
			//	Getting the distance in cm
			distance = (pulse / CM_CONVERT_VALUE); 
			
			//	Checking if the distance is in selected range
			if(distance<=MAX_DISTANCE && distance >= MIN_DISTANCE){//&& distance <= 10){
				PORTB |= 0b00100000;
				//	Adding the new distance to the cumulative total
				scanValue = scanValue + distance;
				scanTimes += 1;
			}
			else{
				PORTB &= 0b11011111;
			}
		}

		//	Checking if there were valid measurements
		if(scanTimes != 0){
			//	Getting the average distance by dividing the cumulative amount by the amount of scans
			int averageDistance;
			averageDistance = scanValue/scanTimes;
			//	Setting the right column of the dot matrix
			clear_column(MotorIndex);
			fill_column(MotorIndex, (int)averageDistance/3);
		}
		else{
			clear_column(MotorIndex);
		}
		//	Resetting values
		scanTimes = 0;
		scanValue = 0;
		wait(1000);
	}
	
}

ISR(INT0_vect)
{
	//	Stopping the timer and storing the pulse
	if(UltrSoneOn == 1){
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		UltrSoneOn = 0;
	}
	//	Starting the timer
	else{
		TCCR1B |= 1<<CS11;
		UltrSoneOn = 1;
	}
}


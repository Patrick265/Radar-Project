/*
 * RadarProject.c
 *
 * Created: 13/03/2019 11:12:05
 * Author : Patrick
 */ 

#define F_CPU 20000000UL 
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		PORTC = 0xFF; //Turns ON All LEDs
		_delay_ms(1000); //1 second delay
		PORTC= 0x00; //Turns OFF All LEDs
		_delay_ms(1000); //1 second delay
    }
}


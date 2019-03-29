/*
 * util.c
 *
 * Created: 13/03/2019 11:38:26
 *  Author: Patrick
 */ 

//	Defines
#define F_CPU 20000000UL
 
 
//	Includes
#include <util/delay.h>
#include "util.h"

// ms : Determines how many milliseconds the wait will last
void wait (int ms)
{
	for (int i  = 0; i<ms; i++){
		_delay_ms(1);
	}
}
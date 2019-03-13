/*
 * util.c
 *
 * Created: 13/03/2019 11:38:26
 *  Author: Patrick
 */ 

#define F_CPU 20000000UL
 
#include <util/delay.h>
#include "Dependency/util.h"

void wait (int ms)
{
	for (int i  = 0; i<ms; i++){
		_delay_ms(1);
	}
}
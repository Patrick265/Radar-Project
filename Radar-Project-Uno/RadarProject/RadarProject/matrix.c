#include <avr/io.h>
#include <util/delay.h>
#include "util.h"

int columns[] = {
	0b0000000, // 0
	0b00000010, // 2
	0b00000100,  // 4
	0b00000110, // 6
	0b00001000, // 8
	0b00001010, // 10
	0b00001100, // 12
0b00001110}; // 14

int rows[] = {
	0b10000000, // 1
	0b10000001, // 2
	0b10000011,  // 3
	0b10000111, // 4
	0b10001111, // 5
	0b10011111, // 6
	0b10111111, // 7
0b11111111}; // 8



void twi_start(void)
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}


void twi_stop(void)
{
	TWCR = (0x80 | 0x10 | 0x04);
}


void twi_tx(unsigned char data)
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while( 0 == (TWCR & 0x80) );
}

void clear_column(int column)
{
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(column);	// Address
	twi_tx(0b00000000);	// data
	twi_stop();
}

void fill_column(int column , int amount) {
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(column);	// Address
	twi_tx(amount);	// data
	twi_stop();
	
}

void clear_board() {
	int i;
	for(i =0; i < 8; i++) {
		clear_column(columns[i]);
		wait(100);
	}
}

void fill_board() {
	int i;
	for ( i = 0; i < 8; i++) {
		fill_column(columns[i], rows[7]);
		wait(200);
	}
}

void twi_init(void)
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
	//Init HT16K22. Page 32 datasheet
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xA0);	// HT16K33 pins all output
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0x81);	// Display OFF - Blink On
	twi_stop();
	clear_board();
}
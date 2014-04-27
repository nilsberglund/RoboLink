/*
 * Slave_Control.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "Slave_control.h"

void SPI_Init_Slave()
{
	waiting_for_instruction = 1;
	DDRB = 0x48;	
	SPCR = 0xC3;		
	sei(); 
}
	
void Slave_TX(uint8_t data)
{
	SPDR = data;
	PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
	PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
}
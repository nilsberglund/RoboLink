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

void SPIInitSlave()
{
	waitingForInstruction = 1;
	DDRB = 0x48;	
	SPCR = 0xC3;		
	sei(); 
}
	
void slaveTX(uint8_t data)
{
	SPDR = data;
	PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
	PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
}

void TXFinishedDrop()
{
	slaveTX(1);
}
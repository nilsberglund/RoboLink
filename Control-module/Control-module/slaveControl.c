/*
 * slaveControl.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: Albin Ålander
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "slaveControl.h"

/*Initializes the AVR as a slave*/
void SPIInitSlave()
{
	waitingForInstruction = 1;
	DDRB = 0x48;	
	SPCR = 0xC3;		
	sei(); 
}

/*Transmits a byte via SPI to Master*/
void slaveTX(uint8_t data)
{
	SPDR = data;
	PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
	PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
}

/*Tells communication AVR that drop of is finished*/
void TXFinishedDrop()
{
	slaveTX(1);
}
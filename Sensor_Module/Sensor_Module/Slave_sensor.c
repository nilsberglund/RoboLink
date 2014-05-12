/*
 * Slave.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Slave_sensor.h"
#include "SensorCalMajor.h"


/* Interrupt that gets triggered when slave has received instruction byte */
ISR(SPI_STC_vect)
{
	instructionData = SPDR;
	if(instructionData == 0b00000100)
	{
		slaveTX(sensorData); 
	} else if(instructionData == 0b10011111)
	{
		calibration();
	}
}

/* Function that initiates the sensor-AVR as a SPI slave*/
void SPIInitSlave()
	{

			DDRB = 0x48; // Sets the data direction for the SPI ports. MISO output, Interrupt output. All others input.
			
			SPCR = 0xC3; // Sets the SPI Control register. C => 1 << SPIE, 1 << SPE, 
			
			sei(); 
	}
	
/* Function for transmitting data via SPI */	
void slaveTX(uint8_t data)
	{
		SPDR = data;
		PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
		PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
	}
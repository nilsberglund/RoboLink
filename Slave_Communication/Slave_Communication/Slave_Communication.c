/*
 * Slave.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t received_data;

void SPI_Init_Slave();
void Slave_TX(uint8_t);

ISR(SPI_STC_vect)
{
	received_data = SPDR;
	PORTD = SPDR;
}


int main(void)
{
	SPI_Init_Slave();
    while(1)
    {
		Slave_TX(0xAA);
    }

}

void SPI_Init_Slave()
	{
			DDRD = 0xFF;
			DDRB = 0x48;
			
			SPCR = 0xC0;
			
			sei(); 
	}
	
void Slave_TX(uint8_t data)
	{
		SPDR = data;
		PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
		PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
	}
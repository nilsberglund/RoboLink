/*
 * Slave.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void SPI_Slave_Init();
uint8_t received_data;
void Slave_TX(uint8_t);

ISR(SPI_STC_vect)
{
	received_data = SPDR;
	PORTD = received_data;
}

int main(void)
{
	SPI_Slave_Init();	
    while(1)
    {
	Slave_TX(0xAA);
    }
}

void SPI_Slave_Init()
{
	//Outputs: PORTB6 = MISO, PORTB3 = Int. 
	DDRB = 0x48;
	
	DDRD = 0xFF;
	
	//SPIE = 1, SPE = 1
	SPCR = 0xC0;
	sei();
}

void  Slave_TX(uint8_t data)
{
	SPDR = data;
	PORTB |= (1 << PORTB3);
	PORTB &= ~(1 << PORTB3);
}




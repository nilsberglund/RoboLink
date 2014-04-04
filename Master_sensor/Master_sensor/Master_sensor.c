/*
 * Master.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void SPI_Init_Master();
void Master_TX(uint8_t);

uint8_t received_data;

ISR(INT2_vect)
{
	PORTB &= ~(1 << PORTB4);
	Master_TX(0x01);
	
	if(received_data == 0xAA)
	{
		PORTD |= (1 << PORTD6);
	}
	
}

int main(void)
{
	SPI_Init_Master();
	PORTB |= (1 << PORTB4);
    while(1)
    {
	//Master_TX(0xAA);
    }
}

void SPI_Init_Master()
{
	
	// OUTPUTS: PORTB7 = SCK, PORTB5 = MOSI, PORTB4 = SS. INPUTS: PORTB6 = MISO, PORTB2 = INT2.
	DDRB = 0xB0;
	
	DDRD |= (1 << DDD6);
	
	// SPIE = 1, SPE = 1, MSTR = 1.
	SPCR |= (1 << SPIE)|(1 << SPE)|(1 << MSTR);
	
	//The rising edge of INT2 generates asynchronously an interrupt request.
	EICRA = 0x30;
	
	//Enables INT2
	EIMSK = 0x04;
	
	//enable global interrupts
	sei();	

}

void Master_TX(uint8_t data)
{
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));	
	
	received_data = SPDR;
	
}

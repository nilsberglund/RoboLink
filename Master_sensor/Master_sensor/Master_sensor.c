/*
 * Master.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
 
uint8_t received_data;
 
/* Functions */
void SPI_Init_Master();
void Master_TX(uint8_t);

ISR(INT2_vect)
{
	PORTB &= ~(1 << PORTB4);
	
	Master_TX(0x01);
	
	if(received_data == 0xAA)
	{
		PORTD = (1 << PORTD6);
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

/* Initializes sensor AVR as master. Sets ports and registers and enables interrupts */
void SPI_Init_Master()
{

	DDRD = 0xFF;
	DDRB = 0xB0;
	
	SPCR |= (1 << SPIE)|(1 << SPE)|(1 << MSTR);
	
	EICRA = 0x30;
	EIMSK = 0x04;
	
	sei();
			
}


void Master_TX(uint8_t data)
{
		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
		
		received_data = SPDR;
}





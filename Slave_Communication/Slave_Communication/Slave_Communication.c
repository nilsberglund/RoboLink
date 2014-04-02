/*
 * Slave.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SS PORTB4
#define MOSI PORTB5
#define MISO PORTB6
#define SCK PORTB7
#define INT_Req PORTB3

uint8_t received_data;

void SPI_Init_Slave();
void Slave_TX();
void Slave_RX();

int main(void)
{
    while(1)
    {
		
    }
	
	void SPI_Init_Slave()
	{
		/* PORTB5 MOSI, PORTB6 MISO, PORTB7 SCK, PORTB4 SS, Sets I/O ports */
		//DDRB |= (0 << PORTB5)|(1 << PORTB6)|(0 << PORTB7)|(0 << PORTB4); 
		DDRB |= (0<<MOSI)|(1<<MISO)|(0<<SCK)|(0<<SS);
		
		DDRB |= (1<<INT_Req); // Sets PortB3 as output. PORTB3 is used to initiate interrupt.
		
		/*Sets the SPI Control Register. SPE = SPI Enable. MSTR = Master Select. Master if 1 << MSTR. SPIE = SPI Interrupt Enable. CPOL = Clock Polarity. If 1 << CPOL SCK is High when idle. CPHA = Clock Phase. 
		0 << CPHA => Leading edge sample , trailing edge Setup. SPR0 = SPI clock rate select. 1 << SPR0, 0 << SPR1 fck/16.*/
		SPCR = (1<<SPE)|(0<<MSTR)|(1<<SPR0)|(1<<SPR1)|(1<<SPIE)|(1 << CPOL) | (0 << CPHA);
	}
	
	void Slave_RX()
	{
			/* Wait for reception complete */
			while(!(SPSR & (1<<SPIF)))
			;
			/* Return Data Register */
			received_data = SPDR;
	}
	
	void Slave_TX(uint8_t data)
	{
		SPDR = data;
		PORTB = (1 << INT_Req); // Sets INT_Req high. I.e throws interrupt.
		_delay_ms(10);
		PORTB = (0 << INT_Req); // Sets INT_Req low. Interrupt finished.
		
		/*Wait for transmission complete*/
		Slave_RX();
	}
}
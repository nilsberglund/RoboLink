/*
 * Slave.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t instruction_data;
uint8_t sensor_data;

void SPI_Init_Slave();
void Slave_TX(uint8_t);

ISR(SPI_STC_vect)
{
	instruction_data = SPDR;
	if(instruction_data == 0b00000100)
	{
		Slave_TX(sensor_data);
	}
}


int main(void)
{
	SPI_Init_Slave();
    while(1)
    {
		
    }
}

void SPI_Init_Slave()
	{
			DDRD = 0xFF;
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
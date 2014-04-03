/*
 * Master.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Ports */
#define SS_COM PORTB3
#define SS_CON PORTB4
#define MOSI PORTB5
#define MISO PORTB6
#define SCK PORTB7

/*Variables*/
int Slave_COM	= 1;
int Slave_CON = 2;
int selected_slave; 
int received_data;
 
/* Functions */
void SPI_Init_Master();
void Slave_Select(int);
void Master_TX(uint8_t);
void Master_RX(uint8_t);

int main(void)
{
	SPI_Init_Master();
	
	Slave_Select(Slave_CON);
    while(1)
    {
			Master_TX(0xAB);
			_delay_us(100);
    }
}

/* Initializes sensor AVR as master. Sets ports and registers and enables interrupts */
void SPI_Init_Master()
{
			
		/* PORTB5 MOSI, PORTB6 MISO, PORTB7 SCK, PORTB4 SS_CON, PORTB3 SS_COM. Sets I/O ports */
		//DDRB |= (1 << PORTB5)|(0 << PORTB6)|(1 << PORTB7)|(1 << PORTB4)|(1 << PORTB3); 
		DDRB |= (1<<MOSI)|(0 << MISO)|(1<<SCK)|(1<<SS_COM)|(1<<SS_CON);
		
		/*Sets the SPI Control Register. SPE = SPI Enable. MSTR = Master Select. Master if 1 << MSTR. SPIE = SPI Interrupt Enable. CPOL = Clock Polarity. If 1 << CPOL SCK is High when idle. CPHA = Clock Phase. 
		0 << CPHA => Leading edge sample , trailing edge Setup. SPR0 = SPI clock rate select. 1 << SPR0, 0 << SPR1 fck/16.*/
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1)|(1<<SPIE)|(1 << CPOL) | (0 << CPHA);
		
		EICRA = 0x3C; //Interrupt control register. Rising edge-triggered interrupts. Activates INT1/2
		EIMSK = 6; //Interrupt mask register
		
		sei(); 
}

void Master_TX(uint8_t data)
{
		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
}

/* Sets slave */
void Slave_Select(int slave)
{
	selected_slave = slave;
	if(selected_slave == Slave_COM)
	{
		PORTB |= (0 << PORTB3)|(1 << PORTB4);
	}
	else
	{
		PORTB |= (1 << PORTB3)|(0 << PORTB4);
	}
	
}

/*Interrupt from communication slave. INT1.*/
ISR(INT1_vect)
{
	Slave_Select(Slave_COM);     //Should be Slave_COM as parameter
	
	while(!(SPSR & (1<<SPIF)));
	
	received_data = SPDR;
	
	if((received_data & 0x80) == 1)
	{
		Slave_Select(Slave_CON);
		Master_TX(received_data);	
	}
}

/*Interrupt from control slave. INT2.*/
ISR(INT2_vect)
{
	Slave_Select(Slave_CON);	   //Should be Slave_CON as parameter
	
	while(!(SPSR & (1<<SPIF)));
	
	received_data = SPDR;
	
	if((received_data & 0x80) == 1)
	{
		Slave_Select(Slave_COM);
		Master_TX(received_data);
	}
}




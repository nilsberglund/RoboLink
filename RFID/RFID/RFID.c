/*
 * RFID.c
 *
 * Created: 3/29/2014 11:53:59 AM
 *  Author: nilbe317
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define lcdE PORTD7;
#define lcdRS PORTD5;
#define lcdRW PORTD6;
#define lcdDataPort PORTA;

int main(void)
{
	char ReceivedByte; //the byte received by the AVR
	char rfidTag[12];
	uint8_t rfnr = 0;
	
	DDRA = 0xFF;
	DDRD |= (1 << DDD5)|(1 << DDD6)|(1 << DDD7);
	
	UCSR0B = (1<<RXEN0 | 1<<TXEN0); //Enable RX0 and TX0
	UCSR0C = (1 << UCSZ01 | 1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = (383 >> 8);
	UBRR0L = 383; //Sets baudvalue in AVR to 383, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1
	UCSR0B |= (1 << RXCIE0)
	sei();
	
	
	
	while(1)
	{
		
	}
}

ISR(USART0_RX_vect){

	rfidTag[rfnr] = UDR0;
	rfnr++;
	if (rfnr == 12) {
		rfnr = 0;
	}
}

	
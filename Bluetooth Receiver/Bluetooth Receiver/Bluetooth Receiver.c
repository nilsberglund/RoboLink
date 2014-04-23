/*
 * Bluetooth_Receiver.c
 *
 * Created: 15/04/2014 12:49:29
 *  Author: Tobias
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
char ReceivedByte; //the byte received by the AVR

ISR(USART0_RX_vect)
{
	ReceivedByte = UDR0;
}

int main(void)
{
	
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); //Enable RX0 and TX0
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0;
	UBRR0L = 0x07; //Sets baudvalue in AVR to 7, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1
	UCSR0B |= (1 << RXCIE0) //Enable rx complete interrupt
	sei();
	
	while(1)
	{
		//while (( UCSR0A & (1 << RXC0 )) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
		//ReceivedByte = UDR0 ; // Fetch the received byte value into the variable "ByteReceived"
		//while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
		//UDR0 = ReceivedByte ; // Echo back the received byte back to the computer
	}
}
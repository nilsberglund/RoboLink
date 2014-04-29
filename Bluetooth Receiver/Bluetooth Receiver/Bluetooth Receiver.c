/*
 * Bluetooth_Receiver.c
 *
 * Created: 15/04/2014 12:49:29
 *  Author: Tobias
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
char instrByte; //the byte received by the AVR, transmitted from computer


ISR(USART0_RX_vect)
{
	instrByte = UDR0;
	transmitData(instrByte);
}

void transmitData(char instrByte)
{
	cli();	//Disable global interrupts
	while ( !(UCSR0A & (1<<RXC0)) ) //wait for byte to be received
	switch (instrByte) {
		case 0x01: //DRIVE instruction value
			// TODO: send DRIVE data byte over SPI to control AVR
		case 0x02: //ARM instruction value
			// TODO: send ARM data byte over SPI to control AVR
		case 0x03: //CAL instruction value
			// TODO: send CAL data byte over SPI to sensor AVR
	}
	sei(); //Enable global interrupts again
}

void setupBluetoothRXTX()
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0); //Enable RX0, TX0 and RX complete interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0x00;
	UBRR0L = 0x07; //Sets baudvalue in AVR to 7, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1	
}

int main(void)
{
	sei();
	
	while(1)
	{
		//while (( UCSR0A & (1 << RXC0 )) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR
		//ReceivedByte = UDR0 ; // Fetch the received byte value into the variable "ByteReceived"
		//while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
		//UDR0 = ReceivedByte ; // Echo back the received byte back to the computer
	}
}
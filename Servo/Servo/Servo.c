/*
 * Servo.c
 *
 * Created: 3/30/2014 12:16:02 PM
 *  Author: susba199
 */ 


#include <avr/io.h>
#include "servo.h"; 

#define F_CPU 16000000
//baud_rate = 1000000


void USART_Init(void) {
	DDRD = (1<<DDD2);
	/*Set baud rate.*/
	UBRR0H = (unsigned char)(0);  //baudvalue = (f_cpu/baudrate*16) -1
	UBRR0L = (unsigned char)(0); 
	/* Enable reciever and transmitter.*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	/*Set frame format. Asynchronous mode, no parity, 1 stopbit, char size 8.  */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); 
	
}

void USART_Transmit( unsigned char data) {
	/*Wait for empty transmit buffer*/
	//PORTD = 0x02; 
	PORTD2 = 1; 
	while( !(UCSR0A & (1<<UDRE0)) ) // UDREn == Data Register Empty (indicates whether the transmit buffer is ready to recieve new data. 
	; 
	/*Put data into buffer, sends the data*/
	UDR0 = data; //UDREn cleared. 
}

unsigned char USART_Recieve(void){
	PORTD2 = 0; 
	return data; 
}


int main(void)
{
	 		
	USART_Init(); 
    while(1)
    {

		//USCR = 0x
		/*USART_Transmit(0xFF);
		USART_Transmit(0xFF);
		USART_Transmit(0xFE);
		USART_Transmit(0x18);
		USART_Transmit(0x83);
		USART_Transmit(0x1E);
		USART_Transmit(0x04);
		USART_Transmit(0x00);
		USART_Transmit(0x10);
		USART_Transmit(0x00);
		USART_Transmit(0x50);
		USART_Transmit(0x01);
		USART_Transmit(0x01);
		USART_Transmit(0x20);
		USART_Transmit(0x02);
		USART_Transmit(0x60);
		USART_Transmit(0x03);
		USART_Transmit(0x02);
		USART_Transmit(0x30);
		USART_Transmit(0x00);
		USART_Transmit(0x70);
		USART_Transmit(0x01);
		USART_Transmit(0x03);
		USART_Transmit(0x20);
		USART_Transmit(0x02);
		USART_Transmit(0x80);
		USART_Transmit(0x03);
		USART_Transmit(0x12);*/
		
		USART_Transmit(0xFF);
		USART_Transmit(0xFF); 
		USART_Transmit(0x01); 
		
		
		
        //TODO:: Please write your application code. Enable USART0 Transmitted => TXD0 is output no matter the value of DDD1. 
		 
		// USART Transmitter enable (TXEN) bit in UCSRnB. 
    }
}
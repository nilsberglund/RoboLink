/*
 * Servo.c
 *
 * Created: 3/30/2014 12:16:02 PM
 *  Author: susba199
 */ 


//TODO: Function calculating the checksum. Function converting degrees to hex. 300 degrees is 0x3FF
#define F_CPU 16000000UL

#include <avr/io.h>
#include "servo.h"
#include <util/delay.h>


//baud_rate = 1000000 for the robot arm

void USART_Init(void) {
	DDRD = (1<<DDD2); //Setting D2 to output to control the tri-state
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
	PORTD |= (1 << PORTD2);
	while (!(UCSR0A & (1<<UDRE0))); // UDREn == Data Register Empty (indicates whether the transmit buffer is ready to recieve new data. 
	/*Put data into buffer, sends the data*/
	UDR0 = data; //UDREn cleared. 
}

unsigned char USART_Recieve(void){
	
	char data;
	PORTD &= ~(1 << PORTD2);
	data = UDR0;
	return data; 
}

void move_Single_Servo(uint8_t pos_l, uint8_t pos_h, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID){
	
	byte checksum;
	
	USART_Transmit(0xFF);	//start byte
	USART_Transmit(0xFF);	//start byte
	USART_Transmit(servo_ID);	//servo ID
	USART_Transmit(0x07);	//length = 7
	USART_Transmit(WRITE);	//instruction = write_data
	USART_Transmit(GOAL_POSITION_L);	//address = goal position(L)
	USART_Transmit(pos_l);	//send low byte of position
	USART_Transmit(pos_h);	//send high byte of position
	USART_Transmit(speed_l); //send low byte of speed
	USART_Transmit(speed_h); //send high byte of speed
	
	checksum = servo_ID + 0x07 + WRITE + GOAL_POSITION_L + pos_l + pos_h + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USART_Transmit(checksum);
	
}

void move_Double_Servo(uint8_t pos_l, uint8_t pos_h, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID1, unsigned int servo_ID2){
	
	byte checksum;
	
	//uint8_t s1_deg = ...
	
	//uint8_t s2_deg = 300 - ...
	
	//s1_low_byte = ...
	//s1_high_byte = ...
	
	//s2_low_byte = ...
	//s2_high_byte = ...
	
	USART_Transmit(0xFF);	//start byte
	USART_Transmit(0xFF);	//start byte
	USART_Transmit(0xFE);	//broadcast ID
	USART_Transmit(0x0E);	//length is 14
	USART_Transmit(SYNC_WRITE);			//instruction = sync_data (2 servo)
	USART_Transmit(GOAL_POSITION_L);	//
	USART_Transmit(0x04);	//data length is 4
	USART_Transmit(servo_ID1);	//servo id1
	//USART_Transmit(pos_l);	//send low byte of position
	//USART_Transmit(pos_h);	//send high byte of position
	
	USART_Transmit(0xD2);
	USART_Transmit(0x00);
	
	USART_Transmit(speed_l); //send low byte of speed
	USART_Transmit(speed_h); //send high byte of speed
	USART_Transmit(servo_ID2); //servo id2
	//USART_Transmit(pos_l);	//send low byte of position
	//USART_Transmit(pos_h);	//send high byte of position
	
	USART_Transmit(0x2A);
	USART_Transmit(0x03);
	
	
	USART_Transmit(speed_l); //send low byte of speed
	USART_Transmit(speed_h); //send high byte of speed
	
	//checksum = 0xFE + 0x0E + SYNC_WRITE + GOAL_POSITION_L + 0x04 + servo_ID1 + pos_l + pos_h + speed_l + speed_h + servo_ID2 + pos_l + pos_h + speed_l + speed_h;
	
	checksum = 0xFE + 0x0E + SYNC_WRITE + GOAL_POSITION_L + 0x04 + servo_ID1 + 0xD2 + 0x00 + speed_l + speed_h + servo_ID2 + 0x2A + 0x03 + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USART_Transmit(checksum);
}

void default_Position(){
	
	move_Single_Servo(0xFF, 0x01, 0x50, 0x01, 0x07);
	move_Single_Servo(0xAA, 0x03, 0x50, 0x01, 0x06);
	move_Double_Servo(0xFF, 0x01, 0x50, 0x00, 0x02, 0x03);
	_delay_ms(500);
	move_Double_Servo(0x00, 0x00, 0x50, 0x00, 0x04, 0x05);
	move_Double_Servo(0xAA, 0x03, 0x50, 0x00, 0x02, 0x03);
	move_Single_Servo(0xFF, 0x01, 0x20, 0x00, 0x01);
	
	
}

int main(void)
{
	 		
	USART_Init();
	
	//default_Position();
	
	move_Double_Servo(0xFF, 0x01, 0x40, 0x00, 0x02, 0x03);
	
    while(1)
    {
		

    }
}
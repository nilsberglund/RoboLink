/*
* Servo.c
*
* Created: 3/30/2014 12:16:02 PM
* Author: susba199
* baud rate is 1000000 for the robot arm
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "servo.h"
#include <util/delay.h>

uint16_t joint1_Pos = 0x1FF; //servo 1
uint16_t joint2_Pos = 0x1FF; //servo 2 & 3
uint16_t joint3_Pos = 0x1FF; //servo 4 & 5
uint16_t joint4_Pos = 0x1FF; //servo 6
uint16_t joint5_Pos = 0x1FF; //servo 7
uint16_t joint6_Pos = 0x1FF; //servo 8

void USART_Init(void) {
	DDRD = (1<<DDD2); //Setting D2 to output to control the tri-state
	/*Set baud rate.*/
	UBRR0H = (unsigned char)(0);  //baudvalue = (f_cpu/baudrate*16) -1
	UBRR0L = (unsigned char)(0);
	/* Enable receiver and transmitter.*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/*Set frame format. Asynchronous mode, no parity, 1 stop bit, char size 8.  */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
}

void USART_Transmit( unsigned char data) {
	/*Wait for empty transmit buffer*/
	PORTD |= (1 << PORTD2);
	while (!(UCSR0A & (1<<UDRE0))); // UDREn == Data Register Empty (indicates whether the transmit buffer is ready to recieve new data.
	/*Put data into buffer, sends the data*/
	UDR0 = data; //UDREn cleared.
}

//receive is never used
// unsigned char USART_Recieve(void){
//
// 	char data;
// 	PORTD &= ~(1 << PORTD2);
// 	data = UDR0;
// 	return data;
// }

/*Move servos placed one by one.*/
void move_Single_Servo(unsigned int position, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID){ 
	
	byte checksum;
	
	volatile unsigned char s_low_byte = position;			//lower byte of position for servo
	volatile unsigned char s_high_byte = (position >> 8);	//higher byte of position for servo
	
	USART_Transmit(0xFF);			//start byte
	USART_Transmit(0xFF);			//start byte
	USART_Transmit(servo_ID);		//servo ID
	USART_Transmit(0x07);			//length = 7
	USART_Transmit(WRITE);			//instruction = write_data
	USART_Transmit(GOAL_POSITION_L);	//address = goal position(L)
	USART_Transmit(s_low_byte);		//send low byte of position
	USART_Transmit(s_high_byte);	//send high byte of position
	USART_Transmit(speed_l);		//send low byte of speed
	USART_Transmit(speed_h);		//send high byte of speed
	
	checksum = servo_ID + 0x07 + WRITE + GOAL_POSITION_L + s_low_byte + s_high_byte + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USART_Transmit(checksum);
	
}

/*Move servos placed two by two.*/
void move_Double_Servo(unsigned int position, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID1, unsigned int servo_ID2){
	
	byte checksum;
	
	volatile unsigned int tmp = 1023-position;
	
	volatile unsigned char s1_low_byte = position;			//lower byte of position for servo 1
	volatile unsigned char s1_high_byte = (position >> 8);	//higher byte of position for servo 1
	
	volatile unsigned char s2_low_byte = tmp;			//lower byte of position for servo 2
	volatile unsigned char s2_high_byte = (tmp >> 8);	//higher byte of position for servo 2
	
	USART_Transmit(0xFF);				//start byte
	USART_Transmit(0xFF);				//start byte
	USART_Transmit(0xFE);				//broadcast ID
	USART_Transmit(0x0E);				//length is 14
	USART_Transmit(SYNC_WRITE);			//instruction = sync_data (2 servo)
	USART_Transmit(GOAL_POSITION_L);	
	USART_Transmit(0x04);				//data length is 4
	USART_Transmit(servo_ID1);			//servo id1
	USART_Transmit(s1_low_byte);		//send low byte of position
	USART_Transmit(s1_high_byte);		//send high byte of position
	USART_Transmit(speed_l);			//send low byte of speed
	USART_Transmit(speed_h);			//send high byte of speed
	USART_Transmit(servo_ID2);			//servo id2
	USART_Transmit(s2_low_byte);		//send low byte of position
	USART_Transmit(s2_high_byte);		//send high byte of position
	USART_Transmit(speed_l);			//send low byte of speed
	USART_Transmit(speed_h);			//send high byte of speed
	
	checksum = 0xFE + 0x0E + SYNC_WRITE + GOAL_POSITION_L + 0x04 + servo_ID1 + s1_low_byte + s1_high_byte + speed_l + speed_h + servo_ID2 + s2_low_byte + s2_high_byte + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USART_Transmit(checksum);
}

/*Move the arm to default position*/
void default_Position(){
	
	move_Single_Servo(0x1FF, 0x50, 0x01, 0x07);			//setting servo 7 straight
	move_Single_Servo(0x332, 0x50, 0x01, 0x06);			//setting servo 6 to straight up
	move_Double_Servo(0x1FF, 0x50, 0x00, 0x02, 0x03);	//0x1FF is straight upwards
	move_Double_Servo(0xCC, 0x50, 0x00, 0x04, 0x05);	//0xCC (60 deg) is 0 degree position for servo 4 and 5
	move_Double_Servo(0xCC, 0x50, 0x00, 0x02, 0x03);	//0xCC (60 deg) is 0 degree position for servo 2 and 3
	move_Single_Servo(0x1FF, 0x20, 0x00, 0x01);			//setting servo one to point forward
	
	//updating positions
	joint1_Pos = 0x1FF;
	joint2_Pos = 0xCC;
	joint3_Pos = 0xCC;
	joint4_Pos = 0x3EE;
	joint5_Pos = 0x1FF;
}

/*Sets the arm in pickup default position. */
void pickup_Default_Position(){
	
	move_Double_Servo(0x288, 0xF0, 0x00, 0x02, 0x03);	//Servo 2 & 3 in position 190 degrees
	move_Double_Servo(0x288, 0xF0, 0x00, 0x04, 0x05);
	move_Single_Servo(0x1FF, 0xF0, 0x00, 0x06);			//Servo 6 in position 150 degrees
	move_Single_Servo(0x1FF, 0x50, 0x01, 0x07);
	move_Single_Servo(0x1FF, 0x50, 0x00, 0x08);
	
	//updating positions
	joint2_Pos = 0x288;
	joint3_Pos = 0x288;
	joint4_Pos = 0x1FF;
	joint5_Pos = 0x1FF;
	joint6_Pos = 0x1FF;
}

/*Drop the item on either right or left side.*/
void drop_Item(_Bool side){ //Side = 1 right side, side = 0 left side
	
	//Only servo 1 that is different
	if(side == 0){ //Drop on left side
		move_Single_Servo(0x332, 0x50, 0x00, 0x01); //Servo 1 in position 240 degrees
	}
	else if(side == 1){ //Drop on right side
		move_Single_Servo(0xCC, 0x50, 0x00, 0x01); //Servo 1 in position 60 (CC) degrees
	}
	
	_delay_ms(3000);
	
	//Vad är lämpligt läge på följande servon?
	move_Double_Servo(0x2CC, 0x60, 0x00, 0x02, 0x03);	//Servo 2 and 3 in position 210 degrees
	move_Double_Servo(0x2CC, 0x60, 0x00, 0x04, 0x05);	//Servo 4 and 5 in position 210 degrees
	move_Single_Servo(0x111, 0x60, 0x00, 0x06);			//Servo 6 in position 80 degrees (111)
	move_Single_Servo(0x1FF, 0x50, 0x01, 0x07);
	
	_delay_ms(4000);
	
	move_Single_Servo(0x1FF, 0x60, 0x00, 0x08); //Servo 8 in position 150 (1FF) degrees
	
	_delay_ms(3000);
	
	default_Position();
	
}

/*
* Device function: Move the arm
*
* @param joint	joint to be moved
* @param direction	direction of movement
* @param amp	if movement is amplified or not
*/
void move_Arm(uint8_t armData){
	
	uint8_t joint = armData & 0x07;
	_Bool direction = (armData >> 4) & 0x01;
	_Bool amp = (armData >> 3) & 0x01;
	_Bool DPP = (armData >> 5) & 0x01;
	
	if (DPP)
	{
		pickup_Default_Position();
	}
	else
	{
		
		
		if (joint == 1)
		{
			if (direction == 0 && (joint1_Pos - (10+40*amp)) > 0)
			{
				joint1_Pos -= (10+40*amp);
				move_Single_Servo(joint1_Pos, 0x50, 0x00, 0x01);
			}
			else if(direction == 1 && (joint1_Pos + (10+40*amp)) < 1023)
			{
				joint1_Pos += (10+40*amp);
				move_Single_Servo(joint1_Pos, 0x50, 0x00, 0x01);
			}
		}
		else if(joint == 2)
		{
			if (direction == 0 && (joint2_Pos - (10+40*amp)) > 0xCD)
			{
				joint2_Pos -= (10+40*amp);
				move_Double_Servo(joint2_Pos, 0x50, 0x00, 0x02, 0x03);
			}
			else if(direction == 1 && (joint2_Pos + (10+40*amp)) < 0x322)
			{
				joint2_Pos += (10+40*amp);
				move_Double_Servo(joint2_Pos, 0x50, 0x00, 0x02, 0x03);
			}
		}
		else if(joint == 3)
		{
			if (direction == 0 && (joint3_Pos - (10+40*amp)) > 0xCD)
			{
				joint3_Pos -= (10+40*amp);
				move_Double_Servo(joint3_Pos, 0x50, 0x00, 0x04, 0x05);
			}
			else if(direction == 1 && (joint3_Pos + (10+40*amp)) < 0x322)
			{
				joint3_Pos += (10+40*amp);
				move_Double_Servo(joint3_Pos, 0x50, 0x00, 0x04, 0x05);
			}
		}
		else if (joint == 4)
		{
			if (direction == 0 && (joint4_Pos - (10+40*amp)) > 0) //kolla gränserna
			{
				joint4_Pos -= (10+40*amp);
				move_Single_Servo(joint4_Pos, 0x50, 0x00, 0x06);
			}
			else if(direction == 1 && (joint4_Pos + (10+40*amp)) < 1023) //kolla gränserna
			{
				joint4_Pos += (10+40*amp);
				move_Single_Servo(joint4_Pos, 0x50, 0x00, 0x06);
			}
		}
		else if (joint == 5)
		{
			if (direction == 0 && (joint5_Pos - (10+40*amp)) > 0)
			{
				joint5_Pos -= (10+40*amp);
				move_Single_Servo(joint5_Pos, 0x50, 0x00, 0x07);
			}
			else if(direction == 1 && (joint5_Pos + (10+40*amp)) < 1023)
			{
				joint5_Pos += (10+40*amp);
				move_Single_Servo(joint5_Pos, 0x50, 0x00, 0x07);
			}
		}
		else if (joint == 6)
		{
			if (direction == 0 && (joint6_Pos - (10+40*amp)) > 0) //kolla gränserna
			{
				joint6_Pos -= (10+40*amp);
				move_Single_Servo(joint6_Pos, 0x50, 0x00, 0x08);
			}
			else if(direction == 1 && (joint6_Pos + (10+40*amp)) < 1023) //kolla gränserna
			{
				joint6_Pos += (10+40*amp);
				move_Single_Servo(joint6_Pos, 0x50, 0x00, 0x08);
			}
		}
	}
}

int main(void)
{
	
	USART_Init();
	
	
	while(1)
	{
		

	}
}
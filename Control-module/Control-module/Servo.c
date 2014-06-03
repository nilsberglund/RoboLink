/*
* Servo.c
*
* Created: 3/30/2014 12:16:02 PM
* Author: Susanna Bäckman
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "Servo.h"
#include <util/delay.h>
#include "slaveControl.h"
#include <avr/interrupt.h>

/*Initializes ports and registers to communicate with robot arm*/
void armInit(void) {
	
	DDRD |= (1<<DDD3); //Setting D2 to output to control the tri-state
	/*Set baud rate.*/
	UBRR0H = (unsigned char)(0);  //baudvalue = (f_cpu/baudrate*16) -1
	UBRR0L = (unsigned char)(0);
	/* Enable receiver and transmitter.*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/*Set frame format. Asynchronous mode, no parity, 1 stop bit, char size 8.  */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
		
	defaultPosition(); 	
	moveSingleServo(0x1FF, 0x50, 0x00, 0x08); //150 grader
}

/*Transmits a byte via USART*/
void USARTTransmit( unsigned char data) {
	/*Wait for empty transmit buffer*/
	PORTD |= (1 << PORTD3);
	while (!(UCSR0A & (1<<UDRE0))); // UDREn == Data Register Empty (indicates whether the transmit buffer is ready to recieve new data.
	/*Put data into buffer, sends the data*/
	UDR0 = data; //UDREn cleared.
}


/*Move single servos to a position*/
void moveSingleServo(unsigned int position, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID) { 
	
	byte checksum;
	
	volatile unsigned char s_low_byte = position;			//lower byte of position for servo
	volatile unsigned char s_high_byte = (position >> 8);	//higher byte of position for servo
	
	USARTTransmit(0xFF);			//start byte
	USARTTransmit(0xFF);			//start byte
	USARTTransmit(servo_ID);		//servo ID
	USARTTransmit(0x07);			//length = 7
	USARTTransmit(WRITE);			//instruction = write_data
	USARTTransmit(GOAL_POSITION_L);	//address = goal position(L)
	USARTTransmit(s_low_byte);		//send low byte of position
	USARTTransmit(s_high_byte);		//send high byte of position
	USARTTransmit(speed_l);			//send low byte of speed
	USARTTransmit(speed_h);			//send high byte of speed
	
	checksum = servo_ID + 0x07 + WRITE + GOAL_POSITION_L + s_low_byte + s_high_byte + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USARTTransmit(checksum);
	
}

/*Move servos placed two and two to a position.*/
void moveDoubleServo(unsigned int position, uint8_t speed_l, uint8_t speed_h, uint8_t servo_ID1, unsigned int servo_ID2) {
	
	byte checksum;
	
	volatile unsigned int tmp = 1023-position;
	
	volatile unsigned char s1_low_byte = position;			//lower byte of position for servo 1
	volatile unsigned char s1_high_byte = (position >> 8);	//higher byte of position for servo 1
	
	volatile unsigned char s2_low_byte = tmp;			//lower byte of position for servo 2
	volatile unsigned char s2_high_byte = (tmp >> 8);	//higher byte of position for servo 2
	
	USARTTransmit(0xFF);			//start byte
	USARTTransmit(0xFF);			//start byte
	USARTTransmit(0xFE);			//broadcast ID
	USARTTransmit(0x0E);			//length is 14
	USARTTransmit(SYNC_WRITE);		//instruction = sync_data (2 servo)
	USARTTransmit(GOAL_POSITION_L);	
	USARTTransmit(0x04);			//data length is 4
	USARTTransmit(servo_ID1);		//servo id1
	USARTTransmit(s1_low_byte);		//send low byte of position
	USARTTransmit(s1_high_byte);	//send high byte of position
	USARTTransmit(speed_l);			//send low byte of speed
	USARTTransmit(speed_h);			//send high byte of speed
	USARTTransmit(servo_ID2);		//servo id2
	USARTTransmit(s2_low_byte);		//send low byte of position
	USARTTransmit(s2_high_byte);	//send high byte of position
	USARTTransmit(speed_l);			//send low byte of speed
	USARTTransmit(speed_h);			//send high byte of speed
	
	checksum = 0xFE + 0x0E + SYNC_WRITE + GOAL_POSITION_L + 0x04 + servo_ID1 + s1_low_byte + s1_high_byte + speed_l + speed_h + servo_ID2 + s2_low_byte + s2_high_byte + speed_l + speed_h;
	
	checksum = ~checksum;
	
	USARTTransmit(checksum);
}

/*Move the arm to default position*/
void defaultPosition() {
	
	moveSingleServo(0x1FF, 0x40, 0x00, 0x07);			//setting servo 7 (joint 5) straight
	moveSingleServo(0x332, 0x50, 0x00, 0x06);			//setting servo 6 (joint 4) straight up
	moveDoubleServo(0x1FF, 0x90, 0x00, 0x02, 0x03);		//setting servo 2 & 3 (joint 2) to 0x1FF (straight upwards)
	moveDoubleServo(0xCC, 0x50, 0x00, 0x04, 0x05);		//0xCC (60 deg) is 0 degree position for servo 4 and 5 (joint 3)
	moveDoubleServo(0xCC, 0x90, 0x00, 0x02, 0x03);		//0xCC (60 deg) is 0 degree position for servo 2 and 3 (joint 2)
	moveSingleServo(0x1FF, 0x15, 0x00, 0x01);			//setting servo 1 (joint 1)
	
	//Updating positions
	joint1_Pos = 0x1FF;
	joint2_Pos = 0xCC;
	joint3_Pos = 0xCC;
	joint4_Pos = 0x332;
	joint5_Pos = 0x1FF;
}

/*Move the arm to pickup default position.*/
void pickupDefaultPosition() {
	
	TIMSK0 &= ~(1<<OCIE0A);
	
	if(side == 0){ //Pickup on left side
		moveSingleServo
		(0x332, 0x80, 0x00, 0x01); //Servo 1 in position 240 degrees
		joint1_Pos = 0x332;
	}
	else if(side == 1){ //Pickup on right side
		moveSingleServo(0xCC, 0x80, 0x00, 0x01); //Servo 1 in position 60 (CC) degrees
		joint1_Pos = 0xCC;
	}
	
	moveDoubleServo(0x270, 0x50, 0x00, 0x02, 0x03);		//Joint 2 //288 innan
	moveDoubleServo(0x288, 0x50, 0x00, 0x04, 0x05);		//Joint 3
	moveSingleServo(0x144, 0x50, 0x00, 0x06);			//Joint 4
	moveSingleServo(0x1FF, 0x50, 0x00, 0x07);			//Joint 5
	moveSingleServo(0x1FF, 0x50, 0x00, 0x08);			//Joint 6
	
	//Updating positions
	
	joint2_Pos = 0x270; //Var 288 innan
	joint3_Pos = 0x288;
	joint4_Pos = 0x144;
	joint5_Pos = 0x1FF;
	joint6_Pos = 0x1FF;
	
	_delay_ms(3000);
	
	TIMSK0 |= (1<<OCIE0A);
	
}

/*Drop the item on either left or right side.*/
void dropItem() { //Side = 1 right side, side = 0 left side
	
	//Only servo 1 that is different
	if(side == 0){ //Drop on left side
		moveSingleServo
		(0x332, 0x80, 0x00, 0x01); //Servo 1 in position 240 degrees
	}
	else if(side == 1){ //Drop on right side
		moveSingleServo(0xCC, 0x80, 0x00, 0x01); //Servo 1 in position 60 (CC) degrees
	}
	
	_delay_ms(2000);
	
	moveDoubleServo(dropJoint2Pos, 0x60, 0x00, 0x02, 0x03);	//Servo 2 and 3 in position 203 (2B4) degrees
	moveDoubleServo(dropJoint3Pos, 0x60, 0x00, 0x04, 0x05);	//Servo 4 and 5 in position 210 degrees
	moveSingleServo(dropJoint4Pos, 0x60, 0x00, 0x06);		//Servo 6 in position 60 grader D3
	moveSingleServo(dropJoint5Pos, 0x50, 0x01, 0x07);
	
	_delay_ms(3000);
	
	moveSingleServo(0x1FF, 0x60, 0x00, 0x08); //Servo 8 in position 150 (1FF) degrees 
	
	_delay_ms(1500);
	
	defaultPosition();
	
	_delay_ms(1200);
	
	TXFinishedDrop();
}

/*Moves the arm according to armData byte. Byte received by communication module*/
void moveArm(uint8_t armData) {
	
	uint8_t joint = armData & 0x07;
	_Bool direction = (armData >> 4) & 0x01;
	_Bool amp = (armData >> 3) & 0x01;
	_Bool DPP = (armData >> 5) & 0x01;
	_Bool DTP = (armData >> 6) & 0x01;
	
	if (DPP)
	{
		pickupDefaultPosition();
	}
	else if (DTP)
	{
		
		TIMSK0 &= ~(1<<OCIE0A);
		
		defaultPosition();
	}
	else
	{
		
		
		if (joint == 1)
		{
			if (direction == 0 && (joint1_Pos - (10+40*amp)) > 0)
			{
				joint1_Pos -= (10+40*amp);
				moveSingleServo(joint1_Pos, 0x50, 0x00, 0x01);
			}
			else if(direction == 1 && (joint1_Pos + (10+40*amp)) < 1023)
			{
				joint1_Pos += (10+40*amp);
				moveSingleServo(joint1_Pos, 0x50, 0x00, 0x01);
			}
		}
		else if(joint == 2)
		{
			if (direction == 0 && (joint2_Pos - (10+40*amp)) > 0xCD)
			{
				joint2_Pos -= (10+40*amp);
				moveDoubleServo(joint2_Pos, 0x50, 0x00, 0x02, 0x03);
			}
			else if(direction == 1 && (joint2_Pos + (10+40*amp)) < 0x322)
			{
				joint2_Pos += (10+40*amp);
				moveDoubleServo(joint2_Pos, 0x50, 0x00, 0x02, 0x03);
			}
		}
		else if(joint == 3)
		{
			if (direction == 0 && (joint3_Pos - (10+40*amp)) > 0xCD)
			{
				joint3_Pos -= (10+40*amp);
				moveDoubleServo(joint3_Pos, 0x50, 0x00, 0x04, 0x05);
			}
			else if(direction == 1 && (joint3_Pos + (10+40*amp)) < 0x322)
			{
				joint3_Pos += (10+40*amp);
				moveDoubleServo(joint3_Pos, 0x50, 0x00, 0x04, 0x05);
			}
		}
		else if (joint == 4)
		{
			if (direction == 0 && (joint4_Pos - (10+40*amp)) > 0)
			{
				joint4_Pos -= (10+40*amp);
				moveSingleServo(joint4_Pos, 0x50, 0x00, 0x06);
			}
			else if(direction == 1 && (joint4_Pos + (10+40*amp)) < 818)
			{
				joint4_Pos += (10+40*amp);
				moveSingleServo(joint4_Pos, 0x50, 0x00, 0x06);
			}
		}
		else if (joint == 5)
		{
			if (direction == 0 && (joint5_Pos - (10+40*amp)) > 0)
			{
				joint5_Pos -= (10+40*amp);
				moveSingleServo(joint5_Pos, 0x50, 0x00, 0x07);
			}
			else if(direction == 1 && (joint5_Pos + (10+40*amp)) < 1023)
			{
				joint5_Pos += (10+40*amp);
				moveSingleServo(joint5_Pos, 0x50, 0x00, 0x07);
			}
		}
		else if (joint == 6)
		{
			if (direction == 0 && amp == 0)
			{
				moveSingleServo(0xBB, 0x50, 0x00, 0x08); //Grip narrow
				dropJoint2Pos = joint2_Pos;
				dropJoint3Pos = joint3_Pos;
				dropJoint4Pos = joint4_Pos;
				dropJoint5Pos = joint5_Pos;
				
			}
			else if (direction == 0 && amp == 1)
			{
				moveSingleServo(0x12C, 0x50, 0x00, 0x08); //Grip broad
				dropJoint2Pos = joint2_Pos;
				dropJoint3Pos = joint3_Pos;
				dropJoint4Pos = joint4_Pos;
				dropJoint5Pos = joint5_Pos;
			}
			else if(direction == 1)
			{
				moveSingleServo(0x1FF, 0x50, 0x00, 0x08); //Release
			}
		}
	}
}

/*Updating the servo positions. Avoiding servos being tired*/
void updateServos()
{
		moveSingleServo(joint1_Pos, 0x30, 0x00, 0x01);
		moveDoubleServo(joint2_Pos, 0x30, 0x00, 0x02, 0x03);
		moveDoubleServo(joint3_Pos, 0x30, 0x00, 0x04, 0x05);
		moveSingleServo(joint4_Pos, 0x30, 0x00, 0x06);
}
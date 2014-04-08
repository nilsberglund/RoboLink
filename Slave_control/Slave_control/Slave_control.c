/*
 * Slave_Control.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t instruction_data;
uint8_t sensor_data;
uint8_t robot_arm_data;
uint8_t wheel_steering_data;
unsigned char RFID[10];
_Bool waiting_for_instruction; 
uint8_t component;
uint8_t sensor = 1;
uint8_t arm = 2;
uint8_t wheel = 3;

void SPI_Init_Slave();
void Slave_TX(uint8_t);

ISR(SPI_STC_vect)
{
	data = SPDR;
	if(waiting_for_instruction)
	{
		if(instruction_data == 0b10000100)
		{
			waiting_for_instruction = !waiting_for_instruction;
			component = sensor;
		} else if ()
	} else
	{
		if(component == sensor)
		{
			sensor_data = data;
		}else if (component == arm)
		{
			robot_arm_data = data;
		} else if (component == wheel)
		{
			wheel_steering_data = data;
		}
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
			
			SPCR = 0xC0;
			
			sei(); 
	}
	
void Slave_TX(uint8_t data)
	{
		SPDR = data;
		PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
		PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
	}
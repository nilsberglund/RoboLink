/*
 * Slave_Control.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>


//uint8_t instruction_data;
uint8_t data; 
uint8_t sensor_data;
uint8_t robot_arm_data;
uint8_t wheel_steering_data;
unsigned char RFID[10];
_Bool waiting_for_instruction = 1; 
uint8_t component = 0;
uint8_t sensor = 1;
uint8_t arm = 2;
uint8_t wheel = 3;
uint8_t RFIDcomp = 4;
uint8_t counter = 0;

void SPI_Init_Slave();
void Slave_TX(uint8_t);

ISR(SPI_STC_vect)
{
	data = SPDR;
	PORTD = data;
	
	if(waiting_for_instruction)
	{
		if(data == 0b10000100)
		{
			waiting_for_instruction = !waiting_for_instruction;
			component = sensor;
		} else if (data == 0b10000101)
		{
			waiting_for_instruction = !waiting_for_instruction;
			component = wheel;
		} else if(data == 0b10000110)
		{
			waiting_for_instruction = !waiting_for_instruction;
			component = arm;
		}	else if(data == 0b10101011)
		{
			waiting_for_instruction = !waiting_for_instruction;
			component = RFIDcomp;
		}	else if(data == 0b00000101)
		{
			Slave_TX(wheel_steering_data);
		}	else if(data == 0b00000110)
		{
			Slave_TX(robot_arm_data);
		}
	} else
	{
		if(component == sensor)
		{
			sensor_data = data;
			waiting_for_instruction = !waiting_for_instruction;
		}else if (component == arm)
		{
			robot_arm_data = data;
			waiting_for_instruction = !waiting_for_instruction;
		} else if (component == wheel)
		{
			wheel_steering_data = data;
			waiting_for_instruction = !waiting_for_instruction;
		} else if (component == RFIDcomp)
		{
			
			RFID[counter] = data;
			if(counter == 9)
			{
				waiting_for_instruction = !waiting_for_instruction;
				counter = 0;
			} else
			{
				counter++;
			}
			
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
			
			SPCR = 0xC3;
			
			sei(); 
			
			wheel_steering_data = 0xAA;
	}
	
void Slave_TX(uint8_t data)
	{
		SPDR = data;
		PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
		PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
	}
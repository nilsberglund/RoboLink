/*
 * Slave_Control.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "Slave_control.h"

ISR(SPI_STC_vect)
{
	volatile uint8_t data;
	data = SPDR;
	
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
			
			RFID[RFID_counter] = data;
			if(RFID_counter == 9)
			{
				waiting_for_instruction = !waiting_for_instruction;
				RFID_counter = 0;
			} else
			{
				RFID_counter++;
			}
		}
	}
}

void SPI_Init_Slave()
	{

			DDRD = 0xFF;
			DDRB = 0x48;
			
			SPCR = 0xC3;
			
			sei(); 
			
			
			waiting_for_instruction = 1;
			component = 0;
			sensor = 1;
			arm = 2;
			wheel = 3;
			RFIDcomp = 4;
			RFID_counter = 0;
	}
	
void Slave_TX(uint8_t data)
{
	SPDR = data;
	PORTB |= (1 << PORTB3); // Sets INT_Req high. I.e throws interrupt.
	PORTB &= ~(1 << PORTB3); // Sets INT_Req low. Interrupt finished.
}
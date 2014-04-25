/*
 * Control_module.c
 *
 * Created: 4/23/2014 9:05:12 AM
 *  Author: NISSE B
 */ 

#include <avr/io.h>
#include "Slave_control.h"
#include "Control_wheel_steering.h"
#include <avr/interrupt.h>


ISR(SPI_STC_vect)
{
	data = SPDR;
	
	if(data == 0x84)
	{
		
	} else if(data != 0x84)
	{
		sensor_data = data;
	}
	
	 controlAlgorithm(); 
}


int main(void)
{
	uint8_t done;
	done = 0;
	if(done == 0)
	{
		SPI_Init_Slave();
		driving_setup();	
	}
	
	//init_arm()
	//init_LCD()
	
    while(1)
    {
       
    }
}










/*


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
			component = 0; 
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
	
	
*/
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
#include "Servo.h"


ISR(SPI_STC_vect)
{
	data = SPDR;

	if(waiting_for_instruction == 1)
	{
		if(data == 0b10000100)
		{
			waiting_for_instruction = 0;
			component = SENSOR;
			
		} else if (data == 0b10000101)
		{
			waiting_for_instruction = 0;
			component = WHEEL;
			
		} else if(data == 0b10000110)
		{
			waiting_for_instruction = 0;
			component = ARM;
			
		}
	}
	else
	{
		if(component == SENSOR)
		{
			sensor_data = data;
			controlAlgorithm();
			waiting_for_instruction = 1;
		}else if (component == ARM)
		{
			robot_arm_data = data;
			move_Arm(robot_arm_data);
			waiting_for_instruction = 1;
		} else if (component == WHEEL)
		{
			wheel_steering_data = data;
			waiting_for_instruction = 1;
		}
	}
}


int main(void)
{
	SPI_Init_Slave();
	driving_setup();
	Arm_Init();
	while(1)
	{
		
	}
}



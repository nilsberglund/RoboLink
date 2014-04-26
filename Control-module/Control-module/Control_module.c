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
	//Arm_Init();
	
	//init_LCD()
	
	while(1)
	{
		
	}
}






// data = SPDR;
//
// if(data == 0x84)
// {
//
// } else if(data != 0x84)
// {
// 	sensor_data = data;
// }
//
// controlAlgorithm();



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
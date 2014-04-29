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

	if(waitingForInstruction == 1)
	{
		if(data == 0b10000100)
		{
			waitingForInstruction = 0;
			component = SENSOR;
			
		} else if (data == 0b10000101)
		{
			waitingForInstruction = 0;
			component = WHEEL;
			
		} else if(data == 0b10000110)
		{
			waitingForInstruction = 0;
			component = ARM;
			
		}
	}
	else
	{
		if(component == SENSOR)
		{
			sensorData = data;
			controlAlgorithm();
			waitingForInstruction = 1;
		}else if (component == ARM)
		{
			armData = data;
			moveArm(armData);
			waitingForInstruction = 1;
		} else if (component == WHEEL)
		{
			steeringData = data;
			moveRobot();
			waitingForInstruction = 1;
		}
	}
}


int main(void)
{
	SPIInitSlave();
	drivingSetup();
	armInit();
	while(1)
	{
		
	}
}



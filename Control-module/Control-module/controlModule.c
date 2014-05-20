/*
* controlModule.c
*
* Created: 4/23/2014 9:05:12 AM
*  Author: Nils Berglund
*/

#include <avr/io.h>
#include "slaveControl.h"
#include "controlWheelSteering.h"
#include <avr/interrupt.h>
#include "Servo.h"
#include "controlModule.h"

/*Interrupt that runs when new SPI data is received*/
ISR(SPI_STC_vect)
{
	data = SPDR;
	
	if(waitingForInstruction == 1)
	{
		if(data == 0b10000100)
		{
			component = SENSOR;
			waitingForInstruction = 0;
			
			TIMSK0 &= ~(1<<OCIE0A);
			
			
		} else if (data == 0b10000101)
		{
			component = WHEEL;
			waitingForInstruction = 0;
		} else if(data == 0b10000110)
		{
			waitingForInstruction = 0;
			component = ARM;
		} else if(data == 0b10000111)
		{
			component = KPROPORTIONAL;
			waitingForInstruction = 0;
		} else if(data == 0b10001011)
		{
			component = KDERIVATIVE;
			waitingForInstruction = 0;
		} else if(data == 0b10001111)
		{
			component = DROPITEM;
			waitingForInstruction = 0;
		} else if(data == 0b10010000)
		{
			waitingForInstruction = 1;
			startTransportMode();
		}
	}
	else
	{
		waitingForInstruction = 1;
		if(component == SENSOR)
		{
			sensorData = data;
			if(stationMode == 0)
			{
				controlAlgorithm();
			}
		}else if (component == ARM)
		{
			armData = data;
			moveArm(armData);
		} else if (component == WHEEL)
		{
			steeringData = data;
			moveRobot();
		}  else if (component == KPROPORTIONAL)
		{
			changeProportional(data);
		}  else if (component == KDERIVATIVE)
		{
			changeDerivative(data);
		}  else if (component == DROPITEM)
		{
			dropItem();
		}
	}
}
/*Interrupt that runs when timer matches compare register. Updates servo position*/
ISR(TIMER0_COMPA_vect)
{
	if(stationMode == 1)
	{
		updateServos();	
	}
	
}

int main(void)
{
	SPIInitSlave();
	drivingSetup();
	armInit();
	initTimer();
	while(1)
	{
		
	}
}

/*Initializes the timer*/
void initTimer()
{
	TCCR0A = 0;
	TCCR0B = 0x05;
	TCNT0 = 0;
	OCR0A = 150;
	
	stationMode = 0;
}

/*Starts transport mode*/
void startTransportMode()
{
	stationMode = 0;
}
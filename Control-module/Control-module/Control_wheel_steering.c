/*
* Control_module.c
*
* Created: 4/23/2014 9:05:12 AM
*  Author: NISSE B
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "Control_wheel_steering.h"
#include "Slave_control.h"

int8_t getError()
{
	
	volatile int8_t res = 0;
	volatile int8_t marker = 0;
	volatile uint8_t counter1 = 0;
	error = 0;
	uint8_t line_data;
	line_data = sensor_data;	
	
	for (int8_t noShift = 6; noShift >= 0; noShift--)
	{
		res = ((line_data >> noShift) & 0x01);
		if(res == 1)
		{
			marker = marker + (7 - noShift);
			counter1++;
		}
	}
	if(counter1 == 1)
	{
		error = marker + (marker-1);
	} else if(counter1 == 2)

	{
		error = marker-1;
	} else if(counter1 == 3)

	{
		error = marker*2;
		error = error/(0b00000011);
		error = error - 1;

	} else
	{
		error = 7;
	}
	error = 7 - error;
	return error;
}

void controlAlgorithm()
{	
	
	error = getError();
	int16_t speed = calculateSpeed(error);
	if ((200-speed) < 10)
	{
		rightWheelSpeed = 30;
		rightWheelDirection = 1;
	} 
	else if ((200-speed) > 235)
	{
		rightWheelSpeed = 235;
		rightWheelDirection = 0;
	}
	else
	{
		rightWheelSpeed = 200 - speed;
		rightWheelDirection = 1;
	}
	
	if ((200+speed) < 10)
	{
		leftWheelSpeed = 30;
		leftWheelDirection = 1;
	}
	else if ((200+speed) > 235)
	{
		leftWheelSpeed = 235;
		leftWheelDirection = 0;
	}
	else
	{
		leftWheelSpeed = 200 + speed;
		leftWheelDirection = 1;
	}
	
	
	drive(1, 1, leftWheelSpeed, rightWheelSpeed);
	
	
}

int8_t calculateSpeed(int8_t error)
{
	volatile int16_t speed = 0;
	int8_t Kp = 20;
	int8_t Kd = 5;
	
	speed = Kp * error + Kd * (error - prevError);

	prevError = error;
	return speed;
}

void driving_setup()
{
	TCCR1A    = 0b11110001; //Sets the mode to Phase Correct PWM and sets the Comp to set on incrementing.
	TCCR1B = 3; //Sets the prescaling to 8
	TCNT1 = 0;
	OCR1A = 248;
	OCR1B = 248;
	DDRD |= (1 << PORTD4)|(1 << PORTD5)|(1 << PORTD6)|(1 << PORTD7);
	
}

void drive(int right_dir, int left_dir, uint16_t leftSpeed, uint16_t rightSpeed)
{
	if(right_dir == 1)
	{
	PORTD |= (0 << PORTD7);
	}
	else
	{
	PORTD |= (1 << PORTD7);
	}
	if(left_dir == 1)
	{
		PORTD |= (1 << PORTD6);
	}
	else
	{
		PORTD |= (0 << PORTD6);
	}
	OCR1A = rightSpeed;
	OCR1B = leftSpeed;
}

void drive_forward(uint8_t speed)
{
	drive(1, 1, speed, speed);
}

void drive_backwards(uint8_t speed)
{
	drive(0, 0, speed, speed);
}

void stop()
{
	drive(1, 1, 255, 255);
}

void drive_right_forward(uint8_t speed)
{
	drive(1, 1, speed + 15, speed);
}

void drive_left_forward(uint8_t speed)
{
	drive(1, 1, speed, speed + 15);
}



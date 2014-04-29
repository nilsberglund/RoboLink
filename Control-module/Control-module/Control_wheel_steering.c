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
#include <stdlib.h>

/*Function that calculates the line error*/
int8_t getError()
{
	volatile int8_t res = 0; 
	volatile int8_t sum = 0; // Sum of all data from active sensors (1,..,7)
	volatile uint8_t counter1 = 0; // Counter which counts number of sensors activated 
	error = 0; // Error from line
	uint8_t lineData; 
	lineData = sensorData;
	
	for (int8_t noShift = 6; noShift >= 0; noShift--)
	{
		res = ((lineData >> noShift) & 0x01);
		if(res == 1)
		{
			sum = sum + (7 - noShift);
			counter1++;
		}
	}
	if(counter1 == 1)
	{
		error = sum + (sum-1);
	} else if(counter1 == 2)

	{
		error = sum-1;
	} else if(counter1 == 3)

	{
		error = sum*2;
		error = error/(0b00000011);
		error = error - 1;
		
	} else if(counter1 == 4 || counter1 == 5)
	{
		if(sum == 10)
		{
			error = 50;
		} else if(sum == 22)
		{
			error = 50;
		} else if(sum == 15)
		{
			error = 50;
		} else if(sum == 25)
		{
			error = 50;
		}
	} 	
	
	 else
	{
		error = -8;
	}
	error = 7 - error;
	return error;
}

/* Function that controls the robot movement when following a line */
void controlAlgorithm()
{
	error = getError();
	
	
	if(error == 15)  // No sensors activated
	{
		rightWheelSpeed = rightWheelSpeed; 
		leftWheelSpeed = leftWheelSpeed;
		drive(1, 1, leftWheelSpeed, rightWheelSpeed);
	} else if(error == -43)
	{
		stop(); //4 or 5 sensors on either side activated, indicates station. Will make the robot stop.
	}	
	else
	{	
		midSpeed = 180;  //Standard speed
		int16_t speed = calculateSpeed(error);  //calculate new speed 
		if ((midSpeed-speed) < 10)
		{
			rightWheelSpeed = 3;
		}
		else if ((midSpeed-speed) > 235)
		{
			rightWheelSpeed = 248;
		}
		else
		{
			rightWheelSpeed = midSpeed - speed;
		}
		
		if ((midSpeed+speed) < 10)
		{
			leftWheelSpeed = 3;
		}
		else if ((midSpeed+speed) > 235)
		{
			leftWheelSpeed = 248;
		}
		else
		{
			leftWheelSpeed = midSpeed + speed;
		}
		drive(1, 1, leftWheelSpeed, rightWheelSpeed);
	}

}

/* function that calculates the speed */
int8_t calculateSpeed(int8_t error)
{
	volatile int16_t speed = 0;
	int8_t Kp = 20;
	int8_t Kd = 5;
	
	speed = Kp * error + Kd * (error - prevError);

	prevError = error;
	return speed;
}

/* function initiates driving */
void drivingSetup()
{
	TCCR1A    = 0b11110001; //Sets the mode to Phase Correct PWM and sets the Comp to set on incrementing.
	TCCR1B = 3; //Sets the prescaling to 128
	TCNT1 = 0; //Starts the timer
	OCR1A = 255; //Sets compare register => Robot does not move
	OCR1B = 255; // Sets compare register => Robot does not move
	DDRD |= (1 << PORTD4)|(1 << PORTD5)|(1 << PORTD6)|(1 << PORTD7); //Sets the data direction for the PWM and direction ports. 
}

/* Function that controls both direction and speed of the motors. 
	rightDir, leftDir = 1 => Forward. rightDir, leftDir = 0 => Backward. */
void drive(int rightDir, int leftDir, uint16_t leftSpeed, uint16_t rightSpeed)
{
	if(rightDir == 1)
	{
		PORTD &= ~(1 << PORTD7);
	}
	else
	{
		PORTD |= (1 << PORTD7);
	}
	if(leftDir == 1)
	{
		PORTD |= (1 << PORTD6);
	}
	else
	{
		PORTD &= ~(1 << PORTD6);
	}
	OCR1A = rightSpeed;
	OCR1B = leftSpeed;
}

/* Function that makes the robot move forward */
void driveForward(uint8_t speed)
{
	drive(1, 1, speed, speed);
}
/* Function that makes the robot move backward */
void driveBackward(uint8_t speed)
{
	drive(0, 0, speed, speed);
}
/* Function that makes the robot stop */
void stop()
{
	drive(1, 1, 255, 255);
}
/* Function that makes the robot move forward and to the right */
void driveForwardRight(uint8_t speed)
{
	drive(1, 1, speed - 40, speed + 40);
}

/* Function that makes the robot move forward and to the left */
void driveForwardLeft(uint8_t speed)
{
	drive(1, 1, speed + 40, speed - 40);
}

/* Function that rotates the robot counterclockwise */
void rotateCCW()
{
	uint8_t speed = 160;
	drive(1, 0, speed, speed);
}

/* Function that rotates the robot clockwise */
void rotateCW()
{
	uint8_t speed = 160;
	drive(0, 1, speed, speed);
}

/* Function that allows the user to control the robot movement via the GUI. */
void moveRobot()
{
	uint8_t FASTSPEED = 80;
	uint8_t SLOWSPEED = 200;
	if(((steeringData & 0x08) >> 3) == 1)
	{
		if(steeringData == 0b00001100)
		{
			driveForward(FASTSPEED);
		} else if(steeringData == 0b00001011)
		{
			driveForwardLeft(FASTSPEED);
		} else if(steeringData == 0b00001010)
		{
			driveForwardRight(FASTSPEED);
		} else if(steeringData == 0b00001001)
		{
			driveBackward(FASTSPEED);
		} else if(steeringData == 0b00001000)
		{
			stop();
		}
	}	else
	{
		if(steeringData == 0b00000100)
		{
			driveForward(SLOWSPEED);
		} else if(steeringData == 0b00000011)
		{
			driveForwardLeft(SLOWSPEED);
		} else if(steeringData == 0b00000010)
		{
			driveForwardRight(SLOWSPEED);
		} else if(steeringData == 0b00000001)
		{
			driveBackward(SLOWSPEED);
		} else if(steeringData == 0b00000000)
		{
			stop();
		} else if(steeringData == 0b00000101)
		{
			rotateCW();
		} else if(steeringData == 0b00000110)
		{
			rotateCCW();
		}
	}
}

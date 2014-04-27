/*
 * Control_wheel_steering.h
 *
 * Created: 4/23/2014 9:48:08 AM
 *  Author: nilbe317
 */ 

#ifndef CONTROL_WHEEL_STEERING_H_
#define CONTROL_WHEEL_STEERING_H_

/*-------Variables---------*/

volatile int8_t error;
int8_t prevError;
volatile uint8_t rightWheelSpeed;
volatile uint8_t leftWheelSpeed;
volatile uint8_t rightWheelDirection;
volatile uint8_t leftWheelDirection;
uint8_t midspeed;

/*-------Functions---------*/
void driving_setup();
void drive(int, int, uint16_t, uint16_t);
void controlAlgorithm();
int8_t getError();
int8_t calculateSpeed(int8_t);
uint8_t calculateMidSpeed(int8_t);
void stop();

#endif /* CONTROL_WHEEL_STEERING_H_ */
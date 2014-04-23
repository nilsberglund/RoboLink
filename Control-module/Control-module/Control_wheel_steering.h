/*
 * Control_wheel_steering.h
 *
 * Created: 4/23/2014 9:48:08 AM
 *  Author: nilbe317
 */ 

#ifndef CONTROL_WHEEL_STEERING_H_
#define CONTROL_WHEEL_STEERING_H_

/*-------Variables---------*/

int8_t error;
int8_t prevError;

/*-------Functions---------*/
void driving_setup();
void drive(int, int, uint8_t, uint8_t);
void controlAlgorithm();
int8_t getError();
int8_t calculateSpeed(int8_t);

#endif /* CONTROL_WHEEL_STEERING_H_ */
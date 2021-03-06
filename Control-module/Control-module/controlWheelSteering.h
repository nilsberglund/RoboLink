﻿/*
 * controlWheelSteering.h
 *
 * Created: 4/23/2014 9:48:08 AM
 *  Author: Nils Berglund
 */ 

#ifndef CONTROLWHEELSTEERING_H_
#define CONTROLWHEELSTEERING_H_

/*-------Variables---------*/
volatile int8_t error;
int8_t prevError;
volatile uint8_t rightWheelSpeed;
volatile uint8_t leftWheelSpeed;
volatile uint8_t rightWheelDirection;
volatile uint8_t leftWheelDirection;
uint8_t midSpeed;
int8_t Kp;
int8_t Kd;

/*-------Functions---------*/
void drivingSetup();
void drive(int, int, uint16_t, uint16_t);
void controlAlgorithm();
int8_t getError();
int8_t calculateSpeed(int8_t);
uint8_t calculatemidSpeed(int8_t);
void stop();
void moveRobot();
void rotateCW();
void rotateCCW();
void changeProportional(uint8_t);
void changeDerivative(uint8_t);

#endif /* CONTROLWHEELSTEERING_H_ */
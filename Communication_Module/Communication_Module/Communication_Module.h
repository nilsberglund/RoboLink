/*
 * Communication_Module.h
 *
 * Created: 4/23/2014 10:30:27 AM
 *  Author: susba199
 */ 


#ifndef COMMUNICATION_MODULE_H_
#define COMMUNICATION_MODULE_H_


/*--------Functions-----------*/
void initManualMode();
void toggleMode();


/*--------Variables-----------*/
volatile uint8_t automaticModeEnabled;
volatile uint8_t manualModeEnabled;
volatile uint8_t stationRightSensCounter;
volatile uint8_t stationLeftSensCounter;


#endif /* COMMUNICATION_MODULE_H_ */
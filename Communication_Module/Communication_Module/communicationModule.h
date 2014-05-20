/*
 * communicationModule.h
 *
 * Created: 4/23/2014 10:30:27 AM
 *  Author: Susanna Bäckman
 */ 


#ifndef COMMUNICATIONMODULE_H_
#define COMMUNICATIONMODULE_H_


/*--------Functions-----------*/
void initManualMode();
void toggleMode();


/*--------Variables-----------*/
volatile uint8_t automaticModeEnabled;
volatile uint8_t manualModeEnabled;
volatile uint8_t stationRightSensCounter;
volatile uint8_t stationLeftSensCounter;
volatile uint8_t lineReadingCounter;
uint8_t btSensDataCnt;
volatile uint8_t notLeftStationCounter;
volatile uint8_t notRightStationCounter;


#endif /* COMMUNICATIONMODULE_H_ */
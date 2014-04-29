/*
 * warehouseMode.h
 *
 * Created: 4/25/2014 2:50:38 PM
 *  Author: nilbe317
 */ 


#ifndef WAREHOUSEMODE_H_
#define WAREHOUSEMODE_H_

#include "hd44780_low.h"

//Booleans////////////////////////////////////////////////
_Bool streamFilled;
_Bool carryItem;
_Bool pickUpItem;
_Bool waitingForStartAbort;
_Bool waitingForEndPickup;

//////////////////////////////////////////////////////////

//Variables////////////////////////////////////////////////////////////////
uint8_t newStream[12];
uint8_t cargo[12];
uint8_t history[3][12];

struct hd44780_l_conf low_conf;
uint8_t digit;
uint8_t historySize;
/////////////////////////////////////////////////////////////////////////////


///Programs/////////////////////////////////////////////////////
void setupLCD();
void stationMode();
void pickUpMode();
void deliveryMode();
_Bool itemInHistory();
void printOnLCD(_Bool);
_Bool cargoEqualsNewStream();
void powerRFID(_Bool);
void waitForUserInputStartAbort();
void waitForUserInputEndPickup();
////////////////////////////////////////////////////////////////////



#endif /* WAREHOUSEMODE_H_ */
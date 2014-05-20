/*
 * warehouseMode.h
 *
 * Created: 4/25/2014 2:50:38 PM
 *  Author: Nils Berglund
 */ 


#ifndef WAREHOUSEMODE_H_
#define WAREHOUSEMODE_H_

#include "hd44780_low.h"

/*-----------Variables------------*/
volatile uint8_t streamFilled;
volatile uint8_t carryItem;
volatile uint8_t pickUpItem;
volatile uint8_t waitingForStartAbort;
volatile uint8_t waitingForEndPickup;
volatile uint8_t stationRightSide;
volatile uint8_t stationModeEnable;

uint8_t newStream[12];
uint8_t cargo[12];
uint8_t history[18];
volatile uint8_t finishedDrop;
struct hd44780_l_conf low_conf;
volatile uint8_t digit;
uint8_t historySize;

/*---------Functions----------------*/
void setupLCD();
void stationMode();
void pickUpMode();
void deliveryMode();
uint8_t itemInHistory();
void printOnLCD(uint8_t);
uint8_t newStreamPairsWithCargo();
void powerRFID(uint8_t);
void waitForUserInputStartAbort();
void waitForUserInputEndPickup();
void setupRFID();
void setupWarehouse();
uint8_t identifyNewStreamCatalogNumber();
uint8_t identifyCargoCatalogNumber();
void addPairToHistory();
void waitForFinishedDrop();

#endif /* WAREHOUSEMODE_H_ */
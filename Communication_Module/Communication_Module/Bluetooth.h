/*
 * Bluetooth.h
 *
 * Created: 4/25/2014 2:49:03 PM
 *  Author: nilbe317
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define ARM 2
#define WHEEL 1
#define CALINSTR 3
#define PCONINSTR 4
#define KPINSTR 5
#define KDINSTR 6

void handleData(uint8_t temp);
void bluetoothTX(uint8_t txdata);
void setupBluetoothRXTX();



uint8_t btdata; //the byte received by the AVR, transmitted from computer
volatile uint8_t waiting_for_instruction;
volatile uint8_t component;

#endif /* BLUETOOTH_H_ */
/*
 * Bluetooth.h
 *
 * Created: 4/25/2014 2:49:03 PM
 *  Author: Nils Berglund
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

/*--------Constants---------*/
#define WHEEL 1
#define ARM 2
#define CALINSTR 3
#define PCONINSTR 4
#define KPINSTR 5
#define KDINSTR 6
#define SENSINSTR 7
#define MODEINSTR 8
#define STATIONINSTR 9
#define HISTORYINSTR 10
#define CARGOINSTR 11

/*----------Variables------------*/
volatile uint8_t btData;
volatile uint8_t waitingForInstruction;
volatile uint8_t btComponent;


/*-----------Functions-----------*/
void handleData(uint8_t temp);
void bluetoothTX(uint8_t txdata);
void setupBluetoothRXTX();
void TXbluetoothInstr(uint8_t,uint8_t);

#endif /* BLUETOOTH_H_ */
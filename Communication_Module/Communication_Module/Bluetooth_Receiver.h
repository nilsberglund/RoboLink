/*
 * Bluetooth_Receiver.h
 *
 * Created: 4/25/2014 2:49:03 PM
 *  Author: nilbe317
 */ 


#ifndef BLUETOOTH_RECEIVER_H_
#define BLUETOOTH_RECEIVER_H_

#define ARM 2
#define WHEEL 1
#define CALINSTR 3
#define PCONINSTR 4

void handleData(char temp);
void handleInstruction(char instrByte);
void setupBluetoothRXTX();



uint8_t data; //the byte received by the AVR, transmitted from computer
uint8_t waiting_for_instruction;
uint8_t component;

#endif /* BLUETOOTH_RECEIVER_H_ */
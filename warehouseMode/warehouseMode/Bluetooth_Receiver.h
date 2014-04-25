/*
 * Bluetooth_Receiver.h
 *
 * Created: 4/25/2014 2:49:03 PM
 *  Author: nilbe317
 */ 


#ifndef BLUETOOTH_RECEIVER_H_
#define BLUETOOTH_RECEIVER_H_

void handleData(char temp);
void handleInstruction(char instrByte);
void setupBluetoothRXTX();



#endif /* BLUETOOTH_RECEIVER_H_ */
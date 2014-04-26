/*
 * Bluetooth_Receiver.c
 *
 * Created: 15/04/2014 12:49:29
 *  Author: Tobias
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Bluetooth_Receiver.h"
//#include "warehouseMode.h"
#include "Communication_Module.h"
#include "Master_communication.h"





// void handleData(char temp) 
// {
// 	switch(temp) {
// 		case 0x01: //START PICKUP button pressed
// 			pickUpItem = 1;
// 			waitingForStartAbort = 1;
// 		case 0x02: //END PICKUP button pressed
// 			waitingForEndPickup = 1;
// 		case 0x03: //ABORT PICKUP button pressed
// 			pickUpItem = 0;
// 			waitingForStartAbort = 1;
// 	}
// }

// void handleInstruction(char instrByte)
// {
// 	uint8_t temp;
// 	cli();	//Disable global interrupts
// 	while ( !(UCSR0A & (1<<RXC0)) ) //wait for byte to be received
// 	temp = UDR0;
// // 	switch (instrByte) {
// // 		case 0x01: //DRIVE instruction value
// // 			// TODO: send DRIVE data byte over SPI to control AVR
// // 			break;
// // 		case 0x02: //ARM instruction value
// // 			robot_arm_data = temp;
// // 			TX_arm_data();
// // 			break;
// // 		case 0x03: //CAL instruction value
// // 			// TODO: send CAL data byte over SPI to sensor AVR
// // 			break;
// // 		case 0x04:
// // 			//	handleData(temp);
// // 			break;
// 	}
// 	sei(); //Enable global interrupts again
// }

void setupBluetoothRXTX()
{
	waiting_for_instruction = 1;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0); //Enable RX0, TX0 and RX complete interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0x00;
	UBRR0L = 0x07; //Sets baudvalue in AVR to 7, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1	
}
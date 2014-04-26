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

void setupBluetoothRXTX()
{
	waiting_for_instruction = 1;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0); //Enable RX0, TX0 and RX complete interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0x00;
	UBRR0L = 0x07; //Sets baudvalue in AVR to 7, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1	
}
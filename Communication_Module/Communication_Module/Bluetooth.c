/*
 * Bluetooth.c
 *
 * Created: 15/04/2014 12:49:29
 *  Author: Tobias
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Bluetooth.h"
#include "warehouseMode.h"
#include "Communication_Module.h"
#include "Master_communication.h"


void handleData(uint8_t temp)
{
	switch(temp) {
		case 0x01: //START PICKUP button pressed
		pickUpItem = 1;
		waitingForStartAbort = 1;
		break;
		case 0x02: //END PICKUP button pressed
		waitingForEndPickup = 1;
		break;
		case 0x03: //ABORT PICKUP button pressed
		pickUpItem = 0;
		waitingForStartAbort = 1;
		break;
	}
}

void bluetoothTX(uint8_t txdata) {
	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = txdata;
}

void setupBluetoothRXTX()
{
	waiting_for_instruction = 1;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1 << RXCIE0); //Enable RX0, TX0 and RX complete interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0x00;
	UBRR0L = 0x07; //Sets baudvalue in AVR to 7, which gives baude rate 115200. baudvalue = (Fcpu/baudrate*16)-1	
}

void TXbluetoothInstr(uint8_t instr, uint8_t data)
{
	switch(instr) {
		case SENSINSTR:
		bluetoothTX(0b00000001);
		bluetoothTX(data);
		break;
		case MODEINSTR:
		bluetoothTX(0b00000010);
		bluetoothTX(data);
		break;
		case STATIONINSTR:
		bluetoothTX(0b00000011);
		bluetoothTX(data);
		break;
		case HISTORYINSTR:
		bluetoothTX(0b00000100);
		bluetoothTX(data);
		break;
		case CARGOINSTR:
		bluetoothTX(0b00000101);
		bluetoothTX(data);
		break;
	}
}
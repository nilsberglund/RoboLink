/*
* communicationModule.c
*
* Created: 4/23/2014 10:30:04 AM
*  Author: Susanna Bäckman
*/

#include <avr/io.h>
#include "communicationModule.h"
#include "masterCommunication.h"
#include <avr/interrupt.h>
#include "Bluetooth.h"
#include "warehouseMode.h"
#include "hd44780_low.h"


/*Receive function. Data is transmitted from the sensor slave. Always sensor data*/
ISR(INT1_vect)
{
	slaveSelect(SENSORSLAVE);	//slave select
	sensorData = masterRX(0x01); //sending dummy and receiving sensor data
	
	if(sensorData == 0b00001111 || sensorData == 0b00011111)
	{
		stationLeftSensCounter++;
		if (stationLeftSensCounter == 10)
		{
			stationRightSide = 1;
		}
	}	else if(sensorData == 0b01111000 || sensorData == 0b01111100)
	{
		stationRightSensCounter++;
		if(stationRightSensCounter == 10)
		{
			stationRightSide = 0;
		}
	} else
	{
		if(stationLeftSensCounter >= 10 || stationRightSensCounter >= 10)
		{
			lineReadingCounter++;
			if(lineReadingCounter == 130)
			{
				
				TIMSK0 = 0; //Interrupt disabled
				TCCR0B = 0; //Counter disabled
				
				if(stationRightSide == 0)
				{
					wheelData = 0x80;
				} else if(stationRightSide)
				{
					wheelData = 0;
				}
				TXwheelData();
				stationModeEnable = 1;
				stationRightSensCounter = 0;
				stationLeftSensCounter = 0;
				lineReadingCounter = 0;
			}
		}
		else
		{
			stationLeftSensCounter = 0;
			stationRightSensCounter = 0;
		}
		
	}
	slaveSelect(CONTROLSLAVE);
}

/*Receive function. Data is transmitted by control slave. Always data about finished drop*/
ISR(INT2_vect)
{
	slaveSelect(CONTROLSLAVE);
	finishedDrop = masterRX(0x01);
}

/* Timer interrupt routine handling sensor data receive */
ISR(TIMER0_COMPA_vect)
{
	if(stationModeEnable == 0)
	{
		RXsensorData();
	}
}

/* Timer interrupt routine handling sensor data transmission*/
ISR(TIMER0_COMPB_vect)
{
	if(stationModeEnable == 0)
	{
		TXsensorData();
		btSensDataCnt++;
		if (btSensDataCnt == 25)
		{
			TXbluetoothInstr(SENSINSTR, sensorData);
			btSensDataCnt = 0;
		}
	}
}


/* Interrupt routine for receiving Bluetooth data */
ISR(USART0_RX_vect)
{
	btData = UDR0;
	if (waitingForInstruction == 1) {
		waitingForInstruction = 0;
		if (btData == 1) {
			btComponent = WHEEL;
		}
		else if(btData == 2) {
			btComponent = ARM;
		}
		else if(btData == 3) {
			waitingForInstruction = 1;
			btComponent = CALINSTR;
			TXCalibration();
			
		}
		else if(btData == 4) {
			btComponent = PCONINSTR;
		}
		else if(btData == 5) {
			btComponent = KPINSTR;
		}
		else if(btData == 6) {
			btComponent = KDINSTR;
		}
		else if(btData == 7) { //Toggle mode instruction
			waitingForInstruction = 1;
			toggleMode();
		}
	}
	else {
		waitingForInstruction = 1;
		if (btComponent == WHEEL) {
			if(manualModeEnabled == 1) {
				wheelData = btData;
				TXwheelData();
			}
		}
		else if (btComponent == ARM) {
			armData = btData;
			TXarmData();
		}
		else if (btComponent == PCONINSTR) {
			handleData(btData);
		}
		else if (btComponent == KPINSTR) {
			Kp = btData;
			TXKpData();
		}
		else if (btComponent == KDINSTR) {
			Kd = btData;
			TXKdData();
		}
	}
}

/* When the receive of one byte from RFID card reader is complete, this interrupt will run.*/
ISR(USART1_RX_vect){
	newStream[digit] = UDR1;
	digit++;
	if (digit == 12) {
		digit = 0;
		streamFilled = 1;
		powerRFID(0);
	}
}

/*Interrupt hooked up to switch for changing between auto/manual mode*/
ISR(PCINT3_vect)
 {
 	toggleMode();
 }

int main(void)
{
	
	setupWarehouse();
	initManualMode();
	SPIinitMaster();
	setupBluetoothRXTX();
	setupRFID();
	setupLCD();
	
	while(1)
	{
		if(stationModeEnable == 1)
		{
			stationMode();
		}
	}
}

/*Function that initiates manual mode*/
void initManualMode()
{
	DDRD &= ~(1 << DDD6);
	PCICR = 0x08; //sets PCINT31..24 as possible external interrupt ports
	PCMSK3 = 0x40; //enables external interrupt on PORT PCINT30   - pin 20
	automaticModeEnabled = 0;
	manualModeEnabled = 1;
	stationLeftSensCounter = 0;
	stationRightSensCounter = 0;
	lineReadingCounter = 0;
	btSensDataCnt = 0;
}

/*Toggles between manual and auto mode*/
void toggleMode()
{
	if(automaticModeEnabled == 0)
	{
		automaticModeEnabled = 1;
		manualModeEnabled = 0;
		
		TIMSK0 = 6; //Enable interrupts for receiving and transmitting sensor data
		TCCR0B = 0x04; //Enable Counter
		
		stationModeEnable = 0;
		TXbluetoothInstr(MODEINSTR, manualModeEnabled); 	//Send info about mode to GUI
	
		
	} else if (manualModeEnabled == 0)
	{
		automaticModeEnabled = 0;
		manualModeEnabled = 1;
		
		TIMSK0 = 0; //Disable interrupts
		TCCR0B = 0; // Disable counter
		
		stationModeEnable = 0;
		wheelData = 0;
		TXwheelData();
		TXbluetoothInstr(MODEINSTR, manualModeEnabled); //Send info about mode to GUI
		
	}
}
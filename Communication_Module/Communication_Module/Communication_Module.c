/*
* Communication_Module.c
*
* Created: 4/23/2014 10:30:04 AM
*  Author: susba199
*/

#include <avr/io.h>
#include "Communication_Module.h"
#include "Master_communication.h"
#include <avr/interrupt.h>
#include "Bluetooth.h"
#include "warehouseMode.h"
#include "hd44780_low.h"

ISR(INT1_vect)			//Receive function. Data is transmitted from the sensor slave
{
	Slave_Select(Sensor_Slave);	//slave select
	sensor_data = Master_RX(0x01); //sending dummy
	
	if(sensor_data == 0b00001111 || sensor_data == 0b00011111)
	{
		stationLeftSensCounter++;
		if (stationLeftSensCounter == 10)
		{
			TIMSK0 = 0;
			wheel_steering_data = 0;
			TX_wheel_data();
			stationRightSide = 1;
			stationLeftSensCounter = 0;
			stationModeEnable = 1;
		}
	}	else if(sensor_data == 0b01111000 || sensor_data == 0b01111100)
	{
		stationRightSensCounter++;
		if(stationRightSensCounter == 10)
		{
			TIMSK0 = 0;
			wheel_steering_data = 0;
			TX_wheel_data();
			stationRightSide = 0;
			stationRightSensCounter = 0;
			stationModeEnable = 1;
		}
	} else 
	{
		stationLeftSensCounter = 0;
		stationRightSensCounter = 0;
		bluetoothTX(sensor_data);
	}
	//Slave_Select(Control_Slave);
}

ISR(INT2_vect)
{
	Slave_Select(Control_Slave);
	finishedDrop = Master_RX(0x01);
}

/* Timer interrupt routine handling sensor data receive */
ISR(TIMER0_COMPA_vect)
{
	if(stationModeEnable == 0)
	{
		RX_sensor_data();
	}
}

/* Timer interrupt routine handling sensor data transmission*/
ISR(TIMER0_COMPB_vect)
{
	if(stationModeEnable == 0)
	{
		TX_sensor_data();
		bluetoothTX(sensor_data);
	}
}


/* Interrupt routine for receiving bluetooth data */
ISR(USART0_RX_vect)
{
	btdata = UDR0;
	if (waiting_for_instruction == 1) {
		waiting_for_instruction = 0;
		if (btdata == 1) {
			component = WHEEL;
		}
		else if(btdata == 2) {
			component = ARM;
		}
		else if(btdata == 3) {
			component = CALINSTR;
		}
		else if(btdata == 4) {
			component = PCONINSTR;
		} 
		else if(btdata == 5) {
			component = KPINSTR;
		}
		else if(btdata == 6) {
			component = KDINSTR;
		} 
		else if(btdata == 7) { //Toggle mode instruction
			waiting_for_instruction = 1;
			toggleMode();
		}
	}
	else {
		waiting_for_instruction = 1;
		if (component == WHEEL) {
			if(manualModeEnabled == 1) {
			wheel_steering_data = btdata;
			TX_wheel_data();
			}
		}
		else if (component == ARM) {
			robot_arm_data = btdata;
			TX_arm_data();
		}
		else if (component == CALINSTR) {
			//calibration();	
		}
		else if (component == PCONINSTR) {
			handleData(btdata);
		}
		else if (component == KPINSTR) {
			Kp = btdata;
			TXKpData();
		}
		else if (component == KDINSTR) {
			Kd = btdata;
			TXKpData();
		}
	}
}

/*
When the receive of one byte is complete, this interrupt will run.
*/
ISR(USART1_RX_vect){
	newStream[digit] = UDR1;
	digit++;
	if (digit == 12) {
		digit = 0;
		streamFilled = 1;
		powerRFID(0);
	}
}

// ISR(PCINT3_vect)
// {
// 	toggleMode();
// }

int main(void)
{
	
	setupWarehouse();
	initManualMode();
	SPI_Init_Master();
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
	//PCICR = 0x08; //sets PCINT31..24 as possible external interrupt port
	//PCMSK3 = 0x40; //enables external interrupt on PORT PCINT30   - pin 20
	automaticModeEnabled = 0;
	manualModeEnabled = 1;
	stationLeftSensCounter = 0;
	stationRightSensCounter = 0;
}

void toggleMode()
{
	if(automaticModeEnabled == 0)
	{
		automaticModeEnabled = 1;
		manualModeEnabled = 0;
		TIMSK0 = 0x06;
		
	} else if (manualModeEnabled == 0)
	{
		automaticModeEnabled = 0;
		manualModeEnabled = 1;
		TIMSK0 = 0;
	}
}
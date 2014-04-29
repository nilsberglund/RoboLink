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

ISR(INT1_vect)			//Receive function. Data is transmitted from the control slave
{
	Slave_Select(Sensor_Slave);	//slave select
	sensor_data = Master_RX(0x01); //sending dummy
	if(sensor_data == 0b00001111 || sensor_data == 0b00011111)
	{
		TX_sensor_data();
		stationRightSide = 0;
		stationModeEnable = 1;
		//stationMode();
		//OCR0A = 0; //no compare => no sensor values.
		//OCR0B = 0;
		
	}	else if(sensor_data == 0b01111000 || sensor_data == 0b01111100)
	{
		TX_sensor_data();
		stationRightSide = 1;
		stationModeEnable = 1;
		//stationMode();
		//OCR0A = 0; //no compare => no sensor values.
		//OCR0B = 0;
		
	}
	Slave_Select(Control_Slave);
	
}

ISR(INT2_vect)
{
	PORTB &= ~(1 << PORTB4);
	//case wanted data : 0x01 = wheel, 0x10 = arm, 0x11 = RFID, 0x02 = sensor_data
	if(wanted_data == wr)
	{
		wheel_steering_data = Master_RX(0x01);
	}
	else if(wanted_data == ar)
	{
		robot_arm_data = Master_RX(0x01);
	}
}

ISR(TIMER0_COMPA_vect)
{
	RX_sensor_data();
}

ISR(TIMER0_COMPB_vect)
{
	TX_sensor_data();
	//bluetoothTX(sensor_data);
}

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
	}
	else {
		waiting_for_instruction = 1;
		if (component == WHEEL) {
			wheel_steering_data = btdata;
			TX_wheel_data();
		}
		else if (component == ARM) {
			robot_arm_data = btdata;
			TX_arm_data();
		}
		else if (component == CALINSTR) {
			
		}
		else if (component == PCONINSTR) {
			handleData(btdata);
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

int main(void)
{
	SPI_Init_Master();
	setupBluetoothRXTX();
	setupRFID();
	setupLCD();
	setupWarehouse();
	
	while(1)
	{
		if(stationModeEnable == 1)
		{
			stationMode();
		}
	}
}




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
#include "Bluetooth_Receiver.h"

ISR(INT1_vect)			//Receive function. Data is transmitted from the control slave
{
	Slave_Select(Sensor_Slave);	//slave select
	sensor_data = Master_RX(0x01);	//sending dummy
	Slave_Select(Control_Slave);
	//received = 1;
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
}

ISR(USART0_RX_vect)
{
	data = UDR0;
	if (waiting_for_instruction == 1)
	{
		waiting_for_instruction = 0;
		if (data == 1)
		{
			component = WHEEL;
		}else if(data == 2)
		{
			component = ARM;
		}else if(data == 3)
		{
			component = CALINSTR;
		}else if(data == 4)
		{
			component = PCONINSTR;
		}
	}else
	{
		waiting_for_instruction = 1;
		if (component == WHEEL)
		{
			
		}else if (component == ARM)
		{
			robot_arm_data = data;
			TX_arm_data();
		}else if (component == CALINSTR)
		{
			
		}else if (component == PCONINSTR)
		{
			
		}
	}
	
}

int main(void)
{
	SPI_Init_Master();
	setupBluetoothRXTX();
	
	while(1)
	{
		
	}
}




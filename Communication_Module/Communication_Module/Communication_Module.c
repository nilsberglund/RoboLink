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

ISR(INT1_vect)			//Receive function. Data is transmitted from the control slave
{
	Slave_Select(Sensor_Slave);	//slave select
	sensor_data = Master_RX(0x01);	//sending dummy
	Slave_Select(Control_Slave);
	received = 1;
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

int main(void)
{
	SPI_Init_Master();
	sei();
	
    while(1)
    {
		RX_sensor_data();
		while(received == 0)
		{
			
		}
		TX_sensor_data();
		received = 0;
    }
}
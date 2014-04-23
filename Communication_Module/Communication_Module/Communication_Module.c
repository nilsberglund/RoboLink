/*
 * Communication_Module.c
 *
 * Created: 4/23/2014 10:30:04 AM
 *  Author: susba199
 */ 


#include <avr/io.h>
#include "Communication_Module.h"
#include "Master_communication.h"

int main(void)
{
	SPI_Init_Master();
    while(1)
    {
     RX_sensor_data();
	 TX_sensor_data();
    }
}
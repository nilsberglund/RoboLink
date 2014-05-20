/*
 * sensorModule.c
 *
 * Created: 4/23/2014 9:06:18 AM
 *  Author: susba199
 */ 


#include <avr/io.h>
#include "sensorModule.h"
#include "sensorHandler.h"
#include "slaveSensor.h"

int main(void)
{
	SPIInitSlave();
	initADC();
	analogRead(0);
	
    while(1)
    {
      
    }
}
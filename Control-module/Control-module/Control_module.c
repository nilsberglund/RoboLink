/*
 * Control_module.c
 *
 * Created: 4/23/2014 9:05:12 AM
 *  Author: NISSE B
 */ 

#include <avr/io.h>
#include "Slave_control.h"
#include "Control_wheel_steering.h"
#include <avr/interrupt.h>


int main(void)
{
	SPI_Init_Slave();
	driving_setup();
	//init_arm()
	//init_LCD()
	
    while(1)
    {
        controlAlgorithm(); 
    }
}
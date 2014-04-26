/*
 * Master.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Master_communication.h"
#include "Bluetooth_Receiver.h"

/* Initializes sensor AVR as master. Sets ports and registers and enables interrupts */
void SPI_Init_Master()
{

	//Sets MOSI, SCK and SS as outputs
	DDRB = 0xB8;
	DDRD &= ~(1<<PORTD3);
	PORTB |= (1 << PORTB3)|(1 << PORTB4);
	//Sets the SPI-control register. Master settings and interrupt enable. SPR0, SPR1 sets clock to f/128.. 
	SPCR |= (1 << SPE)|(1 << MSTR)|(1 << SPR0)|(1 << SPR1);
	//Enables interrupt 2
	EICRA = 0b00111100;
	EIMSK = 0b00000110;
	//Enable global interrupt
	sei();
	
	TCCR0A = 0;
	TCCR0B = 0x05;
	TIMSK0 = 0x06;
	
	Sensor_Slave = 1;
	Control_Slave = 2;
	No_Slave = 0;
	ss = 1;
	sr = 2;
	ws = 3;
	wr = 4;
	as = 5;
	ar = 6;
	rs = 7;
	//received = 0;
	
	OCR0A = 122;
	OCR0B = 125;
	TCNT0 = 0;
	
			
}

//Master transmission to slave
void Master_TX(volatile uint8_t data)
{
		/* Start transmission */

		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));

}

uint8_t Master_RX(volatile uint8_t data){

		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
		
		return SPDR;
}
		



//Selects slave. PORTB4 = Control_Slave, PORTB3 = Sensor_Slave
void Slave_Select(volatile uint8_t slave)
{
	if(slave == Control_Slave)
	{
		PORTB |= (1 << PORTB3);
		PORTB &= ~(1 << PORTB4);

	}
	else if (slave == Sensor_Slave)
	{
		PORTB |= (1 << PORTB4);
		PORTB &= ~(1 << PORTB3);
		
	}
	else
	{
		PORTB |= (1 << PORTB4);
		PORTB |= (1 << PORTB3);
	}
}

// a = arm, s = sensor data, w = wheel data, r = rfid data
void TX_Protocol(uint8_t component)
{

	if(component == ss)
	{
		Master_TX(0b10000100);
	}
	else if(component == ws)
	{
		Master_TX(0b10000101);
	}
	else if(component == as)
	{
		Master_TX(0b10000110);
	}
	else if(component == sr)
	{
		Master_TX(0b00000100);
	}
	else if(component == wr)
	{
		Master_TX(0b00000101);
	}
	else if(component == ar)
	{
		Master_TX(0b00000110);
	}
	else if(component == rs)
	{
		Master_TX(0b10101011);
	}
}


/* Function that transmits sensor data to the control slave. */
void TX_sensor_data()
{
	Slave_Select(Control_Slave);
	TX_Protocol(ss);
	Slave_Select(No_Slave);
	Slave_Select(Control_Slave);
	Master_TX(sensor_data);
}


/* Function that tells the sensor slave to transmit sensor data. */
void RX_sensor_data()
{
	wanted_data = sr;
	Slave_Select(Sensor_Slave);
	TX_Protocol(sr);
	Slave_Select(No_Slave);
}
 
 void TX_wheel_data()
 {
	 Slave_Select(Control_Slave);
	 TX_Protocol(ws);
	 Master_TX(wheel_steering_data);
 }
 
 void RX_wheel_data()
 {
	 wanted_data = wr;
	 Slave_Select(Control_Slave);
	 TX_Protocol(wr);
	 Slave_Select(No_Slave);
 }
 
 void TX_arm_data()
{
	Slave_Select(Control_Slave);
	TX_Protocol(as);
	Master_TX(robot_arm_data);
}
void RX_arm_data()
{
	wanted_data = ar;
	Slave_Select(Control_Slave);
	TX_Protocol(ar);
	Slave_Select(No_Slave);
}

void TX_RFID_data()
{
	Slave_Select(Control_Slave);
	TX_Protocol(rs);
	for(int i = 0; i < 10; i++)
	{
		Master_TX(RFID[i]);
	}
}



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
#include "Bluetooth.h"
#include "warehouseMode.h"

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
	
	sei();//Enable global interrupt
	
	TCCR0A = 0; //Sets Timer register
	TCCR0B = 0x04; //Clk prescaler set to 256 and start counting
	
	OCR0A = 120; //Sets compare register for sensor receive interrupt to 122
	OCR0B = 130; //Sets compare register for sensor send interrupt to 125
	TCNT0 = 0;	//Initiates timer
}

//Master transmission to slave
void masterTX(volatile uint8_t data)
{
		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
}

uint8_t masterRX(volatile uint8_t data){

		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
		
		return SPDR;
}
		
//Selects slave. PORTB4 = CONTROLSLAVE, PORTB3 = SENSORSLAVE
void slaveSelect(volatile uint8_t slave)
{
	if(slave == CONTROLSLAVE)
	{
		PORTB |= (1 << PORTB3);
		PORTB &= ~(1 << PORTB4);

	}
	else if (slave == SENSORSLAVE)
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
void TXprotocol(uint8_t component)
{

	if(component ==SENSORDATASEND)
	{
		masterTX(0b10000100);
	}
	else if(component == DRIVEINSTRSEND)
	{
		masterTX(0b10000101);
	}
	else if(component == ARMINSTRSEND)
	{
		masterTX(0b10000110);
	}
	else if(component == SENSORDATARECEIVE)
	{
		masterTX(0b00000100);
	}
	else if(component == KPSEND)
	{
		masterTX(0b10000111);
	}
	else if(component == KDSEND)
	{
		masterTX(0b10001011);
	}
	else if(component == DROPITEMSEND)
	{
		masterTX(0b10001111);
	}
	else if(component == CALIBRATION)
	{
		masterTX(0b10011111);
	}
}

/* Function that transmits sensor data to the control slave. */
void TXsensorData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(SENSORDATASEND); 
	slaveSelect(NOSLAVE);
	slaveSelect(CONTROLSLAVE);
	masterTX(sensorData);
}

/* Function that tells the sensor slave to transmit sensor data. */
void RXsensorData()
{
	wantedData = SENSORDATARECEIVE;
	slaveSelect(SENSORSLAVE);
	TXprotocol(SENSORDATARECEIVE);
	slaveSelect(NOSLAVE);
}
 
 void TXwheelData()
 {
	 slaveSelect(CONTROLSLAVE);
	 TXprotocol(DRIVEINSTRSEND);
	 masterTX(wheelData);
	 slaveSelect(NOSLAVE);
 }
 
 void TXarmData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(ARMINSTRSEND);
	masterTX(armData);
}

void TXKpData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(KPSEND);
	masterTX(Kp);
	slaveSelect(NOSLAVE);
}

void TXKdData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(KDSEND);
	masterTX(Kd);
	slaveSelect(NOSLAVE);
}

void TXDropItem()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(DROPITEMSEND);
	masterTX(stationRightSide);
	slaveSelect(NOSLAVE);
}

void TXCalibration()
{
	slaveSelect(SENSORSLAVE);
	TXprotocol(CALIBRATION);
	slaveSelect(NOSLAVE);
}


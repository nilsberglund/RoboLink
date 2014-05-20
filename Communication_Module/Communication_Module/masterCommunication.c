/*
 * masterCommunication.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: Albin Ålander
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "masterCommunication.h"
#include "Bluetooth.h"
#include "warehouseMode.h"

/* Initializes sensor AVR as master. Sets ports and registers and enables interrupts */
void SPIinitMaster()
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
	TCCR0B = 0x04; //Clk prescaler set to 1024 and start counting
	
	OCR0A = 100; //Sets compare register for sensor receive interrupt to 122
	OCR0B = 150; //Sets compare register for sensor send interrupt to 125
	TCNT0 = 0;	//Initiates timer
}

/*Master transmission to slave*/
void masterTX(volatile uint8_t data)
{
		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
}

/*Master receive from slave. data should be a dummy byte*/
uint8_t masterRX(volatile uint8_t data){

		/* Start transmission */
		SPDR = data;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
		
		return SPDR;
}
		
/*Selects slave. PORTB4 = CONTROLSLAVE, PORTB3 = SENSORSLAVE*/
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

/*Sends an instruction byte based on instructions*/
void TXprotocol(uint8_t instruction)
{

	if(instruction ==SENSORDATASEND)
	{
		masterTX(0b10000100);
	}
	else if(instruction == DRIVEINSTRSEND)
	{
		masterTX(0b10000101);
	}
	else if(instruction == ARMINSTRSEND)
	{
		masterTX(0b10000110);
	}
	else if(instruction == SENSORDATARECEIVE)
	{
		masterTX(0b00000100);
	}
	else if(instruction == KPSEND)
	{
		masterTX(0b10000111);
	}
	else if(instruction == KDSEND)
	{
		masterTX(0b10001011);
	}
	else if(instruction == DROPITEMSEND)
	{
		masterTX(0b10001111);
	}
	else if(instruction == CALIBRATION)
	{
		masterTX(0b10011111);
	}
	else if(instruction == LEAVESTATIONINSTR)
	{
		masterTX(0b10010000);
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
	slaveSelect(SENSORSLAVE);
	TXprotocol(SENSORDATARECEIVE);
	slaveSelect(NOSLAVE);
}
 
 /*Transmits wheelData to control AVR*/
 void TXwheelData()
 {
	 slaveSelect(CONTROLSLAVE);
	 TXprotocol(DRIVEINSTRSEND);
	 masterTX(wheelData);
	 slaveSelect(NOSLAVE);
 }
 
 
 /*Transmits armData to control AVR*/
 void TXarmData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(ARMINSTRSEND);
	masterTX(armData);
}


/*Transmits Kp value to control AVR*/
void TXKpData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(KPSEND);
	masterTX(Kp);
	slaveSelect(NOSLAVE);
}


/*Transmits Kd value to control AVR*/
void TXKdData()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(KDSEND);
	masterTX(Kd);
	slaveSelect(NOSLAVE);
}


/*Tells control AVR to drop the item*/
void TXDropItem()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(DROPITEMSEND);
	masterTX(stationRightSide);
	slaveSelect(NOSLAVE);
}


/*Tells sensor AVR to perform a calibration*/
void TXCalibration()
{
	slaveSelect(SENSORSLAVE);
	TXprotocol(CALIBRATION);
	slaveSelect(NOSLAVE);
}


/*Tells control AVR to leave station and start line following*/
void TXleaveStation()
{
	slaveSelect(CONTROLSLAVE);
	TXprotocol(LEAVESTATIONINSTR);
	slaveSelect(NOSLAVE);
}


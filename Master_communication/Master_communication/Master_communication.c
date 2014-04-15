/*
 * Master.c
 *
 * Created: 3/30/2014 3:27:11 PM
 *  Author: albal214
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/* Variables */ 
uint8_t received_data;
uint8_t sensor_data;
uint8_t wheel_steering_data;
uint8_t	robot_arm_data;

uint8_t Sensor_Slave = 1;
uint8_t Control_Slave = 2;
uint8_t No_Slave = 0;
uint8_t ss = 1;
uint8_t sr = 2;
uint8_t ws = 3;
uint8_t wr = 4;
uint8_t as = 5;
uint8_t ar = 6;
uint8_t rs = 7;
uint8_t wanted_data;
unsigned char RFID[10];
uint8_t i;
uint8_t dummy;
 
/* Functions */
void SPI_Init_Master();
uint8_t Master_RX(uint8_t);
void RX_sensor_data();
void TX_sensor_data();
void Slave_Select(uint8_t);
void TX_Protocol(uint8_t);
void TX_arm_data();
void RX_arm_data();
void TX_wheel_data();
void Master_TX(uint8_t);

void RX_wheel_data();
void TX_RFID_data();


//Receive function. Data is transmitted from the sensor slave.
ISR(INT1_vect)
{
	PORTB &= ~(1 << PORTB3);		//slave select
	sensor_data = Master_RX(0x01);	//sending dummy
}

//Receive function. Data is transmitted from the control slave
ISR(INT2_vect)
{
	PORTB &= ~(1 << PORTB4);
	//case wanted data : 0x01 = wheel, 0x10 = arm, 0x11 = RFID, 0x02 = sensor_data
	if(wanted_data == wr)
	{
		wheel_steering_data = Master_RX(0x01);
	}
	else if(wanted_data == sr)
	{
		sensor_data = Master_RX(0x01);
	}
	else if(wanted_data == ar)
	{
		robot_arm_data = Master_RX(0x01);
	}
}

int main(void)
{
	SPI_Init_Master();
	PORTB |= (1 << PORTB4);


    while(1)
    {
	//RX_wheel_data();
	TX_sensor_data();
	RX_wheel_data();
    }
}





/* Initializes sensor AVR as master. Sets ports and registers and enables interrupts */
void SPI_Init_Master()
{
	//Sets all the D ports as outputs
	DDRD = 0xFF;
	//Sets MOSI, SCK and SS as outputs
	DDRB = 0xB0;
	PORTB |= (1 << PORTB3)|(1 << PORTB4);
	//Sets the SPI-control register. Master settings and interrupt enable. SPR0, SPR1 sets clock to f/128.. 
	SPCR |= (1 << SPE)|(1 << MSTR)|(1 << SPR0)|(1 << SPR1);
	//Enables interrupt 2
	EICRA = 0x30;
	EIMSK = 0x04;
	//Enable global interrupt
	sei();
	
			
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
void Slave_Select(uint8_t slave)
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
	sensor_data = 0xAA;
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



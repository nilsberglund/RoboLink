#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hd44780_low.h"
#include "Bluetooth_Receiver.h"
#include "warehouseMode.h"

//Booleans////////////////////////////////////////////////
_Bool streamFilled = 0;
_Bool carryItem = 0;
_Bool pickUpItem = 0;
_Bool waitingForStartAbort = 0;
_Bool waitingForEndPickup = 0;

//////////////////////////////////////////////////////////

//Variables////////////////////////////////////////////////////////////////
uint8_t newStream[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t cargo[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t history[3][12] = {{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D},{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D},{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D}};

uint8_t digit = 0;
uint8_t historySize = 0;
/////////////////////////////////////////////////////////////////////////////

/*
When the receive of one byte is complete, this interrupt will run.
*/
ISR(USART0_RX_vect){
	newStream[digit] = UDR0;
	digit++;
	if (digit == 12) {
		digit = 0;
		streamFilled = 1;
		powerRFID(0);
	}
}

int main(void)
{
	setupLCD();
	//Fix rfid setup
	
	
	
	while(1)
	{
		//transportMode();
	}
}


/*Called by transportMode(). Reads rfid tag and enters pickupMode() or deliveryMode() if the robot is carrying object or not */
void stationMode(){
	powerRFID(1);
	
	while (streamFilled == 0)
	{
		//Do nothing and wait for interupts -> do not leave loop unitil entire newStream is filled;
	}
	
	if (carryItem == 0)
	{
		pickUpMode();
	}
	else
	{
		deliveryMode();
	}
}


/*Called by pickupMode(). Waiting for the user to press either START PICKUP or ABORT PICKUP */
void waitForUserInputStartAbort()
{
	while (waitingForStartAbort == 0) {
		
	}
	waitingForStartAbort = 0;
}


/*Called by pickupMode(). Waiting for the user to finish pick up and press END PICKUP */
void waitForUserInputEndPickup()
{
	while (waitingForEndPickup == 0) {
		
	}
	waitingForEndPickup = 0;
}


/*Called by stationMode(). Enters if not carrying object.*/
void pickUpMode(){
	if (itemInHistory() == 1) //checks if mission completed for this station
	{
		// Do nothing -> exit code -> leave station mode
	}
	else
	{
		waitForUserInputStartAbort();
		
		if (pickUpItem == 1)
		{
			for (uint8_t cntDigit = 0; cntDigit<12; cntDigit++) //Storing RFID tag in cargo
			{
				cargo[cntDigit] = newStream[cntDigit];
			}
			carryItem = 1; // Shows that the robot is carrying an object
			printOnLCD(1); //Printing cargo RFID tag on display
			waitForUserInputEndPickup(); // the item has been pickup up and leave stationMode()
		}
		else
		{
			// Do nothing -> exit code -> leave station mode
		}
	}
}



/*Called by stationMode(). Enters if carrying object. */
void deliveryMode(){
	
	if (cargoEqualsNewStream() == 1) //Checks if the station is the correct station to leave cargo
	{
		for (uint8_t cntDigit = 0; cntDigit < 12; cntDigit++) //Adds the completed station to history
		{
			history[historySize][cntDigit] = cargo[cntDigit];
		}
		historySize++;
		
		printOnLCD(0); //Prints "No Cargo" on LCD
		carryItem = 0;
		
		
		//stationDirection = global variabel som beskriver sida om tejp f�r station. tilldelas egentligen s� fort man st�tt p� en station
		//Skicka kommando till styr-AVR och kalla p� dropItem(stationDirection)
		
	}
	else
	{
		// Do nothing -> exit code -> leave station mode
	}
}

/*Called by pickupMode(). */
_Bool itemInHistory(){
	uint8_t cntEqualElements = 0;
	
	for (uint8_t cntHistory = 0; cntHistory < historySize; cntHistory++) //Stegar igenom historiken
	{
		for (uint8_t cntDigit = 0; cntDigit < 12; cntDigit++) //kollar varje digit
		{
			if (newStream[cntDigit] == history[cntHistory][cntDigit])
			{
				cntEqualElements++;
			}
			
			if (cntEqualElements == 12)
			{
				return 1;
			}
			
		}
		cntEqualElements = 0;
	}
	
	return 0; //If no
}

_Bool cargoEqualsNewStream(){
	for (uint8_t cmpel = 0; cmpel<12; cmpel++)
	{
		if (cargo[cmpel] != newStream[cmpel])
		{
			return 0;
		}
	}

	return 1;
}

void powerRFID(_Bool power){
	if (power == 1)
	{
		//Activate port
	}
	else
	{
		//Deactivate port
	}
}

/*
Prints the entire RFID tag on the display.
*/
void printOnLCD(_Bool shipment){ //Eventuellt g�ra generisk om vi vill skicka in andra v�rden �n cargo
	hd44780_l_clear_disp(&low_conf);
	
	if (shipment == 1)
	{
		hd44780_l_write(&low_conf, 0x52);	//R
		hd44780_l_write(&low_conf, 0x46);	//F
		hd44780_l_write(&low_conf, 0x49);	//I
		hd44780_l_write(&low_conf, 0x44);	//D
		hd44780_l_write(&low_conf, 0x3A);	//:

		
		for(int i = 1; i<11; i++)	//Prints the tag's ID bytes.
		{
			hd44780_l_write(&low_conf, cargo[i]);
		}
	}
	else
	{
		hd44780_l_write(&low_conf, 0x4E);	//N
		hd44780_l_write(&low_conf, 0x4F);	//O
		hd44780_l_write(&low_conf, 0x20);	//_
		hd44780_l_write(&low_conf, 0x43);	//C
		hd44780_l_write(&low_conf, 0x41);	//A
		hd44780_l_write(&low_conf, 0x52);	//R
		hd44780_l_write(&low_conf, 0x47);	//G
		hd44780_l_write(&low_conf, 0x4F);	//O

	}
	

}


void setupLCD(){
	// setting I/O configuration for pins
	DDRA = 0xFF; //data outputs to the LCD
	DDRB = (1 << DDB2)|(1 << DDB1)|(1 << DDB0); //rs, rw and en are outputs
	// setting pin numbers and which ports on the LCD the ports on the AVR are hooked up to
	low_conf.rs_i = 2;
	low_conf.rw_i = 1;
	low_conf.en_i = 0;
	
	low_conf.db7_i = 7;
	low_conf.db6_i = 6;
	low_conf.db5_i = 5;
	low_conf.db4_i = 4;
	low_conf.db3_i = 3;
	low_conf.db2_i = 2;
	low_conf.db1_i = 1;
	low_conf.db0_i = 0;
	low_conf.rs_port = &PORTB;
	low_conf.rw_port = &PORTB;
	low_conf.en_port = &PORTB;
	low_conf.db7_port = &PORTA;
	low_conf.db6_port = &PORTA;
	low_conf.db5_port = &PORTA;
	low_conf.db4_port = &PORTA;
	low_conf.db3_port = &PORTA;
	low_conf.db2_port = &PORTA;
	low_conf.db1_port = &PORTA;
	low_conf.db0_port = &PORTA;
	low_conf.line1_base_addr = 0x00;
	low_conf.line2_base_addr = 0x40;
	low_conf.dl = HD44780_L_FS_DL_8BIT;
	hd44780_l_init(&low_conf, HD44780_L_FS_N_DUAL, HD44780_L_FS_F_58, HD44780_L_EMS_ID_INC, HD44780_L_EMS_S_OFF);
	hd44780_l_disp(&low_conf, HD44780_L_DISP_D_ON, HD44780_L_DISP_C_OFF, HD44780_L_DISP_B_OFF);
	UCSR0B = (1<<RXEN0 | 1<<TXEN0); //Enable RX0 and TX0
	UCSR0C = (1 << UCSZ01 | 1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0b00000000;
	UBRR0L = 0b00011001; //Sets baudvalue in AVR to 25(1mhz), which gives baude rate 2400. baudvalue = (Fcpu/baudrate*16)-1
	UCSR0B |= (1 << RXCIE0); //Enables the rc complete interrupt
	
}
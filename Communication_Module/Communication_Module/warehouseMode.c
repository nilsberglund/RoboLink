#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hd44780_low.h"
#include "Bluetooth.h"
#include "warehouseMode.h"
#include "masterCommunication.h"
#include <stdlib.h>
#include <math.h>

uint8_t newStream[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t cargo[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t history[18] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
uint8_t catalog[18][10] = {{0x30, 0x31, 0x30, 0x30, 0x45, 0x32, 0x42, 0x33, 0x37, 0x42}, {0x30, 0x31, 0x30, 0x30, 0x45, 0x32, 0x44, 0x44, 0x44, 0x33}, {0x30, 0x31, 0x30, 0x30, 0x45, 0x31, 0x38, 0x36, 0x46, 0x35}, {0x30, 0x31, 0x30, 0x30, 0x45, 0x31, 0x33, 0x42, 0x39, 0x42}, {0x30, 0x31, 0x30, 0x30, 0x45, 0x37, 0x31, 0x31, 0x32, 0x41}, {0x30, 0x31, 0x30, 0x30, 0x45, 0x31, 0x32, 0x39, 0x43, 0x41}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x42, 0x39, 0x31, 0x36, 0x41}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x42, 0x32, 0x37, 0x43, 0x34}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x38, 0x41, 0x44, 0x30, 0x45}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x38, 0x41, 0x42, 0x38, 0x42}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x33, 0x44, 0x38, 0x32, 0x46}, {0x32, 0x36, 0x30, 0x30, 0x44, 0x33, 0x44, 0x42, 0x42, 0x38}, {0x30, 0x31, 0x30, 0x30, 0x41, 0x37, 0x39, 0x34, 0x39, 0x35}, {0x30, 0x31, 0x30, 0x30, 0x41, 0x37, 0x38, 0x42, 0x46, 0x43}, {0x30, 0x31, 0x30, 0x30, 0x41, 0x37, 0x41, 0x34, 0x39, 0x36}, {0x30, 0x31, 0x30, 0x30, 0x41, 0x37, 0x41, 0x44, 0x36, 0x44}, {0x32, 0x37, 0x30, 0x30, 0x42, 0x38, 0x45, 0x30, 0x39, 0x43}, {0x32, 0x37, 0x30, 0x30, 0x42, 0x39, 0x31, 0x34, 0x36, 0x39}};

/*Called by transportMode(). Reads rfid tag and enters pickupMode() or deliveryMode() if the robot is carrying object or not */
void stationMode(){
	
	streamFilled = 0;
	powerRFID(1);
	
	while (streamFilled == 0)//Do nothing and wait for interrupts -> do not leave loop unitil entire newStream is filled;
	{
		if(stationModeEnable == 0)//Check if we have entered manualmode
		{
			powerRFID(0);
			break;
		}
	}
	streamFilled = 0;
	if(stationModeEnable == 1)
	{
		if (carryItem == 0)
		{
			pickUpMode();
		}
		else
		{
			deliveryMode();
		}
		
		//TXleaveStation();
		TIMSK0 = 6; //Enable interrupts
		TCCR0B = 0x04; //Start counter
		
	}
	stationModeEnable = 0;
	
}



/*Called by stationMode(). Enters if not carrying object.*/
void pickUpMode(){
	if (itemInHistory() == 1) //checks if mission completed for this station
	{
		TXbluetoothInstr(STATIONINSTR, identifyNewStreamCatalogNumber());
		// Do nothing -> exit code -> leave station mode
	}
	else
	{
		waitForUserInputStartAbort();
		
		if (pickUpItem == 1)
		{
			for (uint8_t cntDigit = 0; cntDigit < 12; cntDigit++) //Storing RFID tag in cargo
			{
				
				cargo[cntDigit] = newStream[cntDigit];
			}
			carryItem = 1; // Shows that the robot is carrying an object
			printOnLCD(1); //Printing cargo RFID tag on display
			TXbluetoothInstr(STATIONINSTR, identifyCargoCatalogNumber());
			TXbluetoothInstr(CARGOINSTR, identifyCargoCatalogNumber());
			waitForUserInputEndPickup(); // the item has been pickup up and leave stationMode()
		}
		else
		{
			TXbluetoothInstr(STATIONINSTR, identifyNewStreamCatalogNumber());
			// Do nothing -> exit code -> leave station mode
		}
	}
}



/*Called by stationMode(). Enters if carrying object. */
void deliveryMode(){
	
	if (newStreamPairsWithCargo() == 1) //Checks if the station is the correct station to leave cargo
	{
		addPairToHistory();
		printOnLCD(0); //Prints "No Cargo" on LCD
		TXDropItem(stationRightSide); //1 = right side, 0 = left side
		waitForFinishedDrop();	
		carryItem = 0;
		TXbluetoothInstr(CARGOINSTR, 32);
		TXbluetoothInstr(STATIONINSTR, identifyNewStreamCatalogNumber());
	}
	else
	{
		TXbluetoothInstr(STATIONINSTR, identifyNewStreamCatalogNumber());
		// Do nothing -> exit code -> leave station mode
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


/*Function that waits for item to be dropped of*/
void waitForFinishedDrop()
{
	while (finishedDrop == 0) {
		
	}
	finishedDrop = 0;
}


/*Adds RIFD pair to history when they are finished*/
void addPairToHistory(){
	uint8_t tmp2Cargo = identifyCargoCatalogNumber();
	
	if (tmp2Cargo % 2 == 0) // Even indexes (cards)
	{
		history[historySize] = tmp2Cargo;
		historySize++;
		history[historySize] = tmp2Cargo + 1;
		historySize++;
		TXbluetoothInstr(HISTORYINSTR, tmp2Cargo);
		TXbluetoothInstr(HISTORYINSTR, tmp2Cargo+1);
	}
	
	else if (tmp2Cargo % 2 == 1)
	{
		history[historySize] = tmp2Cargo;
		historySize++;
		history[historySize] = tmp2Cargo - 1;
		historySize++;
		TXbluetoothInstr(HISTORYINSTR, tmp2Cargo);
		TXbluetoothInstr(HISTORYINSTR, tmp2Cargo-1);
	}
	
}

/*Called by pickupMode(). Checks if object is already delivered */
uint8_t itemInHistory(){ //lägg till funktion som drar båda i ett par över samma kant
	
	uint8_t tempCatNumber = identifyNewStreamCatalogNumber();
	
	for (uint8_t cntCatalog = 0; cntCatalog < 18; cntCatalog ++)
	{
		if (history[cntCatalog] == tempCatNumber)
		{
			return 1;
		}
		
	}
	
	return 0; //If no
}

/*Checks if new station RFID value pairs with the cargo*/
uint8_t newStreamPairsWithCargo()
{
	volatile uint8_t tmpCargo;
	volatile uint8_t tmpNewStream;
	
	 tmpCargo = identifyCargoCatalogNumber();
	 tmpNewStream = identifyNewStreamCatalogNumber();
	 
	if ((tmpCargo % 2) == 0) // cargo catalog index is even (even catalog number implies start of new pair)
	{
		if (tmpNewStream == tmpCargo + 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	else if((tmpCargo % 2) == 1) //cargo catalog index is odd (odd catalog number implies end of new pair)
	{
		if (tmpNewStream == tmpCargo - 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}


/*Gets the current station's catalog number in the catalog of RFID IDs*/
uint8_t identifyNewStreamCatalogNumber()
{
	volatile uint8_t newStreamCatalogNumber = 0;
	volatile uint8_t cntMatch = 0;
	
	for (uint8_t cntCatalog = 0; cntCatalog < 18; cntCatalog++)
	{
		for (uint8_t cntDigit = 1; cntDigit<11; cntDigit++)
		{
			if (catalog[cntCatalog][cntDigit-1] == newStream[cntDigit])
			{
				cntMatch++;
				
				if (cntMatch == 10){
					newStreamCatalogNumber = cntCatalog;
					cntMatch = 0;
				}
			}
			else
			{
				cntMatch = 0;
			}
		}
	}
	return newStreamCatalogNumber;
}


/*Gets the cargo's catalog number in the catalog of RFID IDs*/
uint8_t identifyCargoCatalogNumber()
{
	volatile uint8_t cargoCatalogNumber = 0;
	volatile uint8_t cntMatch = 0;
	
	for (uint8_t cntCatalog = 0; cntCatalog < 18; cntCatalog++)
	{
		for (uint8_t cntDigit = 1; cntDigit < 11; cntDigit++)
		{
			if (catalog[cntCatalog][cntDigit-1] == cargo[cntDigit])
			{
				cntMatch++;
				
				if (cntMatch == 10){ 
					cargoCatalogNumber = cntCatalog;
					cntMatch = 0;
				}	
			}
			else
			{
				cntMatch = 0;
			}
		}	
	}
	return cargoCatalogNumber;
}

/*Turns RFID card reader on or off*/
void powerRFID(uint8_t power){
	if (power == 1)
	{
		PORTD &= ~(1<<PORTD5);
		//((Port on AVR connected to RFIDs "Enable") = LOW) => Power up byte reader
	}
	else
	{
		PORTD |= (1<<PORTD5);
		//((Port on AVR connected to RFIDs "Enable") = HIGH) => Shut down byte reader
	}
}

/*Prints the entire RFID tag on the display.*/
void printOnLCD(uint8_t shipment) {
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
		hd44780_l_write(&low_conf, 0x20);	//
		
		hd44780_l_write(&low_conf, 0x43);	//C
		hd44780_l_write(&low_conf, 0x41);	//A
		hd44780_l_write(&low_conf, 0x52);	//R
		hd44780_l_write(&low_conf, 0x47);	//G
		hd44780_l_write(&low_conf, 0x4F);	//O

	}
	

}

/*Sets up ports and registers to communicate with RFID card reader*/
void setupRFID(){
	UCSR1B |= (1<<RXEN1) | (1 << RXCIE1); //Enable RX1 and the RX complete interrupt
	UCSR1C |= (1 << UCSZ11)|(1 << UCSZ10); //set data length to 8-bit;
	UBRR1H = (383 >> 8);
	UBRR1L = 0b01111111; //Sets baudvalue in AVR to 383, which gives baude rate 2400. baudvalue = (Fcpu/baudrate*16)-1
	DDRD |= (1<<DDD5);
	PORTD |= (1<<PORTD5);
}


/*Sets up variables for the simulated warehouse*/
void setupWarehouse(){
	streamFilled = 0;
	carryItem = 0;
	pickUpItem = 0;
	waitingForStartAbort = 0;
	waitingForEndPickup = 0;
	stationModeEnable = 0;
	digit = 0;
	historySize = 0;
	finishedDrop = 0;
}

/*Sets up the LCD display*/
void setupLCD(){
	// setting I/O configuration for pins
	DDRA = 0xFF; //data outputs to the LCD
	DDRB |= (1 << DDB1)|(1 << DDB0); //rs, rw and en are outputs
	DDRC |=	(1<< DDC6);
	// setting pin numbers and which ports on the LCD the ports on the AVR are hooked up to
	low_conf.rs_i = 1;
	low_conf.rw_i = 0;
	low_conf.en_i = 6;
	
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
	low_conf.en_port = &PORTC;
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
	hd44780_l_clear_disp(&low_conf);
}


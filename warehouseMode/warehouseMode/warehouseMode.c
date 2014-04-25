#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hd44780_low.h"

//Booleans////////////////////////////////////////////////
_Bool powerRFID = false;
_Bool streamFilled = false;
_Bool powerRFID = false;
_Bool carryItem = false;
//////////////////////////////////////////////////////////

//Variables////////////////////////////////////////////////////////////////
uint8_t newStream[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t cargo[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t history[3][12] = {{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D},{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D},{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D}};

struct hd44780_l_conf low_conf;
uint8_t digit = 0;
/////////////////////////////////////////////////////////////////////////////


///Programs/////////////////////////////////////////////////////
void setupLCD();
void pickUpMode();
void deliveryMode();
void checkHistory();

void rfid_Print_LCD();
////////////////////////////////////////////////////////////////////




/*
When the receive of one byte is complete, this interrupt will run.
*/
ISR(USART0_RX_vect){
	newStream[digit] = UDR0;
	digit++;
	if (digit == 12) {
		digit = 0;
		streamFilled = true;
		powerRFID = false;
		
		//TODO: Shut of card reader to prevent multiple readings of same card.
	}
}

int main(void)
{
	setupLCD(); 
	
	
	
	while(1)
	{
		
stationMode();
	}
}

void stationMode(){
	powerRFID = true;
	
	while (streamFilled == false)
	{
		//Do nothing and wait for interupts;
	}
	
	if (carryItem == false)
	{
		pickUpMode();
	} 
	else
	{
		deliveryMode();
	}
	
	
}



void pickUpMode(){
	if (checkHistory() == true) //styr upp formalia.. hur funnkar lokal return av bool
	{
		// Do nothing
	} 
	else
	{
		
	}
	
}

void deliveryMode(){
	
	
}


_Bool checkHistory(){
	//j�mnf�r newstream mot history.... returna true om matchning
}





/*
Prints the entire RFID tag on the display.
*/
void rfid_Print_LCD(){
	hd44780_l_clear_disp(&low_conf);
	
	hd44780_l_write(&low_conf, 0x52);	//R
	hd44780_l_write(&low_conf, 0x46);	//F
	hd44780_l_write(&low_conf, 0x49);	//I
	hd44780_l_write(&low_conf, 0x44);	//D
	hd44780_l_write(&low_conf, 0x3A);	//:

	
	for(int i = 1; i<11; i++)	//Prints the tag's ID bytes.
	{
		hd44780_l_write(&low_conf, loaded[i]);
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
	sei();
	
}
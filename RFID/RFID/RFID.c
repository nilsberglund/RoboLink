#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hd44780_low.h"

//void rfid_Print_LCD();
void rfid_Read(uint8_t digit);
void lcd_Setup();

uint8_t loaded[12] = {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D};
uint8_t history[4][12] = {{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D},{0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D}, {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D}, {0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D}};
//struct hd44780_l_conf low_conf;
uint8_t digit = 0;


/*
When the receive of one byte is complete, this interrupt will run.
*/
ISR(USART1_RX_vect){
	rfid_Read(digit);
	
	digit++;
	if (digit == 12) {
		digit = 0;
		
		//TODO: Shut of card reader to prevent multiple readings of same card.
	}
}

int main(void)
{
	lcd_Setup(); //Bör köras först av allt då den även innehåller rfid intrerupt initialisering
	//rfid_Print_LCD();
	sei();
	while(1)
	{
		
		//rfid_Print_LCD();
		//_delay_ms(300);
	}
}


/*
Reads one byte and stores in in the loaded vector.
*/
void rfid_Read(uint8_t digit){
	
	loaded[digit] = UDR0;
}

/*
Prints the entire RFID tag on the display.
*/
// void rfid_Print_LCD(){
// 	hd44780_l_clear_disp(&low_conf);
// 	
// 	hd44780_l_write(&low_conf, 0x52);	//R
// 	hd44780_l_write(&low_conf, 0x46);	//F
// 	hd44780_l_write(&low_conf, 0x49);	//I
// 	hd44780_l_write(&low_conf, 0x44);	//D
// 	hd44780_l_write(&low_conf, 0x3A);	//:
// 
// 	
// 	for(int i = 1; i<11; i++)	//Prints the tag's ID bytes.
// 	{
// 		hd44780_l_write(&low_conf, loaded[i]);
// 	}
// }


void lcd_Setup(){
	// setting I/O configuration for pins
// 	DDRA = 0xFF; //data outputs to the LCD
// 	DDRB = (1 << DDB2)|(1 << DDB1)|(1 << DDB0); //rs, rw and en are outputs
// 	// setting pin numbers and which ports on the LCD the ports on the AVR are hooked up to
// 	low_conf.rs_i = 2;
// 	low_conf.rw_i = 1;
// 	low_conf.en_i = 0;
// 	
// 	low_conf.db7_i = 7;
// 	low_conf.db6_i = 6;
// 	low_conf.db5_i = 5;
// 	low_conf.db4_i = 4;
// 	low_conf.db3_i = 3;
// 	low_conf.db2_i = 2;
// 	low_conf.db1_i = 1;
// 	low_conf.db0_i = 0;
// 	low_conf.rs_port = &PORTB;
// 	low_conf.rw_port = &PORTB;
// 	low_conf.en_port = &PORTB;
// 	low_conf.db7_port = &PORTA;
// 	low_conf.db6_port = &PORTA;
// 	low_conf.db5_port = &PORTA;
// 	low_conf.db4_port = &PORTA;
// 	low_conf.db3_port = &PORTA;
// 	low_conf.db2_port = &PORTA;
// 	low_conf.db1_port = &PORTA;
// 	low_conf.db0_port = &PORTA;
// 	low_conf.line1_base_addr = 0x00;
// 	low_conf.line2_base_addr = 0x40;
// 	low_conf.dl = HD44780_L_FS_DL_8BIT;
// 	hd44780_l_init(&low_conf, HD44780_L_FS_N_DUAL, HD44780_L_FS_F_58, HD44780_L_EMS_ID_INC, HD44780_L_EMS_S_OFF);
// 	hd44780_l_disp(&low_conf, HD44780_L_DISP_D_ON, HD44780_L_DISP_C_OFF, HD44780_L_DISP_B_OFF);
	UCSR1B = (1<<RXEN1 | 1<<TXEN1); //Enable RX0 and TX0
	UCSR1C = (1 << UCSZ11 | 1 << UCSZ10); //set data length to 8-bit;
	UBRR1H = 0b00000000;
	UBRR1L = 0b00011001; //Sets baudvalue in AVR to 25(1mhz), which gives baude rate 2400. baudvalue = (Fcpu/baudrate*16)-1
	UCSR1B |= (1 << RXCIE1); //Enables the rc complete interrupt
	
	DDRD |= (1<<DDD5);
	PORTD &= ~(1<<PORTD5);
	//PORTD |= (1<<PORTD5);
}
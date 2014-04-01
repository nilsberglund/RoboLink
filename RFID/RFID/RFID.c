/*
* RFID.c
*
* Created: 3/29/2014 11:53:59 AM
*  Author: nilbe317
*
* TODO: 
*/

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hd44780_low.h"

void rfid_Print_LCD(uint8_t digit);
void rfid_Read(uint8_t digit);

uint8_t rfidStream[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
struct hd44780_l_conf low_conf;
uint8_t digit = 0;

ISR(USART0_RX_vect){

	rfid_Read(digit);
	digit++;
	
	if (digit == 12) {
		digit = 0;
	}

}

void rfid_Print_LCD(uint8_t digit){
	
	if (digit > 0 || digit < 11 ){
		hd44780_l_write(&low_conf, rfidStream[digit]);
	}
		
	if (rfidStream[digit] == 0x0A || rfidStream[digit] == 0x0D )
	{
		hd44780_l_write(&low_conf, 0x5E);
	}
	else{
		hd44780_l_write(&low_conf, 0x21);
	}
	
	
}

void rfid_Read(uint8_t digit){
	
	rfidStream[digit] = UDR0;
	
	/*
	if (rfidStream[0] == 0x0A && rfnr == 0){
		for (int i = 1; i <= 11; i++)
		{
			rfidStream[i] = 0;
		}
	}
	
	
	rfnr++;
	if (rfnr >= 12) {
		rfnr = 0;
	}
	*/
}

int main(void)
{
	
	DDRA = 0xFF;
	DDRD = (1 << DDD5)|(1 << DDD6)|(1 << DDD7);
	

	low_conf.rs_i = 5;
	low_conf.rw_i = 6;
	low_conf.en_i = 7;
	low_conf.db7_i = 7;
	low_conf.db6_i = 6;
	low_conf.db5_i = 5;
	low_conf.db4_i = 4;
	low_conf.db3_i = 3;
	low_conf.db2_i = 2;
	low_conf.db1_i = 1;
	low_conf.db0_i = 0;
	low_conf.rs_port = &PORTD;
	low_conf.rw_port = &PORTD;
	low_conf.en_port = &PORTD;
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
	
	hd44780_l_disp(&low_conf, HD44780_L_DISP_D_ON, HD44780_L_DISP_C_ON, HD44780_L_DISP_B_ON);
	
	UCSR0B = (1<<RXEN0 | 1<<TXEN0); //Enable RX0 and TX0
	UCSR0C = (1 << UCSZ01 | 1 << UCSZ00); //set data length to 8-bit;
	UBRR0H = 0b00000001;
	UBRR0L = 0b01111111; //Sets baudvalue in AVR to 383, which gives baude rate 2400. baudvalue = (Fcpu/baudrate*16)-1
	UCSR0B |= (1 << RXCIE0);
	sei();
	
	
	
	while(1)
	{
		if (rfidStream[0] =! 0 || newVector() == true)
		{
			rfid_Print_LCD(digit);
		}
		
		
		
	}
}


boolean newVector(){
	for(int cardVectors = 0; i<=7; i++){
		for(int digit = 0; i<=11; i++){
		rfidStream[i] 
	}
	
}
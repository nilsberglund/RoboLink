
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Slave_sensor.h"
#include "SensorCalMajor.h"


/*
void initADC();
void analogRead (int ch);
void calcOneByteLineVector();
void defaultMode(); //denna plus "main"koncept måste styras upp av suspis
void calibrationMode();
void calcThresholds();
*/
/*
//////Variables//////////////
uint8_t adcValue = 0;
int ch = 0;
int caliMode = 0;
int buttonPushed = 0;

///////Values/////////////////////
uint8_t channelThresholds[7] = {100,100,100,100,100,100,100}; //Tillfälligt tillagda innan kalibreringsfunktion tillkommit vars enda syfte i lever är att uppdatera denna vid knapptryck
uint8_t newSensorValues[7];
uint8_t lightVector[7]; 
uint8_t darkVector[7]; 
uint8_t sensor_data = 0;
*/

////////////Interupts///////////////
ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
{
	adcValue = ADCL;					// Must read ADCL before ADCH
	adcValue = ADCH;					// Output ADCH to managable byte adcValue
	if (caliMode == 1)
	{
		calibrationMode();
	}
	else
	{
		defaultMode();
	}


}

ISR(INT0_vect) {						// First ADC conversion after button press
	ch = 0;
	caliMode = 1;
	buttonPushed++;
	if (buttonPushed == 3)
	{
		buttonPushed = 1;
	}
	analogRead(ch);
}


/*
///////////////??????????????///////////////////////////////
int main(void)									// borde heta initADC() sen kanske?
{
	while(1);
	{
		
	}
}
*/


void initADC() {
	ch = 0;										// Make sure that we start on first channel
	DDRA = 0x00;								// Configure PortA as input for analog readings
	DDRB |= (1<<DDB2) | (1<<DDB1) | (1<<DDB0);  // Configure PortB as output, pin PB0, PB1, PB2.
	PORTB &= ~(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2);									// MUX-address = 0 =>Tänd lampa 0,		ändra sen till bara dom lägsta bitarna.
	ADMUX = 0x20;								// AREF, left justify (msb-lsb configuration), Data registers and select ADC0 as input channel. skapa loop som växlar mellan ADC0 och ADC6.
	ADCSRA = 0x8B;								// Enable the ADC and its interrupt feature
	// and set the ACD clock pre-scalar to clk/64
	EICRA |=(1<<ISC00);							// Sets the ISC00 to 1 rising edge triggering
	EICRA |=(1<<ISC01);							// Sets ISC01 to 1
	EIMSK =0x01;
	for(int i=0; i<7; i++)
	{
			channelThresholds[i] = 180;
	}
	sei(); 
}

void analogRead (int ch){
	ADMUX &= 0xF8;									// Set 3 lsb:s to 0
	ADMUX |= ch;									// Select pin ADC0..ADC6 using MUX.
	ADCSRA |=(1<<ADSC);								// Start conversion
	//while(!(ADCSRA & (1<<ADIF)));
}


////////////////////////////Modes for sensor value handlings//////////////////////////////////////////

void defaultMode() {
	newSensorValues[ch] = adcValue;
	ch++;									//go to next channel
	if (ch==7)
	{
		calcOneByteLineVector();
		ch = 0;
	}
	
	PORTB &= 0xF8; 							
	PORTB |= ch;							//Light up new channel, GLÖM EJ måste maskas istället för att överskirvas!
	analogRead(ch);						//Read analog value on new channel
	

}

void calibrationMode() {
	if (buttonPushed == 1){ //calibrate light 
		lightVector[ch] = adcValue;	//Add values in lightVector for first calibration
	}
	
	if (buttonPushed == 2){ //calibrate dark
		darkVector[ch] = adcValue;		//Add values in darkVector for second calibration
		if (ch == 6){
			calcThresholds();
		}
	}
	
	ch++;
	if (ch == 7){								//go to next channel
		caliMode = 0;
		ch = 0;
	}
	
	PORTB &= 0xF8; 							
	PORTB |= ch;								
						//Light up new channel, GLÖM EJ måste maskas istället för att överskrivas!
	analogRead(ch);						//Read analog value on new channel
	

}



////////////////////////////Mathematical functions//////////////////////////////////////////

void calcThresholds(){
	for (int i=0; i<7; i++)
	{
		channelThresholds[i] = lightVector[i] + ((darkVector[i] - lightVector[i]) / 2);
	}
}

void calcOneByteLineVector(){
	
	//Add millis or counter
	
	sensor_data = 0;
	
	for (int i=0; i<7; i++)
	{
		if (newSensorValues[i] > channelThresholds[i])
		{
			sensor_data |= (1<<i);
		}
	}
	//Send to other avr
}

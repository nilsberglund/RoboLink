
#include <avr/io.h>
#include <avr/interrupt.h>

void initADC();
void analogRead (int ch);
void calcOneByteLineVector();
void defaultMode(); //denna plus "main"koncept måste styras upp av suspis


//////Variables//////////////
uint8_t adcValue = 0;
int ch = 0;

///////Values/////////////////////
uint8_t channelThresholds[7] = {60,60,60,60,60,60,60}; //Tillfälligt tillagda innan kalibreringsfunktion tillkommit vars enda syfte i lever är att uppdatera denna vid knaptryck
uint8_t newSensorValues[7];
char oneByteLineVector = 0;

////////////Interupts///////////////
ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
{
	adcValue = ADCL;					// Must read ADCL before ADCH
	adcValue = ADCH;					// Output ADCH to managable byte adcValue
	
	defaultMode();
}



///////////////??????????????///////////////////////////////
int main(void)									// borde heta initADC() sen kanske?
{
	initADC();
	sei(); 										// Enable Global Interrupts
	for (int i=0; i<5; i++)
	{
		analogRead(0);
	}
	while(1);
	{											//Wait for button
	}
}

////////////////////////////Interupts//////////////////////////////////////////




void initADC() {
	ch = 0;										// Make sure that we start on first channel
	DDRA = 0x00;								// Configure PortA as input for analog readings
	DDRB |= (1<<DDB2) | (1<<DDB1) | (1<<DDB0); 	// Configure PortB as output, pin PB0, PB1, PB2.
	PORTB = 0;									// MUX-address = 0 =>Tänd lampa 0,		ändra sen till bara dom lägsta bitarna.
	ADMUX = 0x20;								// AREF, left justify (msb-lsb configuration), Data registers and select ADC0 as input channel. skapa loop som växlar mellan ADC0 och ADC6.
	ADCSRA = 0x8B;								// Enable the ADC and its interrupt feature
	// and set the ACD clock pre-scalar to clk/64
	EICRA |=(1<<ISC00);							// Sets the ISC00 to 1 rising edge triggering
	EICRA |=(1<<ISC01);							// Sets ISC01 to 1
	EIMSK =0x01;
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
	if (ch==7) {
		calcOneByteLineVector();
		ch = 0;
	}
	else {
		
		PORTB = ch;							//Light up new channel, GLÖM EJ måste maskas istället för att överskirvas!
		analogRead(ch);						//Read analog value on new channel
		
	}
}



////////////////////////////Mathematical functions//////////////////////////////////////////
void calcOneByteLineVector(){
	for (int i=0; i<7; i++)
	{
		if (newSensorValues[i] > channelThresholds[i])
		{
			oneByteLineVector |= (1<<i);
		}
	}
}

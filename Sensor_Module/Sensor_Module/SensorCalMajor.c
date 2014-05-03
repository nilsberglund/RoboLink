
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Slave_sensor.h"
#include "SensorCalMajor.h"


/*-------------------------Interrupts------------------------*/

/*Interrupt that is thrown when an AD conversion is complete */ 
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

/*Interrupt that is thrown when button for calibration has been pushed.*/
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


/*Function that initializes the everything involving the AD conversion.*/ 
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
		if(i == 1 || i == 2)
		{
			channelThresholds[i] = 187;
		}
		else
		{
			channelThresholds[i] = 150;
		}
	}
	sei(); 
}

/*Starts an AD conversion on channel ch. 0<= ch <= 6). */
void analogRead (int ch){
	ADMUX &= 0xF8;									// Set 3 lsb:s to 0
	ADMUX |= ch;									// Select pin ADC0..ADC6 using MUX.
	ADCSRA |=(1<<ADSC);								// Start conversion
	//while(!(ADCSRA & (1<<ADIF)));
}


/*---------------------------------Modes for sensor value handlings-------------------------------------------*/


/*Function that handles the values from the last AD conversion. Starts a new conversion on the next channel.*/
void defaultMode() {
	newSensorValues[ch] = adcValue;
	ch++;									//go to next channel
	if (ch==7)
	{
		calcOneByteLineVector();
		ch = 0;
	}
	
	PORTB &= 0xF8; 							
	PORTB |= ch;							//Light up new channel
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
						//Light up new channel
	analogRead(ch);						//Read analog value on new channel
	

}



/*--------------------------Mathematical functions--------------------------------*/

/* Function that calculates sensor thresholds */
void calcThresholds(){
	for (int i=0; i<7; i++)
	{
		channelThresholds[i] = lightVector[i] + ((darkVector[i] - lightVector[i]) / 2);
	}
}

/* Function that reads the sensor values */
void calcOneByteLineVector(){
	
	
	sensorData = 0;
	
	for (int i=0; i<7; i++)
	{
		if (newSensorValues[i] > channelThresholds[i])
		{
			sensorData |= (1<<i);
		}
	}
}

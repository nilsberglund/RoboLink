/*
 * LineSensor.c
 *
 * Created: 4/3/2014 4:09:28 PM
 *  Author: susba199
 */ 

		#include <avr/io.h>
		#include <avr/interrupt.h>
		
		
		void initADC();
		void analogRead (int ch);
		void calibrateSensors();
		void calcThresholds();
		void waitForButton();
		
		uint8_t adcValue = 0;
		int ch = 0;
		
		uint8_t channelThresholds[7];
		// uint8_t res_tot[7];
		uint8_t darkComplete = 0;
		uint8_t darkVector[7];			//Nulla på något sätt??
		uint8_t lightVector[7];		//Nulla på något sätt??


		ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
		{
			adcValue = ADCL;					// Must read ADCL before ADCH
			adcValue = ADCH;					// Output ADCH to managable byte adcValue
			
			if (darkComplete == 0){
				darkVector[ch] = adcValue;	//Add values in darkVector for first calibration
			}
			
			if (darkComplete == 1){
				lightVector[ch] = adcValue;		//Add values in lightVector for second calibration
				if (ch == 6){
					calcThresholds();
				}
			}
			
			ch++;								//go to next channel
			
			if (ch==7) {
				darkComplete = 1;
				ch = 0;
			}
			else {
				
				PORTB = ch;							//Light up new channel, GLÖM EJ måste maskas istället för att överskirvas!
				analogRead(ch);						//Read analog value on new channel
				
			}
		}

		

		ISR(INT0_vect) {						// First ADC conversion after button press
			
			analogRead(0);
			

		}

		
		int main(void)										// borde heta initADC() sen kanske?
		{

			initADC();
			sei(); 								// Enable Global Interrupts
			
			while(1);
			{
			}

			
		}
		

		void initADC() {
			ch = 0;										// Make sure that we start on first channel
			DDRA = 0x00;								// Configure PortA as input for analog readings
			DDRB |= (1<<DDB2) | (1<<DDB1) | (1<<DDB0); 	// Configure PortB as output, pin PB0, PB1, PB2.
			PORTB = 0;									// MUX-address = 0,		ändra sen till bara dom lägsta bitarna.
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
		
		
		void calcThresholds(){
			for (int channel=0; channel<7; channel++)
			{
				channelThresholds[ch] = ((lightVector[ch] - darkVector[ch]) / 2);
			}
		}
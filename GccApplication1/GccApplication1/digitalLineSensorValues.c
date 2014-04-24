	/*	#include <avr/io.h>
		#include <avr/interrupt.h>
		
		void initADC();
		void analogRead (int ch);
		void calibrateSensors();
		static volatile uint8_t adcValue = 0;
		int ch = 0;
		uint8_t res_tot[7];
		
//		uint8_t darkCal[7];
//		uint8_t lightCal[7];


		ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
		{
			adcValue = ADCL;					// OBS! Must read ADCL before ADCH
			adcValue = ADCH;					// Output ADCH to internal version
			
			res_tot[ch] = adcValue;
			
			
			
			ch++;
			if (ch==7) {
				ch = 0;
			}

			PORTB = ch;
			//tillkalla funktion som tar hand om adcValue.
			//välj rätt diod och rätt mux-kanal genom att sätta portarna till rätt värde.

			analogRead(ch);
		}

		ISR(INT0_vect) { //vid kalibrering via knapp
			
			ADCSRA |=(1<<ADSC);
			//while(!(ADCSRA & (1<<ADIF)));

		}

		void analogRead (int ch){
			ADMUX &= 0xF8;								//nollställ de tre minst signifikanta bitarna.
			ADMUX |= ch;								//Select pin ADC0..ADC6 using MUX.
			
			ADCSRA |=(1<<ADSC);
			
		}
		
		//void calibrateSensors() {
		//}

		int main(void)									// borde heta initADC() sen kanske?
		{
			
			initADC();
			sei(); 							// Enable Global Interrupts
			//calibrateSensors();
			
			
			while(1);
			{
				// Wait forever
			}
			
			return 0;
		} 
		
		
		
		
		void initADC() {
			
			DDRA = 0x00;								// Configure PortA as input for analog readings
			DDRB |= (1<<DDB2) | (1<<DDB1) | (1<<DDB0); 	// Configure PortB as output, pin PB0, PB1, PB2.
			PORTB = 0;									// MUX-address = 0,		ändra sen till bara dom lägsta bitarna.
			ADMUX = 0x20;								// AREF, left justify (msb-lsb configuration), Data registers and select ADC0 as input channel. skapa loop som växlar mellan ADC0 och ADC6.
			ADCSRA = 0x8B;								// Enable the ADC and its interrupt feature
			// and set the ACD clock pre-scalar to clk/64
			EICRA |=(1<<ISC00);							// Sets the ISC00 to 1 rising edge triggering
			EICRA |=(1<<ISC01);							// Sets ISC01 to 1
			EIMSK =0x01;
		}*/
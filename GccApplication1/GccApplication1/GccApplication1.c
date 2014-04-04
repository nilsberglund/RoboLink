		/*
		 * ADconvertion_line_sensors.c
		 *
		 * Created: 3/27/2014 2:47:10 PM
		 *  Author: susba199
		 */ 


		#include <avr/io.h>
		#include <avr/interrupt.h>
		
		void initADC();
		void analogRead (int i);
		void calibrateSensors();
		 static volatile uint8_t adc_res = 0; 
		 int i = 0; 
		 uint8_t res_tot[7];  


					ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
					{
						adc_res = ADCL;			// OBS! Must read ADCL before ADCH
						adc_res = ADCH;  // Output ADCH to internal version 
						res_tot[i] = adc_res; 
						
						i++; 
						if (i==7) {
							i = 0; 
						}

						PORTB = i; 
						//tillkalla funktion som tar hand om adc_res. 
						 //välj rätt diod och rätt mux-kanal genom att sätta portarna till rätt värde.

						analogRead(i);
						

					}

					ISR(INT0_vect) { //vid kalibrering via knapp 
				
						ADCSRA |=(1<<ADSC);
						//while(!(ADCSRA & (1<<ADIF)));

					}

		void initADC() {
			
			DDRA = 0x00;				// Configure PortA as input, all pins
			DDRB |= (1<<DDB2) | (1<<DDB1) | (1<<DDB0); 				// Configure PortB as output, pin PB0, PB1, PB2.
			PORTB = 0; //ändra sen till bara dom lägsta bitarna. 
			ADMUX = 0x20;			// AREF, left justify
			// data registers and select ADC0 as input channel. skapa loop som växlar mellan ADC0 och ADC6.
			ADCSRA = 0x8B;			// Enable the ADC and its interrupt feature
			// and set the ACD clock pre-scalar to clk/64
			
			EICRA |=(1<<ISC00);   //Sets the ISC00 to 1 rising edge triggering
			EICRA |=(1<<ISC01);	  //Sets ISC01 to 1
			EIMSK =0x01;
			
			sei(); 				// Enable Global Interrupts
		}
		
		
		void analogRead (int i){
			ADMUX &= 0xF8; //nollställ de tre minst signifikanta bitarna.
			ADMUX |= i; //Select pin ADC0..ADC6 using MUX.
			
			ADCSRA |=(1<<ADSC);
			
		}	
		
		//void calibrateSensors() { 
		//}		

		int main(void) // borde heta initADC() sen kanske? 
		{
	
						initADC(); 
						//calibrateSensors(); 
  
	        
					while(1);
					{
							// Wait forever
					}
								
					return 0; 			
		}
	
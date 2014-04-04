		/*
		 * ADconvertion_line_sensors.c
		 *
		 * Created: 3/27/2014 2:47:10 PM
		 *  Author: susba199
		 */ 


		#include <avr/io.h>
		#include <avr/interrupt.h>
		 static volatile uint8_t adc_res = 0; 
		

					ISR(ADC_vect) //Interrupt Service Routine (ADC Conversion Complete)
					{
						adc_res = ADCL;			// OBS! Must read ADCL before ADCH
						adc_res = ADCH;			// Output ADCH to internal version 
						//tillkalla funktion som tar hand om adc_res. 
				
					}

					ISR(INT0_vect) //vid kalibrering via knapp
					{
						
						ADCSRA |=(1<<ADSC);
						while(!(ADCSRA & (1<<ADIF)));
					
					}
							

		int main(void) // borde heta initADC() sen kanske? 
		{
	
						DDRA = 0x00;				// Configure PortA as input, all pins
						DDRB = 0xFF;				// Configure PortB as output, all pins
						PORTB = 0x04; // demux: enable A in CBA => led nr 2 active. 
						ADMUX = 0x21;			// first byte: left justify, 2nd byte: admux select channel
						// data registers and select ADC0 as input channel. skapa loop som växlar mellan ADC0 och ADC6.
						ADCSRA = 0x8E;			// Enable the ADC and its interrupt feature
							// and set the ACD clock pre-scalar to clk/8
		        
						EICRA |=(1<<ISC00);   //Sets the ISC00 to 1 rising edge triggering
						EICRA |=(1<<ISC01);	  //Sets ISC01 to 1
						EIMSK =0x01; 
		        
						sei(); 				// Enable Global Interrupts
  
	        
					while(1);
					{

					}
								// Wait forever
					return 0; 			
		}
	
	
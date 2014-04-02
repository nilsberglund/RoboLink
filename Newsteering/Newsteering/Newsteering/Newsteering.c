/*
 * Newsteering.c
 *
 * Created: 3/29/2014 4:41:58 PM
 *  Author: albal214
 */ 


#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int speed;

void driving_setup();
void drive(int, int, int, int);
int pulse_width_from_speed(int);
void drive_forward();
void drive_backward();
void drive_stop();
void drive_left_forward();
void drive_right_forward();


int main(void)
{
	speed = 25;
	driving_setup();
	sei();
	while(1)
    {
		
		drive_forward();
		//drive_backward();
	
      
		//		PORTD |= (1 << PORTD6)|(0 << PORTD7);   //PORT 6 = LEFT (FORWARD 1 BACKWARD 0) PORT 7 RIGHT (FORWARD 0 BACKWARD 1) 
														// Corrected in drive function. I.e. Write 1,1 to move forwards, 0,0 to move backwards
		//		OCR1A = 65000;
		//		OCR1B = 65000; 
    }
}

/* Sets up the data directions and sets the PWM mode. */
void driving_setup()
{
	TCCR1A	= 0b11110001; //Sets the mode to Phase Correct PWM and sets the Comp to set on incrementing. 
	TCCR1B  = 2;  //Sets the prescaling to 8
	
	TIMSK1 |= (1 << OCIE1A)|(1 << OCIE0B);   //Enables the compare interrupts
	
	TCNT1 = 0;
	
	OCR1A = 0;   
	OCR1B = 0;
	
	DDRD |= (1 << PORTD4)|(1 << PORTD5)|(1 << PORTD6)|(1 << PORTD7);
}

/* PWM. Ex. If speed is 25, the robot moves at 25% of the maximum speed */
int pulse_width_from_speed(int speed)
{
	int converter = 65535/100;
	int temp = speed * converter;
	return temp;
}

/* Drive-function. Handles direction. Makes the robot move. */
void drive(int right_dir, int left_dir, int speed_left, int speed_right)
{
	if(right_dir == 1){
		PORTD |= (0 << PORTD7);
	}
	else 
	{
		PORTD |= (1 << PORTD7);
	}
	
	if(left_dir == 1)
	{
		PORTD |= (1 << PORTD6);
	}
	else 
	{
		PORTD |= (0 << PORTD6);
	}
	
	/*accelerate.. 
	for (int i = 1; i <= 10; i++)
	{
		OCR1A = pulse_width_from_speed(speed*i/10);
		OCR1B = pulse_width_from_speed(speed*i/10);
		_delay_ms(10);
	}
	*/
}

/* Function allowing the robot to move forwards */
void drive_forward()
{
		drive(1,1, speed, speed);
}

/* Function allowing the robot to move backwards*/
void drive_backward()
{
		drive(0, 0, speed, speed);
}

/* Makes the robot stop */
void drive_stop()
{
	OCR1A = pulse_width_from_speed(0);
	OCR1B = pulse_width_from_speed(0);
}

/* Makes the robot drive left forward */
void drive_left_forward()
{
	int tmp_speed = 0.5*speed;
	drive(1,1, tmp_speed, speed);
}

/* Makes the robot drive right forward */
void drive_right_forward()
{
	int tmp_speed = 0.5*speed;
	drive(1,1, speed, tmp_speed);	
}

/*
void PD_control() 
{
	get_sensor_data();   //Ask for sensor data from the sensor module
	
	calculate_error()?
	
	set_control_constants();
	
	P =
	D =
	
	
}

*/
/*
void set_control_constants(int p, int d)
{
	P = p;
	D = d;
}
*/
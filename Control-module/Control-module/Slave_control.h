/*
 * Slave_control.h
 *
 * Created: 4/23/2014 9:45:58 AM
 *  Author: nilbe317
 */ 


#ifndef SLAVE_CONTROL_H_
#define SLAVE_CONTROL_H_

/*--------variables---------*/

volatile uint8_t data;
volatile uint8_t sensor_data;
volatile uint8_t robot_arm_data;
volatile uint8_t wheel_steering_data;
volatile unsigned char RFID[10];
volatile _Bool waiting_for_instruction;
volatile uint8_t component; 
static uint8_t sensor;
static uint8_t arm;
static uint8_t wheel;
static uint8_t RFIDcomp;
volatile uint8_t RFID_counter;

/*--------Functions---------*/

void SPI_Init_Slave();
void Slave_TX(uint8_t);

#endif /* SLAVE_CONTROL_H_ */
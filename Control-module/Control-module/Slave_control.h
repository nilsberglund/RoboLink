/*
 * Slave_control.h
 *
 * Created: 4/23/2014 9:45:58 AM
 *  Author: nilbe317
 */ 


#ifndef SLAVE_CONTROL_H_
#define SLAVE_CONTROL_H_

/*--------variables---------*/

uint8_t data;
uint8_t sensor_data;
uint8_t robot_arm_data;
uint8_t wheel_steering_data;
unsigned char RFID[10];
_Bool waiting_for_instruction;
uint8_t component; 
uint8_t sensor;
uint8_t arm;
uint8_t wheel;
uint8_t RFIDcomp;
uint8_t RFID_counter;

/*--------Functions---------*/

void SPI_Init_Slave();
void Slave_TX(uint8_t);

#endif /* SLAVE_CONTROL_H_ */
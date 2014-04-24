/*
 * Slave_sensor.h
 *
 * Created: 4/23/2014 9:45:27 AM
 *  Author: susba199
 */ 


#ifndef SLAVE_SENSOR_H_
#define SLAVE_SENSOR_H_

/*------Variables-------*/
volatile uint8_t instruction_data;
volatile uint8_t sensor_data; //from line sensor-file

/*------Functions-------*/
void SPI_Init_Slave();
void Slave_TX(uint8_t);


#endif /* SLAVE_SENSOR_H_ */
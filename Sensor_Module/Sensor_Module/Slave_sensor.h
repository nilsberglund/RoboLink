/*
 * Slave_sensor.h
 *
 * Created: 4/23/2014 9:45:27 AM
 *  Author: susba199
 */ 


#ifndef SLAVE_SENSOR_H_
#define SLAVE_SENSOR_H_

/*------Variables-------*/
volatile uint8_t instructionData;
volatile uint8_t sensorData; //from line sensor-file

/*------Functions-------*/
void SPIInitSlave();
void slaveTX(uint8_t);


#endif /* SLAVE_SENSOR_H_ */
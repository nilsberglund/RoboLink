/*
 * Slave_control.h
 *
 * Created: 4/23/2014 9:45:58 AM
 *  Author: nilbe317
 */ 


#ifndef SLAVE_CONTROL_H_
#define SLAVE_CONTROL_H_


/*-------constants----------*/
#define ARM 1
#define SENSOR 2
#define WHEEL 3
#define KPROPORTIONAL 4
#define KDERIVATIVE 5 
/*--------variables---------*/

volatile uint8_t data;
volatile uint8_t sensorData;
volatile uint8_t armData;
volatile uint8_t steeringData;
volatile uint8_t waitingForInstruction;
volatile uint8_t component;

/*--------Functions---------*/

void SPIInitSlave();
void slaveTX(uint8_t);

#endif /* SLAVE_CONTROL_H_ */
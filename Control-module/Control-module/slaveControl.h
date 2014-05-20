/*
 * slaveControl.h
 *
 * Created: 4/23/2014 9:45:58 AM
 *  Author: Nils Berglund
 */ 


#ifndef SLAVECONTROL_H_
#define SLAVECONTROL_H_


/*-------Constants----------*/
#define ARM 1
#define SENSOR 2
#define WHEEL 3
#define KPROPORTIONAL 4
#define KDERIVATIVE 5 
#define DROPITEM 6

/*--------Variables---------*/

volatile uint8_t data;
volatile uint8_t sensorData;
volatile uint8_t armData;
volatile uint8_t steeringData;
volatile uint8_t waitingForInstruction;
volatile uint8_t component;

/*--------Functions---------*/

void SPIInitSlave();
void slaveTX(uint8_t);
void TXFinishedDrop();

#endif /* SLAVECONTROL_H_ */
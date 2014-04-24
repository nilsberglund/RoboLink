/*
 * SensorCalMajor.h
 *
 * Created: 4/23/2014 9:45:46 AM
 *  Author: susba199
 */ 


#ifndef SENSORCALMAJOR_H_
#define SENSORCALMAJOR_H_

/*------Variables-------*/

uint8_t adcValue;
int ch;
int caliMode;
int buttonPushed;
uint8_t channelThresholds[7];
uint8_t newSensorValues[7];
uint8_t lightVector[7];
uint8_t darkVector[7];


/*------Functions-------*/
void initADC();
void analogRead(int ch);
void calcOneByteLineVector();
void defaultMode();
void calibrationMode();
void calcThresholds();



#endif /* SENSORCALMAJOR_H_ */
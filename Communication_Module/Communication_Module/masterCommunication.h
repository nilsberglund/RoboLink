/*
 * masterCommunication.h
 *
 * Created: 4/23/2014 10:32:37 AM
 *  Author: Susanna Bäckman
 */ 


#ifndef MASTERCOMMUNICATION_H_
#define MASTERCOMMUNICATION_H_

/*--------Constants----------*/
#define SENSORDATASEND 1
#define SENSORDATARECEIVE 2
#define DRIVEINSTRSEND 3
#define ARMINSTRSEND 4
#define KPSEND 5
#define KDSEND 6
#define DROPITEMSEND 7
#define CALIBRATION 8
#define SENSORSLAVE 9
#define CONTROLSLAVE 10
#define NOSLAVE 11
#define LEAVESTATIONINSTR 12

/*----------Variables------------*/
volatile uint8_t receivedData;
volatile uint8_t sensorData;
volatile uint8_t wheelData;
volatile uint8_t armData;
volatile uint8_t Kp;
volatile uint8_t Kd;

/*-------------Functions--------------*/
void SPIinitMaster();
uint8_t masterRX(uint8_t);
void RXsensorData();
void TXsensorData();
void slaveSelect(uint8_t);
void TXprotocol(uint8_t);
void TXarmData();
void TXwheelData();
void masterTX(uint8_t);
void TXKpData();
void TXKdData();
void TXDropItem();
void TXCalibration();
void TXleaveStation();

#endif /* MASTERCOMMUNICATION_H_ */
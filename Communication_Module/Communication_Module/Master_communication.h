/*
 * Master_communication.h
 *
 * Created: 4/23/2014 10:32:37 AM
 *  Author: susba199
 */ 


#ifndef MASTER_COMMUNICATION_H_
#define MASTER_COMMUNICATION_H_

/* Constants */
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

/* Variables */
volatile uint8_t received_data;
volatile uint8_t sensorData;
volatile uint8_t wheelData;
volatile uint8_t armData;
volatile uint8_t Kp;
volatile uint8_t Kd;

uint8_t wantedData;
unsigned char RFID[10];
uint8_t i;


/* Functions */
void SPI_Init_Master();
uint8_t masterRX(uint8_t);
void RXsensorData();
void TXsensorData();
void slaveSelect(uint8_t);
void TXprotocol(uint8_t);
void TXarmData();
void RX_arm_data();
void TXwheelData();
void masterTX(uint8_t);
void receive_sensorData();
void RX_wheel_data();
void TXKpData();
void TXKdData();
void TXDropItem();
void TXCalibration();
void TXleaveStation();

#endif /* MASTER_COMMUNICATION_H_ */
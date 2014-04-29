/*
 * Master_communication.h
 *
 * Created: 4/23/2014 10:32:37 AM
 *  Author: susba199
 */ 


#ifndef MASTER_COMMUNICATION_H_
#define MASTER_COMMUNICATION_H_

/* Variables */
volatile uint8_t received_data;
volatile uint8_t sensor_data;
volatile uint8_t wheel_steering_data;
volatile uint8_t robot_arm_data;

uint8_t Sensor_Slave;
uint8_t Control_Slave;
uint8_t No_Slave;
uint8_t ss;
uint8_t sr;
uint8_t ws;
uint8_t wr;
uint8_t as;
uint8_t ar;
uint8_t rs;
uint8_t wanted_data;
unsigned char RFID[10];
uint8_t i;


/* Functions */
void SPI_Init_Master();
uint8_t Master_RX(uint8_t);
void RX_sensor_data();
void TX_sensor_data();
void Slave_Select(uint8_t);
void TX_Protocol(uint8_t);
void TX_arm_data();
void RX_arm_data();
void TX_wheel_data();
void Master_TX(uint8_t);
void receive_sensor_data();

void RX_wheel_data();
void TX_RFID_data();




#endif /* MASTER_COMMUNICATION_H_ */
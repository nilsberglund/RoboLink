
/*included files
	*
*/

#define ENABLE_BIT_DEFINITIONS
//include <io.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

typedef unsigned char byte; 
typedef unsigned int word; 
#define ON 1; 
#define OFF 0; 
#define _ON 0; 
#define _OFF 1; 

//--- Control Table Address ---
//EEPROM AREA. Non-volatile. 
#define MODEL_NUMBER_L    0
#define MODEL_NUMBER_H    1
#define VERSION           2
#define ID                3
#define BAUD_RATE         4
#define RETURN_DELAY_TIME 5
#define CW_ANGLE_LIMIT_L  6
#define CW_ANGLE_LIMIT_H  7
#define CCW_ANGLE_LIMIT_L 8
#define CCW_ANGLE_LIMIT_H 9
#define SYSTEM_DATA2      10
#define LIMIT_TEMPERATURE 11
#define DOWN_LIMIT_VOLTAGE 12 
#define UP_LIMIT_VOLTAGE  13
#define MAX_TORQUE_L      14
#define MAX_TORQUE_H      15
#define RETURN_LEVEL      16
#define ALARM_LED         17
#define ALARM_SHUTDOWN    18
#define OPERATING_MODE    19
#define DOWN_CALIBRATION_L 20
#define DOWN_CALIBRATION_H 21
#define UP_CALIBRATION_L   22
#define UP_CALIBRATION_H   23

//RAM AREA
#define TORQUE_ENABLE    (24) 
#define LED              (25)
#define CW_COMPLIANCE_MARGIN (26)
#define CCW_COMPLIANCE_MARGIN (27)
#define CW_COMPLIANCE_SLOPE  (28)
#define CCW_COMPLIANCE_SLOPE (29)
#define GOAL_POSITION_L    (30)
#define GOAL_POSITION_H    (31)
#define GOAL_SPEED_L       (32)
#define GOAL_SPEED_H       (33)
#define TORQUE_LIMIT_L     (34)
#define TORQUE_LIMIT_H     (35)
#define PRESENT_POSITION_L (36)
#define PRESENT_POSITION_H (37)
#define PRESENT_SPEED_L    (38) 
#define PRESENT_SPEED_H    (39)
#define PRESENT_LOAD_L     (40)
#define PRESENT_LOAD_H     (41)
#define PRESENT_VOLTAGE    (42)
#define PRESENT_TEMPERATURE (43)
#define REGISTERED_INSTRUCTION (44) 
#define PAUSE_TIME         (45)
#define MOVING             (46)
#define LOCK               (47)
#define PUNCH_L            (48)
#define PUNCH_H            (49)

//---Instruction---
#define PING   0x01 //No action, used for obtaining a Status Packet. 
#define READ   0x02 // Reading values in the Contol Table. 
#define WRITE   0x03 // Writing values to the Control Table. 
#define REG_WRITE 0x04 // Similar to WRITE, but stays in standy mode until the ACTION instruction is given. 
#define ACTION  0x05 //Triggers the action registered by the REG_WRITE instruction. 
#define RESET   0x06 //Changes the control table values of the Dynamixel actuator to the Factory Default Value settings.  
#define DIGITAL_RESET 0x07
#define SYSTEM_READ  0x0C
#define SYSTEM_WRITE 0x0D 
#define SYNC_WRITE 0x83 //Used for controlling many Dynamixel actuators at the same time. 
#define SYNC_REG_WRITE 0x84

#define CLEAR_BUFFER gbRxBufferReadPointer = gbRxBufferWritePointer
#define DEFAULT_RETURN_PACKAGE_SIZE 6 //ska det vara 6? 
#define BROADCASTING_ID 0xFE //Package sent to this ID applies to all servos. Will not return any status packets. 

#define DEFAULT_BAUD_RATE 34 //57600bps at 16MHz




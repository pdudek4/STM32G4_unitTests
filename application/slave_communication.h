/*
 * slave_communication.h
 *
 *  Created on: Mar 25, 2024
 *      Author: w.klimiuk
 */

#ifndef MODULES_COMMUNICATION_SLAVE_COMMUNICATION_H_
#define MODULES_COMMUNICATION_SLAVE_COMMUNICATION_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#endif
#include <stdbool.h>
#include <stdint.h>
#include "configuration_typedef.h"



#define I2C_TX_BUFFER_LENGTH 50
#define I2C_RX_BUFFER_LENGTH 270
#define I2C_RX_FRAME_BUFFER_SIZE 4


#define I2C_TX_COMMAND_GET_SW_VERSION           	0x10
#define I2C_TX_COMMAND_GET_SW_VERSION_LENGTH    	(sizeof(deviceVersions_t))

//#define I2C_TX_COMMAND_GET_ALARM_VALUE          	0x14
//#define I2C_TX_COMMAND_GET_ALARM_VALUE_LENGTH   	2
//
//#define I2C_TX_COMMAND_READ_DATA               	0x20
//#define I2C_TX_COMMAND_READ_DATA_LENGTH        	(sizeof(measure_data_s))
//
//#define I2C_TX_COMMAND_GET_AUTOTEST            	0x21
//#define I2C_TX_COMMAND_GET_AUTOTEST_LENGTH      	(sizeof(autotest_info_s))

#define I2C_TX_COMMAND_SLAVE_STATE					0xE0
#define I2C_TX_COMMAND_SLAVE_STATE_LENGTH			1

#define I2C_TX_COMMAND_FIRMWARE_DATA_VALID      	0xE1
#define I2C_TX_COMMAND_FIRMWARE_DATA_VALID_LENGTH	3


//#define I2C_RX_COMMAND_START_AUTOTEST           	0x90
//#define I2C_RX_COMMAND_START_AUTOTEST_LENGTH    	1
//
//#define I2C_RX_COMMAND_SET_ALARM_VALUE          	0x91
//#define I2C_RX_COMMAND_SET_ALARM_VALUE_LENGTH   	3

#define I2C_RX_COMMAND_IMAGE_INFO					0xE0
#define I2C_RX_COMMAND_IMAGE_INFO_LENGTH			56
#define I2C_RX_IMAGE_INFO_PAYLOAD_LENGTH			48

#define I2C_RX_COMMAND_IMAGE_DATA					0xE1
#define I2C_RX_COMMAND_IMAGE_DATA_LENGTH    		264
#define I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH 			256

#define I2C_RX_COMMAND_RESTART                  	0xF0
#define I2C_RX_COMMAND_RESTART_LENGTH           	1

//#define I2C_RX_COMMAND_RUN_BOOTLOADER           	0xF1
//#define I2C_RX_COMMAND_RUN_BOOTLOADER_LENGTH    	1


#define I2C_COMMAND_TEST                        	0x57
#define I2C_TEST_LENGTH                         	7


#define I2C_FR_LENGTH_OFFSET		2
#define I2C_FR_SEGMENT_OFFSET		4
#define I2C_FR_CRC_OFFSET			6
#define I2C_FR_PAYLOAD_OFFSET		8


typedef enum
{
	COMMAND_OK,
	COMMAND_ERROR
} command_status_e;

typedef struct
{
	uint8_t buffer[I2C_TX_BUFFER_LENGTH];
} I2C_transmission_buffer_s;

typedef struct
{
	uint8_t buffer[I2C_RX_BUFFER_LENGTH];
	uint16_t position;
	uint16_t expected_length;
} I2C_reception_buffer_s;

typedef struct
{
	uint8_t frame[I2C_RX_FRAME_BUFFER_SIZE][I2C_RX_BUFFER_LENGTH];
	uint16_t frame_length[I2C_RX_FRAME_BUFFER_SIZE];
	uint8_t first;
	uint8_t last;
} I2C_frames_round_buffer_s;


void COMM_receptionRequest(void);
void COMM_transmissionRequest(void);
void COMM_receptionCompleted(void);
void COMM_transmissionCompleted(void);
void COMM_processReceivedCommands(void);


#ifdef __cplusplus
}
#endif
#endif /* MODULES_COMMUNICATION_SLAVE_COMMUNICATION_H_ */

/*
 * configuration_typedef.h
 *
 *  Created on: Mar 26, 2024
 *      Author: w.klimiuk
 */

#ifndef CONFIGURATION_CONFIGURATION_TYPEDEF_H_
#define CONFIGURATION_CONFIGURATION_TYPEDEF_H_

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#endif
#include <stdint.h>


#define BOOTLOADER_SETTINGS_POSITION 		0
//#define CAN_CONFIG_POSITION				20
#define BOOTLOADER_CONFIG_POSITION			260
#define HARDWARE_VERSION_POSITION			280
#define SERIAL_NUMBER_EEPROM_POSITION		300

#define BOOTLOADER_SETTINGS_VERSION			1
#define HARDWARE_VERSION_SETTINGS_VERSION	1
#define EEPROM_SERIAL_NUMBER_VERSION		1

#define SERIAL_NUMBER_LENGTH				40
#define VERSION_DATA_NOT_AVAILABLE			0xCC

#define CONF_BUILD_DAY						22
#define CONF_BUILD_MONTH					10
#define CONF_BUILD_YEAR						24

#define SOFT_VER_MAJOR						0
#define SOFT_VER_MINOR						1
#define SOFT_VER_PATCH						1

#define DEVICE_NAME 						IMD6CH_meas
#define DEVICE_ID 							19

#define AVAILABLE_ADDRESSES_COUNT			6


typedef enum BOOTLOADER_BOOT_OPTION
{
	RUN_BOOTLOADER = 0,
	RUN_APPLICATION = 1,
} bootloaderBootOption_t;

typedef enum BOOTLOADER_FLASH_STATE
{
	APPLICATION_PRESENT = 0,
	APPLICATION_MISSING = 1,
} bootloaderFlashState_t;

typedef struct
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
} __attribute__ ((packed)) dateBuild_t;

typedef struct
{
	uint8_t softwareVersion[3];
	dateBuild_t date;
} __attribute__ ((packed)) versionConfig_t;

typedef struct
{
	versionConfig_t boot;
	versionConfig_t app;
	uint8_t in_boot_mode;			//1-boot, 0-app
	uint8_t is_app_missing;
} __attribute__((packed)) deviceVersions_t;

typedef struct
{
	uint8_t softwareVersion[3];
	dateBuild_t date;
	uint16_t crc;
} __attribute__ ((packed)) bootloader_version_flash_t;


typedef struct
{
	uint8_t bootloader_settings_version_number;
	bootloaderBootOption_t boot_state;
	bootloaderFlashState_t application_state;
	uint16_t crc;
} __attribute__((packed)) bootloaderSettings_t;

typedef struct
{
	uint8_t version_number;
	uint16_t device_hardware_version;
	uint16_t crc;
} __attribute__((packed)) hardware_version_t;


#endif /* CONFIGURATION_CONFIGURATION_TYPEDEF_H_ */

/*
 * configuration.c
 *
 *  Created on: Mar 26, 2024
 *      Author: w.klimiuk
 */


#include <string.h>
#include "configuration.h"
#include "bsp_gpio.h"
#include "eeprom.h"
#include "utilities.h"


typedef enum
{
	CONFIG_OK,
	CONFIG_ERROR
} configValue_t;


static bool g_address_correct = false;

const static uint8_t g_device_addresses_array[AVAILABLE_ADDRESSES_COUNT] = { 6, 5, 4, 3, 2, 1 };
const static uint8_t g_address_pins_array[AVAILABLE_ADDRESSES_COUNT] = { 0, 1, 2, 3, 4, 5 };
static uint8_t g_device_address = 0;

static bootloaderSettings_t g_local_bootloader_settings = {0};
static bootloaderSettings_t g_memory_bootloader_settings = {0};

__attribute__ ((section(".bootloaderVersionBlock")))
const bootloader_version_flash_t f_bootloader_version =
{
#ifndef UNIT_TEST
		.softwareVersion[0] = SOFT_VER_MAJOR,
		.softwareVersion[1] = SOFT_VER_MINOR,
		.softwareVersion[2] = SOFT_VER_PATCH,
		.date.day = CONF_BUILD_DAY,
		.date.month = CONF_BUILD_MONTH,
		.date.year = CONF_BUILD_YEAR,
		.crc = 0x52DB
#endif
};

static deviceVersions_t deviceVersions;


void CONF_readMCUAddress(void)
{
	uint8_t pin_setup = BSP_readAddressDetectPins();
	for (uint8_t i = 0; i < AVAILABLE_ADDRESSES_COUNT; i++)
	{
		if (pin_setup == g_address_pins_array[i])
		{
			g_device_address = g_device_addresses_array[i];
			g_address_correct = true;
			return;
		}
	}
}

uint8_t CONF_getMCUAddress(void)
{
	return g_device_address;
}

bool CONF_isMCUAddressCorrect(void)
{
	return g_address_correct;
}

/**
 * @brief	Inicjalizuje strukturę wersji do wysyłki komendą "getSoftwareVersion"
 */
void CONF_initDeviceVersions(void)
{
	memcpy(&deviceVersions.boot, (void*)&f_bootloader_version, sizeof(versionConfig_t));
	memset(&deviceVersions.app, VERSION_DATA_NOT_AVAILABLE, sizeof(versionConfig_t));
	deviceVersions.in_boot_mode = 1;
	deviceVersions.is_app_missing = getFlashByte();
}

/**
 * @brief	Zwraca wskaźnik do struktury deviceVersions_t
 */
deviceVersions_t * CONF_getDeviceVersionHandle(void)
{
	return &deviceVersions;
}

/**
 * @brief	Sprawdzenie, czy wymagana aktualizacja
 * @return	SUCCESS - aktualizacja, ERROR - normalne uruchomienie
 */
uint8_t CONF_checkUpgrade(void)
{
//	uint8_t retVal;
//
//	retVal = loadBootloaderSettings();	//todo problem GTest
//	if (retVal != CONFIG_OK)
//	{
//		getBootloaderSettings()->boot_state = RUN_BOOTLOADER;
//		getBootloaderSettings()->application_state = APPLICATION_PRESENT;
//	}

	if (g_memory_bootloader_settings.application_state == 0xFF)	//tymczasowo na start aplikacji bez danych w EEPROMie
	{
		return ERROR;
	}

	if (getBootByte() == RUN_BOOTLOADER)
	{
		return SUCCESS;
	}
	else
	{
		if (getFlashByte() != APPLICATION_PRESENT)
			return SUCCESS;
	}

	return ERROR;
}

/**
 * @brief	Zapisuje w EEPROMie ustawienia. boot_state, app state
 */
uint8_t saveBootloaderSettings(void)
{
	uint8_t status = CONFIG_OK;
	bootloaderSettings_t compare_array[2];

	if (memcmp((uint8_t*)&g_memory_bootloader_settings, (uint8_t*)&g_local_bootloader_settings, sizeof(bootloaderSettings_t)) != 0)
	{
		g_local_bootloader_settings.bootloader_settings_version_number = BOOTLOADER_SETTINGS_VERSION;
		if (EEPROM_writeBlocks(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&g_local_bootloader_settings, (uint8_t*)compare_array, sizeof(bootloaderSettings_t), EEPROM_ADD_CRC) == EEPROM_OK)
		{
			memcpy((uint8_t*)&g_memory_bootloader_settings, (uint8_t*)&g_local_bootloader_settings, sizeof(bootloaderSettings_t));
		}
		else
		{
			status = CONFIG_ERROR;
		}
	}
	return status;
}

/**
 * @brief	Odczytuje z EEPROMu ustawienia. boot_state, app state
 */
uint8_t loadBootloaderSettings(void)
{
	uint8_t status = CONFIG_ERROR;
	bootloaderSettings_t data_to_load;
	bootloaderSettings_t compare_array;
	uint8_t retVal;

	retVal = EEPROM_readBlocks(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&data_to_load,
			(uint8_t*)&compare_array, sizeof(bootloaderSettings_t), EEPROM_ADD_CRC);
	if (retVal == EEPROM_OK && (data_to_load.bootloader_settings_version_number == BOOTLOADER_SETTINGS_VERSION))
	{
		memcpy((uint8_t*)&g_local_bootloader_settings, (uint8_t*)&data_to_load, sizeof(bootloaderSettings_t));
		memcpy((uint8_t*)&g_memory_bootloader_settings, (uint8_t*)&data_to_load, sizeof(bootloaderSettings_t));
		status = CONFIG_OK;
	}
	else
	{
		memset((uint8_t*)&g_memory_bootloader_settings, 0xff, sizeof(bootloaderSettings_t));
	}
	return status;
}

uint8_t getBootByte(void)
{
	return getBootloaderSettings()->boot_state;
}

void setFlashByte(void)
{
	getBootloaderSettings()->application_state = APPLICATION_PRESENT;
	saveBootloaderSettings();
}

void clearFlashByte(void)
{
	getBootloaderSettings()->application_state = APPLICATION_MISSING;
	saveBootloaderSettings();
}

uint8_t getFlashByte(void)
{
	return getBootloaderSettings()->application_state;
}

bootloaderSettings_t* getBootloaderSettings()
{
	return &g_local_bootloader_settings;
}


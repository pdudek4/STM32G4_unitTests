/*
 * test_eeprom.c
 *
 *  Created on: Oct 28, 2024
 *      Author: pdude
 */

//Test framework
#include "unity.h"

//Test framework local support
#include "testing_defines.h"

//Mocks
#include "mock_bsp_i2c.h"
#include "mock_bsp_crc.h"

//Code Under Test
#include "eeprom.h"

//CUT dependencies

//Other
#include "utilities.h"
#include "eeprom_24AA64.h"
#include "configuration_typedef.h"


void setUp(void)
{
}

void tearDown(void)
{
}


//**********************************EEPROM_read**************************
void test_EEPROM_read(void)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	//mock
	  BSP_I2C_memoryRead_IgnoreAndReturn(HAL_OK);
	retVal = EEPROM_read(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&testVal, sizeof(bootloaderSettings_t));
	TEST_ASSERT_EQUAL(retVal, HAL_OK);
}

//**********************************EEPROM_write**************************
void test_EEPROM_write_aboveRange(void)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	retVal = EEPROM_write(BOOTLOADER_SETTINGS_POSITION + EEPROM_LENGTH, (uint8_t*)&testVal, sizeof(bootloaderSettings_t));
	TEST_ASSERT_EQUAL(retVal, EEPROM_ERROR);
}

void test_EEPROM_write_lenEqZero(void)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	retVal = EEPROM_write(BOOTLOADER_SETTINGS_POSITION + EEPROM_LENGTH, (uint8_t*)&testVal, 0);
	TEST_ASSERT_EQUAL(retVal, EEPROM_ERROR);
}

//**********************************EEPROM_readBlocks**************************
void test_EEPROM_readBlocks_(void)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;
	bootloaderSettings_t testValCmp;

	//mock
	  BSP_I2C_memoryRead_IgnoreAndReturn(HAL_OK);
	retVal = EEPROM_readBlocks(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&testVal, (uint8_t*)&testValCmp,
							   sizeof(bootloaderSettings_t), EEPROM_NO_CRC);
	TEST_ASSERT_EQUAL(retVal, EEPROM_OK);
}


/*
 * test_eeprom.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: pdude
 */

//Test framework
#include "gtest/gtest.h"

//Test framework local support
#include "testing_defines.h"

//Mocks
#include "mock_bsp_crc.h"

//Code Under Test
#include "eeprom.h"

//CUT dependencies

//Other
#include "eeprom_24AA64.h"
#include "configuration_typedef.h"


using ::testing::_;
using ::testing::Return;
using namespace std;


//**********************************EEPROM_read**************************
TEST(EEPROM_test, test_EEPROM_read)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	retVal = EEPROM_read(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&testVal, sizeof(bootloaderSettings_t));
	EXPECT_EQ(retVal, HAL_OK);
}

//**********************************EEPROM_write**************************
TEST(EEPROM_test, test_EEPROM_write_aboveRange)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	retVal = EEPROM_write(BOOTLOADER_SETTINGS_POSITION + EEPROM_LENGTH, (uint8_t*)&testVal, sizeof(bootloaderSettings_t));
	EXPECT_EQ(retVal, EEPROM_ERROR);
}

TEST(EEPROM_test, test_EEPROM_write_lenEqZero)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;

	retVal = EEPROM_write(BOOTLOADER_SETTINGS_POSITION + EEPROM_LENGTH, (uint8_t*)&testVal, 0);
	EXPECT_EQ(retVal, EEPROM_ERROR);
}

//**********************************EEPROM_readBlocks**************************
TEST(EEPROM_test, test_EEPROM_readBlocks_)
{
	uint8_t retVal;
	bootloaderSettings_t testVal;
	bootloaderSettings_t testValCmp;

	retVal = EEPROM_readBlocks(BOOTLOADER_SETTINGS_POSITION, (uint8_t*)&testVal, (uint8_t*)&testValCmp,
							   sizeof(bootloaderSettings_t), EEPROM_NO_CRC);
	EXPECT_EQ(retVal, EEPROM_OK);
}



/*
 * test_application.c
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

//Test framework
#include "unity.h"

//Test framework local support
#include "testing_defines.h"

//Mocks
#include "mock_bsp_crc.h"
#include "mock_bsp_watchdog.h"
#include "mock_bsp_i2c.h"
#include "mock_bsp_gpio.h"
#include "mock_bsp_flash.h"
#include "mock_application_timers.h"

//Code Under Test
#include "application.h"

//CUT dependencies
#include "application.h"
#include "utilities.h"
#include "configuration.h"
#include "eeprom.h"
#include "slave_communication.h"
#include "state_machine.h"
#include "upgrade.h"
#include "crypto.h"
#include "aes.h"


//extern variables
extern volatile appCrcInfo_ts appCrcInfo;

//called before every test_* function
void setUp(void)
{
}

//called after every test_* function
void tearDown(void)
{
}


//building environment test
//**********************************APP_preinitialize**************************
void test_APP_preinitialize(void)
{
	TEST_ASSERT_TRUE(1);
}

//**********************************APP_initialize*****************************
void test_APP_initialize(void)
{
	//mock
	BSP_I2CStartListening_IgnoreAndReturn(HAL_OK);
	BSP_I2CSet7BitSlaveAddress_Ignore();
	BSP_readAddressDetectPins_IgnoreAndReturn(3);
	CRC_calculateArc16_IgnoreAndReturn(0xFFFF);
	BSP_watchdogFreezeInDebug_Ignore();
	BSP_I2C_memoryRead_IgnoreAndReturn(HAL_OK);
//	initTimers_Expect();

	APP_initialize();
	TEST_ASSERT_TRUE(1);
}

//**********************************APP_process********************************
void test_APP_process(void)
{
	//mock
	CRC_calculateArc16_IgnoreAndReturn(0xFFFF);
	BSP_watchdogRefresh_Ignore();
	BSP_I2C_memoryRead_IgnoreAndReturn(HAL_OK);
	BSP_I2CGetError_IgnoreAndReturn(HAL_OK);
	checkTimerTimeout_IgnoreAndReturn(0);
	restartTimer_Ignore();

	APP_process();
	TEST_ASSERT_TRUE(1);
}

//**********************************APP_checkAppCrc****************************
void test_APP_checkAppCrc_debuggerCase(void)
{
	uint8_t retVal;

	retVal = APP_checkAppCrc();
	TEST_ASSERT_EQUAL(retVal, SUCCESS);
}

void test_APP_checkAppCrc_belowFlashAddress(void)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08004000UL;
	appCrcInfo.crcApp = 0xFFFF;

	retVal = APP_checkAppCrc();
	TEST_ASSERT_EQUAL(retVal, ERROR);
}

void test_APP_checkAppCrc_crcRangeTooFar(void)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0xFFFF;

	retVal = APP_checkAppCrc();
	TEST_ASSERT_EQUAL(retVal, ERROR);
}

void test_APP_checkAppCrc_wrongCrc(void)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0x1234;

	CRC_calculateArc16_IgnoreAndReturn(0x5678);
	retVal = APP_checkAppCrc();
	TEST_ASSERT_EQUAL(retVal, ERROR);
}

void test_APP_checkAppCrc_goodCrc(void)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0x1234;

	CRC_calculateArc16_IgnoreAndReturn(0x1234);
	retVal = APP_checkAppCrc();
	TEST_ASSERT_EQUAL(retVal, SUCCESS);
}


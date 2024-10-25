/*
 * test_application.h
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

//Code Under Test
#include "application.h"

//CUT dependencies
#include "state_machine.h"
#include "utilities.h"



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
	BSP_watchdogFreezeInDebug_Ignore();
	APP_initialize();
	TEST_ASSERT_TRUE(1);
}

//**********************************APP_process********************************
void test_APP_process(void)
{
	BSP_watchdogRefresh_Ignore();
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











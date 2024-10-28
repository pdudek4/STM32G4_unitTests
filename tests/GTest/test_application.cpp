/*
 * test_application.cpp
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

//Test framework
#include "gtest/gtest.h"

//Test framework local support
#include "testing_defines.h"

//Mocks
#include "mock_bsp_crc.h"
#include "mock_bsp_watchdog.h"

//Code Under Test
#include "application.h"

//CUT dependencies


using ::testing::_;
using ::testing::Return;
using namespace std;


//extern variables
extern volatile appCrcInfo_ts appCrcInfo;


//building environment test
//**********************************APP_preinitialize**************************
TEST(APP_test, test_preinitialize)
{
	APP_preinitialize();
	EXPECT_TRUE(1);
}

//**********************************APP_initialize*****************************
TEST(APP_test, test_initialize)
{
	//mock
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogFreezeInDebug())
	.Times(1);

	APP_initialize();
	EXPECT_TRUE(1);

	delete bsp_watchdog_MockObj;
}

//**********************************APP_process********************************
TEST(APP_test, test_process)
{
	//mock
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogRefresh())
	.Times(1);

	APP_process();
	EXPECT_TRUE(1);

	delete bsp_watchdog_MockObj;
}

//**********************************APP_checkAppCrc****************************
TEST(APP_test, test_checkAppCrc_debuggerCase)
{
	uint8_t retVal;

	retVal = APP_checkAppCrc();
	EXPECT_EQ(retVal, SUCCESS);
}

TEST(APP_test, test_checkAppCrc_belowFlashAddress)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08004000UL;
	appCrcInfo.crcApp = 0xFFFF;

	retVal = APP_checkAppCrc();
	EXPECT_EQ(retVal, ERROR);
}

TEST(APP_test, test_checkAppCrc_crcRangeTooFar)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0xFFFF;

	retVal = APP_checkAppCrc();
	EXPECT_EQ(retVal, ERROR);
}

TEST(APP_test, test_checkAppCrc_wrongCrc)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0x1234;

	//mock
	bsp_crc_MockObj = new bsp_crc_Mock;
	EXPECT_CALL(*bsp_crc_MockObj, CRC_calculateArc16((uint8_t*)appCrcInfo.appCrcStartCalcAddr, appCrcInfo.appCrcCalcSize))
	.Times(1)
	.WillOnce(Return(0x5678));

	retVal = APP_checkAppCrc();
	EXPECT_EQ(retVal, ERROR);

	delete bsp_crc_MockObj;
}

TEST(APP_test, test_checkAppCrc_goodCrc)
{
	uint8_t retVal;

	appCrcInfo.appCrcCalcSize = 0xFFFF;
	appCrcInfo.appCrcStartCalcAddr = 0x08006000UL;
	appCrcInfo.crcApp = 0x1234;

	//mock
	bsp_crc_MockObj = new bsp_crc_Mock;
	EXPECT_CALL(*bsp_crc_MockObj, CRC_calculateArc16((uint8_t*)appCrcInfo.appCrcStartCalcAddr, appCrcInfo.appCrcCalcSize))
	.Times(1)
	.WillOnce(Return(0x1234));

	retVal = APP_checkAppCrc();
	EXPECT_EQ(retVal, SUCCESS);

	delete bsp_crc_MockObj;
}



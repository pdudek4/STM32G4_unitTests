/*
 * test_upgrade.cpp
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
#include "mock_bsp_watchdog.h"
#include "mock_bsp_flash.h"

//Code Under Test
#include "upgrade.h"

//CUT dependencies
#include "slave_communication.h"
#include "state_machine.h"
#include "crypto.h"


using ::testing::_;
using ::testing::Return;
using namespace std;


extern uint32_t binary_length;
extern uint32_t received_data_bytes;


//**********************************UPGRADE_validateFrame**************************
TEST(UPGRADE_test, test_UPGRADE_validateFrame)
{
	uint16_t dataLen;
	uint16_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x02, 0x00, 0xFF, 0xFF};

	dataLen = (testFr[I2C_FR_LENGTH_OFFSET+1] << 8) | testFr[I2C_FR_LENGTH_OFFSET];

	//mock
	bsp_crc_MockObj = new bsp_crc_Mock;
	EXPECT_CALL(*bsp_crc_MockObj, CRC_calculateArc16((uint8_t*)&testFr[I2C_FR_PAYLOAD_OFFSET], dataLen))
	.Times(1)
	.WillOnce(Return(0xFFFF));

	retVal = UPGRADE_validateFrame(testFr, dataLen, I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH);
	EXPECT_EQ(SUCCESS, retVal);

	delete bsp_crc_MockObj;
}

//**********************************UPGRADE_framePasswordKey**************************
TEST(UPGRADE_test, test_UPGRADE_framePasswordKey_frameOK)
{
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x03, 0x75, 0x8C, 0x3A, 0x27, 0x8D, 0xE5, 0x4D, 0x26, 0x37, 0xB9, 0x48, 0xA0, 0x02, 0xF7, 0x16};

	//mocki
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogRefresh())
	.Times(2);

	bsp_flash_MockObj = new bsp_flash_Mock;
	EXPECT_CALL(*bsp_flash_MockObj, FLASH_erase(FLASH_APP_START_ADDRESS))
	.Times(1).
	WillOnce(Return(FLASH_STATUS_OK));

	//wszystkie dane prawidłowe
	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	EXPECT_EQ(SM_RECEIVING_BINARY, SM_getState());

	delete bsp_watchdog_MockObj;
	delete bsp_flash_MockObj;
}

TEST(UPGRADE_test, test_UPGRADE_framePasswordKey_wrongBootPassword)
{
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xC5, 0x63, 0x27, 0x10, 0xE2, 0x7F, 0xCE, 0x36, 0x8B, 0x04, 0xCC, 0x1E, 0x57, 0x63, 0xCA, 0xA4,
						  0x03, 0x75, 0x8C, 0x3A, 0x27, 0x8D, 0xE5, 0x4D, 0x26, 0x37, 0xB9, 0x48, 0xA0, 0x02, 0xF7, 0x16};

	//mocki
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogRefresh())
	.Times(2);

	testFr[INFO_PASS_OFFSET] = 0xFF;
	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	EXPECT_EQ(SM_RESET_PROCEDURE, SM_getState());

	delete bsp_watchdog_MockObj;
}

TEST(UPGRADE_test, test_UPGRADE_framePasswordKey_wrongBinLength)
{
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x62, 0xAB, 0x3D, 0xF6, 0xCC, 0x7B, 0xCF, 0x00, 0x0F, 0x10, 0x4D, 0xD8, 0xB4, 0x5A, 0x48, 0xEB};

	//mocki
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogRefresh())
	.Times(2);

	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	EXPECT_EQ(SM_RESET_PROCEDURE, SM_getState());

	delete bsp_watchdog_MockObj;
}

TEST(UPGRADE_test, test_UPGRADE_framePasswordKey_wrongDeviceId)
{
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x52, 0xCC, 0x5E, 0xED, 0x02, 0xE7, 0x77, 0x0A, 0xDF, 0x37, 0xE3, 0xE9, 0xBB, 0xE8, 0x6C, 0x5D};

	//mocki
	bsp_watchdog_MockObj = new bsp_watchdog_Mock;
	EXPECT_CALL(*bsp_watchdog_MockObj, BSP_watchdogRefresh())
	.Times(2);

	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	EXPECT_EQ(SM_RESET_PROCEDURE, SM_getState());

	delete bsp_watchdog_MockObj;
}

//**********************************UPGRADE_processBinaryFrame**************************
TEST(UPGRADE_test, test_UPGRADE_processBinaryFrame_frameOk)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mocki
	bsp_flash_MockObj = new bsp_flash_Mock;
	EXPECT_CALL(*bsp_flash_MockObj, FLASH_write(_,_,_))
	.Times(1).
	WillOnce(Return(FLASH_STATUS_OK));

	//mem address w normie
	binary_length = 0x10000;
	received_data_bytes = 0;

	retVal = UPGRADE_processBinaryFrame(testFr, I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH);
	EXPECT_EQ(SUCCESS, retVal);
	EXPECT_EQ(I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH, received_data_bytes);

	delete bsp_flash_MockObj;
}

TEST(UPGRADE_test, test_UPGRADE_processBinaryFrame_memAddressAboveRange)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mem address powyżej
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x10;
	binary_length = 0x10000;
	received_data_bytes = 0;

	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	EXPECT_EQ(ERROR, retVal);
	EXPECT_EQ(I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH, received_data_bytes);
}

TEST(UPGRADE_test, test_UPGRADE_processBinaryFrame_lastSegmentChangeStateMachine)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mocki
	bsp_flash_MockObj = new bsp_flash_Mock;
	EXPECT_CALL(*bsp_flash_MockObj, FLASH_write(_,_,_))
	.Times(1).
	WillOnce(Return(FLASH_STATUS_OK));

	//ostatni segment czy memAddr ma długość binarki
	testFr[I2C_FR_SEGMENT_OFFSET] = 0xFF;
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x00;
	binary_length = 0x10000;
	received_data_bytes = 0xFF00;

	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	EXPECT_EQ(SUCCESS, retVal);
	EXPECT_EQ(binary_length, received_data_bytes);
	EXPECT_EQ(SM_CHECKING_DATA_INTEGRITY, SM_getState());

	delete bsp_flash_MockObj;
}


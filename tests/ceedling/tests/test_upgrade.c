

//Test framework
#include "unity.h"

//Test framework local support
#include "testing_defines.h"

//Mocks
#include "mock_bsp_i2c.h"
#include "mock_bsp_flash.h"
#include "mock_bsp_watchdog.h"
#include "mock_slave_communication.h"
#include "mock_configuration.h"
#include "mock_utilities.h"

//Code Under Test
#include "upgrade.h"

//CUT dependencies
#include "crypto.h"
#include "aes.h"
#include "state_machine.h"
#include "slave_communication.h"
#include "bsp/bsp_crc.h"
#include "configuration_typedef.h"


extern uint32_t binary_length;
extern uint32_t received_data_bytes;


void setUp(void)
{
  ASSERT_Ignore();
}

void tearDown(void)
{
	binary_length = 0;
}

//**********************************UPGRADE_validateFrame**************************
void test_UPGRADE_validateFrame(void)
{
	uint16_t dataLen;
	uint16_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x02, 0x00, 0xFF, 0xFF};

	dataLen = (testFr[I2C_FR_LENGTH_OFFSET+1] << 8) | testFr[I2C_FR_LENGTH_OFFSET];
	retVal = UPGRADE_validateFrame(testFr, dataLen, I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH);

	TEST_ASSERT_EQUAL(SUCCESS, retVal);
}

//**********************************UPGRADE_framePasswordKey**************************
void test_UPGRADE_framePasswordKey_frameOK(void)
{
	uint8_t retVal;
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x03, 0x75, 0x8C, 0x3A, 0x27, 0x8D, 0xE5, 0x4D, 0x26, 0x37, 0xB9, 0x48, 0xA0, 0x02, 0xF7, 0x16};

	//mocki
	BSP_watchdogRefresh_Ignore();
	FLASH_erase_ExpectAndReturn(FLASH_APP_START_ADDRESS, FLASH_STATUS_OK);

//	SM_setState_Expect(SM_RECEIVING_BINARY);	//gdy mock SM
	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	TEST_ASSERT_EQUAL(SM_RECEIVING_BINARY, SM_getState());
}

void test_UPGRADE_framePasswordKey_wrongBootPassword(void)
{
	uint8_t retVal;
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xC5, 0x63, 0x27, 0x10, 0xE2, 0x7F, 0xCE, 0x36, 0x8B, 0x04, 0xCC, 0x1E, 0x57, 0x63, 0xCA, 0xA4,
						  0x03, 0x75, 0x8C, 0x3A, 0x27, 0x8D, 0xE5, 0x4D, 0x26, 0x37, 0xB9, 0x48, 0xA0, 0x02, 0xF7, 0x16};

	//mocki
	BSP_watchdogRefresh_Ignore();

	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	TEST_ASSERT_EQUAL(SM_RESET_PROCEDURE, SM_getState());
}

void test_UPGRADE_framePasswordKey_wrongBinLength(void)
{
	uint8_t retVal;
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x62, 0xAB, 0x3D, 0xF6, 0xCC, 0x7B, 0xCF, 0x00, 0x0F, 0x10, 0x4D, 0xD8, 0xB4, 0x5A, 0x48, 0xEB};

	//mocki
	BSP_watchdogRefresh_Ignore();

	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	TEST_ASSERT_EQUAL(SM_RESET_PROCEDURE, SM_getState());
}

void test_UPGRADE_framePasswordKey_wrongDeviceId(void)
{
	uint8_t retVal;
	uint8_t testFr[48] = {0xC2, 0x8D, 0x0D, 0xA5, 0x74, 0xC0, 0xFB, 0x1E, 0x08, 0x0D, 0xDB, 0xAF, 0x4C, 0x5D, 0x44, 0x72,
						  0xD3, 0xDF, 0x59, 0x76, 0x83, 0xC8, 0xD8, 0xA5, 0x75, 0xB2, 0x1B, 0xED, 0x4A, 0x68, 0xAC, 0x83,
						  0x52, 0xCC, 0x5E, 0xED, 0x02, 0xE7, 0x77, 0x0A, 0xDF, 0x37, 0xE3, 0xE9, 0xBB, 0xE8, 0x6C, 0x5D};

	//mocki
	BSP_watchdogRefresh_Ignore();

	CRYPTO_initSecretKey();
	UPGRADE_framePasswordKey(testFr);
	TEST_ASSERT_EQUAL(SM_RESET_PROCEDURE, SM_getState());
}

//**********************************UPGRADE_processBinaryFrame**************************
void test_UPGRADE_processBinaryFrame_frameOk(void)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mocki
	FLASH_write_ExpectAnyArgsAndReturn(FLASH_STATUS_OK);

	//mem address w normie
	binary_length = 0x10000;
	received_data_bytes = 0;

	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(SUCCESS, retVal);
	TEST_ASSERT_EQUAL(I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH, received_data_bytes);
}

void test_UPGRADE_processBinaryFrame_memAddressAboveRange(void)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mem address powyżej
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x10;
	binary_length = 0x10000;
	received_data_bytes = 0;

	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(ERROR, retVal);
	TEST_ASSERT_EQUAL(I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH, received_data_bytes);
}

void test_UPGRADE_processBinaryFrame_lastSegmentChangeStateMachine(void)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	//mocki
	FLASH_write_ExpectAnyArgsAndReturn(FLASH_STATUS_OK);

	//ostatni segment czy memAddr ma długość binarki
	testFr[I2C_FR_SEGMENT_OFFSET] = 0xFF;
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x00;
	binary_length = 0x10000;
	received_data_bytes = 0xFF00;

	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(SUCCESS, retVal);
	TEST_ASSERT_EQUAL(binary_length, received_data_bytes);
	TEST_ASSERT_EQUAL(SM_CHECKING_DATA_INTEGRITY, SM_getState());
}



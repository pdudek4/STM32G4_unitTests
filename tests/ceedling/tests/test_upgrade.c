

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
#include "mock_crypto.h"
#include "mock_state_machine.h"
#include "mock_utilities.h"

//Code Under Test
#include "upgrade.h"

//CUT dependencies
#include "slave_communication.h"
#include "bsp_crc.h"
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
void test_UPGRADE_framePasswordKey(void)
{
	uint8_t retVal;
	uint8_t testFr[48] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
	            		  'o', 'F', 'G', 'W', 'R', 'y', 'e', 's', 'i', 'K', 'u', 'D', 'x', 'j', '4', 'B',
	            		  0x00, 0x00, 0x01, 0x00, DEVICE_ID, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	BSP_watchdogRefresh_Ignore();
	CRYPTO_decryptBlockWithSecretKey_Ignore();
	CRYPTO_initPrivateKey_Ignore();
	clearFlashByte_Ignore();
	CRYPTO_decryptBlockWithPrivateKey_Ignore();

	//testowanie na podstawie wartości SM_setState_Expect, w razie innej wartości jest błąd
	//wszystkie dane prawidłowe
	FLASH_erase_ExpectAndReturn(FLASH_APP_START_ADDRESS, FLASH_STATUS_OK);
	SM_setState_Expect(SM_RECEIVING_BINARY);
	UPGRADE_framePasswordKey(testFr);

	//złe hasło bootloadera
	testFr[INFO_PASS_OFFSET] = 0xFF;
	SM_setState_Expect(SM_RESET_PROCEDURE);
	UPGRADE_framePasswordKey(testFr);
	testFr[INFO_PASS_OFFSET] = 'o';

	//zły rozmiar (wielokrotność 256) binarki
	testFr[INFO_BIN_INFO_OFFSET] = 0x05;
	SM_setState_Expect(SM_RESET_PROCEDURE);
	UPGRADE_framePasswordKey(testFr);
	testFr[INFO_BIN_INFO_OFFSET] = 0x00;

	//zły DEVICE_ID
	testFr[INFO_DEVICE_ID_OFFSET] = 0x05;
	SM_setState_Expect(SM_RESET_PROCEDURE);
	UPGRADE_framePasswordKey(testFr);
	testFr[INFO_DEVICE_ID_OFFSET] = DEVICE_ID;
}

//**********************************UPGRADE_processBinaryFrame**************************
void test_UPGRADE_processBinaryFrame(void)
{
	uint8_t retVal;
	uint8_t testFr[I2C_RX_COMMAND_IMAGE_DATA_LENGTH] = {0xE1, 0x00, 0x00, 0x01, 0x04, 0x00, 0xFF, 0xFF};

	binary_length = 0x10000;
	received_data_bytes = 0;
	CRYPTO_decryptBlockWithPrivateKey_Ignore();

	//mem address w normie
	FLASH_write_ExpectAnyArgsAndReturn(SUCCESS);
	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(SUCCESS, retVal);

	//mem address powyżej
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x10;
	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(ERROR, retVal);
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x00;

	//ostatni segment czy memAddr ma długość binarki
	testFr[I2C_FR_SEGMENT_OFFSET] = 0xFF;
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x00;
	received_data_bytes = 0xFF00;
	FLASH_write_ExpectAnyArgsAndReturn(SUCCESS);
	SM_setState_Expect(SM_CHECKING_DATA_INTEGRITY);
	retVal = UPGRADE_processBinaryFrame(testFr, 256);
	TEST_ASSERT_EQUAL(SUCCESS, retVal);
	testFr[I2C_FR_SEGMENT_OFFSET+1] = 0x00;
}


/*
 * upgrade.c
 *
 *  Created on: Sep 16, 2024
 *      Author: pdude
 */

#include "upgrade.h"

#include <string.h>

#include "bsp_crc.h"
#include "bsp_watchdog.h"
#include "configuration.h"
#include "crypto.h"
#include "bsp_flash.h"
#include "slave_communication.h"
#include "state_machine.h"
#include "utilities.h"

#define FLASH_WRITE_BUFFER_SIZE			256


const uint8_t g_pass[] = {'o', 'F', 'G', 'W', 'R', 'y', 'e', 's', 'i', 'K', 'u', 'D', 'x', 'j', '4', 'B'};    //oFGWRyesiKuDxj4B - password for bootloader

uint32_t binary_length;
uint32_t received_data_bytes;
uint16_t firmwareSegment;


/**
 * @brief	Sprawdza poprawność ramki
 * @param	pRx - dane ramki od jej początku
 * @param	dataLen - długość payloadu
 */
uint8_t UPGRADE_validateFrame(uint8_t *pRx, uint16_t dataLen, uint16_t refLen)
{

	uint16_t crcRead, crcCalc;

	if (dataLen == refLen)
	{
		//oblicz CRC
		crcRead = (pRx[I2C_FR_CRC_OFFSET+1] << 8) | pRx[I2C_FR_CRC_OFFSET];
		crcCalc = CRC_calculateArc16(&pRx[I2C_FR_PAYLOAD_OFFSET], dataLen);
		if (crcCalc != crcRead)
		{
#ifdef DEBUG
			if (crcRead == 0xFFFF)
				return SUCCESS;
#endif
			return ERROR;
		}
	}
	else
	{
		return ERROR;
	}

	return SUCCESS;
}

/**
 * @brief	Weryfikuje poprawność danych w bloku info w obrazie (odebranej ramce passwordAndKey)
 * @param	pRx - bufor ramki
 */
static uint8_t UPGRADE_verifyInfoBlock(const uint8_t *pRx)
{
	uint16_t mcu_id;

	if (memcmp(g_pass, &pRx[INFO_PASS_OFFSET], 16) != 0)
		return ERROR;

	binary_length = (uint32_t)((uint32_t)(pRx[32]) + (pRx[33] << 8) + (pRx[34] << 16) + (pRx[35] << 24));
	if (binary_length % FLASH_WRITE_BUFFER_SIZE != 0)
		return ERROR;

	mcu_id = (uint32_t)((uint32_t)(pRx[INFO_DEVICE_ID_OFFSET]) + (pRx[INFO_DEVICE_ID_OFFSET+1] << 8));
	if (mcu_id != DEVICE_ID)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief	Przetwarzanie ramki password key
 * @param	pRx - payload ramki
 */
void UPGRADE_framePasswordKey(uint8_t *pRx)
{
	BSP_watchdogRefresh();

	CRYPTO_decryptBlockWithSecretKey(&pRx[0]);
	CRYPTO_initPrivateKey(&pRx[0]);
	CRYPTO_decryptBlockWithPrivateKey(&pRx[INFO_PASS_OFFSET]);
	CRYPTO_decryptBlockWithPrivateKey(&pRx[INFO_BIN_INFO_OFFSET]);
	BSP_watchdogRefresh();

	if (UPGRADE_verifyInfoBlock(pRx) == SUCCESS)
	{
		flash_status retVal;

//		clearFlashByte();	//todo błąd GTtest z EEPROM CRC! oczekuje prawdziwego adresu
		retVal = FLASH_erase(FLASH_APP_START_ADDRESS);
		if (retVal != FLASH_STATUS_OK)
		{
			SM_setState(SM_RESET_PROCEDURE);
			return;
		}
		SM_setState(SM_RECEIVING_BINARY);
	}
	else
	{
		SM_setState(SM_RESET_PROCEDURE);
	}
}

/**
 * @brief	Przetwarzanie ramki z zawartością firmware'u
 * @param	pRx - bufor z ramką
 * @param	dataLen - długość payloadu (w IMD6CH=256)
 */
uint8_t UPGRADE_processBinaryFrame(uint8_t *pRx, uint16_t dataLen)
{
	uint32_t memAddr;
	uint8_t retVal;

	firmwareSegment = (pRx[I2C_FR_SEGMENT_OFFSET+1] << 8) | pRx[I2C_FR_SEGMENT_OFFSET];
	memAddr = FLASH_APP_START_ADDRESS + firmwareSegment * FLASH_WRITE_BUFFER_SIZE;
	received_data_bytes += dataLen;

	if (memAddr + dataLen > FLASH_APP_END_ADDRESS + 1)
	{
		ASSERT(false);
		return ERROR;
	}

	//odszyfrowanie po 16B; pętla 16 razy
	for (uint16_t i=0; i<dataLen; i+=16)
	{
		CRYPTO_decryptBlockWithPrivateKey(&pRx[I2C_FR_PAYLOAD_OFFSET+i]);
	}

	//programowanie 256 bajtów
	retVal = FLASH_write(memAddr, (uint64_t*)&pRx[I2C_FR_PAYLOAD_OFFSET], dataLen/sizeof(uint64_t));
	if (retVal != SUCCESS)
	{
		ASSERT(false);
		return ERROR;
	}

	//sprawdzenie, czy ostatnia ramka
	memAddr += dataLen;
	if ((memAddr - FLASH_APP_START_ADDRESS) == binary_length && received_data_bytes == binary_length)
	{
		SM_setState(SM_CHECKING_DATA_INTEGRITY);
	}

	return SUCCESS;
}

/**
 * @brief	Zwraca ostatnio zaprogramowany segment (po 256 B) pamięi flash.
 */
uint16_t UPGRADE_getFirmwareSegment(void)
{
	return firmwareSegment;
}


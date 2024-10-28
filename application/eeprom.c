/*
 * eeprom.c
 *
 *  Created on: Sep 16, 2024
 *      Author: pdude
 */

#include "eeprom.h"

#include <string.h>

#include "bsp_crc.h"
#include "bsp_i2c.h"
#include "eeprom_24AA64.h"
#include "utilities.h"


#define EEPROM_TIMEOUT	1000


uint8_t EEPROM_write(uint16_t address, uint8_t* data, uint16_t length)
{
	uint8_t retVal = EEPROM_OK;
	uint16_t remainingBytes;
	uint16_t index = 0;

	if (((address+length) > EEPROM_LENGTH) || length == 0)
		return EEPROM_ERROR;

	while (length - index > 0)
	{
		if ((length - index) >= EEPROM_PAGE_SIZE)
		{
			remainingBytes = EEPROM_PAGE_SIZE;
		}
		else
		{
			remainingBytes = (length-index) % EEPROM_PAGE_SIZE;
		}

		if (index == 0 && (EEPROM_PAGE_SIZE < (address%EEPROM_PAGE_SIZE + remainingBytes)))
		{
			remainingBytes = EEPROM_PAGE_SIZE - address%EEPROM_PAGE_SIZE;
		}

		DELAY_MS(10);
		retVal |= (BSP_I2C_memoryWrite(EEPROM_DEVICE_ADDRESS, address+index, EEPROM_ADDRESS_LENGTH,
				&data[index], remainingBytes, EEPROM_TIMEOUT) == HAL_OK?EEPROM_OK:EEPROM_ERROR);
		index += remainingBytes;
	}
	return retVal;
}

uint8_t EEPROM_read(uint16_t address, uint8_t* data, uint16_t length)
{
	return (BSP_I2C_memoryRead(EEPROM_DEVICE_ADDRESS, address, EEPROM_ADDRESS_LENGTH, data, length, EEPROM_TIMEOUT) == HAL_OK?EEPROM_OK:EEPROM_ERROR);
}

uint8_t EEPROM_writeBlocks(uint16_t address, uint8_t* data, uint8_t* compare_array, uint16_t length, uint8_t includeCRC)
{
	uint8_t retVal = EEPROM_OK;

	if (address+length > EEPROM_LENGTH/2)
		return EEPROM_ERROR;

	if (includeCRC == EEPROM_ADD_CRC && length > 2)
	{
		*(uint16_t*)&data[length-2] = CRC_calculateArc16(data, length-2);
	}

	retVal |= EEPROM_write(EEPROM_BLOCK_1 + address, data, length);
	DELAY_MS(5);
	retVal |= EEPROM_write(EEPROM_BLOCK_2 + address, data, length);
	DELAY_MS(5);

	//sprawdzenie

	uint8_t* compare_array1 = (uint8_t*)&compare_array[0];
	uint8_t* compare_array2 = (uint8_t*)&compare_array[length];
	//odczyt bloku 1
	retVal |= EEPROM_read(EEPROM_BLOCK_1 + address, compare_array1, length);
	DELAY_MS(5);
	//odczyt bloku 2
	retVal |= EEPROM_read(EEPROM_BLOCK_2 + address, compare_array2, length);
	DELAY_MS(5);
	//porównanie wartości
	if (memcmp(compare_array1, compare_array2, length) != 0)
	{
		retVal = EEPROM_ERROR;
	}
	if (includeCRC == EEPROM_ADD_CRC && length > 2 && retVal == EEPROM_OK)
	{
		if (CRC_calculateArc16(compare_array1, length) != 0)
		{
			retVal = EEPROM_ERROR;
		}
	}

	return retVal;
}

uint8_t EEPROM_readBlocks(uint16_t address, uint8_t* data, uint8_t* compare_array, uint16_t length, uint8_t includeCRC)
{
	uint8_t retVal = EEPROM_OK;

	//warunek zmieszczenia się w eepromie
	if (address+length > EEPROM_LENGTH/2) return EEPROM_ERROR;

	//bez wbudowanego sprawdzenia crc
	if (includeCRC == EEPROM_NO_CRC)
	{
		//odczyt bloku 1

		retVal |= EEPROM_read(EEPROM_BLOCK_1 + address, data, length);
		memcpy(compare_array, data, length);
		DELAY_MS(5);
		//odczyt bloku 2
		retVal |= EEPROM_read(EEPROM_BLOCK_2 + address, data, length);
		//porównanie wartości
		if (memcmp(data, compare_array, length) != 0)
			retVal = EEPROM_ERROR;

		DELAY_MS(5);
	}
	//z wbudowanym sprawdzeniem crc
	else if (includeCRC == EEPROM_ADD_CRC && length > 2)
	{
		//odczyt bloku 1
		retVal |= EEPROM_read(EEPROM_BLOCK_1 + address, data, length);
		if (CRC_calculateArc16(data, length) == 0)
		{
			DELAY_MS(5);
			//zapis do bloku 2
			if (EEPROM_read(EEPROM_BLOCK_2 + address, compare_array, length) == EEPROM_OK)
			{
				if (memcmp(data, compare_array, length) != 0)
				{
					retVal |= EEPROM_write(EEPROM_BLOCK_2 + address, data, length);
				}
			}
			else
			{
				retVal = EEPROM_ERROR;
			}
		}
		else
		{
			DELAY_MS(5);
			//odczyt bloku 2
			retVal |= EEPROM_read(EEPROM_BLOCK_2 + address, data, length);
			if (CRC_calculateArc16(data, length) == 0)
			{
				DELAY_MS(5);
				//zapis do bloku 1
				retVal |= EEPROM_write(EEPROM_BLOCK_1 + address, data, length);
			}
			else
			{
				retVal = EEPROM_ERROR;
			}
		}
	}

	return retVal;
}


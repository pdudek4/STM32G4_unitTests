/*
 * bsp_flash.c
 *
 *  Created on: Oct 24, 2024
 *      Author: przemyslaw
 */

#include "bsp_flash.h"

/**
 * @brief   This function erases the memory.
 * @param   address: First address to be erased (the last is the end of the flash).
 * @return  status: Report about the success of the erasing.
 */
flash_status FLASH_erase(uint32_t address)
{
	uint32_t flashError;
	FLASH_EraseInitTypeDef erase_init;
	HAL_StatusTypeDef resFlash;

	erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_init.Page = (address - FLASH_START_ADDRESS) / FLASH_PAGE_SIZE;
	erase_init.Banks = FLASH_BANK_1;
	erase_init.NbPages = (FLASH_APP_END_ADDRESS + 1 - address) / FLASH_PAGE_SIZE;

	HAL_FLASH_Unlock();
	resFlash = HAL_FLASHEx_Erase(&erase_init, &flashError);
	HAL_FLASH_Lock();
	if (resFlash == HAL_OK)
	{
		return FLASH_STATUS_OK;
	}

	return FLASH_STATUS_ERROR;
}

/**
 * @brief   This function flashes the memory.
 * @param   address: First address to be written to.
 * @param   *data:   Array of the data that we want to write.
 * @param   *length: Size of the array.
 * @return  status: Report about the success of the writing.
 */
flash_status FLASH_write(uint32_t address, uint64_t *data, uint32_t length)
{
	flash_status status = FLASH_STATUS_OK;
	HAL_FLASH_Unlock();

	/* Loop through the array. */
	for (uint32_t i = 0; (i < length) && (FLASH_STATUS_OK == status); i++)
	{
		/* If we reached the end of the memory, then report an error and don't do anything else.*/
		if (FLASH_APP_END_ADDRESS <= address)
		{
			status |= FLASH_STATUS_ERROR_SIZE;
		}
		else
		{
			/* The actual flashing. If there is an error, then report it. */
			if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data[i]))
			{
				status |= FLASH_STATUS_ERROR_WRITE;
			}
			/* Read back the content of the memory. If it is wrong, then report an error. */
			if ((data[i]) != (*(volatile uint64_t*)address))
			{
				status |= FLASH_STATUS_ERROR_READBACK;
			}

			/* Shift the address by a word. */
			address += 8u;
		}
	}

	HAL_FLASH_Lock();

	return status;
}

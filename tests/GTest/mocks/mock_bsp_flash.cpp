/*
 * mock_bsp_flash.cpp
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#include "mock_bsp_flash.h"
#include "bsp_flash.h"


bsp_flash_Mock *bsp_flash_MockObj;


//Create mock functions definitions for link-time replacement
extern "C"
{
	flash_status FLASH_erase(uint32_t address)
    {
        return bsp_flash_MockObj->FLASH_erase(address);
    }

	flash_status FLASH_write(uint32_t address, uint64_t *data, uint32_t length)
    {
        return bsp_flash_MockObj->FLASH_write(address, data, length);
    }
}



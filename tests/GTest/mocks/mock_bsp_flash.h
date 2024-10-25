/*
 * mock_bsp_flash.h
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#ifndef GTEST_MOCKS_MOCK_BSP_FLASH_H_
#define GTEST_MOCKS_MOCK_BSP_FLASH_H_


#include <stdint.h>
#include <gmock/gmock.h>
#include "bsp_flash.h"


//create the MOCKS
class bsp_flash_Functions
{
    public:
        virtual ~bsp_flash_Functions(){}
        virtual flash_status FLASH_erase(uint32_t address) = 0;
        virtual flash_status FLASH_write(uint32_t address, uint64_t *data, uint32_t length) = 0;
};

class bsp_flash_Mock : public bsp_flash_Functions
{
    public:
        MOCK_METHOD(flash_status, FLASH_erase, (uint32_t address), (override));
        MOCK_METHOD(flash_status, FLASH_write, (uint32_t address, uint64_t *data, uint32_t length), (override));
};

//create global mock object
extern bsp_flash_Mock *bsp_flash_MockObj;


#endif /* GTEST_MOCKS_MOCK_BSP_FLASH_H_ */

/*
 * mock_bsp_crc.h
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#ifndef GTEST_MOCKS_MOCK_BSP_CRC_H_
#define GTEST_MOCKS_MOCK_BSP_CRC_H_


#include <stdint.h>
#include <gmock/gmock.h>
#include "bsp_crc.h"


//create the MOCKS
class bsp_crc_Functions
{
    public:
        virtual ~bsp_crc_Functions(){}
        virtual uint16_t CRC_calculateArc16(uint8_t *data, uint16_t length) = 0;
};

class bsp_crc_Mock : public bsp_crc_Functions
{
    public:
        MOCK_METHOD(uint16_t, CRC_calculateArc16, (uint8_t *data, uint16_t length), (override));
};

//create global mock object
extern bsp_crc_Mock *bsp_crc_MockObj;


#endif /* GTEST_MOCKS_MOCK_BSP_CRC_H_ */

/*
 * mock_bsp_crc.cpp
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#include "mock_bsp_crc.h"
#include "bsp_crc.h"


bsp_crc_Mock *bsp_crc_MockObj;


//Create mock functions definitions for link-time replacement
extern "C"
{
	uint16_t CRC_calculateArc16(uint8_t *data, uint16_t length)
    {
        return bsp_crc_MockObj->CRC_calculateArc16(data, length);
    }
}



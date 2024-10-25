/*
 * bsp_crc.h
 *
 *  Created on: Mar 22, 2024
 *      Author: w.klimiuk
 */

#ifndef BSP_BSP_CRC_BSP_CRC_H_
#define BSP_BSP_CRC_BSP_CRC_H_

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
//#include "crc.h"
#endif
#include <stdint.h>


uint16_t CRC_calculateArc16(uint8_t *data, uint16_t length);


#endif /* BSP_BSP_CRC_BSP_CRC_H_ */

/*
 * upgrade.h
 *
 *  Created on: Sep 16, 2024
 *      Author: pdude
 */

#ifndef UPGRADE_H_
#define UPGRADE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#else
#include "testing_defines.h"
#endif
#include <stdint.h>

#define FLASH_WRITE_BUFFER_SIZE 	256
#define INFO_BLOCK_SIZE 			48

#define INFO_PASS_OFFSET			16
#define INFO_BIN_INFO_OFFSET		32
#define INFO_DEVICE_ID_OFFSET		36
#define INFO_HW_VERSION_OFFSET		38


uint8_t UPGRADE_validateFrame(uint8_t *pRx, uint16_t dataLen, uint16_t refLen);
void UPGRADE_framePasswordKey(uint8_t *pRx);
uint8_t UPGRADE_processBinaryFrame(uint8_t *pRx, uint16_t length);
uint16_t UPGRADE_getFirmwareSegment(void);

#ifdef __cplusplus
}
#endif
#endif /* UPGRADE_H_ */

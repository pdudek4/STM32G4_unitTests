/*
 * bsp_flash.h
 *
 *  Created on: Oct 24, 2024
 *      Author: przemyslaw
 */

#ifndef BSP_BSP_FLASH_H_
#define BSP_BSP_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#endif


/* Start and end addresses of the user application. */
#define FLASH_START_ADDRESS 		((uint32_t)0x08000000UL)
#define FLASH_APP_START_ADDRESS 	((uint32_t)0x08005800UL)
#define FLASH_APP_VECTOR_ADDRESS 	FLASH_APP_START_ADDRESS
#define FLASH_APP_CRC_ADDRESS 		((uint32_t)0x08005A00UL)
#define FLASH_APP_END_ADDRESS		((uint32_t)0x0801FFFFUL)

#define FLASH_APP_PAGE_SIZE			2048U


/* Status report for the functions. */
typedef enum
{
	FLASH_STATUS_OK = 0x00u, /**< The action was successful. */
	FLASH_STATUS_ERROR_SIZE = 0x01u, /**< The binary is too big. */
	FLASH_STATUS_ERROR_WRITE = 0x02u, /**< Writing failed. */
	FLASH_STATUS_ERROR_READBACK = 0x04u, /**< Writing was successful, but the content of the memory is wrong. */
	FLASH_STATUS_ERROR = 0xFFu /**< Generic error. */
} flash_status;


flash_status FLASH_erase(uint32_t address);
flash_status FLASH_write(uint32_t address, uint64_t *data, uint32_t length);
//flash_status FLASH_write(uint32_t address, uint8_t *data, uint32_t length);


#ifdef __cplusplus
}
#endif
#endif /* BSP_BSP_FLASH_H_ */

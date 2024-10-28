/*
 * bsp_i2c.h
 *
 *  Created on: Mar 22, 2024
 *      Author: w.klimiuk
 */

#ifndef BSP_BSP_I2C_BSP_I2C_H_
#define BSP_BSP_I2C_BSP_I2C_H_

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#else
#include "testing_defines.h"
#endif
#include <stdint.h>


typedef enum
{
	I2C_EEPROM,
	I2C_COMMUNICATION
} application_i2c_handlers_e;


I2C_HandleTypeDef* BSP_I2CGetHandler(application_i2c_handlers_e handler_type);
void BSP_I2CRestart(application_i2c_handlers_e handler);
uint32_t BSP_I2CGetError(application_i2c_handlers_e handler);

void BSP_I2CSet7BitSlaveAddress(uint8_t address);
HAL_StatusTypeDef BSP_I2CStartListening();
HAL_StatusTypeDef BSP_I2CReceiveFirstData(uint8_t* data);
HAL_StatusTypeDef BSP_I2CReceiveNextData(uint8_t* data);
HAL_StatusTypeDef BSP_I2CReceiveLastData(uint8_t* data);
HAL_StatusTypeDef BSP_I2CTransmitData(uint8_t* data, uint16_t length);

HAL_StatusTypeDef BSP_I2C_transmitData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout);
HAL_StatusTypeDef BSP_I2C_receiveData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout);
HAL_StatusTypeDef BSP_I2C_memoryWrite(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout);
HAL_StatusTypeDef BSP_I2C_memoryRead(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout);


#endif /* BSP_BSP_I2C_BSP_I2C_H_ */

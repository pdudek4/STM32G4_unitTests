/*
 * stub_bsp_i2c.cpp
 *
 *  Created on: Oct 28, 2024
 *      Author: pdude
 */

#include "bsp_i2c.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;


static I2C_HandleTypeDef* g_i2c_handlers_array[2] =
{
		&hi2c1,
		&hi2c2,
};

I2C_HandleTypeDef* BSP_I2CGetHandler(application_i2c_handlers_e handler_type)
{
	return g_i2c_handlers_array[handler_type];
}

void BSP_I2CRestart(application_i2c_handlers_e handler)
{
//	if (HAL_I2C_DeInit(BSP_I2CGetHandler(handler)) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	if (HAL_I2C_Init(BSP_I2CGetHandler(handler)) != HAL_OK)
//	{
//		Error_Handler();
//	}
}

uint32_t BSP_I2CGetError(application_i2c_handlers_e handler)
{
//	return HAL_I2C_GetError(BSP_I2CGetHandler(handler));
	return HAL_OK;
}

//------------------SLAVE------------------------------
//-----------------------------------------------------
void BSP_I2CSet7BitSlaveAddress(uint8_t address)
{
//	__HAL_I2C_DISABLE(BSP_I2CGetHandler(I2C_COMMUNICATION));
//	BSP_I2CGetHandler(I2C_COMMUNICATION)->Instance->OAR1 &= ~I2C_OAR1_OA1EN;
//	BSP_I2CGetHandler(I2C_COMMUNICATION)->Instance->OAR1 = (I2C_OAR1_OA1EN | (address << 1));
//	__HAL_I2C_ENABLE(BSP_I2CGetHandler(I2C_COMMUNICATION));
}

HAL_StatusTypeDef BSP_I2CStartListening()
{
//	HAL_StatusTypeDef status = HAL_I2C_EnableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
//	if (status != HAL_OK)
//	{
//		status = HAL_I2C_DisableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
//		status |= HAL_I2C_EnableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
//	}
//	return status;
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2CReceiveFirstData(uint8_t* data)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2CReceiveNextData(uint8_t* data)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2CReceiveLastData(uint8_t* data)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2CTransmitData(uint8_t* data, uint16_t length)
{
	return HAL_OK;
}

//------------------EEPROM-----------------------------
//-----------------------------------------------------
HAL_StatusTypeDef BSP_I2C_transmitData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2C_receiveData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2C_memoryWrite(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_OK;
}

HAL_StatusTypeDef BSP_I2C_memoryRead(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_OK;
}




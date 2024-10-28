/*
 * bsp_i2c.c
 *
 *  Created on: Mar 22, 2024
 *      Author: w.klimiuk
 */

#include "bsp_i2c.h"

#include "i2c.h"


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
	if (HAL_I2C_DeInit(BSP_I2CGetHandler(handler)) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_I2C_Init(BSP_I2CGetHandler(handler)) != HAL_OK)
	{
		Error_Handler();
	}
}

uint32_t BSP_I2CGetError(application_i2c_handlers_e handler)
{
	return HAL_I2C_GetError(BSP_I2CGetHandler(handler));
}

//------------------SLAVE------------------------------
//-----------------------------------------------------
void BSP_I2CSet7BitSlaveAddress(uint8_t address)
{
	__HAL_I2C_DISABLE(BSP_I2CGetHandler(I2C_COMMUNICATION));
	BSP_I2CGetHandler(I2C_COMMUNICATION)->Instance->OAR1 &= ~I2C_OAR1_OA1EN;
	BSP_I2CGetHandler(I2C_COMMUNICATION)->Instance->OAR1 = (I2C_OAR1_OA1EN | (address << 1));
	__HAL_I2C_ENABLE(BSP_I2CGetHandler(I2C_COMMUNICATION));
}

HAL_StatusTypeDef BSP_I2CStartListening()
{
	HAL_StatusTypeDef status = HAL_I2C_EnableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
	if (status != HAL_OK)
	{
		status = HAL_I2C_DisableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
		status |= HAL_I2C_EnableListen_IT(BSP_I2CGetHandler(I2C_COMMUNICATION));
	}
	return status;
}

HAL_StatusTypeDef BSP_I2CReceiveFirstData(uint8_t* data)
{
	return HAL_I2C_Slave_Seq_Receive_IT(BSP_I2CGetHandler(I2C_COMMUNICATION), data, 1, I2C_FIRST_FRAME);
}

HAL_StatusTypeDef BSP_I2CReceiveNextData(uint8_t* data)
{
	return HAL_I2C_Slave_Seq_Receive_IT(BSP_I2CGetHandler(I2C_COMMUNICATION), data, 1, I2C_FIRST_FRAME);
}

HAL_StatusTypeDef BSP_I2CReceiveLastData(uint8_t* data)
{
	return HAL_I2C_Slave_Seq_Receive_IT(BSP_I2CGetHandler(I2C_COMMUNICATION), data, 1, I2C_FIRST_FRAME);
}

HAL_StatusTypeDef BSP_I2CTransmitData(uint8_t* data, uint16_t length)
{
	return HAL_I2C_Slave_Seq_Transmit_IT(BSP_I2CGetHandler(I2C_COMMUNICATION), data, length, I2C_FIRST_AND_LAST_FRAME);
}

//------------------EEPROM-----------------------------
//-----------------------------------------------------
HAL_StatusTypeDef BSP_I2C_transmitData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_I2C_Master_Transmit(BSP_I2CGetHandler(I2C_EEPROM), device_address, data, length, timeout);
}

HAL_StatusTypeDef BSP_I2C_receiveData(uint8_t device_address, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_I2C_Master_Receive(BSP_I2CGetHandler(I2C_EEPROM), device_address, data, length, timeout);
}

HAL_StatusTypeDef BSP_I2C_memoryWrite(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_I2C_Mem_Write(BSP_I2CGetHandler(I2C_EEPROM), device_address, memory_address, address_length, data, length, timeout);
}

HAL_StatusTypeDef BSP_I2C_memoryRead(uint8_t device_address, uint16_t memory_address, uint8_t address_length, uint8_t* data, uint16_t length, uint32_t timeout)
{
	return HAL_I2C_Mem_Read(BSP_I2CGetHandler(I2C_EEPROM), device_address, memory_address, address_length, data, length, timeout);
}


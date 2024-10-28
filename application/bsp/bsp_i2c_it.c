/*
 * bsp_i2c_it.c
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#include "application_timers.h"
#include "bsp_i2c.h"
//#include "configuration.h"
//#include "slave_communication.h"


/**
 * @brief	When address slave I2C match, HAL_I2C_AddrCallback() is executed and users can
 * 			add their own code to check the Address Match Code and the transmission direction
 * 			request by master (Write/Read).
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t transferDirection, uint16_t addrMatchCode)
{
//	if (BSP_I2CGetHandler(I2C_COMMUNICATION) == hi2c && CONF_getMCUAddress() == (addrMatchCode >> 1))
//	{
//		restartTimer(I2C_HANG_TIMER);
//		if (transferDirection == I2C_DIRECTION_RECEIVE)
//		{
//			COMM_transmissionRequest();
//		}
//		else if (transferDirection == I2C_DIRECTION_TRANSMIT)
//		{
//			COMM_receptionRequest();
//		}
//	}
}

/**
 * @brief	At reception end of transfer, HAL_I2C_SlaveRxCpltCallback() is executed.
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
//	COMM_receptionCompleted();
}

/**
 * @brief	At transmission end of current frame transfer, HAL_I2C_SlaveTxCpltCallback() is executed.
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
//	COMM_transmissionCompleted();
}

/**
 * @brief	In case of transfer Error, HAL_I2C_ErrorCallback() function is executed.
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	BSP_I2CStartListening();
}

/**
 * @brief	At Listen mode end HAL_I2C_ListenCpltCallback() is executed.
 */
void HAL_I2C_ListenCpltCallback (I2C_HandleTypeDef *hi2c)
{
	BSP_I2CStartListening();
}

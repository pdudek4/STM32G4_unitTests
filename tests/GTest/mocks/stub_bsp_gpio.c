/*
 * stub_bsp_gpio.c
 *
 *  Created on: Oct 28, 2024
 *      Author: pdude
 */


#include "bsp_gpio.h"


uint8_t BSP_readAddressDetectPins()
{
//	uint8_t output = 0;
//	output |= HAL_GPIO_ReadPin(ADDRESS_DETECT_1_PORT, ADDRESS_DETECT_1_PIN) << 0;
//	output |= HAL_GPIO_ReadPin(ADDRESS_DETECT_2_PORT, ADDRESS_DETECT_2_PIN) << 1;
//	output |= HAL_GPIO_ReadPin(ADDRESS_DETECT_3_PORT, ADDRESS_DETECT_3_PIN) << 2;
//	return output;

	return 3;
}

void BSP_toggleHeartbeat(void)
{
//	HAL_GPIO_TogglePin(LED_MPU_PORT, LED_MPU_PIN);
}


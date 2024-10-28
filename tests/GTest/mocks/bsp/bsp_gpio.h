/*
 * bsp_gpio.h
 *
 *  Created on: Mar 22, 2024
 *      Author: w.klimiuk
 */

#ifndef BSP_BSP_GPIO_BSP_GPIO_H_
#define BSP_BSP_GPIO_BSP_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#else
#include "testing_defines.h"
#endif


#define ADDRESS_DETECT_1_PORT	GPIOC
#define ADDRESS_DETECT_1_PIN	GPIO_PIN_13

#define ADDRESS_DETECT_2_PORT	GPIOC
#define ADDRESS_DETECT_2_PIN	GPIO_PIN_14

#define ADDRESS_DETECT_3_PORT	GPIOC
#define ADDRESS_DETECT_3_PIN	GPIO_PIN_15

#define LED_MPU_PORT			GPIOB
#define LED_MPU_PIN				GPIO_PIN_12


enum
{
	OUTPUT_OFF,
	OUTPUT_ON
};


uint8_t BSP_readAddressDetectPins(void);
void BSP_toggleHeartbeat(void);


#ifdef __cplusplus
}
#endif
#endif /* BSP_BSP_GPIO_BSP_GPIO_H_ */

/*
 * utilities.c
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#include "utilities.h"

#include "hal_include.h"
//#include "iwdg.h"

#ifdef USE_HAL_DRIVER
void offsetInterruptVector()
{
	SCB->VTOR = (FLASH_BASE | 0x8000);
//	SCB->VTOR = (FLASH_BASE | APP_OFFSET);
}

void restartDevice()
{
	NVIC_SystemReset();
}

void disableAppInterrupts()
{
//	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); //todo
//	HAL_TIM_Base_MspDeInit(&htim1);
//	HAL_TIM_Base_MspDeInit(&htim4);
}

void ASSERT(bool expr)
{
#ifdef USE_HAL_DRIVER
	if (!expr)
	{
		__asm__("bkpt");
	}
#endif
}
#endif

void DELAY_MS(uint32_t ms)
{
#ifdef USE_HAL_DRIVER
	HAL_Delay(ms);
#endif
}



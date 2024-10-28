/*
 * utilities.c
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#include "utilities.h"

#include "hal_include.h"
//#include "iwdg.h"

void offsetInterruptVector()
{
#ifdef USE_HAL_DRIVER
	SCB->VTOR = (FLASH_BASE | 0x8000);
//	SCB->VTOR = (FLASH_BASE | APP_OFFSET);
#endif
}

void restartDevice()
{
#ifdef USE_HAL_DRIVER
	NVIC_SystemReset();
#endif
}

void disableAppInterrupts()
{

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

void DELAY_MS(uint32_t ms)
{
#ifdef USE_HAL_DRIVER
	HAL_Delay(ms);
#endif
}



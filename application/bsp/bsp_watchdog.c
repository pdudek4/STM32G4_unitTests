/*
 * bsp_watchdog.c
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#ifdef USE_HAL_DRIVER
#include "iwdg.h"
#endif
#include "bsp_watchdog.h"


void BSP_watchdogFreezeInDebug()
{
#ifdef DEBUG
	__HAL_DBGMCU_FREEZE_IWDG();
#endif
}

void BSP_watchdogRefresh()
{
	HAL_IWDG_Refresh(&hiwdg);
}

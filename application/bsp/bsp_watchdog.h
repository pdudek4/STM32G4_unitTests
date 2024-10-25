/*
 * bsp_watchdog.h
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#ifndef BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_
#define BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#else
#include "testing_defines.h"
#endif


void BSP_watchdogFreezeInDebug();
void BSP_watchdogRefresh();


#endif /* BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_ */

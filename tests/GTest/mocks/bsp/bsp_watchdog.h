/*
 * bsp_watchdog.h
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#ifndef BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_
#define BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#else
#include "testing_defines.h"
#endif


void BSP_watchdogFreezeInDebug(void);
void BSP_watchdogRefresh(void);


#ifdef __cplusplus
}
#endif
#endif /* BSP_BSP_WATCHDOG_BSP_WATCHDOG_H_ */

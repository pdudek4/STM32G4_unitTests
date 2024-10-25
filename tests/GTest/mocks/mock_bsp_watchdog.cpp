/*
 * mock_bswp_watchdog.cpp
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#include "mock_bsp_watchdog.h"
#include "bsp_watchdog.h"


bsp_watchdog_Mock *bsp_watchdog_MockObj;


//Create mock functions definitions for link-time replacement
extern "C"
{
	void BSP_watchdogFreezeInDebug(void)
    {
        return bsp_watchdog_MockObj-> BSP_watchdogFreezeInDebug();
    }

	void BSP_watchdogRefresh(void)
    {
        return bsp_watchdog_MockObj->BSP_watchdogRefresh();
    }
}



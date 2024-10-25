/*
 * application.c
 *
 *  Created on: Sep 13, 2024
 *      Author: pdude
 */

#include "application.h"
#include "bsp_watchdog.h"
#include "bsp_flash.h"
#include "bsp_crc.h"
#include "state_machine.h"


#ifndef UNIT_TEST
__attribute__((section(".appCrc")))
volatile appCrcInfo_ts appCrcInfo = {0};
#else
const volatile appCrcInfo_ts appCrcInfo = {0};
#endif



void APP_preinitialize(void)
{

}

void APP_initialize(void)
{
    BSP_watchdogFreezeInDebug();
}

void APP_process(void)
{
	BSP_watchdogRefresh();

//	if (checkTimerTimeout(TIMER_HEARTBEAT))
//	{
//		restartTimer(TIMER_HEARTBEAT);
//		BSP_toggleHeartbeat();
//	}
//
//    if (checkTimerTimeout(I2C_HANG_TIMER))
//    {
//        restartTimer(I2C_HANG_TIMER);
//        BSP_I2CRestart(I2C_COMMUNICATION);
//        BSP_I2CSet7BitSlaveAddress(CONF_getMCUAddress());
//        BSP_I2CStartListening();
//    }
//
//    if (checkTimerTimeout(UPGRADE_TIMER))
//	{
//    	restartTimer(UPGRADE_TIMER);
//    	SM_setState(SM_RESET_PROCEDURE);
//	}
//
//    if (BSP_I2CGetError(I2C_COMMUNICATION) != 0)
//    {
//        BSP_I2CRestart(I2C_COMMUNICATION);
//        BSP_I2CSet7BitSlaveAddress(CONF_getMCUAddress());
//        BSP_I2CStartListening();
//    }
//
//    COMM_processReceivedCommands();
    SM_process();
}

/**
 * @brief	Sprawdza CRC aplikacji
 */
uint8_t APP_checkAppCrc(void)
{
	uint16_t crcCalc;

	if (appCrcInfo.crcApp == 0x0000)	//obejście gdy kod wgrany przez debugger
		return SUCCESS;

	if (appCrcInfo.appCrcStartCalcAddr < FLASH_APP_START_ADDRESS)
		return ERROR;

	if (appCrcInfo.appCrcStartCalcAddr + appCrcInfo.appCrcCalcSize >= FLASH_APP_END_ADDRESS)
		return ERROR;

	crcCalc = CRC_calculateArc16((uint8_t*)appCrcInfo.appCrcStartCalcAddr, appCrcInfo.appCrcCalcSize);

	if (crcCalc != appCrcInfo.crcApp)
		return ERROR;

	return SUCCESS;
}



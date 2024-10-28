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
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "state_machine.h"
#include "application_timers.h"
#include "configuration.h"
#include "slave_communication.h"
#include "utilities.h"



#ifndef UNIT_TEST
__attribute__((section(".appCrc")))
volatile appCrcInfo_ts appCrcInfo = {0};
#else
const volatile appCrcInfo_ts appCrcInfo = {0};
#endif

typedef void (*fnc_ptr)(void);

bool updateFlag = false;


void APP_preinitialize(void)
{

}

void APP_initialize(void)
{
	uint8_t retVal;
    BSP_watchdogFreezeInDebug();

    //sprawdzenie czy przejść do aktualizacji
    retVal = CONF_checkUpgrade();
    if (retVal == SUCCESS)
    {
    	updateFlag = true;
    }
//    else
//    {
//    	//sprawdzenie CRC aplikacji
//    	retVal = APP_checkAppCrc();
//    	if (retVal != SUCCESS)
//    		updateFlag = true;
//    }

//    if (updateFlag == true)
//    {
//    	CONF_readMCUAddress();
//		while (CONF_isMCUAddressCorrect() == false)	//todo potrzebny timeout
//		{
//			CONF_readMCUAddress();
//			BSP_watchdogRefresh();
//		}
//
//		CONF_initDeviceVersions();
//
//		BSP_I2CSet7BitSlaveAddress(CONF_getMCUAddress());
//		BSP_I2CStartListening();
//
//		initTimers();
//		SM_init();
//	}
//	else
//	{
//		DELAY_MS(100);
//		APP_jumpToApplication();
//	}
}

void APP_process(void)
{
	BSP_watchdogRefresh();

	if (checkTimerTimeout(TIMER_HEARTBEAT))
	{
		restartTimer(TIMER_HEARTBEAT);
		BSP_toggleHeartbeat();
	}

    if (checkTimerTimeout(I2C_HANG_TIMER))
    {
        restartTimer(I2C_HANG_TIMER);
        BSP_I2CRestart(I2C_COMMUNICATION);
        BSP_I2CSet7BitSlaveAddress(CONF_getMCUAddress());
        BSP_I2CStartListening();
    }

    if (checkTimerTimeout(UPGRADE_TIMER))
	{
    	restartTimer(UPGRADE_TIMER);
    	SM_setState(SM_RESET_PROCEDURE);
	}

    if (BSP_I2CGetError(I2C_COMMUNICATION) != 0)
    {
        BSP_I2CRestart(I2C_COMMUNICATION);
        BSP_I2CSet7BitSlaveAddress(CONF_getMCUAddress());
        BSP_I2CStartListening();
    }

    COMM_processReceivedCommands();
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

/**
 * @brief	Skok do aplikacji głównej
 */
#pragma GCC push_options
#pragma GCC optimize ("O0")
//	optimization removes critical code from being linked so disable it
void APP_jumpToApplication(void)
{
#ifndef UNIT_TEST
	fnc_ptr jump_to_app;

	jump_to_app = (fnc_ptr) (*(volatile uint32_t*) (FLASH_APP_VECTOR_ADDRESS + 4U));
	__set_PRIMASK(1);
	__disable_irq();
	__set_CONTROL(0);
	HAL_RCC_DeInit();
	HAL_DeInit();
	/* Change the main stack pointer. */
	SCB->VTOR = FLASH_APP_VECTOR_ADDRESS;
	__set_MSP(*(volatile uint32_t*) FLASH_APP_VECTOR_ADDRESS);
	jump_to_app();

	while(1);
#endif
}


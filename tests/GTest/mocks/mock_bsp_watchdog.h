/*
 * mock_bsp_watchdog.h
 *
 *  Created on: Oct 25, 2024
 *      Author: pdude
 */

#ifndef GTEST_MOCKS_MOCK_BSP_WATCHDOG_H_
#define GTEST_MOCKS_MOCK_BSP_WATCHDOG_H_

#include <gmock/gmock.h>
#include "bsp_watchdog.h"


//create the MOCKS
class bsp_watchdog_Functions
{
    public:
        virtual ~bsp_watchdog_Functions(){}
        virtual void BSP_watchdogFreezeInDebug(void) = 0;
        virtual void BSP_watchdogRefresh(void) = 0;
};

class bsp_watchdog_Mock : public bsp_watchdog_Functions
{
    public:
        MOCK_METHOD(void, BSP_watchdogFreezeInDebug, (), (override));
        MOCK_METHOD(void, BSP_watchdogRefresh, (), (override));
};

//create global mock object
extern bsp_watchdog_Mock *bsp_watchdog_MockObj;



#endif /* GTEST_MOCKS_MOCK_BSP_WATCHDOG_H_ */

/*
 * application_timers.h
 *
 *  Created on: Jul 27, 2022
 *      Author: wojciech
 */

#ifndef APPLICATION_TIMERS_APPLICATION_TIMERS_H_
#define APPLICATION_TIMERS_APPLICATION_TIMERS_H_

#include <stdint.h>
#include "timers_core/timer.h"
#include "hal_include.h"


#define UPGRADE_TIME	8000
#define I2C_HANG_TIME	2000
#define HEARTBEAT_TIME	125


typedef enum
{
	UPGRADE_TIMER,
	I2C_HANG_TIMER,
	TIMER_HEARTBEAT,
    USER_TIMERS_COUNT
}user_timers_t;


void startTimer(user_timers_t timer_nr, uint64_t interval);
int checkTimerTimeout(user_timers_t timer_nr);
void restartTimer(user_timers_t timer_nr);
void softRestartTimer(user_timers_t timer_nr);
void timeoutTimer(user_timers_t timer_nr);
void stopTimer(user_timers_t timer_nr);
uint64_t getTimerInterval(user_timers_t timer_nr);
void initTimers();


#endif /* APPLICATION_TIMERS_APPLICATION_TIMERS_H_ */

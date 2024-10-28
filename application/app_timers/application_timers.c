/*
 * application_timers.c
 *
 *  Created on: Jul 27, 2022
 *      Author: wojciech
 */

#include "application_timers.h"


struct timer user_timers_array[USER_TIMERS_COUNT];


void startTimer(user_timers_t timer_nr, uint64_t interval)
{
    timer_set(&user_timers_array[timer_nr], (clock_time_t)interval);
}

int checkTimerTimeout(user_timers_t timer_nr)
{
    return timer_expired(&user_timers_array[timer_nr]);
}

void restartTimer(user_timers_t timer_nr)
{
    timer_restart(&user_timers_array[timer_nr]);
}

void softRestartTimer(user_timers_t timer_nr)
{
    timer_soft_restart(&user_timers_array[timer_nr]);
}

void timeoutTimer(user_timers_t timer_nr)
{
    timer_timeout(&user_timers_array[timer_nr]);
}

void stopTimer(user_timers_t timer_nr)
{
    timer_stop(&user_timers_array[timer_nr]);
}

uint64_t getTimerInterval(user_timers_t timer_nr)
{
	return (uint64_t)timer_getInterval(&user_timers_array[timer_nr]);
}

void initTimers()
{
	startTimer(UPGRADE_TIMER, UPGRADE_TIME);
	startTimer(I2C_HANG_TIMER, I2C_HANG_TIME);
	startTimer(TIMER_HEARTBEAT, HEARTBEAT_TIME);
}


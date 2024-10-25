/*
 * utilities.h
 *
 *  Created on: Mar 27, 2024
 *      Author: w.klimiuk
 */

#ifndef UTILITIES_UTILITIES_H_
#define UTILITIES_UTILITIES_H_

#ifdef USE_HAL_DRIVER
#include "hal_include.h"
#endif
#include <stdint.h>
#include <stdbool.h>


void offsetInterruptVector();
void restartDevice();
void disableAppInterrupts();

void ASSERT(bool expr);
void DELAY_MS(uint32_t ms);

#endif /* UTILITIES_UTILITIES_H_ */

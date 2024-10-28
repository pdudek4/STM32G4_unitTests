/*
 * configuration.h
 *
 *  Created on: Mar 26, 2024
 *      Author: w.klimiuk
 */

#ifndef CONFIGURATION_CONFIGURATION_H_
#define CONFIGURATION_CONFIGURATION_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include "configuration_typedef.h"


void CONF_readMCUAddress(void);
uint8_t CONF_getMCUAddress(void);
bool CONF_isMCUAddressCorrect(void);

void CONF_initDeviceVersions(void);
deviceVersions_t * CONF_getDeviceVersionHandle(void);
uint8_t saveBootloaderSettings(void);
uint8_t loadBootloaderSettings(void);

uint8_t CONF_checkUpgrade(void);

uint8_t getBootByte(void);
void setFlashByte(void);
void clearFlashByte(void);
uint8_t getFlashByte(void);
bootloaderSettings_t* getBootloaderSettings(void);

#ifdef __cplusplus
}
#endif
#endif /* CONFIGURATION_CONFIGURATION_H_ */

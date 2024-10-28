/*
 * application.h
 *
 *  Created on: Sep 13, 2024
 *      Author: pdude
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


typedef struct
{
	uint32_t appCrcStartCalcAddr;
	uint32_t appCrcCalcSize;
	uint16_t crcApp;
} __attribute__ ((packed)) appCrcInfo_ts;


void APP_preinitialize(void);
void APP_initialize(void);
void APP_process(void);

uint8_t APP_checkAppCrc(void);
void APP_jumpToApplication(void);


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_H_ */

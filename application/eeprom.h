/*
 * eeprom.h
 *
 *  Created on: Sep 16, 2024
 *      Author: pdude
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


enum EEPROM_RETURN_VALUE
{
	EEPROM_OK = 0,
	EEPROM_ERROR = 1,
};

enum EEPROM_CRC
{
	EEPROM_NO_CRC  = 0,
	EEPROM_ADD_CRC = 1
};


uint8_t EEPROM_write(uint16_t address, uint8_t* data, uint16_t length);
uint8_t EEPROM_read(uint16_t address, uint8_t* data, uint16_t length);

uint8_t EEPROM_writeBlocks(uint16_t address, uint8_t* data, uint8_t* compare_array, uint16_t length, uint8_t includeCRC);
uint8_t EEPROM_readBlocks(uint16_t address, uint8_t* data, uint8_t* compare_array, uint16_t length, uint8_t includeCRC);

uint16_t EEPROM_calculateCRC(uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif
#endif /* EEPROM_H_ */

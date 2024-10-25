/*
 * bsp_crc.c
 *
 *  Created on: Mar 22, 2024
 *      Author: w.klimiuk
 */

#include "bsp_crc.h"


/**
 * @brief	Obliczanie sumy kontrolnej CRC-16/ARC.  Metoda software'owa.
 * @param	data - bufor z danymi
 * @param	length - długość danych
 * @return	Obliczona suma CRC
 */
uint16_t CRC_calculateArc16(uint8_t *data, uint16_t length)
{
	uint16_t crc_val = 0;

	for (uint16_t j = 0; j < length; j++)
	{
		crc_val ^= data[j];

		for (uint8_t i = 0; i < 8; i++)
		{
			if ((crc_val & 0x0001) > 0)
			{
				crc_val >>= 1;
				crc_val ^= 0xA001;
			}
			else
			{
				crc_val >>= 1;
			}
		}
	}
	return crc_val;
}


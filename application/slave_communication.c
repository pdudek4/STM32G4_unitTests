/*
 * slave_communication.c
 *
 *  Created on: Mar 25, 2024
 *      Author: w.klimiuk
 */

#include <string.h>
#include "slave_communication.h"
#include "bsp_crc.h"
#include "bsp_i2c.h"
#include "application_timers.h"
#include "configuration.h"
#include "configuration_typedef.h"
#include "state_machine.h"
#include "upgrade.h"
#include "utilities.h"


static I2C_transmission_buffer_s slave_comm_tx_buffer = {0};
static I2C_reception_buffer_s slave_comm_rx_buffer = {0};
static I2C_frames_round_buffer_s I2C_rx_round_buffer = {0};

volatile uint8_t firmwareDataFrameOk;
uint8_t debStartFr, debComplFr1, debComplFr2;


/**
 * @brief	Kopiuje ramkę z bufora odbiorczego do bufora kołowego I2C.
 * 			Funkcja w/w w przerwaniu.
 * @param	data - bufor odbiorczy
 * @param	length - długość ramki
 */
static void COMM_putToReceptionBuffer(const uint8_t* data, uint16_t length)
{
	memcpy(I2C_rx_round_buffer.frame[I2C_rx_round_buffer.last], data, length);
	I2C_rx_round_buffer.frame_length[I2C_rx_round_buffer.last] = length;
	I2C_rx_round_buffer.last = (I2C_rx_round_buffer.last + 1) % I2C_RX_FRAME_BUFFER_SIZE;

	if (I2C_rx_round_buffer.last == I2C_rx_round_buffer.first)
	{
		I2C_rx_round_buffer.first = I2C_rx_round_buffer.first + 1 % I2C_RX_FRAME_BUFFER_SIZE;
	}
}
/**
 * @brief	Odczytanie ramki z bufora kołowego do @data.
 * @param[out] data - bufor do którego trafi ramka z bufora I2C
 * @param[out] length - długość ramki
 */
static bool COMM_getFromReceptionBuffer(uint8_t* data, uint16_t* length)
{
	bool got_data = false;

	if (I2C_rx_round_buffer.last != I2C_rx_round_buffer.first)
	{
		memcpy(data, I2C_rx_round_buffer.frame[I2C_rx_round_buffer.first], I2C_RX_BUFFER_LENGTH);
		*length = I2C_rx_round_buffer.frame_length[I2C_rx_round_buffer.first];
		I2C_rx_round_buffer.first = (I2C_rx_round_buffer.first + 1) % I2C_RX_FRAME_BUFFER_SIZE;
		got_data = true;
	}

	return got_data;
}

/**
 * @brief	Ustawia strukturę odbiorczą. W/w po odebraniu bajtu adresu.
 * 			Funkcja w/w w przerwaniu.
 */
void COMM_receptionRequest(void)
{
	slave_comm_rx_buffer.expected_length = 0;
	slave_comm_rx_buffer.position = 0;
	BSP_I2CReceiveFirstData(&slave_comm_rx_buffer.buffer[0]);
	debStartFr++;
}

/**
 * @brief	Odbiór ramki od Mastera i natychmiastowa odpowiedź.
 * 			Funkcja w/w w przerwaniu.
 */
void COMM_transmissionRequest(void)
{
	uint8_t slave_comm_command = slave_comm_rx_buffer.buffer[0];

	switch (slave_comm_command)
	{
	case I2C_TX_COMMAND_GET_SW_VERSION:
		memcpy(slave_comm_tx_buffer.buffer, (uint8_t*)CONF_getDeviceVersionHandle(), I2C_TX_COMMAND_GET_SW_VERSION_LENGTH);
		BSP_I2CTransmitData(slave_comm_tx_buffer.buffer, I2C_TX_COMMAND_GET_SW_VERSION_LENGTH);
		break;
	case I2C_TX_COMMAND_SLAVE_STATE:
		slave_comm_tx_buffer.buffer[0] = SM_getState();
		BSP_I2CTransmitData(slave_comm_tx_buffer.buffer, I2C_TX_COMMAND_SLAVE_STATE_LENGTH);
		break;
	case I2C_TX_COMMAND_FIRMWARE_DATA_VALID:
	{
		uint16_t segment;
		segment = UPGRADE_getFirmwareSegment();
		slave_comm_tx_buffer.buffer[0] = firmwareDataFrameOk;
		slave_comm_tx_buffer.buffer[1] = segment & 0xFF;
		slave_comm_tx_buffer.buffer[2] = (segment >> 8) & 0xFF;
		BSP_I2CTransmitData(slave_comm_tx_buffer.buffer, I2C_TX_COMMAND_FIRMWARE_DATA_VALID_LENGTH);
		firmwareDataFrameOk = 0;
		break;
	}
	default:
		break;
	}
}

/**
 * @brief	Obsługa całej odebranej ramki, czyli po każdym bajcie w naszym przypadku.
 * 			Funkcja w/w w przerwaniu.
 */
void COMM_receptionCompleted(void)
{
	//pierwszy bajt ramki
	if (slave_comm_rx_buffer.position == 0)
	{
		switch (slave_comm_rx_buffer.buffer[0])
		{
		case I2C_RX_COMMAND_RESTART:
			slave_comm_rx_buffer.expected_length = I2C_RX_COMMAND_RESTART_LENGTH;
			break;
		case I2C_RX_COMMAND_IMAGE_INFO:
			slave_comm_rx_buffer.expected_length = I2C_RX_COMMAND_IMAGE_INFO_LENGTH;
			break;
		case I2C_RX_COMMAND_IMAGE_DATA:
			slave_comm_rx_buffer.expected_length = I2C_RX_COMMAND_IMAGE_DATA_LENGTH;
			break;
		case I2C_COMMAND_TEST:
			slave_comm_rx_buffer.expected_length = I2C_TEST_LENGTH;
			break;
		default:
			break;
		}
	}

	slave_comm_rx_buffer.position++;

	//każdy kolejny bajt ramki
	if (slave_comm_rx_buffer.position < slave_comm_rx_buffer.expected_length
		&& slave_comm_rx_buffer.position < I2C_RX_BUFFER_LENGTH)
	{
		if (slave_comm_rx_buffer.position == slave_comm_rx_buffer.expected_length - 1)
		{
			BSP_I2CReceiveLastData(&slave_comm_rx_buffer.buffer[slave_comm_rx_buffer.position]);
		}
		else
		{
			BSP_I2CReceiveNextData(&slave_comm_rx_buffer.buffer[slave_comm_rx_buffer.position]);
		}
	}

	//ostatni bajt ramki
	if (slave_comm_rx_buffer.position == slave_comm_rx_buffer.expected_length)
	{
		debComplFr1++;
		COMM_putToReceptionBuffer(slave_comm_rx_buffer.buffer, slave_comm_rx_buffer.expected_length);
		memset(slave_comm_rx_buffer.buffer, 0x00, I2C_RX_BUFFER_LENGTH);
	}
}

/**
 * @brief	Callback końca transmisji.
 */
void COMM_transmissionCompleted(void)
{
	memset(slave_comm_tx_buffer.buffer, 0x00, I2C_TX_BUFFER_LENGTH);
}

/**
 * @brief	Przetwarzanie odebranych komend. W/w z maina.
 */
void COMM_processReceivedCommands(void)
{
	uint16_t dataLen;
	uint8_t frame[I2C_RX_BUFFER_LENGTH];
	uint16_t frame_length = 0;
	uint8_t retVal;

	if (COMM_getFromReceptionBuffer(frame, &frame_length) == true)
	{
		debComplFr2++;

		switch (frame[0])
		{
		case I2C_RX_COMMAND_RESTART:
			restartDevice();
			break;
		case I2C_RX_COMMAND_IMAGE_INFO:
		{
			dataLen = (frame[I2C_FR_LENGTH_OFFSET+1] << 8) | frame[I2C_FR_LENGTH_OFFSET];
			retVal = UPGRADE_validateFrame(frame, dataLen, I2C_RX_IMAGE_INFO_PAYLOAD_LENGTH);
			if (retVal != SUCCESS)
			{
				ASSERT(false);
				break;
			}

			UPGRADE_framePasswordKey(&frame[I2C_FR_PAYLOAD_OFFSET]);
			restartTimer(UPGRADE_TIMER);
			break;
		}
		case I2C_RX_COMMAND_IMAGE_DATA:
		{
			dataLen = (frame[I2C_FR_LENGTH_OFFSET+1] << 8) | frame[I2C_FR_LENGTH_OFFSET];
			retVal = UPGRADE_validateFrame(frame, dataLen, I2C_RX_IMAGE_DATA_PAYLOAD_LENGTH);
			if (retVal != SUCCESS)
			{
				firmwareDataFrameOk = 0;
				ASSERT(false);
				break;
			}

			retVal = UPGRADE_processBinaryFrame(frame, dataLen);
			if (retVal != SUCCESS)
			{
				firmwareDataFrameOk = 0;
				ASSERT(false);
				break;
			}

			firmwareDataFrameOk = 1;
			restartTimer(UPGRADE_TIMER);
			break;
		}
		default:
			break;
		}
	}
}

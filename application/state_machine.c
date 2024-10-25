/*
 * state_machine.c
 *
 *  Created on: Sep 17, 2024
 *      Author: pdude
 */

#include "state_machine.h"

#include "application.h"
//#include "configuration.h"
//#include "crypto.h"
#include "utilities.h"


static state_machine_states_te current_state = SM_WAITING_FOR_DATA;

extern uint32_t binary_length;
extern uint32_t received_data_bytes;


/**
 * @brief	Inicjalizacja maszyny stanów
 */
void SM_init(void)
{
//	CRYPTO_initSecretKey();
	current_state = SM_RESET_PROCEDURE;
}

/**
 * @brief	Ustawianie stanu
 */
void SM_setState(state_machine_states_te newState)
{
	if (newState >= SM_STATE_TOTAL)
		return;

	current_state = newState;
}

/**
 * @brief	Zwrócenie stanu
 */
state_machine_states_te SM_getState(void)
{
	return current_state;
}

/**
 * @brief	Przetwarzanie - maszyna stanów
 */
void SM_process(void)
{
	switch (current_state)
	{
	case SM_RESET_PROCEDURE:
	{
//		received_data_bytes = 0;
//		binary_length = 0;
//		CRYPTO_deInitPrivateKey();
//		current_state = SM_WAITING_FOR_DATA;
		break;
	}
	case SM_WAITING_FOR_DATA:

		break;

	case SM_RECEIVING_PASSWORD_AND_KEY:

		break;

	case SM_CHECKING_PASSWORD_AND_LENGTH:

		break;

	case SM_RECEIVING_BINARY:

		break;

	case SM_CHECKING_DATA_INTEGRITY:
		//todo sprawdzenie CRC firmware we flashu
//		setFlashByte();
//		current_state = SM_RUNNING_MAIN_APP;
		break;

	case SM_RUNNING_MAIN_APP:
		DELAY_MS(100);
//		APP_jumpToApplication();
		break;

	default:
		break;
	}
}


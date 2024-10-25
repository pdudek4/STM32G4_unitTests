/*
 * state_machine.h
 *
 *  Created on: Sep 17, 2024
 *      Author: pdude
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SM_RESET_PROCEDURE,
	SM_WAITING_FOR_DATA,
	SM_RECEIVING_PASSWORD_AND_KEY,
	SM_CHECKING_PASSWORD_AND_LENGTH,
	SM_RECEIVING_BINARY,
	SM_CHECKING_DATA_INTEGRITY,
	SM_RUNNING_MAIN_APP,
	SM_STATE_TOTAL,
} state_machine_states_te;


void SM_init(void);
void SM_setState(state_machine_states_te state);
state_machine_states_te SM_getState(void);
void SM_process(void);

#ifdef __cplusplus
}
#endif
#endif /* STATE_MACHINE_H_ */

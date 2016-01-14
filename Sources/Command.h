/*
 * Command.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include <PE_Types.h>

typedef enum {
	CommandReserved,
	CommandPing,
	CommandHomeUp,
	CommandHomeDown,
	CommandMoveTo,
	CommandGetStatus,
	CommandConfigure,
	CommandAssociate,
	CommandIdentifyLed,
	CommandJog,
	CommandResetCounters,
	CommandDoubleMoveTo,
	CommandGetHallPos,
	CommandSetPosGetHalls,
	CommandGetPots
} command_t;

void Command_Init();
void Command_CharacterReceived(char c);
void Command_Task();
void Command_ResponseTask();

bool IdentificationLed;
#endif /* COMMAND_H_ */

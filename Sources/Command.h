/*
 * Command.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include <PE_Types.h>

#define VERSION "V1.1"

#define NACK_MASK  0x80

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
	CommandSetPosGetData,
	CommandGetHallPos,
	CommandGetPots,
	CommandGetCurrent,
	CommandGetConfiguration,    //16 x10
	CommandSetBaudRate,
	CommandGetVersion,          //   x11
	CommandInvalidCommand       //   x12
} command_t;

void Command_Init();
void Command_CharacterReceived(char c);
void Command_Task();
void Command_ResponseTask();

bool IdentificationLed;
bool GetHalls;
bool GetPots;
bool GetCurrent;

#endif /* COMMAND_H_ */

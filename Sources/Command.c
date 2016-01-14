/*
 * Command.c
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */
#include "Command.h"
#include "UART.h"
#include "TXEN.h"
#include "Events.h"
#include "motor.h"
#include <string.h>
#include "Cpu.h"
#include "UART_PDD.h"
/// Internal constants
#define COMMAND_SIZE					(16)
#define COMMAND_BUFFER_SIZE				(32)
#define COMMAND_RESPONSE_BUFFER_SIZE	(32)
#define SYNC_WORD_LENGTH				(4)
#define SHIFT_SIZE                      (10000)

/// Internal typedefs
/**
 * This is the command op-codes.
 * These names must be kept in-sync with the C# project or things will get screwy
 */

typedef enum {
	SyncWaiting,
	SyncStarted,
	SyncFinished,
	SyncRestarted,
	SyncRestartAborted,
	SyncComplete
} syncState_t;

typedef enum {
	CommandStateWaiting,
	CommandStateStartedReceiving,
	CommandStateReceiving,
	CommandStateFinishedReceiving,
	CommandStateExecuting
} commandState_t;

/// Internal members
char ReceiveBuffer[COMMAND_BUFFER_SIZE];
int ReceiveIndex = 0;
UART_TComData ResponseBuffer[COMMAND_RESPONSE_BUFFER_SIZE];
int ResponseIndex = 0;
int SyncWordIndex = 0;
const char SyncWord[] = {'\xDA', '\xBA', '\xD0', '\x00' };
syncState_t SyncStatus = SyncWaiting;
commandState_t CommandStatus = CommandStateWaiting;
bool ResponseReadyToSend = FALSE;
uint32 MyAddress;
bool IsAssociated = FALSE;


/// Internal methods
void _SendResponse();
void _Ping();
void _Associate();
void _HomeUp();
void _MoveTo(char* buffer);
void _GetStatus();
void _Discover();
void _ResetCounter(byte motor, bool resetSetpoint);
void _IdentifyLed(uint8 val);
void _Configure(uint8* buffer);
void _Jog(uint8* buffer);
void _DoubleMoveTo(char* buffer);
void _GetHallPos();
void _SetPosGetHalls(char* buffer);
void _GetPots();
bool _CheckCanSend();

/**
 * Initialize the Command processor.
 */
void Command_Init()
{
	MyAddress = SIM_UIDL ^ SIM_UIDMH ^ SIM_UIDML; // hash the Unique Identification Register values into a 32-bit address.
}

void _IdentifyLed(uint8 val)
{
	IdentificationLed = val;
}

void _Discover()
{

}

/**
 * Send a ping message back
 */
void _Ping()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
//	*((uint32*)&(ResponseBuffer[1])) = MyAddress;
	ResponseBuffer[5] = CommandPing;
	_SendResponse();
}

void _Associate()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	uint32 seed = MyAddress + Motor1_PotVal + Motor2_PotVal;
	uint32 delay = seed % 128;
	for(uint32 i = 0; i< 100000U; i++)
	{
		for(uint32 j = 0; j<delay; j++);
	}
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
//	*((uint32*)&(ResponseBuffer[1])) = (uint32)MyAddress;
	ResponseBuffer[5] = CommandAssociate;
	_SendResponse();
}
void _Configure(uint8* buffer)
{
	int32 kp = 0;
	switch(buffer[0])
	{
	case 0:
		Motor1_ControlMode = buffer[1];
//		Motor1_KP = buffer[2];
		kp |= buffer[5] & 0xFF;
		kp <<= 8;
		kp |= buffer[4] & 0xFF;
		kp <<= 8;
		kp |= buffer[3] & 0xFF;
		kp <<= 8;
		kp |= buffer[2] & 0xFF;
		Motor1_KP = kp;
		Motor1_SpeedMax = buffer[6];
		Motor1_CurrentMax = buffer[7] | (buffer[8] << 8);
		Motor1_PotZero = buffer[9] | (buffer[10] << 8);
		Motor1_ClicksPerRev = buffer[11] | (buffer[12] << 8);
		break;
	case 1:
		Motor2_ControlMode = buffer[1];
//		Motor2_KP = buffer[2];
		kp |= buffer[5] & 0xFF;
		kp <<= 8;
		kp |= buffer[4] & 0xFF;
		kp <<= 8;
		kp |= buffer[3] & 0xFF;
		kp <<= 8;
		kp |= buffer[2] & 0xFF;
		Motor2_KP = kp;
		Motor2_SpeedMax = buffer[6];
		Motor2_CurrentMax = buffer[7] | (buffer[8] << 8);
		Motor2_PotZero = buffer[9] | (buffer[10] << 8);
		Motor2_ClicksPerRev = buffer[11] | (buffer[12] << 8);
		break;
	}
}

void _ResetCounter(byte motor, bool resetSetpoint)
{
	if(motor == 0)
	{
		Motor1_ShaftCounter = ((Motor1_ClicksPerRev * SHIFT_SIZE / 65536) * (Motor1_PotVal - Motor1_PotZero)) / SHIFT_SIZE;
		if(resetSetpoint)
			Motor1_Setpoint = Motor1_ShaftCounter;
	}

	if(motor == 1)
	{
		Motor2_ShaftCounter = ((Motor1_ClicksPerRev * SHIFT_SIZE / 65536) * (Motor2_PotVal - Motor2_PotZero)) / SHIFT_SIZE;
		if(resetSetpoint)
			Motor2_Setpoint = Motor2_ShaftCounter;
	}
}

void _Jog(uint8* buffer)
{
switch(buffer[0])
{
case 0:
	if(buffer[1] > 0)
	{
		M1_EN_SetRatio8(buffer[1]);
		M1_DIR_PutVal(buffer[2]);
		Motor1_IsJogging = TRUE;
	}
	else
	{
		M1_EN_SetRatio8(0);
		Motor1_IsJogging = FALSE;
	}
	break;
case 1:
	if(buffer[1] > 0)
	{
		M2_EN_SetRatio8(buffer[1]);
		M2_DIR_PutVal(buffer[2]);
		Motor2_IsJogging = TRUE;
	}
	else
	{
		M2_EN_SetRatio8(0);
		Motor2_IsJogging = FALSE;
	}
}
}

void _MoveTo(char* buffer)
{
//	int32 setpoint = *((int32*)&(buffer[3]));
	int32 setpoint = 0;
	setpoint |= ReceiveBuffer[12] & 0xFF;
	setpoint <<= 8;
	setpoint |= ReceiveBuffer[11] & 0xFF;
	setpoint <<= 8;
	setpoint |= ReceiveBuffer[10] & 0xFF;
	setpoint <<= 8;
	setpoint |= ReceiveBuffer[9] & 0xFF;

	switch(buffer[0])
	{
	case 0:
		Motor1_Setpoint = setpoint;
		break;
	case 1:
		Motor2_Setpoint = setpoint;
		break;
	}
}


void _GetStatus()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
//	*((uint32*)&(ResponseBuffer[1])) = MyAddress;
	ResponseBuffer[5] = CommandGetStatus;
	ResponseBuffer[6] = Motor1_ControlMode;
	ResponseBuffer[7] = Motor2_ControlMode;
	*((uint32*)&(ResponseBuffer[8])) = Motor1_ShaftCounter;
	*((uint32*)&(ResponseBuffer[12])) = Motor2_ShaftCounter;
	*((uint32*)&(ResponseBuffer[16])) = (uint32)Motor1_Current << 16 | (uint32)Motor1_PotVal;
	*((uint32*)&(ResponseBuffer[20])) = (uint32)Motor2_Current << 16 | (uint32)Motor2_PotVal;
	_SendResponse();
}

void _DoubleMoveTo(char* buffer)
{
	int32 setpointOne = 0;
	int32 setpointTwo = 0;

	setpointOne |= ReceiveBuffer[9] & 0xFF;
	setpointOne <<= 8;
	setpointOne |= ReceiveBuffer[8] & 0xFF;
	setpointOne <<= 8;
	setpointOne |= ReceiveBuffer[7] & 0xFF;
	setpointOne <<= 8;
	setpointOne |= ReceiveBuffer[6] & 0xFF;

	setpointTwo |= ReceiveBuffer[13] & 0xFF;
	setpointTwo <<= 8;
	setpointTwo |= ReceiveBuffer[12] & 0xFF;
	setpointTwo <<= 8;
	setpointTwo |= ReceiveBuffer[11] & 0xFF;
	setpointTwo <<= 8;
	setpointTwo |= ReceiveBuffer[10] & 0xFF;

	Motor1_Setpoint = setpointOne;
	Motor2_Setpoint = setpointTwo;
}

void _GetHallPos()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
	ResponseBuffer[5] = CommandGetHallPos;
//	*((uint32*)&(ResponseBuffer[6])) = Motor1_ShaftCounter;
//	*((uint32*)&(ResponseBuffer[10])) = Motor2_ShaftCounter;
	ResponseBuffer[6] = (Motor1_ShaftCounter & 0x000000ff);
	ResponseBuffer[7] = (Motor1_ShaftCounter & 0x0000ff00) >> 8;
	ResponseBuffer[8] = (Motor1_ShaftCounter & 0x00ff0000) >> 16;
	ResponseBuffer[9] = (Motor1_ShaftCounter & 0xff000000) >> 24;
	ResponseBuffer[10] = (Motor2_ShaftCounter & 0x000000ff);
	ResponseBuffer[11] = (Motor2_ShaftCounter & 0x0000ff00) >> 8;
	ResponseBuffer[12] = (Motor2_ShaftCounter & 0x00ff0000) >> 16;
	ResponseBuffer[13] = (Motor2_ShaftCounter & 0xff000000) >> 24;
	_SendResponse();
}

void _SetPosGetHalls(char* buffer)
{
	_DoubleMoveTo(buffer);
	_GetHallPos();
}

void _GetPots()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
	ResponseBuffer[5] = CommandGetPots;
	*((uint16*)&(ResponseBuffer[6])) = Motor1_PotVal;
	*((uint16*)&(ResponseBuffer[8])) = Motor2_PotVal;
	_SendResponse();
}

/**
 * Call this function periodically outside the interrupt context to process commands
 */
void Command_Task()
{
	if(CommandStatus != CommandStateFinishedReceiving)
		return;

	LED_GREEN_ClrVal();

	CommandStatus = CommandStateExecuting;

	uint32 intendedAddress = 0; // this is the address the packet is intended for
//	uint32 intendedAddress = ( (uint32)ReceiveBuffer[1] << 24 ) | ( (uint32)ReceiveBuffer[2] << 16 ) | ( (uint32)ReceiveBuffer[3] << 8 ) | (uint32)ReceiveBuffer[4];
//	intendedAddress = (intendedAddress << 8) + ReceiveBuffer[4];
//	intendedAddress = (intendedAddress << 8) + ReceiveBuffer[3];
//	intendedAddress = (intendedAddress << 8) + ReceiveBuffer[2];
//	intendedAddress = (intendedAddress << 8) + ReceiveBuffer[1];
	intendedAddress |= ReceiveBuffer[4] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[3] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[2] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[1] & 0xFF;

	if(intendedAddress == MyAddress || intendedAddress == 0) // 0 = broadcast
	{
		switch(ReceiveBuffer[5])
		{
		case CommandPing:
			_Ping();
			break;
		case CommandConfigure:
			_Configure(&ReceiveBuffer[6]);
			break;
		case CommandHomeUp:
			//_HomeUp();
			break;
		case CommandHomeDown:
			//_HomeDown();
			break;
		case CommandMoveTo:
			_MoveTo(&ReceiveBuffer[6]);
			break;
		case CommandGetStatus:
			_GetStatus();
			break;
		case CommandAssociate:
			_Associate();
			break;
		case CommandIdentifyLed:
			_IdentifyLed(ReceiveBuffer[6]);
			break;
		case CommandJog:
			_Jog(&ReceiveBuffer[6]);
			break;
		case CommandResetCounters:
			_ResetCounter(ReceiveBuffer[6], ReceiveBuffer[7]);
			break;
		case CommandDoubleMoveTo:
			_DoubleMoveTo(&ReceiveBuffer[6]);
			break;
		case CommandGetHallPos:
			_GetHallPos();
			break;
		case CommandSetPosGetHalls:
			_SetPosGetHalls(&ReceiveBuffer[6]);
			break;
		case CommandGetPots:
			_GetPots();
			break;
		}
	}
	CommandStatus = CommandStateWaiting;
	LED_GREEN_SetVal();
}

/**
 * Call this function whenever a character is received by the UART
 * @param c the character that was received
 */
void Command_CharacterReceived(char c)
{
	// check for a sync word (even if we already found one)
	if(c == SyncWord[0] || c == SyncWord[SyncWordIndex])
	{
		// Weird special case where the first sync character is repeated
		if(c != SyncWord[SyncWordIndex] && SyncWordIndex == 1)
		{
			SyncWordIndex = 0;
		}
		if(SyncWordIndex == 0) // this is the start of a sync word
		{
			switch(SyncStatus)
			{
			case SyncWaiting:				// We were waiting for a sync word
				SyncStatus = SyncStarted;	// ...and now we've started to find one
				break;
			case SyncFinished:					// We already found a sync word, but haven't processed our command yet.
				SyncStatus = SyncRestarted;		// ...but here's a new one
				break;
			}
		}

		SyncWordIndex++;

		if(SyncWordIndex == SYNC_WORD_LENGTH) // We found the whole sync word. Reset the buffer
		{
			SyncWordIndex = 0;
			SyncStatus = SyncFinished;
			CommandStatus = CommandStateStartedReceiving;
			return;
		}
	} else {
		// The character is not a sync word. Reset the index and update the state machine
		SyncWordIndex = 0;

		switch(SyncStatus)
		{
		case SyncStarted:
			SyncStatus = SyncWaiting;
			break;
		case SyncRestarted:						// We *were* finished, but we thought we had a sync word, but we didn't.
			SyncStatus = SyncFinished;			// Abort the sync restart
			break;
		}
	}

	switch(CommandStatus)
	{
	case CommandStateWaiting:
		return;
	case CommandStateStartedReceiving:
		ReceiveIndex = 0;
		CommandStatus = CommandStateReceiving;
	case CommandStateReceiving:
		ReceiveBuffer[ReceiveIndex] = c;
		ReceiveIndex++;
		if(ReceiveIndex == ReceiveBuffer[0])
		{
			CommandStatus = CommandStateFinishedReceiving;
		}
		if(ReceiveIndex == COMMAND_BUFFER_SIZE) // we've filled our command buffer.
		{
			ReceiveIndex = 0;
		}
		break;
	}
}

void _SendResponse()
{
	ResponseReadyToSend = TRUE;
	Command_ResponseTask();
}

/**
 * This task will send any response data out of the UART.
 *
 * This should be called in the UART_OnTxChar event. Internally, the method checks to make sure the buffer
 * is empty and that the buffer is ready to send, so this method can also be called in the main application
 * loop.
 *
 * This method will automagically wiggle the TXEN line when necessary.
 */
void Command_ResponseTask()
{
	if(ResponseReadyToSend == FALSE)
		return;

//	if(UART_PDD_GetTxCompleteStatus(UART1_BASE_PTR) != 0x40u) // we're waiting for a transmit to complete already
//		return;
	if(UART_GetCharsInTxBuf() > 0)
		return; // we're waiting for a transmit to complete already

	if(ResponseIndex == 0)
	{
		TXEN_SetVal();
		for(int i=0;i<10000;i++);
	}

	// We have to wait a bit before we send our first byte.

	UART_SendChar(ResponseBuffer[ResponseIndex++]);
	if(ResponseIndex == COMMAND_RESPONSE_BUFFER_SIZE) // we've sent everything
	{
//		while(!(UART_PDD_ReadStatus1Flags(UART1_BASE_PTR) && UART_S1_TC_MASK) )
		while(UART_PDD_GetTxCompleteStatus(UART1_BASE_PTR) == 0U)
//		while(UART_GetCharsInTxBuf() > 0 );
//		for(int i=0;i<10000;i++);
		ResponseIndex = 0;
		ResponseReadyToSend = FALSE;
		TXEN_ClrVal();
		LED_GREEN_SetVal();
	}
}

bool _CheckCanSend()
{
	return !ResponseReadyToSend;
}



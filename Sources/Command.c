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
#include "VREF.h"

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
int ResponseFillIndex = 0;
int SyncWordIndex = 0;
const char SyncWord[] = {'\xDA', '\xBA', '\xD0', '\x00' };
syncState_t SyncStatus = SyncWaiting;
commandState_t CommandStatus = CommandStateWaiting;
bool ResponseReadyToSend = FALSE;
uint32 MyAddress;
bool IsAssociated = FALSE;
uint32_t badCommandsRecieved = 0;
extern LDD_TDeviceData* crc;

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
void _SetPosGetData(char* buffer);
void _GetHallPos();
void _GetPots();
void _GetCurrent();
bool _CheckCanSend();
void _AddResponseChecksum();
void _GetVersionString();

void _SendResponseMessage(byte commandType, bool bGoodMessage);
void _FillResponseHeader(byte command);

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
}
void _Configure(uint8* buffer)
{
	int32 kp = 0;
	switch(buffer[0])
	{
	case 0:
		Motor1.ControlMode = buffer[1];
//		Motor1_KP = buffer[2];
		kp |= buffer[5] & 0xFF;
		kp <<= 8;
		kp |= buffer[4] & 0xFF;
		kp <<= 8;
		kp |= buffer[3] & 0xFF;
		kp <<= 8;
		kp |= buffer[2] & 0xFF;
		Motor1.KP = kp;
		Motor1.SpeedMin = buffer[6];
		Motor1.CurrentMax = buffer[7] | (buffer[8] << 8);
		Motor1.PotZero = buffer[9] | (buffer[10] << 8);
		Motor1.ClicksPerRev = buffer[11] | (buffer[12] << 8);
		GetHalls = buffer[13];
		GetPots = buffer [14];
		GetCurrent = buffer[15];
		break;
	case 1:
		Motor2.ControlMode = buffer[1];
//		Motor2_KP = buffer[2];
		kp |= buffer[5] & 0xFF;
		kp <<= 8;
		kp |= buffer[4] & 0xFF;
		kp <<= 8;
		kp |= buffer[3] & 0xFF;
		kp <<= 8;
		kp |= buffer[2] & 0xFF;
		Motor2.KP = kp;
		Motor2.SpeedMin = buffer[6];
		Motor2.CurrentMax = buffer[7] | (buffer[8] << 8);
		Motor2.PotZero = buffer[9] | (buffer[10] << 8);
		Motor2.ClicksPerRev = buffer[11] | (buffer[12] << 8);
		GetHalls = buffer[13];
		GetPots = buffer [14];
		GetCurrent = buffer[15];
		break;
	}
	if(Motor1.CurrentMax > Motor2.CurrentMax)
	{
		uint32_t dummyVal = Motor2.CurrentMax << 4;
		VREF_SetValue(&dummyVal);
	}
	else
	{
		uint32_t dummyVal = Motor1.CurrentMax << 4;
		VREF_SetValue(&dummyVal);
	}

}

void _ResetCounter(byte motor, bool resetSetpoint)
{
	if(motor == 0)
	{
		Motor1.ShaftCounter = 0;
		if(resetSetpoint)
			Motor1.Setpoint = 0;
	}

	if(motor == 1)
	{
		Motor2.ShaftCounter = 0;
		if(resetSetpoint)
			Motor2.Setpoint = 0;
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
	int32 setpoint = 0;
	setpoint |= buffer[4] & 0xFF;
	setpoint <<= 8;
	setpoint |= buffer[3] & 0xFF;
	setpoint <<= 8;
	setpoint |= buffer[2] & 0xFF;
	setpoint <<= 8;
	setpoint |= buffer[1] & 0xFF;

	switch(buffer[0])
	{
	case 0:
		Motor1.Setpoint = setpoint;
		break;
	case 1:
		Motor2.Setpoint = setpoint;
		break;
	}
}

void _GetVersionString()
{
	//response buffer size - header size;
	int sz = snprintf(&ResponseBuffer[ResponseFillIndex],
			COMMAND_RESPONSE_BUFFER_SIZE-ResponseFillIndex-2,
			"%s %s %s",
			VERSION, __DATE__,__TIME__);
	ResponseFillIndex+=(sz+1); //size plus null
}

void _FillResponseHeader(byte command)
{
	memset(ResponseBuffer,0,COMMAND_RESPONSE_BUFFER_SIZE);
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
	ResponseBuffer[5] = command;
	ResponseFillIndex = 6;
}

void _AddResponseChecksum()
{
	//add a crc
	uint32_t result;
	CRC1_ResetCRC(crc);
	CRC1_GetBlockCRC(crc, ResponseBuffer, ResponseFillIndex, &result );
	memcpy(&ResponseBuffer[ResponseFillIndex],&result,sizeof(result));
	ResponseFillIndex+=2;
}

void _SendResponseMessage(byte commandType, bool bGoodMessage)
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
	{
		return;
	}


	if(bGoodMessage == TRUE)
	{
		_FillResponseHeader(commandType);
	}
	else
	{
		//let the sender know the sent message was bad
		_FillResponseHeader(commandType | NACK_MASK);
	}

	switch(commandType)
	{
	case CommandAssociate:
		_Associate();
	case CommandSetPosGetData:
		_GetStatus();
		break;
	case CommandGetStatus:
		_GetStatus();
		break;
	case CommandGetVersion:
		_GetVersionString();
		break;
	case CommandInvalidCommand: //invalid commands should send a response back to the sender
		break;
	default:
		return;
	}

	_AddResponseChecksum();
	_SendResponse();
}

void _GetStatus()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;

	if(GetHalls)
	{
		ResponseBuffer[6] = (Motor1.ShaftCounter & 0x000000ff);
		ResponseBuffer[7] = (Motor1.ShaftCounter & 0x0000ff00) >> 8;
		ResponseBuffer[8] = (Motor1.ShaftCounter & 0x00ff0000) >> 16;
		ResponseBuffer[9] = (Motor1.ShaftCounter & 0xff000000) >> 24;
		ResponseBuffer[10] = (Motor2.ShaftCounter & 0x000000ff);
		ResponseBuffer[11] = (Motor2.ShaftCounter & 0x0000ff00) >> 8;
		ResponseBuffer[12] = (Motor2.ShaftCounter & 0x00ff0000) >> 16;
		ResponseBuffer[13] = (Motor2.ShaftCounter & 0xff000000) >> 24;
		ResponseFillIndex += 8;
	}
	if(GetPots)
	{
		*((uint16*)&(ResponseBuffer[ResponseFillIndex])) = (uint16_t)badCommandsRecieved;
		*((uint16*)&(ResponseBuffer[ResponseFillIndex + 2])) = Motor2_PotVal;
		ResponseFillIndex += 4;
	}
	if(GetCurrent)
	{
		*((uint16*)&(ResponseBuffer[ResponseFillIndex])) = Motor1_AvgCurrent;
		*((uint16*)&(ResponseBuffer[ResponseFillIndex + 2])) = Motor2_AvgCurrent;
		ResponseFillIndex += 4;
	}
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

	Motor1.Setpoint = setpointOne;
	Motor2.Setpoint = setpointTwo;
}

void _SetPosGetData(char* buffer)
{
	_DoubleMoveTo(buffer);
	_GetStatus();
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

	ResponseBuffer[6] = (Motor1.ShaftCounter & 0x000000ff);
	ResponseBuffer[7] = (Motor1.ShaftCounter & 0x0000ff00) >> 8;
	ResponseBuffer[8] = (Motor1.ShaftCounter & 0x00ff0000) >> 16;
	ResponseBuffer[9] = (Motor1.ShaftCounter & 0xff000000) >> 24;

	ResponseBuffer[10] = (Motor2.ShaftCounter & 0x000000ff);
	ResponseBuffer[11] = (Motor2.ShaftCounter & 0x0000ff00) >> 8;
	ResponseBuffer[12] = (Motor2.ShaftCounter & 0x00ff0000) >> 16;
	ResponseBuffer[13] = (Motor2.ShaftCounter & 0xff000000) >> 24;
	_SendResponse();
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

void _GetCurrent()
{
	if(ResponseReadyToSend == TRUE) // We're still sending our last response, so exit immediately
		return;
	*((uint32*)ResponseBuffer) = 200;
	ResponseBuffer[1] = (MyAddress & 0x000000ff);
	ResponseBuffer[2] = (MyAddress & 0x0000ff00) >> 8;
	ResponseBuffer[3] = (MyAddress & 0x00ff0000) >> 16;
	ResponseBuffer[4] = (MyAddress & 0xff000000) >> 24;
	ResponseBuffer[5] = CommandGetCurrent;
	*((uint16*)&(ResponseBuffer[6])) = (uint16)Motor1_AvgCurrent;
	*((uint16*)&(ResponseBuffer[8])) = (uint16)Motor2_AvgCurrent;
	_SendResponse();
}

/**
 * Validate the CRC-16 of an incoming packet, stored in ReceiveBuffer
 */
bool _CheckIncomingPacketCRC()
{
	//get packet length
	uint8_t length = ReceiveBuffer[0];

	//start CRC on header DABAD000
	uint32_t header = 0x00D0BADA;

	//add a crc
	uint32_t result=0;
	CRC1_ResetCRC(crc);
	CRC1_GetBlockCRC(crc, &header, 4, &result );

	CRC1_GetBlockCRC(crc, ReceiveBuffer, length, &result);

	bool ret = (result==0) ? TRUE : FALSE;

	return ret;
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
	intendedAddress |= ReceiveBuffer[4] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[3] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[2] & 0xFF;
	intendedAddress <<= 8;
	intendedAddress |= ReceiveBuffer[1] & 0xFF;

	if(intendedAddress == MyAddress || intendedAddress == 0) // 0 = broadcast
	{
		bool crcOK = _CheckIncomingPacketCRC();
		if(crcOK == FALSE) //NACK message
		{
			badCommandsRecieved++;
			return;
		}

		switch(ReceiveBuffer[5])
		{
		case CommandPing:
			_SendResponseMessage(CommandPing, crcOK);
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
			_SendResponseMessage(CommandGetStatus, crcOK);
			break;
		case CommandAssociate:
			_SendResponseMessage(CommandAssociate, crcOK);
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
		case CommandSetPosGetData:
			if(crcOK == TRUE)
				_DoubleMoveTo(&ReceiveBuffer[6]);
			_SendResponseMessage(CommandSetPosGetData, crcOK);
			break;
		case CommandGetHallPos:
			_GetHallPos();
			break;
		case CommandGetPots:
			_GetPots();
			break;
		case CommandGetCurrent:
			_GetCurrent();
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
		while(UART_PDD_GetTxCompleteStatus(UART1_BASE_PTR) == 0U)
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

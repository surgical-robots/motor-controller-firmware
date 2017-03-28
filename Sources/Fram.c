/*
 * Fram.c
 *
 *  Created on: Mar 11, 2017
 *      Author: lcubrich
 */
#include "Fram.h"
#include <string.h>
#include "I2C.h"
#include "CRC1.h"
#include "motor.h"
#define CONFIG1_START 0x00
#define CONFIG2_START (sizeof(Config_t)+1)
#define ERROR_COUNT_START	(sizeof(Config_t) * 2 + 1)

Config_t configs[2];
int configIdx = 0;
LDD_TDeviceData* crc;
uint8_t dataToSend[sizeof(Config_t) + 2];
int errorCount;
void Fram_init()
{
	crc = CRC1_Init(NULL);
	I2C_SelectSlave(80);
	word dummy;
	// get error count
	uint8_t errorDater[2 + sizeof(uint32_t)];
	errorDater[0] = 0x00;
	errorDater[1] = ERROR_COUNT_START;

	errorDater[2] = 0;
	errorDater[3] = 0;
	errorDater[4] = 0;
	errorDater[5] = 0;

	Fram_CommandComplete = FALSE;
 	I2C_SendBlock(errorDater, 2, &dummy);
	while(!Fram_CommandComplete);

	Fram_CommandComplete = FALSE;
	while(!Fram_CommandComplete)
		I2C_RecvBlock(&errorCount, sizeof(uint32_t), &dummy);

}

/** Write a config to the FRAM */
void Fram_write()
{
	if(!Fram_CommandComplete)
		return;

	Config_t* currentConfig = Fram_getCurrent();
	uint32_t result;

	CRC1_ResetCRC(crc);
	CRC1_GetBlockCRC(crc, currentConfig, sizeof(Config_t) - sizeof(uint32_t), &result );
	currentConfig->CRC = result;
	word dummy;


	I2C_SelectSlave(80);

	// start address depends on config index
	dataToSend[0] = 0x00;
	dataToSend[1] = configIdx == 0 ? CONFIG1_START : CONFIG2_START;

	// copy the appropriate config into dataToSend
	memcpy(&dataToSend[2],currentConfig,sizeof(Config_t) );

	// write to FRAM
	Fram_CommandComplete = FALSE;
	I2C_SendBlock(&dataToSend[0], sizeof(dataToSend), &dummy);

	// switch to the other config
	configIdx = ((configIdx+1) % 2);

}

Config_t* Fram_getCurrent()
{
	return &configs[configIdx];
}

/** Recall the config stored in the FRAM into the specified location
 * param[out] config	A pointer to the config to read the FRAM data into
 * return	Whether FRAM data was successfully recalled
 * */
int Fram_recall()
{
	Config_t* config0 = &configs[0];
	I2C_SelectSlave(80);
	uint8_t dater[2];
	dater[0] = 0x00;
	dater[1] = CONFIG1_START;
	word dummy;

	Fram_CommandComplete = FALSE;
	while(!Fram_CommandComplete)
		I2C_SendBlock(dater, 2, &dummy);


	Fram_CommandComplete = FALSE;
	while(!Fram_CommandComplete)
		I2C_RecvBlock(config0, sizeof(Config_t), &dummy);

	uint32_t result = 0;
	CRC1_ResetCRC(crc);
	CRC1_GetBlockCRC(crc, config0, sizeof(Config_t) - sizeof(uint32_t), &result );
	if(config0->CRC == (uint16_t)result)
	{
		return TRUE;
	}
	else
	{
		dater[0] = 0x00;
		dater[1] = CONFIG2_START;

		Fram_CommandComplete = FALSE;
		while(!Fram_CommandComplete)
			I2C_SendBlock(dater, 2, &dummy);

		Fram_CommandComplete = FALSE;
		while(!Fram_CommandComplete)
			I2C_RecvBlock(config0, sizeof(Config_t), &dummy);

		CRC1_ResetCRC(crc);
		CRC1_GetBlockCRC(crc, config0, sizeof(Config_t) - sizeof(uint32_t), &result );

		if(config0->CRC == (uint16_t)result)
		{
			return TRUE;
		}
		else
		{
			uint8_t errorDater[2 + sizeof(uint32_t)];
			errorDater[0] = 0x00;
			errorDater[1] = ERROR_COUNT_START;

			errorCount++;
			memcpy(&errorDater[2], &errorCount, sizeof(uint32_t));
			Fram_CommandComplete = FALSE;
			while(!Fram_CommandComplete)
				I2C_SendBlock(errorDater, sizeof(errorDater), &dummy);

			config0->motor1.ControlMode = MotorControlModeReserved;
			config0->motor2.ControlMode = MotorControlModeReserved;

			config0->motor1.KP = 1;
			config0->motor2.KP = 1;

			config0->motor1.Setpoint = 0;
			config0->motor2.Setpoint = 0;

			config0->motor1.ShaftCounter = 0;
			config0->motor2.ShaftCounter = 0;

			config0->motor1.ClicksPerRev = 1;
			config0->motor2.ClicksPerRev = 1;

			config0->motor1.SpeedMin = 0;
			config0->motor2.SpeedMin = 0;

			config0->motor1.CurrentMax = 250;
			config0->motor2.CurrentMax = 250;

			config0->motor1.PotZero = 0;
			config0->motor2.PotZero = 0;

			return FALSE;
		}
	}

}

int Fram_getErrorCount()
{
	return errorCount;
}

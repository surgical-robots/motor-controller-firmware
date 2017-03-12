/*
 * Fram.h
 *
 *  Created on: Mar 11, 2017
 *      Author: lcubrich
 */

#ifndef FRAM_H_
#define FRAM_H_

#include "PE_Types.h"

typedef struct MotorConfig {
	uint32_t KP;
	int32_t Setpoint;
	int32_t ShaftCounter;
	uint16_t ClicksPerRev;
	uint16_t SpeedMin;
	uint16_t CurrentMax;
	uint16_t PotZero;
	uint8_t ControlMode;
} MotorConfig_t;

typedef struct Config {
	MotorConfig_t motor1;
	MotorConfig_t motor2;
	uint32_t CRC;
} Config_t;

/** Initialize the FRAM */
void Fram_init();

/** Write a config to the FRAM */
void Fram_write();

Config_t* Fram_getCurrent();

/** Recall the config stored in the FRAM into the specified location
 * param[out] config	A pointer to the config to read the FRAM data into
 * return	Whether FRAM data was successfully recalled
 * */
int Fram_recall();



#endif /* FRAM_H_ */

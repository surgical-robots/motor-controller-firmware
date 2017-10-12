/*
 * motor.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#define SHIFT_SIZE		(10000)
#define SAMPLE_SIZE		(100)
#define MAXON			(0)
#define FAULHABER		(1)
#define CLAMPING_M1		(0)
#define CLAMPING_M2		(0)

#include "PE_Types.h"

void Motor_Init();
void Motor_Motor1_Tach1();
void Motor_Motor1_Tach2();
void Motor_Motor1_Tach3();
void Motor_Motor2_Tach1();
void Motor_Motor2_Tach2();
void Motor_Motor2_Tach3();
void Motor_Update();

typedef enum {
	MotorControlModeReserved,
	MotorControlModeJog,
	MotorControlModeRelativeStep,
	MotorControlModePotentiometer
} motorControlMode_t;

typedef struct MotorConfig {
	uint32_t KP;
	int32_t KI;
	int32_t KD;
	int32_t Setpoint;
	int32_t ShaftCounter;
	int16_t ClicksPerRev;
	uint16_t SpeedMin;
	uint16_t CurrentMax;
	uint16_t PotZero;
	uint16_t Deadband;
	motorControlMode_t ControlMode;
} MotorConfig_t;

MotorConfig_t Motor1;
MotorConfig_t Motor2;

int32_t Motor1_Position;
int32_t Motor2_Position;

int32_t Motor1_PotVal;
int32_t Motor2_PotVal;

uint16_t Motor1_Current;
uint16_t Motor2_Current;

bool Motor1_IsJogging;
bool Motor2_IsJogging;

bool Motor_IsMoving;

uint32 Motor1_AvgCurrent;
uint32 Motor2_AvgCurrent;
uint32 oldAvgCurrent;

uint16 Motor1_LimitedSpeed;
uint16 Motor2_LimitedSpeed;

#endif /* MOTOR_H_ */

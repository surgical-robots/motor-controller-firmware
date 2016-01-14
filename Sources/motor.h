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

#include "PE_Types.h"

void Motor_Init();
void Motor_Motor1_Tach();
void Motor_Motor2_Tach();
void Motor_Update();

typedef enum {
	MotorControlModeReserved,
	MotorControlModePotentiometer,
	MotorControlModeRelativeStep
} motorControlMode_t;

motorControlMode_t Motor1_ControlMode;
motorControlMode_t Motor2_ControlMode;

bool Motor1_IsJogging;
bool Motor2_IsJogging;

int32 Motor1_KP;
int32 Motor2_KP;

int32 Motor1_Setpoint;
int32 Motor2_Setpoint;

int32 Motor1_Position;
int32 Motor2_Position;

bool Motor_IsMoving;

uint16 Motor1_PotVal;
uint16 Motor2_PotVal;

uint16 Motor1_Current;
uint16 Motor2_Current;

int32 Motor1_ShaftCounter;
int32 Motor2_ShaftCounter;

uint16 Motor1_SpeedMax;
uint16 Motor2_SpeedMax;

uint16 Motor1_CurrentMax;
uint16 Motor2_CurrentMax;

uint16 Motor1_PotZero;
uint16 Motor2_PotZero;

uint16 Motor1_ClicksPerRev;
uint16 Motor2_ClicksPerRev;

uint32 Motor1_AvgCurrent;
uint32 Motor2_AvgCurrent;
uint32 oldAvgCurrent;

uint16 Motor1_LimitedSpeed;
uint16 Motor2_LimitedSpeed;

#endif /* MOTOR_H_ */

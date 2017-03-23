/*
 * motor.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define SHIFT_SIZE		(10000)		// shift shift to get fixed point math
#define SAMPLE_SIZE		(100)		// sample size for motor current running average
#define MAXON			(1)			// set to 1 if using Maxon motors, else 0
#define FAULHABER		(0)			// set to 1 if using Faulhaber motors, else 0

#define HIGH_RES		(1)			// set to 1 to count all Hall sensor ticks, 0 to only count 1 Hall sensor

#define CLAMPING_M1		(1)			// set to 1 to enable clamping on motor 1
#define CLAMPING_M2		(0)			// set to 1 to enable clamping on motor 2

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

uint16 deadband;

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

uint16 Motor1_SpeedMin;
uint16 Motor2_SpeedMin;

uint16 Motor1_CurrentMax;
uint16 Motor2_CurrentMax;

uint16 Motor1_PotZero;
uint16 Motor2_PotZero;

int16 Motor1_ClicksPerRev;
int16 Motor2_ClicksPerRev;

uint32 Motor1_AvgCurrent;
uint32 Motor2_AvgCurrent;
uint32 oldAvgCurrent;

uint16 Motor1_LimitedSpeed;
uint16 Motor2_LimitedSpeed;

#endif /* MOTOR_H_ */

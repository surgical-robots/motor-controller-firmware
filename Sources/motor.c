/*
 * motor.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: jcarlson5
 */
#include "motor.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Pins1.h"
#include "M1_DIR.h"
#include "M2_DIR.h"
#include "M1_EN.h"
#include "VREF.h"
#include "M2_EN.h"
#include "LED_BLUE.h"
#include "LED_GREEN.h"
#include "M1_HALL1.h"
#include "M2_HALL1.h"
#include "M1_HALL2.h"
#include "M2_HALL2.h"
#include "M1_HALL3.h"
#include "M2_HALL3.h"

int32 maxVal = 255;
bool m1_dir, m2_dir, moving;
bool m1_clamp = FALSE;
bool m2_clamp = FALSE;
int32 m1_error, m2_error, m1_lastpos, m2_lastpos, m1_p, m1_i, m1_d,
		m2_p, m2_i, m2_d, m1_output, m2_output;
uint16 deadband = 1;
uint16 i = 0;
uint16 j = 0;

void Motor_Init() {
	Motor1.Setpoint = 0;
	Motor2.Setpoint = 0;
	Motor1.Deadband = 1;
	Motor2.Deadband = 1;
	Motor1.KP = 1;
	Motor2.KP = 1;
	Motor1.KI = 0;
	Motor2.KI = 0;
	Motor1.KD = 0;
	Motor2.KD = 0;
}

void Motor_Motor1_Tach1() {
	// We don't count clicks when we're jogging
	if (Motor1.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M1_HALL1_GetVal()) {
			if (M1_HALL2_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		} else {
			if (M1_HALL2_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		}
	}
	else if (FAULHABER)
	{
		if (M1_HALL1_GetVal()) {
			if (M1_HALL2_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		} else {
			if (M1_HALL2_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		}
	}
}

void Motor_Motor1_Tach2() {
	// We don't count clicks when we're jogging
	if (Motor1.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M1_HALL2_GetVal()) {
			if (M1_HALL3_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		} else {
			if (M1_HALL3_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		}
	}
	else if(FAULHABER)
	{
		if (M1_HALL2_GetVal()) {
			if (M1_HALL3_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		} else {
			if (M1_HALL3_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		}
	}
}

void Motor_Motor1_Tach3() {
	// We don't count clicks when we're jogging
	if (Motor1.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M1_HALL3_GetVal()) {
			if (M1_HALL1_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		} else {
			if (M1_HALL1_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		}
	}
	else if(FAULHABER)
	{
		if (M1_HALL3_GetVal()) {
			if (M1_HALL1_GetVal())
				Motor1.ShaftCounter++;
			else
				Motor1.ShaftCounter--;
		} else {
			if (M1_HALL1_GetVal())
				Motor1.ShaftCounter--;
			else
				Motor1.ShaftCounter++;
		}
	}
}

void Motor_Motor2_Tach1() {
	if (Motor2.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M2_HALL1_GetVal()) {
			if (M2_HALL2_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		} else {
			if (M2_HALL2_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		}
	}
	else if(FAULHABER)
	{
		if (M2_HALL1_GetVal()) {
			if (M2_HALL2_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		} else {
			if (M2_HALL2_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		}
	}
}

void Motor_Motor2_Tach2() {
	if (Motor2.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M2_HALL2_GetVal()) {
			if (M2_HALL3_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		} else {
			if (M2_HALL3_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		}
	}
	else if(FAULHABER)
	{
		if (M2_HALL2_GetVal()) {
			if (M2_HALL3_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		} else {
			if (M2_HALL3_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		}
	}
}

void Motor_Motor2_Tach3() {
	if (Motor2.ControlMode == MotorControlModeReserved)
		return;
	if(MAXON)
	{
		if (M2_HALL3_GetVal()) {
			if (M2_HALL1_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		} else {
			if (M2_HALL1_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		}
	}
	else if(FAULHABER)
	{
		if (M2_HALL3_GetVal()) {
			if (M2_HALL1_GetVal())
				Motor2.ShaftCounter++;
			else
				Motor2.ShaftCounter--;
		} else {
			if (M2_HALL1_GetVal())
				Motor2.ShaftCounter--;
			else
				Motor2.ShaftCounter++;
		}
	}
}

void Motor_Update() {
	oldAvgCurrent = Motor1_AvgCurrent;
	Motor1_AvgCurrent = ((oldAvgCurrent*SAMPLE_SIZE*SHIFT_SIZE) + ((Motor1_Current * SHIFT_SIZE) - oldAvgCurrent*SHIFT_SIZE)) / SAMPLE_SIZE / SHIFT_SIZE;

	if (!Motor1_IsJogging) {
		if (Motor1.ControlMode == MotorControlModePotentiometer) {
			Motor1_Position = Motor1_PotVal;
		} else if (Motor1.ControlMode == MotorControlModeRelativeStep) {
			Motor1_Position = Motor1.ShaftCounter;
		}
		if((Motor1_Position - m1_lastpos) != 0)
			moving = TRUE;
		else
			moving = FALSE;

		if (Motor1.ControlMode != MotorControlModeReserved && Motor1.ControlMode != MotorControlModeJog) {

			m1_error = Motor1.Setpoint - Motor1_Position;

			m1_p = (int)Motor1.KP * m1_error / SHIFT_SIZE;
//			m1_i += Motor1.KI * m1_error;
//			m1_d = Motor1.KD * (Motor1_Position - m1_lastpos);
			m1_i = 0;
			m1_d = 0;

			m1_output = m1_p + m1_i + m1_d;

			if (m1_error > 0) {
				m1_dir = 1;
			} else {
				m1_dir = 0;
				m1_output = -m1_output;
			}

			m1_output += Motor1.SpeedMin;

			if(m1_output > 255)
				m1_output = 255;

			M1_DIR_PutVal(m1_dir);

//			if(CLAMPING_M1 && !moving)
//			{
//				if(!m1_clamp)
//				{
//					Motor1_LimitedSpeed = m1_output;
//					m1_clamp = TRUE;
//				}
//				if(Motor1_LimitedSpeed > Motor1.SpeedMin && i++ > 5)
//				{
//					Motor1_LimitedSpeed--;
//					i = 0;
//				}
//				if(i > 5000)
//					i = 11;
//				if(abs(m1_error) < 5)
//					M1_EN_SetRatio8(0);
//				else
//					M1_EN_SetRatio8(Motor1_LimitedSpeed);
//			}
			if (abs(m1_error) < Motor1.Deadband)
			{
				m1_clamp = FALSE;
				m1_output = 0;
				M1_EN_SetRatio8(0);
			}
			else
			{
				m1_clamp = FALSE;
				M1_EN_SetRatio8(m1_output);
			}
		}
		else
		{
			m1_output = 0;
			M1_EN_SetRatio8(0);
		}
		m1_lastpos = Motor1_Position;
	}

	oldAvgCurrent = Motor2_AvgCurrent;
	Motor2_AvgCurrent = ((oldAvgCurrent*SAMPLE_SIZE*SHIFT_SIZE) + ((Motor2_Current * SHIFT_SIZE) - oldAvgCurrent*SHIFT_SIZE)) / SAMPLE_SIZE / SHIFT_SIZE;

	if (!Motor2_IsJogging) {
		if (Motor2.ControlMode == MotorControlModePotentiometer) {
			Motor2_Position = Motor2_PotVal;
		} else if (Motor2.ControlMode == MotorControlModeRelativeStep) {
			Motor2_Position = Motor2.ShaftCounter;
		}
		if((Motor2_Position - m2_lastpos) != 0)
			moving = TRUE;
		else
			moving = FALSE;

		if (Motor2.ControlMode != MotorControlModeReserved && Motor2.ControlMode != MotorControlModeJog) {
			m2_error = Motor2.Setpoint - Motor2_Position;

			m2_p = (int)Motor2.KP * m2_error / SHIFT_SIZE;
//			m2_i += Motor2.KI * m2_error;
//			m2_d = Motor2.KD * (Motor2_Position - m2_lastpos);
			m2_i = 0;
			m2_d = 0;

			m2_output = m2_p + m2_i + m2_d;

			if (m2_error > 0) {
				m2_dir = 1;
			} else {
				m2_dir = 0;
				m2_output = -m2_output;
			}

			m2_output += Motor2.SpeedMin;

			if(m2_output > 255)
				m2_output = 255;

			M2_DIR_PutVal(m2_dir);

//			if(CLAMPING_M2 && !moving)
//			{
//				if(!m2_clamp)
//				{
//					Motor2_LimitedSpeed = m2_output;
//					m2_clamp = TRUE;
//				}
//				if(Motor2_LimitedSpeed > Motor2.SpeedMin && j++ > 5)
//				{
//					Motor2_LimitedSpeed--;
//					j = 0;
//				}
//				if(j > 5000)
//					j = 11;
//				if(abs(m2_error) < Motor2.Deadband)
//					M2_EN_SetRatio8(0);
//				else
//					M2_EN_SetRatio8(Motor2_LimitedSpeed);
//			}
			if (abs(m2_error) < Motor2.Deadband)
			{
				m2_output = 0;
				M2_EN_SetRatio8(0);
			}
			else
				M2_EN_SetRatio8(m2_output);
		}
		else
		{
			m2_output = 0;
			M2_EN_SetRatio8(0);
		}
		m2_lastpos = Motor2_Position;

		if (m2_output > 5 || m1_output > 5) {
			Motor_IsMoving = TRUE;
		}
		if (m2_output < 5 && m1_output < 5) {
			Motor_IsMoving = FALSE;
		}
	}
}


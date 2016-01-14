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

int32 maxVal = 255;
bool m1_dir, m2_dir;
int32 m1_error, m2_error, m1_lastpos, m2_lastpos, ki, kd, m1_p, m1_i, m1_d,
		m2_p, m2_i, m2_d, m1_output, m2_output;

void Motor_Init() {
	Motor1_Setpoint = 0;
	Motor2_Setpoint = 0;
	Motor1_KP = 0;
	Motor2_KP = 0;
}

void Motor_Motor1_Tach() {
	// We don't count clicks when we're jogging
	if (Motor1_IsJogging)
		return;
	if (M1_HALL1_GetVal()) {
		if (M1_HALL2_GetVal())
			Motor1_ShaftCounter++;
		else
			Motor1_ShaftCounter--;
	} else {
		if (M1_HALL2_GetVal())
			Motor1_ShaftCounter--;
		else
			Motor1_ShaftCounter++;
	}
}

void Motor_Motor2_Tach() {
	if (Motor2_IsJogging)
		return;
	if (M2_HALL1_GetVal()) {
		if (M2_HALL2_GetVal())
			Motor2_ShaftCounter++;
		else
			Motor2_ShaftCounter--;
	} else {
		if (M2_HALL2_GetVal())
			Motor2_ShaftCounter--;
		else
			Motor2_ShaftCounter++;
	}
}

void Motor_Update() {

	oldAvgCurrent = Motor1_AvgCurrent;
	Motor1_AvgCurrent = ((oldAvgCurrent*SAMPLE_SIZE*SHIFT_SIZE) + ((Motor1_Current * SHIFT_SIZE) - oldAvgCurrent*SHIFT_SIZE)) / SAMPLE_SIZE / SHIFT_SIZE;

	if (!Motor1_IsJogging) {
		if (Motor1_ControlMode == MotorControlModePotentiometer) {
			Motor1_Position = Motor1_PotVal;
		} else if (Motor1_ControlMode == MotorControlModeRelativeStep) {
			Motor1_Position = Motor1_ShaftCounter;
		}

		if (Motor1_ControlMode != MotorControlModeReserved) {
			m1_error = Motor1_Setpoint - Motor1_Position;

			m1_p = Motor1_KP * m1_error / SHIFT_SIZE;
			m1_i += ki * m1_error;
			m1_d = kd * (Motor1_Position - m1_lastpos);

			m1_output = m1_p + m1_i + m1_d;

			if (m1_output > 0) {
				m1_dir = 1;
			} else {
				m1_dir = 0;
				m1_output = -m1_output;
			}

			m1_lastpos = Motor1_Position;

			M1_DIR_PutVal(m1_dir);

			if((Motor1_AvgCurrent > Motor1_CurrentMax) && (Motor1_LimitedSpeed > 0))
			{
				Motor1_LimitedSpeed--;
			}
			else if((Motor1_AvgCurrent < Motor1_CurrentMax) && (Motor1_LimitedSpeed < Motor1_SpeedMax))
			{
				Motor1_LimitedSpeed++;
			}

			if (abs(m1_error) < 5) {
				m1_output = 0;
				M1_EN_SetRatio8(0);
			} else if (m1_output > 10) {
				if (m1_output > Motor1_LimitedSpeed)
					M1_EN_SetRatio8(Motor1_LimitedSpeed);
				else
					M1_EN_SetRatio8(m1_output);
			}
		} else {
			m1_output = 0;
			M1_EN_SetRatio8(0);
		}
	}

	oldAvgCurrent = Motor2_AvgCurrent;
	Motor2_AvgCurrent = ((oldAvgCurrent*SAMPLE_SIZE*SHIFT_SIZE) + ((Motor2_Current * SHIFT_SIZE) - oldAvgCurrent*SHIFT_SIZE)) / SAMPLE_SIZE / SHIFT_SIZE;

	if (!Motor2_IsJogging) {
		if (Motor2_ControlMode == MotorControlModePotentiometer) {
			Motor2_Position = Motor2_PotVal;
		} else if (Motor2_ControlMode == MotorControlModeRelativeStep) {
			Motor2_Position = Motor2_ShaftCounter;
		}

		if (Motor2_ControlMode != MotorControlModeReserved) {
			m2_error = Motor2_Setpoint - Motor2_Position;

			m2_p = Motor2_KP * m2_error / SHIFT_SIZE;
			m2_i += ki * m2_error;
			m2_d = kd * (Motor2_Position - m2_lastpos);

			m2_output = m2_p + m2_i + m2_d;

			if (m2_output > 0) {
				m2_dir = 1;
			} else {
				m2_dir = 0;
				m2_output = -m2_output;
			}

			m2_lastpos = Motor2_Position;

			M2_DIR_PutVal(m2_dir);

			if((Motor2_AvgCurrent > Motor2_CurrentMax) && (Motor2_LimitedSpeed > 0))
			{
				Motor1_LimitedSpeed--;
			}
			else if((Motor2_AvgCurrent < Motor2_CurrentMax) && (Motor2_LimitedSpeed < Motor2_SpeedMax))
			{
				Motor1_LimitedSpeed++;
			}

			if (abs(m2_error) < 5) {
				m2_output = 0;
				M2_EN_SetRatio8(0);
			} else if (m2_output > 10) {
				if (m2_output > Motor2_LimitedSpeed)
					M2_EN_SetRatio8(Motor2_LimitedSpeed);
				else
					M2_EN_SetRatio8(m2_output);
			}
		} else {
			m2_output = 0;
			M2_EN_SetRatio8(0);
		}

		if (m2_output > 5 || m1_output > 5) {
			Motor_IsMoving = TRUE;
		}
		if (m2_output < 5 && m1_output < 5) {
			Motor_IsMoving = FALSE;
		}
	}

}


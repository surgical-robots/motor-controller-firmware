/* ###################################################################
**     Filename    : main.c
**     Project     : BLDC_KV10
**     Processor   : MKV10Z32VFM7
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-11-20, 21:17, # CodeGen: 21
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "M2_EN.h"
#include "PwmLdd1.h"
#include "Cpu.h"
#include "ADC0.h"
#include "AdcLdd1.h"
#include "ADC1.h"
#include "AdcLdd2.h"
#include "M1_DIR.h"
#include "BitIoLdd1.h"
#include "M2_DIR.h"
#include "BitIoLdd2.h"
#include "TXEN.h"
#include "BitIoLdd3.h"
#include "VREF.h"
#include "DacLdd1.h"
#include "M1_HALL1.h"
#include "ExtIntLdd1.h"
#include "M2_HALL1.h"
#include "ExtIntLdd2.h"
#include "M1_HALL2.h"
#include "ExtIntLdd6.h"
#include "M1_HALL3.h"
#include "ExtIntLdd5.h"
#include "M2_HALL2.h"
#include "ExtIntLdd4.h"
#include "M2_HALL3.h"
#include "ExtIntLdd3.h"
#include "LED_RED.h"
#include "BitIoLdd7.h"
#include "LED_GREEN.h"
#include "BitIoLdd8.h"
#include "LED_BLUE.h"
#include "BitIoLdd9.h"
#include "FastLoop.h"
#include "TimerIntLdd1.h"
#include "TU2.h"
#include "SlowLoop.h"
#include "TimerIntLdd2.h"
#include "TU3.h"
#include "I2C.h"
#include "IntI2cLdd1.h"
#include "CRC1.h"
#include "TU1.h"
#include "UART.h"
#include "PwmLdd2.h"
#include "ASerialLdd1.h"
#include "M1_EN.h"
#include "AdcLdd1.h"
#include "AdcLdd2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
#include "motor.h"
#include "Command.h"
#include "Fram.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
int redCounts = 0;


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */


  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */


  	Motor_Init();
  	// initialize buffers and variables
	GetHalls = TRUE;
	GetPots = TRUE;
	GetCurrent = TRUE;

	// Initialize the FRAM
//	Fram_init();
//
//	Fram_recall();

	Config_t* currentConfig = Fram_getCurrent();

	Motor1_ControlMode = currentConfig->motor1.ControlMode;
	Motor2_ControlMode = currentConfig->motor2.ControlMode;

	Motor1_KP = currentConfig->motor1.KP;
	Motor2_KP = currentConfig->motor2.KP;

	// make setpoint = current position on startup
	Motor1_Setpoint = currentConfig->motor1.ShaftCounter;
	Motor2_Setpoint = currentConfig->motor2.ShaftCounter;

	Motor1_ShaftCounter = currentConfig->motor1.ShaftCounter;
	Motor2_ShaftCounter = currentConfig->motor2.ShaftCounter;

	Motor1_ClicksPerRev = currentConfig->motor1.ClicksPerRev;
	Motor2_ClicksPerRev = currentConfig->motor2.ClicksPerRev;

	Motor1_SpeedMin = currentConfig->motor1.SpeedMin;
	Motor2_SpeedMin = currentConfig->motor2.SpeedMin;

	Motor1_CurrentMax = currentConfig->motor1.CurrentMax;
	Motor2_CurrentMax = currentConfig->motor2.CurrentMax;

	Motor1_PotZero = currentConfig->motor1.PotZero;
	Motor2_PotZero = currentConfig->motor2.PotZero;

	// set motor driver VREF
//	if(Motor1_CurrentMax > Motor2_CurrentMax)
//	{
//		uint32_t dummyVal = Motor2_CurrentMax << 4;
//		VREF_SetValue(&dummyVal);
//	}
//	else
//	{
//		uint32_t dummyVal = Motor1_CurrentMax << 4;
//		VREF_SetValue(&dummyVal);
//	}

	LED_RED_SetVal();
    Command_Init();
    // Start calibration of the ADC.
    // Once calibration is finished, CalibrationEnd will start continuous conversion.
    ADC1_Calibrate(0);
    ADC0_Calibrate(0);

  int SlowLoopCounter=0;

//  TXEN_SetVal();
  while(TRUE)
  {
	  Command_Task();

	  if(timerFlag)
	  {
		  // Begin write parameters to FRAM
			Config_t* config = Fram_getCurrent();
			EnterCritical();
			config->motor1.ControlMode = Motor1_ControlMode;
			config->motor2.ControlMode = Motor2_ControlMode;

			config->motor1.KP = Motor1_KP;
			config->motor2.KP = Motor2_KP;

			config->motor1.Setpoint = Motor1_Setpoint;
			config->motor2.Setpoint = Motor2_Setpoint;

			config->motor1.ShaftCounter = Motor1_ShaftCounter;
			config->motor2.ShaftCounter = Motor2_ShaftCounter;

			config->motor1.ClicksPerRev = Motor1_ClicksPerRev;
			config->motor2.ClicksPerRev = Motor2_ClicksPerRev;

			config->motor1.SpeedMin = Motor1_SpeedMin;
			config->motor2.SpeedMin = Motor2_SpeedMin;

			config->motor1.CurrentMax = Motor1_CurrentMax;
			config->motor2.CurrentMax = Motor2_CurrentMax;

			config->motor1.PotZero = Motor1_PotZero;
			config->motor2.PotZero = Motor2_PotZero;
			ExitCritical();
			Fram_write();
			// End write parameters to FRAM


		  if(Motor_IsMoving)
		  {
			  LED_BLUE_NegVal();
		  } else {
			  LED_BLUE_SetVal();
		  }

		  if(redCounts < 6)
		  {
			  LED_RED_NegVal();
			  redCounts++;
		  }

		  if(IdentificationLed)
		  {
			  LED_RED_NegVal();
		  } else {
			  if(redCounts >= 6)
				  LED_RED_SetVal();
		  }

		  SlowLoopCounter++;

		  if(SlowLoopCounter == 20)
		  {
			  SlowLoopCounter = 0;
		  }

		  timerFlag = 0;
	  }
  }
//  TXEN_ClrVal();
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.10]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

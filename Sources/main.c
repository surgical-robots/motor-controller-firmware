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
#include "M2_ANALOG.h"
#include "AdcLdd1.h"
#include "M1_ANALOG.h"
#include "AdcLdd2.h"
#include "M1_DIR.h"
#include "BitIoLdd1.h"
#include "M2_DIR.h"
#include "BitIoLdd2.h"
#include "TXEN.h"
#include "BitIoLdd3.h"
#include "VREF.h"
#include "BitIoLdd4.h"
#include "M1_HALL1.h"
#include "ExtIntLdd1.h"
#include "M2_HALL1.h"
#include "ExtIntLdd2.h"
#include "M1_HALL2.h"
#include "BitIoLdd5.h"
#include "M2_HALL2.h"
#include "BitIoLdd6.h"
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
	LED_RED_SetVal();
    Command_Init();
    // Start calibration of the ADC.
    // Once calibration is finished, CalibrationEnd will start continuous conversion.
    M1_ANALOG_Calibrate(0);
    M2_ANALOG_Calibrate(0);

  Motor_Init();
  int SlowLoopCounter=0;
  int setpoint = 100;
//  TXEN_SetVal();
  while(TRUE)
  {
	  Command_Task();
	  if(timerFlag)
	  {
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

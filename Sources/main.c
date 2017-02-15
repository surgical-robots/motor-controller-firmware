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
  	Motor_Init();

	uint16 readWriteSize;
	int32 dummyVar = 0;
	byte i2cTxBuffer[47];
	byte i2cRxBuffer[45];
	ComSuccess = FALSE;
	GetHalls = FALSE;
	GetPots = FALSE;
	GetCurrent = FALSE;
	// set slave address and memory address
	I2C_SelectSlave(80);
	i2cTxBuffer[0] = (0x00);
	i2cTxBuffer[1] = (0x00);
	while(!ComSuccess)
	{
		I2C_SendBlock(&i2cTxBuffer[0], 2, &readWriteSize);
	}
	ComSuccess = FALSE;

	while(!ComSuccess)
	{
		I2C_RecvBlock(&i2cRxBuffer[0], 45, &readWriteSize);
	}
	ComSuccess = FALSE;
	//read Motor1 configuration
	Motor1_ControlMode = i2cRxBuffer[0];
	dummyVar |= i2cRxBuffer[4] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[3] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[2] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[1] & 0xFF;
	Motor1_KP = dummyVar;
	dummyVar = 0;
	dummyVar |= i2cRxBuffer[8] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[7] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[6] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[5] & 0xFF;
	Motor1_Setpoint = dummyVar;
	dummyVar = 0;
	dummyVar |= i2cRxBuffer[12] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[11] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[10] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[9] & 0xFF;
	Motor1_ShaftCounter = dummyVar;
	Motor1_ClicksPerRev = i2cRxBuffer[13] | (i2cRxBuffer[14] << 8);
	Motor1_SpeedMin = i2cRxBuffer[15] | (i2cRxBuffer[16] << 8);
	Motor1_CurrentMax = i2cRxBuffer[17] | (i2cRxBuffer[18] << 8);
	Motor1_PotZero = i2cRxBuffer[19] | (i2cRxBuffer[20] << 8);

	// read Motor2 configuration
	Motor2_ControlMode = i2cRxBuffer[21];
	dummyVar = 0;
	dummyVar |= i2cRxBuffer[25] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[24] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[23] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[22] & 0xFF;
	Motor2_KP = dummyVar;
	dummyVar = 0;
	dummyVar |= i2cRxBuffer[29] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[28] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[27] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[26] & 0xFF;
	Motor2_Setpoint = dummyVar;
	dummyVar = 0;
	dummyVar |= i2cRxBuffer[33] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[32] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[31] & 0xFF;
	dummyVar <<= 8;
	dummyVar |= i2cRxBuffer[30] & 0xFF;
	Motor2_ShaftCounter = dummyVar;
	Motor2_ClicksPerRev = i2cRxBuffer[34] | (i2cRxBuffer[35] << 8);
	Motor2_SpeedMin = i2cRxBuffer[36] | (i2cRxBuffer[37] << 8);
	Motor2_CurrentMax = i2cRxBuffer[38] | (i2cRxBuffer[39] << 8);
	Motor2_PotZero = i2cRxBuffer[40] | (i2cRxBuffer[41] << 8);

	// read controller configurations
	GetHalls = i2cRxBuffer[42];
	GetPots = i2cRxBuffer[43];
	GetCurrent = i2cRxBuffer[44];

	// set motor driver VREF
//	if(Motor1_CurrentMax > Motor2_CurrentMax)
//		VREF_SetValue(Motor1_CurrentMax);
//	else
//		VREF_SetValue(Motor2_CurrentMax);

	LED_RED_SetVal();
    Command_Init();
    // Start calibration of the ADC.
    // Once calibration is finished, CalibrationEnd will start continuous conversion.
    M1_ANALOG_Calibrate(0);
    M2_ANALOG_Calibrate(0);

  int SlowLoopCounter=0;
  int saveLoopCounter=0;
  int setpoint = 100;
//  TXEN_SetVal();
  while(TRUE)
  {
	  Command_Task();

	  saveLoopCounter++;
	  if(saveLoopCounter > 1000)
	  {
		  // starting memory address
		  i2cTxBuffer[0] = (0x00);
		  i2cTxBuffer[1] = (0x00);
		  // motor1 properties
		  i2cTxBuffer[2] = (byte)Motor1_ControlMode;
		  i2cTxBuffer[3] = (Motor1_KP & 0x000000ff);
		  i2cTxBuffer[4] = (Motor1_KP & 0x0000ff00) >> 8;
		  i2cTxBuffer[5] = (Motor1_KP & 0x00ff0000) >> 16;
		  i2cTxBuffer[6] = (Motor1_KP & 0xff000000) >> 24;
		  i2cTxBuffer[7] = (Motor1_Setpoint & 0x000000ff);
		  i2cTxBuffer[8] = (Motor1_Setpoint & 0x0000ff00) >> 8;
		  i2cTxBuffer[9] = (Motor1_Setpoint & 0x00ff0000) >> 16;
		  i2cTxBuffer[10] = (Motor1_Setpoint & 0xff000000) >> 24;
		  i2cTxBuffer[11] = (Motor1_ShaftCounter & 0x000000ff);
		  i2cTxBuffer[12] = (Motor1_ShaftCounter & 0x0000ff00) >> 8;
		  i2cTxBuffer[13] = (Motor1_ShaftCounter & 0x00ff0000) >> 16;
		  i2cTxBuffer[14] = (Motor1_ShaftCounter & 0xff000000) >> 24;
		  i2cTxBuffer[15] = (Motor1_ClicksPerRev & 0x00ff);
		  i2cTxBuffer[16] = (Motor1_ClicksPerRev & 0xff00) >> 8;
		  i2cTxBuffer[17] = (Motor1_SpeedMin & 0x00ff);
		  i2cTxBuffer[18] = (Motor1_SpeedMin & 0xff00) >> 8;
		  i2cTxBuffer[19] = (Motor1_CurrentMax & 0x00ff);
		  i2cTxBuffer[20] = (Motor1_CurrentMax & 0xff00) >> 8;
		  i2cTxBuffer[21] = (Motor1_PotZero & 0x00ff);
		  i2cTxBuffer[22] = (Motor1_PotZero & 0xff00) >> 8;
		  // motor2 properties
		  i2cTxBuffer[23] = (byte)Motor2_ControlMode;
		  i2cTxBuffer[24] = (Motor2_KP & 0x000000ff);
		  i2cTxBuffer[25] = (Motor2_KP & 0x0000ff00) >> 8;
		  i2cTxBuffer[26] = (Motor2_KP & 0x00ff0000) >> 16;
		  i2cTxBuffer[27] = (Motor2_KP & 0xff000000) >> 24;
		  i2cTxBuffer[28] = (Motor2_Setpoint & 0x000000ff);
		  i2cTxBuffer[29] = (Motor2_Setpoint & 0x0000ff00) >> 8;
		  i2cTxBuffer[30] = (Motor2_Setpoint & 0x00ff0000) >> 16;
		  i2cTxBuffer[31] = (Motor2_Setpoint & 0xff000000) >> 24;
		  i2cTxBuffer[32] = (Motor2_ShaftCounter & 0x000000ff);
		  i2cTxBuffer[33] = (Motor2_ShaftCounter & 0x0000ff00) >> 8;
		  i2cTxBuffer[34] = (Motor2_ShaftCounter & 0x00ff0000) >> 16;
		  i2cTxBuffer[35] = (Motor2_ShaftCounter & 0xff000000) >> 24;
		  i2cTxBuffer[36] = (Motor2_ClicksPerRev & 0x00ff);
		  i2cTxBuffer[37] = (Motor2_ClicksPerRev & 0xff00) >> 8;
		  i2cTxBuffer[38] = (Motor2_SpeedMin & 0x00ff);
		  i2cTxBuffer[39] = (Motor2_SpeedMin & 0xff00) >> 8;
		  i2cTxBuffer[40] = (Motor2_CurrentMax & 0x00ff);
		  i2cTxBuffer[41] = (Motor2_CurrentMax & 0xff00) >> 8;
		  i2cTxBuffer[42] = (Motor2_PotZero & 0x00ff);
		  i2cTxBuffer[43] = (Motor2_PotZero & 0xff00) >> 8;
		  // controller properties
		  i2cTxBuffer[44] = (byte)GetHalls;
		  i2cTxBuffer[45] = (byte)GetPots;
		  i2cTxBuffer[46] = (byte)GetCurrent;

		  I2C_SendBlock(&i2cTxBuffer[0], 47, &readWriteSize);
		  ComSuccess = FALSE;
		  saveLoopCounter = 0;
	  }

//	  while(readWriteSize < 40);

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

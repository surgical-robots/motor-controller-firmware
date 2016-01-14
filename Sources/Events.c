/* ###################################################################
**     Filename    : Events.c
**     Project     : BLDC_KV10
**     Processor   : MKV10Z32VFM7
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-10-16, 16:21, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
#include "motor.h"
#include "Command.h"
#include "string.h"
#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MKV10Z32LF7]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SlowLoop_OnInterrupt (module Events)
**
**     Component   :  SlowLoop [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SlowLoop_OnInterrupt(void)
{
	timerFlag = 1;
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  UART_OnRxChar (module Events)
**
**     Component   :  UART [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void UART_OnRxChar(void)
{
	char character;
	UART_RecvChar(&character);
	Command_CharacterReceived(character);
//	switch(character)
//	{
//	case '1':
//		Motor_M2_Setpoint(1000);
//		break;
//	case '2':
//		Motor_M2_Setpoint(2000);
//		break;
//	case '3':
//		Motor_M2_Setpoint(3000);
//		break;
//	case '8':
//		Motor_M1_Setpoint(1000);
//		break;
//	case '9':
//		Motor_M1_Setpoint(2000);
//		break;
//	case '0':
//		Motor_M1_Setpoint(3000);
//
//
//	}
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  UART_OnTxChar (module Events)
**
**     Component   :  UART [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void UART_OnTxChar(void)
{
	Command_ResponseTask();
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  FastLoop_OnInterrupt (module Events)
**
**     Component   :  FastLoop [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void FastLoop_OnInterrupt(void)
{
  /* Write your code here ... */
	Motor_Update();
}

/*
** ===================================================================
**     Event       :  M1_ANALOG_OnEnd (module Events)
**
**     Component   :  M1_ANALOG [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M1_ANALOG_OnEnd(void)
{
	//uint16_t values[10];
	//memset(values, 0, 20);
  /* Write your code here ... */
	//M1_ANALOG_GetValue(values);
	M1_ANALOG_GetChanValue16(0, &Motor1_PotVal);
	M1_ANALOG_GetChanValue16(1, &Motor1_Current);
}

/*
** ===================================================================
**     Event       :  M1_ANALOG_OnCalibrationEnd (module Events)
**
**     Component   :  M1_ANALOG [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M1_ANALOG_OnCalibrationEnd(void)
{
  /* Write your code here ... */
	  M1_ANALOG_Start();
}

/*
** ===================================================================
**     Event       :  M2_ANALOG_OnEnd (module Events)
**
**     Component   :  M2_ANALOG [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M2_ANALOG_OnEnd(void)
{
  /* Write your code here ... */
	M2_ANALOG_GetChanValue16(0, &Motor2_PotVal);
	M2_ANALOG_GetChanValue16(1, &Motor2_Current);
}

/*
** ===================================================================
**     Event       :  M2_ANALOG_OnCalibrationEnd (module Events)
**
**     Component   :  M2_ANALOG [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M2_ANALOG_OnCalibrationEnd(void)
{
  /* Write your code here ... */
	  M2_ANALOG_Start();
}

/*
** ===================================================================
**     Event       :  M2_HALL1_OnInterrupt (module Events)
**
**     Component   :  M2_HALL1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M2_HALL1_OnInterrupt(void)
{
  /* Write your code here ... */
	Motor_Motor2_Tach();
}

/*
** ===================================================================
**     Event       :  M1_HALL1_OnInterrupt (module Events)
**
**     Component   :  M1_HALL1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void M1_HALL1_OnInterrupt(void)
{
  /* Write your code here ... */
	Motor_Motor1_Tach();
}

/*
** ===================================================================
**     Event       :  UART_OnError (module Events)
**
**     Component   :  UART [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void UART_OnError(void)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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

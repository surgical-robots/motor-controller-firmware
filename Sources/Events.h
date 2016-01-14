/* ###################################################################
**     Filename    : Events.h
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
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
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

#ifdef __cplusplus
extern "C" {
#endif 

uint_least8_t timerFlag;

// UART stuff
uint8 cmdBuffer[32];
uint_least8_t cmdBufferIdx;
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
void Cpu_OnNMI(void);


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
void SlowLoop_OnInterrupt(void);

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
void UART_OnRxChar(void);

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
void UART_OnTxChar(void);

void M2_TACHO_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  M2_TACHO_OnInterrupt (module Events)
**
**     Component   :  M2_TACHO [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void M1_ANALOG_TACHO_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  M1_ANALOG_TACHO_OnInterrupt (module Events)
**
**     Component   :  M1_ANALOG_TACHO [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

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
void FastLoop_OnInterrupt(void);

void POT1_OnEnd(void);
/*
** ===================================================================
**     Event       :  POT1_OnEnd (module Events)
**
**     Component   :  POT1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void POT2_OnEnd(void);
/*
** ===================================================================
**     Event       :  POT2_OnEnd (module Events)
**
**     Component   :  POT2 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void M1_ANALOG_OnEnd(void);
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

void M1_ANALOG_OnCalibrationEnd(void);
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

void M2_ANALOG_OnEnd(void);
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

void M2_ANALOG_OnCalibrationEnd(void);
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

void M2_HALL1_OnInterrupt(void);
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

void M1_HALL1_OnInterrupt(void);
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
void UART_OnError(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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

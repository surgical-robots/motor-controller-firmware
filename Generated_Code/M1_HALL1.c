/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : M1_HALL1.c
**     Project     : BLDC_KV10
**     Processor   : MKV10Z32VFM7
**     Component   : ExtInt
**     Version     : Component 02.105, Driver 01.00, CPU db: 3.50.001
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-29, 12:54, # CodeGen: 80
**     Abstract    :
**         This component "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The component uses one pin which generates interrupt on 
**         selected edge.
**     Settings    :
**         Interrupt name              : INT_PORTA
**         User handling procedure     : M1_HALL1_OnInterrupt
**
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       18            |  XTAL0/PTA19/FTM0_FLT0/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1
**             ----------------------------------------------------
**
**         Port name                   : GPIOA
**
**         Bit number (in port)        : 19
**         Bit mask of the port        : 0x00080000
**
**         Signal edge/level           : both
**         Priority                    : 0
**         Pull option                 : off
**         Initial state               : Enabled
**
**
**         Port data register          : GPIOA_PDOR [0x400FF000]
**         Port control register       : GPIOA_PDDR [0x400FF014]
**     Contents    :
**         GetVal - bool M1_HALL1_GetVal(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file M1_HALL1.c
** @version 01.00
** @brief
**         This component "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The component uses one pin which generates interrupt on 
**         selected edge.
*/         
/*!
**  @addtogroup M1_HALL1_module M1_HALL1 module documentation
**  @{
*/         

/* MODULE M1_HALL1. */

#include "Events.h"
#include "M1_HALL1.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Method      :  M1_HALL1_GetVal (component ExtInt)
**     Description :
**         Returns the actual value of the input pin of the component.
**     Parameters  : None
**     Returns     :
**         ---             - Returned input value. Possible values:
**                           <false> - logical "0" (Low level) <true> -
**                           logical "1" (High level)
** ===================================================================
*/
/*
bool M1_HALL1_GetVal(void)

**  This method is implemented as a macro. See M1_HALL1.h file.  **
*/

/*
** ===================================================================
**     Method      :  M1_HALL1_OnInterrupt (component ExtInt)
**
**     Description :
**         This event is called when an active signal edge/level has 
**         occurred. The event services the event of the inherited 
**         component and eventually invokes other events.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void ExtIntLdd1_OnInterrupt(LDD_TUserData *UserDataPtr)
{
  (void)UserDataPtr;                   /* Parameter is not used, suppress unused argument warning */
  M1_HALL1_OnInterrupt();              /* Invoke OnInterrupt event */
}

/* END M1_HALL1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

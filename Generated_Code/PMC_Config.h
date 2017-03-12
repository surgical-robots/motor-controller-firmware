/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PMC_Config.h
**     Project     : motor-controller-firmware
**     Processor   : MKV10Z32VFM7
**     Component   : Init_PMC
**     Version     : Component 01.001, Driver 01.01, CPU db: 3.50.001
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-19, 12:47, # CodeGen: 26
**     Abstract    :
**          This file implements the PMC (PMC) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : PMC
**          Device                                         : PMC
**          Settings                                       : 
**            LVD reset                                    : Enabled
**            LVD voltage                                  : High
**            LVW voltage                                  : Low
**            Bandgap buffer                               : Disabled
**            Bandgap buffer in VLPx                       : Disabled
**          Interrupts                                     : 
**            Low-voltage                                  : 
**              Interrupt                                  : INT_PMC
**              Interrupt request                          : disabled
**              Interrupt priority                         : 0
**              ISR Name                                   : 
**              Low voltage detect                         : Disabled
**              Low voltage warning                        : Disabled
**          Initialization                                 : 
**            Call Init method                             : yes
**            Utilize after reset values                   : default
**     Contents    :
**         Init - void PMC_Init(void);
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
** @file PMC_Config.h                                                  
** @version 01.01
** @brief
**          This file implements the PMC (PMC) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup PMC_Config_module PMC_Config module documentation
**  @{
*/         

#ifndef PMC_Config_H_
#define PMC_Config_H_

/* MODULE PMC. */

/* PMC_LVDSC1: LVDF=0,LVDACK=1,LVDIE=0,LVDRE=1,??=0,??=0,LVDV=1 */
#define PMC_LVDSC1_VALUE     0x51
/* PMC_LVDSC2: LVWF=0,LVWACK=1,LVWIE=0,??=0,??=0,??=0,LVWV=0 */
#define PMC_LVDSC2_VALUE     0x40
/* PMC_REGSC: ??=0,??=0,??=0,BGEN=0,ACKISO=0,REGONS=1,??=0,BGBE=0 */
#define PMC_REGSC_VALUE      0x04

#define PMC_AUTOINIT

/* END PMC. */
#endif /* #ifndef __PMC_Config_H_ */
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

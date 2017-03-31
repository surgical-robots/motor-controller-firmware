

/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CRC1.h
**     Project     : BLDC_KV10
**     Processor   : MKV10Z32VFM7
**     Component   : CRC_LDD
**     Version     : Component 01.014, Driver 01.07, CPU db: 3.50.001
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-29, 12:54, # CodeGen: 80
**     Abstract    :
**         This component implements the Cyclic Redundancy Check (CRC) code calculation.
**     Settings    :
**          Component name                                 : CRC1
**          Device                                         : CRC
**          Settings                                       : 
**            CRC standard                                 : CRC16
**          Initialization                                 : 
**            Auto initialization                          : no
**     Contents    :
**         Init         - LDD_TDeviceData* CRC1_Init(LDD_TUserData *UserDataPtr);
**         GetCRCResult - uint32_t CRC1_GetCRCResult(LDD_TDeviceData *DeviceDataPtr);
**         GetCRC8      - uint32_t CRC1_GetCRC8(LDD_TDeviceData *DeviceDataPtr, uint8_t Data);
**         GetBlockCRC  - LDD_TError CRC1_GetBlockCRC(LDD_TDeviceData *DeviceDataPtr, LDD_TData* Ptr,...
**         ResetCRC     - void CRC1_ResetCRC(LDD_TDeviceData *DeviceDataPtr);
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
** @file CRC1.h
** @version 01.07
** @brief
**         This component implements the Cyclic Redundancy Check (CRC) code calculation.
*/         
/*!
**  @addtogroup CRC1_module CRC1 module documentation
**  @{
*/         


#ifndef __CRC1_H
#define __CRC1_H

/* MODULE CRC1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "CRC_PDD.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 


typedef struct {
  LDD_TUserData *UserDataPtr;          /* Pointer to user data */
  LDD_CRC_TCRCStandard CRCStandard;    /* Current CRC standard configuration */
  bool CRC32bit;                       /* Current CRC width */
  uint16_t SeedHigh;                   /* 16 most significant seed bits */
  uint16_t SeedLow;                    /* 16 least significant seed bits */
} CRC1_TDeviceData;

typedef CRC1_TDeviceData *CRC1_TDeviceDataPtr; /* Pointer to the device data structure. */

/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define CRC1_PRPH_BASE_ADDRESS  0x40032000U
  
/* Methods configuration constants - generated for all enabled component's methods */
#define CRC1_Init_METHOD_ENABLED       /*!< Init method of the component CRC1 is enabled (generated) */
#define CRC1_GetCRCResult_METHOD_ENABLED /*!< GetCRCResult method of the component CRC1 is enabled (generated) */
#define CRC1_GetCRC8_METHOD_ENABLED    /*!< GetCRC8 method of the component CRC1 is enabled (generated) */
#define CRC1_GetBlockCRC_METHOD_ENABLED /*!< GetBlockCRC method of the component CRC1 is enabled (generated) */
#define CRC1_ResetCRC_METHOD_ENABLED   /*!< ResetCRC method of the component CRC1 is enabled (generated) */


/*
** ===================================================================
**     Method      :  CRC1_Init (component CRC_LDD)
*/
/*!
**     @brief
**         Initializes the device and allocates memory for the private
**         device data structure.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* CRC1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  CRC1_ResetCRC (component CRC_LDD)
*/
/*!
**     @brief
**         Reset CRC module for new CRC computation. This method keeps
**         previous settings (CRC standard, seed value...).
**     @param
**         DeviceDataPtr   - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
*/
/* ===================================================================*/
void CRC1_ResetCRC(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  CRC1_GetCRC8 (component CRC_LDD)
*/
/*!
**     @brief
**         This method appends 1 data byte to current CRC calculation
**         and returns new result. It automatically enables code
**         generation of GetCRCResult method.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by Init method. 
**     @param
**         Data            - New data value for actual CRC
**                           computation.
**     @return
**                         - Returns current CRC result. 
*/
/* ===================================================================*/
uint32_t CRC1_GetCRC8(LDD_TDeviceData *DeviceDataPtr, uint8_t Data);

/*
** ===================================================================
**     Method      :  CRC1_GetBlockCRC (component CRC_LDD)
*/
/*!
**     @brief
**         Gets CRC of data block. This method receives a pointer to a
**         memory location and calculates the CRC of the whole memory
**         block. It automatically enables code generation of
**         GetCRCResult method.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by Init method. 
**     @param
**         Ptr             - Pointer to data block in memory which will
**                           be CRC checksummed.
**     @param
**         Length          - Length of buffer in bytes.
**     @param
**         Result          - Pointer to variable where the result
**                           will be stored. 
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_RANGE - NULL pointer passed or length
**                           is 0.
*/
/* ===================================================================*/
LDD_TError CRC1_GetBlockCRC(LDD_TDeviceData *DeviceDataPtr, LDD_TData* Ptr, uint32_t Length, uint32_t *Result);

/*
** ===================================================================
**     Method      :  CRC1_GetCRCResult (component CRC_LDD)
*/
/*!
**     @brief
**         Returns actual CRC result. This method cannot be disabled if
**         at least one of the following methods is enabled: GetCRC8,
**         GetCRC16, GetCRC32, GetBlockCRC.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by Init method. 
**     @return
**                         - Returns current CRC result. 
*/
/* ===================================================================*/
uint32_t CRC1_GetCRCResult(LDD_TDeviceData *DeviceDataPtr);

/* END CRC1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __CRC1_H */
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

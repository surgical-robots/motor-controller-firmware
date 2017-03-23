/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : M1_ANALOG.c
**     Project     : motor-controller-firmware
**     Processor   : MKV10Z32VFM7
**     Component   : ADC
**     Version     : Component 01.697, Driver 01.00, CPU db: 3.50.001
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-23, 14:11, # CodeGen: 42
**     Abstract    :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings    :
**          Component name                                 : M1_ANALOG
**          A/D converter                                  : ADC1
**          Sharing                                        : Disabled
**          ADC_LDD                                        : ADC_LDD
**          Interrupt service/event                        : Enabled
**            A/D interrupt                                : INT_ADC1
**            A/D interrupt priority                       : low priority
**          A/D channels                                   : 1
**            Channel0                                     : 
**              A/D channel (pin)                          : M1_ADC
**              A/D channel (pin) signal                   : 
**              Mode select                                : Single Ended
**          A/D resolution                                 : 12 bits
**          Conversion time                                : 20 �s
**          Low-power mode                                 : Disabled
**          High-speed conversion mode                     : Disabled
**          Asynchro clock output                          : Disabled
**          Sample time                                    : 20 = long
**          Internal trigger                               : Disabled
**          Number of conversions                          : 8
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Events enabled in init.                      : yes
**          CPU clock/speed selection                      : 
**            High speed mode                              : This component enabled
**            Low speed mode                               : This component disabled
**            Slow speed mode                              : This component disabled
**          High input limit                               : 1
**          Low input limit                                : 0
**          Get value directly                             : yes
**          Wait for result                                : yes
**     Contents    :
**         Enable         - byte M1_ANALOG_Enable(void);
**         Disable        - byte M1_ANALOG_Disable(void);
**         Start          - byte M1_ANALOG_Start(void);
**         Measure        - byte M1_ANALOG_Measure(bool WaitForResult);
**         MeasureChan    - byte M1_ANALOG_MeasureChan(bool WaitForResult, byte Channel);
**         GetValue       - byte M1_ANALOG_GetValue(void* Values);
**         GetChanValue   - byte M1_ANALOG_GetChanValue(byte Channel, void* Value);
**         GetValue16     - byte M1_ANALOG_GetValue16(word *Values);
**         GetChanValue16 - byte M1_ANALOG_GetChanValue16(byte Channel, word *Value);
**         Calibrate      - byte M1_ANALOG_Calibrate(bool WaitForResult);
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
** @file M1_ANALOG.c
** @version 01.00
** @brief
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
*/         
/*!
**  @addtogroup M1_ANALOG_module M1_ANALOG module documentation
**  @{
*/         

/* MODULE M1_ANALOG. */

#include "Events.h"
#include "M1_ANALOG.h"

#ifdef __cplusplus
extern "C" {
#endif 
#define STOP            0x00U          /* STOP state           */
#define MEASURE         0x01U          /* MESURE state         */
#define CONTINUOUS      0x02U          /* CONTINUOUS state     */
#define SINGLE          0x03U          /* SINGLE state         */
#define CALIBRATING     0x04U          /* CALIBRATING state    */

static bool EnUser;                    /* Enable/Disable device */
static volatile byte ModeFlg;          /* Current state of device */
static volatile byte SumCnt;           /* Counter of conversions */
LDD_TDeviceData *AdcLdd2_DeviceDataPtr; /* Device data pointer */
/* Sample group configuration */
static LDD_ADC_TSample SampleGroup[M1_ANALOG_SAMPLE_GROUP_SIZE];
/* Temporary buffer for converting results */
volatile word M1_ANALOG_SumV;          /* Temporary sum of measured values */
volatile word M1_ANALOG_OutV;          /* Sum of measured values */
/* Calibration in progress flag */
static volatile bool OutFlg;           /* Measurement finish flag */

/*
** ===================================================================
**     Method      :  M1_ANALOG_HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void M1_ANALOG_HWEnDi(void)
{
  if (EnUser) {                        /* Enable device? */
    (void)AdcLdd2_Enable(AdcLdd2_DeviceDataPtr); /* Enable device */
    if (ModeFlg) {                     /* Start or stop measurement? */
      SumCnt = 0U;                     /* Set counter of conversions to 0 */
      OutFlg = FALSE;                  /* Output value isn't available */
      M1_ANALOG_SumV = 0U;             /* Set variable for storing measured values to 0 */
      SampleGroup[0].ChannelIdx = 0U;
      (void)AdcLdd2_CreateSampleGroup(AdcLdd2_DeviceDataPtr, (LDD_ADC_TSample *)SampleGroup, 1U); /* Configure sample group */
      (void)AdcLdd2_StartSingleMeasurement(AdcLdd2_DeviceDataPtr);
    }
  }
  else {
    (void)AdcLdd2_Disable(AdcLdd2_DeviceDataPtr); /* Disable device */
  }
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Enable (component ADC)
*/
/*!
**     @brief
**         Enables A/D converter component. [Events] may be generated
**         ([DisableEvent]/[EnableEvent]). If possible, this method
**         switches on A/D converter device, voltage reference, etc.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/
byte M1_ANALOG_Enable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    return ERR_OK;                     /* If yes then set the flag "device enabled" */
  }
  EnUser = TRUE;                       /* Set the flag "device enabled" */
  M1_ANALOG_HWEnDi();                  /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Disable (component ADC)
*/
/*!
**     @brief
**         Disables A/D converter component. No [events] will be
**         generated. If possible, this method switches off A/D
**         converter device, voltage reference, etc.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
*/
/* ===================================================================*/
byte M1_ANALOG_Disable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_OK;                     /* If yes then OK */
  }
  EnUser = FALSE;                      /* If yes then set the flag "device disabled" */
  M1_ANALOG_HWEnDi();                  /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Start (component ADC)
*/
/*!
**     @brief
**         This method starts continuous conversion on all channels
**         that are set in the component inspector. When each
**         measurement on all channels has finished the [OnEnd ] event
**         may be invoked. This method is not available if the
**         [interrupt service] is disabled and the device doesn't
**         support the continuous mode. Note: If time of measurement is
**         too short and the instruction clock is too slow then the
**         conversion complete interrupt and its handler may cause a
**         system overflow.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
byte M1_ANALOG_Start(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in running mode? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = CONTINUOUS;                /* Set state of device to the continuous mode */
  M1_ANALOG_HWEnDi();                  /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Measure (component ADC)
*/
/*!
**     @brief
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the [number of
**         conversions] is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of a
**                           conversion. If [interrupt service] is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the [number of channels] is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the [interrupt
**                           service] is disabled and a [number of
**                           conversions] is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
byte M1_ANALOG_Measure(bool WaitForResult)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = MEASURE;                   /* Set state of device to the measure mode */
  M1_ANALOG_HWEnDi();                  /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == MEASURE) {}      /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_MeasureChan (component ADC)
*/
/*!
**     @brief
**         This method performs measurement on one channel. (Note: If
**         the [number of conversions] is more than one the conversion
**         of the A/D channel is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of
**                           conversion. If the [interrupt service] is
**                           disabled and at the same time a [number of
**                           conversions] is greater than 1, the
**                           WaitForResult parameter is ignored and the
**                           method waits for each result every time.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set this
**                           parameter is ignored, because the parameter
**                           is set inside this method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
**                           ERR_RANGE - Parameter "Channel" out of range
*/
/* ===================================================================*/
/*
The definition of method:
byte M1_ANALOG_MeasureChan(bool WaitForResult, byte Channel)
was optimised based on the current component setting. An appropriate macro has been defined 
in the AD1.h to maintain API compatibility.
*/
byte PE_M1_ANALOG_MeasureChan(bool WaitForResult)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = SINGLE;                    /* Set state of device to the measure mode */
  M1_ANALOG_HWEnDi();                  /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == SINGLE) {}       /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_GetValue (component ADC)
*/
/*!
**     @brief
**         Returns the last measured values for all channels. Format
**         and width of the value is a native format of the A/D
**         converter.
**     @param
**         Values          - Pointer to the array that contains
**                           the measured data. Data type is a byte, a
**                           word or an int. It depends on the supported
**                           modes, resolution, etc. of the AD converter.
**                           See the Version specific information for
**                           the current CPU in [General Info].
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte M1_ANALOG_GetValue(void* Values)
{
  if (!OutFlg) {                       /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  /* Copy measured values */
  *(AdcLdd2_TResultData *)Values = (word)(M1_ANALOG_OutV / 8U);
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_GetChanValue (component ADC)
*/
/*!
**     @brief
**         Returns the last measured value of the required channel.
**         Format and width of the value is a native format of the A/D
**         converter.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set then this
**                           parameter is ignored.
**     @param
**         Value           - Pointer to the measured value. Data
**                           type is a byte, a word or an int. It
**                           depends on the supported modes, resolution,
**                           etc. of the AD converter. See the Version
**                           specific information for the current CPU in
**                           [General Info].
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Parameter "Channel" out of
**                           range
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
/*
The definition of method:
byte M1_ANALOG_GetChanValue(byte Channel, void* Value)
was optimised based on the current component setting. An appropriate macro has been defined 
in the AD1.h to maintain API compatibility.
*/
byte PE_M1_ANALOG_GetChanValue(void* Value)
{
  if (!OutFlg) {                       /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  /* Copy value from temporary buffer */
  *(AdcLdd2_TResultData *)Value = (word)(M1_ANALOG_OutV / 8U);
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_GetValue16 (component ADC)
*/
/*!
**     @brief
**         This method returns the last measured values of all channels.
**         Compared with [GetValue] method this method returns more
**         accurate result if the [number of conversions] is greater
**         than 1 and [AD resolution] is less than 16 bits. In addition,
**         the user code dependency on [AD resolution] is eliminated.
**     @param
**         Values          - Pointer to the array that contains
**                           the measured data.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte M1_ANALOG_GetValue16(word *Values)
{
  if (!OutFlg) {                       /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Values = (word)((M1_ANALOG_OutV) << 1U); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_GetChanValue16 (component ADC)
*/
/*!
**     @brief
**         This method returns the last measured value of the required
**         channel. Compared with [GetChanValue] method this method
**         returns more accurate result if the [number of conversions]
**         is greater than 1 and [AD resolution] is less than 16 bits.
**         In addition, the user code dependency on [AD resolution] is
**         eliminated.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set then this
**                           parameter is ignored.
**     @param
**         Value           - Pointer to the measured value.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Parameter "Channel" out of
**                           range
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte PE_M1_ANALOG_GetChanValue16(word *Value)
{
  if (!OutFlg) {                       /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Value = (word)((M1_ANALOG_OutV) << 1U); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Calibrate (component ADC)
**     Description :
**         This method starts self calibration process. Calibration is
**         typically used to remove the effects of the gain and offset
**         from a specific reading.
**     Parameters  :
**         NAME            - DESCRIPTION
**         WaitForResult   - Wait for a result of
**                           calibration. If the <interrupt service> is
**                           disabled, the WaitForResult parameter is
**                           ignored and the method waits for
**                           calibration result every time.
**     Returns     :
**         ---             - Error code
**                           ERR_OK - OK
**                           ERR_BUSY - A conversion is already running
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_FAILED - Calibration hasn't been
**                           finished correctly
** ===================================================================
*/
byte M1_ANALOG_Calibrate(bool WaitForResult)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = CALIBRATING;               /* Set state of device to the calibration mode */
  (void)AdcLdd2_GetMeasurementCompleteStatus(AdcLdd2_DeviceDataPtr); /* Clear measurement complete status */
  (void)AdcLdd2_StartCalibration(AdcLdd2_DeviceDataPtr); /* Start calibration */
  if (!WaitForResult) {                /* If doesn't wait for result */
    return ERR_OK;                     /* then return ERR_OK, but user have to check the result of calibration e.g. by GetCalibrationStatus method */
  }
  while (!AdcLdd2_GetMeasurementCompleteStatus(AdcLdd2_DeviceDataPtr)) {}; /* Wait until calibration ends */
  if (AdcLdd2_GetCalibrationResultStatus(AdcLdd2_DeviceDataPtr) != ERR_OK) { /* If calibration failed flag is set */
    ModeFlg = STOP;                    /* Set the device to the stop mode */
    return ERR_FAILED;                 /* Return ERR_FAILED error code */
  }
  return ERR_OK;                       /* ADC device is now calibrated */
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_OnMeasurementComplete (component ADC)
**
**     Description :
**         The method services the conversion complete interrupt of the 
**         selected peripheral(s) and eventually invokes the beans 
**         event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AdcLdd2_OnMeasurementComplete(LDD_TUserData *UserDataPtr)
{
  word ResultData;                     /* Temporary result data */
    
  (void)UserDataPtr;                   /* Parameter is not used, suppress unused argument warning */
  if (ModeFlg == CALIBRATING) {        /* If the driver is in CALIBRATING mode */
    (void)AdcLdd2_GetCalibrationResultStatus(AdcLdd2_DeviceDataPtr);
    ModeFlg = STOP;                    /* Set the device to the stop mode */
    M1_ANALOG_OnCalibrationEnd();      /* If yes then invoke user event */
    return;                            /* Return from interrupt */
  }
  if (ModeFlg != SINGLE) {
    AdcLdd2_GetMeasuredValues(AdcLdd2_DeviceDataPtr, (LDD_TData *)&ResultData);
    M1_ANALOG_SumV += ResultData;
    SumCnt++;                          /* Increase counter of conversions*/
    if (SumCnt == 8U) {                /* Is number of conversions on each channel equal to the number of conversions defined in the component? */
      OutFlg = TRUE;                   /* Measured value is available */
      M1_ANALOG_OutV = M1_ANALOG_SumV; /* Save measured value to the output buffer */
      if (ModeFlg != MEASURE) {        /* Is the device in other then measure state? */
        M1_ANALOG_SumV = 0U;           /* Set mesured values to 0 */
        SumCnt = 0U;                   /* Set counter of conversions to 0 */
      }
      M1_ANALOG_OnEnd();               /* If yes then invoke user event */
      if (ModeFlg == MEASURE) {        /* Is the device in the measure state? */
        ModeFlg = STOP;                /* Set the device to the stop mode */
        return;                        /* Return from interrupt */
      }
    }
    SampleGroup[0].ChannelIdx = 0U;    /* Restart measurement */
    (void)AdcLdd2_CreateSampleGroup(AdcLdd2_DeviceDataPtr, (LDD_ADC_TSample *)SampleGroup, 1U); /* Configure sample group */
    (void)AdcLdd2_StartSingleMeasurement(AdcLdd2_DeviceDataPtr);
  }
  else {
    AdcLdd2_GetMeasuredValues(AdcLdd2_DeviceDataPtr, (LDD_TData *)&ResultData);
    M1_ANALOG_SumV += ResultData;
    SumCnt++;                          /* Increase counter of conversions*/
    if (SumCnt == 8U) {                /* Is number of conversions on each channel equal to the number of conversions defined in the component? */
      OutFlg = TRUE;                   /* Measured value is available */
      M1_ANALOG_OutV = M1_ANALOG_SumV; /* Save measured value to the output buffer */
      M1_ANALOG_OnEnd();               /* If yes then invoke user event */
      ModeFlg = STOP;                  /* Set the device to the stop mode */
      return;                          /* Return from interrupt */
    }
    SampleGroup[0].ChannelIdx = 0U;    /* Restart measurement */
    (void)AdcLdd2_CreateSampleGroup(AdcLdd2_DeviceDataPtr, (LDD_ADC_TSample *)SampleGroup, 1U); /* Configure sample group */
    (void)AdcLdd2_StartSingleMeasurement(AdcLdd2_DeviceDataPtr);
  }
}

/*
** ===================================================================
**     Method      :  M1_ANALOG_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void M1_ANALOG_Init(void)
{
  EnUser = TRUE;                       /* Enable device */
  OutFlg = FALSE;                      /* No measured value */
  ModeFlg = STOP;                      /* Device isn't running */
  AdcLdd2_DeviceDataPtr = AdcLdd2_Init(NULL); /* Calling init method of the inherited component */
}

/* END M1_ANALOG. */

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

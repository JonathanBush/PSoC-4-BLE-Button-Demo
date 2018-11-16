/*******************************************************************************
* File Name: WireClock.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "WireClock.h"

/* Error message for removed <resource> through optimization */
#ifdef WireClock_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* WireClock_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 WireClock_initVar = 0u;


/*******************************************************************************
* Function Name: WireClock_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireClock_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void WireClock_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(WireClock_initVar == 0u)
    {
        WireClock_Init();
        WireClock_initVar = 1u;
    }
    WireClock_Enable();

}


/*******************************************************************************
* Function Name: WireClock_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  WireClock_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void WireClock_Init(void) 
{
    #if (WireClock_UsingFixedFunction || WireClock_UseControl)
        uint8 ctrl;
    #endif /* (WireClock_UsingFixedFunction || WireClock_UseControl) */

    #if(!WireClock_UsingFixedFunction)
        #if(WireClock_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 WireClock_interruptState;
        #endif /* (WireClock_UseStatus) */
    #endif /* (!WireClock_UsingFixedFunction) */

    #if (WireClock_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        WireClock_CONTROL |= WireClock_CFG0_MODE;
        #if (WireClock_DeadBand2_4)
            WireClock_CONTROL |= WireClock_CFG0_DB;
        #endif /* (WireClock_DeadBand2_4) */

        ctrl = WireClock_CONTROL3 & ((uint8 )(~WireClock_CTRL_CMPMODE1_MASK));
        WireClock_CONTROL3 = ctrl | WireClock_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        WireClock_RT1 &= ((uint8)(~WireClock_RT1_MASK));
        WireClock_RT1 |= WireClock_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        WireClock_RT1 &= ((uint8)(~WireClock_SYNCDSI_MASK));
        WireClock_RT1 |= WireClock_SYNCDSI_EN;

    #elif (WireClock_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = WireClock_CONTROL & ((uint8)(~WireClock_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~WireClock_CTRL_CMPMODE1_MASK));
        WireClock_CONTROL = ctrl | WireClock_DEFAULT_COMPARE2_MODE |
                                   WireClock_DEFAULT_COMPARE1_MODE;
    #endif /* (WireClock_UsingFixedFunction) */

    #if (!WireClock_UsingFixedFunction)
        #if (WireClock_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            WireClock_AUX_CONTROLDP0 |= (WireClock_AUX_CTRL_FIFO0_CLR);
        #else /* (WireClock_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            WireClock_AUX_CONTROLDP0 |= (WireClock_AUX_CTRL_FIFO0_CLR);
            WireClock_AUX_CONTROLDP1 |= (WireClock_AUX_CTRL_FIFO0_CLR);
        #endif /* (WireClock_Resolution == 8) */

        WireClock_WriteCounter(WireClock_INIT_PERIOD_VALUE);
    #endif /* (!WireClock_UsingFixedFunction) */

    WireClock_WritePeriod(WireClock_INIT_PERIOD_VALUE);

        #if (WireClock_UseOneCompareMode)
            WireClock_WriteCompare(WireClock_INIT_COMPARE_VALUE1);
        #else
            WireClock_WriteCompare1(WireClock_INIT_COMPARE_VALUE1);
            WireClock_WriteCompare2(WireClock_INIT_COMPARE_VALUE2);
        #endif /* (WireClock_UseOneCompareMode) */

        #if (WireClock_KillModeMinTime)
            WireClock_WriteKillTime(WireClock_MinimumKillTime);
        #endif /* (WireClock_KillModeMinTime) */

        #if (WireClock_DeadBandUsed)
            WireClock_WriteDeadTime(WireClock_INIT_DEAD_TIME);
        #endif /* (WireClock_DeadBandUsed) */

    #if (WireClock_UseStatus || WireClock_UsingFixedFunction)
        WireClock_SetInterruptMode(WireClock_INIT_INTERRUPTS_MODE);
    #endif /* (WireClock_UseStatus || WireClock_UsingFixedFunction) */

    #if (WireClock_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        WireClock_GLOBAL_ENABLE |= WireClock_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        WireClock_CONTROL2 |= WireClock_CTRL2_IRQ_SEL;
    #else
        #if(WireClock_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            WireClock_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            WireClock_STATUS_AUX_CTRL |= WireClock_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(WireClock_interruptState);

            /* Clear the FIFO to enable the WireClock_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            WireClock_ClearFIFO();
        #endif /* (WireClock_UseStatus) */
    #endif /* (WireClock_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: WireClock_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void WireClock_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (WireClock_UsingFixedFunction)
        WireClock_GLOBAL_ENABLE |= WireClock_BLOCK_EN_MASK;
        WireClock_GLOBAL_STBY_ENABLE |= WireClock_BLOCK_STBY_EN_MASK;
    #endif /* (WireClock_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (WireClock_UseControl || WireClock_UsingFixedFunction)
        WireClock_CONTROL |= WireClock_CTRL_ENABLE;
    #endif /* (WireClock_UseControl || WireClock_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: WireClock_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void WireClock_Stop(void) 
{
    #if (WireClock_UseControl || WireClock_UsingFixedFunction)
        WireClock_CONTROL &= ((uint8)(~WireClock_CTRL_ENABLE));
    #endif /* (WireClock_UseControl || WireClock_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (WireClock_UsingFixedFunction)
        WireClock_GLOBAL_ENABLE &= ((uint8)(~WireClock_BLOCK_EN_MASK));
        WireClock_GLOBAL_STBY_ENABLE &= ((uint8)(~WireClock_BLOCK_STBY_EN_MASK));
    #endif /* (WireClock_UsingFixedFunction) */
}

#if (WireClock_UseOneCompareMode)
    #if (WireClock_CompareMode1SW)


        /*******************************************************************************
        * Function Name: WireClock_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void WireClock_SetCompareMode(uint8 comparemode) 
        {
            #if(WireClock_UsingFixedFunction)

                #if(0 != WireClock_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << WireClock_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != WireClock_CTRL_CMPMODE1_SHIFT) */

                WireClock_CONTROL3 &= ((uint8)(~WireClock_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                WireClock_CONTROL3 |= comparemodemasked;

            #elif (WireClock_UseControl)

                #if(0 != WireClock_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << WireClock_CTRL_CMPMODE1_SHIFT)) &
                                                WireClock_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & WireClock_CTRL_CMPMODE1_MASK;
                #endif /* (0 != WireClock_CTRL_CMPMODE1_SHIFT) */

                #if(0 != WireClock_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << WireClock_CTRL_CMPMODE2_SHIFT)) &
                                               WireClock_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & WireClock_CTRL_CMPMODE2_MASK;
                #endif /* (0 != WireClock_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                WireClock_CONTROL &= ((uint8)(~(WireClock_CTRL_CMPMODE1_MASK |
                                            WireClock_CTRL_CMPMODE2_MASK)));
                WireClock_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (WireClock_UsingFixedFunction) */
        }
    #endif /* WireClock_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (WireClock_CompareMode1SW)


        /*******************************************************************************
        * Function Name: WireClock_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void WireClock_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != WireClock_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << WireClock_CTRL_CMPMODE1_SHIFT)) &
                                           WireClock_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & WireClock_CTRL_CMPMODE1_MASK;
            #endif /* (0 != WireClock_CTRL_CMPMODE1_SHIFT) */

            #if (WireClock_UseControl)
                WireClock_CONTROL &= ((uint8)(~WireClock_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                WireClock_CONTROL |= comparemodemasked;
            #endif /* (WireClock_UseControl) */
        }
    #endif /* WireClock_CompareMode1SW */

#if (WireClock_CompareMode2SW)


    /*******************************************************************************
    * Function Name: WireClock_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != WireClock_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << WireClock_CTRL_CMPMODE2_SHIFT)) &
                                                 WireClock_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & WireClock_CTRL_CMPMODE2_MASK;
        #endif /* (0 != WireClock_CTRL_CMPMODE2_SHIFT) */

        #if (WireClock_UseControl)
            WireClock_CONTROL &= ((uint8)(~WireClock_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            WireClock_CONTROL |= comparemodemasked;
        #endif /* (WireClock_UseControl) */
    }
    #endif /*WireClock_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!WireClock_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: WireClock_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void WireClock_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(WireClock_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: WireClock_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 WireClock_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(WireClock_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(WireClock_CAPTURE_LSB_PTR));
    }

    #if (WireClock_UseStatus)


        /*******************************************************************************
        * Function Name: WireClock_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void WireClock_ClearFIFO(void) 
        {
            while(0u != (WireClock_ReadStatusRegister() & WireClock_STATUS_FIFONEMPTY))
            {
                (void)WireClock_ReadCapture();
            }
        }

    #endif /* WireClock_UseStatus */

#endif /* !WireClock_UsingFixedFunction */


/*******************************************************************************
* Function Name: WireClock_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void WireClock_WritePeriod(uint8 period) 
{
    #if(WireClock_UsingFixedFunction)
        CY_SET_REG16(WireClock_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(WireClock_PERIOD_LSB_PTR, period);
    #endif /* (WireClock_UsingFixedFunction) */
}

#if (WireClock_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: WireClock_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void WireClock_WriteCompare(uint8 compare) \
                                       
    {
        #if(WireClock_UsingFixedFunction)
            CY_SET_REG16(WireClock_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(WireClock_COMPARE1_LSB_PTR, compare);
        #endif /* (WireClock_UsingFixedFunction) */

        #if (WireClock_PWMMode == WireClock__B_PWM__DITHER)
            #if(WireClock_UsingFixedFunction)
                CY_SET_REG16(WireClock_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(WireClock_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (WireClock_UsingFixedFunction) */
        #endif /* (WireClock_PWMMode == WireClock__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: WireClock_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_WriteCompare1(uint8 compare) \
                                        
    {
        #if(WireClock_UsingFixedFunction)
            CY_SET_REG16(WireClock_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(WireClock_COMPARE1_LSB_PTR, compare);
        #endif /* (WireClock_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: WireClock_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_WriteCompare2(uint8 compare) \
                                        
    {
        #if(WireClock_UsingFixedFunction)
            CY_SET_REG16(WireClock_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(WireClock_COMPARE2_LSB_PTR, compare);
        #endif /* (WireClock_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (WireClock_DeadBandUsed)


    /*******************************************************************************
    * Function Name: WireClock_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!WireClock_DeadBand2_4)
            CY_SET_REG8(WireClock_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            WireClock_DEADBAND_COUNT &= ((uint8)(~WireClock_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(WireClock_DEADBAND_COUNT_SHIFT)
                WireClock_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << WireClock_DEADBAND_COUNT_SHIFT)) &
                                                    WireClock_DEADBAND_COUNT_MASK;
            #else
                WireClock_DEADBAND_COUNT |= deadtime & WireClock_DEADBAND_COUNT_MASK;
            #endif /* (WireClock_DEADBAND_COUNT_SHIFT) */

        #endif /* (!WireClock_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: WireClock_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 WireClock_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!WireClock_DeadBand2_4)
            return (CY_GET_REG8(WireClock_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(WireClock_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(WireClock_DEADBAND_COUNT & WireClock_DEADBAND_COUNT_MASK)) >>
                                                                           WireClock_DEADBAND_COUNT_SHIFT));
            #else
                return (WireClock_DEADBAND_COUNT & WireClock_DEADBAND_COUNT_MASK);
            #endif /* (WireClock_DEADBAND_COUNT_SHIFT) */
        #endif /* (!WireClock_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (WireClock_UseStatus || WireClock_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: WireClock_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(WireClock_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: WireClock_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 WireClock_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(WireClock_STATUS_PTR));
    }

#endif /* (WireClock_UseStatus || WireClock_UsingFixedFunction) */


#if (WireClock_UseControl)


    /*******************************************************************************
    * Function Name: WireClock_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 WireClock_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(WireClock_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: WireClock_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(WireClock_CONTROL_PTR, control);
    }

#endif /* (WireClock_UseControl) */


#if (!WireClock_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: WireClock_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 WireClock_ReadCapture(void) 
    {
        return (CY_GET_REG8(WireClock_CAPTURE_LSB_PTR));
    }

#endif /* (!WireClock_UsingFixedFunction) */


#if (WireClock_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: WireClock_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 WireClock_ReadCompare(void) 
    {
        #if(WireClock_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(WireClock_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(WireClock_COMPARE1_LSB_PTR));
        #endif /* (WireClock_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: WireClock_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 WireClock_ReadCompare1(void) 
    {
        return (CY_GET_REG8(WireClock_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: WireClock_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 WireClock_ReadCompare2(void) 
    {
        return (CY_GET_REG8(WireClock_COMPARE2_LSB_PTR));
    }

#endif /* (WireClock_UseOneCompareMode) */


/*******************************************************************************
* Function Name: WireClock_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 WireClock_ReadPeriod(void) 
{
    #if(WireClock_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(WireClock_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(WireClock_PERIOD_LSB_PTR));
    #endif /* (WireClock_UsingFixedFunction) */
}

#if ( WireClock_KillModeMinTime)


    /*******************************************************************************
    * Function Name: WireClock_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void WireClock_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(WireClock_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: WireClock_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 WireClock_ReadKillTime(void) 
    {
        return (CY_GET_REG8(WireClock_KILLMODEMINTIME_PTR));
    }

#endif /* ( WireClock_KillModeMinTime) */

/* [] END OF FILE */

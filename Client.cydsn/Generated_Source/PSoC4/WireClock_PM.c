/*******************************************************************************
* File Name: WireClock_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WireClock.h"

static WireClock_backupStruct WireClock_backup;


/*******************************************************************************
* Function Name: WireClock_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireClock_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void WireClock_SaveConfig(void) 
{

    #if(!WireClock_UsingFixedFunction)
        #if(!WireClock_PWMModeIsCenterAligned)
            WireClock_backup.PWMPeriod = WireClock_ReadPeriod();
        #endif /* (!WireClock_PWMModeIsCenterAligned) */
        WireClock_backup.PWMUdb = WireClock_ReadCounter();
        #if (WireClock_UseStatus)
            WireClock_backup.InterruptMaskValue = WireClock_STATUS_MASK;
        #endif /* (WireClock_UseStatus) */

        #if(WireClock_DeadBandMode == WireClock__B_PWM__DBM_256_CLOCKS || \
            WireClock_DeadBandMode == WireClock__B_PWM__DBM_2_4_CLOCKS)
            WireClock_backup.PWMdeadBandValue = WireClock_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(WireClock_KillModeMinTime)
             WireClock_backup.PWMKillCounterPeriod = WireClock_ReadKillTime();
        #endif /* (WireClock_KillModeMinTime) */

        #if(WireClock_UseControl)
            WireClock_backup.PWMControlRegister = WireClock_ReadControlRegister();
        #endif /* (WireClock_UseControl) */
    #endif  /* (!WireClock_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: WireClock_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireClock_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void WireClock_RestoreConfig(void) 
{
        #if(!WireClock_UsingFixedFunction)
            #if(!WireClock_PWMModeIsCenterAligned)
                WireClock_WritePeriod(WireClock_backup.PWMPeriod);
            #endif /* (!WireClock_PWMModeIsCenterAligned) */

            WireClock_WriteCounter(WireClock_backup.PWMUdb);

            #if (WireClock_UseStatus)
                WireClock_STATUS_MASK = WireClock_backup.InterruptMaskValue;
            #endif /* (WireClock_UseStatus) */

            #if(WireClock_DeadBandMode == WireClock__B_PWM__DBM_256_CLOCKS || \
                WireClock_DeadBandMode == WireClock__B_PWM__DBM_2_4_CLOCKS)
                WireClock_WriteDeadTime(WireClock_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(WireClock_KillModeMinTime)
                WireClock_WriteKillTime(WireClock_backup.PWMKillCounterPeriod);
            #endif /* (WireClock_KillModeMinTime) */

            #if(WireClock_UseControl)
                WireClock_WriteControlRegister(WireClock_backup.PWMControlRegister);
            #endif /* (WireClock_UseControl) */
        #endif  /* (!WireClock_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: WireClock_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireClock_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void WireClock_Sleep(void) 
{
    #if(WireClock_UseControl)
        if(WireClock_CTRL_ENABLE == (WireClock_CONTROL & WireClock_CTRL_ENABLE))
        {
            /*Component is enabled */
            WireClock_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            WireClock_backup.PWMEnableState = 0u;
        }
    #endif /* (WireClock_UseControl) */

    /* Stop component */
    WireClock_Stop();

    /* Save registers configuration */
    WireClock_SaveConfig();
}


/*******************************************************************************
* Function Name: WireClock_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  WireClock_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void WireClock_Wakeup(void) 
{
     /* Restore registers values */
    WireClock_RestoreConfig();

    if(WireClock_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        WireClock_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

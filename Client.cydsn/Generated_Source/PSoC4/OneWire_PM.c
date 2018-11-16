/*******************************************************************************
* File Name: OneWire_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OneWire.h"


/***************************************
* Local data allocation
***************************************/

static OneWire_BACKUP_STRUCT  OneWire_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: OneWire_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the OneWire_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  OneWire_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void OneWire_SaveConfig(void)
{
    #if(OneWire_CONTROL_REG_REMOVED == 0u)
        OneWire_backup.cr = OneWire_CONTROL_REG;
    #endif /* End OneWire_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: OneWire_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  OneWire_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling OneWire_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void OneWire_RestoreConfig(void)
{
    #if(OneWire_CONTROL_REG_REMOVED == 0u)
        OneWire_CONTROL_REG = OneWire_backup.cr;
    #endif /* End OneWire_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: OneWire_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The OneWire_Sleep() API saves the current component state. Then it
*  calls the OneWire_Stop() function and calls 
*  OneWire_SaveConfig() to save the hardware configuration.
*  Call the OneWire_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  OneWire_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void OneWire_Sleep(void)
{
    #if(OneWire_RX_ENABLED || OneWire_HD_ENABLED)
        if((OneWire_RXSTATUS_ACTL_REG  & OneWire_INT_ENABLE) != 0u)
        {
            OneWire_backup.enableState = 1u;
        }
        else
        {
            OneWire_backup.enableState = 0u;
        }
    #else
        if((OneWire_TXSTATUS_ACTL_REG  & OneWire_INT_ENABLE) !=0u)
        {
            OneWire_backup.enableState = 1u;
        }
        else
        {
            OneWire_backup.enableState = 0u;
        }
    #endif /* End OneWire_RX_ENABLED || OneWire_HD_ENABLED*/

    OneWire_Stop();
    OneWire_SaveConfig();
}


/*******************************************************************************
* Function Name: OneWire_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  OneWire_Sleep() was called. The OneWire_Wakeup() function
*  calls the OneWire_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  OneWire_Sleep() function was called, the OneWire_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  OneWire_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void OneWire_Wakeup(void)
{
    OneWire_RestoreConfig();
    #if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )
        OneWire_ClearRxBuffer();
    #endif /* End (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */
    #if(OneWire_TX_ENABLED || OneWire_HD_ENABLED)
        OneWire_ClearTxBuffer();
    #endif /* End OneWire_TX_ENABLED || OneWire_HD_ENABLED */

    if(OneWire_backup.enableState != 0u)
    {
        OneWire_Enable();
    }
}


/* [] END OF FILE */

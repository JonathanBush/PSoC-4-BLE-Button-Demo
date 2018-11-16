/*******************************************************************************
* File Name: Wire_UART_PM.c
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

#include "Wire_UART.h"


/***************************************
* Local data allocation
***************************************/

static Wire_UART_BACKUP_STRUCT  Wire_UART_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: Wire_UART_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the Wire_UART_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Wire_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Wire_UART_SaveConfig(void)
{
    #if(Wire_UART_CONTROL_REG_REMOVED == 0u)
        Wire_UART_backup.cr = Wire_UART_CONTROL_REG;
    #endif /* End Wire_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: Wire_UART_RestoreConfig
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
*  Wire_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling Wire_UART_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void Wire_UART_RestoreConfig(void)
{
    #if(Wire_UART_CONTROL_REG_REMOVED == 0u)
        Wire_UART_CONTROL_REG = Wire_UART_backup.cr;
    #endif /* End Wire_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: Wire_UART_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The Wire_UART_Sleep() API saves the current component state. Then it
*  calls the Wire_UART_Stop() function and calls 
*  Wire_UART_SaveConfig() to save the hardware configuration.
*  Call the Wire_UART_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Wire_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Wire_UART_Sleep(void)
{
    #if(Wire_UART_RX_ENABLED || Wire_UART_HD_ENABLED)
        if((Wire_UART_RXSTATUS_ACTL_REG  & Wire_UART_INT_ENABLE) != 0u)
        {
            Wire_UART_backup.enableState = 1u;
        }
        else
        {
            Wire_UART_backup.enableState = 0u;
        }
    #else
        if((Wire_UART_TXSTATUS_ACTL_REG  & Wire_UART_INT_ENABLE) !=0u)
        {
            Wire_UART_backup.enableState = 1u;
        }
        else
        {
            Wire_UART_backup.enableState = 0u;
        }
    #endif /* End Wire_UART_RX_ENABLED || Wire_UART_HD_ENABLED*/

    Wire_UART_Stop();
    Wire_UART_SaveConfig();
}


/*******************************************************************************
* Function Name: Wire_UART_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  Wire_UART_Sleep() was called. The Wire_UART_Wakeup() function
*  calls the Wire_UART_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  Wire_UART_Sleep() function was called, the Wire_UART_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Wire_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Wire_UART_Wakeup(void)
{
    Wire_UART_RestoreConfig();
    #if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )
        Wire_UART_ClearRxBuffer();
    #endif /* End (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */
    #if(Wire_UART_TX_ENABLED || Wire_UART_HD_ENABLED)
        Wire_UART_ClearTxBuffer();
    #endif /* End Wire_UART_TX_ENABLED || Wire_UART_HD_ENABLED */

    if(Wire_UART_backup.enableState != 0u)
    {
        Wire_UART_Enable();
    }
}


/* [] END OF FILE */

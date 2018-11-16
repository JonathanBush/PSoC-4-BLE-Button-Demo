/*******************************************************************************
* File Name: Com_0.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Com_0.h"

static Com_0_BACKUP_STRUCT  Com_0_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Com_0_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Com_0_SUT.c usage_Com_0_Sleep_Wakeup
*******************************************************************************/
void Com_0_Sleep(void)
{
    #if defined(Com_0__PC)
        Com_0_backup.pcState = Com_0_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Com_0_backup.usbState = Com_0_CR1_REG;
            Com_0_USB_POWER_REG |= Com_0_USBIO_ENTER_SLEEP;
            Com_0_CR1_REG &= Com_0_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Com_0__SIO)
        Com_0_backup.sioState = Com_0_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Com_0_SIO_REG &= (uint32)(~Com_0_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Com_0_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Com_0_Sleep() for an example usage.
*******************************************************************************/
void Com_0_Wakeup(void)
{
    #if defined(Com_0__PC)
        Com_0_PC = Com_0_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Com_0_USB_POWER_REG &= Com_0_USBIO_EXIT_SLEEP_PH1;
            Com_0_CR1_REG = Com_0_backup.usbState;
            Com_0_USB_POWER_REG &= Com_0_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Com_0__SIO)
        Com_0_SIO_REG = Com_0_backup.sioState;
    #endif
}


/* [] END OF FILE */

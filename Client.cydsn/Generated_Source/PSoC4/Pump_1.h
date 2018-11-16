/*******************************************************************************
* File Name: Pump_1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pump_1_H) /* Pins Pump_1_H */
#define CY_PINS_Pump_1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Pump_1_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Pump_1_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Pump_1_Read(void);
void    Pump_1_Write(uint8 value);
uint8   Pump_1_ReadDataReg(void);
#if defined(Pump_1__PC) || (CY_PSOC4_4200L) 
    void    Pump_1_SetDriveMode(uint8 mode);
#endif
void    Pump_1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pump_1_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Pump_1_Sleep(void); 
void Pump_1_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Pump_1__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Pump_1_DRIVE_MODE_BITS        (3)
    #define Pump_1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Pump_1_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Pump_1_SetDriveMode() function.
         *  @{
         */
        #define Pump_1_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Pump_1_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Pump_1_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Pump_1_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Pump_1_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Pump_1_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Pump_1_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Pump_1_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Pump_1_MASK               Pump_1__MASK
#define Pump_1_SHIFT              Pump_1__SHIFT
#define Pump_1_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pump_1_SetInterruptMode() function.
     *  @{
     */
        #define Pump_1_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Pump_1_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Pump_1_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Pump_1_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Pump_1__SIO)
    #define Pump_1_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Pump_1__PC) && (CY_PSOC4_4200L)
    #define Pump_1_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Pump_1_USBIO_DISABLE              ((uint32)(~Pump_1_USBIO_ENABLE))
    #define Pump_1_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Pump_1_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Pump_1_USBIO_ENTER_SLEEP          ((uint32)((1u << Pump_1_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Pump_1_USBIO_SUSPEND_DEL_SHIFT)))
    #define Pump_1_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Pump_1_USBIO_SUSPEND_SHIFT)))
    #define Pump_1_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Pump_1_USBIO_SUSPEND_DEL_SHIFT)))
    #define Pump_1_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Pump_1__PC)
    /* Port Configuration */
    #define Pump_1_PC                 (* (reg32 *) Pump_1__PC)
#endif
/* Pin State */
#define Pump_1_PS                     (* (reg32 *) Pump_1__PS)
/* Data Register */
#define Pump_1_DR                     (* (reg32 *) Pump_1__DR)
/* Input Buffer Disable Override */
#define Pump_1_INP_DIS                (* (reg32 *) Pump_1__PC2)

/* Interrupt configuration Registers */
#define Pump_1_INTCFG                 (* (reg32 *) Pump_1__INTCFG)
#define Pump_1_INTSTAT                (* (reg32 *) Pump_1__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Pump_1_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Pump_1__SIO)
    #define Pump_1_SIO_REG            (* (reg32 *) Pump_1__SIO)
#endif /* (Pump_1__SIO_CFG) */

/* USBIO registers */
#if !defined(Pump_1__PC) && (CY_PSOC4_4200L)
    #define Pump_1_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Pump_1_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Pump_1_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Pump_1_DRIVE_MODE_SHIFT       (0x00u)
#define Pump_1_DRIVE_MODE_MASK        (0x07u << Pump_1_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Pump_1_H */


/* [] END OF FILE */

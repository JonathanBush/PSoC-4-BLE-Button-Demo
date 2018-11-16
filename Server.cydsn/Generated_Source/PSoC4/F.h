/*******************************************************************************
* File Name: F.h  
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

#if !defined(CY_PINS_F_H) /* Pins F_H */
#define CY_PINS_F_H

#include "cytypes.h"
#include "cyfitter.h"
#include "F_aliases.h"


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
} F_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   F_Read(void);
void    F_Write(uint8 value);
uint8   F_ReadDataReg(void);
#if defined(F__PC) || (CY_PSOC4_4200L) 
    void    F_SetDriveMode(uint8 mode);
#endif
void    F_SetInterruptMode(uint16 position, uint16 mode);
uint8   F_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void F_Sleep(void); 
void F_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(F__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define F_DRIVE_MODE_BITS        (3)
    #define F_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - F_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the F_SetDriveMode() function.
         *  @{
         */
        #define F_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define F_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define F_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define F_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define F_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define F_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define F_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define F_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define F_MASK               F__MASK
#define F_SHIFT              F__SHIFT
#define F_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in F_SetInterruptMode() function.
     *  @{
     */
        #define F_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define F_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define F_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define F_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(F__SIO)
    #define F_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(F__PC) && (CY_PSOC4_4200L)
    #define F_USBIO_ENABLE               ((uint32)0x80000000u)
    #define F_USBIO_DISABLE              ((uint32)(~F_USBIO_ENABLE))
    #define F_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define F_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define F_USBIO_ENTER_SLEEP          ((uint32)((1u << F_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << F_USBIO_SUSPEND_DEL_SHIFT)))
    #define F_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << F_USBIO_SUSPEND_SHIFT)))
    #define F_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << F_USBIO_SUSPEND_DEL_SHIFT)))
    #define F_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(F__PC)
    /* Port Configuration */
    #define F_PC                 (* (reg32 *) F__PC)
#endif
/* Pin State */
#define F_PS                     (* (reg32 *) F__PS)
/* Data Register */
#define F_DR                     (* (reg32 *) F__DR)
/* Input Buffer Disable Override */
#define F_INP_DIS                (* (reg32 *) F__PC2)

/* Interrupt configuration Registers */
#define F_INTCFG                 (* (reg32 *) F__INTCFG)
#define F_INTSTAT                (* (reg32 *) F__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define F_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(F__SIO)
    #define F_SIO_REG            (* (reg32 *) F__SIO)
#endif /* (F__SIO_CFG) */

/* USBIO registers */
#if !defined(F__PC) && (CY_PSOC4_4200L)
    #define F_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define F_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define F_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define F_DRIVE_MODE_SHIFT       (0x00u)
#define F_DRIVE_MODE_MASK        (0x07u << F_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins F_H */


/* [] END OF FILE */

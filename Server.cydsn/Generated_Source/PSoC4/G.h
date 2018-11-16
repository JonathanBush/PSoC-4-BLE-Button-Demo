/*******************************************************************************
* File Name: G.h  
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

#if !defined(CY_PINS_G_H) /* Pins G_H */
#define CY_PINS_G_H

#include "cytypes.h"
#include "cyfitter.h"
#include "G_aliases.h"


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
} G_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   G_Read(void);
void    G_Write(uint8 value);
uint8   G_ReadDataReg(void);
#if defined(G__PC) || (CY_PSOC4_4200L) 
    void    G_SetDriveMode(uint8 mode);
#endif
void    G_SetInterruptMode(uint16 position, uint16 mode);
uint8   G_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void G_Sleep(void); 
void G_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(G__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define G_DRIVE_MODE_BITS        (3)
    #define G_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - G_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the G_SetDriveMode() function.
         *  @{
         */
        #define G_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define G_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define G_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define G_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define G_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define G_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define G_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define G_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define G_MASK               G__MASK
#define G_SHIFT              G__SHIFT
#define G_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in G_SetInterruptMode() function.
     *  @{
     */
        #define G_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define G_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define G_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define G_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(G__SIO)
    #define G_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(G__PC) && (CY_PSOC4_4200L)
    #define G_USBIO_ENABLE               ((uint32)0x80000000u)
    #define G_USBIO_DISABLE              ((uint32)(~G_USBIO_ENABLE))
    #define G_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define G_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define G_USBIO_ENTER_SLEEP          ((uint32)((1u << G_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << G_USBIO_SUSPEND_DEL_SHIFT)))
    #define G_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << G_USBIO_SUSPEND_SHIFT)))
    #define G_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << G_USBIO_SUSPEND_DEL_SHIFT)))
    #define G_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(G__PC)
    /* Port Configuration */
    #define G_PC                 (* (reg32 *) G__PC)
#endif
/* Pin State */
#define G_PS                     (* (reg32 *) G__PS)
/* Data Register */
#define G_DR                     (* (reg32 *) G__DR)
/* Input Buffer Disable Override */
#define G_INP_DIS                (* (reg32 *) G__PC2)

/* Interrupt configuration Registers */
#define G_INTCFG                 (* (reg32 *) G__INTCFG)
#define G_INTSTAT                (* (reg32 *) G__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define G_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(G__SIO)
    #define G_SIO_REG            (* (reg32 *) G__SIO)
#endif /* (G__SIO_CFG) */

/* USBIO registers */
#if !defined(G__PC) && (CY_PSOC4_4200L)
    #define G_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define G_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define G_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define G_DRIVE_MODE_SHIFT       (0x00u)
#define G_DRIVE_MODE_MASK        (0x07u << G_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins G_H */


/* [] END OF FILE */

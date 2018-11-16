/*******************************************************************************
* File Name: Wire1.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Wire1_ALIASES_H) /* Pins Wire1_ALIASES_H */
#define CY_PINS_Wire1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Wire1_0			(Wire1__0__PC)
#define Wire1_0_PS		(Wire1__0__PS)
#define Wire1_0_PC		(Wire1__0__PC)
#define Wire1_0_DR		(Wire1__0__DR)
#define Wire1_0_SHIFT	(Wire1__0__SHIFT)
#define Wire1_0_INTR	((uint16)((uint16)0x0003u << (Wire1__0__SHIFT*2u)))

#define Wire1_INTR_ALL	 ((uint16)(Wire1_0_INTR))


#endif /* End Pins Wire1_ALIASES_H */


/* [] END OF FILE */

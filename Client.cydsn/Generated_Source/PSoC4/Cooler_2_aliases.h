/*******************************************************************************
* File Name: Cooler_2.h  
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

#if !defined(CY_PINS_Cooler_2_ALIASES_H) /* Pins Cooler_2_ALIASES_H */
#define CY_PINS_Cooler_2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Cooler_2_0			(Cooler_2__0__PC)
#define Cooler_2_0_PS		(Cooler_2__0__PS)
#define Cooler_2_0_PC		(Cooler_2__0__PC)
#define Cooler_2_0_DR		(Cooler_2__0__DR)
#define Cooler_2_0_SHIFT	(Cooler_2__0__SHIFT)
#define Cooler_2_0_INTR	((uint16)((uint16)0x0003u << (Cooler_2__0__SHIFT*2u)))

#define Cooler_2_INTR_ALL	 ((uint16)(Cooler_2_0_INTR))


#endif /* End Pins Cooler_2_ALIASES_H */


/* [] END OF FILE */

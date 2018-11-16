/*******************************************************************************
* File Name: G.h  
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

#if !defined(CY_PINS_G_ALIASES_H) /* Pins G_ALIASES_H */
#define CY_PINS_G_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define G_0			(G__0__PC)
#define G_0_PS		(G__0__PS)
#define G_0_PC		(G__0__PC)
#define G_0_DR		(G__0__DR)
#define G_0_SHIFT	(G__0__SHIFT)
#define G_0_INTR	((uint16)((uint16)0x0003u << (G__0__SHIFT*2u)))

#define G_INTR_ALL	 ((uint16)(G_0_INTR))


#endif /* End Pins G_ALIASES_H */


/* [] END OF FILE */

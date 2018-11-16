/*******************************************************************************
* File Name: Com_0.h  
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

#if !defined(CY_PINS_Com_0_ALIASES_H) /* Pins Com_0_ALIASES_H */
#define CY_PINS_Com_0_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Com_0_0			(Com_0__0__PC)
#define Com_0_0_PS		(Com_0__0__PS)
#define Com_0_0_PC		(Com_0__0__PC)
#define Com_0_0_DR		(Com_0__0__DR)
#define Com_0_0_SHIFT	(Com_0__0__SHIFT)
#define Com_0_0_INTR	((uint16)((uint16)0x0003u << (Com_0__0__SHIFT*2u)))

#define Com_0_INTR_ALL	 ((uint16)(Com_0_0_INTR))


#endif /* End Pins Com_0_ALIASES_H */


/* [] END OF FILE */

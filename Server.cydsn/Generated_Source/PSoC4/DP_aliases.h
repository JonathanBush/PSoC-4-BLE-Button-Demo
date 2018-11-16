/*******************************************************************************
* File Name: DP.h  
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

#if !defined(CY_PINS_DP_ALIASES_H) /* Pins DP_ALIASES_H */
#define CY_PINS_DP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DP_0			(DP__0__PC)
#define DP_0_PS		(DP__0__PS)
#define DP_0_PC		(DP__0__PC)
#define DP_0_DR		(DP__0__DR)
#define DP_0_SHIFT	(DP__0__SHIFT)
#define DP_0_INTR	((uint16)((uint16)0x0003u << (DP__0__SHIFT*2u)))

#define DP_INTR_ALL	 ((uint16)(DP_0_INTR))


#endif /* End Pins DP_ALIASES_H */


/* [] END OF FILE */

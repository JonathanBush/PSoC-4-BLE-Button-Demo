/*******************************************************************************
* File Name: F.h  
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

#if !defined(CY_PINS_F_ALIASES_H) /* Pins F_ALIASES_H */
#define CY_PINS_F_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define F_0			(F__0__PC)
#define F_0_PS		(F__0__PS)
#define F_0_PC		(F__0__PC)
#define F_0_DR		(F__0__DR)
#define F_0_SHIFT	(F__0__SHIFT)
#define F_0_INTR	((uint16)((uint16)0x0003u << (F__0__SHIFT*2u)))

#define F_INTR_ALL	 ((uint16)(F_0_INTR))


#endif /* End Pins F_ALIASES_H */


/* [] END OF FILE */

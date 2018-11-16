/*******************************************************************************
* File Name: Power1.h  
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

#if !defined(CY_PINS_Power1_ALIASES_H) /* Pins Power1_ALIASES_H */
#define CY_PINS_Power1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Power1_0			(Power1__0__PC)
#define Power1_0_PS		(Power1__0__PS)
#define Power1_0_PC		(Power1__0__PC)
#define Power1_0_DR		(Power1__0__DR)
#define Power1_0_SHIFT	(Power1__0__SHIFT)
#define Power1_0_INTR	((uint16)((uint16)0x0003u << (Power1__0__SHIFT*2u)))

#define Power1_INTR_ALL	 ((uint16)(Power1_0_INTR))


#endif /* End Pins Power1_ALIASES_H */


/* [] END OF FILE */

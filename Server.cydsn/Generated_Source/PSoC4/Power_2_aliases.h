/*******************************************************************************
* File Name: Power_2.h  
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

#if !defined(CY_PINS_Power_2_ALIASES_H) /* Pins Power_2_ALIASES_H */
#define CY_PINS_Power_2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Power_2_0			(Power_2__0__PC)
#define Power_2_0_PS		(Power_2__0__PS)
#define Power_2_0_PC		(Power_2__0__PC)
#define Power_2_0_DR		(Power_2__0__DR)
#define Power_2_0_SHIFT	(Power_2__0__SHIFT)
#define Power_2_0_INTR	((uint16)((uint16)0x0003u << (Power_2__0__SHIFT*2u)))

#define Power_2_INTR_ALL	 ((uint16)(Power_2_0_INTR))


#endif /* End Pins Power_2_ALIASES_H */


/* [] END OF FILE */

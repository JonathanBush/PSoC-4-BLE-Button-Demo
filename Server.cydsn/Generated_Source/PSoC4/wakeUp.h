/*******************************************************************************
* File Name: wakeUp.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_wakeUp_H)
#define CY_ISR_wakeUp_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void wakeUp_Start(void);
void wakeUp_StartEx(cyisraddress address);
void wakeUp_Stop(void);

CY_ISR_PROTO(wakeUp_Interrupt);

void wakeUp_SetVector(cyisraddress address);
cyisraddress wakeUp_GetVector(void);

void wakeUp_SetPriority(uint8 priority);
uint8 wakeUp_GetPriority(void);

void wakeUp_Enable(void);
uint8 wakeUp_GetState(void);
void wakeUp_Disable(void);

void wakeUp_SetPending(void);
void wakeUp_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the wakeUp ISR. */
#define wakeUp_INTC_VECTOR            ((reg32 *) wakeUp__INTC_VECT)

/* Address of the wakeUp ISR priority. */
#define wakeUp_INTC_PRIOR             ((reg32 *) wakeUp__INTC_PRIOR_REG)

/* Priority of the wakeUp interrupt. */
#define wakeUp_INTC_PRIOR_NUMBER      wakeUp__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable wakeUp interrupt. */
#define wakeUp_INTC_SET_EN            ((reg32 *) wakeUp__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the wakeUp interrupt. */
#define wakeUp_INTC_CLR_EN            ((reg32 *) wakeUp__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the wakeUp interrupt state to pending. */
#define wakeUp_INTC_SET_PD            ((reg32 *) wakeUp__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the wakeUp interrupt. */
#define wakeUp_INTC_CLR_PD            ((reg32 *) wakeUp__INTC_CLR_PD_REG)



#endif /* CY_ISR_wakeUp_H */


/* [] END OF FILE */

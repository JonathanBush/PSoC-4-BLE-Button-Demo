/*******************************************************************************
* File Name: OneWire.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OneWire.h"
#if (OneWire_INTERNAL_CLOCK_USED)
    #include "OneWire_IntClock.h"
#endif /* End OneWire_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 OneWire_initVar = 0u;

#if (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED)
    volatile uint8 OneWire_txBuffer[OneWire_TX_BUFFER_SIZE];
    volatile uint8 OneWire_txBufferRead = 0u;
    uint8 OneWire_txBufferWrite = 0u;
#endif /* (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED) */

#if (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED))
    uint8 OneWire_errorStatus = 0u;
    volatile uint8 OneWire_rxBuffer[OneWire_RX_BUFFER_SIZE];
    volatile uint8 OneWire_rxBufferRead  = 0u;
    volatile uint8 OneWire_rxBufferWrite = 0u;
    volatile uint8 OneWire_rxBufferLoopDetect = 0u;
    volatile uint8 OneWire_rxBufferOverflow   = 0u;
    #if (OneWire_RXHW_ADDRESS_ENABLED)
        volatile uint8 OneWire_rxAddressMode = OneWire_RX_ADDRESS_MODE;
        volatile uint8 OneWire_rxAddressDetected = 0u;
    #endif /* (OneWire_RXHW_ADDRESS_ENABLED) */
#endif /* (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED)) */


/*******************************************************************************
* Function Name: OneWire_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  OneWire_Start() sets the initVar variable, calls the
*  OneWire_Init() function, and then calls the
*  OneWire_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The OneWire_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time OneWire_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the OneWire_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void OneWire_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(OneWire_initVar == 0u)
    {
        OneWire_Init();
        OneWire_initVar = 1u;
    }

    OneWire_Enable();
}


/*******************************************************************************
* Function Name: OneWire_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call OneWire_Init() because
*  the OneWire_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void OneWire_Init(void) 
{
    #if(OneWire_RX_ENABLED || OneWire_HD_ENABLED)

        #if (OneWire_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(OneWire_RX_VECT_NUM, &OneWire_RXISR);
            CyIntSetPriority(OneWire_RX_VECT_NUM, OneWire_RX_PRIOR_NUM);
            OneWire_errorStatus = 0u;
        #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        #if (OneWire_RXHW_ADDRESS_ENABLED)
            OneWire_SetRxAddressMode(OneWire_RX_ADDRESS_MODE);
            OneWire_SetRxAddress1(OneWire_RX_HW_ADDRESS1);
            OneWire_SetRxAddress2(OneWire_RX_HW_ADDRESS2);
        #endif /* End OneWire_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        OneWire_RXBITCTR_PERIOD_REG = OneWire_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        OneWire_RXSTATUS_MASK_REG  = OneWire_INIT_RX_INTERRUPTS_MASK;
    #endif /* End OneWire_RX_ENABLED || OneWire_HD_ENABLED*/

    #if(OneWire_TX_ENABLED)
        #if (OneWire_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(OneWire_TX_VECT_NUM, &OneWire_TXISR);
            CyIntSetPriority(OneWire_TX_VECT_NUM, OneWire_TX_PRIOR_NUM);
        #endif /* (OneWire_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (OneWire_TXCLKGEN_DP)
            OneWire_TXBITCLKGEN_CTR_REG = OneWire_BIT_CENTER;
            OneWire_TXBITCLKTX_COMPLETE_REG = ((OneWire_NUMBER_OF_DATA_BITS +
                        OneWire_NUMBER_OF_START_BIT) * OneWire_OVER_SAMPLE_COUNT) - 1u;
        #else
            OneWire_TXBITCTR_PERIOD_REG = ((OneWire_NUMBER_OF_DATA_BITS +
                        OneWire_NUMBER_OF_START_BIT) * OneWire_OVER_SAMPLE_8) - 1u;
        #endif /* End OneWire_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (OneWire_TX_INTERRUPT_ENABLED)
            OneWire_TXSTATUS_MASK_REG = OneWire_TX_STS_FIFO_EMPTY;
        #else
            OneWire_TXSTATUS_MASK_REG = OneWire_INIT_TX_INTERRUPTS_MASK;
        #endif /*End OneWire_TX_INTERRUPT_ENABLED*/

    #endif /* End OneWire_TX_ENABLED */

    #if(OneWire_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        OneWire_WriteControlRegister( \
            (OneWire_ReadControlRegister() & (uint8)~OneWire_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(OneWire_PARITY_TYPE << OneWire_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End OneWire_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: OneWire_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call OneWire_Enable() because the OneWire_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  OneWire_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void OneWire_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (OneWire_RX_ENABLED || OneWire_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        OneWire_RXBITCTR_CONTROL_REG |= OneWire_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        OneWire_RXSTATUS_ACTL_REG  |= OneWire_INT_ENABLE;

        #if (OneWire_RX_INTERRUPT_ENABLED)
            OneWire_EnableRxInt();

            #if (OneWire_RXHW_ADDRESS_ENABLED)
                OneWire_rxAddressDetected = 0u;
            #endif /* (OneWire_RXHW_ADDRESS_ENABLED) */
        #endif /* (OneWire_RX_INTERRUPT_ENABLED) */
    #endif /* (OneWire_RX_ENABLED || OneWire_HD_ENABLED) */

    #if(OneWire_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!OneWire_TXCLKGEN_DP)
            OneWire_TXBITCTR_CONTROL_REG |= OneWire_CNTR_ENABLE;
        #endif /* End OneWire_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        OneWire_TXSTATUS_ACTL_REG |= OneWire_INT_ENABLE;
        #if (OneWire_TX_INTERRUPT_ENABLED)
            OneWire_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            OneWire_EnableTxInt();
        #endif /* (OneWire_TX_INTERRUPT_ENABLED) */
     #endif /* (OneWire_TX_INTERRUPT_ENABLED) */

    #if (OneWire_INTERNAL_CLOCK_USED)
        OneWire_IntClock_Start();  /* Enable the clock */
    #endif /* (OneWire_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: OneWire_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void OneWire_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (OneWire_RX_ENABLED || OneWire_HD_ENABLED)
        OneWire_RXBITCTR_CONTROL_REG &= (uint8) ~OneWire_CNTR_ENABLE;
    #endif /* (OneWire_RX_ENABLED || OneWire_HD_ENABLED) */

    #if (OneWire_TX_ENABLED)
        #if(!OneWire_TXCLKGEN_DP)
            OneWire_TXBITCTR_CONTROL_REG &= (uint8) ~OneWire_CNTR_ENABLE;
        #endif /* (!OneWire_TXCLKGEN_DP) */
    #endif /* (OneWire_TX_ENABLED) */

    #if (OneWire_INTERNAL_CLOCK_USED)
        OneWire_IntClock_Stop();   /* Disable the clock */
    #endif /* (OneWire_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (OneWire_RX_ENABLED || OneWire_HD_ENABLED)
        OneWire_RXSTATUS_ACTL_REG  &= (uint8) ~OneWire_INT_ENABLE;

        #if (OneWire_RX_INTERRUPT_ENABLED)
            OneWire_DisableRxInt();
        #endif /* (OneWire_RX_INTERRUPT_ENABLED) */
    #endif /* (OneWire_RX_ENABLED || OneWire_HD_ENABLED) */

    #if (OneWire_TX_ENABLED)
        OneWire_TXSTATUS_ACTL_REG &= (uint8) ~OneWire_INT_ENABLE;

        #if (OneWire_TX_INTERRUPT_ENABLED)
            OneWire_DisableTxInt();
        #endif /* (OneWire_TX_INTERRUPT_ENABLED) */
    #endif /* (OneWire_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: OneWire_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 OneWire_ReadControlRegister(void) 
{
    #if (OneWire_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(OneWire_CONTROL_REG);
    #endif /* (OneWire_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: OneWire_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  OneWire_WriteControlRegister(uint8 control) 
{
    #if (OneWire_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       OneWire_CONTROL_REG = control;
    #endif /* (OneWire_CONTROL_REG_REMOVED) */
}


#if(OneWire_RX_ENABLED || OneWire_HD_ENABLED)
    /*******************************************************************************
    * Function Name: OneWire_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      OneWire_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      OneWire_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      OneWire_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      OneWire_RX_STS_BREAK            Interrupt on break.
    *      OneWire_RX_STS_OVERRUN          Interrupt on overrun error.
    *      OneWire_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      OneWire_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void OneWire_SetRxInterruptMode(uint8 intSrc) 
    {
        OneWire_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: OneWire_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  OneWire_rxBuffer - RAM buffer pointer for save received data.
    *  OneWire_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  OneWire_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  OneWire_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 OneWire_ReadRxData(void) 
    {
        uint8 rxData;

    #if (OneWire_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        OneWire_DisableRxInt();

        locRxBufferRead  = OneWire_rxBufferRead;
        locRxBufferWrite = OneWire_rxBufferWrite;

        if( (OneWire_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = OneWire_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= OneWire_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            OneWire_rxBufferRead = locRxBufferRead;

            if(OneWire_rxBufferLoopDetect != 0u)
            {
                OneWire_rxBufferLoopDetect = 0u;
                #if ((OneWire_RX_INTERRUPT_ENABLED) && (OneWire_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( OneWire_HD_ENABLED )
                        if((OneWire_CONTROL_REG & OneWire_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            OneWire_RXSTATUS_MASK_REG  |= OneWire_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        OneWire_RXSTATUS_MASK_REG  |= OneWire_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end OneWire_HD_ENABLED */
                #endif /* ((OneWire_RX_INTERRUPT_ENABLED) && (OneWire_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = OneWire_RXDATA_REG;
        }

        OneWire_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = OneWire_RXDATA_REG;

    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: OneWire_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  OneWire_RX_STS_FIFO_NOTEMPTY.
    *  OneWire_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  OneWire_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   OneWire_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   OneWire_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 OneWire_ReadRxStatus(void) 
    {
        uint8 status;

        status = OneWire_RXSTATUS_REG & OneWire_RX_HW_MASK;

    #if (OneWire_RX_INTERRUPT_ENABLED)
        if(OneWire_rxBufferOverflow != 0u)
        {
            status |= OneWire_RX_STS_SOFT_BUFF_OVER;
            OneWire_rxBufferOverflow = 0u;
        }
    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: OneWire_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. OneWire_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  OneWire_rxBuffer - RAM buffer pointer for save received data.
    *  OneWire_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  OneWire_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  OneWire_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 OneWire_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (OneWire_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        OneWire_DisableRxInt();

        locRxBufferRead  = OneWire_rxBufferRead;
        locRxBufferWrite = OneWire_rxBufferWrite;

        if( (OneWire_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = OneWire_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= OneWire_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            OneWire_rxBufferRead = locRxBufferRead;

            if(OneWire_rxBufferLoopDetect != 0u)
            {
                OneWire_rxBufferLoopDetect = 0u;
                #if( (OneWire_RX_INTERRUPT_ENABLED) && (OneWire_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( OneWire_HD_ENABLED )
                        if((OneWire_CONTROL_REG & OneWire_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            OneWire_RXSTATUS_MASK_REG |= OneWire_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        OneWire_RXSTATUS_MASK_REG |= OneWire_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end OneWire_HD_ENABLED */
                #endif /* OneWire_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = OneWire_RXSTATUS_REG;
            if((rxStatus & OneWire_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = OneWire_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (OneWire_RX_STS_BREAK | OneWire_RX_STS_PAR_ERROR |
                                OneWire_RX_STS_STOP_ERROR | OneWire_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        OneWire_EnableRxInt();

    #else

        rxStatus =OneWire_RXSTATUS_REG;
        if((rxStatus & OneWire_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = OneWire_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (OneWire_RX_STS_BREAK | OneWire_RX_STS_PAR_ERROR |
                            OneWire_RX_STS_STOP_ERROR | OneWire_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: OneWire_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 OneWire_GetByte(void) 
    {
        
    #if (OneWire_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        OneWire_DisableRxInt();
        locErrorStatus = (uint16)OneWire_errorStatus;
        OneWire_errorStatus = 0u;
        OneWire_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | OneWire_ReadRxData() );
    #else
        return ( ((uint16)OneWire_ReadRxStatus() << 8u) | OneWire_ReadRxData() );
    #endif /* OneWire_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: OneWire_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  OneWire_rxBufferWrite - used to calculate left bytes.
    *  OneWire_rxBufferRead - used to calculate left bytes.
    *  OneWire_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 OneWire_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (OneWire_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        OneWire_DisableRxInt();

        if(OneWire_rxBufferRead == OneWire_rxBufferWrite)
        {
            if(OneWire_rxBufferLoopDetect != 0u)
            {
                size = OneWire_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(OneWire_rxBufferRead < OneWire_rxBufferWrite)
        {
            size = (OneWire_rxBufferWrite - OneWire_rxBufferRead);
        }
        else
        {
            size = (OneWire_RX_BUFFER_SIZE - OneWire_rxBufferRead) + OneWire_rxBufferWrite;
        }

        OneWire_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((OneWire_RXSTATUS_REG & OneWire_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: OneWire_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_rxBufferWrite - cleared to zero.
    *  OneWire_rxBufferRead - cleared to zero.
    *  OneWire_rxBufferLoopDetect - cleared to zero.
    *  OneWire_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void OneWire_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        OneWire_RXDATA_AUX_CTL_REG |= (uint8)  OneWire_RX_FIFO_CLR;
        OneWire_RXDATA_AUX_CTL_REG &= (uint8) ~OneWire_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (OneWire_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        OneWire_DisableRxInt();

        OneWire_rxBufferRead = 0u;
        OneWire_rxBufferWrite = 0u;
        OneWire_rxBufferLoopDetect = 0u;
        OneWire_rxBufferOverflow = 0u;

        OneWire_EnableRxInt();

    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: OneWire_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  OneWire__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  OneWire__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  OneWire__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  OneWire__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  OneWire__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  OneWire_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void OneWire_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(OneWire_RXHW_ADDRESS_ENABLED)
            #if(OneWire_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* OneWire_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = OneWire_CONTROL_REG & (uint8)~OneWire_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << OneWire_CTRL_RXADDR_MODE0_SHIFT);
                OneWire_CONTROL_REG = tmpCtrl;

                #if(OneWire_RX_INTERRUPT_ENABLED && \
                   (OneWire_RXBUFFERSIZE > OneWire_FIFO_LENGTH) )
                    OneWire_rxAddressMode = addressMode;
                    OneWire_rxAddressDetected = 0u;
                #endif /* End OneWire_RXBUFFERSIZE > OneWire_FIFO_LENGTH*/
            #endif /* End OneWire_CONTROL_REG_REMOVED */
        #else /* OneWire_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End OneWire_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: OneWire_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void OneWire_SetRxAddress1(uint8 address) 
    {
        OneWire_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: OneWire_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void OneWire_SetRxAddress2(uint8 address) 
    {
        OneWire_RXADDRESS2_REG = address;
    }

#endif  /* OneWire_RX_ENABLED || OneWire_HD_ENABLED*/


#if( (OneWire_TX_ENABLED) || (OneWire_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: OneWire_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   OneWire_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   OneWire_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   OneWire_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   OneWire_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void OneWire_SetTxInterruptMode(uint8 intSrc) 
    {
        OneWire_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: OneWire_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  OneWire_txBuffer - RAM buffer pointer for save data for transmission
    *  OneWire_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  OneWire_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  OneWire_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void OneWire_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(OneWire_initVar != 0u)
        {
        #if (OneWire_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            OneWire_DisableTxInt();

            if( (OneWire_txBufferRead == OneWire_txBufferWrite) &&
                ((OneWire_TXSTATUS_REG & OneWire_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                OneWire_TXDATA_REG = txDataByte;
            }
            else
            {
                if(OneWire_txBufferWrite >= OneWire_TX_BUFFER_SIZE)
                {
                    OneWire_txBufferWrite = 0u;
                }

                OneWire_txBuffer[OneWire_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                OneWire_txBufferWrite++;
            }

            OneWire_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            OneWire_TXDATA_REG = txDataByte;

        #endif /*(OneWire_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: OneWire_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 OneWire_ReadTxStatus(void) 
    {
        return(OneWire_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: OneWire_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_txBuffer - RAM buffer pointer for save data for transmission
    *  OneWire_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  OneWire_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  OneWire_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void OneWire_PutChar(uint8 txDataByte) 
    {
    #if (OneWire_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            OneWire_DisableTxInt();
        #endif /* (OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = OneWire_txBufferWrite;
            locTxBufferRead  = OneWire_txBufferRead;

        #if ((OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            OneWire_EnableTxInt();
        #endif /* (OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(OneWire_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((OneWire_TXSTATUS_REG & OneWire_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            OneWire_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= OneWire_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            OneWire_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3))
            OneWire_DisableTxInt();
        #endif /* (OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3) */

            OneWire_txBufferWrite = locTxBufferWrite;

        #if ((OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3))
            OneWire_EnableTxInt();
        #endif /* (OneWire_TX_BUFFER_SIZE > OneWire_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (OneWire_TXSTATUS_REG & OneWire_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                OneWire_SetPendingTxInt();
            }
        }

    #else

        while((OneWire_TXSTATUS_REG & OneWire_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        OneWire_TXDATA_REG = txDataByte;

    #endif /* OneWire_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: OneWire_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void OneWire_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(OneWire_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                OneWire_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: OneWire_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void OneWire_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(OneWire_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                OneWire_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: OneWire_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void OneWire_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(OneWire_initVar != 0u)
        {
            OneWire_PutChar(txDataByte);
            OneWire_PutChar(0x0Du);
            OneWire_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: OneWire_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  OneWire_txBufferWrite - used to calculate left space.
    *  OneWire_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 OneWire_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (OneWire_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        OneWire_DisableTxInt();

        if(OneWire_txBufferRead == OneWire_txBufferWrite)
        {
            size = 0u;
        }
        else if(OneWire_txBufferRead < OneWire_txBufferWrite)
        {
            size = (OneWire_txBufferWrite - OneWire_txBufferRead);
        }
        else
        {
            size = (OneWire_TX_BUFFER_SIZE - OneWire_txBufferRead) +
                    OneWire_txBufferWrite;
        }

        OneWire_EnableTxInt();

    #else

        size = OneWire_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & OneWire_TX_STS_FIFO_FULL) != 0u)
        {
            size = OneWire_FIFO_LENGTH;
        }
        else if((size & OneWire_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (OneWire_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: OneWire_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_txBufferWrite - cleared to zero.
    *  OneWire_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void OneWire_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        OneWire_TXDATA_AUX_CTL_REG |= (uint8)  OneWire_TX_FIFO_CLR;
        OneWire_TXDATA_AUX_CTL_REG &= (uint8) ~OneWire_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (OneWire_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        OneWire_DisableTxInt();

        OneWire_txBufferRead = 0u;
        OneWire_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        OneWire_EnableTxInt();

    #endif /* (OneWire_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: OneWire_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   OneWire_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   OneWire_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   OneWire_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   OneWire_SEND_WAIT_REINIT - Performs both options: 
    *      OneWire_SEND_BREAK and OneWire_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with OneWire_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The OneWire_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void OneWire_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(OneWire_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(OneWire_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == OneWire_SEND_BREAK) ||
                (retMode == OneWire_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                OneWire_WriteControlRegister(OneWire_ReadControlRegister() |
                                                      OneWire_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                OneWire_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = OneWire_TXSTATUS_REG;
                }
                while((tmpStat & OneWire_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == OneWire_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == OneWire_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = OneWire_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & OneWire_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == OneWire_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == OneWire_REINIT) ||
                (retMode == OneWire_SEND_WAIT_REINIT) )
            {
                OneWire_WriteControlRegister(OneWire_ReadControlRegister() &
                                              (uint8)~OneWire_CTRL_HD_SEND_BREAK);
            }

        #else /* OneWire_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == OneWire_SEND_BREAK) ||
                (retMode == OneWire_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (OneWire_PARITY_TYPE != OneWire__B_UART__NONE_REVB) || \
                                    (OneWire_PARITY_TYPE_SW != 0u) )
                    OneWire_WriteControlRegister(OneWire_ReadControlRegister() |
                                                          OneWire_CTRL_HD_SEND_BREAK);
                #endif /* End OneWire_PARITY_TYPE != OneWire__B_UART__NONE_REVB  */

                #if(OneWire_TXCLKGEN_DP)
                    txPeriod = OneWire_TXBITCLKTX_COMPLETE_REG;
                    OneWire_TXBITCLKTX_COMPLETE_REG = OneWire_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = OneWire_TXBITCTR_PERIOD_REG;
                    OneWire_TXBITCTR_PERIOD_REG = OneWire_TXBITCTR_BREAKBITS8X;
                #endif /* End OneWire_TXCLKGEN_DP */

                /* Send zeros */
                OneWire_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = OneWire_TXSTATUS_REG;
                }
                while((tmpStat & OneWire_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == OneWire_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == OneWire_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = OneWire_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & OneWire_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == OneWire_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == OneWire_REINIT) ||
                (retMode == OneWire_SEND_WAIT_REINIT) )
            {

            #if(OneWire_TXCLKGEN_DP)
                OneWire_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                OneWire_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End OneWire_TXCLKGEN_DP */

            #if( (OneWire_PARITY_TYPE != OneWire__B_UART__NONE_REVB) || \
                 (OneWire_PARITY_TYPE_SW != 0u) )
                OneWire_WriteControlRegister(OneWire_ReadControlRegister() &
                                                      (uint8) ~OneWire_CTRL_HD_SEND_BREAK);
            #endif /* End OneWire_PARITY_TYPE != NONE */
            }
        #endif    /* End OneWire_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: OneWire_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       OneWire_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       OneWire_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears OneWire_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void OneWire_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( OneWire_CONTROL_REG_REMOVED == 0u )
            OneWire_WriteControlRegister(OneWire_ReadControlRegister() |
                                                  OneWire_CTRL_MARK);
        #endif /* End OneWire_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( OneWire_CONTROL_REG_REMOVED == 0u )
            OneWire_WriteControlRegister(OneWire_ReadControlRegister() &
                                                  (uint8) ~OneWire_CTRL_MARK);
        #endif /* End OneWire_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndOneWire_TX_ENABLED */

#if(OneWire_HD_ENABLED)


    /*******************************************************************************
    * Function Name: OneWire_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void OneWire_LoadRxConfig(void) 
    {
        OneWire_WriteControlRegister(OneWire_ReadControlRegister() &
                                                (uint8)~OneWire_CTRL_HD_SEND);
        OneWire_RXBITCTR_PERIOD_REG = OneWire_HD_RXBITCTR_INIT;

    #if (OneWire_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        OneWire_SetRxInterruptMode(OneWire_INIT_RX_INTERRUPTS_MASK);
    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: OneWire_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void OneWire_LoadTxConfig(void) 
    {
    #if (OneWire_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        OneWire_SetRxInterruptMode(0u);
    #endif /* (OneWire_RX_INTERRUPT_ENABLED) */

        OneWire_WriteControlRegister(OneWire_ReadControlRegister() | OneWire_CTRL_HD_SEND);
        OneWire_RXBITCTR_PERIOD_REG = OneWire_HD_TXBITCTR_INIT;
    }

#endif  /* OneWire_HD_ENABLED */


/* [] END OF FILE */

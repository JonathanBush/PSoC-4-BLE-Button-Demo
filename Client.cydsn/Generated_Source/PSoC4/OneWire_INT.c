/*******************************************************************************
* File Name: OneWireINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OneWire.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED))
    /*******************************************************************************
    * Function Name: OneWire_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_rxBuffer - RAM buffer pointer for save received data.
    *  OneWire_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  OneWire_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  OneWire_rxBufferOverflow - software overflow flag. Set to one
    *     when OneWire_rxBufferWrite index overtakes
    *     OneWire_rxBufferRead index.
    *  OneWire_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when OneWire_rxBufferWrite is equal to
    *    OneWire_rxBufferRead
    *  OneWire_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  OneWire_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(OneWire_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef OneWire_RXISR_ENTRY_CALLBACK
        OneWire_RXISR_EntryCallback();
    #endif /* OneWire_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START OneWire_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = OneWire_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in OneWire_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (OneWire_RX_STS_BREAK | 
                            OneWire_RX_STS_PAR_ERROR |
                            OneWire_RX_STS_STOP_ERROR | 
                            OneWire_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                OneWire_errorStatus |= readStatus & ( OneWire_RX_STS_BREAK | 
                                                            OneWire_RX_STS_PAR_ERROR | 
                                                            OneWire_RX_STS_STOP_ERROR | 
                                                            OneWire_RX_STS_OVERRUN);
                /* `#START OneWire_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef OneWire_RXISR_ERROR_CALLBACK
                OneWire_RXISR_ERROR_Callback();
            #endif /* OneWire_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & OneWire_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = OneWire_RXDATA_REG;
            #if (OneWire_RXHW_ADDRESS_ENABLED)
                if(OneWire_rxAddressMode == (uint8)OneWire__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & OneWire_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & OneWire_RX_STS_ADDR_MATCH) != 0u)
                        {
                            OneWire_rxAddressDetected = 1u;
                        }
                        else
                        {
                            OneWire_rxAddressDetected = 0u;
                        }
                    }
                    if(OneWire_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        OneWire_rxBuffer[OneWire_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    OneWire_rxBuffer[OneWire_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                OneWire_rxBuffer[OneWire_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (OneWire_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(OneWire_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        OneWire_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    OneWire_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(OneWire_rxBufferWrite >= OneWire_RX_BUFFER_SIZE)
                    {
                        OneWire_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(OneWire_rxBufferWrite == OneWire_rxBufferRead)
                    {
                        OneWire_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (OneWire_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            OneWire_RXSTATUS_MASK_REG  &= (uint8)~OneWire_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(OneWire_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (OneWire_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & OneWire_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START OneWire_RXISR_END` */

        /* `#END` */

    #ifdef OneWire_RXISR_EXIT_CALLBACK
        OneWire_RXISR_ExitCallback();
    #endif /* OneWire_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED)) */


#if (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED)
    /*******************************************************************************
    * Function Name: OneWire_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  OneWire_txBuffer - RAM buffer pointer for transmit data from.
    *  OneWire_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  OneWire_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(OneWire_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef OneWire_TXISR_ENTRY_CALLBACK
        OneWire_TXISR_EntryCallback();
    #endif /* OneWire_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START OneWire_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((OneWire_txBufferRead != OneWire_txBufferWrite) &&
             ((OneWire_TXSTATUS_REG & OneWire_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(OneWire_txBufferRead >= OneWire_TX_BUFFER_SIZE)
            {
                OneWire_txBufferRead = 0u;
            }

            OneWire_TXDATA_REG = OneWire_txBuffer[OneWire_txBufferRead];

            /* Set next pointer */
            OneWire_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START OneWire_TXISR_END` */

        /* `#END` */

    #ifdef OneWire_TXISR_EXIT_CALLBACK
        OneWire_TXISR_ExitCallback();
    #endif /* OneWire_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED) */


/* [] END OF FILE */

/*******************************************************************************
* File Name: Wire_UART.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_Wire_UART_H)
#define CY_UART_Wire_UART_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define Wire_UART_RX_ENABLED                     (1u)
#define Wire_UART_TX_ENABLED                     (1u)
#define Wire_UART_HD_ENABLED                     (0u)
#define Wire_UART_RX_INTERRUPT_ENABLED           (0u)
#define Wire_UART_TX_INTERRUPT_ENABLED           (0u)
#define Wire_UART_INTERNAL_CLOCK_USED            (0u)
#define Wire_UART_RXHW_ADDRESS_ENABLED           (0u)
#define Wire_UART_OVER_SAMPLE_COUNT              (8u)
#define Wire_UART_PARITY_TYPE                    (0u)
#define Wire_UART_PARITY_TYPE_SW                 (0u)
#define Wire_UART_BREAK_DETECT                   (0u)
#define Wire_UART_BREAK_BITS_TX                  (13u)
#define Wire_UART_BREAK_BITS_RX                  (13u)
#define Wire_UART_TXCLKGEN_DP                    (1u)
#define Wire_UART_USE23POLLING                   (1u)
#define Wire_UART_FLOW_CONTROL                   (0u)
#define Wire_UART_CLK_FREQ                       (0u)
#define Wire_UART_TX_BUFFER_SIZE                 (4u)
#define Wire_UART_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define Wire_UART_CONTROL_REG_REMOVED            (0u)
#else
    #define Wire_UART_CONTROL_REG_REMOVED            (1u)
#endif /* End Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct Wire_UART_backupStruct_
{
    uint8 enableState;

    #if(Wire_UART_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End Wire_UART_CONTROL_REG_REMOVED */

} Wire_UART_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void Wire_UART_Start(void) ;
void Wire_UART_Stop(void) ;
uint8 Wire_UART_ReadControlRegister(void) ;
void Wire_UART_WriteControlRegister(uint8 control) ;

void Wire_UART_Init(void) ;
void Wire_UART_Enable(void) ;
void Wire_UART_SaveConfig(void) ;
void Wire_UART_RestoreConfig(void) ;
void Wire_UART_Sleep(void) ;
void Wire_UART_Wakeup(void) ;

/* Only if RX is enabled */
#if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )

    #if (Wire_UART_RX_INTERRUPT_ENABLED)
        #define Wire_UART_EnableRxInt()  CyIntEnable (Wire_UART_RX_VECT_NUM)
        #define Wire_UART_DisableRxInt() CyIntDisable(Wire_UART_RX_VECT_NUM)
        CY_ISR_PROTO(Wire_UART_RXISR);
    #endif /* Wire_UART_RX_INTERRUPT_ENABLED */

    void Wire_UART_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void Wire_UART_SetRxAddress1(uint8 address) ;
    void Wire_UART_SetRxAddress2(uint8 address) ;

    void  Wire_UART_SetRxInterruptMode(uint8 intSrc) ;
    uint8 Wire_UART_ReadRxData(void) ;
    uint8 Wire_UART_ReadRxStatus(void) ;
    uint8 Wire_UART_GetChar(void) ;
    uint16 Wire_UART_GetByte(void) ;
    uint8 Wire_UART_GetRxBufferSize(void)
                                                            ;
    void Wire_UART_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define Wire_UART_GetRxInterruptSource   Wire_UART_ReadRxStatus

#endif /* End (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */

/* Only if TX is enabled */
#if(Wire_UART_TX_ENABLED || Wire_UART_HD_ENABLED)

    #if(Wire_UART_TX_INTERRUPT_ENABLED)
        #define Wire_UART_EnableTxInt()  CyIntEnable (Wire_UART_TX_VECT_NUM)
        #define Wire_UART_DisableTxInt() CyIntDisable(Wire_UART_TX_VECT_NUM)
        #define Wire_UART_SetPendingTxInt() CyIntSetPending(Wire_UART_TX_VECT_NUM)
        #define Wire_UART_ClearPendingTxInt() CyIntClearPending(Wire_UART_TX_VECT_NUM)
        CY_ISR_PROTO(Wire_UART_TXISR);
    #endif /* Wire_UART_TX_INTERRUPT_ENABLED */

    void Wire_UART_SetTxInterruptMode(uint8 intSrc) ;
    void Wire_UART_WriteTxData(uint8 txDataByte) ;
    uint8 Wire_UART_ReadTxStatus(void) ;
    void Wire_UART_PutChar(uint8 txDataByte) ;
    void Wire_UART_PutString(const char8 string[]) ;
    void Wire_UART_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void Wire_UART_PutCRLF(uint8 txDataByte) ;
    void Wire_UART_ClearTxBuffer(void) ;
    void Wire_UART_SetTxAddressMode(uint8 addressMode) ;
    void Wire_UART_SendBreak(uint8 retMode) ;
    uint8 Wire_UART_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define Wire_UART_PutStringConst         Wire_UART_PutString
    #define Wire_UART_PutArrayConst          Wire_UART_PutArray
    #define Wire_UART_GetTxInterruptSource   Wire_UART_ReadTxStatus

#endif /* End Wire_UART_TX_ENABLED || Wire_UART_HD_ENABLED */

#if(Wire_UART_HD_ENABLED)
    void Wire_UART_LoadRxConfig(void) ;
    void Wire_UART_LoadTxConfig(void) ;
#endif /* End Wire_UART_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Wire_UART) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    Wire_UART_CyBtldrCommStart(void) CYSMALL ;
    void    Wire_UART_CyBtldrCommStop(void) CYSMALL ;
    void    Wire_UART_CyBtldrCommReset(void) CYSMALL ;
    cystatus Wire_UART_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus Wire_UART_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Wire_UART)
        #define CyBtldrCommStart    Wire_UART_CyBtldrCommStart
        #define CyBtldrCommStop     Wire_UART_CyBtldrCommStop
        #define CyBtldrCommReset    Wire_UART_CyBtldrCommReset
        #define CyBtldrCommWrite    Wire_UART_CyBtldrCommWrite
        #define CyBtldrCommRead     Wire_UART_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Wire_UART) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define Wire_UART_BYTE2BYTE_TIME_OUT (25u)
    #define Wire_UART_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define Wire_UART_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define Wire_UART_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define Wire_UART_SET_SPACE      (0x00u)
#define Wire_UART_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (Wire_UART_TX_ENABLED) || (Wire_UART_HD_ENABLED) )
    #if(Wire_UART_TX_INTERRUPT_ENABLED)
        #define Wire_UART_TX_VECT_NUM            (uint8)Wire_UART_TXInternalInterrupt__INTC_NUMBER
        #define Wire_UART_TX_PRIOR_NUM           (uint8)Wire_UART_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* Wire_UART_TX_INTERRUPT_ENABLED */

    #define Wire_UART_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define Wire_UART_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define Wire_UART_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(Wire_UART_TX_ENABLED)
        #define Wire_UART_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (Wire_UART_HD_ENABLED) */
        #define Wire_UART_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (Wire_UART_TX_ENABLED) */

    #define Wire_UART_TX_STS_COMPLETE            (uint8)(0x01u << Wire_UART_TX_STS_COMPLETE_SHIFT)
    #define Wire_UART_TX_STS_FIFO_EMPTY          (uint8)(0x01u << Wire_UART_TX_STS_FIFO_EMPTY_SHIFT)
    #define Wire_UART_TX_STS_FIFO_FULL           (uint8)(0x01u << Wire_UART_TX_STS_FIFO_FULL_SHIFT)
    #define Wire_UART_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << Wire_UART_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (Wire_UART_TX_ENABLED) || (Wire_UART_HD_ENABLED)*/

#if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )
    #if(Wire_UART_RX_INTERRUPT_ENABLED)
        #define Wire_UART_RX_VECT_NUM            (uint8)Wire_UART_RXInternalInterrupt__INTC_NUMBER
        #define Wire_UART_RX_PRIOR_NUM           (uint8)Wire_UART_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* Wire_UART_RX_INTERRUPT_ENABLED */
    #define Wire_UART_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define Wire_UART_RX_STS_BREAK_SHIFT             (0x01u)
    #define Wire_UART_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define Wire_UART_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define Wire_UART_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define Wire_UART_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define Wire_UART_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define Wire_UART_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define Wire_UART_RX_STS_MRKSPC           (uint8)(0x01u << Wire_UART_RX_STS_MRKSPC_SHIFT)
    #define Wire_UART_RX_STS_BREAK            (uint8)(0x01u << Wire_UART_RX_STS_BREAK_SHIFT)
    #define Wire_UART_RX_STS_PAR_ERROR        (uint8)(0x01u << Wire_UART_RX_STS_PAR_ERROR_SHIFT)
    #define Wire_UART_RX_STS_STOP_ERROR       (uint8)(0x01u << Wire_UART_RX_STS_STOP_ERROR_SHIFT)
    #define Wire_UART_RX_STS_OVERRUN          (uint8)(0x01u << Wire_UART_RX_STS_OVERRUN_SHIFT)
    #define Wire_UART_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << Wire_UART_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define Wire_UART_RX_STS_ADDR_MATCH       (uint8)(0x01u << Wire_UART_RX_STS_ADDR_MATCH_SHIFT)
    #define Wire_UART_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << Wire_UART_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define Wire_UART_RX_HW_MASK                     (0x7Fu)
#endif /* End (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */

/* Control Register definitions */
#define Wire_UART_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define Wire_UART_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define Wire_UART_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define Wire_UART_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define Wire_UART_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define Wire_UART_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define Wire_UART_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define Wire_UART_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define Wire_UART_CTRL_HD_SEND               (uint8)(0x01u << Wire_UART_CTRL_HD_SEND_SHIFT)
#define Wire_UART_CTRL_HD_SEND_BREAK         (uint8)(0x01u << Wire_UART_CTRL_HD_SEND_BREAK_SHIFT)
#define Wire_UART_CTRL_MARK                  (uint8)(0x01u << Wire_UART_CTRL_MARK_SHIFT)
#define Wire_UART_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << Wire_UART_CTRL_PARITY_TYPE0_SHIFT)
#define Wire_UART_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << Wire_UART_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define Wire_UART_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define Wire_UART_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define Wire_UART_SEND_BREAK                         (0x00u)
#define Wire_UART_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define Wire_UART_REINIT                             (0x02u)
#define Wire_UART_SEND_WAIT_REINIT                   (0x03u)

#define Wire_UART_OVER_SAMPLE_8                      (8u)
#define Wire_UART_OVER_SAMPLE_16                     (16u)

#define Wire_UART_BIT_CENTER                         (Wire_UART_OVER_SAMPLE_COUNT - 2u)

#define Wire_UART_FIFO_LENGTH                        (4u)
#define Wire_UART_NUMBER_OF_START_BIT                (1u)
#define Wire_UART_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define Wire_UART_TXBITCTR_BREAKBITS8X   ((Wire_UART_BREAK_BITS_TX * Wire_UART_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define Wire_UART_TXBITCTR_BREAKBITS ((Wire_UART_BREAK_BITS_TX * Wire_UART_OVER_SAMPLE_COUNT) - 1u)

#define Wire_UART_HALF_BIT_COUNT   \
                            (((Wire_UART_OVER_SAMPLE_COUNT / 2u) + (Wire_UART_USE23POLLING * 1u)) - 2u)
#if (Wire_UART_OVER_SAMPLE_COUNT == Wire_UART_OVER_SAMPLE_8)
    #define Wire_UART_HD_TXBITCTR_INIT   (((Wire_UART_BREAK_BITS_TX + \
                            Wire_UART_NUMBER_OF_START_BIT) * Wire_UART_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define Wire_UART_RXBITCTR_INIT  ((((Wire_UART_BREAK_BITS_RX + Wire_UART_NUMBER_OF_START_BIT) \
                            * Wire_UART_OVER_SAMPLE_COUNT) + Wire_UART_HALF_BIT_COUNT) - 1u)

#else /* Wire_UART_OVER_SAMPLE_COUNT == Wire_UART_OVER_SAMPLE_16 */
    #define Wire_UART_HD_TXBITCTR_INIT   ((8u * Wire_UART_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define Wire_UART_RXBITCTR_INIT      (((7u * Wire_UART_OVER_SAMPLE_COUNT) - 1u) + \
                                                      Wire_UART_HALF_BIT_COUNT)
#endif /* End Wire_UART_OVER_SAMPLE_COUNT */

#define Wire_UART_HD_RXBITCTR_INIT                   Wire_UART_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 Wire_UART_initVar;
#if (Wire_UART_TX_INTERRUPT_ENABLED && Wire_UART_TX_ENABLED)
    extern volatile uint8 Wire_UART_txBuffer[Wire_UART_TX_BUFFER_SIZE];
    extern volatile uint8 Wire_UART_txBufferRead;
    extern uint8 Wire_UART_txBufferWrite;
#endif /* (Wire_UART_TX_INTERRUPT_ENABLED && Wire_UART_TX_ENABLED) */
#if (Wire_UART_RX_INTERRUPT_ENABLED && (Wire_UART_RX_ENABLED || Wire_UART_HD_ENABLED))
    extern uint8 Wire_UART_errorStatus;
    extern volatile uint8 Wire_UART_rxBuffer[Wire_UART_RX_BUFFER_SIZE];
    extern volatile uint8 Wire_UART_rxBufferRead;
    extern volatile uint8 Wire_UART_rxBufferWrite;
    extern volatile uint8 Wire_UART_rxBufferLoopDetect;
    extern volatile uint8 Wire_UART_rxBufferOverflow;
    #if (Wire_UART_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 Wire_UART_rxAddressMode;
        extern volatile uint8 Wire_UART_rxAddressDetected;
    #endif /* (Wire_UART_RXHW_ADDRESS_ENABLED) */
#endif /* (Wire_UART_RX_INTERRUPT_ENABLED && (Wire_UART_RX_ENABLED || Wire_UART_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define Wire_UART__B_UART__AM_SW_BYTE_BYTE 1
#define Wire_UART__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define Wire_UART__B_UART__AM_HW_BYTE_BY_BYTE 3
#define Wire_UART__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define Wire_UART__B_UART__AM_NONE 0

#define Wire_UART__B_UART__NONE_REVB 0
#define Wire_UART__B_UART__EVEN_REVB 1
#define Wire_UART__B_UART__ODD_REVB 2
#define Wire_UART__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define Wire_UART_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define Wire_UART_NUMBER_OF_STOP_BITS    (1u)

#if (Wire_UART_RXHW_ADDRESS_ENABLED)
    #define Wire_UART_RX_ADDRESS_MODE    (0u)
    #define Wire_UART_RX_HW_ADDRESS1     (0u)
    #define Wire_UART_RX_HW_ADDRESS2     (0u)
#endif /* (Wire_UART_RXHW_ADDRESS_ENABLED) */

#define Wire_UART_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << Wire_UART_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_BREAK_SHIFT) \
                                        | (0 << Wire_UART_RX_STS_OVERRUN_SHIFT))

#define Wire_UART_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << Wire_UART_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << Wire_UART_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << Wire_UART_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << Wire_UART_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define Wire_UART_CONTROL_REG \
                            (* (reg8 *) Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define Wire_UART_CONTROL_PTR \
                            (  (reg8 *) Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(Wire_UART_TX_ENABLED)
    #define Wire_UART_TXDATA_REG          (* (reg8 *) Wire_UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define Wire_UART_TXDATA_PTR          (  (reg8 *) Wire_UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define Wire_UART_TXDATA_AUX_CTL_REG  (* (reg8 *) Wire_UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define Wire_UART_TXDATA_AUX_CTL_PTR  (  (reg8 *) Wire_UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define Wire_UART_TXSTATUS_REG        (* (reg8 *) Wire_UART_BUART_sTX_TxSts__STATUS_REG)
    #define Wire_UART_TXSTATUS_PTR        (  (reg8 *) Wire_UART_BUART_sTX_TxSts__STATUS_REG)
    #define Wire_UART_TXSTATUS_MASK_REG   (* (reg8 *) Wire_UART_BUART_sTX_TxSts__MASK_REG)
    #define Wire_UART_TXSTATUS_MASK_PTR   (  (reg8 *) Wire_UART_BUART_sTX_TxSts__MASK_REG)
    #define Wire_UART_TXSTATUS_ACTL_REG   (* (reg8 *) Wire_UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define Wire_UART_TXSTATUS_ACTL_PTR   (  (reg8 *) Wire_UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(Wire_UART_TXCLKGEN_DP)
        #define Wire_UART_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define Wire_UART_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define Wire_UART_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define Wire_UART_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define Wire_UART_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define Wire_UART_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define Wire_UART_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define Wire_UART_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define Wire_UART_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define Wire_UART_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) Wire_UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* Wire_UART_TXCLKGEN_DP */

#endif /* End Wire_UART_TX_ENABLED */

#if(Wire_UART_HD_ENABLED)

    #define Wire_UART_TXDATA_REG             (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define Wire_UART_TXDATA_PTR             (  (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define Wire_UART_TXDATA_AUX_CTL_REG     (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define Wire_UART_TXDATA_AUX_CTL_PTR     (  (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define Wire_UART_TXSTATUS_REG           (* (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_REG )
    #define Wire_UART_TXSTATUS_PTR           (  (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_REG )
    #define Wire_UART_TXSTATUS_MASK_REG      (* (reg8 *) Wire_UART_BUART_sRX_RxSts__MASK_REG )
    #define Wire_UART_TXSTATUS_MASK_PTR      (  (reg8 *) Wire_UART_BUART_sRX_RxSts__MASK_REG )
    #define Wire_UART_TXSTATUS_ACTL_REG      (* (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define Wire_UART_TXSTATUS_ACTL_PTR      (  (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End Wire_UART_HD_ENABLED */

#if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )
    #define Wire_UART_RXDATA_REG             (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define Wire_UART_RXDATA_PTR             (  (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define Wire_UART_RXADDRESS1_REG         (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define Wire_UART_RXADDRESS1_PTR         (  (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define Wire_UART_RXADDRESS2_REG         (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define Wire_UART_RXADDRESS2_PTR         (  (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define Wire_UART_RXDATA_AUX_CTL_REG     (* (reg8 *) Wire_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define Wire_UART_RXBITCTR_PERIOD_REG    (* (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define Wire_UART_RXBITCTR_PERIOD_PTR    (  (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define Wire_UART_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define Wire_UART_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define Wire_UART_RXBITCTR_COUNTER_REG   (* (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__COUNT_REG )
    #define Wire_UART_RXBITCTR_COUNTER_PTR   (  (reg8 *) Wire_UART_BUART_sRX_RxBitCounter__COUNT_REG )

    #define Wire_UART_RXSTATUS_REG           (* (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_REG )
    #define Wire_UART_RXSTATUS_PTR           (  (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_REG )
    #define Wire_UART_RXSTATUS_MASK_REG      (* (reg8 *) Wire_UART_BUART_sRX_RxSts__MASK_REG )
    #define Wire_UART_RXSTATUS_MASK_PTR      (  (reg8 *) Wire_UART_BUART_sRX_RxSts__MASK_REG )
    #define Wire_UART_RXSTATUS_ACTL_REG      (* (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define Wire_UART_RXSTATUS_ACTL_PTR      (  (reg8 *) Wire_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */

#if(Wire_UART_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define Wire_UART_INTCLOCK_CLKEN_REG     (* (reg8 *) Wire_UART_IntClock__PM_ACT_CFG)
    #define Wire_UART_INTCLOCK_CLKEN_PTR     (  (reg8 *) Wire_UART_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define Wire_UART_INTCLOCK_CLKEN_MASK    Wire_UART_IntClock__PM_ACT_MSK
#endif /* End Wire_UART_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(Wire_UART_TX_ENABLED)
    #define Wire_UART_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End Wire_UART_TX_ENABLED */

#if(Wire_UART_HD_ENABLED)
    #define Wire_UART_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End Wire_UART_HD_ENABLED */

#if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )
    #define Wire_UART_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define Wire_UART_WAIT_1_MS      Wire_UART_BL_CHK_DELAY_MS   

#define Wire_UART_TXBUFFERSIZE   Wire_UART_TX_BUFFER_SIZE
#define Wire_UART_RXBUFFERSIZE   Wire_UART_RX_BUFFER_SIZE

#if (Wire_UART_RXHW_ADDRESS_ENABLED)
    #define Wire_UART_RXADDRESSMODE  Wire_UART_RX_ADDRESS_MODE
    #define Wire_UART_RXHWADDRESS1   Wire_UART_RX_HW_ADDRESS1
    #define Wire_UART_RXHWADDRESS2   Wire_UART_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define Wire_UART_RXAddressMode  Wire_UART_RXADDRESSMODE
#endif /* (Wire_UART_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define Wire_UART_initvar                    Wire_UART_initVar

#define Wire_UART_RX_Enabled                 Wire_UART_RX_ENABLED
#define Wire_UART_TX_Enabled                 Wire_UART_TX_ENABLED
#define Wire_UART_HD_Enabled                 Wire_UART_HD_ENABLED
#define Wire_UART_RX_IntInterruptEnabled     Wire_UART_RX_INTERRUPT_ENABLED
#define Wire_UART_TX_IntInterruptEnabled     Wire_UART_TX_INTERRUPT_ENABLED
#define Wire_UART_InternalClockUsed          Wire_UART_INTERNAL_CLOCK_USED
#define Wire_UART_RXHW_Address_Enabled       Wire_UART_RXHW_ADDRESS_ENABLED
#define Wire_UART_OverSampleCount            Wire_UART_OVER_SAMPLE_COUNT
#define Wire_UART_ParityType                 Wire_UART_PARITY_TYPE

#if( Wire_UART_TX_ENABLED && (Wire_UART_TXBUFFERSIZE > Wire_UART_FIFO_LENGTH))
    #define Wire_UART_TXBUFFER               Wire_UART_txBuffer
    #define Wire_UART_TXBUFFERREAD           Wire_UART_txBufferRead
    #define Wire_UART_TXBUFFERWRITE          Wire_UART_txBufferWrite
#endif /* End Wire_UART_TX_ENABLED */
#if( ( Wire_UART_RX_ENABLED || Wire_UART_HD_ENABLED ) && \
     (Wire_UART_RXBUFFERSIZE > Wire_UART_FIFO_LENGTH) )
    #define Wire_UART_RXBUFFER               Wire_UART_rxBuffer
    #define Wire_UART_RXBUFFERREAD           Wire_UART_rxBufferRead
    #define Wire_UART_RXBUFFERWRITE          Wire_UART_rxBufferWrite
    #define Wire_UART_RXBUFFERLOOPDETECT     Wire_UART_rxBufferLoopDetect
    #define Wire_UART_RXBUFFER_OVERFLOW      Wire_UART_rxBufferOverflow
#endif /* End Wire_UART_RX_ENABLED */

#ifdef Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define Wire_UART_CONTROL                Wire_UART_CONTROL_REG
#endif /* End Wire_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(Wire_UART_TX_ENABLED)
    #define Wire_UART_TXDATA                 Wire_UART_TXDATA_REG
    #define Wire_UART_TXSTATUS               Wire_UART_TXSTATUS_REG
    #define Wire_UART_TXSTATUS_MASK          Wire_UART_TXSTATUS_MASK_REG
    #define Wire_UART_TXSTATUS_ACTL          Wire_UART_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(Wire_UART_TXCLKGEN_DP)
        #define Wire_UART_TXBITCLKGEN_CTR        Wire_UART_TXBITCLKGEN_CTR_REG
        #define Wire_UART_TXBITCLKTX_COMPLETE    Wire_UART_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define Wire_UART_TXBITCTR_PERIOD        Wire_UART_TXBITCTR_PERIOD_REG
        #define Wire_UART_TXBITCTR_CONTROL       Wire_UART_TXBITCTR_CONTROL_REG
        #define Wire_UART_TXBITCTR_COUNTER       Wire_UART_TXBITCTR_COUNTER_REG
    #endif /* Wire_UART_TXCLKGEN_DP */
#endif /* End Wire_UART_TX_ENABLED */

#if(Wire_UART_HD_ENABLED)
    #define Wire_UART_TXDATA                 Wire_UART_TXDATA_REG
    #define Wire_UART_TXSTATUS               Wire_UART_TXSTATUS_REG
    #define Wire_UART_TXSTATUS_MASK          Wire_UART_TXSTATUS_MASK_REG
    #define Wire_UART_TXSTATUS_ACTL          Wire_UART_TXSTATUS_ACTL_REG
#endif /* End Wire_UART_HD_ENABLED */

#if( (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) )
    #define Wire_UART_RXDATA                 Wire_UART_RXDATA_REG
    #define Wire_UART_RXADDRESS1             Wire_UART_RXADDRESS1_REG
    #define Wire_UART_RXADDRESS2             Wire_UART_RXADDRESS2_REG
    #define Wire_UART_RXBITCTR_PERIOD        Wire_UART_RXBITCTR_PERIOD_REG
    #define Wire_UART_RXBITCTR_CONTROL       Wire_UART_RXBITCTR_CONTROL_REG
    #define Wire_UART_RXBITCTR_COUNTER       Wire_UART_RXBITCTR_COUNTER_REG
    #define Wire_UART_RXSTATUS               Wire_UART_RXSTATUS_REG
    #define Wire_UART_RXSTATUS_MASK          Wire_UART_RXSTATUS_MASK_REG
    #define Wire_UART_RXSTATUS_ACTL          Wire_UART_RXSTATUS_ACTL_REG
#endif /* End  (Wire_UART_RX_ENABLED) || (Wire_UART_HD_ENABLED) */

#if(Wire_UART_INTERNAL_CLOCK_USED)
    #define Wire_UART_INTCLOCK_CLKEN         Wire_UART_INTCLOCK_CLKEN_REG
#endif /* End Wire_UART_INTERNAL_CLOCK_USED */

#define Wire_UART_WAIT_FOR_COMLETE_REINIT    Wire_UART_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_Wire_UART_H */


/* [] END OF FILE */

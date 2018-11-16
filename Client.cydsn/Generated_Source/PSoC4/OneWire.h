/*******************************************************************************
* File Name: OneWire.h
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


#if !defined(CY_UART_OneWire_H)
#define CY_UART_OneWire_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define OneWire_RX_ENABLED                     (1u)
#define OneWire_TX_ENABLED                     (1u)
#define OneWire_HD_ENABLED                     (0u)
#define OneWire_RX_INTERRUPT_ENABLED           (0u)
#define OneWire_TX_INTERRUPT_ENABLED           (0u)
#define OneWire_INTERNAL_CLOCK_USED            (0u)
#define OneWire_RXHW_ADDRESS_ENABLED           (0u)
#define OneWire_OVER_SAMPLE_COUNT              (8u)
#define OneWire_PARITY_TYPE                    (0u)
#define OneWire_PARITY_TYPE_SW                 (0u)
#define OneWire_BREAK_DETECT                   (0u)
#define OneWire_BREAK_BITS_TX                  (13u)
#define OneWire_BREAK_BITS_RX                  (13u)
#define OneWire_TXCLKGEN_DP                    (1u)
#define OneWire_USE23POLLING                   (1u)
#define OneWire_FLOW_CONTROL                   (0u)
#define OneWire_CLK_FREQ                       (0u)
#define OneWire_TX_BUFFER_SIZE                 (4u)
#define OneWire_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define OneWire_CONTROL_REG_REMOVED            (0u)
#else
    #define OneWire_CONTROL_REG_REMOVED            (1u)
#endif /* End OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct OneWire_backupStruct_
{
    uint8 enableState;

    #if(OneWire_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End OneWire_CONTROL_REG_REMOVED */

} OneWire_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void OneWire_Start(void) ;
void OneWire_Stop(void) ;
uint8 OneWire_ReadControlRegister(void) ;
void OneWire_WriteControlRegister(uint8 control) ;

void OneWire_Init(void) ;
void OneWire_Enable(void) ;
void OneWire_SaveConfig(void) ;
void OneWire_RestoreConfig(void) ;
void OneWire_Sleep(void) ;
void OneWire_Wakeup(void) ;

/* Only if RX is enabled */
#if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )

    #if (OneWire_RX_INTERRUPT_ENABLED)
        #define OneWire_EnableRxInt()  CyIntEnable (OneWire_RX_VECT_NUM)
        #define OneWire_DisableRxInt() CyIntDisable(OneWire_RX_VECT_NUM)
        CY_ISR_PROTO(OneWire_RXISR);
    #endif /* OneWire_RX_INTERRUPT_ENABLED */

    void OneWire_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void OneWire_SetRxAddress1(uint8 address) ;
    void OneWire_SetRxAddress2(uint8 address) ;

    void  OneWire_SetRxInterruptMode(uint8 intSrc) ;
    uint8 OneWire_ReadRxData(void) ;
    uint8 OneWire_ReadRxStatus(void) ;
    uint8 OneWire_GetChar(void) ;
    uint16 OneWire_GetByte(void) ;
    uint8 OneWire_GetRxBufferSize(void)
                                                            ;
    void OneWire_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define OneWire_GetRxInterruptSource   OneWire_ReadRxStatus

#endif /* End (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */

/* Only if TX is enabled */
#if(OneWire_TX_ENABLED || OneWire_HD_ENABLED)

    #if(OneWire_TX_INTERRUPT_ENABLED)
        #define OneWire_EnableTxInt()  CyIntEnable (OneWire_TX_VECT_NUM)
        #define OneWire_DisableTxInt() CyIntDisable(OneWire_TX_VECT_NUM)
        #define OneWire_SetPendingTxInt() CyIntSetPending(OneWire_TX_VECT_NUM)
        #define OneWire_ClearPendingTxInt() CyIntClearPending(OneWire_TX_VECT_NUM)
        CY_ISR_PROTO(OneWire_TXISR);
    #endif /* OneWire_TX_INTERRUPT_ENABLED */

    void OneWire_SetTxInterruptMode(uint8 intSrc) ;
    void OneWire_WriteTxData(uint8 txDataByte) ;
    uint8 OneWire_ReadTxStatus(void) ;
    void OneWire_PutChar(uint8 txDataByte) ;
    void OneWire_PutString(const char8 string[]) ;
    void OneWire_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void OneWire_PutCRLF(uint8 txDataByte) ;
    void OneWire_ClearTxBuffer(void) ;
    void OneWire_SetTxAddressMode(uint8 addressMode) ;
    void OneWire_SendBreak(uint8 retMode) ;
    uint8 OneWire_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define OneWire_PutStringConst         OneWire_PutString
    #define OneWire_PutArrayConst          OneWire_PutArray
    #define OneWire_GetTxInterruptSource   OneWire_ReadTxStatus

#endif /* End OneWire_TX_ENABLED || OneWire_HD_ENABLED */

#if(OneWire_HD_ENABLED)
    void OneWire_LoadRxConfig(void) ;
    void OneWire_LoadTxConfig(void) ;
#endif /* End OneWire_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OneWire) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    OneWire_CyBtldrCommStart(void) CYSMALL ;
    void    OneWire_CyBtldrCommStop(void) CYSMALL ;
    void    OneWire_CyBtldrCommReset(void) CYSMALL ;
    cystatus OneWire_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus OneWire_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OneWire)
        #define CyBtldrCommStart    OneWire_CyBtldrCommStart
        #define CyBtldrCommStop     OneWire_CyBtldrCommStop
        #define CyBtldrCommReset    OneWire_CyBtldrCommReset
        #define CyBtldrCommWrite    OneWire_CyBtldrCommWrite
        #define CyBtldrCommRead     OneWire_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_OneWire) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define OneWire_BYTE2BYTE_TIME_OUT (25u)
    #define OneWire_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define OneWire_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define OneWire_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define OneWire_SET_SPACE      (0x00u)
#define OneWire_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (OneWire_TX_ENABLED) || (OneWire_HD_ENABLED) )
    #if(OneWire_TX_INTERRUPT_ENABLED)
        #define OneWire_TX_VECT_NUM            (uint8)OneWire_TXInternalInterrupt__INTC_NUMBER
        #define OneWire_TX_PRIOR_NUM           (uint8)OneWire_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* OneWire_TX_INTERRUPT_ENABLED */

    #define OneWire_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define OneWire_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define OneWire_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(OneWire_TX_ENABLED)
        #define OneWire_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (OneWire_HD_ENABLED) */
        #define OneWire_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (OneWire_TX_ENABLED) */

    #define OneWire_TX_STS_COMPLETE            (uint8)(0x01u << OneWire_TX_STS_COMPLETE_SHIFT)
    #define OneWire_TX_STS_FIFO_EMPTY          (uint8)(0x01u << OneWire_TX_STS_FIFO_EMPTY_SHIFT)
    #define OneWire_TX_STS_FIFO_FULL           (uint8)(0x01u << OneWire_TX_STS_FIFO_FULL_SHIFT)
    #define OneWire_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << OneWire_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (OneWire_TX_ENABLED) || (OneWire_HD_ENABLED)*/

#if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )
    #if(OneWire_RX_INTERRUPT_ENABLED)
        #define OneWire_RX_VECT_NUM            (uint8)OneWire_RXInternalInterrupt__INTC_NUMBER
        #define OneWire_RX_PRIOR_NUM           (uint8)OneWire_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* OneWire_RX_INTERRUPT_ENABLED */
    #define OneWire_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define OneWire_RX_STS_BREAK_SHIFT             (0x01u)
    #define OneWire_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define OneWire_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define OneWire_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define OneWire_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define OneWire_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define OneWire_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define OneWire_RX_STS_MRKSPC           (uint8)(0x01u << OneWire_RX_STS_MRKSPC_SHIFT)
    #define OneWire_RX_STS_BREAK            (uint8)(0x01u << OneWire_RX_STS_BREAK_SHIFT)
    #define OneWire_RX_STS_PAR_ERROR        (uint8)(0x01u << OneWire_RX_STS_PAR_ERROR_SHIFT)
    #define OneWire_RX_STS_STOP_ERROR       (uint8)(0x01u << OneWire_RX_STS_STOP_ERROR_SHIFT)
    #define OneWire_RX_STS_OVERRUN          (uint8)(0x01u << OneWire_RX_STS_OVERRUN_SHIFT)
    #define OneWire_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << OneWire_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define OneWire_RX_STS_ADDR_MATCH       (uint8)(0x01u << OneWire_RX_STS_ADDR_MATCH_SHIFT)
    #define OneWire_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << OneWire_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define OneWire_RX_HW_MASK                     (0x7Fu)
#endif /* End (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */

/* Control Register definitions */
#define OneWire_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define OneWire_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define OneWire_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define OneWire_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define OneWire_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define OneWire_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define OneWire_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define OneWire_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define OneWire_CTRL_HD_SEND               (uint8)(0x01u << OneWire_CTRL_HD_SEND_SHIFT)
#define OneWire_CTRL_HD_SEND_BREAK         (uint8)(0x01u << OneWire_CTRL_HD_SEND_BREAK_SHIFT)
#define OneWire_CTRL_MARK                  (uint8)(0x01u << OneWire_CTRL_MARK_SHIFT)
#define OneWire_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << OneWire_CTRL_PARITY_TYPE0_SHIFT)
#define OneWire_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << OneWire_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define OneWire_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define OneWire_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define OneWire_SEND_BREAK                         (0x00u)
#define OneWire_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define OneWire_REINIT                             (0x02u)
#define OneWire_SEND_WAIT_REINIT                   (0x03u)

#define OneWire_OVER_SAMPLE_8                      (8u)
#define OneWire_OVER_SAMPLE_16                     (16u)

#define OneWire_BIT_CENTER                         (OneWire_OVER_SAMPLE_COUNT - 2u)

#define OneWire_FIFO_LENGTH                        (4u)
#define OneWire_NUMBER_OF_START_BIT                (1u)
#define OneWire_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define OneWire_TXBITCTR_BREAKBITS8X   ((OneWire_BREAK_BITS_TX * OneWire_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define OneWire_TXBITCTR_BREAKBITS ((OneWire_BREAK_BITS_TX * OneWire_OVER_SAMPLE_COUNT) - 1u)

#define OneWire_HALF_BIT_COUNT   \
                            (((OneWire_OVER_SAMPLE_COUNT / 2u) + (OneWire_USE23POLLING * 1u)) - 2u)
#if (OneWire_OVER_SAMPLE_COUNT == OneWire_OVER_SAMPLE_8)
    #define OneWire_HD_TXBITCTR_INIT   (((OneWire_BREAK_BITS_TX + \
                            OneWire_NUMBER_OF_START_BIT) * OneWire_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define OneWire_RXBITCTR_INIT  ((((OneWire_BREAK_BITS_RX + OneWire_NUMBER_OF_START_BIT) \
                            * OneWire_OVER_SAMPLE_COUNT) + OneWire_HALF_BIT_COUNT) - 1u)

#else /* OneWire_OVER_SAMPLE_COUNT == OneWire_OVER_SAMPLE_16 */
    #define OneWire_HD_TXBITCTR_INIT   ((8u * OneWire_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define OneWire_RXBITCTR_INIT      (((7u * OneWire_OVER_SAMPLE_COUNT) - 1u) + \
                                                      OneWire_HALF_BIT_COUNT)
#endif /* End OneWire_OVER_SAMPLE_COUNT */

#define OneWire_HD_RXBITCTR_INIT                   OneWire_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 OneWire_initVar;
#if (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED)
    extern volatile uint8 OneWire_txBuffer[OneWire_TX_BUFFER_SIZE];
    extern volatile uint8 OneWire_txBufferRead;
    extern uint8 OneWire_txBufferWrite;
#endif /* (OneWire_TX_INTERRUPT_ENABLED && OneWire_TX_ENABLED) */
#if (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED))
    extern uint8 OneWire_errorStatus;
    extern volatile uint8 OneWire_rxBuffer[OneWire_RX_BUFFER_SIZE];
    extern volatile uint8 OneWire_rxBufferRead;
    extern volatile uint8 OneWire_rxBufferWrite;
    extern volatile uint8 OneWire_rxBufferLoopDetect;
    extern volatile uint8 OneWire_rxBufferOverflow;
    #if (OneWire_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 OneWire_rxAddressMode;
        extern volatile uint8 OneWire_rxAddressDetected;
    #endif /* (OneWire_RXHW_ADDRESS_ENABLED) */
#endif /* (OneWire_RX_INTERRUPT_ENABLED && (OneWire_RX_ENABLED || OneWire_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define OneWire__B_UART__AM_SW_BYTE_BYTE 1
#define OneWire__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define OneWire__B_UART__AM_HW_BYTE_BY_BYTE 3
#define OneWire__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define OneWire__B_UART__AM_NONE 0

#define OneWire__B_UART__NONE_REVB 0
#define OneWire__B_UART__EVEN_REVB 1
#define OneWire__B_UART__ODD_REVB 2
#define OneWire__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define OneWire_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define OneWire_NUMBER_OF_STOP_BITS    (1u)

#if (OneWire_RXHW_ADDRESS_ENABLED)
    #define OneWire_RX_ADDRESS_MODE    (0u)
    #define OneWire_RX_HW_ADDRESS1     (0u)
    #define OneWire_RX_HW_ADDRESS2     (0u)
#endif /* (OneWire_RXHW_ADDRESS_ENABLED) */

#define OneWire_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << OneWire_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << OneWire_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << OneWire_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << OneWire_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << OneWire_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << OneWire_RX_STS_BREAK_SHIFT) \
                                        | (0 << OneWire_RX_STS_OVERRUN_SHIFT))

#define OneWire_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << OneWire_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << OneWire_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << OneWire_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << OneWire_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define OneWire_CONTROL_REG \
                            (* (reg8 *) OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define OneWire_CONTROL_PTR \
                            (  (reg8 *) OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(OneWire_TX_ENABLED)
    #define OneWire_TXDATA_REG          (* (reg8 *) OneWire_BUART_sTX_TxShifter_u0__F0_REG)
    #define OneWire_TXDATA_PTR          (  (reg8 *) OneWire_BUART_sTX_TxShifter_u0__F0_REG)
    #define OneWire_TXDATA_AUX_CTL_REG  (* (reg8 *) OneWire_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define OneWire_TXDATA_AUX_CTL_PTR  (  (reg8 *) OneWire_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define OneWire_TXSTATUS_REG        (* (reg8 *) OneWire_BUART_sTX_TxSts__STATUS_REG)
    #define OneWire_TXSTATUS_PTR        (  (reg8 *) OneWire_BUART_sTX_TxSts__STATUS_REG)
    #define OneWire_TXSTATUS_MASK_REG   (* (reg8 *) OneWire_BUART_sTX_TxSts__MASK_REG)
    #define OneWire_TXSTATUS_MASK_PTR   (  (reg8 *) OneWire_BUART_sTX_TxSts__MASK_REG)
    #define OneWire_TXSTATUS_ACTL_REG   (* (reg8 *) OneWire_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define OneWire_TXSTATUS_ACTL_PTR   (  (reg8 *) OneWire_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(OneWire_TXCLKGEN_DP)
        #define OneWire_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define OneWire_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define OneWire_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define OneWire_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define OneWire_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define OneWire_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define OneWire_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define OneWire_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define OneWire_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define OneWire_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) OneWire_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* OneWire_TXCLKGEN_DP */

#endif /* End OneWire_TX_ENABLED */

#if(OneWire_HD_ENABLED)

    #define OneWire_TXDATA_REG             (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__F1_REG )
    #define OneWire_TXDATA_PTR             (  (reg8 *) OneWire_BUART_sRX_RxShifter_u0__F1_REG )
    #define OneWire_TXDATA_AUX_CTL_REG     (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define OneWire_TXDATA_AUX_CTL_PTR     (  (reg8 *) OneWire_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define OneWire_TXSTATUS_REG           (* (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_REG )
    #define OneWire_TXSTATUS_PTR           (  (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_REG )
    #define OneWire_TXSTATUS_MASK_REG      (* (reg8 *) OneWire_BUART_sRX_RxSts__MASK_REG )
    #define OneWire_TXSTATUS_MASK_PTR      (  (reg8 *) OneWire_BUART_sRX_RxSts__MASK_REG )
    #define OneWire_TXSTATUS_ACTL_REG      (* (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define OneWire_TXSTATUS_ACTL_PTR      (  (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End OneWire_HD_ENABLED */

#if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )
    #define OneWire_RXDATA_REG             (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__F0_REG )
    #define OneWire_RXDATA_PTR             (  (reg8 *) OneWire_BUART_sRX_RxShifter_u0__F0_REG )
    #define OneWire_RXADDRESS1_REG         (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__D0_REG )
    #define OneWire_RXADDRESS1_PTR         (  (reg8 *) OneWire_BUART_sRX_RxShifter_u0__D0_REG )
    #define OneWire_RXADDRESS2_REG         (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__D1_REG )
    #define OneWire_RXADDRESS2_PTR         (  (reg8 *) OneWire_BUART_sRX_RxShifter_u0__D1_REG )
    #define OneWire_RXDATA_AUX_CTL_REG     (* (reg8 *) OneWire_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define OneWire_RXBITCTR_PERIOD_REG    (* (reg8 *) OneWire_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define OneWire_RXBITCTR_PERIOD_PTR    (  (reg8 *) OneWire_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define OneWire_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) OneWire_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define OneWire_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) OneWire_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define OneWire_RXBITCTR_COUNTER_REG   (* (reg8 *) OneWire_BUART_sRX_RxBitCounter__COUNT_REG )
    #define OneWire_RXBITCTR_COUNTER_PTR   (  (reg8 *) OneWire_BUART_sRX_RxBitCounter__COUNT_REG )

    #define OneWire_RXSTATUS_REG           (* (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_REG )
    #define OneWire_RXSTATUS_PTR           (  (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_REG )
    #define OneWire_RXSTATUS_MASK_REG      (* (reg8 *) OneWire_BUART_sRX_RxSts__MASK_REG )
    #define OneWire_RXSTATUS_MASK_PTR      (  (reg8 *) OneWire_BUART_sRX_RxSts__MASK_REG )
    #define OneWire_RXSTATUS_ACTL_REG      (* (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define OneWire_RXSTATUS_ACTL_PTR      (  (reg8 *) OneWire_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */

#if(OneWire_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define OneWire_INTCLOCK_CLKEN_REG     (* (reg8 *) OneWire_IntClock__PM_ACT_CFG)
    #define OneWire_INTCLOCK_CLKEN_PTR     (  (reg8 *) OneWire_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define OneWire_INTCLOCK_CLKEN_MASK    OneWire_IntClock__PM_ACT_MSK
#endif /* End OneWire_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(OneWire_TX_ENABLED)
    #define OneWire_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End OneWire_TX_ENABLED */

#if(OneWire_HD_ENABLED)
    #define OneWire_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End OneWire_HD_ENABLED */

#if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )
    #define OneWire_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define OneWire_WAIT_1_MS      OneWire_BL_CHK_DELAY_MS   

#define OneWire_TXBUFFERSIZE   OneWire_TX_BUFFER_SIZE
#define OneWire_RXBUFFERSIZE   OneWire_RX_BUFFER_SIZE

#if (OneWire_RXHW_ADDRESS_ENABLED)
    #define OneWire_RXADDRESSMODE  OneWire_RX_ADDRESS_MODE
    #define OneWire_RXHWADDRESS1   OneWire_RX_HW_ADDRESS1
    #define OneWire_RXHWADDRESS2   OneWire_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define OneWire_RXAddressMode  OneWire_RXADDRESSMODE
#endif /* (OneWire_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define OneWire_initvar                    OneWire_initVar

#define OneWire_RX_Enabled                 OneWire_RX_ENABLED
#define OneWire_TX_Enabled                 OneWire_TX_ENABLED
#define OneWire_HD_Enabled                 OneWire_HD_ENABLED
#define OneWire_RX_IntInterruptEnabled     OneWire_RX_INTERRUPT_ENABLED
#define OneWire_TX_IntInterruptEnabled     OneWire_TX_INTERRUPT_ENABLED
#define OneWire_InternalClockUsed          OneWire_INTERNAL_CLOCK_USED
#define OneWire_RXHW_Address_Enabled       OneWire_RXHW_ADDRESS_ENABLED
#define OneWire_OverSampleCount            OneWire_OVER_SAMPLE_COUNT
#define OneWire_ParityType                 OneWire_PARITY_TYPE

#if( OneWire_TX_ENABLED && (OneWire_TXBUFFERSIZE > OneWire_FIFO_LENGTH))
    #define OneWire_TXBUFFER               OneWire_txBuffer
    #define OneWire_TXBUFFERREAD           OneWire_txBufferRead
    #define OneWire_TXBUFFERWRITE          OneWire_txBufferWrite
#endif /* End OneWire_TX_ENABLED */
#if( ( OneWire_RX_ENABLED || OneWire_HD_ENABLED ) && \
     (OneWire_RXBUFFERSIZE > OneWire_FIFO_LENGTH) )
    #define OneWire_RXBUFFER               OneWire_rxBuffer
    #define OneWire_RXBUFFERREAD           OneWire_rxBufferRead
    #define OneWire_RXBUFFERWRITE          OneWire_rxBufferWrite
    #define OneWire_RXBUFFERLOOPDETECT     OneWire_rxBufferLoopDetect
    #define OneWire_RXBUFFER_OVERFLOW      OneWire_rxBufferOverflow
#endif /* End OneWire_RX_ENABLED */

#ifdef OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define OneWire_CONTROL                OneWire_CONTROL_REG
#endif /* End OneWire_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(OneWire_TX_ENABLED)
    #define OneWire_TXDATA                 OneWire_TXDATA_REG
    #define OneWire_TXSTATUS               OneWire_TXSTATUS_REG
    #define OneWire_TXSTATUS_MASK          OneWire_TXSTATUS_MASK_REG
    #define OneWire_TXSTATUS_ACTL          OneWire_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(OneWire_TXCLKGEN_DP)
        #define OneWire_TXBITCLKGEN_CTR        OneWire_TXBITCLKGEN_CTR_REG
        #define OneWire_TXBITCLKTX_COMPLETE    OneWire_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define OneWire_TXBITCTR_PERIOD        OneWire_TXBITCTR_PERIOD_REG
        #define OneWire_TXBITCTR_CONTROL       OneWire_TXBITCTR_CONTROL_REG
        #define OneWire_TXBITCTR_COUNTER       OneWire_TXBITCTR_COUNTER_REG
    #endif /* OneWire_TXCLKGEN_DP */
#endif /* End OneWire_TX_ENABLED */

#if(OneWire_HD_ENABLED)
    #define OneWire_TXDATA                 OneWire_TXDATA_REG
    #define OneWire_TXSTATUS               OneWire_TXSTATUS_REG
    #define OneWire_TXSTATUS_MASK          OneWire_TXSTATUS_MASK_REG
    #define OneWire_TXSTATUS_ACTL          OneWire_TXSTATUS_ACTL_REG
#endif /* End OneWire_HD_ENABLED */

#if( (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) )
    #define OneWire_RXDATA                 OneWire_RXDATA_REG
    #define OneWire_RXADDRESS1             OneWire_RXADDRESS1_REG
    #define OneWire_RXADDRESS2             OneWire_RXADDRESS2_REG
    #define OneWire_RXBITCTR_PERIOD        OneWire_RXBITCTR_PERIOD_REG
    #define OneWire_RXBITCTR_CONTROL       OneWire_RXBITCTR_CONTROL_REG
    #define OneWire_RXBITCTR_COUNTER       OneWire_RXBITCTR_COUNTER_REG
    #define OneWire_RXSTATUS               OneWire_RXSTATUS_REG
    #define OneWire_RXSTATUS_MASK          OneWire_RXSTATUS_MASK_REG
    #define OneWire_RXSTATUS_ACTL          OneWire_RXSTATUS_ACTL_REG
#endif /* End  (OneWire_RX_ENABLED) || (OneWire_HD_ENABLED) */

#if(OneWire_INTERNAL_CLOCK_USED)
    #define OneWire_INTCLOCK_CLKEN         OneWire_INTCLOCK_CLKEN_REG
#endif /* End OneWire_INTERNAL_CLOCK_USED */

#define OneWire_WAIT_FOR_COMLETE_REINIT    OneWire_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_OneWire_H */


/* [] END OF FILE */

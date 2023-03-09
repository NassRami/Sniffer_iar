/*===========================================================================*/
/* Module : rscan.h                                                          */
/* Version: V1.04                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN Macro Definitions                                          */
/*                                                                           */
/*===========================================================================*/
/* In case of any question please do not hesitate to contact:                */
/*                                                                           */
/*        ABG.Support                                                        */
/*                                                                           */
/*        Renesas Electronics Europe GmbH                                    */
/*        Arcadiastrasse 10                                                  */
/*        D-40472 Duesseldorf, Germany                                       */
/*                                                                           */
/*        e-mail: device_support.rl78-eu@lm.renesas.com                      */
/*        FAX:   +49 - (0)211 / 65 03 - 12 79                                */
/*                                                                           */
/*===========================================================================*/
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:          RL78 including RSCAN macro                  */
/*              IDE:             IAR Systems                                 */
/*                               Embedded Workbench for RL78 V1.30.x         */
/*                                                                           */
/*===========================================================================*/
/* History:                                                                  */
/*              V1.00: Initial version                                       */
/*                                                                           */
/*===========================================================================*/


#ifndef _RSCAN_H
#define _RSCAN_H

#include "ree_types.h"

/*===========================================================================*/
/* RSCAN Globals */
#define EE_RSCAN_0       ( 0u )        /* RSCAN macro selection */
#define EE_RSCAN_INVALID ( 0xFFu )     /* no macro assignment */

#define EE_RSCAN_NOIRQ   false         /* status report defines */
#define EE_RSCAN_IRQ     true
#define EE_RSCAN_OK      true
#define EE_RSCAN_ERROR   false

// RSCAN dimensions
#define EE_RSCAN_MAXCHANNELS   	(  8u )  /* max. channels of hardware = f(Macro_Design) */
#define EE_RSCAN_AFLPAGESIZE   	(  1u )  /* common AFL page size */
#define EE_RSCAN_MAXAFLENTRIES 	( 16u )  /* max. entries in AFL total */
#define EE_RSCAN_MAXAFLCHANSET 	( 16u )  /* max. entries in AFL per channel */
#define EE_RSCAN_MAXRXFIFOS    	(  2u )  /* for all channels */
#define EE_RSCAN_MAXRXBUFFERS  	( 16u )  /* for all channels */
#define EE_RSCAN_MAXCOMFIFOS   	(  1u )  /* per channel */
#define EE_RSCAN_MAXTXBUFFERS  	(  4u )  /* per channel */
#define EE_RSCAN_MAXTXQUEUES   	(  0u )  /* per channel */

#define EE_RSCAN_CHANNELS      	(  1u )  /* number of channels */
#define EE_RSCAN_CHANNEL0      	(  0u )  /* RSCAN channels */
#define EE_RSCAN_GLOBAL       ( 0xFFu )  /* for ALL channels */

#define EE_RSCAN_COMREGSBITS	( 16u )	 /* dk_0 */
#define EE_RSCAN_COMTXREGS     	(  1u )
#define EE_RSCAN_COMRXREGS     	(  1u )
#define EE_RSCAN_COMRXREG( N ) 	( ( ( N / EE_RSCAN_COMREGSBITS ) >= 1 ) ? 1 : 0 ) /* dk_0 */
#define EE_RSCAN_COMRXBIT( N ) 	( N % EE_RSCAN_COMREGSBITS ) /* dk_0 */

#define EE_RSCAN_ID_STD        (  0u )
#define EE_RSCAN_ID_EXT        (  1u )
#define EE_RSCAN_ID_MAX        ( 0x1FFFFFFFu )

#define EE_RSCAN_FRAME_DATA    (  0u )
#define EE_RSCAN_FRAME_REMOTE  (  1u )

#define EE_RSCAN_MASK_IDFULLCAN  ( 0xFFFFFFFFu )
#define EE_RSCAN_MASK_IDDONTCARE ( 0x00000000u )
#define EE_RSCAN_MASK_DONTCARE   (  0u )
#define EE_RSCAN_MASK_FILTER     (  1u )

#define EE_RSCAN_DLC_MAX   ( 8u ) // Data Length Code

/* Interrupt Selection */

/* Global vectors */
#define EE_RSCAN_INT_GERR  ( 0x00u )  /* Interrupt vector definition */
#define EE_RSCAN_INT_RXF   ( 0x01u )
#define EE_RSCAN_INT_RXF0  ( 0x02u )
#define EE_RSCAN_INT_RXF1  ( 0x03u )

/* Channel vectors */
#define EE_RSCAN_INT_TX    ( 0x00u )
#define EE_RSCAN_INT_TXA   ( 0x01u )
#define EE_RSCAN_INT_TXQ   ( 0x02u )
#define EE_RSCAN_INT_TXHL  ( 0x03u )
#define EE_RSCAN_INT_TXCF  ( 0x04u )
#define EE_RSCAN_INT_RXCF  ( 0x05u )
#define EE_RSCAN_INT_CERR  ( 0x06u )


/* Interrupt Sub Selection */
#define EE_RSCAN_CINT_OFF         ( 0x000u )
#define EE_RSCAN_CINT_BUSERR      ( 0x001u )
#define EE_RSCAN_CINT_WARNING     ( 0x002u )
#define EE_RSCAN_CINT_PASSIVE     ( 0x004u )
#define EE_RSCAN_CINT_BUSOFF      ( 0x008u )
#define EE_RSCAN_CINT_RECOVERY    ( 0x010u )
#define EE_RSCAN_CINT_OVERLOAD    ( 0x020u )
#define EE_RSCAN_CINT_BUSLOCK     ( 0x040u )
#define EE_RSCAN_CINT_ARBLOST     ( 0x080u )
#define EE_RSCAN_CINT_TXABORT     ( 0x100u )

#define EE_RSCAN_GINT_NONE        ( 0x00u )
#define EE_RSCAN_GINT_DLCCHECK    ( 0x01u )
#define EE_RSCAN_GINT_MSGLOST     ( 0x02u )
#define EE_RSCAN_GINT_THLLOST     ( 0x04u )
#define EE_RSCAN_GINT_RAMPARITY   ( 0x08u )
/* #define EE_RSCAN_GINT_PLLMON      ( 0x10u ) */
/* #define EE_RSCAN_GINT_OTBFIFO     ( 0x20u ) */

#define EE_RSCAN_INT_ENABLE  ( 0x01u )
#define EE_RSCAN_INT_DISABLE ( 0x00u )

#define EE_RSCAN_INT_NOINT ( 0xFFu )


#define EE_RSCAN_INT_GLOBAL  ( 1u + EE_RSCAN_MAXRXFIFOS )
#define EE_RSCAN_INT_CHANNEL ( 7u )

/* Bittiming settings */

#define EE_RSCAN_BT_MAXTQ           ( 25u )
#define EE_RSCAN_BT_MINTQ           (  8u )
#define EE_RSCAN_BT_MINTSEG1        (  4u )
#define EE_RSCAN_BT_MAXTSEG1        ( 16u )
#define EE_RSCAN_BT_MINTSEG2        (  2u )
#define EE_RSCAN_BT_MAXTSEG2        (  8u )
#define EE_RSCAN_BT_MINSJW          (  1u )
#define EE_RSCAN_BT_MAXSJW          (  4u )
#define EE_RSCAN_BT_SYNCSEGLEN      (  1u )

#define EE_RSCAN_BT_MAXBRP          ( 1024u )
#define EE_RSCAN_BT_SPTOPTIMUM      ( 0.75 )

#define EE_RSCAN_TRLAYERFACTOR      (  1u )
/*#define EE_RSCAN_CLKMINFACTOR     ( 8u * EE_RSCAN_CHANNELS ) */
#define EE_RSCAN_CLKMINFACTOR       ( 8u ) /* Min_Frequency / Bitrate */

/* Prescaler settings (FIFO interval, Timestamp) */

#define EE_RSCAN_FIFO_MAXPRS        (   256u )
#define EE_RSCAN_FIFO_MINPRS        (     1u )
#define EE_RSCAN_FIFO_CKPRS         (    10u )
#define EE_RSCAN_TS_MAXPRS          ( 32768u )
#define EE_RSCAN_TS_MINPRS          (     1u )

/* RSCAN Constant Values */

#define EE_RSCAN_SET              ( 1u )
#define EE_RSCAN_CLEAR            ( 0u )

/* Globale Modes         */
#define EE_RSCAN_GLOBAL_OPER	  ( 0x00u )
#define EE_RSCAN_GLOBAL_RST	  ( 0x01u )
#define EE_RSCAN_GLOBAL_TST 	  ( 0x02u ) /* Global halt mode */
#define EE_RSCAN_GLOBAL_SLEEP	  ( 0x03u )

/* Channel Modes         */
#define EE_RSCAN_CH_SLEEP     ( 0x04u ) /* STOP */
#define EE_RSCAN_CH_RESET     ( 0x01u )
#define EE_RSCAN_CH_OPER      ( 0x00u )
#define EE_RSCAN_CH_HALT      ( 0x02u )
#define EE_RSCAN_CH_KEEP      ( 0x03u )

/* Channel Modes         */
#define EE_RSCAN_CH_RST           ( 0x01u )
#define EE_RSCAN_CH_HLT	          ( 0x02u )
#define EE_RSCAN_CH_COM	          ( 0x00u )

#define EE_RSCAN_SLEEP_ENABLE     ( 0x01u )
#define EE_RSCAN_SLEEP_DISABLE    ( 0x00u )


#define EE_RSCAN_BOM_ISO          ( 0x00u )
#define EE_RSCAN_BOM_HALTBOFF     ( 0x01u )
#define EE_RSCAN_BOM_HALTRECV     ( 0x02u )
#define EE_RSCAN_BOM_SW           ( 0x03u )

#define EE_RSCAN_TEST_BASIC       ( 0x00u )
#define EE_RSCAN_TEST_RXONLY      ( 0x01u )
#define EE_RSCAN_TEST_EXTLOOP     ( 0x02u )
#define EE_RSCAN_TEST_INTLOOP     ( 0x03u )

#define EE_RSCAN_TXPRIO_ID        ( 0x00u )
#define EE_RSCAN_TXPRIO_MBOX      ( 0x01u )

#define EE_RSCAN_CLOCK_INTERN     ( 0x00u )
#define EE_RSCAN_CLOCK_EXTERN_XIN ( 0x01u )

#define EE_RSCAN_CLOCK_EXT        ( 0x01u )
#define EE_RSCAN_CLOCK_EXTPIN     ( 0x00u )
#define EE_RSCAN_CLOCK_EXTTEST    ( 0x01u )
#define EE_RSCAN_CLOCK_TSMACRO    ( 0x00u )
#define EE_RSCAN_CLOCK_TSBIT      ( 0x01u )
#define EE_RSCAN_CLOCK_FIFO_OFF   ( 0x0000u )

#define EE_RSCAN_TIMESTAMP_PRE_0  ( 0x00u )

#define EE_RSCAN_TIMESTAMP_PCLK     ( 0x0u )
#define EE_RSCAN_TIMESTAMP_BIT_CLK  ( 0x1u )


/* RSCAN Status Return identifiers */

#define EE_RSCAN_STATUS_OPMODE        0u
#define EE_RSCAN_STATUS_PSMODE        1u
#define EE_RSCAN_STATUS_RECEIVE       2u
#define EE_RSCAN_STATUS_TRANSMIT      3u
#define EE_RSCAN_STATUS_BUSOFF        4u
#define EE_RSCAN_STATUS_TOVF         10u
#define EE_RSCAN_STATUS_THPM         11u
#define EE_RSCAN_STATUS_TGPT         12u
#define EE_RSCAN_STATUS_VALID        13u
#define EE_RSCAN_STATUS_TRERRCOUNT   16u
#define EE_RSCAN_STATUS_RXERRCOUNT   17u
#define EE_RSCAN_STATUS_NEWTXHISTORY 22u
#define EE_RSCAN_STATUS_INTERRUPT    24u
#define EE_RSCAN_STATUS_ERRCNTLEV    25u
#define EE_RSCAN_STATUS_ERRPLEV      26u
#define EE_RSCAN_STATUS_INT_RXFIFO   27u

/* RSCAN Status Return values */

#define EE_RSCAN_RSTATUS_OPERATION ( 0x00u )
#define EE_RSCAN_RSTATUS_RESET     ( 0x01u )
#define EE_RSCAN_RSTATUS_HALT      ( 0x02u )
#define EE_RSCAN_RSTATUS_SLEEP     ( 0x04u )
#define EE_RSCAN_RSTATUS_PASSIVE   ( 0x08u )
#define EE_RSCAN_RSTATUS_BUSOFF    ( 0x10u )
#define EE_RSCAN_RSTATUS_TRANSMIT  ( 0x20u )
#define EE_RSCAN_RSTATUS_RECEIVE   ( 0x40u )
#define EE_RSCAN_RSTATUS_RDY       ( 0x80u )
#define EE_RSCAN_RSTATUS_RAMINIT   ( 0x08u )

/* RSCAN Error Status Return values */

#define EE_RSCAN_GERROR_DLCCHECK  ( 0x01u )
#define EE_RSCAN_GERROR_MSGLOST   ( 0x02u )
#define EE_RSCAN_GERROR_THLLOST   ( 0x04u )
#define EE_RSCAN_GERROR_RAMPARITY ( 0x08u )
#define EE_RSCAN_GERROR_PLLFAIL   ( 0x10u )
#define EE_RSCAN_GERROR_OTBFAIL   ( 0x20u )

#define EE_RSCAN_ERROR_BUSERR     ( 0x0001u )
#define EE_RSCAN_ERROR_WARNING    ( 0x0002u )
#define EE_RSCAN_ERROR_PASSIVE    ( 0x0004u )
#define EE_RSCAN_ERROR_BUSOFF     ( 0x0008u )
#define EE_RSCAN_ERROR_RECOVERY   ( 0x0010u )
#define EE_RSCAN_ERROR_OVERLOAD   ( 0x0020u )
#define EE_RSCAN_ERROR_BUSLOCK    ( 0x0040u )
#define EE_RSCAN_ERROR_ARBLOST    ( 0x0080u )
#define EE_RSCAN_ERROR_STUFFING   ( 0x0100u )
#define EE_RSCAN_ERROR_FORM       ( 0x0200u )
#define EE_RSCAN_ERROR_ACK        ( 0x0400u )
#define EE_RSCAN_ERROR_CRC        ( 0x0800u )
#define EE_RSCAN_ERROR_BITLEV1    ( 0x1000u )
#define EE_RSCAN_ERROR_BITLEV0    ( 0x2000u )
#define EE_RSCAN_ERROR_ACKDELIM   ( 0x4000u )

#define EE_RSCAN_ERROR_CLEARALL   ( 0x0000u )

#define EE_RSCAN_DN_CLEARALL      ( 0x0000u )

#define EE_RSCAN_RX_MB_DIS	  ( 0u )


#define EE_RSCAN_COM_FIFO_DIS	  ( 0x00u )
#define EE_RSCAN_COM_FIFO_EN	  ( 0x01u )
#define EE_RSCAN_COM_FIFO_TX_DIS  ( 0x00u )
#define EE_RSCAN_COM_FIFO_TX_EN	  ( 0x01u )
#define EE_RSCAN_COM_FIFO_RX_DIS  ( 0x00u )
#define EE_RSCAN_COM_FIFO_RX_EN	  ( 0x01u )

#define EE_RSCAN_FIFO_DEPTH_0     ( 0x00u )
#define EE_RSCAN_FIFO_DEPTH_4     ( 0x01u )
#define EE_RSCAN_FIFO_DEPTH_8     ( 0x02u )
#define EE_RSCAN_FIFO_DEPTH_16    ( 0x03u )
#define EE_RSCAN_FIFO_DEPTH_32    ( 0x04u )
#define EE_RSCAN_FIFO_DEPTH_48    ( 0x05u )
#define EE_RSCAN_FIFO_DEPTH_64    ( 0x06u )
#define EE_RSCAN_FIFO_DEPTH_128   ( 0x07u )

#define	EE_RSCAN_RX_FIFO_EN	  ( 0x01u )
#define	EE_RSCAN_RX_FIFO_DIS	  ( 0x00u )
#define	EE_RSCAN_RX_FIFO_INT_EN	  ( 0x01u )
#define	EE_RSCAN_RX_FIFO_INT_DIS  ( 0x00u )

#define RESERVED (0u)

#define EE_RSCAN_FIFO_ILEVEL_1DIV8  ( 0x00u )
#define EE_RSCAN_FIFO_ILEVEL_2DIV8  ( 0x01u )
#define EE_RSCAN_FIFO_ILEVEL_3DIV8  ( 0x02u )
#define EE_RSCAN_FIFO_ILEVEL_4DIV8  ( 0x03u )
#define EE_RSCAN_FIFO_ILEVEL_5DIV8  ( 0x04u )
#define EE_RSCAN_FIFO_ILEVEL_6DIV8  ( 0x05u )
#define EE_RSCAN_FIFO_ILEVEL_7DIV8  ( 0x06u )
#define EE_RSCAN_FIFO_ILEVEL_8DIV8  ( 0x07u )

#define EE_RSCAN_FIFO_INT_AT_EVERY  ( 0x01u )
#define EE_RSCAN_FIFO_INT_AT_LEVEL  ( 0x00u )

#define EE_RSCAN_FIFO_MODE_RX     ( 0x00u )
#define EE_RSCAN_FIFO_MODE_TX     ( 0x01u )
#define EE_RSCAN_FIFO_MODE_GW     ( 0x02u )

#define EE_RSCAN_FIFO_IT_REFCLK   ( 0x00u )
#define EE_RSCAN_FIFO_IT_BTCLK    ( 0x01u )
#define EE_RSCAN_FIFO_IT_REFCLK1  ( 0x00u )
#define EE_RSCAN_FIFO_IT_REFCLK10 ( 0x01u )

#define EE_RSCAN_FIFO_NEXTELEMENT ( 0xFFu )

#define EE_RSCAN_TID_NOTUSED      ( 0xFFu )

#define EE_RSCAN_TRQ_ONGOING      ( 0x00u )
#define EE_RSCAN_TRQ_CLEAR        ( 0x00u )
#define EE_RSCAN_TRQ_ABORTED      ( 0x01u )
#define EE_RSCAN_TRQ_COMPLETED    ( 0x02u )
#define EE_RSCAN_TRQ_ABORTERROR   ( 0x03u )

#define EE_RSCAN_TXQ_OFF          ( 0x00u )
#define EE_RSCAN_TXQ_MIN          ( 0x02u )
#define EE_RSCAN_TXQ_INT_ONEVERY  ( 0x01u )
#define EE_RSCAN_TXQ_INT_ONLAST   ( 0x00u )

#define EE_RSCAN_TXQ_NEXTELEMENT  ( 0xFFu )

#define EE_RSCAN_TMC_SET_TR       ( 0x01u )
#define EE_RSCAN_TMC_SET_AR       ( 0x02u )
#define EE_RSCAN_TMC_SET_OM       ( 0x04u )

#define EE_RSCAN_TMSTS_CLEAR      ( 0x00u )
#define EE_RSCAN_TMSTS_STSMSK     ( 0x01u )
#define EE_RSCAN_TMSTS_RFMSK      ( 0x06u )

#define EE_RSCAN_THL_INT_ONLEVEL  ( 0x00u )
#define EE_RSCAN_THL_INT_ONEVERY  ( 0x01u )
#define EE_RSCAN_THL_ENTRY_ALL    ( 0x01u )
#define EE_RSCAN_THL_ENTRY_QUEUED ( 0x00u )

#define EE_RSCAN_THL_NEXTELEMENT  ( 0xFFu )
#define EE_RSCAN_THL_EMPTY        ( 0x01u )

#define EE_RSCAN_THL_TX_BUFFER    ( 0x01u )
#define EE_RSCAN_THL_TX_FIFO      ( 0x02u )
#define EE_RSCAN_THL_TX_QUEUE     ( 0x04u )
#define EE_RSCAN_THL_BN_P         ( 3u )

#define EE_RSCAN_GL_AFL_ENTRY_0	   ( 0u )

#define EE_RSCAN_AFL_EXTERN_ENTRY  ( 0x00u )
#define EE_RSCAN_AFL_INTERN_ENTRY  ( 0x01u )

#define EE_RSCAN_AFL_RX_MB_VALID   ( 0x01u )
#define EE_RSCAN_AFL_RX_MB_INVALID ( 0x00u )

#define EE_RSCAN_AFL_RXFIF0_NONE  ( 0x00u )
#define EE_RSCAN_AFL_RXFIF0_EN0   ( 0x01u )
#define EE_RSCAN_AFL_RXFIF0_EN1   ( 0x02u )

#define EE_RSCAN_GL_CONFIG_RXFIF0_EN0   ( 0x01u )
#define EE_RSCAN_GL_CONFIG_RXFIF0_EN1   ( 0x02u )

#define EE_RSCAN_AFL_COMFIFO_NONE ( 0x0000u )
#define EE_RSCAN_AFL_COMFIFO_C0E0 ( 0x0001u )

#define EE_RSCAN_BUF_DATA0_MASK   ( 0x000000FFu )
#define EE_RSCAN_BUF_DATA1_MASK   ( 0x0000FF00u )
#define EE_RSCAN_BUF_DATA2_MASK   ( 0x00FF0000u )
#define EE_RSCAN_BUF_DATA3_MASK   ( 0xFF000000u )
#define EE_RSCAN_BUF_DATA4_MASK   ( 0x000000FFu )
#define EE_RSCAN_BUF_DATA5_MASK   ( 0x0000FF00u )
#define EE_RSCAN_BUF_DATA6_MASK   ( 0x00FF0000u )
#define EE_RSCAN_BUF_DATA7_MASK   ( 0xFF000000u )

#define EE_RSCAN_BUF_DATA0_P      ( 0x00u )
#define EE_RSCAN_BUF_DATA1_P      ( 0x08u )
#define EE_RSCAN_BUF_DATA2_P      ( 0x10u )
#define EE_RSCAN_BUF_DATA3_P      ( 0x18u )
#define EE_RSCAN_BUF_DATA4_P      ( 0x00u )
#define EE_RSCAN_BUF_DATA5_P      ( 0x08u )
#define EE_RSCAN_BUF_DATA6_P      ( 0x10u )
#define EE_RSCAN_BUF_DATA7_P      ( 0x18u )

#define EE_RSCAN_DLCCHECK_ENABLE  ( 0x01u )
#define EE_RSCAN_DLCCHECK_DISABLE ( 0x00u )

#define EE_RSCAN_DLCREPL_ENABLE   ( 0x01u )
#define EE_RSCAN_DLCREPL_DISABLE  ( 0x00u )

#define EE_RSCAN_MIRROR_ENABLE    ( 0x01u )
#define EE_RSCAN_MIRROR_DISABLE   ( 0x00u )

#define EE_RSCAN_TIMESTAMP_RESET  ( 0x01u )
#define EE_RSCAN_TIMESTAMP_KEEP   ( 0x00u )
#define EE_RSCAN_TIMESTAMP_WRITE  ( 0x01u )

#define EE_RSCAN_PATH_MSGBOX      ( 0x00u )
#define EE_RSCAN_PATH_COMFIFO     ( 0x01u )
#define EE_RSCAN_PATH_TXQUEUE     ( 0x02u )
#define EE_RSCAN_PATH_RXFIFO      ( 0x03u )
#define EE_RSCAN_PATH_ANY         ( 0xFFu )

#define EE_RSCAN_PATHDETAIL_ANY   ( 0xFFu )

#define EE_RSCAN_FAULT_NONE       ( 0x00u )
#define EE_RSCAN_FAULT_PARAMETER  ( 0x01u )
#define EE_RSCAN_FAULT_BUSY       ( 0x02u )
#define EE_RSCAN_FAULT_NODATA     ( 0x04u )
#define EE_RSCAN_FAULT_OTHER      ( 0xFFu )

#define EE_RSCAN_RAMTEST_KEY1     ( 0x7575u )
#define EE_RSCAN_RAMTEST_KEY2     ( 0x8A8Au )
#define EE_RSCAN_RAMTEST_MODE     ( 0x00000004u )
#define EE_RSCAN_RAMTEST_PAGES    ( 47u ) /* 12160 Bytes RAM -> Pages = 12160 Byte / 256 Byte */
#define EE_RSCAN_RAMTEST_ENTRIES  ( 64u ) /* 256 Bytes per Page -> Entries = 256 / 4	      */

/*............... The RSCAN access structures ............................*/

typedef struct ee_rscan_c_cfg
{
    volatile u32 brp       : 10;
    volatile u32 RESERVED0 : 6;
    volatile u32 tseg1     : 4;
    volatile u32 tseg2     : 3;
    volatile u32 RESERVED1 : 1;
    volatile u32 sjw       : 2;
    volatile u32 RESERVED2 : 6;

} ee_rscan_c_cfg;

typedef struct ee_rscan_c_ctr
{
    volatile u32 chmdc     : 2;
    volatile u32 cslpr     : 1;
    volatile u32 rtbo      : 1;
    volatile u32 RESERVED0 : 4;
    volatile u32 ie	 : 9;
    volatile u32 RESERVED1 : 4;
    volatile u32 bom       : 2;
    volatile u32 errd      : 1;
    volatile u32 ctme      : 1;
    volatile u32 ctms      : 2;
    volatile u32 RESERVED2 : 5;

} ee_rscan_c_ctr;

typedef struct ee_rscan_c_sts
{
    volatile u32 mds       : 2;
    volatile u32 slps      : 1;
    volatile u32 eps       : 1;
    volatile u32 boffs     : 1;
    volatile u32 trxs      : 1;
    volatile u32 recs      : 1;
    volatile u32 coms      : 1;
    volatile u32 RESERVED0 : 8;
    volatile u32 rec       : 8;
    volatile u32 tec       : 8;

} ee_rscan_c_sts;

typedef struct ee_rscan_c_erfl
{
    volatile u32 err       : 15;
    volatile u32 RESERVED0 : 1;
    volatile u32 crc       : 15;
    volatile u32 RESERVED1 : 1;

} ee_rscan_c_erfl;

typedef struct ee_rscan_c_ch
{
    struct ee_rscan_c_cfg  cfg;
    struct ee_rscan_c_ctr  ctr;
    struct ee_rscan_c_sts  sts;
    struct ee_rscan_c_erfl erfl;

} ee_rscan_c_ch;

typedef struct ee_rscan_c_gcfg
{
    volatile u32 tpri      : 1;
    volatile u32 dce       : 1;
    volatile u32 dre       : 1;
    volatile u32 mme       : 1;
    volatile u32 pllbp     : 1;
    volatile u32 RESERVED0 : 3;
    volatile u32 tsp       : 4;
    volatile u32 tsss      : 1;
    volatile u32 RESERVED1 : 3;
    volatile u32 itrcp     : 16;

} ee_rscan_c_gcfg;

typedef struct ee_rscan_c_gctr
{
    volatile u32 gmdc      : 2;
    volatile u32 gslpr     : 1;
    volatile u32 RESERVED0 : 5;
    volatile u32 ie        : 3;
    volatile u32 RESERVED1 : 5;
    volatile u32 tsrst     : 1;
    volatile u32 RESERVED2 : 1;
    volatile u32 RESERVED3 : 14;

} ee_rscan_c_gctr;

typedef struct ee_rscan_c_gsts
{
    volatile u16 mds       : 2;	/* {HLTSTS, STSTS} = mds */
    volatile u16 slps      : 1;
    volatile u16 rsts      : 1;
    volatile u16 RESERVED0 : 12;

} ee_rscan_c_gsts;

typedef struct ee_rscan_c_gerfl
{
    volatile u08  err       : 3;
    volatile u08  RESERVED0 : 5;

} ee_rscan_c_gerfl;


typedef struct ee_rscan_c_gaflcfg
{
    volatile u16 rnc0      : 5;
    volatile u16 RESERVED0 : 11;

} ee_rscan_c_gaflcfg;

typedef struct ee_rscan_c_rfcc
{
    volatile u16 rfe       : 1;
    volatile u16 rfie      : 1;
    volatile u16 RESERVED0 : 6;
    volatile u16 rfdc      : 3;
    volatile u16 RESERVED1 : 1;
    volatile u16 rfim      : 1;
    volatile u16 rfigcv    : 3;

} ee_rscan_c_rfcc;


typedef struct ee_rscan_c_cfcc
{
    volatile u32 cfe       : 1;
    volatile u32 cfrxie    : 1;
    volatile u32 cftxie    : 1;
    volatile u32 RESERVED0 : 5;
    volatile u32 cfdc      : 3;
    volatile u32 RESERVED1 : 1;
    volatile u32 cfim      : 1;
    volatile u32 cfigcv    : 3;
    volatile u32 cfm       : 2;
    volatile u32 cfitss    : 1;
    volatile u32 cfitr     : 1;
    volatile u32 cftml     : 2;
    volatile u32 RESERVED2 : 2;
    volatile u32 cfitt     : 8;

} ee_rscan_c_cfcc;

typedef struct ee_rscan_c_rfsts
{
    volatile u16 rfemp     : 1;
    volatile u16 rffll     : 1;
    volatile u16 rfmlt     : 1;
    volatile u16 rfif      : 1;
    volatile u16 RESERVED0 : 4;
    volatile u16 rfmc      : 8;

} ee_rscan_c_rfsts;

typedef struct ee_rscan_c_cfsts
{
    volatile u16 cfemp     : 1;
    volatile u16 cffll     : 1;
    volatile u16 cfmlt     : 1;
    volatile u16 cfrxif    : 1;
    volatile u16 cftxif    : 1;
    volatile u16 RESERVED0 : 3;
    volatile u16 cfmc      : 6;
    volatile u16 RESERVED1 : 2;

} ee_rscan_c_cfsts;

typedef struct ee_rscan_c_rfpctr
{
    volatile u16 fpc       : 8;
    volatile u16 RESERVED0 : 8;

} ee_rscan_c_rfpctr;

typedef struct ee_rscan_c_cfpctr
{
    volatile u16 fpc       : 8;
    volatile u16 RESERVED0 : 8;

} ee_rscan_c_cfpctr;

typedef struct ee_rscan_rfmsts
{
    volatile u08 mlt0      : 1;
    volatile u08 mlt1      : 1;
    volatile u08 RESERVED0 : 6;

} ee_rscan_rfmsts;

typedef struct ee_rscan_cfmsts
{
    volatile u08 mlt0      : 1;
    volatile u08 RESERVED0 : 7;

} ee_rscan_cfmsts;

typedef struct ee_rscan_c_tmc
{
    volatile u08 tr        : 1;
    volatile u08 ar        : 1;
    volatile u08 om        : 1;
    volatile u08 RESERVED0 : 5;

} ee_rscan_c_tmc;

typedef struct ee_rscan_c_tmsts
{
    volatile u08 sts       : 1;
    volatile u08 rf        : 2;
    volatile u08 rm        : 1;
    volatile u08 arm       : 1;
    volatile u08 RESERVED0 : 3;

} ee_rscan_c_tmsts;


typedef struct ee_rscan_c_thlcc
{
    volatile u16 thle      : 1;
    volatile u16 RESERVED0 : 7;
    volatile u16 ie        : 1;
    volatile u16 im        : 1;
    volatile u16 dte       : 1;
    volatile u16 RESERVED1 : 5;

} ee_rscan_c_thlcc;

typedef struct ee_rscan_c_thlsts
{
    volatile u16 emp       : 1;
    volatile u16 fll       : 1;
    volatile u16 elt       : 1;
    volatile u16 irq       : 1;
    volatile u16 RESERVED0 : 4;
    volatile u16 mc        : 4;
    volatile u16 RESERVED1 : 4;

} ee_rscan_c_thlsts;

typedef struct ee_rscan_c_gtstcfg
{
    volatile u16 RESERVED0 : 8;
    volatile u16 rtmps     : 3;
    volatile u16 RESERVED1 : 5;

} ee_rscan_c_gtstcfg;

typedef struct ee_rscan_c_grwcr
{
    volatile u16 rpage	 : 1;
    volatile u16 RESERVED0 : 15;

} ee_rscan_c_grwcr;



typedef struct ee_rscan_c_gtstctr
{
    volatile u08 RESERVED0 : 2;
    volatile u08 rtme      : 1;
    volatile u08 RESERVED1 : 5;

} ee_rscan_c_gtstctr;

typedef struct ee_rscan_c_gtsc
{
    volatile u16 ts		 : 16;
} ee_rscan_c_gtsc;

typedef struct ee_rscan_common           /* <<-- specific for 1-channel macro */
{
    struct ee_rscan_c_ch     ch[ 1 ];         /* channel registers */
    volatile u16             RESERVED00[9];
    struct ee_rscan_c_gcfg   gcfg;
    struct ee_rscan_c_gctr   gctr;
    struct ee_rscan_c_gsts   gsts;                        /* global status */
    struct ee_rscan_c_gerfl  gerfl;
    volatile u08			   RESERVED1;
    struct ee_rscan_c_gtsc   gtsc;                       	/* global AFL configuration */
    struct ee_rscan_c_gaflcfg gaflcfg;
    volatile u16            rmnb;                         /* RX buffer configuration */
    volatile u16            rmnd[ EE_RSCAN_COMRXREGS ];
    volatile u16            RESERVED2;
    struct ee_rscan_c_rfcc  rfcc[ EE_RSCAN_MAXRXFIFOS ];  /* RX FIFO configuration */
    volatile u16			  RESERVED3[2];
    struct ee_rscan_c_rfsts rfsts[ EE_RSCAN_MAXRXFIFOS ];
    volatile u16			  RESERVEDx[2];
    struct ee_rscan_c_rfpctr rfpctr[ EE_RSCAN_MAXRXFIFOS ];  /* Common FIFO configuration */
    volatile u16			  RESERVEDy[2];
    struct ee_rscan_c_cfcc  cfcc;
    volatile u16            RESERVED4[2];
    struct ee_rscan_c_cfsts cfsts;
    volatile u16            RESERVED5[1];
    struct ee_rscan_c_cfpctr cfpctr;
    volatile u16            RESERVED6[1];
    volatile ee_rscan_rfmsts rfmsts;
    volatile ee_rscan_cfmsts cfmsts;
    volatile u08            rfists;
    volatile u08            cfists;                        /* TX buffer configuration */
    volatile u08            tmc[ EE_RSCAN_MAXTXBUFFERS];
    volatile u16            RESERVED7[2];
    volatile u08            tmsts[ EE_RSCAN_MAXTXBUFFERS ];
    volatile u08            RESERVED8[3];
    volatile u16            tmtrsts[ EE_RSCAN_COMTXREGS ]; /* TX buffer status */
    volatile u16            tmtcsts[ EE_RSCAN_COMTXREGS ];
    volatile u16            tmtasts[ EE_RSCAN_COMTXREGS ];
    volatile u16            tmiec;
    struct ee_rscan_c_thlcc thlcc[ EE_RSCAN_CHANNELS ];   /* THL configuration */
    volatile u16			  RESERVEDz;
    struct ee_rscan_c_thlsts thlsts[ EE_RSCAN_CHANNELS ];
    volatile u16            RESERVED9;
    volatile u16            thlpctr[ EE_RSCAN_CHANNELS ];
    volatile u16            RESERVED10;
    volatile u16            gtintsts[ 1 ];
    struct ee_rscan_c_grwcr grwcr;
    struct ee_rscan_c_gtstcfg gtstcfg;
    struct ee_rscan_c_gtstctr gtstctr;
    volatile u16            RESERVED11[2];
    volatile u16            glockk;

} ee_rscan_common;

typedef struct ee_rscan_a_aflid
{
    volatile u32 id  : 29;
    volatile u32 lb  : 1;
    volatile u32 rtr : 1;
    volatile u32 ide : 1;

} ee_rscan_a_aflid;

typedef struct ee_rscan_r_mask /* GAFLM */
{
    volatile u32 id        : 29;
    volatile u32 RESERVED0 : 1;
    volatile u32 rtr       : 1;
    volatile u32 ide       : 1;

} ee_rscan_r_mask;

typedef struct ee_rscan_t_mask
{
    volatile u32 id        : 29;
    volatile u32 thlen     : 1;
    volatile u32 rtr       : 1;
    volatile u32 ide       : 1;

} ee_rscan_t_mask;

typedef struct ee_rscan_a_aflptr0 /* (GAFLPLj) dk_ok */
{
    volatile u32 rxfifomask 	: 2;
    volatile u32 RESERVED0 	: 2;
    volatile u32 comfifomask 	: 1;
    volatile u32 RESERVED1 	: 3;
    volatile u32 rmdp      	: 7;
    volatile u32 rmv       	: 1;
    volatile u32 ptr       	: 12;
    volatile u32 dlc       	: 4;

} ee_rscan_a_aflptr0;

typedef struct ee_rscan_a_afl
{
    struct ee_rscan_a_aflid   id;
    struct ee_rscan_r_mask    mask;
    struct ee_rscan_a_aflptr0 ptr0;

} ee_rscan_a_afl;

typedef struct ee_rscan_aflpage
{
    struct ee_rscan_a_afl af[ EE_RSCAN_AFLPAGESIZE ];

} ee_rscan_aflpage;

typedef struct ee_rscan_r_ptr	/* RMPTRn */
{
    volatile u32 ts  : 16;	/* rmts   */
    volatile u32 ptr : 12;	/* rmptr  */
    volatile u32 dlc : 4;		/* rmdlc  */

} ee_rscan_r_ptr;

typedef struct ee_rscan_t_ptr /* dk open */
{
    volatile u32 RESERVED0 : 16;
    volatile u32 tid       : 8;
    volatile u32 RESERVED1 : 4;
    volatile u32 dlc       : 4;

} ee_rscan_t_ptr;

typedef struct ee_rscan_r_rmsg
{
    struct ee_rscan_r_mask id;
    struct ee_rscan_r_ptr  ptr;
    volatile u16          dl0;
    volatile u16          dl1;
    volatile u16          dh0;
    volatile u16          dh1;

} ee_rscan_r_rmsg;

typedef struct ee_rscan_r_cmsg
{
    struct ee_rscan_t_mask id;
    struct ee_rscan_r_ptr  ptr;
    volatile u16          dl0;
    volatile u16          dl1;
    volatile u16          dh0;
    volatile u16          dh1;

} ee_rscan_r_cmsg;

typedef struct ee_rscan_t_msg
{
    struct ee_rscan_t_mask id;
    struct ee_rscan_t_ptr  ptr;
    volatile u16          dl0;
    volatile u16          dl1;
    volatile u16          dh0;
    volatile u16          dh1;

} ee_rscan_t_msg;

typedef struct ee_rscan_message
{
    struct ee_rscan_t_mask hdr;
    struct ee_rscan_r_ptr  flag;
    u08                    data[ EE_RSCAN_DLC_MAX ];
    u08                    path;
    u08                    pathdetail;

} ee_rscan_message;

typedef struct ee_rscan_rxmsg
{
    struct ee_rscan_r_rmsg buf[ EE_RSCAN_MAXRXBUFFERS ];

} ee_rscan_rxmsg;

typedef struct ee_rscan_rxfifo
{
    struct ee_rscan_r_rmsg buf[ EE_RSCAN_MAXRXFIFOS ];

} ee_rscan_rxfifo;

typedef struct ee_rscan_comfifo
{
    struct ee_rscan_r_cmsg buf[ EE_RSCAN_MAXCOMFIFOS ];

} ee_rscan_comfifo;

typedef struct ee_rscan_txmsg
{
    struct ee_rscan_t_msg buf[ EE_RSCAN_MAXTXBUFFERS ];

} ee_rscan_txmsg;

typedef struct ee_rscan_thl
{
    volatile u32 bt        : 2;
    volatile u32 RESERVED0 : 1;
    volatile u32 bn        : 2;
    volatile u32 RESERVED1 : 3;
    volatile u32 tid       : 8;

} ee_rscan_thl;

typedef struct ee_rscan_ram
{
    volatile u32 cell[ EE_RSCAN_RAMTEST_ENTRIES ];

} ee_rscan_ram;

/* SW Channel configuration */

typedef struct ee_rscan_cfg_channel
{
    u32                     bitrate;
    u08                     tq_perbit;        /* optional Parameter, can be 0 */
    u08                     syncjumpwidth;    /* optional Parameter, can be 0 */
    flt                     samplingpointpos; /* optional Parameter, can be 0.0 */
    struct ee_rscan_c_ctr   ctr;
    u16                     tmiec;            /* TX Interrupt Enable per channel */
    struct ee_rscan_c_thlcc thlcc;            /* THL configuration */
    struct ee_rscan_c_cfcc  cfcc;             /* COM FIFO configuration */

} ee_rscan_cfg_channel;

/* Global configuration */

typedef struct ee_rscan_cfg_global
{
    struct ee_rscan_c_gcfg   gcfg;
    struct ee_rscan_c_gctr   gctr;
    volatile u32             rmnb;
    volatile u32             rnc[ EE_RSCAN_CHANNELS ];
    struct ee_rscan_c_rfcc   rfcc[ EE_RSCAN_MAXRXFIFOS ];

} ee_rscan_cfg_global;

#endif


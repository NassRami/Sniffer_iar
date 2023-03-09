/*===========================================================================*/
/* Module : rscan_s.h                                                        */
/* Version: V1.03                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN module                                                     */
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
/*              V1.04: Use X1 Oscillator for CAN Communication Clock         */
/*                                                                           */
/*===========================================================================*/


#include <stddef.h>
#include <ree_types.h>

#include <map_rscan.h>

/* Default Configuration Macros */
#define EE_RSCAN_A_COMFIFO_OFF { EE_RSCAN_COM_FIFO_DIS,\
                                 EE_RSCAN_COM_FIFO_RX_DIS,\
				 EE_RSCAN_COM_FIFO_TX_DIS, RESERVED, \
				 EE_RSCAN_FIFO_DEPTH_0, RESERVED, \
                                 EE_RSCAN_FIFO_INT_AT_LEVEL, \
                                 EE_RSCAN_FIFO_ILEVEL_1DIV8, \
                                 EE_RSCAN_FIFO_MODE_RX, \
                                 EE_RSCAN_FIFO_IT_REFCLK, \
                                 EE_RSCAN_FIFO_IT_REFCLK1, \
                                 0, 0 } // Link to TX MB, delay TX start not 

// COM FIFO as TX -> ON
#define EE_RSCAN_A_COMFIFO_TX_ON { EE_RSCAN_COM_FIFO_EN,\
				   EE_RSCAN_COM_FIFO_RX_DIS, \
				   EE_RSCAN_COM_FIFO_TX_EN, RESERVED,\
				   EE_RSCAN_FIFO_DEPTH_4, RESERVED, \
                                   EE_RSCAN_FIFO_INT_AT_EVERY, \
                                   EE_RSCAN_FIFO_ILEVEL_1DIV8, \
                                   EE_RSCAN_FIFO_MODE_TX, \
                                   EE_RSCAN_FIFO_IT_REFCLK, \
                                   EE_RSCAN_FIFO_IT_REFCLK1, \
                                   0, 0 } // Link to TX MB, delay TX start not


// COM FIFO as RX -> ON
#define EE_RSCAN_A_COMFIFO_RX_ON { EE_RSCAN_COM_FIFO_EN,\
				   EE_RSCAN_COM_FIFO_RX_EN,\
				   EE_RSCAN_COM_FIFO_TX_DIS, RESERVED,\
				   EE_RSCAN_FIFO_DEPTH_8, RESERVED,\
                                   EE_RSCAN_FIFO_INT_AT_EVERY, \
                                   EE_RSCAN_FIFO_ILEVEL_1DIV8, \
                                   EE_RSCAN_FIFO_MODE_RX, \
                                   EE_RSCAN_FIFO_IT_REFCLK, \
                                   EE_RSCAN_FIFO_IT_REFCLK1, \
                                   0, 0 } // not used for RX							  


#define EE_RSCAN_A_RXFIFO_OFF  { EE_RSCAN_RX_FIFO_DIS,\
				 EE_RSCAN_RX_FIFO_INT_DIS,\
				 RESERVED,\
				 EE_RSCAN_FIFO_DEPTH_0, \
                                 EE_RSCAN_FIFO_INT_AT_LEVEL, \
                                 EE_RSCAN_FIFO_ILEVEL_1DIV8 } /* RX FIFO disabled */

#define EE_RSCAN_A_RXFIFO_ON  { EE_RSCAN_RX_FIFO_EN,\
				EE_RSCAN_RX_FIFO_INT_EN,\
				RESERVED,\
				EE_RSCAN_FIFO_DEPTH_16,\
				RESERVED,\
                                EE_RSCAN_FIFO_INT_AT_EVERY, \
                                EE_RSCAN_FIFO_ILEVEL_1DIV8 } /* RX FIFO disabled */

#define EE_RSCAN_A_TXQ_OFF { 0, 0, EE_RSCAN_TXQ_OFF, 0, 0 }   /* TX Queue OFF */
#define EE_RSCAN_A_THL_OFF { 0, 0, 0, EE_RSCAN_THL_INT_ONLEVEL, EE_RSCAN_THL_ENTRY_QUEUED }           /* THL OFF */
#define EE_RSCAN_A_THL_ON  { 1, 0, 0, EE_RSCAN_THL_INT_ONLEVEL, EE_RSCAN_THL_ENTRY_QUEUED }           /* THL ON */




////////////////////////////////////////////////////////
// GLOBAL CONFIGURATION
////////////////////////////////////////////////////////
const struct ee_rscan_cfg_global EE_RSCAN_GCFG_CF_EXTERN =
{
    {
        // Global Configuration Register GCFG
        EE_RSCAN_TXPRIO_ID,				/* TX priority by ID (standard) */
        EE_RSCAN_DLCCHECK_DISABLE, 			/* DLC (Data length) check disable */
        EE_RSCAN_DLCREPL_DISABLE,			/* no DLC check or replacement */
        EE_RSCAN_MIRROR_DISABLE,			/* no Mirror Mode */
        EE_RSCAN_CLOCK_EXTERN_XIN,		        /* use MOSC clock */
        //EE_RSCAN_CLOCK_INTERN,			        /* use PLL clock */
        RESERVED,
        EE_RSCAN_TIMESTAMP_PRE_0,			/* Timestamp Prescaler = 0 */
        EE_RSCAN_TIMESTAMP_PCLK,			/* Timestamp Source PCLK */
        EE_RSCAN_CHANNEL0,          		        /* Select Channel Bit Time Clock (default) */
        EE_RSCAN_CLOCK_FIFO_OFF                         /* Interval timer of FIFO disabled */
    },
    {
        // Global Control Register GCTR
        EE_RSCAN_CH_KEEP,
        EE_RSCAN_SLEEP_DISABLE,			        /* No implicit change of Operation Mode */
        RESERVED,
        EE_RSCAN_GINT_NONE,				/* No Error Interrupts */
        RESERVED,
        EE_RSCAN_TIMESTAMP_RESET,			/* Timestamp reset */
        EE_RSCAN_TIMESTAMP_KEEP,			/* Timestamp is not written by software */
        RESERVED
    },
    EE_RSCAN_RX_MB_DIS,				/* non classical RX Message buffers are allocated */
    {
        EE_RSCAN_GL_AFL_ENTRY_0,			/* Number of AFL entrys per channel */
    },
    {
        EE_RSCAN_A_RXFIFO_ON,			/* disable all RX FIFOs */
        EE_RSCAN_A_RXFIFO_OFF,
    }
};

const struct ee_rscan_cfg_global EE_RSCAN_GCFG_CF_ST =
{
    {
        // Global Configuration Register GCFG
        EE_RSCAN_TXPRIO_ID,				/* TX priority by ID (standard) */
        EE_RSCAN_DLCCHECK_DISABLE, 			/* DLC (Data length) check disable */
        EE_RSCAN_DLCREPL_DISABLE,			/* no DLC check or replacement */
        EE_RSCAN_MIRROR_DISABLE,			/* no Mirror Mode */
        EE_RSCAN_CLOCK_EXTERN_XIN,		        /* use MOSC clock */
        //EE_RSCAN_CLOCK_INTERN,			/* use PLL clock */
        0,						/* RESERVED0 */
        EE_RSCAN_TIMESTAMP_PRE_0,			/* Timestamp Prescaler = 0 */
        EE_RSCAN_TIMESTAMP_PCLK,			/* Timestamp Source PCLK */
        EE_RSCAN_CHANNEL0,          		        /* Select Channel Bit Time Clock */
        EE_RSCAN_CLOCK_FIFO_OFF                         /* Interval timer of FIFO disabled */
    },
    {
        // Global Control Register GCTR
        EE_RSCAN_CH_KEEP,
        EE_RSCAN_SLEEP_DISABLE,				/* No implicit change of Operation Mode */
        0,
        EE_RSCAN_GINT_NONE,				/* No Error Interrupts */
        0,
        EE_RSCAN_TIMESTAMP_RESET,			/* Timestamp reset */
        EE_RSCAN_TIMESTAMP_KEEP,			/* Timestamp is not written by software */
        0
    },
    0,				                        /* non classical RX Message buffers are allocated */
    {
        0			/* only channel 0 has one AFL entry (minimum for a reception) ( Number of AFL entrys per channel) */
    },
    {
        EE_RSCAN_A_RXFIFO_OFF,				/* disable all RX FIFOs */
        EE_RSCAN_A_RXFIFO_OFF
    }
};

const struct ee_rscan_cfg_global EE_RSCAN_GCFG_RX_MB =
{
    {
        // Global Configuration Register GCFG
        EE_RSCAN_TXPRIO_ID,				/* TX priority by ID (standard) */
        EE_RSCAN_DLCCHECK_DISABLE, 			/* DLC (Data length) check disable */
        EE_RSCAN_DLCREPL_DISABLE,			/* no DLC check or replacement */
        EE_RSCAN_MIRROR_DISABLE,			/* no Mirror Mode */
        EE_RSCAN_CLOCK_EXTERN_XIN,		        /* use MOSC clock */
        //EE_RSCAN_CLOCK_INTERN,				/* use PLL clock */
        0,						/* RESERVED0 */
        EE_RSCAN_TIMESTAMP_PRE_0,			/* Timestamp Prescaler = 0 */
        EE_RSCAN_TIMESTAMP_PCLK,			/* Timestamp Source PCLK */
        EE_RSCAN_CHANNEL0,          		        /* Select Channel Bit Time Clock */
        EE_RSCAN_CLOCK_FIFO_OFF                         /* Interval timer of FIFO disabled */
    },
    {
        // Global Control Register GCTR
        EE_RSCAN_CH_KEEP,
        EE_RSCAN_SLEEP_DISABLE,				/* No implicit change of Operation Mode */
        0,
        EE_RSCAN_GINT_NONE,					/* No Error Interrupts */
        0,
        EE_RSCAN_TIMESTAMP_RESET,			/* Timestamp reset */
        EE_RSCAN_TIMESTAMP_KEEP,			/* Timestamp is not written by software */
        0
    },
    0,				/* non classical RX Message buffers are allocated, should be modified */
    {
        0       		/* only channel 0 has one AFL entry (minimum for a reception) ( Number of AFL entrys per channel) */
    },
    {
        EE_RSCAN_A_RXFIFO_OFF,				/* disable all RX FIFOs */
        EE_RSCAN_A_RXFIFO_OFF
    }
};

const struct ee_rscan_cfg_global EE_RSCAN_GCFG_TX_MB_EXTERN =
{
    {
        // Global Configuration Register GCFG
        EE_RSCAN_TXPRIO_ID,				/* TX priority by ID (standard) */
        EE_RSCAN_DLCCHECK_DISABLE, 			/* DLC (Data length) check disable */
        EE_RSCAN_DLCREPL_DISABLE,			/* no DLC check or replacement */
        EE_RSCAN_MIRROR_DISABLE,			/* no Mirror Mode */
        EE_RSCAN_CLOCK_EXTERN_XIN,		        /* use MOSC clock */
        //EE_RSCAN_CLOCK_INTERN,			/* use PLL clock */
        0,						/* RESERVED0 */
        EE_RSCAN_TIMESTAMP_PRE_0,			/* Timestamp Prescaler = 0 */
        EE_RSCAN_TIMESTAMP_PCLK,			/* Timestamp Source PCLK */
        EE_RSCAN_CHANNEL0,          		        /* Select Channel Bit Time Clock */
        EE_RSCAN_CLOCK_FIFO_OFF                         /* Interval timer of FIFO disabled */
    },
    {
        // Global Control Register GCTR
        EE_RSCAN_CH_KEEP,
        EE_RSCAN_SLEEP_DISABLE,				/* No implicit change of Operation Mode */
        0,
        EE_RSCAN_GINT_NONE,				/* No Error Interrupts */
        0,
        EE_RSCAN_TIMESTAMP_RESET,			/* Timestamp reset */
        EE_RSCAN_TIMESTAMP_KEEP,			/* Timestamp is not written by software */
        0
    },
    0,				/* non classical RX Message buffers are allocated */
    {
        0       		/* only channel 0 has one AFL entry (minimum for a reception) ( Number of AFL entrys per channel) */
    },
    {
        EE_RSCAN_A_RXFIFO_OFF,				/* disable all RX FIFOs */
        EE_RSCAN_A_RXFIFO_OFF
    }
};

const struct ee_rscan_cfg_global EE_RSCAN_GCFG_TX_MB_LOOP =
{
    {
        // Global Configuration Register GCFG
        EE_RSCAN_TXPRIO_ID,				/* TX priority by ID (standard) */
        EE_RSCAN_DLCCHECK_DISABLE, 			/* DLC (Data length) check disable */
        EE_RSCAN_DLCREPL_DISABLE,			/* no DLC check or replacement */
        EE_RSCAN_MIRROR_ENABLE,				/* no Mirror Mode */
        EE_RSCAN_CLOCK_EXTERN_XIN,		        /* use MOSC clock */
        //EE_RSCAN_CLOCK_INTERN,			/* use PLL clock */
        0,						/* RESERVED0 */
        EE_RSCAN_TIMESTAMP_PRE_0,			/* Timestamp Prescaler = 0 */
        EE_RSCAN_TIMESTAMP_PCLK,			/* Timestamp Source PCLK */
        EE_RSCAN_CHANNEL0,          		        /* Select Channel Bit Time Clock */
        EE_RSCAN_CLOCK_FIFO_OFF                         /* Interval timer of FIFO disabled */
    },
    {
        // Global Control Register GCTR
        EE_RSCAN_CH_KEEP,
        EE_RSCAN_SLEEP_DISABLE,				/* No implicit change of Operation Mode */
        0,
        EE_RSCAN_GINT_NONE,				/* No Error Interrupts */
        0,
        EE_RSCAN_TIMESTAMP_RESET,			/* Timestamp reset */
        EE_RSCAN_TIMESTAMP_KEEP,			/* Timestamp is not written by software */
        0
    },
    0,				/* non classical RX Message buffers are allocated */
    {
        0       		/* only channel 0 has one AFL entry (minimum for a reception) ( Number of AFL entrys per channel) */
    },
    {
        EE_RSCAN_A_RXFIFO_OFF,				/* disable all RX FIFOs */
        EE_RSCAN_A_RXFIFO_OFF
    }
};

////////////////////////////////////////////////////////
// CHANNEL CONFIGURATION
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/// COM FIFO 0 TX Mode; COM FIFO 1 RX MODE
////////////////////////////////////////////////////////
const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG_CF_EXTERN =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_TX_ON
};

// COM FIFO 0 TX Mode;
const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG1_CF_ST =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_TX_ON
};

// COM FIFO 1 RX MODE
const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG2_CF_ST =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_OFF
};

//////////////////////////////////////////////////////////////////////
/// RX Message Buffer Channel x
//////////////////////////////////////////////////////////////////////

// Channel x RX ( Reception into RX Message Buffer )
const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG1_RX_MB_ST =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_OFF
};

// Channel x+1 TX ( Transmission with TX Message Buffer )
const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG2_RX_MB_ST =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,				        /* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_OFF
};

////////////////////////////////////////////////////////////////////////
// EXTERN TX Message Buffer Channel Config			     //
//////////////////////////////////////////////////////////////////////

const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG_TX_MB_EXTERN =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_BASIC,                    /* Basic Test Mode */
        RESERVED
    },
    0x0001,					/* enable IRQ of TX Buffer0, others disabled */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_OFF
};

////////////////////////////////////////////////////////////////////////
// LOOP TX Message Buffer Channel Config							 //
//////////////////////////////////////////////////////////////////////

const struct ee_rscan_cfg_channel EE_RSCAN_CHCFG_TX_MB_LOOP =
{

    500000, 0, 0, 0.0,				/* Bitrate 500 kbit/s, Bit Timing defualt ( calc by SW )  */
    {
        EE_RSCAN_CH_KEEP,			/* No implicit change of Operation Mode */
        0,					/* Sleep Request disable */
        0,					/* Ch not forced to return from BUS-OFF */
        RESERVED,				/* RESERVED0 */
        EE_RSCAN_CINT_OFF,			/* No Error Interrupts */
        RESERVED,				/* RESERVED1 */
        EE_RSCAN_BOM_ISO,			/* Standard Bus Off behaviour */
        0,   					/* only 1st set of error codes displayed */
        0,					/* Test Mode Off */
        EE_RSCAN_TEST_INTLOOP,                  /* Basic Test Mode */
        RESERVED
    },
    0x0000,					/* disable all IRQ of TX Buffers */
    EE_RSCAN_A_THL_OFF,				/* THL off */
    EE_RSCAN_A_COMFIFO_OFF
};


// Akzeptansfilter Konfiguration
const struct ee_rscan_a_afl EE_RSCAN_AFL_any_MSG_CF_ST =
{
    {
        0x0000000,               	/* Default ID, not relevant because mask is open */
        EE_RSCAN_AFL_EXTERN_ENTRY,    	/* for loopback only: receive entry type of AFL */
        EE_RSCAN_FRAME_DATA,     	/* RTR data frame configuration */
        EE_RSCAN_ID_STD          	/* standard frame configuration */
    },
    {
        EE_RSCAN_MASK_IDDONTCARE,  	/* mask is open for all IDs */
        RESERVED,
        EE_RSCAN_MASK_DONTCARE,  	/* any type of frame is accepted */
        EE_RSCAN_MASK_DONTCARE
    },
    {
        EE_RSCAN_AFL_RXFIF0_NONE,	/* this AFL entry only receives in */
        RESERVED,			/* reserved */
        EE_RSCAN_AFL_COMFIFO_C0E0, 	/* COM FIFO 1 Channel 0 for reception */
        RESERVED,			/* reserved */
        0,            			/* RX MB Box pointer */
        EE_RSCAN_CLEAR,             	/* RX Box Number pointer is invalid */
        0x000,          		/* Receive HRH pointer - to be replaced with actual value */
        EE_RSCAN_DLCCHECK_DISABLE   	/* to enable DLC check, enter DLC here */
    }
};

// Akzeptansfilter Konfiguration
const struct ee_rscan_a_afl EE_RSCAN_AFL_any_MSG_CF_EXTERN =
{
    {
        0x0000000,               	/* Default ID, not relevant because mask is open */
        EE_RSCAN_AFL_EXTERN_ENTRY,    	/* for loopback only: receive entry type of AFL */
        EE_RSCAN_FRAME_DATA,     	/* RTR data frame configuration */
        EE_RSCAN_ID_STD          	/* standard frame configuration */
    },
    {
        EE_RSCAN_MASK_IDDONTCARE,  	/* mask is open for all IDs */
        RESERVED,
        EE_RSCAN_MASK_DONTCARE,  	/* any type of frame is accepted */
        EE_RSCAN_MASK_DONTCARE
    },
    {
        EE_RSCAN_AFL_RXFIF0_EN0,	/* this AFL entry only receives in */
        RESERVED,			/* reserved */
        EE_RSCAN_AFL_COMFIFO_NONE, 	/* COM FIFO 1 Channel 0 for reception */
        RESERVED,			/* reserved */
        0,            			/* RX MB Box pointer */
        EE_RSCAN_CLEAR,             	/* RX Box Number pointer is invalid */
        0x000,          		/* Receive HRH pointer - to be replaced with actual value */
        EE_RSCAN_DLCCHECK_DISABLE   	/* to enable DLC check, enter DLC here */
    }
};


// Akzeptansfilter Konfiguration
const struct ee_rscan_a_afl EE_RSCAN_AFL_RX_MSG_ID20 =
{
    {
        20,               		/* Default ID, not relevant because mask is open */
        EE_RSCAN_AFL_EXTERN_ENTRY,    	/* for loopback only: receive entry type of AFL */
        EE_RSCAN_FRAME_DATA,     	/* RTR data frame configuration */
        EE_RSCAN_ID_STD          	/* standard frame configuration */
    },
    {
        EE_RSCAN_MASK_IDDONTCARE,  	/* mask is open for all IDs */
        RESERVED,
        EE_RSCAN_MASK_DONTCARE,  	/* any type of frame is accepted */
        EE_RSCAN_MASK_DONTCARE
    },
    {
        EE_RSCAN_AFL_RXFIF0_EN0,	/* this AFL entry only receives in */
        RESERVED,			/* reserved */
        EE_RSCAN_AFL_COMFIFO_NONE, 	/* COM FIFO 1 Channel 0 for reception */
        RESERVED,			/* reserved */
        0,            			/* RX MB Box pointer */
        EE_RSCAN_SET,             	/* RX Box Number pointer is invalid */
        0x000,          		/* Receive HRH pointer - to be replaced with actual value */
        EE_RSCAN_DLCCHECK_DISABLE   	/* to enable DLC check, enter DLC here */
    }
};

// Akzeptansfilter Konfiguration
const struct ee_rscan_a_afl EE_RSCAN_AFL_RX_MSG_LOOP_ID20 =
{
    {
        20,               		/* Default ID, not relevant because mask is open */
        EE_RSCAN_AFL_EXTERN_ENTRY,    	/* for loopback only: receive entry type of AFL */
        EE_RSCAN_FRAME_DATA,     	/* RTR data frame configuration */
        EE_RSCAN_ID_STD          	/* standard frame configuration */
    },
    {
        EE_RSCAN_MASK_IDFULLCAN,  	/* mask is open for all IDs */
        RESERVED,
        EE_RSCAN_MASK_FILTER,  		/* any type of frame is accepted */
        EE_RSCAN_MASK_FILTER
    },
    {
        EE_RSCAN_AFL_RXFIF0_NONE,	/* this AFL entry only receives in */
        RESERVED,			/* reserved */
        EE_RSCAN_AFL_COMFIFO_NONE, 	/* COM FIFO 1 Channel 0 for reception */
        RESERVED,			/* reserved */
        1,            			/* RX MB Box pointer */
        EE_RSCAN_SET,             	/* RX Box Number pointer is invalid */
        0x000,          		/* Receive HRH pointer - to be replaced with actual value */
        EE_RSCAN_DLCCHECK_DISABLE   	/* to enable DLC check, enter DLC here */
    }
};

// Akzeptansfilter Konfiguration
const struct ee_rscan_a_afl EE_RSCAN_AFL_any_MSG_RX_MB =
{
    {
        0x0000000,               	/* Default ID, not relevant because mask is open */
        EE_RSCAN_AFL_EXTERN_ENTRY,    	/* for loopback only: receive entry type of AFL */
        EE_RSCAN_FRAME_DATA,     	/* RTR data frame configuration */
        EE_RSCAN_ID_STD          	/* standard frame configuration */
    },
    {
        EE_RSCAN_MASK_IDDONTCARE,  	/* mask is open for all IDs */
        RESERVED,
        EE_RSCAN_MASK_DONTCARE,  	/* any type of frame is accepted */
        EE_RSCAN_MASK_DONTCARE
    },
    {
        EE_RSCAN_AFL_RXFIF0_NONE,	/* this AFL entry only receives in */
        RESERVED,			/* reserved */
        EE_RSCAN_AFL_COMFIFO_C0E0, 	/* COM FIFO 1 Channel 0 for reception */
        RESERVED,			/* reserved */
        0,            			/* RX MB Box pointer */
        EE_RSCAN_CLEAR,             	/* RX Box Number pointer is invalid */
        0x000,          		/* Receive HRH pointer - to be replaced with actual value */
        EE_RSCAN_DLCCHECK_DISABLE   	/* to enable DLC check, enter DLC here */
    }
};
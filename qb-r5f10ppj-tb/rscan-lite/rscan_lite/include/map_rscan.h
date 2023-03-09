/*===========================================================================*/
/* Module : map_rscan_p.h                                                    */
/* Version: V1.04                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN Hardware Mapping Definition                                */
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
/*              V1.04: Changed main Clock ( X1 )                             */
/*                                                                           */
/*===========================================================================*/

#ifndef _MAP_RSCAN_H
#define _MAP_RSCAN_H

/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
/* IAR Systems Includes                                                      */
#include "device.h"

#include "rscan.h"

/*-------------------------------------------------------- */
#define EE_RSCAN_MACROS   		1
#define EE_RSCAN_CHANNEL_TYPE   1
/*-------------------------------------------------------- */
/* Frequencies in Hertz and floating point */
/* Source: MOSC / PLL / CPU */
#define PERIPHERAL_FREQUENCY	( ( float )( 32.0E6 ) )
/* Source: PLL */
#define CLEAN_FREQUENCY     	( ( float )( 16.0E6 ) )
/* Source: MOSC/DIV */
#define PLL_BYPASS_FREQUENCY   ( ( float )( 4.0E6 ) )
/*-------------------------------------------------------- */
/* ADDRESS MAPPING */
#define EE_RSCAN_BASE           ( 0x0300u )

#define EE_RSCAN_OFFSET_COMMON  ( 0x0000u )
#define EE_RSCAN_OFFSET_AFL     ( 0x00A0u )
#define EE_RSCAN_OFFSET_RXMSG   ( 0x00A0u )
#define EE_RSCAN_OFFSET_RXFIFO  ( 0x02A0u )
#define EE_RSCAN_OFFSET_COMFIFO ( 0x02E0u )
#define EE_RSCAN_OFFSET_TXMSG   ( 0x0300u )
#define EE_RSCAN_OFFSET_TXHL    ( 0x0380u )
#define EE_RSCAN_OFFSET_RAM     ( 0x03A0u )
/*-------------------------------------------------------- */
#define EE_RSCAN_MAXBAUDRATE 		( ( u32 ) (1000000u))
#define EE_RSCAN_MAXBAUDRATEPLLBP	( ( u32 ) (1000000u))
/*-------------------------------------------------------- */

/*-------------------------------------------------------- */
#define EE_RSCAN_INTCLEAR       ( 0x0080u ) // Interrupt mask bit
/*-------------------------------------------------------- */
/* The RSCAN Memory Mapping */

static const u08 ee_rscan_channels[ EE_RSCAN_MACROS ] =
{
    1                         /* unit 0 */
};

static struct ee_rscan_common *ee_rscan_common_p[ EE_RSCAN_MACROS ] =
{
    ( ee_rscan_common * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_COMMON + 0x00000u ) /* unit 0 */
};

static struct ee_rscan_aflpage *ee_rscan_aflpage_p[ EE_RSCAN_MACROS ] =
{
    ( ee_rscan_aflpage * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_AFL + 0x00000u )   /* unit 0 */
};

static struct ee_rscan_rxmsg *ee_rscan_rxmsg_p[ EE_RSCAN_MACROS ] =
{
    ( ee_rscan_rxmsg * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_RXMSG + 0x00000u )   /* unit 0 */
};

static struct ee_rscan_txmsg *ee_rscan_txmsg_p[ EE_RSCAN_MACROS ] [ EE_RSCAN_CHANNELS ] =
{
    ( ee_rscan_txmsg * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_TXMSG + 0x00000u )   /* unit 0, ch 0 */
};

static struct ee_rscan_rxfifo *ee_rscan_rxfifo_p[ EE_RSCAN_MACROS ] =
{
    ( ee_rscan_rxfifo * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_RXFIFO + 0x00000u ) /* unit 0 */
};

static struct ee_rscan_comfifo *ee_rscan_comfifo_p[ EE_RSCAN_MACROS ] [ EE_RSCAN_CHANNELS ] =
{
    ( ee_rscan_comfifo * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_COMFIFO + 0x00000u ) /* unit 0, ch 0 */
};

static struct ee_rscan_thl *ee_rscan_thl_p[ EE_RSCAN_MACROS ] [ EE_RSCAN_CHANNELS ] =
{
    ( ee_rscan_thl * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_TXHL + 0x00000u )  /* unit 0, ch 0 */
};

static struct ee_rscan_ram *ee_rscan_ram_p[ EE_RSCAN_MACROS ] =
{
    ( ee_rscan_ram * )( EE_RSCAN_BASE + EE_RSCAN_OFFSET_RAM + 0x00000u )     /* unit 0, ch 0 */
};


#endif

/*===========================================================================*/
/* Module : low_level_initialization.c                                       */
/* Version: V1.04                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: basic microcontroller initialization                             */
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
/*              Device:          R5F10PPJ                                    */
/*              Target Hardware: QB-R5F10PPJ-TB                              */
/*              IDE:             IAR Systems                                 */
/*                               Embedded Workbench for RL78 V1.30.x         */
/*                                                                           */
/*===========================================================================*/
/* History:                                                                  */
/*              V1.00: Initial version                                       */
/*              V1.04: Enable/provide X1 to CAN Communication Clock          */
/*                                                                           */
/*===========================================================================*/


/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
/* IAR Systems Includes                                                      */
#include <intrinsics.h>
#include <ior5f10ppj.h>
#include <ior5f10ppj_ext.h>
#include <stdlib.h>


/* Application Includes                                                      */
#include "ree_types.h"
#include "low_level_initialization.h"

/*===========================================================================*/
/* Constants                                                                 */
/*===========================================================================*/
/* Option Byte Definition                                                    */
/* watchdog disabled, LVD off, 64MHz High-Speed on-chip clock,
   OCD interface enabled                                                     */
__root __far const uint8_t OptionByte[OPT_BYTES_SIZE] @ 0x00C0u =
{
    0x6Eu, 0xFFu, 0xF8u, 0x85u,
};

/* Security Byte Definition                                                  */
__root __far const uint8_t SecuIDCode[SECU_ID_SIZE]   @ 0x00C4u =
{
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
};


/*===========================================================================*/
/* Prototypes                                                                */
/*===========================================================================*/
void WDT_Restart(void);
#pragma diag_suppress=Pm011
int __low_level_init (void);
#pragma diag_default=Pm011

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/
#pragma diag_suppress=Pm011
int __low_level_init (void)
#pragma diag_default=Pm011
{
    uint8_t wait_u08;

    /* -------------------------------------------------------------------------*/
    /* oscillator settings                                                      */
    /* -------------------------------------------------------------------------*/
    CMC     = 0x42u;   /* Crystal/ceramic resonator connection, less 10 MHz */
    OSTS    = 0x02u;   /* wait at least 102,4 µs                            */
    CSC     &= ~( 1u << 7u ); /* Clear MSTOP Bit                             */
    while ( !(OSTC && (1u << 5u) ) );

    CKC     &= ~( 1u << 6u);      /*  f_CLK   */
    while ( (CKC && (1u << 7u))); /* if not 0 */

    PLLCTL  = 0x42u;     /* PLL clock =< 32 MHz; PLL source clock 4 MHz; PLL_CLK > 32MHz; DIV 2 ; MULT 16 */
    for( wait_u08 = 0u; wait_u08 < 200u; wait_u08++) asm("nop");

    PLLCTL |= ( 1u << 0u );
    while ( (PLLSTS && ( 1u << 7u) ));

    PLLCTL |= ( 1u << 2u);
    while ( (PLLSTS && ( 1u << 3u) ));

    PER2  |= (1u << 0u); /* CAN Clock Enable / P_CLK */
    CANCKSEL = 1;        /* Enable X1 as Communication Clock CAN */

    /* -------------------------------------------------------------------------*/
    /* Safety Configuration                                                     */
    /* -------------------------------------------------------------------------*/
    CRC0CTL = 0x00u;           /* disable CRC calculation                       */
    IAWCTL  = 0x80u;           /* enable detection of invalid memory access     */

    return (int)(ENABLE_VAR_INIT);
}


void WDT_Restart(void)
{
    WDTE = 0xACu;                        /* restart watchdog timer              */
}

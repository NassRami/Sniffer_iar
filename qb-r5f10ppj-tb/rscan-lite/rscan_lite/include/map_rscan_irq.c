/*===========================================================================*/
/* Module : map_rscan_irq.c                                                  */
/* Version: V1.04                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN Interrupt Vectors                                          */
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

/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
#include "device.h"
#include "rscan.h"

extern u08 EE_RSCAN_InterruptFlag_Global_u08;
extern u08 EE_RSCAN_InterruptFlag_Channel0_u08;

extern void EE_RSCAN_Interrupt( void );

#pragma vector=INTCANGRFR_vect
__interrupt void INTRCANGRECC (void)
{
    // EITRAP_ENTRY();
    EE_RSCAN_InterruptFlag_Global_u08 = EE_RSCAN_INT_RXF;
    EE_RSCAN_Interrupt( );
    // EITRAP_LEAVE();
}

#pragma vector=INTCANGERR_vect
__interrupt void INTRCANGERR (void)
{
    // EITRAP_ENTRY();
    EE_RSCAN_InterruptFlag_Global_u08 = EE_RSCAN_INT_GERR;
    EE_RSCAN_Interrupt( );
    // EITRAP_LEAVE();
}

#pragma vector=INTCAN0TRM_vect
__interrupt void INTRCAN0TRX (void)
{
    // EITRAP_ENTRY();
    EE_RSCAN_InterruptFlag_Channel0_u08 = EE_RSCAN_INT_TX;
    EE_RSCAN_Interrupt( );
    // EITRAP_LEAVE();
}

#pragma vector=INTCAN0CFR_vect
__interrupt void INTRCAN0REC (void)
{
    // EITRAP_ENTRY();
    EE_RSCAN_InterruptFlag_Channel0_u08 = EE_RSCAN_INT_RXCF;
    EE_RSCAN_Interrupt( );
    // EITRAP_LEAVE();
}

#pragma vector=INTCAN0ERR_vect
__interrupt void INTRCAN0ERR (void)
{
    // EITRAP_ENTRY();
    EE_RSCAN_InterruptFlag_Channel0_u08 = EE_RSCAN_INT_CERR;
    EE_RSCAN_Interrupt( );
    // EITRAP_LEAVE();
}

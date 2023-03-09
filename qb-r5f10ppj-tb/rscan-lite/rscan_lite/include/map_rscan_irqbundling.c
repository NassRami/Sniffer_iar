/*===========================================================================*/
/* Module : map_rscan_irqbundling.c                                          */
/* Version: V1.04                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN Hardware Interrupt Bundling Decoder                        */
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

#include "ree_types.h"
#include "map_rscan.h"

void RSCAN_IntBundlingDecode( u08 *GlobalInterruptFlag_pu08,
                              u08 *MachineInterruptFlag0_pu08,
                              u08 *MachineInterruptFlag1_pu08,
                              u08 *MachineInterruptFlag2_pu08,
                              u08 *MachineInterruptFlag3_pu08,
                              u08 *MachineInterruptFlag4_pu08 )
{
    u08 FIFONumber_u08;
    u08 TXIRQBundling_u08;

    if( *GlobalInterruptFlag_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *GlobalInterruptFlag_pu08 )
        {
        case EE_RSCAN_INT_GERR:
            break;

        case EE_RSCAN_INT_RXF0:
        case EE_RSCAN_INT_RXF1:
            for( FIFONumber_u08 = 0;
                    FIFONumber_u08 < EE_RSCAN_MAXRXFIFOS;
                    FIFONumber_u08++ )
            {
                if( ee_rscan_common_p[ 0 ]->rfsts[ FIFONumber_u08 ].rfif )
                {
                    *GlobalInterruptFlag_pu08 = EE_RSCAN_INT_RXF0 + FIFONumber_u08;
                }
            }
            break;

        default:
            break;
        }
    }

    if( *MachineInterruptFlag0_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *MachineInterruptFlag0_pu08 )
        {
        case EE_RSCAN_INT_CERR:
        case EE_RSCAN_INT_RXCF:
            break;

        case EE_RSCAN_INT_TX:
            TXIRQBundling_u08 = ( u08 )( ee_rscan_common_p[ 0 ]->gtintsts[ 0 ] & 0x000000FF );
            if( TXIRQBundling_u08 & 0x01 ) *MachineInterruptFlag0_pu08 = EE_RSCAN_INT_TX;
            if( TXIRQBundling_u08 & 0x02 ) *MachineInterruptFlag0_pu08 = EE_RSCAN_INT_TXA;
            if( TXIRQBundling_u08 & 0x04 ) *MachineInterruptFlag0_pu08 = EE_RSCAN_INT_TXQ;
            if( TXIRQBundling_u08 & 0x08 ) *MachineInterruptFlag0_pu08 = EE_RSCAN_INT_TXCF;
            if( TXIRQBundling_u08 & 0x10 ) *MachineInterruptFlag0_pu08 = EE_RSCAN_INT_TXHL;
            break;

        default:
            break;
        }
    }

    if( *MachineInterruptFlag1_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *MachineInterruptFlag1_pu08 )
        {
        case EE_RSCAN_INT_CERR:
        case EE_RSCAN_INT_RXCF:
            break;

        case EE_RSCAN_INT_TX:
            TXIRQBundling_u08 = ( u08 )( ( ee_rscan_common_p[ 0 ]->gtintsts[ 0 ] & 0x0000FF00 ) >> 8 );
            if( TXIRQBundling_u08 & 0x01 ) *MachineInterruptFlag1_pu08 = EE_RSCAN_INT_TX;
            if( TXIRQBundling_u08 & 0x02 ) *MachineInterruptFlag1_pu08 = EE_RSCAN_INT_TXA;
            if( TXIRQBundling_u08 & 0x04 ) *MachineInterruptFlag1_pu08 = EE_RSCAN_INT_TXQ;
            if( TXIRQBundling_u08 & 0x08 ) *MachineInterruptFlag1_pu08 = EE_RSCAN_INT_TXCF;
            if( TXIRQBundling_u08 & 0x10 ) *MachineInterruptFlag1_pu08 = EE_RSCAN_INT_TXHL;
            break;

        default:
            break;
        }
    }

    if( *MachineInterruptFlag2_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *MachineInterruptFlag2_pu08 )
        {
        case EE_RSCAN_INT_CERR:
        case EE_RSCAN_INT_RXCF:
            break;

        case EE_RSCAN_INT_TX:
            TXIRQBundling_u08 = ( u08 )( ( ee_rscan_common_p[ 0 ]->gtintsts[ 0 ] & 0x00FF0000 ) >> 16 );
            if( TXIRQBundling_u08 & 0x01 ) *MachineInterruptFlag2_pu08 = EE_RSCAN_INT_TX;
            if( TXIRQBundling_u08 & 0x02 ) *MachineInterruptFlag2_pu08 = EE_RSCAN_INT_TXA;
            if( TXIRQBundling_u08 & 0x04 ) *MachineInterruptFlag2_pu08 = EE_RSCAN_INT_TXQ;
            if( TXIRQBundling_u08 & 0x08 ) *MachineInterruptFlag2_pu08 = EE_RSCAN_INT_TXCF;
            if( TXIRQBundling_u08 & 0x10 ) *MachineInterruptFlag2_pu08 = EE_RSCAN_INT_TXHL;
            break;

        default:
            break;
        }
    }

    if( *MachineInterruptFlag3_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *MachineInterruptFlag3_pu08 )
        {
        case EE_RSCAN_INT_CERR:
        case EE_RSCAN_INT_RXCF:
            break;

        case EE_RSCAN_INT_TX:
            TXIRQBundling_u08 = ( u08 )( ( ee_rscan_common_p[ 0 ]->gtintsts[ 0 ] & 0xFF000000 ) >> 16 );
            if( TXIRQBundling_u08 & 0x01 ) *MachineInterruptFlag3_pu08 = EE_RSCAN_INT_TX;
            if( TXIRQBundling_u08 & 0x02 ) *MachineInterruptFlag3_pu08 = EE_RSCAN_INT_TXA;
            if( TXIRQBundling_u08 & 0x04 ) *MachineInterruptFlag3_pu08 = EE_RSCAN_INT_TXQ;
            if( TXIRQBundling_u08 & 0x08 ) *MachineInterruptFlag3_pu08 = EE_RSCAN_INT_TXCF;
            if( TXIRQBundling_u08 & 0x10 ) *MachineInterruptFlag3_pu08 = EE_RSCAN_INT_TXHL;
            break;

        default:
            break;
        }
    }

    if( *MachineInterruptFlag4_pu08 != EE_RSCAN_INT_NOINT )
    {
        switch( *MachineInterruptFlag4_pu08 )
        {
        case EE_RSCAN_INT_CERR:
        case EE_RSCAN_INT_RXCF:
            break;

        case EE_RSCAN_INT_TX:
            TXIRQBundling_u08 = ( u08 )( ee_rscan_common_p[ 0 ]->gtintsts[ 1 ] & 0x000000FF );
            if( TXIRQBundling_u08 & 0x01 ) *MachineInterruptFlag4_pu08 = EE_RSCAN_INT_TX;
            if( TXIRQBundling_u08 & 0x02 ) *MachineInterruptFlag4_pu08 = EE_RSCAN_INT_TXA;
            if( TXIRQBundling_u08 & 0x04 ) *MachineInterruptFlag4_pu08 = EE_RSCAN_INT_TXQ;
            if( TXIRQBundling_u08 & 0x08 ) *MachineInterruptFlag4_pu08 = EE_RSCAN_INT_TXCF;
            if( TXIRQBundling_u08 & 0x10 ) *MachineInterruptFlag4_pu08 = EE_RSCAN_INT_TXHL;
            break;

        default:
            break;
        }
    }
}

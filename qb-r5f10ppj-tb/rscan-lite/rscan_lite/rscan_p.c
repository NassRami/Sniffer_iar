/*===========================================================================*/
/* Module : rscan_p.c                                                        */
/* Version: V1.04                                                            */
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
/*              V1.03: Improved Interrupt Handling function                  */
/*                                                                           */
/*===========================================================================*/

#include "device.h"

#include <ree_types.h>

#include "rscan.h"
#include "map_rscan.h"

#include <stddef.h>

#if( ( EE_RSCAN_CHANNELS > 1 ) || ( EE_RSCAN_MACROS > 1 ) )
#error "NUMBER OF CHANNELS OR UNITS NOT SUPPORTED"
#endif

/*
* CreateInterrupt() Jump Vector Table
*/

///////////////////////////////////////////////////////////////////////////////
//	----------------------------------
//	Generell Configuration Procedure
//	----------------------------------
//	1. Exit Global Sleep Mode
//	2. Exit Channel Sleep Mode
//	3. Global Configuration Settings
//	4. Rule Table Config ( AFL )
//	5. Message Buffer Config ( FIFO, RX MB, TX Queue)
//	6. all active Channel Config ( baudrate, bit timing, bus-off, error )
//  7. Interrupt Configure
//	8. Global Mode -> Operation Mode
//	9. Channel Mode -> Operation Mode
//
///////////////////////////////////////////////////////////////////////////////
// extern function
// -> reference to header
// intern function



void EE_RSCAN_IRQDefault( );

// RSCAN 1CH -> x Interrupts					1		*	(	1 + 2		 + 			7			*		1 )
static void ( *EE_RSCAN_CIVector [ ( EE_RSCAN_MACROS * ( EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNELS ) ) ] )( ) =
{
    EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault, EE_RSCAN_IRQDefault
};

/*
* Global Variables
*/

/* Number of interrupt vector */

u08 EE_RSCAN_InterruptFlag_Global_u08   = EE_RSCAN_INT_NOINT;  /* Unit 0 */
u08 EE_RSCAN_InterruptFlag_Channel0_u08 = EE_RSCAN_INT_NOINT;  /* Unit 0 */

u08 EE_RSCAN_LastErrorCode_Global_u08   = EE_RSCAN_GINT_NONE;
u08 EE_RSCAN_InterruptFlag_Channel_u08[ EE_RSCAN_CHANNELS ] = { EE_RSCAN_INT_NOINT };
u16 EE_RSCAN_LastErrorCode_Channel_u16[ EE_RSCAN_CHANNELS ] = { EE_RSCAN_ERROR_CLEARALL };

u08 EE_RSCAN_LastTxObjectChannel0_u08   = EE_RSCAN_INT_NOINT;

/*
* Internal Support Functions
*/

void EE_RSCAN_IRQDefault( )
{
    asm("nop");
}

u08 EE_RSCAN_GetTxBuffer( u32 FlagRegister_u32 )
{
    u08 CurrentScan_u08;

    for( CurrentScan_u08 = 0; CurrentScan_u08 < EE_RSCAN_MAXTXBUFFERS; CurrentScan_u08++ )
    {
        if( ( ( FlagRegister_u32 >> CurrentScan_u08 ) & 0x01 ) == 0x01 )
        {
            return( CurrentScan_u08 );
        }
    }
    return( EE_RSCAN_INT_NOINT );
}

u08 EE_RSCAN_GetComFIFOIntStatus( u08 Channel_u08, u08 InterruptType_u08 )   // to check
{
    u08 FlagSet_u08;
    u08 FlagFirst_u08;

    switch( InterruptType_u08 )
    {
    case EE_RSCAN_INT_RXCF:
        if( ee_rscan_common_p[ 0 ]->cfsts.cfrxif )
        {
            FlagSet_u08 = ( u08 )( 1 );
        }
        break;
    case EE_RSCAN_INT_TXCF:
        if( ee_rscan_common_p[ 0 ]->cfsts.cftxif )
        {
            FlagSet_u08 = ( u08 )( 1 );
        }
        break;
    }

    for( FlagFirst_u08 = 0; FlagFirst_u08 < EE_RSCAN_MAXCOMFIFOS; FlagFirst_u08++ )
    {
        if( FlagSet_u08 & ( 1 << FlagFirst_u08 ) ) return( FlagFirst_u08 );
    }
    return( EE_RSCAN_GLOBAL );
}

bit EE_RSCAN_SetIDData( struct ee_rscan_t_mask  *Id, volatile u16 *DataLow0, volatile u16 *DataLow1, volatile u16 *DataHigh0, volatile u16 *DataHigh1, struct ee_rscan_message *Message )
{
    u08 DataCount_u08;

    Id->id    = Message->hdr.id;
    Id->thlen = Message->hdr.thlen;
    Id->rtr   = Message->hdr.rtr;
    Id->ide   = Message->hdr.ide;

    *DataLow0   = 0;
    *DataLow1   = 0;
    *DataHigh0  = 0;
    *DataHigh1  = 0;

    for( DataCount_u08 = 0; DataCount_u08 < Message->flag.dlc; DataCount_u08++ )
    {
        if( DataCount_u08 < 2 )
        {
            *DataLow0 += ( Message->data[ DataCount_u08 ] << ( 8 * DataCount_u08 ) );
        }
        else if ( DataCount_u08 < 4 )
        {
            *DataLow1 += ( Message->data[ DataCount_u08 ] << ( 8 * ( DataCount_u08 - 2)) );
        }
        else if ( DataCount_u08 < 6 )
        {
            *DataHigh0 += ( Message->data[ DataCount_u08 ] << ( 8 * ( DataCount_u08 - 4)) );
        }
        else if ( DataCount_u08 < 8 )
        {
            *DataHigh1 += ( Message->data[ DataCount_u08 ] << ( 8 * ( DataCount_u08 - 6)) );
        }
        else
        {
            return false;
        }
    }

    return true;
}

bit EE_RSCAN_GetMessage( struct ee_rscan_r_rmsg  *Buffer, struct ee_rscan_message *Message )
{
    u08 DataCount_u08;

    Message->hdr.id  = Buffer->id.id;
    Message->hdr.rtr = Buffer->id.rtr;
    Message->hdr.ide = Buffer->id.ide;

    Message->flag.ts  = Buffer->ptr.ts;
    Message->flag.ptr = Buffer->ptr.ptr;
    Message->flag.dlc = Buffer->ptr.dlc;

    for( DataCount_u08 = 0; DataCount_u08 < Message->flag.dlc; DataCount_u08++ )
    {
        if( DataCount_u08 < 2 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dl0 >> ( 8 * DataCount_u08 ) );
        }
        else if ( DataCount_u08 < 4 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dl1 >> ( 8 * ( DataCount_u08 - 2 )));
        }
        else if ( DataCount_u08 < 6 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dh0 >> ( 8 * ( DataCount_u08 - 4 )));
        }
        else if ( DataCount_u08 < 8 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dh1 >> ( 8 * ( DataCount_u08 - 6 )));
        }
        else
        {
            return false;
        }
    }
    return true;
}

bit EE_RSCAN_GetMessageCF( struct ee_rscan_r_cmsg  *Buffer, struct ee_rscan_message *Message )
{
    u08 DataCount_u08;

    Message->hdr.id  = Buffer->id.id;
    Message->hdr.rtr = Buffer->id.rtr;
    Message->hdr.ide = Buffer->id.ide;

    Message->flag.ts  = Buffer->ptr.ts;
    Message->flag.ptr = Buffer->ptr.ptr;
    Message->flag.dlc = Buffer->ptr.dlc;

    for( DataCount_u08 = 0; DataCount_u08 < Message->flag.dlc; DataCount_u08++ )
    {
        if( DataCount_u08 < 2 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dl0 >> ( 8 * DataCount_u08 ) );
        }
        else if ( DataCount_u08 < 4 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dl1 >> ( 8 * DataCount_u08 - 2 ) );
        }
        else if ( DataCount_u08 < 6 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dh0 >> ( 8 * DataCount_u08 - 4 ) );
        }
        else if ( DataCount_u08 < 8 )
        {
            Message->data[ DataCount_u08 ] = ( u08 )( Buffer->dh1 >> ( 8 * DataCount_u08 - 6 ) );
        }
        else
        {
            return false;
        }
    }

    return true;
}

/*
* The interrupt Functions
*/


//=============================================================================
// FunctionName: RSCAN_Interrupt
// IN  :         -
// OUT :         -
// Description : Central Interrupt Entry, clears pending bits and stores status
//
//=============================================================================
void EE_RSCAN_Interrupt(void)
{
    u08 FIFONumber_u08;

    /* Handling of Unit 0 */

    if( EE_RSCAN_InterruptFlag_Global_u08 != EE_RSCAN_INT_NOINT )
    {
        switch( EE_RSCAN_InterruptFlag_Global_u08 )
        {
        case EE_RSCAN_INT_GERR:
            EE_RSCAN_LastErrorCode_Global_u08 = ee_rscan_common_p[ 0 ]->gerfl.err;
            ee_rscan_common_p[ 0 ]->gerfl.err  = EE_RSCAN_GINT_NONE;
            EE_RSCAN_CIVector[ EE_RSCAN_INT_GERR ]( );
            break;
        case EE_RSCAN_INT_RXF:
            FIFONumber_u08 = 0;
            if( ee_rscan_common_p[ 0 ]->rfsts[0].rfif != 0 )
            {
                FIFONumber_u08 = 0;
            }
            else if ( ee_rscan_common_p[ 0 ]->rfsts[1].rfif != 0 )
            {
                FIFONumber_u08 = 1;
            }

            if( ee_rscan_common_p[ 0 ]->rfsts[ FIFONumber_u08 ].rfmlt )
            {
                EE_RSCAN_LastErrorCode_Global_u08 = EE_RSCAN_GINT_MSGLOST;
                ee_rscan_common_p[ 0 ]->rfsts[ FIFONumber_u08 ].rfmlt = 0;
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GERR ]( );
            }

            ee_rscan_common_p[ 0 ]->rfsts[ FIFONumber_u08 ].rfif = 0;
            EE_RSCAN_CIVector[ EE_RSCAN_INT_RXF0 + FIFONumber_u08 ]( );
            break;
        default:
            break;
        }
        EE_RSCAN_InterruptFlag_Global_u08 = EE_RSCAN_INT_NOINT;
    }
    else if( EE_RSCAN_InterruptFlag_Channel0_u08 != EE_RSCAN_INT_NOINT )
    {
        EE_RSCAN_InterruptFlag_Channel_u08[ EE_RSCAN_CHANNEL0 ] = EE_RSCAN_InterruptFlag_Channel0_u08;

        switch( EE_RSCAN_InterruptFlag_Channel0_u08 )
        {
        case EE_RSCAN_INT_TX:
            /* TX Buffer Complete Check */
            EE_RSCAN_LastTxObjectChannel0_u08 = EE_RSCAN_GetTxBuffer( ee_rscan_common_p[ 0 ]->tmtcsts[ 0 ]);
            if( EE_RSCAN_LastTxObjectChannel0_u08 != EE_RSCAN_INT_NOINT )
            {
                ee_rscan_common_p[ 0 ]->tmsts[ EE_RSCAN_LastTxObjectChannel0_u08 / 4 ] &= ( u32 )( ~( ( EE_RSCAN_TMSTS_RFMSK << ( EE_RSCAN_LastTxObjectChannel0_u08 % 4 ) ) ) );
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_TX + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]();
            }

            /* COM FIFO transmit */
            if ( ee_rscan_common_p[ 0 ]->cfsts.cftxif != 0 )
            {
                if( ee_rscan_common_p[ 0 ]->cfsts.cfmlt )
                {
                    EE_RSCAN_LastErrorCode_Global_u08 = EE_RSCAN_GINT_MSGLOST;
                    ee_rscan_common_p[ 0 ]->cfsts.cfmlt = 0;
                    EE_RSCAN_CIVector[ EE_RSCAN_INT_GERR ]( );
                }
                ee_rscan_common_p[ 0 ]->cfsts.cftxif = 0;
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_TXCF + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]( );
            }

            /* Transmit Abort Check */
            EE_RSCAN_LastTxObjectChannel0_u08 = EE_RSCAN_GetTxBuffer( ee_rscan_common_p[ 0 ]->tmtasts[ 0 ]);
            if( EE_RSCAN_LastTxObjectChannel0_u08 != EE_RSCAN_INT_NOINT )
            {
                ee_rscan_common_p[ 0 ]->tmsts[ EE_RSCAN_LastTxObjectChannel0_u08 / 4 ] &= ( u32 )( ~( ( EE_RSCAN_TMSTS_RFMSK << ( EE_RSCAN_LastTxObjectChannel0_u08 % 4 ) ) ) );
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_TXA + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]( );
            }
            /* Transmit History Check */
            if ( ee_rscan_common_p[ 0 ]->thlsts[ EE_RSCAN_CHANNEL0 ].irq != 0 )
            {
                ee_rscan_common_p[ 0 ]->thlsts[ EE_RSCAN_CHANNEL0 ].irq = 0;
                if( ee_rscan_common_p[ 0 ]->thlsts[ EE_RSCAN_CHANNEL0 ].elt )
                {
                    ee_rscan_common_p[ 0 ]->thlsts[ EE_RSCAN_CHANNEL0 ].elt = 0;
                    EE_RSCAN_LastErrorCode_Global_u08 = EE_RSCAN_GERROR_THLLOST;
                    EE_RSCAN_CIVector[ EE_RSCAN_INT_GERR ]( );
                }
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_TXHL + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]( );
            }


            break;

        case EE_RSCAN_INT_RXCF:
            if( ee_rscan_common_p[ 0 ]->cfsts.cfmlt )
            {
                EE_RSCAN_LastErrorCode_Global_u08 = EE_RSCAN_GINT_MSGLOST;
                ee_rscan_common_p[ 0 ]->cfsts.cfmlt = 0;
                EE_RSCAN_CIVector[ EE_RSCAN_INT_GERR ]( );
            }
            ee_rscan_common_p[ 0 ]->cfsts.cfrxif = 0;
            EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_RXCF + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]( );
            break;
        case EE_RSCAN_INT_CERR:
            EE_RSCAN_LastErrorCode_Channel_u16[ 0 ] = ( u16 )( ee_rscan_common_p[ 0 ]->ch[ EE_RSCAN_CHANNEL0 ].erfl.err );
            ee_rscan_common_p[ 0 ]->ch[ EE_RSCAN_CHANNEL0 ].erfl.err = EE_RSCAN_ERROR_CLEARALL;
            EE_RSCAN_CIVector[ EE_RSCAN_INT_GLOBAL + EE_RSCAN_INT_CERR + ( EE_RSCAN_INT_CHANNEL * EE_RSCAN_CHANNEL0 ) ]( );
            break;
        }
        EE_RSCAN_InterruptFlag_Channel0_u08 = EE_RSCAN_INT_NOINT;

    }
}


//=============================================================================
// FunctionName: EE_RSCAN_CreateInterrupt
// IN  :         Unit Number, Channel Number, Int Number, Int Level,
//               Int Function Vector
// OUT :         Success Status
// Description : Enables and Sets Interrupt Vector to given Function
//
//=============================================================================
bit EE_RSCAN_CreateInterrupt( u08  Unit_u08, u08  Channel_u08, u08  IntNumber_u08, u08 SetIntLevel_u08, void ( *FunctionVector )( ) )
{
    u08 IntLevel_u08 = SetIntLevel_u08;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( SetIntLevel_u08 > 3 ) return EE_RSCAN_ERROR;

    if( FunctionVector == NULL )
    {
        IntLevel_u08 	=  EE_RSCAN_INTCLEAR;
        FunctionVector 	=  EE_RSCAN_IRQDefault;
    }

    if( Channel_u08 == EE_RSCAN_GLOBAL )
    {
        EE_RSCAN_CIVector[ Unit_u08 * ( EE_RSCAN_CHANNELS * EE_RSCAN_INT_CHANNEL + EE_RSCAN_INT_GLOBAL ) + IntNumber_u08 ] = FunctionVector;
        switch( Unit_u08 )
        {
        case EE_RSCAN_0:
            switch( IntNumber_u08 )
            {
            case EE_RSCAN_INT_GERR:
                CANGERRPR0 =IntLevel_u08 & ( 1 << 0);
                CANGERRPR1 =IntLevel_u08 & ( 1 << 1);
                CANGERRMK = 0;
                break;
            case EE_RSCAN_INT_RXF0:
                CANGRFRPR0 =IntLevel_u08 & ( 1 << 0);
                CANGRFRPR0 =IntLevel_u08 & ( 1 << 1);
                CANGRFRMK  = 0;
                break;
            case EE_RSCAN_INT_RXF1:
                CANGRFRPR0 =IntLevel_u08 & ( 1 << 0);
                CANGRFRPR0 =IntLevel_u08 & ( 1 << 1);
                CANGRFRMK  = 0;
                break;

            }
            break;
        default:
            return( EE_RSCAN_ERROR );
        }
    }
    else
    {
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );

        EE_RSCAN_CIVector[ Unit_u08 * ( EE_RSCAN_CHANNELS * EE_RSCAN_INT_CHANNEL + EE_RSCAN_INT_GLOBAL ) + Channel_u08 * EE_RSCAN_INT_CHANNEL + EE_RSCAN_INT_GLOBAL + IntNumber_u08 ] = FunctionVector;
        switch( Unit_u08 )
        {
        case EE_RSCAN_0:
            switch( Channel_u08 )
            {
            case EE_RSCAN_CHANNEL0:
                switch( IntNumber_u08 )
                {
                case EE_RSCAN_INT_TX:
                    CAN0TRMPR0 = IntLevel_u08 & ( 1 << 0);
                    CAN0TRMPR1 = IntLevel_u08 & ( 1 << 1);
                    CAN0TRMMK  = 0;
                    break;
                case EE_RSCAN_INT_TXA:
                    CAN0TRMPR0 =IntLevel_u08 & ( 1 << 0);
                    CAN0TRMPR1 =IntLevel_u08 & ( 1 << 1);
                    CAN0TRMMK  = 0;
                    break;
                case EE_RSCAN_INT_CERR:
                    CAN0ERRPR0 =IntLevel_u08 & ( 1 << 0);
                    CAN0ERRPR1 =IntLevel_u08 & ( 1 << 1);
                    CAN0ERRMK  = 0;
                    break;
                case EE_RSCAN_INT_TXHL:
                    CAN0TRMPR0 =IntLevel_u08 & ( 1 << 0);
                    CAN0TRMPR1 =IntLevel_u08 & ( 1 << 1);
                    CAN0TRMMK  = 0;
                    break;
                case EE_RSCAN_INT_RXCF:
                    CAN0CFRPR0 =IntLevel_u08 & ( 1 << 0);
                    CAN0CFRPR0 =IntLevel_u08 & ( 1 << 1);
                    CAN0CFRMK  = 0;
                    break;
                case EE_RSCAN_INT_TXCF:
                    CAN0TRMPR0 =IntLevel_u08 & ( 1 << 0);
                    CAN0TRMPR1 =IntLevel_u08 & ( 1 << 1);
                    CAN0TRMMK  = 0;
                    break;
                }
                break;

            }
            break;
        default:
            return( EE_RSCAN_ERROR );
        }
    }

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_SetInterrupt
// IN  :         UnitNumber, Channel Number, Int Mask
// OUT :         Success Status
// Description : Enables or disables dedicated Interrupts
//
//=============================================================================
bit EE_RSCAN_SetInterrupt( u08 Unit_u08, u08 Channel_u08, u08 InterruptSelection_u08, u16 InterruptSubSelection_u16 )
{
    u16 RegBits_u16;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );

    if( Channel_u08 == EE_RSCAN_GLOBAL )
    {
        switch( InterruptSelection_u08 )
        {
        case EE_RSCAN_INT_GERR:
            ee_rscan_common_p[ Unit_u08 ]->gctr.ie = ( u32 )InterruptSubSelection_u16;
            break;
        default:   /* RX FIFO Interrupt selected */
            if( InterruptSelection_u08 > EE_RSCAN_MAXRXFIFOS ) return( EE_RSCAN_ERROR );
            ee_rscan_common_p[ Unit_u08 ]->rfcc[ InterruptSelection_u08 - 1].rfie = InterruptSubSelection_u16;
            break;
        }
    }
    else
    {
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );

        switch( InterruptSelection_u08 )
        {
        case EE_RSCAN_INT_TX:
        case EE_RSCAN_INT_TXA:
            if( InterruptSubSelection_u16 == EE_RSCAN_INT_NOINT )   /* clears all */
            {
                ee_rscan_common_p[ Unit_u08 ]->tmiec = 0x0000;
            }
            else     /* Sub selection defines the Tx Buffer IRQ to be enabled */
            {
                if( InterruptSubSelection_u16 >= EE_RSCAN_MAXTXBUFFERS ) return( EE_RSCAN_ERROR );
                RegBits_u16 = ( u16 )( 1 << ( InterruptSubSelection_u16 ) );
                ee_rscan_common_p[ Unit_u08 ]->tmiec |= RegBits_u16;
            }
            break;
        case EE_RSCAN_INT_CERR:
            ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.ie = InterruptSubSelection_u16;
            break;
        case EE_RSCAN_INT_TXHL:
            ee_rscan_common_p[ Unit_u08 ]->thlcc[ Channel_u08 ].ie = InterruptSubSelection_u16;
            break;
        case EE_RSCAN_INT_RXCF:
            if( InterruptSubSelection_u16 == EE_RSCAN_INT_NOINT )
            {
                /* clears all */
                ee_rscan_common_p[ Unit_u08 ]->cfcc.cfrxie = EE_RSCAN_INT_DISABLE;
            }
            else     /* Sub Index selects the COM FIFO number per channel */
            {
                ee_rscan_common_p[ Unit_u08 ]->cfcc.cfrxie = EE_RSCAN_INT_ENABLE;
            }
            break;
        case EE_RSCAN_INT_TXCF:
            if( InterruptSubSelection_u16 == EE_RSCAN_INT_NOINT )   /* clears all */
            {
                ee_rscan_common_p[ Unit_u08 ]->cfcc.cftxie = EE_RSCAN_INT_DISABLE;
            }
            else     /* Sub Index selects the COM FIFO number per channel */
            {
                ee_rscan_common_p[ Unit_u08 ]->cfcc.cftxie = EE_RSCAN_INT_ENABLE;
            }
            break;
        }
    }

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_GetError
// IN  :         UnitNumber, Channel Number
// OUT :         Success Status, Error States
// Description : Returns global or channel local error states
//
//=============================================================================
bit EE_RSCAN_GetError( u08  Unit_u08, u08  Channel_u08, pu16 InterruptErrorFlag_pu16, pu16 LastErrorFlag_pu16 )
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );

    if( Channel_u08 == EE_RSCAN_GLOBAL )
    {
        *InterruptErrorFlag_pu16 = ( u16 )EE_RSCAN_LastErrorCode_Global_u08;
        *LastErrorFlag_pu16      = ( u16 )( ee_rscan_common_p[ Unit_u08 ]->gerfl.err );
    }
    else
    {
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );

        *InterruptErrorFlag_pu16 = EE_RSCAN_LastErrorCode_Channel_u16[ Channel_u08 ];
        *LastErrorFlag_pu16      = ( u16 )( ee_rscan_common_p[ Unit_u08 ]->ch
                                            [ Channel_u08 ].erfl.err );
    }

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_GetStatus
// IN  :         UnitNumber, Channel Number
// OUT :         Success Status, Channel Status Values
// Description : Returns channel local status information flags
//
//=============================================================================
bit EE_RSCAN_GetStatus( u08 Unit_u08, u08  Channel_u08, u08  StatusNumber_u08, pu08 StatusValue_u08 )
{
    static u08 LocalStatusValue_THL;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( ( Channel_u08 >= EE_RSCAN_CHANNELS ) && ( Channel_u08 != EE_RSCAN_GLOBAL ) )return( EE_RSCAN_ERROR );

    switch( StatusNumber_u08 )
    {
    case EE_RSCAN_STATUS_OPMODE:              /* Operation mode of channel */
        ( ( Channel_u08 == EE_RSCAN_GLOBAL ) ? ( *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->gsts.mds ) :
          ( *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.mds ) );
        break;

    case EE_RSCAN_STATUS_PSMODE:              /* Operation mode of channel */
        ( ( Channel_u08 == EE_RSCAN_GLOBAL  ) ? ( *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->gsts.slps ) :
          ( *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.slps ) );
        break;

    case EE_RSCAN_STATUS_RECEIVE:        /* indicates channel is receiving */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.recs;
        break;

    case EE_RSCAN_STATUS_TRANSMIT:        /* indicates channel is sending */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.trxs;
        break;

    case EE_RSCAN_STATUS_BUSOFF:         /* indicates channel is bus off */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.boffs;
        break;

    case EE_RSCAN_STATUS_ERRCNTLEV:                       /* error level */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 )( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].erfl.err &
                                    ( EE_RSCAN_ERROR_WARNING | EE_RSCAN_ERROR_PASSIVE | EE_RSCAN_ERROR_BUSOFF ) );
        break;

    case EE_RSCAN_STATUS_TOVF:               /* send history overrun flag */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->thlsts[ Channel_u08 ].elt;
        if( *StatusValue_u08 )
        {
            ee_rscan_common_p[ Unit_u08 ]->thlsts[ Channel_u08 ].elt = 0;
        }
        break;

    case EE_RSCAN_STATUS_THPM:                 /* send history empty flag */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->thlsts[ Channel_u08 ].emp;
        break;

    case EE_RSCAN_STATUS_TGPT:                    /* send history pointer */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        if( !ee_rscan_common_p[ Unit_u08 ]->thlsts[ Channel_u08 ].emp )
        {

            ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1; // Select RAM Window 1
            LocalStatusValue_THL = ( u08 )ee_rscan_thl_p[ Unit_u08 ][ Channel_u08 ]->tid;
            if( LocalStatusValue_THL == EE_RSCAN_TID_NOTUSED )
            {
                LocalStatusValue_THL = ( u08 ) ( ee_rscan_thl_p[ Unit_u08 ][ Channel_u08 ]->bt );
                LocalStatusValue_THL = ( u08 ) ( (ee_rscan_thl_p[ Unit_u08 ][ Channel_u08 ]->bt ) + LocalStatusValue_THL );
            }
            *StatusValue_u08 = LocalStatusValue_THL;
        }
        else
        {
            *StatusValue_u08 = EE_RSCAN_TID_NOTUSED;
        }
        break;

    case EE_RSCAN_STATUS_NEWTXHISTORY:               /* get next THL entry */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        ee_rscan_common_p[ Unit_u08 ]->thlpctr[ Channel_u08 ] = EE_RSCAN_THL_NEXTELEMENT;
        break;

    case EE_RSCAN_STATUS_VALID:        /* channel has received sucessfully */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch
                           [ Channel_u08 ].sts.coms;
        break;

    case EE_RSCAN_STATUS_TRERRCOUNT:       /* transmit error counter value */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch
                           [ Channel_u08 ].sts.tec;
        break;

    case EE_RSCAN_STATUS_RXERRCOUNT:       /* receive error counter value */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch
                           [ Channel_u08 ].sts.rec;
        break;

    case EE_RSCAN_STATUS_ERRPLEV:                /* error counter passive */
        if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
        *StatusValue_u08 = ( u08 ) ee_rscan_common_p[ Unit_u08 ]->ch
                           [ Channel_u08 ].sts.eps;
        break;

    case EE_RSCAN_STATUS_INT_RXFIFO:
        *StatusValue_u08 = ( u08 )ee_rscan_common_p[ Unit_u08 ]->rfists;
        break;

    case EE_RSCAN_STATUS_INTERRUPT:
        if( Channel_u08 == EE_RSCAN_GLOBAL )
        {
            *StatusValue_u08 = EE_RSCAN_InterruptFlag_Global_u08;
            EE_RSCAN_InterruptFlag_Global_u08 = EE_RSCAN_INT_NOINT;
        }
        else
        {
            if( Channel_u08 >= EE_RSCAN_CHANNELS ) return( EE_RSCAN_ERROR );
            *StatusValue_u08 = EE_RSCAN_InterruptFlag_Channel_u08[ Channel_u08 ];
            EE_RSCAN_InterruptFlag_Channel_u08[ Channel_u08 ] = EE_RSCAN_INT_NOINT;
        }
        break;

    default:
        return( EE_RSCAN_ERROR );

    } // end switch( StatusNumber_u08 )

    return( EE_RSCAN_OK );
}


//=============================================================================
// FunctionName: EE_RSCAN_SetGlobalFIFOConfiguration
// IN  :         UnitNumber, Global configuration set
// Description : Performs Global Configurations
//
//=============================================================================

bit EE_RSCAN_SetGlobalFIFOConfiguration( u08 Unit_u08, const struct ee_rscan_cfg_global *Config )
{
    u08 CurrentFIFO_u08;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );

    /* Must be in global reset to set the global configuration! */

    if( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_OPER ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gsts.slps != 0 ) return( EE_RSCAN_ERROR );

    for( CurrentFIFO_u08 = 0; CurrentFIFO_u08 < EE_RSCAN_MAXRXFIFOS; CurrentFIFO_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfie =  Config->rfcc[ CurrentFIFO_u08 ].rfie;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfdc =  Config->rfcc[ CurrentFIFO_u08 ].rfdc;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfim =  Config->rfcc[ CurrentFIFO_u08 ].rfim;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfigcv =  Config->rfcc[ CurrentFIFO_u08 ].rfigcv;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfe =  Config->rfcc[ CurrentFIFO_u08 ].rfe;
    }
    return( EE_RSCAN_OK );
}


//=============================================================================
// FunctionName: EE_RSCAN_SetBittiming
// IN  :         InputFrequency, Bitrate, TQ per Bit, Sampling Point, SJW
// OUT :         Main Prescaler Setting, Bit Rate Prescaler Setting, Bit Timing
// Description : Calculates Settings from Clock Supply and Parameters
//
//=============================================================================

bit EE_RSCAN_SetBittiming( u32  InputFrequency_u32,
                           u32  Bitrate_u32,
                           u08  TQPerBit_u08,         // optional Parameter, can be 0
                           u08  SyncJumpWidth_u08,    // optional Parameter, can be 0
                           pu16 BRPSetting_pu16,
                           pu08 TSEG1Setting_pu08,
                           pu08 TSEG2Setting_pu08,
                           pu08 SJWSetting_pu08,
                           flt  SamplingPointPos_flt )   // optional Parameter, can be 0.0
{
    u16 TQPerBitUsed_u16 = ( u16 )TQPerBit_u08;
    u16 TQPerBitMinDeviation_u16;
    u16 BRPSettingUsed_u16;
    u08 TSEG1SettingUsed_u16;
    u08 TSEG2SettingUsed_u16;
    flt SamplingPointPosUsed_flt = SamplingPointPos_flt;
    u32 BitrateAchieved_u32;
    s32 MinBitrateDeviation_s32;
    s32 BitrateDeviation_s32;

    /* Automatic settings generation */

    if( TQPerBit_u08 == 0 ) TQPerBitUsed_u16 = ( u16 )( InputFrequency_u32 / ( Bitrate_u32 * EE_RSCAN_TRLAYERFACTOR ) );
    if( TQPerBitUsed_u16 > EE_RSCAN_BT_MAXTQ ) TQPerBitUsed_u16 = EE_RSCAN_BT_MAXTQ;
    if( TQPerBitUsed_u16 < EE_RSCAN_BT_MINTQ ) return( EE_RSCAN_ERROR );

    if( SamplingPointPos_flt == 0.0 ) SamplingPointPosUsed_flt = EE_RSCAN_BT_SPTOPTIMUM;

    /* Other plausibility checks */

    if( ( SyncJumpWidth_u08 != 0 ) && ( ( SyncJumpWidth_u08 < EE_RSCAN_BT_MINSJW ) || ( SyncJumpWidth_u08 > EE_RSCAN_BT_MAXSJW ) ) )
        return( EE_RSCAN_ERROR );

    if(( InputFrequency_u32 / Bitrate_u32 ) < EE_RSCAN_CLKMINFACTOR ) return( EE_RSCAN_ERROR );

    /* Maximum Bitrate check */

    if( ( InputFrequency_u32 / ( ( u32 )TQPerBitUsed_u16 * EE_RSCAN_TRLAYERFACTOR ) ) < Bitrate_u32 ) return( EE_RSCAN_ERROR );

    /* Minimum Bitrate check */

    if( ( InputFrequency_u32 / ( ( u32 )TQPerBitUsed_u16 * EE_RSCAN_TRLAYERFACTOR * Bitrate_u32 ) ) > EE_RSCAN_BT_MAXBRP )
        return( EE_RSCAN_ERROR );

    /* Maximum Prescaler check */

    BRPSettingUsed_u16 = ( u16 )( InputFrequency_u32 / ( Bitrate_u32 * ( u32 )TQPerBitUsed_u16 * EE_RSCAN_TRLAYERFACTOR ) );

    /* Frequency rounding optimization */

    BitrateAchieved_u32 = ( u32 )InputFrequency_u32 / ( ( u32 ) ( ( u32 )BRPSettingUsed_u16 * ( u32 )TQPerBitUsed_u16 * ( u32 )EE_RSCAN_TRLAYERFACTOR ) );

    MinBitrateDeviation_s32 = ( s32 )( BitrateAchieved_u32 - Bitrate_u32 );
    if( MinBitrateDeviation_s32 < 0L ) MinBitrateDeviation_s32 = -MinBitrateDeviation_s32;
    BitrateDeviation_s32    = MinBitrateDeviation_s32;

    while( BitrateAchieved_u32 != Bitrate_u32 )
    {
        TQPerBitUsed_u16--;
        if( TQPerBitUsed_u16 < EE_RSCAN_BT_MINTQ ) break;
        BRPSettingUsed_u16 = ( u16 )( InputFrequency_u32 / ( Bitrate_u32 * ( u32 )TQPerBitUsed_u16 * EE_RSCAN_TRLAYERFACTOR ) );

        if( BRPSettingUsed_u16 > EE_RSCAN_BT_MAXBRP ) return( EE_RSCAN_ERROR );

        BitrateAchieved_u32 = ( u32 )InputFrequency_u32 / ( ( u32 ) ( ( u32 )BRPSettingUsed_u16 * ( u32 )TQPerBitUsed_u16 * ( u32 )EE_RSCAN_TRLAYERFACTOR ) );

        BitrateDeviation_s32 = ( s32 )( BitrateAchieved_u32 - Bitrate_u32 );
        if( BitrateDeviation_s32 < 0L ) BitrateDeviation_s32 = -BitrateDeviation_s32;
        if( BitrateDeviation_s32 < MinBitrateDeviation_s32 )
        {
            MinBitrateDeviation_s32  = BitrateDeviation_s32;
            TQPerBitMinDeviation_u16 = TQPerBitUsed_u16;
        }
    }

    if( BitrateAchieved_u32 != Bitrate_u32 )   /* Optimum Bitrate was not found */
    {
        TQPerBitUsed_u16 = TQPerBitMinDeviation_u16;   /* use the best value instead */
        BRPSettingUsed_u16 = ( u16 )( InputFrequency_u32 / ( Bitrate_u32 * ( u32 )TQPerBitUsed_u16 * EE_RSCAN_TRLAYERFACTOR ) );
    }

    /* Sampling Point setting and segment splitting */

    TSEG1SettingUsed_u16 = ( u16 )( ( flt )( TQPerBitUsed_u16 - EE_RSCAN_BT_SYNCSEGLEN ) * SamplingPointPosUsed_flt );
    if( TSEG1SettingUsed_u16 < EE_RSCAN_BT_MINTSEG1 ) TSEG1SettingUsed_u16 = EE_RSCAN_BT_MINTSEG1;
    if( TSEG1SettingUsed_u16 > EE_RSCAN_BT_MAXTSEG1 ) TSEG1SettingUsed_u16 = EE_RSCAN_BT_MAXTSEG1;

    TSEG2SettingUsed_u16 = TQPerBitUsed_u16 - TSEG1SettingUsed_u16 - EE_RSCAN_BT_SYNCSEGLEN;
    if( TSEG2SettingUsed_u16 > EE_RSCAN_BT_MAXTSEG2 ) TSEG2SettingUsed_u16 = EE_RSCAN_BT_MAXTSEG2;
    if( TSEG2SettingUsed_u16 < EE_RSCAN_BT_MINTSEG2 ) TSEG2SettingUsed_u16 = EE_RSCAN_BT_MINTSEG2;

    TSEG1SettingUsed_u16 = TQPerBitUsed_u16 - TSEG2SettingUsed_u16 - EE_RSCAN_BT_SYNCSEGLEN;
    if( ( TSEG1SettingUsed_u16 < EE_RSCAN_BT_MINTSEG1 ) || ( TSEG1SettingUsed_u16 > EE_RSCAN_BT_MAXTSEG1 ) ) return( EE_RSCAN_ERROR );

    /* Synchronisation Jump Width setting */

    if( ( SyncJumpWidth_u08 > ( u08 )TSEG2SettingUsed_u16 ) || ( SyncJumpWidth_u08 == 0 ) )
    {
        *SJWSetting_pu08 = ( u08 )TSEG2SettingUsed_u16 - 1;
        if( *SJWSetting_pu08 >= EE_RSCAN_BT_MAXSJW )
            *SJWSetting_pu08 = EE_RSCAN_BT_MAXSJW - 1;
    }
    else
        *SJWSetting_pu08 = EE_RSCAN_BT_MAXSJW - 1;

    /* Definition of Register Values */

    *BRPSetting_pu16   = ( u16 )( BRPSettingUsed_u16 - 1 );
    *TSEG1Setting_pu08 = ( u08 )( TSEG1SettingUsed_u16 - 1 );
    *TSEG2Setting_pu08 = ( u08 )( TSEG2SettingUsed_u16 - 1 );

    return( EE_RSCAN_OK );
}


//=============================================================================
// FunctionName: EE_RSCAN_TxAbort
// IN  :         UnitNumber, Channel Number, Message
// OUT :         Success Status
// Description : Performs Message Sending
//=============================================================================
bit EE_RSCAN_TxAbort( u08  Unit_u08, u08  Channel_u08, struct ee_rscan_message *Message )
{
    u08 SendBox_u08;
    u08 FirstBox_u08;
    u08 LastBox_u08;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 >= ee_rscan_channels[ Unit_u08 ] )
        return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.mds != EE_RSCAN_CH_OPER )
        return( EE_RSCAN_ERROR );

    switch( Message->path )
    {
    case EE_RSCAN_PATH_MSGBOX:         /* direct abortion of message box */
        if( Message->pathdetail == EE_RSCAN_PATHDETAIL_ANY )   /* abort all */
        {
            FirstBox_u08 = 0;
            LastBox_u08  = EE_RSCAN_MAXTXBUFFERS - 1;
        }
        else
        {
            FirstBox_u08 = Message->pathdetail;
            LastBox_u08  = FirstBox_u08;
        }
        break;

    case EE_RSCAN_PATH_COMFIFO:        /* only one FIFO abort at a time */
        if( Message->pathdetail == EE_RSCAN_PATHDETAIL_ANY )
        {
            return( EE_RSCAN_ERROR );
        }
        else
        {
            SendBox_u08 = Message->pathdetail;
            if( SendBox_u08 >= EE_RSCAN_MAXCOMFIFOS )
            {
                return( EE_RSCAN_ERROR );
            }
            else
            {
                if( ee_rscan_common_p[ Unit_u08 ]->cfcc.cfe )
                {
                    FirstBox_u08 = ee_rscan_common_p[ Unit_u08 ]->cfcc.cftml;
                    LastBox_u08 = FirstBox_u08;
                }
                else return( EE_RSCAN_ERROR );
            }
        }
        break;
    default:
        return( EE_RSCAN_ERROR );
    }

    for( SendBox_u08 =  FirstBox_u08;
            SendBox_u08 <= LastBox_u08;
            SendBox_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->tmc[( EE_RSCAN_MAXTXBUFFERS * Channel_u08 + SendBox_u08 ) / 4 ] |= ( u32 ) ( EE_RSCAN_TMC_SET_AR << ( 8 * ( ( EE_RSCAN_MAXTXBUFFERS * Channel_u08 + SendBox_u08 ) % 4) ) );
    }

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_SendMessage
// IN  :         UnitNumber, Channel Number, Message
// OUT :         Success Status
// Description : Performs Message Sending
//=============================================================================
bit EE_RSCAN_SendMessage( u08  Unit_u08, u08 Channel_u08, u08 *Status_u08, struct ee_rscan_message *Message )
{
    u08 SendBox_u08;
    u08 FirstBox_u08;
    u08 LastBox_u08;
    u08 SendBoxOccupied_u08 = EE_RSCAN_CLEAR;

    *Status_u08 = EE_RSCAN_FAULT_PARAMETER;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 >= ee_rscan_channels[ Unit_u08 ] ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.mds != EE_RSCAN_CH_OPER ) return( EE_RSCAN_ERROR );

    *Status_u08 = EE_RSCAN_FAULT_NONE;

    switch( Message->path )
    {
    case EE_RSCAN_PATH_MSGBOX:      /* search a free SendBox, if required */
    {
        if( Message->pathdetail != EE_RSCAN_PATHDETAIL_ANY )
        {
            FirstBox_u08 = Message->pathdetail;
            LastBox_u08  = FirstBox_u08;
            if( FirstBox_u08 >= EE_RSCAN_MAXTXBUFFERS )
            {
                *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
                return( EE_RSCAN_ERROR );
            }
        }
        else
        {
            FirstBox_u08 = 0;
            LastBox_u08  = ( EE_RSCAN_MAXTXBUFFERS - 1 );
        }

        for( SendBox_u08 =  FirstBox_u08; SendBox_u08 <= LastBox_u08; SendBox_u08++ )
        {
            if( ( ( ( ee_rscan_common_p[ Unit_u08 ]->tmsts[ ( EE_RSCAN_MAXTXBUFFERS * Channel_u08 + SendBox_u08 ) / 4  ] )
                    >> ( ( ( EE_RSCAN_MAXTXBUFFERS * Channel_u08 + SendBox_u08 ) % 4 ) * 8 ) ) & EE_RSCAN_TMSTS_STSMSK ) == EE_RSCAN_TMSTS_CLEAR )    /* check pending TRQ */
            {
                if( ( ( u08 )( ee_rscan_common_p[ Unit_u08 ]->cfcc.cftml ) == SendBox_u08 )
                        && ( ee_rscan_common_p[ Unit_u08 ]->cfcc.cfe )
                        && !(( ee_rscan_common_p[ Unit_u08 ]->cfcc.cfm ) == EE_RSCAN_FIFO_MODE_RX ))
                {
                    SendBoxOccupied_u08 = EE_RSCAN_SET;
                    break;
                }
            }
            else
            {
                SendBoxOccupied_u08 = EE_RSCAN_SET;
            }

            if( SendBoxOccupied_u08 == EE_RSCAN_CLEAR )
            {

                ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1;
                /* Initiate Sending with this box and exit the box searching */
                EE_RSCAN_SetIDData( &ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].id,
                                    &ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dl0,
                                    &ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dl1,
                                    &ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dh0,
                                    &ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dh1,
                                    Message );

                ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].ptr.tid = Message->flag.ptr;
                ee_rscan_txmsg_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].ptr.dlc = Message->flag.dlc;
                ee_rscan_common_p[ Unit_u08 ]->tmsts[ SendBox_u08 ] &= ( u32 )( ~( EE_RSCAN_TMSTS_RFMSK ));
                ee_rscan_common_p[ Unit_u08 ]->tmc[SendBox_u08] |= ( u32 )( EE_RSCAN_TMC_SET_TR );
                break;
            }
            else
            {
                SendBoxOccupied_u08 = EE_RSCAN_CLEAR;   /* test next SendBox */
            }
        }

        if( SendBox_u08 >= EE_RSCAN_MAXTXBUFFERS )
        {
            *Status_u08 = EE_RSCAN_FAULT_BUSY;
            return( EE_RSCAN_ERROR );
        }
        break;
    }

    case EE_RSCAN_PATH_COMFIFO:                    /* use dedicated FIFO */
    {
        // use path any, not available for 1 COM FIFO
        SendBox_u08 = Message->pathdetail;
        if( SendBox_u08 >= EE_RSCAN_MAXCOMFIFOS )
        {
            *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
            return( EE_RSCAN_ERROR );
        }

        if( ee_rscan_common_p[ Unit_u08 ]->cfsts.cffll != 1 )   /* feed message into FIFO */
        {

            ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1;	// Select RAM Window 1

            EE_RSCAN_SetIDData( &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].id,
                                &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dl0,
                                &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dl1,
                                &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dh0,
                                &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].dh1,
                                Message );

            ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]->buf[ SendBox_u08 ].ptr.dlc = Message->flag.dlc;
            ee_rscan_common_p[ Unit_u08 ]->cfpctr.fpc = 0xFF;	// send directly

        }
        else                  /* free and enabled FIFO was not found */
        {
            *Status_u08 = EE_RSCAN_FAULT_BUSY;
            return( EE_RSCAN_ERROR );
        }
        break;
    }

    default:
        *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
        return( EE_RSCAN_ERROR );
    }

    return( EE_RSCAN_OK );
}


//=============================================================================
// FunctionName: EE_RSCAN_ReceiveMessage
// IN  :         UnitNumber, (preferred) Reception path within message
// OUT :         Success Status
//               Message with content and information
// Description : Performs Message Reception
//
//=============================================================================
bit EE_RSCAN_ReceiveMessage( u08  Unit_u08, u08  Channel_u08, u08 *Status_u08, struct ee_rscan_message *Message )
{
    u08 ReceiveBox_u08;
    u08 SearchBegin_u08;
    u08 SearchEnd_u08;
    bit temp_check;

    *Status_u08 = EE_RSCAN_FAULT_PARAMETER;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_CH_OPER ) return( EE_RSCAN_ERROR );

    *Status_u08 = EE_RSCAN_FAULT_NODATA;

    if( ( Message->path == EE_RSCAN_PATH_MSGBOX ) || ( Message->path == EE_RSCAN_PATH_ANY ) )   /* check within message boxes */
    {
        if( Message->pathdetail == EE_RSCAN_GLOBAL )    /* read first filled box */
        {
            SearchBegin_u08 = 0;
            SearchEnd_u08   = EE_RSCAN_MAXRXBUFFERS - 1;
        }
        else                             /* read dedicated box (typical polling) */
        {
            SearchBegin_u08 = Message->pathdetail;
            SearchEnd_u08   = Message->pathdetail;

            if( SearchBegin_u08 >= EE_RSCAN_MAXRXBUFFERS )
            {
                *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
                return( EE_RSCAN_ERROR );
            }
        }

        for( ReceiveBox_u08 =  SearchBegin_u08; ReceiveBox_u08 <= SearchEnd_u08; ReceiveBox_u08++ )
        {
            while( ee_rscan_common_p[ Unit_u08 ]->rmnd[ EE_RSCAN_COMRXREG( ReceiveBox_u08 ) ] >> EE_RSCAN_COMRXBIT( ReceiveBox_u08 ) & EE_RSCAN_SET )
            {
                *Status_u08 = EE_RSCAN_FAULT_NONE;
                /* Clear new data flag */
                ee_rscan_common_p[ Unit_u08 ]->rmnd[ EE_RSCAN_COMRXREG( ReceiveBox_u08 ) ] &= ( ~( 1 << EE_RSCAN_COMRXBIT( ReceiveBox_u08 ) ) );

                ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1;		// Select RAM Window 1

                temp_check = EE_RSCAN_GetMessage( &ee_rscan_rxmsg_p[ Unit_u08 ]->buf[ ReceiveBox_u08 ], Message );
                if ( temp_check == false ) *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
            }
            if( *Status_u08 != EE_RSCAN_FAULT_NODATA ) break;
        }
    }

    if( ( Message->path == EE_RSCAN_PATH_RXFIFO ) || ( ( Message->path == EE_RSCAN_PATH_ANY ) && ( *Status_u08 == EE_RSCAN_FAULT_NODATA ) ) )   /* check within RX FIFO */
    {
        if( Message->pathdetail == EE_RSCAN_GLOBAL )    /* read first filled FIFO */
        {
            SearchBegin_u08 = 0;
            SearchEnd_u08   = EE_RSCAN_MAXRXFIFOS - 1;
        }
        else                                               /* read dedicated FIFO */
        {
            SearchBegin_u08 = Message->pathdetail;
            SearchEnd_u08   = Message->pathdetail;

            if( SearchBegin_u08 >= EE_RSCAN_MAXRXFIFOS )
            {
                *Status_u08 = EE_RSCAN_FAULT_PARAMETER;
                return( EE_RSCAN_ERROR );
            }
        }

        for( ReceiveBox_u08 =  SearchBegin_u08; ReceiveBox_u08 <= SearchEnd_u08; ReceiveBox_u08++ )
        {
            if( !ee_rscan_common_p[ Unit_u08 ]->rfsts[ ReceiveBox_u08 ].rfemp )
            {
                *Status_u08 = EE_RSCAN_FAULT_NONE;

                ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1;		// Select RAM Window 1

                temp_check = EE_RSCAN_GetMessage( &ee_rscan_rxfifo_p[ Unit_u08 ]->buf[ ReceiveBox_u08 ], Message );
                if ( temp_check == false ) *Status_u08 = EE_RSCAN_FAULT_PARAMETER;

                ee_rscan_common_p[ Unit_u08 ]->rfpctr[ ReceiveBox_u08 ].fpc = EE_RSCAN_FIFO_NEXTELEMENT;

                break;
            }
        }
    }

    if( Message->path == EE_RSCAN_PATH_COMFIFO )   /* check within COMFIFO */
    {
        ReceiveBox_u08 = Message->pathdetail;
        if( !ee_rscan_common_p[ Unit_u08 ]->cfsts.cfemp )
        {
            *Status_u08 = EE_RSCAN_FAULT_NONE;

            ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 1;		// Select RAM Window 1
            temp_check = EE_RSCAN_GetMessageCF( &ee_rscan_comfifo_p[ Unit_u08 ][ Channel_u08 ]-> buf[ Message->pathdetail ], Message );
            if ( temp_check == false ) *Status_u08 = EE_RSCAN_FAULT_PARAMETER;

            ee_rscan_common_p[ Unit_u08 ]->cfpctr.fpc = EE_RSCAN_FIFO_NEXTELEMENT;
        }
    }

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_SetAFLEntry
// IN  :         UnitNumber, Channel Number, AFL Entry object
// OUT :         Success Status
// Description : Enters a new entry into the AFL
//               Limitations for routing are not verified!
//=============================================================================
bit EE_RSCAN_SetAFLEntry( u08  Unit_u08, u08  Channel_u08, u16  RuleNumber_u16, struct ee_rscan_a_afl AFLEntry )
{
    u16 AFLElement_u16 = RuleNumber_u16;
    u08 AFLChannelEntry[ EE_RSCAN_CHANNELS ];

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 >= ee_rscan_channels[ Unit_u08 ] ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.mds == EE_RSCAN_CH_OPER ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gctr.gslpr == EE_RSCAN_SET ) return( EE_RSCAN_ERROR );


    // Number of AFL_Rules
    AFLChannelEntry[ 0 ] = (u08) ee_rscan_common_p[ Unit_u08 ]->gaflcfg.rnc0;


    if( AFLElement_u16 >= EE_RSCAN_MAXAFLENTRIES ) return( EE_RSCAN_ERROR );
    if( RuleNumber_u16 >= AFLChannelEntry[ Channel_u08 ] ) return( EE_RSCAN_ERROR );
    if( AFLChannelEntry[ Channel_u08 ] > EE_RSCAN_MAXAFLCHANSET ) return( EE_RSCAN_ERROR );

    AFLElement_u16 = AFLElement_u16 % EE_RSCAN_AFLPAGESIZE;

    ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 0;

    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].id.id 		= AFLEntry.id.id;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].id.lb 		= AFLEntry.id.lb;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].id.rtr 	= AFLEntry.id.rtr;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].id.ide 	= AFLEntry.id.ide;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].mask.id 	= AFLEntry.mask.id;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].mask.rtr 	= AFLEntry.mask.rtr;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].mask.ide 	= AFLEntry.mask.ide;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.rmdp 	= AFLEntry.ptr0.rmdp;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.rmv 	= AFLEntry.ptr0.rmv;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.ptr 	= AFLEntry.ptr0.ptr;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.dlc 	= AFLEntry.ptr0.dlc;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.rxfifomask = AFLEntry.ptr0.rxfifomask;
    ee_rscan_aflpage_p[ Unit_u08 ]->af[ AFLElement_u16 ].ptr0.comfifomask = AFLEntry.ptr0.comfifomask;

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_Globale_Mode_trans
// IN  :         Unit, Mode
// OUT :         Success Status
// Description : Generell Global Mode Transistion
//=============================================================================
extern bit EE_RSCAN_Global_Mode_change( u08 Unit_u08, u08 Global_Mode_u08)
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );

    switch( Global_Mode_u08 )
    {
    case EE_RSCAN_GLOBAL_SLEEP:
        // Global: Reset -> Sleep
        if( ee_rscan_common_p[ Unit_u08 ]->gsts.mds  == EE_RSCAN_GLOBAL_RST )
        {
            ee_rscan_common_p[ Unit_u08 ]->gctr.gslpr  = 1;
            while ( ee_rscan_common_p[ Unit_u08 ]->gsts.slps != 1 );
        }
        else return EE_RSCAN_ERROR;
        break;
    case EE_RSCAN_GLOBAL_RST:
        if( ee_rscan_common_p[ Unit_u08 ]->gsts.slps  == 1 )
        {
            ee_rscan_common_p[ Unit_u08 ]->gctr.gslpr  = 0; // Global: Sleep -> Reset
            while( ee_rscan_common_p[ Unit_u08 ]->gsts.slps  != 0 );
        }
        else
        {
            ee_rscan_common_p[ Unit_u08 ]->gctr.gmdc = EE_RSCAN_GLOBAL_RST;  // Global: Operation -> Reset
            while( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_RST );
        }
        break;
    case EE_RSCAN_GLOBAL_OPER:
        if( ee_rscan_common_p[ Unit_u08 ]->gsts.slps  == 0 )
        {
            ee_rscan_common_p[ Unit_u08 ]->gctr.gmdc = EE_RSCAN_GLOBAL_OPER;
            while( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_OPER );
        }
        else return EE_RSCAN_ERROR;
        break;
    case EE_RSCAN_GLOBAL_TST:
        if( ee_rscan_common_p[ Unit_u08 ]->gsts.slps  == 0 )
        {
            ee_rscan_common_p[ Unit_u08 ]->gctr.gmdc = EE_RSCAN_GLOBAL_TST;  // Global: Operation -> Test
            while( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_TST );
        }
        else return EE_RSCAN_ERROR;
        break;
    default:
        return EE_RSCAN_ERROR;
    }

    return EE_RSCAN_OK;
}



//=============================================================================
// FunctionName: EE_RSCAN_Channel_Mode_change
// IN  :         Unit, Channel,Mode
// OUT :         Success Status
// Description : Generell Channel Mode Transistion
//=============================================================================
extern bit EE_RSCAN_Channel_Mode_change( u08 Unit_u08, u08 Channel_u08, u08 Channel_Mode_u08)
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 >= EE_RSCAN_CHANNELS ) return ( EE_RSCAN_ERROR);

    switch( Channel_Mode_u08 )
    {
    case EE_RSCAN_CH_SLEEP:
        // Channel: Reset -> Sleep
        if( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.mds  == EE_RSCAN_CH_RESET )
        {
            ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].ctr.cslpr  = 1;
            while( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.slps != 1 );
        }
        else return EE_RSCAN_ERROR;
        break;
    case EE_RSCAN_CH_RESET:
        if( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.slps  == 1 )
        {
            ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].ctr.cslpr  = 0;
            while( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.slps != 0 );
        }
        else
        {
            ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].ctr.chmdc = EE_RSCAN_CH_RESET;
            while( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.mds != EE_RSCAN_CH_RESET );
        }
        break;
    case EE_RSCAN_CH_OPER:
        if( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.slps  == 0 )
        {
            ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].ctr.chmdc = EE_RSCAN_CH_OPER;
            while( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.mds != EE_RSCAN_CH_OPER );
        }
        else return EE_RSCAN_ERROR;
        break;
    case EE_RSCAN_CH_HALT:
        if( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.slps  == 0 )
        {
            ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].ctr.chmdc = EE_RSCAN_CH_HALT;
            while( ee_rscan_common_p[ Unit_u08 ]->ch[Channel_u08].sts.mds != EE_RSCAN_CH_HALT );
        }
        else return EE_RSCAN_ERROR;
        break;
    default:
        return EE_RSCAN_ERROR;
    }

    return EE_RSCAN_OK;
}

//=============================================================================
// FunctionName: EE_RSCAN_Enable_RXFIFO
// IN  :         Unit, Channel, RX FIFO
// OUT :         Success Status
// Description : Enable RX FIFO in Operation Mode
//=============================================================================
extern bit EE_RSCAN_Enable_RXFIFO( u08 Unit_u08, u08 RXFIFO_EN_u08)
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_OPER)  return( EE_RSCAN_ERROR );

    switch( RXFIFO_EN_u08 )
    {
    case EE_RSCAN_GL_CONFIG_RXFIF0_EN0:
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ 0 ].rfe = EE_RSCAN_SET; // only in Global Operation Mode
        break;
    case EE_RSCAN_GL_CONFIG_RXFIF0_EN1:
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ 1 ].rfe = EE_RSCAN_SET; // only in Global Operation Mode
        break;
    default:
        return EE_RSCAN_ERROR;
    }

    return EE_RSCAN_OK;
}




//=============================================================================
// FunctionName: EE_RSCAN_SetGlobalConfiguration
// IN  :         UnitNumber, Global configuration set
// Description : Performs Global Configurations
//
//=============================================================================

bit EE_RSCAN_SetGlobalConfiguration( u08 Unit_u08, const struct ee_rscan_cfg_global *Config )
{
    u08 CurrentFIFO_u08;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );

    /* Must be in global reset to set the global configuration! */

    if( (ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_GLOBAL_RST) && (ee_rscan_common_p[Unit_u08]->gsts.slps == 0)  ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_RST);

    ee_rscan_common_p[ Unit_u08 ]->gcfg.tpri   = Config->gcfg.tpri;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.dce    = Config->gcfg.dce;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.dre    = Config->gcfg.dre;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.mme    = Config->gcfg.mme;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.pllbp  = Config->gcfg.pllbp;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.tsp    = Config->gcfg.tsp;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.tsss   = Config->gcfg.tsss;
    ee_rscan_common_p[ Unit_u08 ]->gcfg.itrcp  = Config->gcfg.itrcp;

    ee_rscan_common_p[ Unit_u08 ]->gctr.gmdc   = Config->gctr.gmdc;
    ee_rscan_common_p[ Unit_u08 ]->gctr.ie     = Config->gctr.ie;
    ee_rscan_common_p[ Unit_u08 ]->gctr.tsrst  = Config->gctr.tsrst;

    ee_rscan_common_p[ Unit_u08 ]->rmnb        = Config->rmnb;

    ee_rscan_common_p[ Unit_u08 ]->gaflcfg.rnc0 = ( u16 ) Config->rnc[ 0 ];

    for( CurrentFIFO_u08 = 0; CurrentFIFO_u08 < EE_RSCAN_MAXRXFIFOS; CurrentFIFO_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfie =  Config->rfcc[ CurrentFIFO_u08 ].rfie;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfdc =  Config->rfcc[ CurrentFIFO_u08 ].rfdc;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfim =  Config->rfcc[ CurrentFIFO_u08 ].rfim;
        ee_rscan_common_p[ Unit_u08 ]->rfcc[ CurrentFIFO_u08 ].rfigcv =  Config->rfcc[ CurrentFIFO_u08 ].rfigcv;
    }

    return( EE_RSCAN_OK );
}


//=============================================================================
// FunctionName: EE_RSCAN_SetChannelConfiguration
// IN  :         UnitNumber, Channel Number, Config Set
// OUT :         Success Status
// Description : Performs Channel Configurations
//
//=============================================================================

bit EE_RSCAN_SetChannelConfiguration( u08 Unit_u08, u08 Channel_u08, const struct ee_rscan_cfg_channel *Config )
{
    u32 ClockFrequency_u32;
    u16 BRPSetting_u16;
    u08 TSEG1Setting_u08;
    u08 TSEG2Setting_u08;
    u08 SJWSetting_u08;
    u08 TXCounter_u08;
    bit Status_bit;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 >= ee_rscan_channels[ Unit_u08 ] ) return( EE_RSCAN_ERROR );
    // if( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].sts.mds != EE_RSCAN_CH_RESET ) return( EE_RSCAN_ERROR );

    // Edit for CAN_CHANNEL at CH_SLEEP
    for( TXCounter_u08 = 0; TXCounter_u08 < EE_RSCAN_MAXTXBUFFERS; TXCounter_u08++ )
    {
        if( ( Config->tmiec >> TXCounter_u08 ) & 0x01 )
        {
            ee_rscan_common_p[ Unit_u08 ]->tmiec |= ( 1 << ( TXCounter_u08 ));
        }
        else
        {
            ee_rscan_common_p[ Unit_u08 ]-> tmiec &=  ~( 1 << ( TXCounter_u08 ) );
        }
    }

    // Clear any sleep mode
    EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_RESET);

    // Edit at GL_Reset
    if( ee_rscan_common_p[ Unit_u08 ]->gcfg.pllbp == EE_RSCAN_CLOCK_EXTERN_XIN )
    {
        ClockFrequency_u32 = ( u32 )( PLL_BYPASS_FREQUENCY );
        if( Config->bitrate > EE_RSCAN_MAXBAUDRATE ) return( EE_RSCAN_ERROR );
    }
    else
    {
        ClockFrequency_u32 = ( u32 )( CLEAN_FREQUENCY);
        if( Config->bitrate > EE_RSCAN_MAXBAUDRATEPLLBP ) return( EE_RSCAN_ERROR );
    }

    Status_bit = EE_RSCAN_SetBittiming( ClockFrequency_u32, Config->bitrate, Config->tq_perbit, Config->syncjumpwidth, &BRPSetting_u16, &TSEG1Setting_u08, &TSEG2Setting_u08, &SJWSetting_u08, Config->samplingpointpos );
    if( Status_bit != EE_RSCAN_OK ) return( EE_RSCAN_ERROR );

    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].cfg.brp = ( u32 )BRPSetting_u16;
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].cfg.tseg1 = ( u32 )TSEG1Setting_u08;
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].cfg.tseg2 = ( u32 )TSEG2Setting_u08;
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].cfg.sjw = ( u32 )SJWSetting_u08;


    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.chmdc = Config->ctr.chmdc;
    //ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.rtbo = Config->ctr.rtbo;	// only in CH_OPER
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.ie = Config->ctr.ie;
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.bom = Config->ctr.bom;
    ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.errd = Config->ctr.errd;
    // ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.ctme = Config->ctr.ctme; 	// only in CH_HALT
    // ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.ctms = Config->ctr.ctms;  // only in CH_HALT
    // ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.trwe = Config->ctr.trwe;	// only in CH_HALT
    // ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.trh = Config->ctr.trh;	// only in CH_HALT
    // ee_rscan_common_p[ Unit_u08 ]->ch[ Channel_u08 ].ctr.trr = Config->ctr.trr;	// only in CH_HALT

    ee_rscan_common_p[ Unit_u08 ]->thlcc[ Channel_u08 ].thle = Config->thlcc.thle;
    ee_rscan_common_p[ Unit_u08 ]->thlcc[ Channel_u08 ].im   = Config->thlcc.im;
    ee_rscan_common_p[ Unit_u08 ]->thlcc[ Channel_u08 ].dte  = Config->thlcc.dte;

    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cftml = Config->cfcc.cftml;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfitr = Config->cfcc.cfitr;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfitss = Config->cfcc.cfitss;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfm = Config->cfcc.cfm;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfigcv = Config->cfcc.cfigcv;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfim = Config->cfcc.cfim;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfdc = Config->cfcc.cfdc;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cftxie = Config->cfcc.cftxie;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfrxie = Config->cfcc.cfrxie;
    ee_rscan_common_p[ Unit_u08 ]-> cfcc.cfitt = Config->cfcc.cfitt;

    /* Set Channel to HALT mode, in order to configure queues and lists */

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_Channel_Buffer_enable
// IN  :         UnitNumber, Channel Number, Ch_Config
// OUT :         Success Status
// Description : Enable Channel ( History Buffer, Com Fifo)
//
//=============================================================================

bit EE_RSCAN_Channel_Buffer_enable( u08 Unit_u08, u08 Channel_u08, const struct ee_rscan_cfg_channel *Config )
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return EE_RSCAN_ERROR;
    if( Channel_u08 > EE_RSCAN_CHANNELS ) return EE_RSCAN_ERROR;

    EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_HALT );

    ee_rscan_common_p[ Unit_u08 ]->thlcc[ Channel_u08 ].ie   = Config->thlcc.ie;
    ee_rscan_common_p[ Unit_u08 ]->cfcc.cfe = Config->cfcc.cfe;

    return( EE_RSCAN_OK );
}

//=============================================================================
// FunctionName: EE_RSCAN_Port_Enable
// IN  :         Unit, Channel
// OUT :         Success Status
// Description : Channel depended Port Configuration
//=============================================================================
extern bit EE_RSCAN_Port_Enable( u08 Unit_u08, u08 Channel_u08 )
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel_u08 > EE_RSCAN_CHANNELS ) return EE_RSCAN_ERROR;

    if ( Channel_u08 == 0 )
    {

        /* CH0_RX: P1_1 */
        PIOR1_bit.no1 = 0;
        PM1_bit.no1 = 1;               /* use port P1.1 as input              */

        /* CH0_TX: P1_0 */
        PIOR1_bit.no0 = 0;
        POM1_bit.no0  = 0;
        P1_bit.no0    = 1;
        PM1_bit.no0   = 0;               /* use port P1.0 as output              */

        return (EE_RSCAN_OK);
    }
    return (EE_RSCAN_ERROR);
}

bit EE_RSCAN_GetAFL_Nmb( u08 Unit_u08, u08 Channel_u08, u08 *AFL_count_ch_u08)
{
    switch( Channel_u08 )
    {
    case EE_RSCAN_CHANNEL0:
        *AFL_count_ch_u08 = (u08) ee_rscan_common_p[ Unit_u08 ]->gaflcfg.rnc0;
        break;
    default:
        return( EE_RSCAN_ERROR );
    }
    return EE_RSCAN_OK;
}

//========================================================================================
// Function Name: EE_RSCAN_Config
// IN  :         Unit Numbers, Channel Numbers, Global CFG, Channel CFG, AFL_Rules[]
// OUT :         Success Status
// Description : Performs GLOBAL, CHANNEL and AFL-Rules CONFIGURTAION for one Channel
//
//========================================================================================

extern bit EE_RSCAN_Config( u08 Unit_u08, u08 Channel_u08, const struct ee_rscan_cfg_global* Global_CFG, const struct ee_rscan_cfg_channel* Ch_CFG, struct ee_rscan_a_afl FilterEntry[] )
{
    bit EE_RSCAN_Status_bit   = EE_RSCAN_OK;
    u08 AFL_count_ch_u08 = 0;
    u08 i_u08 = 0;

    // Port activation
    EE_RSCAN_Status_bit &= EE_RSCAN_Port_Enable( Unit_u08, Channel_u08 );

    // Global: Sleep -> Reset
    EE_RSCAN_Status_bit &= EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_RST );

    // Channel: Sleep -> Reset
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_RESET);


    // GLOBAL CONFIGURATION
    // Global Mode: Reset Mode;
    // Channel Mode: Reset Mode
    // Config: global Config ( PLL, rnc ... )
    EE_RSCAN_Status_bit &= EE_RSCAN_SetGlobalConfiguration( Unit_u08, Global_CFG );
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );


    EE_RSCAN_Status_bit &= EE_RSCAN_GetAFL_Nmb ( Unit_u08, Channel_u08, &AFL_count_ch_u08);

    // SET AFL ENTRY
    for ( i_u08 = 0; i_u08 < AFL_count_ch_u08; i_u08++)
    {
        EE_RSCAN_Status_bit &= EE_RSCAN_SetAFLEntry( Unit_u08, Channel_u08 , i_u08, FilterEntry[i_u08] ); // Unit, Channel, Ruler Number, Filter Entry
        if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
    }

    // CHANNEL CONFIGURATION
    // Global Mode: Reset -> HALT
    // Channel Mode: Sleep bit + Reset -> Reset -> HALT
    EE_RSCAN_Status_bit &= EE_RSCAN_SetChannelConfiguration( Unit_u08, Channel_u08, Ch_CFG );
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_OPER );
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Buffer_enable( Unit_u08, Channel_u08, Ch_CFG );

    return( EE_RSCAN_Status_bit );
}

//=============================================================================
// FunctionName: EE_RSCAN_RAMTest
// IN  :         Unit Number
// OUT :         Success Status
// Description : Executes RAM Test
// + 256 bytes per Page
//=============================================================================
bit EE_RSCAN_Channel_Config_Self_Test( u08 Unit_u08, u08 Channel1_u08, u08 Self_test_u08 )
{
    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( Channel1_u08 > EE_RSCAN_CHANNELS ) return EE_RSCAN_ERROR;

    if( ee_rscan_common_p[ Unit_u08 ]->ch[ Channel1_u08 ].sts.mds != EE_RSCAN_CH_HLT ) return( EE_RSCAN_ERROR );

    if ( (Self_test_u08 == EE_RSCAN_TEST_INTLOOP) | (Self_test_u08 == EE_RSCAN_TEST_EXTLOOP))
    {
        ee_rscan_common_p[ Unit_u08 ]->ch[ Channel1_u08 ].ctr.ctme = 1;
        ee_rscan_common_p[ Unit_u08 ]->ch[ Channel1_u08 ].ctr.ctms = Self_test_u08;
    }
    else
    {
        return ( EE_RSCAN_ERROR );
    }

    return ( EE_RSCAN_OK );
}


//========================================================================================
// FunctionName: EE_RSCAN_Config_Self_Test
// IN  :         Unit Numbers, Channel Numbers, Global CFG, Channel CFG, AFL_Rules[]
// OUT :         Success Status
// Description : Performs GLOBAL, CHANNEL and AFL-Rules CONFIGURTAION for one Channel
//
//========================================================================================
extern bit EE_RSCAN_Config_Self_Test( u08 Unit_u08, u08 Channel1_u08, const struct ee_rscan_cfg_global* Global_CFG, const struct ee_rscan_cfg_channel* Ch_CFG, struct ee_rscan_a_afl FilterEntry[] )
{
    bit EE_RSCAN_Status_bit   = EE_RSCAN_OK;
    u08 AFL_count_ch_u08 = 0;
    u08 i_u08 = 0;

    // Global: Sleep -> Reset
    EE_RSCAN_Status_bit &= EE_RSCAN_Global_Mode_change( 0, EE_RSCAN_GLOBAL_RST );

    // Channel1: Sleep -> Reset
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel1_u08, EE_RSCAN_CH_RESET);

    // GLOBAL CONFIGURATION
    // Global Mode: Reset Mode;
    // Channel Mode: Reset Mode
    // Config: global Config ( PLL, rnc ... )
    EE_RSCAN_Status_bit &= EE_RSCAN_SetGlobalConfiguration( Unit_u08, Global_CFG );
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    AFL_count_ch_u08 = (u08) ee_rscan_common_p[ Unit_u08 ]->gaflcfg.rnc0;


    // SET AFL ENTRY
    for ( i_u08 = 0; i_u08 < AFL_count_ch_u08; i_u08++)
    {
        EE_RSCAN_Status_bit &= EE_RSCAN_SetAFLEntry( Unit_u08, Channel1_u08 , i_u08, FilterEntry[i_u08] ); // Unit, Channel, Ruler Number, Filter Entry
        if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
    }

    // CHANNEL CONFIGURATION
    // Global Mode: Reset -> HALT
    // Channel Mode: Sleep bit + Reset -> Reset -> HALT
    EE_RSCAN_Status_bit &= EE_RSCAN_SetChannelConfiguration( Unit_u08, Channel1_u08, Ch_CFG );
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Status_bit &= EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_TST );

    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Buffer_enable( Unit_u08, Channel1_u08, Ch_CFG );

    // Global: Reset -> Halt
    EE_RSCAN_Status_bit &= EE_RSCAN_Global_Mode_change( 0, EE_RSCAN_GLOBAL_TST );

    // Channel: Reset -> Halt
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel1_u08, EE_RSCAN_CH_HALT);


    //---------------------  Self Test Communication ( 1 Channel ) Test ------------------------//
    // Inter Loop Back

    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Config_Self_Test( Unit_u08, Channel1_u08, EE_RSCAN_TEST_INTLOOP);

    return( EE_RSCAN_Status_bit );
}


//=============================================================================
// FunctionName: EE_RSCAN_RAMTest
// IN  :         Unit Number
// OUT :         Success Status
// Description : Executes RAM Test
// + 256 bytes per Page
//=============================================================================
bit EE_RSCAN_RAMTest( u08 Unit_u08 )
{
    u08 RAMPage_u08;
    u16 RAMElement_u16;
    u32 RAMData_u32 = 0x00000001;

    if( Unit_u08 >= EE_RSCAN_MACROS ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gsts.mds != EE_RSCAN_CH_HALT ) return( EE_RSCAN_ERROR );
    if( ee_rscan_common_p[ Unit_u08 ]->gctr.gslpr == EE_RSCAN_SET ) return( EE_RSCAN_ERROR );

    // Unlock the test mode
    ee_rscan_common_p[ Unit_u08 ]->glockk     = EE_RSCAN_RAMTEST_KEY1;
    ee_rscan_common_p[ Unit_u08 ]->glockk     = EE_RSCAN_RAMTEST_KEY2;
    ee_rscan_common_p[ Unit_u08 ]->gtstctr.rtme = 1;


    // Reading zero value in intialized RAM
    for( RAMPage_u08 = 0; RAMPage_u08 < EE_RSCAN_RAMTEST_PAGES; RAMPage_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->gtstcfg.rtmps = RAMPage_u08;

        ee_rscan_common_p[ Unit_u08 ]->grwcr.rpage = 0; // Select RAM Window 0

        for( RAMElement_u16 = 0; RAMElement_u16 < EE_RSCAN_RAMTEST_ENTRIES; RAMElement_u16++ )
        {
            if( ee_rscan_ram_p[ Unit_u08 ]->cell[ RAMElement_u16 ] != 0x00000000 )
            {
                return( EE_RSCAN_ERROR );
            }
        }
    }

    // Write walking-1 to all RAM pages
    for( RAMPage_u08 = 0; RAMPage_u08 < EE_RSCAN_RAMTEST_PAGES; RAMPage_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->gtstcfg.rtmps = RAMPage_u08;

        for( RAMElement_u16 = 0; RAMElement_u16 < EE_RSCAN_RAMTEST_ENTRIES; RAMElement_u16++ )
        {
            ee_rscan_ram_p[ Unit_u08 ]->cell[ RAMElement_u16 ] = RAMData_u32;
            if( RAMData_u32 >= 0x80000000)
            {
                RAMData_u32 = 0x00000001;
            }
            else
            {
                RAMData_u32 = RAMData_u32 * 2;
            }
        }
    }

    // Checking Walking-1 through all pages
    for( RAMPage_u08 = 0; RAMPage_u08 < EE_RSCAN_RAMTEST_PAGES; RAMPage_u08++ )
    {
        ee_rscan_common_p[ Unit_u08 ]->gtstcfg.rtmps = RAMPage_u08;

        for( RAMElement_u16 = 0; RAMElement_u16 < EE_RSCAN_RAMTEST_ENTRIES; RAMElement_u16++ )
        {
            if ( RAMPage_u08 == 0 && RAMElement_u16 == 0 )
            {
                RAMData_u32 = 0x00000001;
            }
            else if( RAMData_u32 >= 0x80000000)
            {
                RAMData_u32 = 0x00000001;
            }
            else
            {
                RAMData_u32 = RAMData_u32 * 2;
            }

            if( ee_rscan_ram_p[ Unit_u08 ]->cell[ RAMElement_u16 ] != RAMData_u32 )
            {
                return( EE_RSCAN_ERROR );
            }
        }
    }
    return( EE_RSCAN_OK );
}


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
/*                                                                           */
/*===========================================================================*/
#include <stddef.h>
#include <stdio.h>

#include "ree_types.h"
#include "map_rscan.h"
#include "rscan_p.h"
#include "rscan_s.h"


/*===========================================================================*/
/*	----------------------------------
    Generell Configuration Procedure
    ----------------------------------
    1. Exit Global Sleep Mode
    2. Exit Channel Sleep Mode
    3. Global Configuration Settings
    4. Rule Table Config ( AFL )
    5. Message Buffer Config ( FIFO, RX MB, TX Queue)
    6. all active Channel Config ( baudrate, bit timing, bus-off, error )
    7. Interrupt Configure
    8. Global Mode -> Operation Mode
    9. Channel Mode -> Operation Mode
*/
/*===========================================================================*/

/* GLOBAL VARIABLE */

bit EE_RSCAN_TRANSMIT_STATUS_TEST = false;


void RSCAN_A_UnitChannel_TX_RX_MB_Test_IRQ( void )
{
    EE_RSCAN_TRANSMIT_STATUS_TEST = true;
    return;
}

void RSCAN_A_UnitChannel_RX_FIFO_Test_IRQ( void )
{
    return;
}

void RSCAN_A_UnitChannel_COM_FIFO_TX_Test_IRQ( void )
{
    return;
}

void RSCAN_A_UnitChannel2TransmitIRQ( void )
{
    return;
}

/*===========================================================================*/
/*	 Test: 	COM FIFO in TX Mode and RX FIFO as Reception Test:
    Description:	- Test the COM FIFO for one Channel
                    - Transmit Test Messages with COM FIFO
                        -> Put Messages into FIFO
                        -> Check COM FIFO Full functionality
                    - Receipt Messages with RX FIFO
                        -> 64 AFL Rules Test
*/
/*===========================================================================*/
/* Test Configuration:
    Send_Count 	= Number to Send Messages
    Receipt_Count 	= Number of Messages to be send
    AFL_Count	= Number of AFL Rules
*/
/*===========================================================================*/
#define Send_Count_COM_FIFO     6
#define Receipt_Count_RX_FIFO   16
#define AFL_Count_RX_FIFO	    16

extern bit EE_RSCAN_COM_FIFO_TX_and_RX_FIFO_Test( u08 Unit_u08, u08 Channel_u08)
{
    bit EE_RSCAN_Status_bit   = EE_RSCAN_OK;
    u08 SendStatus_u08 = EE_RSCAN_FAULT_PARAMETER;
    u08 ReceiveStatus_u08;
    u08 DataCounter_u08 = 0;
    u08 i_u08 = 0;

    struct ee_rscan_message SendMessage_n[Send_Count_COM_FIFO];
    struct ee_rscan_message ReceiveMessage_n[Receipt_Count_RX_FIFO];

    struct ee_rscan_a_afl FilterEntry[AFL_Count_RX_FIFO];
    struct ee_rscan_cfg_global Global_Config = EE_RSCAN_GCFG_CF_EXTERN;

    /* TEMPORARY TEST VARIABLE: AFL Rules, Global Config, Variable for Transmission and Reception Messages */
    /* TEST SPECIFIC RECONFIGURATION of the Pre-Configuration Settings:                                    */

    for( i_u08 = 0; i_u08 < AFL_Count_RX_FIFO; i_u08++)
    {
        FilterEntry[i_u08] = EE_RSCAN_AFL_any_MSG_CF_EXTERN;
        FilterEntry[i_u08].ptr0.rxfifomask = EE_RSCAN_AFL_RXFIF0_EN0;
        FilterEntry[i_u08].id.id =  (u32) ( 20 + i_u08);
    }


    Global_Config.rnc[Channel_u08] = AFL_Count_RX_FIFO;


    for (i_u08 = 0; i_u08 < Send_Count_COM_FIFO; i_u08++)
    {
        SendMessage_n[i_u08].hdr.id     = 15;			   /* ID              */
        SendMessage_n[i_u08].hdr.thlen  = 0;        		   /* No entry in THL */
        SendMessage_n[i_u08].hdr.rtr    = 0;          	   /* Data Frame      */
        SendMessage_n[i_u08].hdr.ide    = 0;          	   /* Standard Frame  */
        SendMessage_n[i_u08].flag.ts    = 0;          	   /* timestamp       */
        SendMessage_n[i_u08].flag.ptr   = 0;            	   /* HTH value       */
        SendMessage_n[i_u08].flag.dlc   = 8;			   /* Data Length     */
        SendMessage_n[i_u08].path       = EE_RSCAN_PATH_COMFIFO; /* Search at COM FIFO */
        SendMessage_n[i_u08].pathdetail = 0;  		   /* use COM FIFO 0  */

        for( DataCounter_u08 = 0; DataCounter_u08 < SendMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            SendMessage_n[i_u08].data[ DataCounter_u08 ] = ( i_u08 * 8 ) + DataCounter_u08 + 1;
        }
    }

    for (i_u08 = 0; i_u08 < Receipt_Count_RX_FIFO; i_u08++)
    {
        ReceiveMessage_n[i_u08].hdr.id     = 0;		   /* ID              */
        ReceiveMessage_n[i_u08].hdr.thlen  = 0;        	   /* No entry in THL */
        ReceiveMessage_n[i_u08].hdr.rtr    = 0;          	   /* Data Frame      */
        ReceiveMessage_n[i_u08].hdr.ide    = 0;          	   /* Standard Frame  */
        ReceiveMessage_n[i_u08].flag.ts    = 0;          	   /* timestamp       */
        ReceiveMessage_n[i_u08].flag.ptr   = 0;            	   /* HTH value       */
        ReceiveMessage_n[i_u08].flag.dlc   = 8;		   /* Data Length     */
        ReceiveMessage_n[i_u08].path    = EE_RSCAN_PATH_RXFIFO;  /* Search at COM FIFO */
        ReceiveMessage_n[i_u08].pathdetail = 0;  		   /* use RX FIFO 0/1 */

        for( DataCounter_u08 = 0; DataCounter_u08 < ReceiveMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            ReceiveMessage_n[i_u08].data[ DataCounter_u08 ] = 0;
        }
    }

    EE_RSCAN_Status_bit &= EE_RSCAN_Config( Unit_u08, Channel_u08, &Global_Config, &EE_RSCAN_CHCFG_CF_EXTERN, FilterEntry);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Status_bit &= EE_RSCAN_CreateInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TXCF, 1, RSCAN_A_UnitChannel_COM_FIFO_TX_Test_IRQ );
    EE_RSCAN_Status_bit &= EE_RSCAN_SetInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TXCF, EE_RSCAN_INT_ENABLE);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Status_bit &= EE_RSCAN_CreateInterrupt( Unit_u08, EE_RSCAN_GLOBAL, EE_RSCAN_INT_RXF0, 2, RSCAN_A_UnitChannel_RX_FIFO_Test_IRQ );
    EE_RSCAN_Status_bit &= EE_RSCAN_SetInterrupt( Unit_u08, EE_RSCAN_GLOBAL, EE_RSCAN_INT_RXF0, EE_RSCAN_INT_ENABLE);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    __enable_interrupt();

    /* Global: Reset -> Operation; */
    EE_RSCAN_Status_bit &= EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_OPER );

    /* Channel: Reset -> Operation */
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_OPER);

    for (i_u08 = 0; i_u08 < Send_Count_COM_FIFO; i_u08++)
    {
        EE_RSCAN_Status_bit &= EE_RSCAN_SendMessage( Unit_u08, Channel_u08, &SendStatus_u08, &SendMessage_n[i_u08] );
        if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
    }

    EE_RSCAN_Status_bit &= EE_RSCAN_Enable_RXFIFO( Unit_u08, EE_RSCAN_GL_CONFIG_RXFIF0_EN0);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );


    for (i_u08 = 0; i_u08 < Receipt_Count_RX_FIFO; i_u08++)
    {
        __no_operation();
        do
        {
            EE_RSCAN_Status_bit &= EE_RSCAN_ReceiveMessage( Unit_u08, Channel_u08, &ReceiveStatus_u08, &ReceiveMessage_n[i_u08] );
            if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
        }
        while( ReceiveStatus_u08 == EE_RSCAN_FAULT_NODATA );
    }

    /* Now the Responder should send Messages Back */
    /* This Check Function only success, if Messages with ID = 20++ are send */
    for (i_u08 = 0; i_u08 < Receipt_Count_RX_FIFO; i_u08++)
    {
        if( !(ReceiveMessage_n[ i_u08 ].hdr.id >= 20) ) return false;
    }


    /* here break point */
    return EE_RSCAN_Status_bit;
}


/*===========================================================================*/
/*
    Test: 	Com TX RX MB Test:
    Description:	- Test the TX MB for one Channel
                    - Transmit Test Messages with TX MB
                    - Receipt Messages with RX MB
                        -> x AFL Rules Test
*/
/*===========================================================================*/
/*
    Test Configuration:
    Send_Count 	    = Number to Send Messages
    Receipt_Count 	= Number of Messages to be send
    AFL_Count	    = Number of AFL Rules
*/
/*===========================================================================*/

#define Send_Count	    1
#define Receipt_Count 	1
#define AFL_Count	    1

extern bit EE_RSCAN_TX_RX_MB_Test( u08 Unit_u08, u08 Channel_u08)
{
    bit EE_RSCAN_Status_bit   = EE_RSCAN_OK;
    u08 SendStatus_u08 = EE_RSCAN_FAULT_PARAMETER;
    u08 ReceiveStatus_u08 = 0;
    u08 DataCounter_u08 = 0;
    u08 i_u08 = 0;

    struct ee_rscan_message SendMessage_n[Send_Count];
    struct ee_rscan_message ReceiveMessage_n[Receipt_Count];

    struct ee_rscan_a_afl FilterEntry[AFL_Count];
    struct ee_rscan_cfg_global Global_Config = EE_RSCAN_GCFG_TX_MB_EXTERN;


    for( i_u08 = 0; i_u08 < AFL_Count; i_u08++)
    {
        FilterEntry[i_u08] = EE_RSCAN_AFL_RX_MSG_ID20;
        FilterEntry[i_u08].id.id =  (u32) ( 20 + i_u08);
    }


    Global_Config.rnc[Channel_u08] = AFL_Count;
    Global_Config.rmnb = Receipt_Count;


    for (i_u08 = 0; i_u08 < Send_Count; i_u08++)
    {
        SendMessage_n[i_u08].hdr.id     = 15;			            /* ID               */
        SendMessage_n[i_u08].hdr.thlen  = 0;                        /* No entry in THL  */
        SendMessage_n[i_u08].hdr.rtr    = 0;                 	    /* Data Frame       */
        SendMessage_n[i_u08].hdr.ide    = 0;                 	    /* Standard Frame   */
        SendMessage_n[i_u08].flag.ts    = 0;                 	    /* timestamp        */
        SendMessage_n[i_u08].flag.ptr   = 0;                 	    /* HTH value        */
        SendMessage_n[i_u08].flag.dlc   = 8;			            /* Data Length      */
        SendMessage_n[i_u08].path       = EE_RSCAN_PATH_MSGBOX;     /* Send at Message Box ( TX MB ) */
        SendMessage_n[i_u08].pathdetail = 0;  		                /* use TX MB 0      */

        for( DataCounter_u08 = 0; DataCounter_u08 < SendMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            SendMessage_n[i_u08].data[ DataCounter_u08 ] = ( i_u08 * 8 ) + DataCounter_u08 + 1;
        }
    }


    for (i_u08 = 0; i_u08 < Receipt_Count; i_u08++)
    {
        ReceiveMessage_n[i_u08].hdr.id     = 0;		                /* ID            */
        ReceiveMessage_n[i_u08].hdr.thlen  = 0;        	            /* No entry in THL */
        ReceiveMessage_n[i_u08].hdr.rtr    = 0;                     /* Data Frame    */
        ReceiveMessage_n[i_u08].hdr.ide    = 0;                     /* Standard Frame */
        ReceiveMessage_n[i_u08].flag.ts    = 0;                     /* timestamp     */
        ReceiveMessage_n[i_u08].flag.ptr   = 0;                     /* HTH value     */
        ReceiveMessage_n[i_u08].flag.dlc   = 8;		                /* Data Length   */
        ReceiveMessage_n[i_u08].path       = EE_RSCAN_PATH_MSGBOX;  /* Search at RX MB */
        ReceiveMessage_n[i_u08].pathdetail = 0;  		            /* use COM FIFO 0/1/2 */

        for( DataCounter_u08 = 0; DataCounter_u08 < ReceiveMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            ReceiveMessage_n[i_u08].data[ DataCounter_u08 ] = 0;
        }
    }

    EE_RSCAN_Status_bit &= EE_RSCAN_Config( Unit_u08, Channel_u08, &Global_Config, &EE_RSCAN_CHCFG_TX_MB_EXTERN, FilterEntry);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    EE_RSCAN_Status_bit &= EE_RSCAN_CreateInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TX, 1, RSCAN_A_UnitChannel_TX_RX_MB_Test_IRQ );
    EE_RSCAN_Status_bit &= EE_RSCAN_SetInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TX,  (u16)(SendMessage_n[0].pathdetail));
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    __enable_interrupt();

    /* Global: Reset -> Operation;*/
    EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_OPER );

    /* Channel: Reset -> Operation*/
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_OPER);


    for (i_u08 = 0; i_u08 < Send_Count; i_u08++)
    {
        EE_RSCAN_TRANSMIT_STATUS_TEST = false;
        EE_RSCAN_Status_bit = EE_RSCAN_SendMessage( Unit_u08, Channel_u08, &SendStatus_u08, &SendMessage_n[i_u08] );
        while ( EE_RSCAN_TRANSMIT_STATUS_TEST == false );
    }


    for (i_u08 = 0; i_u08 < Receipt_Count; i_u08++)
    {
        do
        {
            EE_RSCAN_Status_bit &= EE_RSCAN_ReceiveMessage( Unit_u08, Channel_u08, &ReceiveStatus_u08, &ReceiveMessage_n[i_u08] );
            if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
        }
        while( ReceiveStatus_u08 == EE_RSCAN_FAULT_NODATA );
    }

    /* Now the Responder should send Messages Back                            */
    /* This Check Function only success, if Messages with ID = 20++ are send  */
    for (i_u08 = 0; i_u08 < Receipt_Count; i_u08++)
    {
        if(  ReceiveMessage_n[ i_u08 ].hdr.id != 20 + i_u08 ) return false;
    }

    /* here break point */
    return EE_RSCAN_Status_bit;
}


/*===========================================================================*/
/*
        Test:	Com TX RX MB Loop Test:
        Description:	- Test the TX MB for one Channel
                        - Transmit Test Messages with TX MB
                        - Receipt Messages with RX MB
                            -> x AFL Rules Test
                        - Loop Back Test
*/
/*===========================================================================*/
/*
Test Configuration:
    Send_Count	= Number to Send Messages
    Receipt_Count 	= Number of Messages to be send
    AFL_Count       = Number of AFL Rules
*/
/*===========================================================================*/

#define Send_Count_Loop_MB             1
#define Receipt_Count_Loop_MB          Send_Count_Loop_MB
#define AFL_Count_Loop_MB              Send_Count_Loop_MB

extern bit EE_RSCAN_TX_RX_MB_Loop_Test( u08 Unit_u08, u08 Channel_u08)
{
    bit EE_RSCAN_Status_bit   = EE_RSCAN_OK;
    u08 SendStatus_u08 = EE_RSCAN_FAULT_PARAMETER;
    u08 ReceiveStatus_u08 = 0;
    u08 DataCounter_u08 = 0;
    u08 i_u08 = 0;
    u08  SetIntLevel_u08 = 0;

    struct ee_rscan_message SendMessage_n[Send_Count_Loop_MB];
    struct ee_rscan_message ReceiveMessage_n[Receipt_Count_Loop_MB];

    struct ee_rscan_a_afl FilterEntry[AFL_Count_Loop_MB];
    struct ee_rscan_cfg_global Global_Config = EE_RSCAN_GCFG_TX_MB_LOOP;


    for( i_u08 = 0; i_u08 < AFL_Count_Loop_MB; i_u08++)
    {
        FilterEntry[i_u08] = EE_RSCAN_AFL_RX_MSG_LOOP_ID20;
        FilterEntry[i_u08].id.id =  (u32) ( 20 + i_u08);
    }


    Global_Config.rnc[Channel_u08] = AFL_Count_Loop_MB;
    Global_Config.rmnb = Receipt_Count_Loop_MB;


    for (i_u08 = 0; i_u08 < Send_Count_Loop_MB; i_u08++)
    {
        SendMessage_n[i_u08].hdr.id     = 20;                   /* ID               */
        SendMessage_n[i_u08].hdr.thlen  = 0;                    /* No entry in THL  */
        SendMessage_n[i_u08].hdr.rtr    = 0;                 	  /* Data Frame       */
        SendMessage_n[i_u08].hdr.ide    = 0;                 	  /* Standard Frame   */
        SendMessage_n[i_u08].flag.ts    = 0;                 	  /* timestamp        */
        SendMessage_n[i_u08].flag.ptr   = 0;                 	  /* HTH value        */
        SendMessage_n[i_u08].flag.dlc   = 8;                    /* Data Length      */
        SendMessage_n[i_u08].path       = EE_RSCAN_PATH_MSGBOX; /* Send at Message Box ( TX MB ) */
        SendMessage_n[i_u08].pathdetail = 0;                    /* use TX MB 0      */

        for( DataCounter_u08 = 0; DataCounter_u08 < SendMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            SendMessage_n[i_u08].data[ DataCounter_u08 ] = ( i_u08 * 8 ) + DataCounter_u08 + 1;
        }
    }


    for (i_u08 = 0; i_u08 < Receipt_Count_Loop_MB; i_u08++)
    {
        ReceiveMessage_n[i_u08].hdr.id     = 0;                    /* ID            */
        ReceiveMessage_n[i_u08].hdr.thlen  = 0;                    /* No entry in THL */
        ReceiveMessage_n[i_u08].hdr.rtr    = 0;                    /* Data Frame    */
        ReceiveMessage_n[i_u08].hdr.ide    = 0;                    /* Standard Frame*/
        ReceiveMessage_n[i_u08].flag.ts    = 0;                    /* timestamp     */
        ReceiveMessage_n[i_u08].flag.ptr   = 0;                    /* HTH value     */
        ReceiveMessage_n[i_u08].flag.dlc   = 8;                    /* Data Length   */
        ReceiveMessage_n[i_u08].path       = EE_RSCAN_PATH_MSGBOX; /* Search at RX MB */
        ReceiveMessage_n[i_u08].pathdetail = EE_RSCAN_PATH_ANY;    /* use COM FIFO 0/1/2 */

        for( DataCounter_u08 = 0; DataCounter_u08 < ReceiveMessage_n[i_u08].flag.dlc; DataCounter_u08++ )
        {
            ReceiveMessage_n[i_u08].data[ DataCounter_u08 ] = 0;
        }
    }

    EE_RSCAN_Status_bit &= EE_RSCAN_Config_Self_Test( Unit_u08, Channel_u08, &Global_Config, &EE_RSCAN_CHCFG_TX_MB_LOOP, FilterEntry);
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    SetIntLevel_u08 = 1;
    EE_RSCAN_Status_bit &= EE_RSCAN_CreateInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TX, SetIntLevel_u08, RSCAN_A_UnitChannel_TX_RX_MB_Test_IRQ );
    EE_RSCAN_Status_bit &= EE_RSCAN_SetInterrupt( Unit_u08, Channel_u08, EE_RSCAN_INT_TX, (u16)(SendMessage_n[0].pathdetail));
    if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );

    __enable_interrupt();

    /* Global: Reset -> Operation; */
    EE_RSCAN_Global_Mode_change( Unit_u08, EE_RSCAN_GLOBAL_OPER );

    /* Channel: Reset -> Operation */
    EE_RSCAN_Status_bit &= EE_RSCAN_Channel_Mode_change( Unit_u08, Channel_u08, EE_RSCAN_CH_OPER);


    for (i_u08 = 0; i_u08 < Send_Count_Loop_MB; i_u08++)
    {
        EE_RSCAN_TRANSMIT_STATUS_TEST = false;
        EE_RSCAN_Status_bit = EE_RSCAN_SendMessage( Unit_u08, Channel_u08, &SendStatus_u08, &SendMessage_n[i_u08] );
        while ( EE_RSCAN_TRANSMIT_STATUS_TEST == false );

        do
        {
            EE_RSCAN_Status_bit &= EE_RSCAN_ReceiveMessage( Unit_u08, Channel_u08, &ReceiveStatus_u08, &ReceiveMessage_n[i_u08] );
            if( EE_RSCAN_Status_bit == EE_RSCAN_ERROR ) return( EE_RSCAN_ERROR );
        }
        while( ReceiveStatus_u08 == EE_RSCAN_FAULT_NODATA );
    }

    /* Now the Responder should send Messages Back                           */
    /* This Check Function only success, if Messages with ID = 20++ are send */
    for (i_u08 = 0; i_u08 < Receipt_Count_Loop_MB; i_u08++)
    {
        if(  ReceiveMessage_n[ i_u08 ].hdr.id != 20 + i_u08 ) return false;
    }

    /* here break point */
    return EE_RSCAN_Status_bit;
}

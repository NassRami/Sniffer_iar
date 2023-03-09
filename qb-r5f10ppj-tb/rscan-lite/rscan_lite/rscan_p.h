/*===========================================================================*/
/* Module : rscan_p.h                                                        */
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
/*                                                                           */
/*===========================================================================*/
#ifndef _RSCAN_P_H
#define _RSCAN_P_H

#include <ree_types.h>

extern bit EE_RSCAN_Port_Enable( u08 Unit_u08, u08 Channel_u08 );

extern bit EE_RSCAN_Global_Mode_change( u08 Unit_u08, u08 Global_Mode_u08);

extern bit EE_RSCAN_Channel_Mode_change( u08 Unit_u08,
        u08 Channel_u08,
        u08 Channel_Mode_u08);

extern bit EE_RSCAN_Config( u08 Unit_u08,
                            u08 Channel_u08,
                            const struct ee_rscan_cfg_global* Global_CFG,
                            const struct ee_rscan_cfg_channel* Ch_CFG,
                            struct ee_rscan_a_afl FilterEntry[] );

extern bit EE_RSCAN_Config_Self_Test( 	u08 Unit_u08,
                                        u08 Channel1_u08,
                                        const struct ee_rscan_cfg_global* Global_CFG,
                                        const struct ee_rscan_cfg_channel* Ch_CFG,
                                        struct ee_rscan_a_afl FilterEntry[] );


extern bit EE_RSCAN_CreateInterrupt( u08  Unit_u08,
                                     u08  Channel_u08,
                                     u08  IntNumber_u08,
                                     u08  SetIntLevel_u08,
                                     void ( *FunctionVector )( ) );

extern bit EE_RSCAN_SetInterrupt( u08 Unit_u08,
                                  u08 Channel_u08,
                                  u08 InterruptSelection_u08,
                                  u16 InterruptSubSelection_u16 );

extern bit EE_RSCAN_GetStatus( 	u08  Unit_u08,
                                u08  Channel_u08,
                                u08  StatusNumber_u08,
                                pu08 StatusValue_u08 );

extern bit EE_RSCAN_GetError( u08  Unit_u08,
                              u08  Channel_u08,
                              pu16 InterruptErrorFlag_pu16,
                              pu16 LastErrorFlag_pu16 );


extern bit EE_RSCAN_SetGlobalConfiguration( u08 Unit_u08,
        const struct ee_rscan_cfg_global *Config );

extern bit EE_RSCAN_SetChannelConfiguration( u08 Unit_u08,
        u08 Channel_u08,
        const struct ee_rscan_cfg_channel *Config );

extern bit EE_RSCAN_SendMessage( u08  Unit_u08,
                                 u08  Channel_u08,
                                 u08 *Status_u08,
                                 struct ee_rscan_message *Message );

extern bit EE_RSCAN_TxAbort( u08  Unit_u08,
                             u08  Channel_u08,
                             struct ee_rscan_message *Message );

extern bit EE_RSCAN_ReceiveMessage( u08  Unit_u08,
                                    u08  Channel_u08,
                                    pu08 Status_pu08,
                                    struct ee_rscan_message *Message );

extern bit EE_RSCAN_SetAFLEntry( u08  Unit_u08,
                                 u08  Channel_u08,
                                 u16  RuleNumber_u16,
                                 struct ee_rscan_a_afl AFLEntry );

extern bit EE_RSCAN_Enable_RXFIFO(u08 Unit_u08, u08 RXFIFO_EN_u08);

extern bit EE_RSCAN_RAMTest( u08 Unit_u08 );

#endif

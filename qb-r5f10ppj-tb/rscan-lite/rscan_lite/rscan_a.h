/*===========================================================================*/
/* Module : rscan_a.h                                                        */
/* Version: V1.03                                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RSCAN prototypes                                                 */
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

#ifndef _RSCAN_A_H
#define _RSCAN_A_H

#include <ree_types.h>

extern bit EE_RSCAN_EnableINT( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_DisableINT( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_SendMessage_app( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_ReceiveMessage_app( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_COM_FIFO_TX_and_RX_FIFO_Test( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_RX_MB_self_test( u08 Unit_u08, u08 Channel1_u08, u08 Channel2_u08);

extern bit EE_RSCAN_Cyclic_COM_FIFO_self_test( u08 Unit_u08, u08 Channel1_u08, u08 Channel2_u08);

extern bit EE_RSCAN_TX_RX_MB_Test( u08 Unit_u08, u08 Channel_u08);

extern bit EE_RSCAN_TX_RX_MB_Loop_Test( u08 Unit_u08, u08 Channel_u08);
#endif

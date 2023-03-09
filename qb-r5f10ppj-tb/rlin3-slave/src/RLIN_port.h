/*===========================================================================*/
/* Module :  RLIN_port.h                                                     */
/* Version:  V1.00                                                           */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2012 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: main routine of QB-R5F10PPJ-TB startup sample                    */
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
/*              Device:          R5F10PPJ                                   */
/*              Target Hardware: QB-R5F10PPJ-TB                              */
/*              IDE:             IAR Systems                                 */
/*                               Embedded Workbench for RL78 V1.30.x         */
/*                                                                           */
/*===========================================================================*/
/* History:                                                                  */
/*              V1.00: Initial version                                       */
/*                                                                           */
/*===========================================================================*/

/***********************************************************************************************************************
* File Name    : RLIN_port.h
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for PORT module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

#ifndef PORT_H
#define PORT_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
  Port Mode Register (PMm)
*/
/* Pmn pin I/O mode selection (PMm7 - PMm0) */
#define _01_PMn0_NOT_USE            (0x01U) /* not use Pn0 as digital I/O */
#define _01_PMn0_MODE_INPUT         (0x01U) /* use Pn0 as input mode */
#define _00_PMn0_MODE_OUTPUT        (0x00U) /* use Pn0 as output mode */
#define _02_PMn1_NOT_USE            (0x02U) /* not use Pn1 as digital I/O */
#define _02_PMn1_MODE_INPUT         (0x02U) /* use Pn1 as input mode */
#define _00_PMn1_MODE_OUTPUT        (0x00U) /* use Pn1 as output mode */
#define _04_PMn2_NOT_USE            (0x04U) /* not use Pn2 as digital I/O */
#define _04_PMn2_MODE_INPUT         (0x04U) /* use Pn2 as input mode */
#define _00_PMn2_MODE_OUTPUT        (0x00U) /* use Pn2 as output mode */
#define _08_PMn3_NOT_USE            (0x08U) /* not use Pn3 as digital I/O */
#define _08_PMn3_MODE_INPUT         (0x08U) /* use Pn3 as input mode */
#define _00_PMn3_MODE_OUTPUT        (0x00U) /* use Pn3 as output mode */
#define _10_PMn4_NOT_USE            (0x10U) /* not use Pn4 as digital I/O */
#define _10_PMn4_MODE_INPUT         (0x10U) /* use Pn4 as input mode */
#define _00_PMn4_MODE_OUTPUT        (0x00U) /* use Pn4 as output mode */
#define _20_PMn5_NOT_USE            (0x20U) /* not use Pn5 as digital I/O */
#define _20_PMn5_MODE_INPUT         (0x20U) /* use Pn5 as input mode */
#define _00_PMn5_MODE_OUTPUT        (0x00U) /* use Pn5 as output mode */
#define _40_PMn6_NOT_USE            (0x40U) /* not use Pn6 as digital I/O */
#define _40_PMn6_MODE_INPUT         (0x40U) /* use Pn6 as input mode */
#define _00_PMn6_MODE_OUTPUT        (0x00U) /* use Pn6 as output mode */
#define _80_PMn7_NOT_USE            (0x80U) /* not use Pn7 as digital I/O */
#define _80_PMn7_MODE_INPUT         (0x80U) /* use Pn7 as input mode */
#define _00_PMn7_MODE_OUTPUT        (0x00U) /* use Pn7 as output mode */

/*
  Port Register (Pm)
*/
/* Pmn pin data (Pm0 to Pm7) */
#define _00_Pn0_OUTPUT_0            (0x00U) /* Pn0 output 0 */
#define _01_Pn0_OUTPUT_1            (0x01U) /* Pn0 output 1 */
#define _00_Pn1_OUTPUT_0            (0x00U) /* Pn1 output 0 */
#define _02_Pn1_OUTPUT_1            (0x02U) /* Pn1 output 1 */
#define _00_Pn2_OUTPUT_0            (0x00U) /* Pn2 output 0 */
#define _04_Pn2_OUTPUT_1            (0x04U) /* Pn2 output 1 */
#define _00_Pn3_OUTPUT_0            (0x00U) /* Pn3 output 0 */
#define _08_Pn3_OUTPUT_1            (0x08U) /* Pn3 output 1 */
#define _00_Pn4_OUTPUT_0            (0x00U) /* Pn4 output 0 */
#define _10_Pn4_OUTPUT_1            (0x10U) /* Pn4 output 1 */
#define _00_Pn5_OUTPUT_0            (0x00U) /* Pn5 output 0 */
#define _20_Pn5_OUTPUT_1            (0x20U) /* Pn5 output 1 */
#define _00_Pn6_OUTPUT_0            (0x00U) /* Pn6 output 0 */
#define _40_Pn6_OUTPUT_1            (0x40U) /* Pn6 output 1 */
#define _00_Pn7_OUTPUT_0            (0x00U) /* Pn7 output 0 */
#define _80_Pn7_OUTPUT_1            (0x80U) /* Pn7 output 1 */

/*
  Pull-up Resistor Option Register (PUm)
*/
/* Pmn pin on-chip pull-up resistor selection (PUmn) */
#define _00_PUn0_PULLUP_OFF         (0x00U) /* Pn0 pull-up resistor not connected */
#define _01_PUn0_PULLUP_ON          (0x01U) /* Pn0 pull-up resistor connected */
#define _00_PUn1_PULLUP_OFF         (0x00U) /* Pn1 pull-up resistor not connected */
#define _02_PUn1_PULLUP_ON          (0x02U) /* Pn1 pull-up resistor connected */
#define _00_PUn2_PULLUP_OFF         (0x00U) /* Pn2 Pull-up resistor not connected */
#define _04_PUn2_PULLUP_ON          (0x04U) /* Pn2 pull-up resistor connected */
#define _00_PUn3_PULLUP_OFF         (0x00U) /* Pn3 pull-up resistor not connected */
#define _08_PUn3_PULLUP_ON          (0x08U) /* Pn3 pull-up resistor connected */
#define _00_PUn4_PULLUP_OFF         (0x00U) /* Pn4 pull-up resistor not connected */
#define _10_PUn4_PULLUP_ON          (0x10U) /* Pn4 pull-up resistor connected */
#define _00_PUn5_PULLUP_OFF         (0x00U) /* Pn5 pull-up resistor not connected */
#define _20_PUn5_PULLUP_ON          (0x20U) /* Pn5 pull-up resistor connected */
#define _00_PUn6_PULLUP_OFF         (0x00U) /* Pn6 pull-up resistor not connected */
#define _40_PUn6_PULLUP_ON          (0x40U) /* Pn6 pull-up resistor connected */
#define _00_PUn7_PULLUP_OFF         (0x00U) /* Pn7 pull-up resistor not connected */
#define _80_PUn7_PULLUP_ON          (0x80U) /* Pn7 pull-up resistor connected */

/*
  Port Input Mode Register (PIMm)
*/
/* Pmn pin input buffer selection (PIMmn) */
#define _00_PIMn0_TTL_OFF           (0x00U) /* set Pn0 normal input buffer */
#define _01_PIMn0_TTL_ON            (0x01U) /* set Pn0 TTL input buffer */
#define _00_PIMn1_TTL_OFF           (0x00U) /* set Pn1 normal input buffer */
#define _02_PIMn1_TTL_ON            (0x02U) /* set Pn1 TTL input buffer */
#define _00_PIMn2_TTL_OFF           (0x00U) /* set Pn2 normal input buffer */
#define _04_PIMn2_TTL_ON            (0x04U) /* set Pn2 TTL input buffer */
#define _00_PIMn3_TTL_OFF           (0x00U) /* set Pn3 normal input buffer */
#define _08_PIMn3_TTL_ON            (0x08U) /* set Pn3 TTL input buffer */
#define _00_PIMn4_TTL_OFF           (0x00U) /* set Pn4 normal input buffer */
#define _10_PIMn4_TTL_ON            (0x10U) /* set Pn4 TTL input buffer */
#define _00_PIMn5_TTL_OFF           (0x00U) /* set Pn5 normal input buffer */
#define _20_PIMn5_TTL_ON            (0x20U) /* set Pn5 TTL input buffer */
#define _00_PIMn6_TTL_OFF           (0x00U) /* set Pn6 normal input buffer */
#define _40_PIMn6_TTL_ON            (0x40U) /* set Pn6 TTL input buffer */
#define _00_PIMn7_TTL_OFF           (0x00U) /* set Pn7 normal input buffer */
#define _80_PIMn7_TTL_ON            (0x80U) /* set Pn7 TTL input buffer */

/*
  Port Input Threshold Control Register (PITHLm)
*/
/* Pmn pin input threshold selection (PITHLmn) */
#define _00_PITHLn0_SCHMITT3_OFF    (0x00U) /* set Pn0 schmitt 1 input (default) */
#define _01_PITHLn0_SCHMITT3_ON     (0x01U) /* set Pn0 schmitt 3 input */
#define _00_PITHLn1_SCHMITT3_OFF    (0x00U) /* set Pn1 schmitt 1 input (default) */
#define _02_PITHLn1_SCHMITT3_ON     (0x02U) /* set Pn1 schmitt 3 input */
#define _00_PITHLn2_SCHMITT3_OFF    (0x00U) /* set Pn2 schmitt 1 input (default) */
#define _04_PITHLn2_SCHMITT3_ON     (0x04U) /* set Pn2 schmitt 3 input */
#define _00_PITHLn3_SCHMITT3_OFF    (0x00U) /* set Pn3 schmitt 1 input (default) */
#define _08_PITHLn3_SCHMITT3_ON     (0x08U) /* set Pn3 schmitt 3 input */
#define _00_PITHLn4_SCHMITT3_OFF    (0x00U) /* set Pn4 schmitt 1 input (default) */
#define _10_PITHLn4_SCHMITT3_ON     (0x10U) /* set Pn4 schmitt 3 input */
#define _00_PITHLn5_SCHMITT3_OFF    (0x00U) /* set Pn5 schmitt 1 input (default) */
#define _20_PITHLn5_SCHMITT3_ON     (0x20U) /* set Pn5 schmitt 3 input */
#define _00_PITHLn6_SCHMITT3_OFF    (0x00U) /* set Pn6 schmitt 1 input (default) */
#define _40_PITHLn6_SCHMITT3_ON     (0x40U) /* set Pn6 schmitt 3 input */
#define _00_PITHLn7_SCHMITT3_OFF    (0x00U) /* set Pn7 schmitt 1 input (default) */
#define _80_PITHLn7_SCHMITT3_ON     (0x80U) /* set Pn7 schmitt 3 input */

/*
  Port Output Mode Register (POMm)
*/
/* Pmn pin output mode selection (POMmn) */
#define _00_POMn0_NCH_OFF           (0x00U) /* set Pn0 output normal mode */
#define _01_POMn0_NCH_ON            (0x01U) /* set Pn0 output N-ch open-drain mode */
#define _00_POMn1_NCH_OFF           (0x00U) /* set Pn1 output normal mode */
#define _02_POMn1_NCH_ON            (0x02U) /* set Pn1 output N-ch open-drain mode */
#define _00_POMn2_NCH_OFF           (0x00U) /* set Pn2 output normal mode */
#define _04_POMn2_NCH_ON            (0x04U) /* set Pn2 output N-ch open-drain mode */
#define _00_POMn3_NCH_OFF           (0x00U) /* set Pn3 output normal mode */
#define _08_POMn3_NCH_ON            (0x08U) /* set Pn3 output N-ch open-drain mode */
#define _00_POMn4_NCH_OFF           (0x00U) /* set Pn4 output normal mode */
#define _10_POMn4_NCH_ON            (0x10U) /* set Pn4 output N-ch open-drain mode */
#define _00_POMn5_NCH_OFF           (0x00U) /* set Pn5 output normal mode */
#define _20_POMn5_NCH_ON            (0x20U) /* set Pn5 output N-ch open-drain mode */
#define _00_POMn6_NCH_OFF           (0x00U) /* set Pn6 output normal mode */
#define _40_POMn6_NCH_ON            (0x40U) /* set Pn6 output N-ch open-drain mode */
#define _00_POMn7_NCH_OFF           (0x00U) /* set Pn7 output normal mode */
#define _80_POMn7_NCH_ON            (0x80U) /* set Pn7 output N-ch open-drain mode */

/*
  Port Operation Mode Register (PMCm)
*/
/* Pmn pin digital input buffer selection (PMCmn) */
#define _01_PMCn0_NOT_USE           (0x01U) /* not use Pn0 digital input */
#define _00_PMCn0_DI_ON             (0x00U) /* enable Pn0 digital input */
#define _02_PMCn1_NOT_USE           (0x02U) /* not use Pn1 digital input */
#define _00_PMCn1_DI_ON             (0x00U) /* enable Pn1 digital input */
#define _04_PMCn2_NOT_USE           (0x04U) /* not use Pn2 digital input */
#define _00_PMCn2_DI_ON             (0x00U) /* enable Pn2 digital input */
#define _08_PMCn3_NOT_USE           (0x08U) /* not use Pn3 digital input */
#define _00_PMCn3_DI_ON             (0x00U) /* enable Pn3 digital input */
#define _10_PMCn4_NOT_USE           (0x10U) /* not use Pn4 digital input */
#define _00_PMCn4_DI_ON             (0x00U) /* enable Pn4 digital input */
#define _20_PMCn5_NOT_USE           (0x20U) /* not use Pn5 digital input */
#define _00_PMCn5_DI_ON             (0x00U) /* enable Pn5 digital input */
#define _40_PMCn6_NOT_USE           (0x40U) /* not use Pn6 digital input */
#define _00_PMCn6_DI_ON             (0x00U) /* enable Pn6 digital input */
#define _80_PMCn7_NOT_USE           (0x80U) /* not use Pn7 digital input */
#define _00_PMCn7_DI_ON             (0x00U) /* enable Pn7 digital input */

/*
  AD port configuration register (ADPC)
*/
/* Analog input/digital input switching (ADPC4 - ADPC0) */
#define _00_ADPC_DI_OFF             (0x00U) /* use P33, P34, P80 - P87, P90 - P97 as analog input */
#define _12_ADPC_DI_ON              (0x12U) /* use P97 as digital input */
#define _11_ADPC_DI_ON              (0x11U) /* use P96 - P97 as digital input */
#define _10_ADPC_DI_ON              (0x10U) /* use P95 - P97 as digital input */
#define _0F_ADPC_DI_ON              (0x0FU) /* use P94 - P97 as digital input */
#define _0E_ADPC_DI_ON              (0x0EU) /* use P93 - P97 as digital input */
#define _0D_ADPC_DI_ON              (0x0DU) /* use P92 - P97 as digital input */
#define _0C_ADPC_DI_ON              (0x0CU) /* use P91 - P97 as digital input */
#define _0B_ADPC_DI_ON              (0x0BU) /* use P90 - P97 as digital input */
#define _0A_ADPC_DI_ON              (0x0AU) /* use P87, P90 - P97 as digital input */
#define _09_ADPC_DI_ON              (0x09U) /* use P86 - P87, P90 - P97 as digital input */
#define _08_ADPC_DI_ON              (0x08U) /* use P85 - P87, P90 - P97 as digital input */
#define _07_ADPC_DI_ON              (0x07U) /* use P84 - P87, P90 - P97 as digital input */
#define _06_ADPC_DI_ON              (0x06U) /* use P83 - P87, P90 - P97 as digital input */
#define _05_ADPC_DI_ON              (0x05U) /* use P82 - P87, P90 - P97 as digital input */
#define _04_ADPC_DI_ON              (0x04U) /* use P81 - P87, P90 - P97 as digital input */
#define _03_ADPC_DI_ON              (0x03U) /* use P80 - P87, P90 - P97 as digital input */
#define _02_ADPC_DI_ON              (0x02U) /* use P34, P80 - P87, P90 - P97 as digital input */
#define _01_ADPC_DI_ON              (0x01U) /* use P33, P34, P80 - P87, P90 - P97 as digital input */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _F8_PM0_DEFAULT             (0xF8U) /* PM0 default value */
#define _F0_PM3_DEFAULT             (0xF0U) /* PM3 default value */
#define _9E_PM12_DEFAULT            (0x9EU) /* PM12 default value */
#define _FE_PM14_DEFAULT            (0xFEU) /* PM14 default value */
#define _3F_PMC9_DEFAULT            (0x3FU) /* PMC9 default value */
#define _DE_PMC12_DEFAULT           (0xDEU) /* PMC12 default value */

/*
  Port mode select register (PMS)
*/
#define _00_PMS_READ_PORT_LATCH     (0x00U) /* When PMmn=0 (output mode), Pnm (port latch) is read */
#define _01_PMS_READ_PIN_LEVEL      (0x01U) /* When PMmn=0 (output mode), pin level is read */


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_PORT_Create(void);

#endif

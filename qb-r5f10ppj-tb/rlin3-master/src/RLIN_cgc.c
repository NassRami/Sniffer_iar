/*===========================================================================*/
/* Module :  RLIN_cgc.c                                                      */
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
/*              Device:          R5F10PPJ                                    */
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
* File Name    : RLIN_cgc.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for CGC module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "RLIN_macrodriver.h"
#include "RLIN_cgc.h"
#include "RLIN_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function initializes the clock generator.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_Create(void)
{
    uint16_t w_count;
    uint8_t           temp_stab_set;
    uint8_t           temp_stab_wait; 

    /* Set fMX */
    CMC = _40_CGC_HISYS_OSC | _00_CGC_SUB_PORT | _00_CGC_SYSOSC_UNDER10M | _00_CGC_SUBMODE_DEFAULT;
    OSTS = _07_CGC_OSCSTAB_SEL18;
    MSTOP = 0U;
    temp_stab_set = _FF_CGC_OSCSTAB_STA18;

    do
    {
        temp_stab_wait = OSTC;
        temp_stab_wait &= temp_stab_set;
    }
    while (temp_stab_wait != temp_stab_set);

    /* Set fMAIN */
    MCM0 = 1U;
    /* Set fPLL */
    PLLCTL = _40_CGC_LOCKUP_WAIT_8 | _00_CGC_PLL_BELOW_32MHZ | _00_CGC_PLL_DIVISION_4MHZ | _02_CGC_PLL_MULTIPLY_X16;
    PLLON = 1U;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= CGC_PLLWAITTIME; w_count++)
    {
        NOP();
    }

    while ((PLLSTS & 0x80U) == 0U)
    {
        ;
    }

    /* Set fMP to PLL clock select mode */
    SELPLL = 1U;
    /* Set fSUB */
    XTSTOP = 1U;
    OSMC = _00_CGC_RTC_CLK_OTHER;
    /* Set fCLK */
    CSS = 0U;
    MDIV = _00_CGC_FMP_DIV_DEFAULT;
    /* Set fIH */
    HIOSTOP = 0U;
    /* Set RTC clock source */
    RTCCL = _00_CGC_RTC_FMX | _42_CGC_RTC_DIV122;
    /* Set Timer RD clock source to fCLK, fMP */
    TRD_CKSEL = 0U;
}



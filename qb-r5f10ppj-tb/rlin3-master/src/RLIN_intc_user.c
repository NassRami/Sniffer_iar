/*===========================================================================*/
/* Module :  RLIN_intc_user.c                                                */
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
* File Name    : RLIN_intc_user.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for INTC module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "RLIN_macrodriver.h"
#include "RLIN_intc.h"
#include "RLIN_Driver.h"
#include "RLIN_userdefine.h"

void Delay_ms(uint8_t i);
__interrupt static void r_intc0_interrupt(void);

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint8_t  Intp0_flag = 0U;

/***********************************************************************************************************************
* Function Name: r_intc0_interrupt
* Description  : This function is INTP0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTP0_vect
__interrupt static void r_intc0_interrupt(void)
{
    Delay_ms(80U);  /*Delay for debounce */
  R_INTC0_Stop();
   
    switch(Intp0_flag)
  {
  case 0U:  RLIN_Master_HeaderTransmit(0x08U);
    Intp0_flag++;
    LED1 = ON;
    LED2 = OFF;
     break;
  case 1U:   RLIN_Master_HeaderTransmit(0x49U);
    Intp0_flag++;
    LED1 = OFF;
    LED2 = ON;
     break;
  case 2U :  RLIN_Master_HeaderTransmit(0xCAU);
    Intp0_flag++;
    LED1 = ON;
    LED2 = ON;
    break;
  case 3U :  RLIN_Master_HeaderTransmit(0x8BU);
    Intp0_flag = 0U;
     break;
    
  default : break;  
  }
  
  
  
  R_INTC0_Start();

    
}



void Delay_ms(uint8_t i)    /*delay per ms*/
{
 uint16_t k;
  while(i > 0U)
  {
    i--;
    for(k=0U;k<3580U;k++) {
    }
  }
}

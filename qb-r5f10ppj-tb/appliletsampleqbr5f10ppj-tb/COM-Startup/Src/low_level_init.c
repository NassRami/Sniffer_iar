/*===========================================================================*/
/* Module : low_level_initialization.c                                       */
/* Version: V1.00                                                            */
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
/*        ABG Support                                                        */
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


/*===========================================================================*/
/* Includes                                                                  */
/*===========================================================================*/
/* IAR Systems Includes                                                      */
#include <intrinsics.h>
#include <ior5f10ppj.h>
#include <ior5f10ppj_ext.h>


/* Application Includes                                                      */
#include <types.h>
#include <macros.h>
#include <low_level_init.h>

/*===========================================================================*/
/* Constants                                                                 */
/*===========================================================================*/
/* Option Byte Definition                                                    */
/* watchdog enabled, LVD off, 32MHz clock, OCD interface enabled             */
__root __far const UCHAR OptionByte[OPT_BYTES_SIZE] @ 0x00C0u = {
  0xEEu, 0xFFu, 0xE8u, 0x04u
};

/* Security Byte Definition                                                  */
__root __far const UCHAR SecuIDCode[SECU_ID_SIZE]   @ 0x00C4u = {
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
  __disable_interrupt();
  int tmp = 0xff;


  /* -------------------------------------------------------------------------*/
  /* oscillator settings                                                      */
  /* -------------------------------------------------------------------------*/
  /* Set fMX */
  CMC = 0x42;
  OSTS = 0x80;


  CSC = 0x40;

  CSC &= ~0x80;



  while( (OSTC & 0x80) != 0x80) {  }

  /* Set fMAIN */
  CKC |= 0x10;
  /* Set fPLL */
  PLLCTL = 0x82;
  PLLON = 1U;

  /* Change the waiting time according to the system */
  for (tmp = 0; tmp <= 0xFF; tmp++)
  {
      asm("NOP");
  }

  while ((PLLSTS & 0x80) == 0)
  {
      ;
  }

  /* Set fMP to PLL clock select mode */
  SELPLL = 1U;
  /* Set fSUB */
  CSC |= 0x40;
  OSMC = 0x00;
  /* Set fCLK */
  CKC &= ~0x40;
  MDIV = 0x00;
  /* Set fIH */
  CSC |= 0x01;
  /* Set RTC clock source */
  RTCCL = 0x42;
  /* Set Timer RD clock source to fCLK, fMP */
  CKSEL &= ~0x04;



  
  /* -------------------------------------------------------------------------*/
  /* Timer Configuration                                                      */
  /* -------------------------------------------------------------------------*/
  

  /* -------------------------------------------------------------------------*/
  /* External Interrupt Configuration                                         */
  /* -------------------------------------------------------------------------*/
  PPR10         = 0u;                 /* Set INTP0 level 1 priority           */
  PPR00         = 1u;
  EGN0_bit.no0  = TRUE;               /* falling edge detection               */
  PIF0          = FALSE;              /* clear interrupt request flag         */
  PMK0          = IRQ_ENABLE;         /* enable ext. INTP0 interrupt          */

  /* -------------------------------------------------------------------------*/
  /* Safety Configuration                                                     */
  /* -------------------------------------------------------------------------*/
  CRC0CTL = 0x00U;           /* disable CRC calculation                       */
  IAWCTL  = 0x80U;           /* enable detection of invalid memory access     */

  return (int)(ENABLE_VAR_INIT);
}


void WDT_Restart(void) {
  WDTE = 0xACU;                        /* restart watchdog timer              */
}

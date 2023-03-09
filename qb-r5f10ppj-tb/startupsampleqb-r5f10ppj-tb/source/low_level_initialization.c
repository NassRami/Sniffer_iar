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
#include <low_level_initialization.h>

/*===========================================================================*/
/* Constants                                                                 */
/*===========================================================================*/
/* Option Byte Definition                                                    */
/* watchdog enabled, LVD off, 32MHz clock, OCD interface enabled             */
__root __far const UCHAR OptionByte[OPT_BYTES_SIZE] @ 0x00C0u = {
  0xFEu, 0xFFu, 0xE8u, 0x85u,
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
  /* -------------------------------------------------------------------------*/
  /* oscillator settings                                                      */
  /* -------------------------------------------------------------------------*/
  CMC     = 0x00u;    /* x1,x2,xt1 and xt2 as I/O pins,  low power oscillator */
  CSC     = 0xC0u;    /* use int. high-speed osc. clock fIH as system clock   */
  CKC     = 0x00u;    /* use internal high speed osc as cpu clock             */
  XTSTOP  = 1u;       /* XT1 oscillator stopped                               */
  HOCODIV = 0x00u;    /* select 32MHz clock                                   */  
  
  /* -------------------------------------------------------------------------*/
  /* I/O Port Configuration                                                   */
  /* -------------------------------------------------------------------------*/
  PM6_bit.no6 = OUTPUT;               /* use port P6.6 as output              */
  PM6_bit.no7 = OUTPUT;               /* use port P6.7 as output              */
  P6_bit.no6  = FALSE;                /* clear port register P6.6             */
  P6_bit.no7  = FALSE;                /* clear port register P6.7             */

  /* -------------------------------------------------------------------------*/
  /* Timer Configuration                                                      */
  /* -------------------------------------------------------------------------*/
  TAU0EN  = 1u;                        /* supply input clock                  */
  TT0     = 0x0AFFu;                   /* stop all channels                   */
  TPS0    = 0x0003u;                   /* clock frequency                     */
  TMMK00  = 1u;                        /* disable INTTM00 interrupt           */
  TMIF00  = 0u;                        /* clear INTTM00 interrupt flag        */
  TMPR100 = 0u;                        /* Set INTTM00 high priority           */
  TMPR000 = 0u;
  /* Channel 0 used as interval timer */
  TMR00   =  0x0000u;
  TDR00   =  0x9C3Fu;
  TO0    &= ~0x0001u;
  TOE0   &= ~0x0001u;

  TMIF00  = 0u;                        /* clear INTTM00 interrupt flag        */
  TMMK00 &= 0u;                        /* enable INTTM00 interrupt            */
  TS0    |= 0x0001u;                   /* start timer TM00                    */

  /* -------------------------------------------------------------------------*/
  /* Watchdog Timer Configuration                                             */
  /* -------------------------------------------------------------------------*/
  WDTIMK  = 1u;                        /* disable INTWDTI interrupt           */
  WDTIIF  = 0u;                        /* clear INTWDTI interrupt flag        */
  WDTIPR1 = 0u;                        /* Set INTWDTI high priority           */
  WDTIPR0 = 0u;
  WDTIMK  = 0u;                        /* enable INTWDTI interrupt            */

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

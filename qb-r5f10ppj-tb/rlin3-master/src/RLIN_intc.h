/*===========================================================================*/
/* Module :  RLIN_intc.h                                                     */
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
* File Name    : RLIN_intc.h
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for INTC module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

#ifndef INTC_H
#define INTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/*
    External Interrupt Rising Edge Enable Register 0 (EGP0)
*/
/* INTPn pin valid edge selection (EGPn) */
#define _01_INTP0_EDGE_RISING_SEL           (0x01U)   /* rising edge selected for INTP0 pin */
#define _00_INTP0_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP0 pin */
#define _02_INTP1_EDGE_RISING_SEL           (0x02U)   /* rising edge selected for INTP1 pin */
#define _00_INTP1_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP1 pin*/
#define _04_INTP2_EDGE_RISING_SEL           (0x04U)   /* rising edge selected for INTP2 pin */
#define _00_INTP2_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP2 pin */
#define _08_INTP3_EDGE_RISING_SEL           (0x08U)   /* rising edge selected for INTP3 pin */
#define _00_INTP3_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP3 pin */
#define _10_INTP4_EDGE_RISING_SEL           (0x10U)   /* rising edge selected for INTP4 pin */
#define _00_INTP4_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP4 pin */
#define _20_INTP5_EDGE_RISING_SEL           (0x20U)   /* rising edge selected for INTP5 pin */
#define _00_INTP5_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP5 pin */
#define _40_INTP6_EDGE_RISING_SEL           (0x40U)   /* rising edge selected for INTP6 pin */
#define _00_INTP6_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP6 pin */
#define _80_INTP7_EDGE_RISING_SEL           (0x80U)   /* rising edge selected for INTP7 pin */
#define _00_INTP7_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP7 pin */

/*
    External Interrupt Falling Edge Enable Register 0 (EGN0)
*/
/* INTPn pin valid edge selection (EGNn) */
#define _01_INTP0_EDGE_FALLING_SEL          (0x01U)   /* falling edge selected for INTP0 pin */
#define _00_INTP0_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP0 pin */
#define _02_INTP1_EDGE_FALLING_SEL          (0x02U)   /* falling edge selected for INTP1 pin */
#define _00_INTP1_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP1 pin */
#define _04_INTP2_EDGE_FALLING_SEL          (0x04U)   /* falling edge selected for INTP2 pin */
#define _00_INTP2_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP2 pin */
#define _08_INTP3_EDGE_FALLING_SEL          (0x08U)   /* falling edge selected for INTP3 pin */
#define _00_INTP3_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP3 pin */
#define _10_INTP4_EDGE_FALLING_SEL          (0x10U)   /* falling edge selected for INTP4 pin */
#define _00_INTP4_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP4 pin */
#define _20_INTP5_EDGE_FALLING_SEL          (0x20U)   /* falling edge selected for INTP5 pin */
#define _00_INTP5_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP5 pin */
#define _40_INTP6_EDGE_FALLING_SEL          (0x40U)   /* falling edge selected for INTP6 pin */
#define _00_INTP6_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP6 pin */
#define _80_INTP7_EDGE_FALLING_SEL          (0x80U)   /* falling edge selected for INTP7 pin */
#define _00_INTP7_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP7 pin */

/*
    External Interrupt Rising Edge Enable Register 1 (EGP1)
*/
/* INTPn pin valid edge selection (EGPn) */
#define _01_INTP8_EDGE_RISING_SEL           (0x01U)   /* rising edge selected for INTP8 pin */
#define _00_INTP8_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP8 pin */
#define _02_INTP9_EDGE_RISING_SEL           (0x02U)   /* rising edge selected for INTP9 pin */
#define _00_INTP9_EDGE_RISING_UNSEL         (0x00U)   /* rising edge not selected for INTP9 pin*/
#define _04_INTP10_EDGE_RISING_SEL          (0x04U)   /* rising edge selected for INTP10 pin */
#define _00_INTP10_EDGE_RISING_UNSEL        (0x00U)   /* rising edge not selected for INTP10 pin */
#define _08_INTP11_EDGE_RISING_SEL          (0x08U)   /* rising edge selected for INTP11 pin */
#define _00_INTP11_EDGE_RISING_UNSEL        (0x00U)   /* rising edge not selected for INTP11 pin*/

/*
    External Interrupt Falling Edge Enable Register 1 (EGN1)
*/
/* INTPn pin valid edge selection (EGNn) */
#define _01_INTP8_EDGE_FALLING_SEL          (0x01U)   /* falling edge selected for INTP8 pin */
#define _00_INTP8_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP8 pin */
#define _02_INTP9_EDGE_FALLING_SEL          (0x02U)   /* falling edge selected for INTP9 pin */
#define _00_INTP9_EDGE_FALLING_UNSEL        (0x00U)   /* falling edge not selected for INTP9 pin */
#define _04_INTP10_EDGE_FALLING_SEL         (0x04U)   /* falling edge selected for INTP10 pin */
#define _00_INTP10_EDGE_FALLING_UNSEL       (0x00U)   /* falling edge not selected for INTP10 pin */
#define _08_INTP11_EDGE_FALLING_SEL         (0x08U)   /* falling edge selected for INTP11 pin */
#define _00_INTP11_EDGE_FALLING_UNSEL       (0x00U)   /* falling edge not selected for INTP11 pin */

/*
    Key Return Mode Register (KRM)
*/
/* Key interrupt mode control (KRM0) */
#define _00_KR0_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR0 signal */
#define _01_KR0_SIGNAL_DETECT_ON            (0x01U)   /* detect KR0 signal */
/* Key interrupt mode control (KRM1) */
#define _00_KR1_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR1 signal */
#define _02_KR1_SIGNAL_DETECT_ON            (0x02U)   /* detect KR1 signal */        
/* Key interrupt mode control (KRM2) */
#define _00_KR2_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR2 signal */
#define _04_KR2_SIGNAL_DETECT_ON            (0x04U)   /* detect KR2 signal */        
/* Key interrupt mode control (KRM3) */
#define _00_KR3_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR3 signal */
#define _08_KR3_SIGNAL_DETECT_ON            (0x08U)   /* detect KR3 signal */        
/* Key interrupt mode control (KRM4) */
#define _00_KR4_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR4 signal */
#define _10_KR4_SIGNAL_DETECT_ON            (0x10U)   /* detect KR4 signal */        
/* Key interrupt mode control (KRM5) */
#define _00_KR5_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR5 signal */
#define _20_KR5_SIGNAL_DETECT_ON            (0x20U)   /* detect KR5 signal */        
/* Key interrupt mode control (KRM6) */
#define _00_KR6_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR6 signal */
#define _40_KR6_SIGNAL_DETECT_ON            (0x40U)   /* detect KR6 signal */        
/* Key interrupt mode control (KRM7) */
#define _00_KR7_SIGNAL_DETECT_OFF           (0x00U)   /* not detect KR7 signal */
#define _80_KR7_SIGNAL_DETECT_ON            (0x80U)   /* detect KR7 signal */        

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _FF_INTMSK_MASK_VALUE               (0xFFU)    /* Set mask control of interrupt factor */
#define KEY_WAITTIME                        (U)      /* set wait time */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_INTC_Create(void);
void R_INTC0_Start(void);
void R_INTC0_Stop(void);


#endif

/*===========================================================================*/
/* Module :  RLIN_macrodriver.h                                              */
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
* File Name    : RLIN_macrodriver.h
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements general head file.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

#ifndef STATUS_H
#define STATUS_H
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "ior5f10ppj.h"
#include "ior5f10ppj_ext.h"
#include "intrinsics.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF__
#define DI      __disable_interrupt
#define EI      __enable_interrupt
#define HALT    __halt
#define NOP     __no_operation
#define STOP    __stop
/* Status list definition */
#define MD_STATUSBASE       0x00U
#define MD_OK               (MD_STATUSBASE + 0x00U) /* register setting OK */
#define MD_SPT              (MD_STATUSBASE + 0x01U) /* IIC stop */
#define MD_NACK             (MD_STATUSBASE + 0x02U) /* IIC no ACK */
#define MD_BUSY1            (MD_STATUSBASE + 0x03U) /* busy 1 */
#define MD_BUSY2            (MD_STATUSBASE + 0x04U) /* busy 2 */

/* Error list definition */
#define MD_ERRORBASE        0x80U
#define MD_ERROR            (MD_ERRORBASE + 0x00U)  /* error */
#define MD_ARGERROR         (MD_ERRORBASE + 0x01U)  /* error agrument input error */
#define MD_ERROR1           (MD_ERRORBASE + 0x02U)  /* error 1 */
#define MD_ERROR2           (MD_ERRORBASE + 0x03U)  /* error 2 */
#define MD_ERROR3           (MD_ERRORBASE + 0x04U)  /* error 3 */
#define MD_ERROR4           (MD_ERRORBASE + 0x05U)  /* error 4 */
#define MD_DATAEXISTS       (MD_ERRORBASE + 0x06U)  /* data to be transferred next exists in TXBn register */
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#ifndef __TYPEDEF__
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;
typedef signed long         int32_t;
typedef unsigned long       uint32_t;
typedef unsigned short      MD_STATUS;
#define __TYPEDEF__
#endif

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/

#endif

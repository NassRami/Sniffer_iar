/*===========================================================================*/
/* Module : ree_types.h                                                      */
/* Version: V1.1.1.1                                                         */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: RENESAS Coding Standard Type Definitions                         */
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
/*              Device:          RL78                                        */
/*              IDE:             IAR Systems                                 */
/*                               Embedded Workbench for RL78 V1.30.x         */
/*                                                                           */
/*===========================================================================*/
/* History:                                                                  */
/* Revision 1.1.1.1  2002.01.22 LiederR   Coding Standard introduced.        */
/*                                                                           */
/*===========================================================================*/


#ifndef _REE_TYPES_H
#define _REE_TYPES_H
#define _NEC_TYPES_H

#define TRUE    1
#define FALSE   0

#ifndef EE_API
#define EE_API
#endif

typedef enum { false = FALSE, true = TRUE } bit;

#ifndef _TYPEDEF_H_
#define EE_NULL (void *)0
typedef bit ee_Bool_t;
#ifndef uint8_t
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
#ifdef __ghs__
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;
#endif
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
#endif /* uint8_t */
#endif /* _TYPEDEF_H_ */

typedef bit bit_t;

typedef unsigned char  u08;
typedef unsigned char  u08_t;
typedef unsigned short u16;
typedef unsigned short u16_t;
typedef unsigned long  u32;
typedef unsigned long  u32_t;

typedef signed char  s08;
typedef signed char  s08_t;
typedef signed short s16;
typedef signed short s16_t;
typedef signed long  s32;
typedef signed long  s32_t;

typedef float       flt;
typedef float       flt_t;
typedef double      dbl;
typedef double      dbl_t;
typedef long double lgd;
typedef long double lgd_t;

typedef unsigned char  * pu08;
typedef unsigned char  * pu08_t;
typedef unsigned short * pu16;
typedef unsigned short * pu16_t;
typedef unsigned long  * pu32;
typedef unsigned long  * pu32_t;

typedef signed char  * ps08;
typedef signed char  * ps08_t;
typedef signed short * ps16;
typedef signed short * ps16_t;
typedef signed long  * ps32;
typedef signed long  * ps32_t;

typedef float       * pflt;
typedef float       * pflt_t;
typedef double      * pdbl;
typedef double      * pdbl_t;
typedef long double * plgd;
typedef long double * plgd_t;

#endif

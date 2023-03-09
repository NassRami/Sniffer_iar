/*===========================================================================*/
/* Module :  RLIN_driver_user.c                                              */
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
* File Name    : RLIN_driver_user.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Interrupt module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

#include "RLIN_macrodriver.h"
#include "RLIN_driver.h"
#include "RLIN_userdefine.h"

uint8_t GetIDbuffer;
uint8_t Slave_RxData1[8];            /*reception data store array*/
uint8_t Slave_RxData2[8];            /*reception data store array*/
uint8_t Slave_RxData3[8];            /*reception data store array*/
uint8_t Slave_TxData[]={0x55U,0xC0U};  /*Transmission data store array*/

__interrupt static void RLIN0_Transmission_interrupt(void);
__interrupt static void RLIN0_Reception_interrupt(void);
__interrupt static void RLIN0_Status_interrupt(void);
__interrupt static void RLIN0_Wakeup_interrupt(void);

/***********************************************************************************************************************
* Function Name: RLIN0_Transmission_interrupt
* Description  : This function is RLIN0 Transmission interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTLIN0TRM_vect
__interrupt static void RLIN0_Transmission_interrupt(void)
{
    
  LST0&=0xFEU; 

}


/***********************************************************************************************************************
* Function Name: RLIN0_Reception_interrupt
* Description  : This function is RLIN0 Reception interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTLIN0RVC_vect
__interrupt static void RLIN0_Reception_interrupt(void)
{  


  uint8_t receive_header_flag;
  uint8_t receive_reponse_flag;
  receive_header_flag  = LST0 & 0x80U;         /* get header reception flag */
  receive_reponse_flag = LST0 & 0X02U;         /* get response rception flag*/

    
GetIDbuffer=LIDB0;

if(receive_header_flag)  /* Header successful receive*/
{
  LIE0 &= 0xF7U;    /*Disable header interrupt*/
  LST0 &= 0X7FU;    /*clear successful header reception flag*/
 
switch(GetIDbuffer)
{
   case 0x08: RLIN_Slave_Receive(3U);
           break;
  case 0x49: RLIN_Slave_Receive(3U);
           break;
  case 0xCA: RLIN_Slave_Receive(3U);
           break;
  case 0x8B: RLIN_Slave_Transmit(Slave_TxData,2U);
             P6=Slave_TxData[1];
           break;  
  default:   RLIN_Slave_NoResponse();
           break;
  }
  LIE0 |= 0x80U;   /*enable header reception interrupt*/
}
  
if(receive_reponse_flag)
{
  LIE0 &= 0xFDU;      /* disable response reception interrupt*/
  LST0 &= 0xFDU;      /* clear response reception successful flag*/
  
  switch(GetIDbuffer)
{
  case 0x08:  P6 = Get_reponse_RxData(Slave_RxData1);            
              break;
  case 0x49:  P6 = Get_reponse_RxData(Slave_RxData2);              
              break;
  case 0xCA:  P6 = Get_reponse_RxData(Slave_RxData3);             
              break;
  default: break;
  }

  LIE0|=0x02U;  /*enable reception interrupt*/
  }
  LTRC0=0x01U;    /*enabled header reception interrupt*/
}

/***********************************************************************************************************************
* Function Name: RLIN0_Status_interrupt
* Description  : This function is RLIN0 Status interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#pragma vector = INTLIN0_vect
__interrupt static void RLIN0_Status_interrupt(void)
{
while(1U)
{
  ;
}
}



/***********************************************************************************************************************
* Function Name: RLIN0_Wakeup_interrupt
* Description  : This function is RLIN0 Wakeup interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = INTLIN0WUP_vect
__interrupt static void RLIN0_Wakeup_interrupt(void)
{
 LCUC0 = 0x03U;
 LED1  = ON; 
 LED2  = ON;
}

/*===========================================================================*/
/* Module :  RLIN_driver.c                                                   */
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
* File Name    : RLIN_driver.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for PORT module.
* Creation Date: 15.07.2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "RLIN_macrodriver.h"
#include "RLIN_driver.h"
#include "RLIN_userdefine.h"


uint8_t Master_TxData1[]={0x08U,0x80U,0x20U};   /*Transmission data store array*/
uint8_t Master_TxData2[]={0x49U,0x40U,0x10U};   /*Transmission data store array*/
uint8_t Master_TxData3[]={0xCAU,0x0FU,0x0FU};   /*Transmission data store array*/

uint8_t Master_RxData1[8U];
uint8_t Master_RxData2[8U];
uint8_t Master_RxData3[8U];







/***********************************************************************************************************************
* Function Name: RLIN_Master_Init(void)
* Description  : This function initializes the RLIN Slave node, setting clock supply,baud rate,ect.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void RLIN_Master_Init(void)
{
  LCHSEL = 0x00U;        /*  Selects RLIN0 */
  PER2  |= 0x04U;        /*  Enable input clock supply RLIN0*/
  LINCKSEL=0x00U;        /*  selects the  fclk=32MHz  clock to RLIN0.*/
  LWBR0  = 0x01U;        /*  b0=1, LIN2.0 or 2.1;  Prescaler Clock Selcet 1/1;  bit sampling count select 0000 : 16 sampling. */ 
  LBRP00 = 0x67U;        /*   fa:0X67=103D, Baud rate= 32M/ (103+1)*16= 19230 bps  fb:9615bps    fc:2403bps */
  LBRP01 = 0x5FU;        /*   fd:10416bps*/
  
  LIN0RVCIF = 0U;        /*  Clear Reception interrupt request signal */
  LIN0TRMIF = 0U;        /*  Clear Transmission interrupt request signal */
  LIN0WUPIF = 0U;        /*  Clear Wake up interrupt request signal */
  LIN0IF    = 0U;        /*  Clear LIN or LIN Status interrupt */
  
  LIN0RVCMK = 0U;        /*  interrupt reception servicing enable */
  LIN0TRMMK = 0U;        /*  interrupt transmission servicing enable */
  LIN0WUPMK = 0U;        /*  interrupt wake up servicing enable */
  LIN0MK    = 0U;        /*  interrupt Status servicing enable */
  LIE0  |=  0x0FU;       /*  Enable successful response/wake-up reception interrupt, enable all interrupt*/
  LEDE0 |=  0x8FU;       /*  Enable error detection */
  
   /*  Header format setting*/
  LMD0  =   0x10U;        /*   b0b1=00: LIN master mode ; b3b2=00: fa=LIN sysclock;  b4=1:transmission interrupt,sucessful reception interrupt...; b5=0: The noise filter is enable.*/
  LBFC0 =   0x15U;        /*  b3-b0=0101: transmission break width 18Tbits; b5b4=01: break delimiter 2Tbit*/
  LSC0  =   0x11U;        /*  b2-b0=001:inter-byte space 1bit or Response space 4bit; b5b4=01: inter-byte space 1Tbit */
  LWUP0 =   0x30U;        /*  b7-b4=0100: Wake-up Transmission low width 4 bits.*/
  LIDB0 &=  0x00U;        /*  Clear the ID buffer */
 
  ISC   =   0x00U;        /*  INTP11 pin input signal is set as external interrupt input*/
  LINCKSEL|=0x10U;        /*  Enable RLIN0 engine clock supply,*/
}

/***********************************************************************************************************************
* Function Name: RLIN_Master_HeaderTransmit(uint8_t ID)
* Description  : This function is setting in slave mode, enable header reception is started.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void RLIN_Master_HeaderTransmit(uint8_t ID)
{
 
  LCUC0 = 0x03U;          /*  01: RLIN rest mode is canceled; 03:RLIN operation mode */
  LIDB0 = 0X00U;          /*  clear ID buffer */
  LIDB0 = ID;             /*  ID  load to ID buffer */ 
  Clear_DataBuffer();     /* clear all data buffer */
  
  switch(ID)                
      {
        case 0x08U: RLIN_Master_Transmit(Master_TxData1,3U);      /* ready for response transmit*/
                    break;
        case 0x49U: RLIN_Master_Transmit(Master_TxData2,3U);      /* ready for response transmit*/
                    break;
        case 0xCAU: RLIN_Master_Transmit(Master_TxData3,3U);      /* ready for response transmit*/
                    break;
        case 0x8BU: RLIN_Master_Receive(2U);                      /* ready for response receive*/
                    break;
        default:    break;
        }
    
    LTRC0=0x01U;  /*FTS=1,Frame transmit is started*/
}

/***********************************************************************************************************************
* Function Name: RLIN_Master_Transmit(void)
* Description  : This function seting data buffer for response transmission start
* Arguments    : uint8_t* databuf    : variable array data. 
                 uint8_t Data_length : transmit data length.
* Return Value : None
***********************************************************************************************************************/
void RLIN_Master_Transmit(uint8_t * databuf,uint8_t Data_length)
{
  uint8_t i;
  uint16_t  Databuf_adr;
  LDFC0=0x30U;
  LDFC0|=Data_length;    /*  MSB=0011, RCDS=1:Transmission, LCS=1: Enhanced checksum mode;  LSB=0100: response data lengh select 4 byte*/
  Databuf_adr=RLIN_DateBuffer;
  for(i=0U;i<Data_length;i++)
  {
    *((uint8_t *)(Databuf_adr+i))=databuf[i];  
  }   
}

/***********************************************************************************************************************
* Function Name: RLIN_Master_Receive(void)
* Description  : This function clear data buffer ready for response reception start
* Arguments    : uint8_t Data_length : receive data length.
* Return Value : None
***********************************************************************************************************************/
void RLIN_Master_Receive(uint8_t Data_length)
{

  Clear_DataBuffer();   
  LDFC0 =0x20U;
  LDFC0|=Data_length;   /* MSB=0011, RCDS=1:Transmission, LCS=1: Enhanced checksum mode;  LSB=0100: response data lengh select 4 byte*/
}


/***********************************************************************************************************************
* Function Name: RLIN_Master_GetData(void)
* Description  : This function is get data from data buffr according to ID data during the response reception
* Arguments    : uint8_t Data_length : receive data length.
* Return Value : None
***********************************************************************************************************************/

void RLIN_Master_GetData(void)
{
    switch(LIDB0)
    {
    case 0x8B: P6 = Get_response_RxData(Master_RxData1);           
             break;
 /*  case 0x4c: Get_reponse_RxData(Master_RxData2);      
             break*/
 /*    case 0x0D: Get_reponse_RxData(Master_RxData3);      
              break*/            
    default: break;    
      }
}

   

/***********************************************************************************************************************
* Function Name: Clear_DataBuffer
* Description  : This function setting all data buffer to some value
* Arguments    : uint8_t x : setting data buff value
* Return Value : None
***********************************************************************************************************************/

void Clear_DataBuffer()
{   
  uint8_t i;
  uint16_t Databuf_adr;
  Databuf_adr=RLIN_DateBuffer;
  for(i=0U;i<8U;i++)
  {
   *((uint8_t *)(Databuf_adr+i))=0U;
  } 

}


/***********************************************************************************************************************
* Function Name: Get_reponse_RxData
* Description  : This function get data buffer value to a variable array  
* Arguments    : uint8_t * RxData : a avriable array for store Data
* Return Value : None
***********************************************************************************************************************/
uint8_t Get_response_RxData(uint8_t * RxData)
{
  uint8_t i,k;
  uint16_t Databuf_adr;
  k=LDFC0&0x0FU;
  Databuf_adr=RLIN_DateBuffer;
  for(i=0U;i<k;i++)
  {
    RxData[i]=(*((uint8_t *)(Databuf_adr+i)));  
  } 
  return RxData[1U];
}



/***************************************************************************
;**
;** FILE NAME      : STARTUPapi.c
;**
;** FILE AUTHOR    : GRASSET Bruno
;**
;** MODULES        : STARTUPapi.1.1 / main
;**                  STARTUPapi.1.2 / STARTUP_GetCurrentTime
;**
;**************************************************************************/
#include "LGM_OS.h"
#include "APPLI.h"

#include "STARTUP.h"
#include "STARTUP_priv.h"

extern volatile UINT8 FlagConnected;
/*!
*****************************************************************************
*  @brief            Start the full application
*****************************************************************************
*  - MODULE Nø/NAME : STARTUPapi.1.1 / main
*  - REFERENCES     : XYZ
*  @requirement       SDDD-STARTUP-Req 01-1 \n
*                     -
*****************************************************************************
*  - PRECOND:        None
*  - CONSTRT:        None
* .
*  @param[in]        None
*  @param[in,out]    None
*  @param[out]       None
*  @return           INT16     Application return code
*****************************************************************************/
void main(void)
{
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * Configuration de la PLL
     * A faire dans __low_level_init
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */
  
 // __enable_interrupt();
  /* Initializing OSAL */
  if( LGM_OS_Init() == LGM_E_OK )
  {
    /* Initializing application */
    APPLI_Init();

    /* Loop until end of program */
 	Appli_Run();
  }
  else
  {
		; /* Problem while initializing OSAL, return error code */
	}
}



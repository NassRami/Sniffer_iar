; ===========================================================================
;  Module : hpi_init.asm                                       
;  Version: E1.00a                                                            
; ===========================================================================
;                                   COPYRIGHT                                
; ===========================================================================
;  Copyright (c) 2013 by Renesas Electronics Europe GmbH,                    
;                a company of the Renesas Electronics Corporation            
; ===========================================================================
;  Purpose: hot plugin initialization
;                                                                            
; ===========================================================================
;  In case of any question please do not hesitate to contact:                
;                                                                            
;         ABG.Support                                          
;                                                                            
;         Renesas Electronics Europe GmbH                                    
;         Arcadiastrasse 10                                                  
;         D-40472 Duesseldorf, Germany                                       
;                                                                            
;         e-mail: device_support.rl78-eu@lm.renesas.com                      
;         FAX:   +49 - (0)211 / 65 03 - 12 79                                
;                                                                            
; ===========================================================================
;  Warranty Disclaimer                                                       
;                                                                            
;  Because the Product(s) is licensed free of charge, there is no warranty   
;  of any kind whatsoever and expressly disclaimed and excluded by Renesas,  
;  either expressed or implied, including but not limited to those for       
;  non-infringement of intellectual property, merchantability and/or         
;  fitness for the particular purpose.                                       
;  Renesas shall not have any obligation to maintain, service or provide bug 
;  fixes for the supplied Product(s) and/or the Application.                 
;                                                                            
;  Each User is solely responsible for determining the appropriateness of    
;  using the Product(s) and assumes all risks associated with its exercise   
;  of rights under this Agreement, including, but not limited to the risks   
;  and costs of program errors, compliance with applicable laws, damage to   
;  or loss of data, programs or equipment, and unavailability or             
;  interruption of operations.                                               
;                                                                            
;  Limitation of Liability                                                   
;                                                                            
;  In no event shall Renesas be liable to the User for any incidental,       
;  consequential, indirect, or punitive damage (including but not limited    
;  to lost profits) regardless of whether such liability is based on breach  
;  of contract, tort, strict liability, breach of warranties, failure of     
;  essential purpose or otherwise and even if advised of the possibility of  
;  such damages. Renesas shall not be liable for any services or products    
;  provided by third party vendors, developers or consultants identified or  
;  referred to the User by Renesas in connection with the Product(s) and/or  
;  the Application.                                                          
;                                                                            
; ===========================================================================
;  Environment:                                                              
;               Device:          R5F10PMF                                    
;               Target Hardware: QB-R5F10PMF-TB                              
;               IDE:             IAR Systems                                 
;                                Embedded Workbench for RL78 V1.30.x         
;                                                                            
; ===========================================================================
;  History:                                                                  
;               E1.00a: Initial version  for IAR assembler                                    
;                                                                            
; ===========================================================================


#include <ior5f10PMF.h>
#include <ior5f10PMF_ext.h>

#define DTCEN      PER1.3

        MODULE __HPI_INIT__
        PUBLIC hpi_initialize_prog
      
    
        RSEG RCODE:CODE(0)
hpi_initialize_prog:
  PUSH    DE
; ===========================================================================
; --- Live Debug Setting                                                  ---
; ===========================================================================
; --- Store the ID Code into BRAM                                         ---
  MOV     ES,#0x0000
  MOVW    DE,#0x00C4
  MOVW    HL,#0x0722
  BT     [HL].7,SET_TRCSIZE1  ; BRKMOD(0xF0722)
SET_TRCSIZE0: 
  MOVW    HL,#0x0D00          ; Store Address of ID Code (TRCSIZE=0)
  BR      SEC_CODE_SET
SET_TRCSIZE1:
  MOVW    HL,#0x0E00          ; Store Address of ID Code (TRCSIZE=1)
SEC_CHK:
  MOV     A,0x0723                    
  BT      A.0, DTC_DES_SET    
            
SEC_CODE_SET:         
  MOV     A,ES:[DE]           ; Write ID Code Data(Copy from ROM) to BRAM
  MOV     [HL],A
  INCW    DE
  INCW    HL
  MOVW    AX,DE
  CMPW    AX,#0x00CE          ; 10byte
  SKZ
  BR      SEC_CODE_SET      
DTC_DES_SET:          
  MOV     ES,#0x0F    
  MOV     L,#0x0E
  MOVW    AX,#0xFFAD          ; TXS/RXD
  MOVW    [HL],AX             ; 0x0E <- 0xAD, 0x0F <- 0xFF
  MOV     L,#0x10             ; HL=0xF0010
; ===========================================================================
DTC_SET:
  MOV     [HL+0x00],#0x13     ; DTCCR
  MOV     [HL+0x01],#0x01     ; DTBLS 1byte
  MOV     [HL+0x02],#0x0A     ; DTCCT 10-times
  MOV     [HL+0x03],#0x0A     ; DTRLD 10-times
  MOV     [HL+0x04],#0x00     ; DTSAR <- F0D00H or F0E00H
  MOV     A,H                 ; 
  MOV     [HL+0x5],A          ; 
  MOV     [HL+0x06],#0x23     ; DTDAR <- F0723H(SECCHK)
  MOV     [HL+0x07],#0x07     ; 
; chain transfer (for handshake)
  MOV     [HL+0x08],#0x00     ; DTCCR normal
  MOV     [HL+0x09],#0x01     ; DTBLS 1byte
  MOV     [HL+0x0A],#0x01     ; DTCCT 1-time
  MOV     [HL+0x0B],#0x01     ; DTRLD 1-time
  MOV     [HL+0x0C],#0x23     ; DTSAR <- F0723H(LDSTA)
  MOV     [HL+0x0D],#0x07     ; 
  MOV     [HL+0x0E],#0xAD     ; DTDAR <- FFFADH(TXS/RXB)
  MOV     [HL+0x0F],#0xFF     ; 
; ===========================================================================
; --- Store the Control Data for RRM/DMM Address Setting to BRAM          ---
  MOV     [HL+0x10],#0x11    ; DTCCR
  MOV     [HL+0x11],#0x01    ; DTBLS 1byte
  MOV     [HL+0x12],#0x10    ; DTCCT 10-times
  MOV     [HL+0x13],#0x10    ; DTRLD 10-times
  MOV     [HL+0x14],#0xAD    ; DTSAR <- FFFADH(TXS/RXB)
  MOV     [HL+0x15],#0xFF    ; 
  MOV     [HL+0x16],#0x00    ; 
  MOV     A,H                ; 
  MOV     [HL+0x17],A        ; 
; chain transfer (for hand shake)
  MOV     [HL+0x18],#0x03    ; DTCCR normal
  MOV     [HL+0x19],#0x01    ; DTBLS 1byte
  MOV     [HL+0x1A],#0x10    ; DTCCT 1-time
  MOV     [HL+0x1B],#0x10    ; DTRLD 1-time
  MOV     [HL+0x1C],#0x00    ; DTSAR <- 0xF0D00 or 0xF0E00
  MOV     A,H   ; 
  MOV     [HL+0x1D],A ; 
  MOV     [HL+0x1E],#0xAD    ; DTDAR <- 0xFFFAD(TXS/RXB)
  MOV     [HL+0x1F],#0xFF    ;
; ===========================================================================
  SET1     DTCEN             ; DTC enable
  
  POP      DE
  RET 
  
  ENDMOD
  
  END

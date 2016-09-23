;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME:   LED4.asm
;;  Version: 1.40, Updated on 2010/9/19 at 23:40:44
;;  Generated by PSoC Designer 5.1.1875.0
;;
;;  DESCRIPTION: LED user module.
;;
;;
;;  NOTE: User Module APIs conform to the fastcall16 convention for marshalling
;;        arguments and observe the associated "Registers are volatile" policy.
;;        This means it is the caller's responsibility to preserve any values
;;        in the X and A registers that are still needed after the API functions
;;        returns. For Large Memory Model devices it is also the caller's 
;;        responsibility to perserve any value in the CUR_PP, IDX_PP, MVR_PP and 
;;        MVW_PP registers. Even though some of these registers may not be modified
;;        now, there is no guarantee that will remain the case in future releases.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2010. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "LED4.inc"
include "memory.inc"

export _LED4_Start
export  LED4_Start

export _LED4_Stop
export  LED4_Stop

export _LED4_On
export  LED4_On

export _LED4_Off
export  LED4_Off

export _LED4_Switch
export  LED4_Switch

export _LED4_Invert
export  LED4_Invert

export _LED4_GetState
export  LED4_GetState


AREA UserModules (ROM, REL)


.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED4_Start(void)
;  FUNCTION NAME: LED4_Stop(void)
;
;  FUNCTION NAME: LED4_Switch(void)
;
;  DESCRIPTION: ( Switch )
;     Turn LED on or off     
;
;  DESCRIPTION: ( Start, Stop )
;     Turn LED off                       
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  ( Switch )
;     A => If 0, turn off LED, if > 0 turn on LED
;
;  ARGUMENTS:  ( Start, Stop )
;      None
;
;  RETURNS:  none
;
;  SIDE EFFECTS:
;    REGISTERS ARE VOLATILE: THE A AND X REGISTERS MAY BE MODIFIED!
;
;-----------------------------------------------------------------------------
_LED4_On:
 LED4_On:
   mov  A,0x01
   jmp  LED4_Switch 

_LED4_Start:
 LED4_Start:
_LED4_Stop:
 LED4_Stop:
_LED4_Off:
 LED4_Off:
   mov  A,0x00

_LED4_Switch:
 LED4_Switch:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   or   A,0x00                                   ; Check mode
   jz   .Turn_Off_LED

.Turn_On_LED:
IF(0)                                            ; Active High Digit Drive
   or   [Port_2_Data_SHADE],LED4_PinMask
ELSE                                             ; Active Low Digit Drive
   and  [Port_2_Data_SHADE],~LED4_PinMask
ENDIF
   jmp  .Switch_LED

.Turn_Off_LED:
IF(0)                      ; Active High Digit Drive
   and  [Port_2_Data_SHADE],~LED4_PinMask
ELSE                              ; Active Low Digit Drive
   or   [Port_2_Data_SHADE],LED4_PinMask
ENDIF

.Switch_LED:
   mov  A,[Port_2_Data_SHADE]
   mov  reg[LED4_PortDR],A

   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION



.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED4_Invert(void)
;
;  DESCRIPTION:
;     Invert state of LED                               
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS:  none
;
;  SIDE EFFECTS:
;    REGISTERS ARE VOLATILE: THE A AND X REGISTERS MAY BE MODIFIED!
;
;-----------------------------------------------------------------------------
_LED4_Invert:
 LED4_Invert:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   xor  [Port_2_Data_SHADE],LED4_PinMask
   mov  A,[Port_2_Data_SHADE]
   mov  reg[LED4_PortDR],A

   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED4_GetState(void)
;
;  DESCRIPTION:
;     Get state of LED
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS:  
;    State of LED   1 = ON,  0 = OFF
;
;  SIDE EFFECTS:
;    REGISTERS ARE VOLATILE: THE A AND X REGISTERS MAY BE MODIFIED!
;
;-----------------------------------------------------------------------------
_LED4_GetState:
 LED4_GetState:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   mov   A,[Port_2_Data_SHADE]         ; Get shade value
IF(0)                                  ; Active High Digit Drive
   // Nothing for now
ELSE                                   ; Active Low Digit Drive
   cpl   A                             ; Invert bit if Active low
ENDIF
   and   A,LED4_PinMask                ; Mask off the trash
   jz    .End_LED_GS                   ; If zero, we're done
   mov   A,0x01                        ; Return a 1 no mater what the mask is.

.End_LED_GS:
   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION
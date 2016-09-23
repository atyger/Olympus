;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME:   LED1.asm
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

include "LED1.inc"
include "memory.inc"

export _LED1_Start
export  LED1_Start

export _LED1_Stop
export  LED1_Stop

export _LED1_On
export  LED1_On

export _LED1_Off
export  LED1_Off

export _LED1_Switch
export  LED1_Switch

export _LED1_Invert
export  LED1_Invert

export _LED1_GetState
export  LED1_GetState


AREA UserModules (ROM, REL)


.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED1_Start(void)
;  FUNCTION NAME: LED1_Stop(void)
;
;  FUNCTION NAME: LED1_Switch(void)
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
_LED1_On:
 LED1_On:
   mov  A,0x01
   jmp  LED1_Switch 

_LED1_Start:
 LED1_Start:
_LED1_Stop:
 LED1_Stop:
_LED1_Off:
 LED1_Off:
   mov  A,0x00

_LED1_Switch:
 LED1_Switch:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   or   A,0x00                                   ; Check mode
   jz   .Turn_Off_LED

.Turn_On_LED:
IF(0)                                            ; Active High Digit Drive
   or   [Port_2_Data_SHADE],LED1_PinMask
ELSE                                             ; Active Low Digit Drive
   and  [Port_2_Data_SHADE],~LED1_PinMask
ENDIF
   jmp  .Switch_LED

.Turn_Off_LED:
IF(0)                      ; Active High Digit Drive
   and  [Port_2_Data_SHADE],~LED1_PinMask
ELSE                              ; Active Low Digit Drive
   or   [Port_2_Data_SHADE],LED1_PinMask
ENDIF

.Switch_LED:
   mov  A,[Port_2_Data_SHADE]
   mov  reg[LED1_PortDR],A

   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION



.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED1_Invert(void)
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
_LED1_Invert:
 LED1_Invert:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   xor  [Port_2_Data_SHADE],LED1_PinMask
   mov  A,[Port_2_Data_SHADE]
   mov  reg[LED1_PortDR],A

   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: LED1_GetState(void)
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
_LED1_GetState:
 LED1_GetState:
   RAM_PROLOGUE RAM_USE_CLASS_4
   RAM_PROLOGUE RAM_USE_CLASS_2
   RAM_SETPAGE_CUR >Port_2_Data_SHADE

   mov   A,[Port_2_Data_SHADE]         ; Get shade value
IF(0)                                  ; Active High Digit Drive
   // Nothing for now
ELSE                                   ; Active Low Digit Drive
   cpl   A                             ; Invert bit if Active low
ENDIF
   and   A,LED1_PinMask                ; Mask off the trash
   jz    .End_LED_GS                   ; If zero, we're done
   mov   A,0x01                        ; Return a 1 no mater what the mask is.

.End_LED_GS:
   RAM_EPILOGUE RAM_USE_CLASS_2
   RAM_EPILOGUE RAM_USE_CLASS_4
   ret
.ENDSECTION

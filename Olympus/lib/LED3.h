//*****************************************************************************
//*****************************************************************************
//  FILENAME: LED3.h
//   Version: 1.40, Updated on 2010/9/19 at 23:40:44                                          
//  Generated by PSoC Designer 5.1.1875.0
//
//  DESCRIPTION: LED3 User Module C Language interface file
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2010. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef LED3_INCLUDE
#define LED3_INCLUDE

#include <m8c.h>


/* Create pragmas to support proper argument and return value passing */
#pragma fastcall16  LED3_Stop
#pragma fastcall16  LED3_Start
#pragma fastcall16  LED3_On
#pragma fastcall16  LED3_Off
#pragma fastcall16  LED3_Switch
#pragma fastcall16  LED3_Invert
#pragma fastcall16  LED3_GetState


//-------------------------------------------------
// Constants for LED3 API's.
//-------------------------------------------------
//
#define  LED3_ON   1
#define  LED3_OFF  0

//-------------------------------------------------
// Prototypes of the LED3 API.
//-------------------------------------------------
extern void  LED3_Start(void);                                     
extern void  LED3_Stop(void);                                      
extern void  LED3_On(void);                                      
extern void  LED3_Off(void);                                      
extern void  LED3_Switch(BYTE bSwitch);
extern void  LED3_Invert(void);                                         
extern BYTE  LED3_GetState(void);                                         

//-------------------------------------------------
// Define global variables.                 
//-------------------------------------------------



#endif

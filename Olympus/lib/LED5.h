//*****************************************************************************
//*****************************************************************************
//  FILENAME: LED5.h
//   Version: 1.40, Updated on 2010/9/19 at 23:40:44                                          
//  Generated by PSoC Designer 5.1.1875.0
//
//  DESCRIPTION: LED5 User Module C Language interface file
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2010. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef LED5_INCLUDE
#define LED5_INCLUDE

#include <m8c.h>


/* Create pragmas to support proper argument and return value passing */
#pragma fastcall16  LED5_Stop
#pragma fastcall16  LED5_Start
#pragma fastcall16  LED5_On
#pragma fastcall16  LED5_Off
#pragma fastcall16  LED5_Switch
#pragma fastcall16  LED5_Invert
#pragma fastcall16  LED5_GetState


//-------------------------------------------------
// Constants for LED5 API's.
//-------------------------------------------------
//
#define  LED5_ON   1
#define  LED5_OFF  0

//-------------------------------------------------
// Prototypes of the LED5 API.
//-------------------------------------------------
extern void  LED5_Start(void);                                     
extern void  LED5_Stop(void);                                      
extern void  LED5_On(void);                                      
extern void  LED5_Off(void);                                      
extern void  LED5_Switch(BYTE bSwitch);
extern void  LED5_Invert(void);                                         
extern BYTE  LED5_GetState(void);                                         

//-------------------------------------------------
// Define global variables.                 
//-------------------------------------------------



#endif

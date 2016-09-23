//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------
	#include <m8c.h>        // part specific constants and macros
	#include <E2PROM.h>
	#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
	#include "PSoCGPIOINT.h"
	#include "Timer8.h"    // timer8 API header file
	#include "Timer8_ISR.h"
	#include "dmk.h"
	#include "nonvolatile.h"
	#include "constants.h"
	#include "incline.h"
	#include "machine_table.h"
	#include "fifo.h"
	#include "myuart.h"
	#include "Main.h"
	

//constants


//global variables
	signed char temp_s_char;
	unsigned char init_complete;
//	unsigned char Port_2_Data_SHADE;

void set_init_complete (unsigned char flag)
{
	init_complete = flag;
}


void main(void)
{
	init_complete = 0;
	
	
	// Insert your main routine code here.
	/////////////////////////////////INITIALIZATION//////////////////////////////////////////////////

	
	Timer8_WritePeriod(24); /* interrupt will occur every 250uS with this set to 24 */
	Timer8_EnableInt();  	/* ensure interrupt is enabled */  
	M8C_EnableGInt;  		/* enable the global interrupts */  
	Timer8_Start();  	    /* start the counter! */  
	
	LED1_Start();					
	LED2_Start();
	LED3_Start();
	LED4_Start();
	LED5_Start();
	LED6_Start();					
	
	E2PROM_Start();		//a null function, maintained for user module API consistency
	
	Port_2_Data_SHADE &= ~INCL_DN_MASK;  //Set low so the incline won't go
	Port_2_Data_SHADE &= ~INCL_UP_MASK;  //Set low so the incline won't go
	PRT2DR = Port_2_Data_SHADE;
	
//	#ifdef DELTA_MOTOR_CONTROLLER
//	set_pwm(10);  	// SET TO 10 ONLY FOR Delta Motor Controller
//	vfd_m_init();	//BJD
//	#endif
	
	myuart_init ();						// Enable the fifo
	UART_IntCntl(UART_ENABLE_RX_INT);   // Enable RX interrupts  
	UART_Start(UART_PARITY_NONE);       // Enable UART  
	M8C_EnableGInt ;                    // Turn on interrupts  
	
	M8C_EnableWatchDog;
	M8C_ClearWDT;
	
	
	//nv_test();		 //ATT test stuff
		
	//#if( MACHINE_TYPE == TREADMILL || MACHINE_TYPE == HIKER )
	//#endif	// ( MACHINE_TYPE == TREADMILL || MACHINE_TYPE == HIKER )
	//041	load_machine_table_defaults();
	//load_nv_defaults();
	//041 	read_nonvolatile(E2PROM_NOERROR);
	
	
	
	//set_update_nv(1);   //TAKE THIS OUT LATER
	//017	nv_ram.current_incline = 0xFE;		//Take this out later --THIS FORCES TO CALIBRATE EVERY TIME
	//017   nv_ram.trans_max = 0;			//Take this out later --THIS FORCES TO CALIBRATE EVERY TIME
	
	//017while(!get_dmk())
	//017{
	//017M8C_ClearWDT;
	//017}
	//LED6_Switch(1);
	//017calibrate_incline();
	//LED6_Switch(0);
	
//	myuart_init ();						// Enable the fifo
//	UART_IntCntl(UART_ENABLE_RX_INT);   // Enable RX interrupts  
//	UART_Start(UART_PARITY_NONE);       // Enable UART  
//	M8C_EnableGInt ;                    // Turn on interrupts  
	
	
	//set DE high
	//set re high
	
//019	PRT1DR &= ~RS485_DE_MASK;			//SET LOW
//019	PRT1DR &= ~RS485_RE_MASK;			//SET LOW
	
	//PRT1DR = Port_1_Data_SHADE;
	
	//When you first power up you need to get the max_incline, min_incline, transmax and then set the incline to 0
	//if you already have received it just go to 0
	
	//initialization--Don't allow anything else until it reaches 0
//019	set_desired_incline(0);				//bottom seek when you power up
	//----------set default incline values
	load_incline_defaults ();
	
	
//031	while (!init_complete)
//031	{	M8C_ClearWDT; 
//031		check_uart();
//031		LED6_Invert();
//031	}	// Stay in this loop until initial communication with BB is complete
//031	LED6_Switch(0);
	system_idle();		//This is the main loop.  It will sit here for the rest of the time
	//test_idle();
	
	
}


/********************************REVISION NOTES*********************************
	001		This version allows a switch to be hooked to INS and TACH.
			It will run the incline up and down.	1-11-2010
	
	002		Set up uart timing. Added a whole bunch of Brads renesas files. 
	003		Kept uart timing, but went back to add files a little at a time.
			Working on non-volitle, machine_table, constants
			Got nv_test to work and light led showing success.
	004		Can read and write from nonvolitle.  Verified timing of interrupt.
	005		There was a delay in my output running incline up and down.  Needed to add the line
			PRT2DR = Port_2_Data_SHADE;			//Added in version 005
			This fixed it.
			Incline calibrate seems to be working.
			Looks like flash is working.  May need to add more blocks to increase writes.
	006
	007
	008		The enable line stays high only while you transmit now.
			Right after a transmit the RX interrupt fires a couple times.  Not sure why.
			test_uart works to send back what you receive.  Since it picks up a couple of things after a transmit. there are a couple of bytes on the front.
			in check uart I can get 0x07 0x06 0x0A 0x0B 0x00 and then it looks like i am changing the value
			but for some reason the incline only moves once.  It should change every 7 seconds or so.
	009
	010		Works for incline commands sent without every 7 seconds.
			Doesn't seem to work when the delta commands are comming every 50mS, and incline is comming often.
	011		Had to cut out as much ram use as possible so it wouldn't wig, but it seems to be receiving commands to move the incline now.
	012		Changes to the Uart ignores everything until it gets an 'I' then it will load 8 bytes in the receive interrupt.
	013		Incline was moving backward so I swapped the up and down.
	014		This version was stable and was sent to the show.
	015		Added code to deal with 14bit integer communication.  Took out calibrating every time.
	016 	Added back in all the dmk stuff.  Calibrating incline can't happen until you see that the dmk is in.  But you need to have like 100 counts before it is valid.
	017
	018		Fixed the set_trans_max.  it was loading backwards.
	019		Working version
	020		Added in something to send back an error if the transmax or the max incline is bad.
	021		Added in reading from the nonvolitile.  
	022		incline_stop should be set each time the key transitions.  So it should stop when you put the key in or when you remove the key.
	023		Added set_init_complete.  init_complete = 0
			Added a loop before the system idle call to check_uart and refresh watchdog.
			init complete gets set to 1 after the power board receives a transmax.
			Added ability to write a current incline.
			commented out some stuff in load_nv_defaults.
	024		Added lines to incline.c to allow use of non linear table for incline.
	027		Added in ability to disable dmk.
	029		SWITCH_DIR is the number of 1/10th seconds it will wait before changing directions.
			Haven't changed anything from 028 yet.
	030		Updated to new computer and had errors when trying to compile Port_2_Data_SHADE  added my own Port_2_Data_SHADE 
	031		Changed to allow clearing of calibration error.
	045		The Fifo did not seem to be working as expected.  Changed the fifo routines.
			Also changed dmk and ins to use the DMK_Data_ADDR and INS_Data_ADDR.
			Added additional check_uart calls in Timer8_ISR.c
			Changed LED4 to mimic the actual DMK pin.  Before it would show the key always in if the dmk is disabled.
				LED1	INS
				LED2	INVERTS EVERY TIME SOMETHING IS PUSHED ONTO THE STACK
				LED3	ON= DMK DISABLED OFF= DMK ENABLED
				LED4	ON= DMK IS OUT	 OFF= DMK IS IN
				LED5 	INVERTS EVERY TIME A Ir or Iw is received with a valid crc.
				LED6	TACH

				
*/	
			

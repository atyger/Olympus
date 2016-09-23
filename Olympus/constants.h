/*************************************************
 *  constants.h
 *  
 *************************************************/

#ifndef CONSTANTS
#define CONSTANTS



//#include "bradg3_bsp.h"
//#include "macros.h"
//#include "machine_table.h"

//timer information
//#define  TIMER_CYCLES_1MS 4
//#define  MS_IN_100MS 100
//#define  TENTHS_IN_1S 10


//console information
//#define	MODEL		"OLYMPUS"
//#define PARTNUMBER	290887
//#define DEFAULT_MACHINE_TYPE 	0x03;

//#define SERIAL_FLASH_VERSION 1	//current version of serial flash 


//#define UART0	0
//#define UART1	1
//#define UART2	2
//#define UART3	3
//#define UART4	4
//#define UART5	5





//#define THREE_SECONDS 		3000 //milliseconds
//#define DEFAULT_LANGUAGE 	ENGLISH

/*******************************************/
//general
//#define TRUE	1
#define YES		1

//#define FALSE	0
#define NO		0
//files

/*************************************************
 *  machine_table
 *  
 *************************************************/
//general
#define		DISABLED	0
#define		ENABLED		1

#define		OFF			0
#define		ON			1



//backlight
//#define		AUTO		2

//languages
//#define		LANG_ENGLISH	0
//#define		LANG_SPANISH	1
//#define		LANG_FRENCH		2

//units
//#define		UNITS_ENGLISH	0
//#define		UNITS_METRIC	1

#define		NOT_USED	-2	//for settings not implemented in the application


//rollers
//#define ROLLER_CONST_16			11817 //speed constant for 1.6 inch roller
//#define ROLLER_CONST_19			13780 //speed constant for 1.9 inch roller
//#define ROLLER_CONST_25			18064 //speed constant for 2.5 inch roller
//#define ROLLER_CONST_17			12368 //speed constant for 1.73 inch roller (44mm brazil roller)
//product
//#define		BIKE		1
//#define		ELLIPTICAL	2
#define		TREADMILL	3
#define		HIKER		4
//#define		SYSTEM		5
//#define		VIBRATION	6


//#define CONFIG_UNITS			UNITS_ENGLISH	// UNITS_ENGLISH,UNITS_METRIC
//#define CONFIG_MIN_SPEED		5				// set to english or metric: units flag determines interpretation
//#define CONFIG_MAX_SPEED		120				// set to english or metric: units flag determines interpretation
//#define CONFIG_INTERCEPT		(15 * 2)		// speed intercept
//#define CONFIG_TRANS_MAX		100				// -2 means not calibrated
//#define CONFIG_MIN_INCLINE		(0 * 2)			// incline value * 2 for half step resolution
//#define CONFIG_MAX_INCLINE		(52 * 2 - CONFIG_MIN_INCLINE)	// incline value * 2 for half step resolution
//#define CONFIG_ACTUAL_INCLINE	(52 * 2 - CONFIG_MIN_INCLINE)	// incline value * 2 for half step resolution 
//#define CONFIG_ROLLER_SIZE		ROLLER_CONST_19 // ROLLER_CONST_16, ROLLER_CONST_17, ROLLER_CONST_19, ROLLER_CONST_25 (special values are hard coded by production
///#define	CONFIG_DEFAULT_VOLUME	NOT_USED		// use 1-100
//#define RESERVED_WORD_1			NOT_USED		// future use
//#define RESERVED_WORD_2			NOT_USED		// future use
//#define RESERVED_WORD_3			NOT_USED		// future use
//#define RESERVED_BYTE_1			NOT_USED		// future use
//#define RESERVED_BYTE_2			NOT_USED		// future use
//#define RESERVED_BYTE_3			NOT_USED		// future use
//#define CONFIG_CURRENT_LIMIT	NOT_USED		// 
//#define CONFIG_IR_COMP			NOT_USED		// 
//#define CONFIG_CONTROLLER_MAX_SPEED	NOT_USED	// future use
//#define CONFIG_FOLDBACK				NOT_USED	// future use
//#define CONFIG_CONTROLLER_PRESENT	NO			// YES, NO
//#define CONFIG_MAX_RESISTANCE	NOT_USED	// future use
//#define CONFIG_MIN_RESISTANCE	NOT_USED	// future use
//#define CONFIG_MAX_STRIDE_LENGTH	NOT_USED			// YES, NO
//#define FLASH_CHECKSUM			07F10h			// 2 byte console checksum

#define MACHINE_TYPE			HIKER		// BIKE, ELLIPTICAL, TREADMILL, HIKER, SYSTEM,VIBRATION

#if (MACHINE_TYPE == HIKER)
	#define DEFAULT_TRANS_MAX		330
	#define DEFAULT_MIN_INCLINE		0
	#define	DEFAULT_MAX_INCLINE		66
	#define	DEFAULT_CURRENT_INCLINE	66
	#define	DEFAULT_ACTUAL_INCLINE	66
	#define VERSION 	45    	//current version of software
#elif (MACHINE_TYPE == TREADMILL)
	#define DEFAULT_TRANS_MAX		210
	#define DEFAULT_MIN_INCLINE		0
	#define	DEFAULT_MAX_INCLINE		36
	#define	DEFAULT_CURRENT_INCLINE	36
	#define	DEFAULT_ACTUAL_INCLINE	36
	#define VERSION 	450    	//current version of software
#else
	#define DEFAULT_TRANS_MAX		330
	#define DEFAULT_MIN_INCLINE		0
	#define	DEFAULT_MAX_INCLINE		66
	#define	DEFAULT_CURRENT_INCLINE	66
	#define	DEFAULT_ACTUAL_INCLINE	66
	#define VERSION 	4500    	//current version of software
#endif
/*************************************************
 *  adc
 *  
 *************************************************/
//#define WIPER			ad3 // Wiper reading in resistence motor

/*************************************************
 *  display
 *************************************************/
/*enum screen_table {
	SCREEN_TIME_DIST,			//0
	SCREEN_VERT_CAL,			//1
	SCREEN_TIME_DIST_VERT_CAL,	//2
	SCREEN_ALL_VALUES,			//3
	SCREEN_PROG_AUTO,			//4
	SCREEN_PROG_VALUES,			//5
	SCREEN_PROG_2ND_VALUES,		//6
	SCREEN_PROG_3RD_VALUES,		//7
	SCREEN_CANNED_PROG_INTRO,	//8
	SCREEN_CANNED_PROG_FULL,	//9
	SCREEN_CANNED_2,			//10
	SCREEN_CANNED_3,			//11
	GENERATING_RANDOM,			//12
};

enum zoom_level {
	PROG_FULL_VIEW,
	PROG_PATH_VIEW,
	PROG_PROFILE_VIEW,
	PROG_TRAIL_VIEW,
};

enum auto_screen {
	AUTO_DIST,
	AUTO_VERT,
	AUTO_CAL_HR,
	AUTO_CAL_TOT,
	AUTO_PULSE,
	AUTO_SPEED,
	AUTO_INCLINE,
}; */

/*************************************************
 *  fan
 *  
 *************************************************/
//#define	OPTION_FAN
//#define	FAN_PWM_CHANNEL		3				// The fan is on PWM 3

//#define  PWM8_CONFIG 0b10100111  /* 10100111 value to load into timer A3 mode register
//	                   ||||||||_  TMOD0,TMOD1: PWM MODE SELECTED
//                       ||||||____ MR0:         = 1 FOR PWM MODE 
//                       |||||_____ MR1,MR2:     EXTERNAL TRIGGER NOT SELECTED
//                       |||_______ MR3:         SET TO 1 FOR 8 BIT PWM      
//                       ||________ TCK0,TCK1:   f32 SELECTED */


//#define HARDWARE_HAND_DET
//#define HRDWR_HAND_DET_LEVEL 10

/*************************************************
 *  incline
 *  
 *************************************************/
#define DELTA_MOTOR_CONTROLLER
#define MAX_IPWD 		1500
#define MIN_IPWD 		25
#define SWITCH_DIR 		6
#define DATA_OK 		0x3A
#define GOING_UP 		1
#define GOING_DOWN 		2
#define NO_ERROR 		0
#define DMK_PULLED 		1
#define ZERO_TRANS_MAX 	2
#define TENTHS_LOW 		3
#define	TRANS_MAX_BAD	0xFFFE
#define MAX_OSCILLATIONS	2

/*************************************************
 *  keyboard
 *  
 *************************************************/

/*************************************************
 *  nonvolatile
 *  
 *************************************************/
//#define MACHINE_DATA_SIZE 0x40	//number of bytes
#define ENGLISH 0
//#define METRIC 1
//#define DEFAULT_UNITS ENGLISH
//#define DEFAULT_UC1610_CONTRAST 	200//53
//#define DEFAULT_UC1610_BIAS_RATIO	03

/*  PSOC CY8C27443 information    */
//Total size is 16K.  So ending address is 3E80
//#define BLOCK_A     ((unsigned int *)0x2000)       /*  BLOCK A first address   */
//#define BLOCK_A_END ((unsigned int *)0x2800)       /*  BLOCK A last address    */
//#define BLOCK_B     ((unsigned int *)0x2801)       /*  BLOCK B first address   */
//#define BLOCK_B_END ((unsigned int *)0x2FFF)       /*  BLOCK B last address    */

//#define BLOCK_A_SELECT      0           /*  Use BLOCK A */
//#define BLOCK_B_SELECT      1           /*  Use BLOCK B */

//#define COMPLETE            0x00        /*  FLASH write/erase complete  */
//#define PROGRAM_ERR         0x01        /*  FLASH data write error  */
//#define ERASE_ERR           0x02        /*  FLASH erase error   */

//#define CUSTOM_COLUMNS 3
//#define CUSTOM_SEGMENTS 40
//#define NUMBER_OF_CUSTOM_PROGRAMS 4

//#define MIN_MACHINE_SPEED_KPH 10
//#define MIN_MACHINE_SPEED_MPH 5

//#define CHECKSUM_CONFIG LOBYTE(MAX_SPEED_CONFIG)

//#define USER_AGE NO
//#define USER_WEIGHT NO

/*************************************************
 *  print
 *  
 *************************************************/
/*#define HW_OFFSET 1 //empty bits at the bottom (change to 0 if missing line at top)
#define LCD_DISPLAY_HEIGHT			160
#define LCD_DISPLAY_BIT_WIDTH		100
#define LCD_DISPLAY_BYTE_WIDTH		(((LCD_DISPLAY_BIT_WIDTH - 1) / 4 ) + 1)
#define TEN_MIN 10*60

//print_control parameters
#define PC_TENTHS 			0x0010		//print control 10ths
#define PC_HUNDREDTHS 		0x0020		//print control 100ths
#define PC_THOUSANDTHS 		0x0040		//print control 1000ths
#define PC_HEX 				0x0080		//print control hex (set for hex, clear for decimal)
#define PC_TIME 			0x0100		//print control time
#define PC_NO_COMMAS		0x0200		//
#define PC_WRITE_XOR		0x0400		// write by XOR rather than OR
//#define PC_NEGATIVE			0x0400		// negative value
#define	PC_PAD_ZEROES		0x0800		// pad with zeroes
#define PC_ALIGN_VCENTER	0x1000		// vertical centering
#define PC_ALIGN_CENTER		0x2000	 	// horizontal centering
#define PC_ALIGN_RIGHT	 	0x4000		// right justification
#define PC_ALIGN_LEFT		0x8000

#define	TIME_DISP				5
#define	TIME_DISP_SIZE			4
#define	VERT_FT_DISP			0
#define	VERT_FT_DISP_SIZE		4
#define	DIST_DISP				0
#define	DIST_DISP_SIZE			4
#define	CALS_DISP				0
#define	CALS_DISP_SIZE			4
#define CALS_HOUR_DISP			5
#define CALS_HOUR_DISP_SIZE		4
#define	PULSE_DISP				0
#define	PULSE_DISP_SIZE			3
#define	INCLINE_DISP			0
#define	INCLINE_DISP_SIZE		3
#define	SPEED_DISP				3
#define	SPEED_DISP_SIZE			3
#define	SEG_TIME_DISP			0
#define	SEG_TIME_DISP_SIZE		4
#define	SMALL_DISP				0
#define	SMALL_DISP_SIZE			2
#define	VFT_MIN_DISP			0
#define	VFT_MIN_DISP_SIZE		4

#define MATRIX_DISP						2 //7
#define MATRIX_DISPLAY_WIDTH			20
#define MATRIX_DISPLAY_BIT_HEIGHT		7
#define MATRIX_DISPLAY_BYTE_HEIGHT		(((MATRIX_DISPLAY_BIT_HEIGHT - 1) / 8 ) + 1)

#define ZONELEDS	16
*/
/*************************************************
 *  serial_flash
 *  
 *************************************************/
/*#define SLAVE_DEVICE 1
#define UART_TIMEOUT_COUNT 60000
#define SF_PORT SIO4
#define SF_BAUD_RATE 500000
*/
/*
//--------------------------------------------------
//  timer
//  
//--------------------------------------------------
//beep constants
#define NO_BEEP					0x00
#define SINGLE_BEEP				0x80
#define DOUBLE_BEEP				0xA0
#define ERROR_BEEP				0xAA
#define PROGRAM_DONE_BEEP		0xD0
#define STOP_BEEP				0xD5
#define START_BEEP				0xE0
#define BLEEP					0x11
#define CAL_PWM_LOW_BEEP		0xC0
#define CAL_PWM_HIGH_BEEP		0xAA


//MAIN_TIMER definitions
#define MAIN_TIMER 				TIMER0
#define MAIN_TIMER_H 			T0H
#define MAIN_TIMER_L 			T0L
#define MAIN_TIMER_RH 			T0RH
#define MAIN_TIMER_RL 			T0RL
#define MAIN_TIMER_CTL 			T0CTL
#define MAIN_TIMER_PRIORITY_HI	T0ENH
#define MAIN_TIMER_PRIORITY_LO	T0ENL
#define MAIN_TIMER_COUNT			( CLOCK_FREQ / ( 4 * 8000) )		//current count for 8kHz (125us) main clock tick
#define MAIN_TIMER_RELOAD_COUNT		( CLOCK_FREQ / ( 4 * 8000) )		//reload value (8000 divide by 4 events)
//IRQxENH[x] IRQxENL[x] Priority Description
//0 0 Disabled Disabled
//0 1 Level 1 Low
//1 0 Level 2 Nominal
//1 1 Level 3 High
#define MAIN_TIMER_IRQ_PRIORITY_H 1
#define MAIN_TIMER_IRQ_PRIORITY_L 0
//VOLUME_TIMER definitions
#define VOLUME_TIMER_H 		T1H
#define VOLUME_TIMER_L 		T1L
#define VOLUME_TIMER_RH 	T1RH
#define VOLUME_TIMER_RL 	T1RL
#define VOLUME_TIMER_PWMH 	T1PWMH
#define VOLUME_TIMER_PWML 	T1PWML
#define VOLUME_TIMER_CTL 	T1CTL
#define VOLUME_TIMER_COUNT	250
#define VOLUME_TIMER_RELOAD_COUNT		250
//AUDIO_TIMER definitions
#define AUDIO_TIMER_H 		T3H
#define AUDIO_TIMER_L 		T3L
#define AUDIO_TIMER_RH 		T3RH
#define AUDIO_TIMER_RL 		T3RL
#define AUDIO_TIMER_PWMH 	T3PWMH
#define AUDIO_TIMER_PWML 	T3PWML
#define AUDIO_TIMER_CTL 	T3CTL
#define AUDIO_TIMER_COUNT	250
#define AUDIO_TIMER_RELOAD_COUNT		250



//------------------------------------------------
//	general	constants
//  
//------------------------------------------------
#define PULSE_IGNORE_TIMEOUT 1*10 //ten tenth seconds

//modes
#define NORMAL_MODE	1
#define SCREEN1	2
#define SCREEN2	3
#define SCREEN3	4
#define SCREEN4	5
#define SCREEN5	6
#define COMM_MODE 7

//#define MEMBRANE_TEST 8
#define USER_MODE 9
#define HELD_MODE 10		//keyboard mode for held keys
#define LOST_MODE 			14		//ifit communication has been lost
#define CUSTOM1 24
#define CUSTOM2 25
#define CUSTOM3 26
#define FITNESS_TEST_MODE 27
#define ADJUSTABLE_WORKOUT_MODE 28
#define CHECKSUM_FAILED_MODE 29

#define	CLOCK_FREQ				24000000L		// 24MHz 
*/
#define NULL 0
#endif

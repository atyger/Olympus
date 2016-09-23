/*************************************************
 *  nonvolatile.h
 *
 *************************************************/

#ifndef NONVOLATILE
#define NONVOLATILE

#include "constants.h"

// Possible result codes
//ATT #define ERR_SUCCESS		0		/*  FLASH write/erase complete  */
 //ATT#define	ERR_UPDATE		-3		/*	update_nv was not set, so nothing was done */
//ATT #define ERR_PROGRAM		-2		/*  FLASH data write error  */
//ATT #define ERR_ERASE		-3 	    /*  FLASH erase error   */

// Possible result codes
#define NOERROR  		0		/*Successful completion of operation.  0  */
#define	FAILURE  		-1		/*Unsuccessful completion of operation. Most likely a result of Flash-protection bit errors.  -1  */
#define STACKOVERFLOW   -2		/*Stack space was not sufficient for algorithm requirements.  -2  */
#define	ERR_UPDATE		-3		/*	update_nv was not set, so nothing was done */

// For nv_test only
#define nvtestVERSION  	0x1111
#define nvtestTRANSMAX  0x2222
#define nvtestMOVEFLAG  0x33
#define nvtestCURINCL  	0x44
#define nvtestINTERCEPT 0x55
#define nvtestMININCL  	0x66
#define nvtestMAXINCL  	0x77
#define nvtestINCLCNFG  0x88
#define nvtestCHKSUM  	0x9999
// For nv_test only

void set_update_nv( unsigned char );
void load_nv_defaults( void );
void set_update_custom(unsigned char);
void read_nonvolatile( signed char );
void read_custom( void );

signed char update_nonvolatile( void ); // writes nv_ram to flash, returns result

unsigned char get_intercept_config(void);
unsigned char calculate_slope(unsigned char);
unsigned char get_max_speed_config(void);
unsigned char get_update_nv( void );
void set_nv_bError( signed char );
signed char get_nv_bError( void );
void nv_test (void);


//Make sure when you program the cypress that the protection level allows for internal writes.
/*		Level 		Protection Description
		0 			Unprotected All reads enabled
					All writes enabled
		1 			Factory Upgrade External reads disabled
					All writes enabled
		2 			Field Upgrade External reads disabled
					External writes disabled
					Internal writes enabled
		3 			Full Protection External reads disabled
					All writes disabled		
					
The 16-kilobyte families have 256 blocks numbered 0 through 255.

Writing to flash requires that an entire 64-byte (or
128-byte) block be written, even if only 1 byte is to be
modified. Any single block write uses an “endurance
cycle.” Each specific PSoC device has a maximum
number of allowed endurance cycles to stay within the
electrical specifications of the data sheet.

The FlashENPB value for a PSoC specifies how many endurance cycles
each flash block can endure. The FlashENT value for a
PSoC specifies the total number of endurance cycles the
entire flash array can endure. See specific PSoC device
data sheets for these flash endurance specifications.

For CY8C27443
FlashENPB Flash Endurance (per block) 50,000[11] – – Cycles Erase/write cycles per block.
FlashENT Flash Endurance (total)[12] 1,800,000 – – Cycles Erase/write cycles.

11. The 50,000 cycle flash endurance per block will only be guaranteed if the flash is operating within one voltage range. Voltage ranges are 3.0V to 3.6V and 4.75V
to 5.25V.
12. A maximum of 36 x 50,000 block endurance cycles is allowed. This may be balanced between operations on 36x1 blocks of 50,000 maximum cycles each, 36x2
blocks of 25,000 maximum cycles each, or 36x4 blocks of 12,500 maximum cycles each (to limit the total number of cycles to 36x50,000 and that no single block
ever sees more than 50,000 cycles).
For the full industrial range, the user must employ a temperature sensor user module (FlashTemp) and feed the result to the temperature argument before writing.
Refer to the Flash APIs Application Note AN2015 at http://www.cypress.com under Application Notes for more information
This is because flash reads are not
restricted to reading one flash block at a time. In fact, the
flash read is done on a byte-by-byte basis.

Calling bFlashWriteBlock() globally disables
interrupts while erasing and writing the specified block.
Calling FlashReadBlock() does not disable interrupts.
*/
					
typedef	struct NV	{
	/* Don't change the order or location of page_free and checksum
		The order is to avoid problems if there is a power loss while
		the flash is being written, causing the checksum to be invalid.
	*/
//	unsigned char	page_free;					// 0xFF == page free, otherwise used
	/* If there's any messing around to be done, do it below here, and above the checksum */
	unsigned int	version;

// incline calibration data
	unsigned int 	trans_max; 				//max transistion count value
	unsigned char 	move_flag; 				//flag = %3a if incline data is ok
	unsigned char 	current_incline; 		//current incline value
//	unsigned char 	intercept_config;		//where is this used
	unsigned char 	min_incline_config;
	unsigned char 	max_incline_config;
	unsigned char 	actual_incline_config;
	unsigned char   load_machine_table_defaults_count;	//020
	unsigned char	load_nv_defaults_count;
	unsigned char	dmk_disabled;			//1=dmk disabled, 0=dmk enabled
	
	unsigned char   calib_error_code;
	
	unsigned char   f1;
	unsigned char   f2;
	unsigned int    f3;
	unsigned char   f4;
	unsigned char   f5;
	unsigned int   f6;
	unsigned char   f7;
	unsigned char   f8;
	unsigned char   f9;
	unsigned char   f10;
	unsigned char   f11;
	unsigned char   f12;

	
//ATT	unsigned char   f1;
//ATT	unsigned char   f2;
//ATT	
//ATT	int	trans_count;		//added by att
//ATT	int	trans_des;			//added by att
//ATT	unsigned char desired_incline;	//added by att
//ATT	unsigned char my_current_incline;
//ATT	unsigned int  my_crc;
//ATT	unsigned char   f3;
//ATT	unsigned char   f4;
	

// Info about the treadmill
//	unsigned long 	tottime;
//	unsigned long 	totdist;
//	unsigned long 	totvert_ft;
//	unsigned char	last_program;
//	unsigned char 	metric;
//	unsigned char 	demo_enabled;
//	unsigned char 	max_target_pulse;
// unsigned char 	backlight_state;
//ATT	unsigned char 	tv_volume;			//this needs to moved into saved data sometime
//	unsigned char 	tv_channel;				//this needs to moved into saved data sometime
//	enum tv_modes 	tv_input;				//this needs to moved into saved data sometime
//	unsigned char 	tv_source; //this needs to moved into saved data sometime
//	unsigned char 	tv_channel_list[16];
	
//ATT	unsigned char 	contrast_level;
	
//ATT	unsigned char	fan_low_level;
//ATT	unsigned char	fan_high_level;	

//	unsigned char tv_input;

//Configuration information

	/* Make sure the checksum is last in memory */
	unsigned short	checksum;					//Checksum of this structure
	
}NV_BLOCK;


// Record size in bytes, rounded up to whole word boundary
#define RECORD_SIZE		(2*((sizeof(NV_BLOCK)+1)/2))

//012 typedef	struct CUST_PROGRAM	
//012{
//012	unsigned char	program_type;
//012	unsigned char	nsegs;
//012	unsigned char	ncolumns;
//012	unsigned char	custom_program_buffer[CUSTOM_COLUMNS*CUSTOM_SEGMENTS];	// copy this to cleared out program_buffer
//012}CUSTOM_PROGRAM;

//012typedef	struct CUSTOM_STRUCT	
//012{
//012	unsigned char	page_free;				// 0xFF == page free, otherwise used
//012	CUSTOM_PROGRAM	custom_program[NUMBER_OF_CUSTOM_PROGRAMS];
//012	unsigned int	checksum;				// Checksum of this structure
//012}CUSTOM_BLOCK;

#define CUSTOM_BLOCK_SIZE    (2*((sizeof(CUSTOM_BLOCK)+1)/2))         /*  Record size in words */

// non-volatile storage is only in block B, at 0x0F000-0x0FFFF
//#define BLOCK_SIZE		0x1000
#define BLOCK_SIZE		0x200		//ATT have to write a 64byte block ,  64*8=512 or 200h

#define NV_START_ADDR	(0x0E000)		//ATT need to figure out where this should be.
#define NV_END_ADDR		( NV_START_ADDR + \
						  ((BLOCK_SIZE/RECORD_SIZE)-1) * RECORD_SIZE )

#define CUSTOM_START_ADDR	(0x0F000)	//ATT need to figure out where this should be.
#define CUSTOM_END_ADDR		( CUSTOM_START_ADDR + \
						  ((BLOCK_SIZE/CUSTOM_BLOCK_SIZE)-1) * CUSTOM_BLOCK_SIZE )
						  
extern NV_BLOCK	nv_ram;		// This is the ram copy
//extern CUSTOM_BLOCK	custom_ram;		// This is the ram copy

#endif

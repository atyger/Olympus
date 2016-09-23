/*************************************************
 *  nonvolatile.c
 *  
 *************************************************/

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "PSoCGPIOINT.h"
#include <E2PROM.h>  
#include "constants.h"
//#include "crc16.h"
//#include "fan.h"
#include "incline.h"
#include "machine_table.h"
//#include "motor.h"
#include "nonvolatile.h"
//#include "pulse.h"
//#include "LED2.h"
#include "Main.h"


//static unsigned char init_flag;
static unsigned char update_nv;			// This is the flag to set if we need to update
signed char nv_bError;			// This is the error flags when writing to non volatile
//static unsigned char update_custom;	// This is the flag to set if we need to update
//012  unsigned char erasing_nv;

NV_BLOCK nv_ram;						// This is the ram copy used for everything else
NV_BLOCK * ptr_nv_ram = &nv_ram;
//BYTE byte_nv_ram[sizeof(NV_BLOCK)];	//This is the ram copy used for non volitile

//POINT  *byte_ptr_nv_ram to point at beginning address of nv_ram


/************************************************************************************
Name:        update_nonvolatile
Parameters:  None
Returns:     Result code
Description: User main
************************************************************************************/
signed char update_nonvolatile( void )
{
	signed char bError;  //ATT
	//BYTE i;
	//BYTE test_array[] = {1,2,3,4,5,6,7,8,9,10,11};
	

	if ( !update_nv ) //&& ( !update_custom))
		return ERR_UPDATE;
	if (update_nv)
	{
		update_nv = 0;
		
		//put code here to load the nv_ram into byte_nv_ram
		//	Calculate the checksum.
		//	NOTE: We don't include page_free or checksum itself
		//
		
		//???add back in later		nv_ram.checksum = crc16( (unsigned long)(&nv_ram) + 1,
		//???add back in later								 (unsigned long)(&nv_ram) + sizeof(NV_BLOCK) - sizeof(unsigned short) - 1 );
       	
		bError = E2PROM_bE2Write(0,(unsigned char *) ptr_nv_ram, 64, 25);   //write 64byte block to save ram needed
	}
	return bError;
	// Possible result codes
	//	NOERROR  		0		/*Successful completion of operation.  0  */
	//	FAILURE  		-1		/*Unsuccessful completion of operation. Most likely a result of Flash-protection bit errors.  -1  */
	//	STACKOVERFLOW   -2		/*Stack space was not sufficient for algorithm requirements.  -2  */
	//	ERR_UPDATE		-3		/*	update_nv was not set, so nothing was done */
	
}


/************************************************************************************
 *  read_nonvolatile
 *  read flash into ram
 ************************************************************************************/
void read_nonvolatile( signed char bError )
{
	//BYTE	i=0;
	
/*REMOVED 	if ( bError == E2PROM_NOERROR )  
    {  
		E2PROM_E2Read( 0, (unsigned char *) ptr_nv_ram, sizeof(NV_BLOCK) );  //Read nv_ram
    }  
	else
	{
		load_nv_defaults();
	}
	if (nv_ram.version != VERSION)	//This is my check to see if the stuff in memory is valid.
	{
		load_nv_defaults();
	}
*/	//Removed in version 25, Trans max should always be -2 on power up.
	load_nv_defaults();
	// Calculate what the checksum should be
//???add back in later		chksum = crc16( (unsigned long)(byte) + 1,
//???add back in later						(unsigned long)(byte) + sizeof(NV_BLOCK) - sizeof(unsigned short) - 1 );
//???add back in late	if ( ( nv_ram.checksum != chksum ) || (nv_ram.version != VERSION) )
//???add back in late	// The checksum doesn't match
//???add back in late	{
//???add back in late		load_nv_defaults();
//???add back in late	}

}
		
/************************************************************************************
 *  load_nv_defaults
 *    Initialize the nv_ram area to known values.
 *    The checksum and the page_free flag will be
 *  calculated when it is written to flash.
 ************************************************************************************/
void load_nv_defaults( void )
{
	//unsigned char x,y;
	//BYTE i=0;
	//020 read_nonvolatile();
	
		nv_ram.version = VERSION;			// load nv_ram with current version, so it can clear nonvolatile when a new version is detected
//041		nv_ram.trans_max = 0xFFFE;
//041		nv_ram.current_incline = 0xFE;
		
//23		nv_ram.trans_max = machine_table->trans_max; 	//max transistion count value
//23		if (nv_ram.trans_max == 0x0FFFE)
//23			nv_ram.move_flag = 0; 			//flag = 0 if incline data is bad
//23		else
//23			nv_ram.move_flag = DATA_OK; 	//flag = DATA_OK if incline data is ok
//23		nv_ram.current_incline = 0xFE; 		//current incline value
		
		//ATT nv_ram.intercept_config = machine_table->intercept;
		//ATT nv_ram.min_incline_config = machine_table->min_incline;
//23		nv_ram.max_incline_config = machine_table->max_incline;
	//	nv_ram.actual_incline_config = machine_table->min_incline;
//23		nv_ram.actual_incline_config = 0;		//min incline ATT
	
//23		if ((nv_ram.move_flag != 0x3a) || (nv_ram.current_incline == 0xFE))
//23		{
//23			nv_ram.current_incline = machine_table->current_incline;
//23			set_desired_incline(machine_table->current_incline);
//23		}
		nv_ram.checksum = 0;				// Checksum of this structure	
		nv_ram.load_nv_defaults_count++;
		set_update_nv(1);
	
}

/************************************************************************************
 *  set_update_nv
 *  
 ************************************************************************************/
void set_update_nv( unsigned char uf )
{
	update_nv = uf;
}


/************************************************************************************
 *  get_update_nv
 *  
 ************************************************************************************/
unsigned char get_update_nv( void )
{
	return update_nv;
}

/************************************************************************************
 *  set_nv_bError
 *  
 ************************************************************************************/
void set_nv_bError( signed char uf )
{
	nv_bError = uf;
}


/************************************************************************************
 *  get_nv_bError
 *  
 ************************************************************************************/
signed char get_nv_bError( void )
{
	return nv_bError;
}
/************************************************************************************
 *  nv_test
 *  
 ************************************************************************************/
void nv_test (void)
 {
// 	
// 	
//	BYTE bError;
//	
///*	unsigned char test_array[] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA};
//	unsigned char test_array_buffer[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//	
//	BYTE i;
//	M8C_ClearWDT;
//	
//	bError = E2PROM_bE2Write(0, test_array, 64, 25); //for pointing at array of unsigned char
//	
//	for (i=0; i<(sizeof(test_array)); i++)
//	{
//		test_array_buffer[i]=0;   //set the array to 0
//	}
//	// bError = E2PROM_NOERROR;
//	if (bError == E2PROM_FAILURE)
//	{
//	}
//	if (bError == E2PROM_STACKOVERFLOW)
//	{
//	}
//	if ( bError == E2PROM_NOERROR )  
//    {  
//        
//        // Read the Serial Number back into a RAM buffer   
//       //E2PROM_E2Read( 0, test_ptr, sizeof(NV_BLOCK) );  //Read nv_ram
//	   E2PROM_E2Read( 0, test_array_buffer, 10 );  //Read nv_ram
//	   
//	   
//	 Switch(1);  //Let you know that bError was E2PROM_NOERROR
//    }  
//	if ( test_array_buffer[0] == test_array[0] )
//	{
//		if ( test_array_buffer[1] == test_array[1] )
//		{
//			
//		}
//		else
//		{
//
//		}
//	}
//	else 
//	{
//
//	}
//*/
//	NV_BLOCK test;
//	NV_BLOCK * test_ptr = &test;
//	
//	test.version=nvtestVERSION;
//	test.trans_max=nvtestTRANSMAX; 				//max transistion count value
//	test.move_flag=nvtestMOVEFLAG; 				//flag = %3a if incline data is ok
//	test.current_incline=nvtestCURINCL; 		//current incline value
//	//	test.intercept_config=nvtestINTERCEPT;		//where is this used
//	test.max_incline_config=nvtestMAXINCL;
//	test.actual_incline_config=nvtestINCLCNFG;
//	test.checksum=nvtestCHKSUM;					//Checksum of this structure
//	
//	
//	
//	bError = E2PROM_bE2Write(0, (unsigned char *) test_ptr, 64, 25); //for pointing at structure
//	
//
//	test.version=0;
//	test.trans_max=0; 				//max transistion count value
//	test.move_flag=0; 				//flag = %3a if incline data is ok
//	test.current_incline=0; 		//current incline value
//	//test.intercept_config=0;		//where is this used
//	test.max_incline_config=0;
//	test.actual_incline_config=0;
//	test.checksum=0;				// Checksum of this structure
//	
//	if (bError == E2PROM_FAILURE)
//	{
//
//	}
//	if (bError == E2PROM_STACKOVERFLOW)
//	{
//
//	}
//	if ( bError == E2PROM_NOERROR )  
//    {  
//       E2PROM_E2Read( 0,(unsigned char *) test_ptr, sizeof(NV_BLOCK) );  //Read nv_ram
//
//    }  
//	if ( test.version == nvtestVERSION )
//	{
//		if ( test.trans_max == nvtestTRANSMAX )
//		{
//
//		}
//		else
//		{
//
//		}
//	} 
}

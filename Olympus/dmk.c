/*************************************************
 *  dmk.c
 *  
 *************************************************/

#include "dmk.h"
#include "constants.h"
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "PSoCGPIOINT.h"
#include "incline.h"
#include "nonvolatile.h"

unsigned char dmk_count = 0;	//debounce count
unsigned char dmk = 0;		//default to no dmk in
unsigned char last_dmk = 0;

/**************************************
 *		get_dmk
 **************************************/
unsigned char get_dmk(void)
{
	return (dmk);
}

/**************************************
 *		update_dmk
 **************************************/
void update_dmk(void)
{
	DMK_Data_ADDR |= DMK_MASK;  //Set the DMK pin high before I read it.
	if( !(DMK_Data_ADDR & DMK_MASK) )
	//if(!(dmk_pin))
		dmk_count++; //debounce this
	else
		dmk_count = 0;

	if(nv_ram.dmk_disabled == 0){	//dmk is enabled
		if(dmk_count > 100)
		{
			dmk = 1; //positive logic   KEY in
			dmk_count = 100;
		}
		else
		{
			dmk = 0;//KEY out
		}
	}
	else	//dmk is disabled
	{
		dmk=1;  //Act like the key is in, because it is disabled
	}	
	
	//Added in 022 modified in 27
	if (dmk == 0)  //Check this once each time the key is pulled
	{
		if(last_dmk != dmk)
		{
			set_stop_incline(1);
		}
	} 
	last_dmk = dmk;
	//End Added in 022
	
}

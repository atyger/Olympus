//*************************************************
//*  machine_table.c
//*************************************************

#include "machine_table.h"
#include "constants.h"
#include "nonvolatile.h"
//??? add back in later  #include "incline.h"


//const far MACHINE_TABLE *	machine_table = (far MACHINE_TABLE *)(0xFDF80);   //Sets where machine table is stored in Flash
//const far unsigned short * 	flash_checksum = (far unsigned short *)(0xFDFFE);  //Sets where checksum is located in Flash

//const MACHINE_TABLE * machine_table = (MACHINE_TABLE *)(0x3FA0);   //Sets where machine table is stored in Flash
//const unsigned short * 	flash_checksum = (unsigned short *)(0x3FAE);  //Sets where checksum is located in Flash

MACHINE_TABLE machine_table_ram;
MACHINE_TABLE * machine_table = &machine_table_ram;
unsigned short * flash_checksum;


/* ROM table of console information, to be used by production to configure machine
	defaults
*/
/*#pragma ASM
	.section	machine_tbl,ROMDATA

	// table
	.org		0x3FA0h
	//informational--read only
		.byte	MACHINE_TYPE				// unsigned char	machine_type
	//configurable area
		.byte	CONFIG_MIN_SPEED			// unsigned char	min_speed;
		.byte	CONFIG_MAX_SPEED			// unsigned char	max_speed;
		.byte	CONFIG_INTERCEPT			// unsigned char	intercept;
		.word	CONFIG_TRANS_MAX			// unsigned short	transmax;
		.byte	CONFIG_MIN_INCLINE			// unsigned char	min_incline;
		.byte	CONFIG_MAX_INCLINE			// unsigned char	max_incline;
		.byte	CONFIG_ACTUAL_INCLINE		// unsigned char	actual_incline;
		.word	CONFIG_ROLLER_SIZE			// unsigned short	roller_size;
		.byte	CONFIG_DEFAULT_VOLUME		// unsigned char	default_volume
		.byte	CONFIG_CURRENT_LIMIT		// unsigned char	current_limit;
		.byte	CONFIG_IR_COMP				// unsigned char	ir_comp;
		.byte	CONFIG_CONTROLLER_MAX_SPEED	// unsigned char	controller_max_speed;
		.byte	CONFIG_FOLDBACK				// unsigned char	foldback;
		.byte	CONFIG_CONTROLLER_PRESENT	// unsigned char	controller_present;
		.byte	CONFIG_MAX_RESISTANCE		// unsigned char	max_resistance;
		.byte	CONFIG_MIN_RESISTANCE		// unsigned char	min_resistance;
		.byte	CONFIG_MAX_STRIDE_LENGTH	// unsigned char	max_stride_length;
		

	//checksum
	.org		0x3FAEh
		.word	FLASH_CHECKSUM				// unsigned short	flash_checksum
	
#pragma ENDASM
*/

void load_machine_table_defaults(void)
{
	machine_table->machine_type = 0x03;
//	machine_table->min_speed = 0x05;
//	machine_table->max_speed = 0x78;
//	machine_table->intercept = 0x20;
	machine_table->trans_max = 0xFFFE;
//	machine_table->min_incline = 0x00;
	machine_table->max_incline = 0xFF;
	machine_table->current_incline = 0xFE;	//This needs to be fe so the brain can come up & control.
	machine_table->actual_incline = 0xFF;
//	machine_table->roller_size = 0x292E;
//	machine_table->lcd_contrast = 0x08;
//	machine_table->default_volume = 0x00;
//	machine_table->current_limit = 0x05;
//	machine_table->ir_comp = 0x78;
//	machine_table->controller_max_speed = 0x0F;
//	machine_table->foldback = 0x00;
//	machine_table->controller_present = 0x00;
//	machine_table->min_resistance = 0x00;
//	machine_table->max_resistance  = 0x00;
//	machine_table->max_stride_length = 0x00;
	nv_ram.load_machine_table_defaults_count++;
	set_update_nv(1);
}


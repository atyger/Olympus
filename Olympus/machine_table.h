//*************************************************
//*  machine_table.h
//*************************************************

#ifndef		MACHINE_TABLE_H
#define		MACHINE_TABLE_H


//languages
#define		LANG_ENGLISH	0
#define		LANG_SPANISH	1
#define		LANG_FRENCH		2

//units
#define		UNITS_ENGLISH	0
#define		UNITS_METRIC	1

//rollers
#define ROLLER_CONST_16			11817 //speed constant for 1.6 inch roller
#define ROLLER_CONST_19			13780 //speed constant for 1.9 inch roller
#define ROLLER_CONST_25			18064 //speed constant for 2.5 inch roller

//void SetMachineTable (void);

typedef	struct _MACHINE_TABLE
{
	// Machine-specific constants
	unsigned char		machine_type;
//	unsigned char		min_speed;
//	unsigned char		max_speed;
//	unsigned char		intercept;
	unsigned short		trans_max;
//	unsigned char		min_incline;
	unsigned char		max_incline;
	unsigned char		actual_incline;
	unsigned char		current_incline;	
//	unsigned short		roller_size;
//	unsigned char		lcd_contrast;
//	unsigned char		default_volume;
//	unsigned char		current_limit;
//	unsigned char		ir_comp;
//	unsigned char		controller_max_speed;
//	unsigned char		foldback;
//	unsigned char		controller_present;
//	unsigned char		min_resistance;
//	unsigned char		max_resistance;
//	unsigned char		max_stride_length;
} MACHINE_TABLE;


extern MACHINE_TABLE * machine_table;
extern MACHINE_TABLE machine_table_ram;
//extern const far unsigned short * flash_checksum;


void load_machine_table_defaults(void);

#endif	// #ifndef		MACHINE_TABLE_H

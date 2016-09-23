/*************************************************
 *  incline.c
 *  
 *************************************************/
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "PSoCGPIOINT.h"

#include "timer8_ISR.h"
#include "constants.h"
#include "incline.h"
#include "nonvolatile.h"
#include "main.h"
#include "machine_table.h"
#include "myuart.h"


//#include "bradg3_bsp.h"
//ATT #include "timer.h"
//ADD BACK LATER #include "keyboard.h"
//ADD BACK LATER #include "vfd_m.h"
//ADD BACK LATER #include "motor.h"
#include "dmk.h"
//ATT #include "buzzer.h"

//extern unsigned char language;
volatile unsigned char upFlag;
volatile unsigned char dnFlag;



//extern NV_BLOCK	nv_ram;		// This is the ram copy

void move_incline(void);
void clear_all(void);
unsigned char trans_wait(void);
void set_stop_incline(unsigned char);

unsigned char iuen=0;		//ATT global variable since there is not a physical iuen pin
unsigned char iden=0;		//ATT global variable since there is not a physical iden pin
unsigned char ins;
unsigned char cal_flag;
unsigned char stop_incline;
unsigned char inc_tocnt;
unsigned char inc_wait;
unsigned char last_incline_state;
unsigned char incline_state;
unsigned char coast_timer;
unsigned char transinp;
unsigned char desired_incline;
unsigned char error_code;
//unsigned char min_incline;
unsigned char actual_incline;
unsigned char calibrating_incline;
unsigned char last_current_incline;
unsigned char oscillation_count;


unsigned int inc_msc;
unsigned int inc_pwid;
unsigned int trans_count;
unsigned int trans_des;
//unsigned int trans_max;
unsigned int step_time = 20;
unsigned int total_tenths;


unsigned int no_trans_count;


const unsigned int inc_table[]={
// New table (-3.0% - 30%) Jan. 2009
0,
100,
100,
90,
90,
95,
110,
110,
110,
115,
117,
120,
122,
122,
124,
120,
125,
123,
125,
123,
125,
125,
122,
122,
122,
122,
122,
122,
122,
119,
122,
120,  //038 CHANGED THIS TO 120 SO IT WOULDN'T OVERLAP
119,
117,
118,
118,
115,
114,
112,
112,
112,
112,
112,
110,
110,
110,
109,
108,
107,
107,
106,
105,
105,
104,
104,
103,
102,
101,
101,//26
100,//26.5
100,//27
100,//27.5
100,//99,	28
100,//99,	28.5
100,//98,	29
100,//97,	29.5
100	//30
	};

/*************************************************
 *  set_stop_incline
 *
 *************************************************/
void set_stop_incline(unsigned char si)
{
	if(!(iuen) || !(iden))
		stop_incline = si;
}

/*************************************************
 *  update_incline_tenth
 *
 *************************************************/
void update_incline_tenth(void)
{
	//nv_ram.current_incline >= desired_incline
//	unsigned char temp_array[] = {'I','w','d','i',0x00,0x0A};
//	
//	nv_ram.trans_des = trans_des;
//	nv_ram.trans_count = trans_count;
//	
//	nv_ram.f1=0xAA;
//	nv_ram.f2=0xFF;
//	
//	nv_ram.trans_count=trans_count;		//added by att
//	nv_ram.trans_des=trans_des;			//added by att
//	nv_ram.desired_incline=desired_incline;	//added by att
//	nv_ram.my_current_incline = nv_ram.current_incline;
///	nv_ram.my_crc=CRC(temp_array,sizeof(temp_array));
//
//	nv_ram.f3=0xFF;
//	nv_ram.f4=0xAA;
	

		
	
	if(inc_wait)
	{
		if(inc_tocnt)
		{
			inc_tocnt--;
			reset_inc_timers();	//038
		}
		else
		{
			inc_wait = 0;
			incline_state = 0;
		}
	}
	if(coast_timer)
		coast_timer--;
}

/*************************************************
 *  update_incline_interrupt
 *  
 *************************************************/
void update_incline_interrupt(void)
{
	if (get_calibrating_incline())
	{
		LED3_Switch(1);
	}
	else
	{
		LED3_Switch(0);
	}
	
	//-----------------------Changed this section for version 030-------------------------
	
	/*if (iden==0 && iuen==0)
	{
		Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
		Port_2_Data_SHADE &= ~INCL_DN_MASK;
	}
	else if (iden==1 && iuen==0)
	{
		if(Port_2_Data_SHADE & INCL_UP_MASK)		//if heading up don't change directions immediately
		{	
			Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
			Port_2_Data_SHADE &= ~INCL_DN_MASK;
			inc_wait = 1;
			inc_tocnt = SWITCH_DIR;
		}
		else if (!inc_wait)
		{
			Port_2_Data_SHADE |= INCL_DN_MASK;		//set the incline_dn pin high to turn on the incline
			Port_2_Data_SHADE &= ~INCL_UP_MASK;     //set the incline_up pin low
		}
	}
	else if (iuen==1 && iden==0)
	{
		if(Port_2_Data_SHADE & INCL_DN_MASK)		//if heading down don't change directions immediately
		{	
			Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
			Port_2_Data_SHADE &= ~INCL_DN_MASK;
			inc_wait = 1;
			inc_tocnt = SWITCH_DIR;
		}
		else if (!inc_wait)
		{
			Port_2_Data_SHADE |= INCL_UP_MASK;		//set the incline_up pin high to turn on the incline
			Port_2_Data_SHADE &= ~INCL_DN_MASK;     //set the incline_dn pin low 
		}
	}
	else if (iuen==1 && iden==1)
	{
		Port_2_Data_SHADE &= ~INCL_UP_MASK;		//don't try to drive them both at the same time.
		Port_2_Data_SHADE &= ~INCL_DN_MASK;
	}
	*/
	//----------------------------End Changed this section for version 030---------------------------------------
	//-----------------------------037---------------------------------------------------------------------------
	//IF BOTH ARE OFF THEN TURN THE INCLINE OFF
	if (iden==1 && iuen==1)						//if both are off then turn off
	{
		Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
		Port_2_Data_SHADE &= ~INCL_DN_MASK;
	}
	//IF INCLINE DOWN 
	else if (iden==0 && iuen==1)					//We want to move down
	{
		if(Port_2_Data_SHADE & INCL_UP_MASK)		//if heading up don't change directions immediately
		{	
			Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
			Port_2_Data_SHADE &= ~INCL_DN_MASK;
			inc_wait = 1;
			inc_tocnt = SWITCH_DIR;
		}
		else if (!inc_wait)
		//else if (!inc_wait && !coast_timer)			//040  don't try to move while coasting
		{
			Port_2_Data_SHADE |= INCL_DN_MASK;		//set the incline_dn pin high to turn on the incline
			Port_2_Data_SHADE &= ~INCL_UP_MASK;     //set the incline_up pin low
		}
	}
	//IF INCLINE UP
	else if (iuen==0 && iden==1)					//We want to move up
	{
		if(Port_2_Data_SHADE & INCL_DN_MASK)		//if heading down don't change directions immediately
		{	
			Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
			Port_2_Data_SHADE &= ~INCL_DN_MASK;
			inc_wait = 1;
			inc_tocnt = SWITCH_DIR;
		}
		else if (!inc_wait)
		//else if (!inc_wait && !coast_timer)			//040  don't try to move while coasting
		{
			Port_2_Data_SHADE |= INCL_UP_MASK;		//set the incline_up pin high to turn on the incline
			Port_2_Data_SHADE &= ~INCL_DN_MASK;     //set the incline_dn pin low 
		}
	}
	//IF BOTH ARE ON THEN TURN OFF
	else if (iuen==0 && iden==0)				//if both are on then turn off
	{
		Port_2_Data_SHADE &= ~INCL_UP_MASK;		//don't try to drive them both at the same time.
		Port_2_Data_SHADE &= ~INCL_DN_MASK;
	}
	//------------------------------------------037end----------------------------------------------------
	
	PRT2DR = Port_2_Data_SHADE;					//Actually write it out to the pin
	if ((INS_Data_ADDR & INS_MASK)==0)					//Look at the pin and set the variable to match
	{
		ins = 0;	
		LED1_Switch(1);	//041
	
	}
	else
	{
		ins = 1;
		LED1_Switch(0);	//041
	}
	
	if ( inc_msc )
		inc_msc--;
	if ( inc_pwid )
		inc_pwid--;
	else
	{
		if (transinp != ins)
		{
			transinp = ins;
			inc_msc = MAX_IPWD;
			inc_pwid = MIN_IPWD;
			//039 if ( !(iuen) )	//I think this needs to look at the actual pin 
			if ((Port_2_Data_SHADE & INCL_UP_MASK))	//if going up
			{
				trans_count++;	//LED5_Invert();
			}
			//039 else if ( !(iden) )
			else if ((Port_2_Data_SHADE & INCL_DN_MASK))	//if going down
			{
				//if ((PRT2DR & INCL_DN_MASK)==1)	//if going down
				if (trans_count)
				{
					trans_count--;	//LED3_Invert();
				}
				if (!(trans_count)) //bottom seek
				{
					trans_count++;	//LED5_Invert();
				}
			}
		//	else 
			else if (coast_timer)
			{
				if (coast_timer)
				{
					if ( GOING_UP == last_incline_state )
					{
						trans_count++;	//LED5_Invert();
					}
					else
					{
						if (trans_count)
						{
							trans_count--;	//LED3_Invert();
						}
						if (!(trans_count)) //bottom seek
						{
							trans_count++;	//LED5_Invert();
						}
					}
				}
				else
				{
					if (!(trans_count)) //bottom seek
					{
						trans_count++;	//LED5_Invert();
					}
				}
			}
			else
			{
				//040  getting ins signals but not changing trans_count
				no_trans_count++;	//LED5_Switch(1);
			}
			
		}
	}
}

/*************************************************
 *  update_incline
 *  
 *************************************************/
void update_incline(void)
{
	if ( !(iuen) )			//incline up is enabled
	{
		if ( trans_count < trans_des )
		{
			if(!(inc_msc))
			{
				trans_count = nv_ram.trans_max;
				trans_des = nv_ram.trans_max;
				//nv_ram.current_incline = machine_table->max_incline;
				nv_ram.current_incline = nv_ram.max_incline_config;
				//desired_incline = machine_table->max_incline;
				//043	desired_incline = nv_ram.max_incline_config;
				desired_incline = nv_ram.current_incline;	//043
				stop_inc();
			}
		}
		else
		{
			last_incline_state = GOING_UP;
			coast_timer = 200;
			M8C_DisableGInt;  //DISABLE_IRQ;	// Disable global interrupts
			if(inc_msc >= step_time)
				inc_msc = step_time;
			M8C_EnableGInt;  //ENABLE_IRQ;		// Enable global interrupts
			if(!(inc_msc))
			{
				nv_ram.current_incline++;
				stop_inc();
			}
		}
	}
	else if(!(iden))	//incline down is enabled
	{
		if(trans_count > trans_des)  
		{
			if(!(inc_msc))
			{
				trans_count = 0;
				trans_des = 0;
				nv_ram.current_incline = 0;
				desired_incline = 0;
				stop_inc();
			}
		}
		else
		{
			last_incline_state = GOING_DOWN;
			coast_timer = 200;
			M8C_DisableGInt;  //DISABLE_IRQ;	// Disable global interrupts
			if(inc_msc >= step_time)
				inc_msc = step_time;
			M8C_EnableGInt;  //ENABLE_IRQ;		// Enable global interrupts
			if(!(inc_msc))
			{
				nv_ram.current_incline--;
				stop_inc();
			}
		}
	}
	else if((desired_incline != nv_ram.current_incline) && (!(inc_wait) && (nv_ram.current_incline != 0xFE)))
		move_incline();
}


/*************************************************
 *  stop_inc
 *  
 *************************************************/
void stop_inc(void)
{
	iuen = 1; upFlag = 1;
	iden = 1; dnFlag = 1;
	if(!(stop_incline) && (desired_incline != nv_ram.current_incline))
		move_incline();
	else
	{
		if(stop_incline)
			desired_incline = nv_ram.current_incline;
		stop_incline = 0;
		inc_wait = 1;
		inc_tocnt = SWITCH_DIR;
		nv_ram.move_flag = DATA_OK;
		set_update_nv(1);
	}
}


/*************************************************
 *  move_incline
 *  
 *************************************************/
void move_incline(void)
{
	unsigned int  desired_calculation;
	char delta;
	unsigned char moving = 0;
	
	if(nv_ram.current_incline == desired_incline)
	{
		nv_ram.move_flag = DATA_OK;
		set_update_nv(1);
		//Set the delay the first time you reach your destination.
		if (moving)								 //Just do this one time when you are done moving.
		{
			set_turn_around_delay(SWITCH_DIR);   //040  after reaching destination pause before moving again
			moving = 0;
		}
	}
	else
	{
		moving = 1;
		if(nv_ram.current_incline >= desired_incline)
			delta = -1;
		else
			delta = 1;

		//Changed this section to make it work ATT
		if (delta == 1)
		{
			desired_calculation = (unsigned int)(nv_ram.current_incline + 1);
			desired_calculation *= nv_ram.trans_max;
//			desired_calculation /= machine_table->max_incline;
			desired_calculation /= nv_ram.max_incline_config;
//	next two lines to be used to change to a curved incline - need to manually smooth out curved line
			desired_calculation *= inc_table[nv_ram.current_incline + 1];	//Brad & Larry added these 3 lines 6-24-10 to get closer incline values on this non-linear incline motor
			desired_calculation /=100;
			trans_des = desired_calculation;
		}
		else	//delta = -1
		{
			desired_calculation = (unsigned int)(nv_ram.current_incline -1);
			desired_calculation *= nv_ram.trans_max;
//			desired_calculation /= machine_table->max_incline;
			desired_calculation /= nv_ram.max_incline_config;
//	next two lines to be used to change to a curved incline - need to manually smooth out curved line
			desired_calculation *= inc_table[nv_ram.current_incline - 1];	//Brad & Larry added these 3 lines 6-24-10 to get closer incline values on this non-linear incline motor
			desired_calculation /=100;
			trans_des = desired_calculation;
		}
		//end  Changed this section to make it work ATT
		
	//	desired_calculation = (unsigned int)(nv_ram.current_incline + delta);
	//	desired_calculation *= nv_ram.trans_max;
	//	desired_calculation /= machine_table->max_incline;
	//	trans_des = desired_calculation;
		if(delta == 1)
		{
			if(trans_des <= trans_count)
			{
				nv_ram.current_incline++;
				nv_ram.move_flag = DATA_OK;
				set_update_nv(1);

			}
			else
			{
				if(incline_state == GOING_DOWN)
				{
					inc_wait = 1;
					inc_tocnt = SWITCH_DIR;
				}
				else
				{
					last_incline_state = GOING_UP;
					incline_state = GOING_UP;
					iuen = 0; upFlag=0;
					M8C_DisableGInt;  //DISABLE_IRQ;	// Disable global interrupts
					inc_msc = MAX_IPWD;
					inc_pwid = MIN_IPWD;
					M8C_EnableGInt;  //ENABLE_IRQ;		// Enable global interrupts
				}
			}
		}
		else //delta = -1
		{
			if(trans_des >= trans_count)
			{
				nv_ram.current_incline--;
				nv_ram.move_flag = DATA_OK;
				set_update_nv(1);
			}
			else
			{
				if(incline_state == GOING_UP)
				{
					inc_wait = 1;
					inc_tocnt = SWITCH_DIR;
				}
				else
				{
					last_incline_state = GOING_DOWN;
					incline_state = GOING_DOWN;
					iden = 0; dnFlag=0;
					M8C_DisableGInt;  //DISABLE_IRQ;	// Disable global interrupts
					inc_msc = MAX_IPWD;
					inc_pwid = MIN_IPWD;
					M8C_EnableGInt;  //ENABLE_IRQ;		// Enable global interrupts
				}
			}
		}
	}
}

/*************************************************
 *  true_calibrate_incline
 *  
 *************************************************/
void true_calibrate_incline(void)
{
	
	unsigned int calculate;
	calibrating_incline=1;
	
	
	clear_all();
	error_code = NO_ERROR; //no error
	nv_ram.calib_error_code = NO_ERROR; //no error
//019	if((nv_ram.move_flag != DATA_OK) || (get_max_incline_config() != nv_ram.max_incline_config) ||  (nv_ram.trans_max == 0))
//019	{ //do calibration

		// motor off
		iuen = 1; upFlag=1;
		iden = 1; dnFlag=1;
		nv_ram.move_flag = 0;
		set_update_nv( 1 );
		// motor up
		iuen = 0; upFlag=0;
		if(trans_wait())
		{
			error_code = DMK_PULLED;
			nv_ram.calib_error_code = DMK_PULLED;
			clear_all();
			calibrating_incline=0;
			set_update_nv(1);
			return;
		}
		// motor off
		iuen = 1; upFlag=1;
		tenth_delay(SWITCH_DIR);
		// motor down
		iden = 0; dnFlag=0;
		if(trans_wait())
		{
			error_code = DMK_PULLED;
			nv_ram.calib_error_code = DMK_PULLED;
			clear_all();
			calibrating_incline=0;
			set_update_nv(1);
			return;
			
		}
		// motor off
		iden = 1; dnFlag=1;
		if(total_tenths < 40)
		{
			error_code = TENTHS_LOW;
			nv_ram.calib_error_code = TENTHS_LOW;
			clear_all();
			cal_flag=0;
			calibrating_incline=0;
			set_update_nv(1);
			return;
		}
//		nv_ram.f1 = 0xAA;
//		nv_ram.f2 = 0xAA;
//		nv_ram.f3 = nv_ram.trans_max;
//		nv_ram.f4 = 0xAA;
//		nv_ram.f5 = 0xAA;
		calculate = 100*total_tenths; //number of milliseconds
		calculate -= MAX_IPWD;
		calculate /= 16 * nv_ram.trans_max;
		step_time = calculate;
		//calculate = machine_table->max_incline*nv_ram.trans_max;
		calculate = nv_ram.max_incline_config*nv_ram.trans_max;
		//calculate /= machine_table->actual_incline;
		calculate /= nv_ram.actual_incline_config;
		if(calculate >= 2)
			calculate -= 2;
		clear_all();
		nv_ram.trans_max = calculate;
		nv_ram.f6 = nv_ram.trans_max;
		nv_ram.f7 = 0xAA;
		nv_ram.f8 = 0xAA;
		nv_ram.f9 = nv_ram.max_incline_config;
		nv_ram.f10 = nv_ram.actual_incline_config;
		nv_ram.f11 = 0xAA;
		nv_ram.f12 = 0xAA;
		tenth_delay(SWITCH_DIR);
		// motor up
		iuen = 0; upFlag=0;
		if(!(nv_ram.trans_max))
		{
			error_code = ZERO_TRANS_MAX;
			nv_ram.calib_error_code = ZERO_TRANS_MAX;
			clear_all();
			cal_flag=0;
			calibrating_incline=0;
			set_update_nv(1);
			return;
		}
		// motor off
		iuen = 1; upFlag=1;
		nv_ram.move_flag = DATA_OK;
		set_update_nv(1);
		//clear the display, update lcd
//019	}
//019	else
//019	{ //no calibration
//019		desired_incline = nv_ram.current_incline;
//019		calculate = nv_ram.trans_max*nv_ram.current_incline;
//019		calculate /= nv_ram.max_incline_config;
//019		trans_des = calculate;
//019		trans_count = calculate;
//019		transinp = ins;
//019	}
	cal_flag=0;
	if (trans_count > 0)
	{
		cal_flag = 0;
	}
	calibrating_incline=0;
	
}	
	
/*************************************************
 *  clear_all
 *  
 *************************************************/
void clear_all(void)
{
	iuen = 1; upFlag=1;
	iden = 1; dnFlag=1;
	desired_incline = 0;
	nv_ram.current_incline = 0;
	nv_ram.trans_max = 0;
	trans_count = 0;
	trans_des = 0;
	nv_ram.max_incline_config = get_max_incline_config();
	//min_incline=nv_ram.min_incline_config;  						
//ATT	min_incline=0;  Changed in Ver011  						
}

/*************************************************
 *  trans_wait
 *  returns 0 for no error; 1 for dmk pulled
 *************************************************/
unsigned char trans_wait(void)
{
	inc_msc = MAX_IPWD*2;
	total_tenths = 0;
	nv_ram.trans_max = 0;
	trans_count = 5;
	while(inc_msc)
	{
//		LED6_Switch(1);
		M8C_ClearWDT;		//RESET_WDT;	//feed the watch dog
		if(!get_dmk())  // if no dmk and dmk is enabled
		{
//			LED6_Switch(0);
			return(1);
		}
		if(trans_count != 5)
		{
			nv_ram.trans_max++;
			trans_count = 5;
		}
		if(get_tenth_secf())
		{
			set_tenth_secf(0);
			total_tenths++;
		}
	check_uart();	//016
//	#ifdef DELTA_MOTOR_CONTROLLER
//		vfd_m_update();
//	#endif		
	}
	
	return(0);
}


/*************************************************
 *  get_desired_incline
 *
 *************************************************/
unsigned char get_desired_incline(void)
{
	return desired_incline;
}

/*************************************************
 *  get_current_incline
 *  
 *************************************************/
unsigned char get_current_incline(void)
{
	return nv_ram.current_incline;
}
/*************************************************
 *  get_calibrating_incline
 *  
 *************************************************/
unsigned char get_calibrating_incline(void)
{
	return calibrating_incline;
}
/*************************************************
 *  get_trans_max
 *  
 *************************************************/
unsigned int get_trans_max(void)
{
	return nv_ram.trans_max;
}
/*************************************************
 *  set_trans_max
 *  
 *************************************************/
void set_trans_max(unsigned int my_trans_max)
{
	nv_ram.trans_max=my_trans_max;		//018 fixed this it was backwards.
}
/*************************************************
 *  get_min_incline
 *  
 *************************************************/
//unsigned char get_min_incline(void)
//{
//	return(min_incline);
//}

/*************************************************
 *  set_desired_incline
 *  
 *************************************************/
void set_desired_incline(unsigned char d_inc)
{
	//Check for oscilation
		//if the last current incline was max is the new current incline min
	if((nv_ram.current_incline == DEFAULT_MAX_INCLINE) && (last_current_incline == DEFAULT_MIN_INCLINE))
	{
		oscillation_count++;
	}
	//if the last current incline was min is the new current incline max
	else if ((nv_ram.current_incline == DEFAULT_MIN_INCLINE) && (last_current_incline == DEFAULT_MAX_INCLINE))
	{
		oscillation_count++;
	}
	//no oscillation then clear the count
	else
	{
		oscillation_count = 0;
	}
		
	if (oscillation_count >= MAX_OSCILLATIONS)
	{
		desired_incline = nv_ram.current_incline;
		oscillation_count = MAX_OSCILLATIONS;
	}
	else //do this if not oscillating
	{
		if(d_inc > nv_ram.max_incline_config)
		{
			//ATT		error_beep();
		}
		else if(d_inc == desired_incline)
		{		
			//set_buzzer(SINGLE_BEEP);		
		}
		else
		{	
			desired_incline = d_inc;
			nv_ram.move_flag = 0;
			set_update_nv( 1 );
			move_incline();
		}
	}
	last_current_incline = nv_ram.current_incline;
}

/*************************************************
 *  get_max_incline_config
 *  
 *************************************************/
unsigned char get_max_incline_config(void)
{
//	if(check_configuration())
//	{
//		min_incline = machine_table->min_incline;
//		min_incline = 0;	
		//actual_incline = machine_table->actual_incline;
		//return(machine_table->max_incline);
		actual_incline = nv_ram.actual_incline_config;
		return(nv_ram.max_incline_config);
//	}
//	else
//	{
//		min_incline = CONFIG_MIN_INCLINE;
//		actual_incline = CONFIG_ACTUAL_INCLINE;
//		return(CONFIG_MAX_INCLINE);
//	}
}

/*************************************************
 *  no_calibration_initialize
 *  
 *************************************************/
void no_calibration_initialize (void)
{
	unsigned int calculate;
	desired_incline = nv_ram.current_incline;
	calculate = nv_ram.trans_max*nv_ram.current_incline;
//	calculate /= machine_table->max_incline;
	calculate /= nv_ram.max_incline_config;
	trans_des = calculate;
	trans_count = calculate;
	transinp = ins;
}

//--------------------------------------------------
//  load_incline_defaults
//  
//--------------------------------------------------
void load_incline_defaults (void)
{
	nv_ram.version = VERSION;
	nv_ram.trans_max = DEFAULT_TRANS_MAX; 				
	nv_ram.current_incline = DEFAULT_CURRENT_INCLINE; 	
	nv_ram.max_incline_config = DEFAULT_MAX_INCLINE;
//	nv_ram.min_incline_config = DEFAULT_MIN_INCLINE;
	nv_ram.actual_incline_config = DEFAULT_ACTUAL_INCLINE;
	nv_ram.dmk_disabled=0;			//1=dmk disabled, 0=dmk enabled
	nv_ram.move_flag = DATA_OK;
	set_update_nv(1);
	step_time = 20;
	//set_desired_incline(nv_ram.current_incline);
//042	set_desired_incline(0);	//Added back in in 42 bottom seek
	//set_desired_incline(10);	//bottom seek
	set_timer_do_once();
}

void calibrate_incline (void)
{
	calibrating_incline=1;
	load_incline_defaults();
	while(get_current_incline());
	calibrating_incline=0;
}


//--------------------------------------------------
//  get_trans_count
//  
//--------------------------------------------------
unsigned int get_trans_count(void)
{
	return trans_count;
}
//--------------------------------------------------
//  get_no_trans_count
//  
//--------------------------------------------------
unsigned int get_no_trans_count(void)
{
	return no_trans_count;
}
//--------------------------------------------------
//  get_trans_des
//  
//--------------------------------------------------
unsigned int get_trans_des(void)
{
	return trans_des;
}

//--------------------------------------------------
//  reset_inc_timers
//  
//--------------------------------------------------
void reset_inc_timers (void)
{
	inc_msc = MAX_IPWD;
	inc_pwid = MIN_IPWD;
}

//--------------------------------------------------
//  set_turn_around_delay
//  
//--------------------------------------------------
void set_turn_around_delay (unsigned char f)
{
	inc_msc = MAX_IPWD;
	inc_pwid = MIN_IPWD;
	inc_wait = 1;
	inc_tocnt = f;
}

void set_incline_off_pins (void)
{
	Port_2_Data_SHADE &= ~INCL_UP_MASK;		//turn both off.
	Port_2_Data_SHADE &= ~INCL_DN_MASK;
	PRT2DR = Port_2_Data_SHADE;				//Actually write it out to the pin
}

void set_incline_up_pins (void)
{
	Port_2_Data_SHADE |= INCL_UP_MASK;		//set the incline_up pin high to turn on the incline
	Port_2_Data_SHADE &= ~INCL_DN_MASK;     //set the incline_dn pin low 
	PRT2DR = Port_2_Data_SHADE;				//Actually write it out to the pin
}

void set_incline_dn_pins (void)
{
	Port_2_Data_SHADE |= INCL_DN_MASK;		//set the incline_dn pin high to turn on the incline
	Port_2_Data_SHADE &= ~INCL_UP_MASK;     //set the incline_up pin low
	PRT2DR = Port_2_Data_SHADE;				//Actually write it out to the pin
}
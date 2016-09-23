/*************************************************
 *  Timer8_ISR.c
 *  
 *************************************************/
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "main.h"
#include "constants.h"
#include "PSoCGPIOINT.h"

#include "timer8_ISR.h"
#include "incline.h"
#include "nonvolatile.h"
#include "machine_table.h"
#include "myuart.h"
#include "incline.h"
//#include "motor.h"
//#include "pulse.h"
//#include "hpulse.h"
//#include "keyboard.h"
//#include "fan.h"
//#include "host_comm.h"
//#include "usb_cdc.h"
//#include "usb_cdc_app.h"
//#include "crc16.h"
//#include "eq.h"
//#include "display.h"
//#include "lcd_uc1610.h"
#include "dmk.h"
//#include "buzzer.h"
//#include "print.h"
//#include "vfd_m.h"

//static unsigned char continuous_beep;
volatile unsigned char timer=0;
volatile unsigned char tmrMotor=0;
//unsigned char backlight_state;
//unsigned char shade;

// Variables accesible to this file
//BYTE timer_125us_count;
BYTE timer_250us_count;
BYTE timer_500us_count;
BYTE timer_1ms_count;
BYTE timer_50ms_count;
BYTE timer_100ms_count;
BYTE timer_1s_count;
BYTE timer_do_once;
//BYTE tenth_count;

//012 static unsigned char mode;	//normal mode, test mode, user mode, etc.
//012 unsigned int random_seed;
//unsigned short byteCnt = 0;
//unsigned char volume_temp = 0;
//unsigned char usb_init_flag;
//unsigned char while_cnt;
//unsigned char backlight_pwm;
//unsigned char backlight_state;
//unsigned char backlight_percent = 0;
//012 extern unsigned char erasing_nv;
//012 unsigned char tx_status;
//const unsigned char send_array[]={0x41 + 0x80, 0x6C+0x80, 0x61 + 0x80, 0x6E + 0x80, 0x20 + 0x80};
//const unsigned char send_array[]={0xC1, 0xEC, 0xE1, 0xEE, 0xA0};
//const unsigned char err1[]={0xC5, 0xD2, 0xD2, 0xB1, 0xA0};
//const unsigned char err2[]={0xC5, 0xD2, 0xD2, 0xB2, 0xA0};
//const unsigned char err3[]={0xC5, 0xD2, 0xD2, 0xB3, 0xA0};
//const unsigned char err4[]={0xC5, 0xD2, 0xD2, 0xB4, 0xA0};
//const unsigned char err5[]={0xC5, 0xD2, 0xD2, 0xB5, 0xA0};
//const unsigned char err6[]={0xB1, 0xB2, 0xB3, 0xB4, 0xA0};
//unsigned char read_buffer[8];
//unsigned char send_buffer[8];
//unsigned char read_index=0;
//unsigned char send_index=0;
//BOOL address_received;

//BYTE read_char;
//const unsigned char send_array[]={0xC1};

/*struct bf {  //buzzer flags
	union {
    	unsigned char BYTE;
        struct {
           	unsigned char myb:6;	//for alignment
           	unsigned char err_b:1;	//error buzz
           	unsigned char b1:1;		//buzzer on/off
		}   BIT;
	} buzz;
} buzz_flags;*/

//#define err_buzz buzz_flags.buzz.BIT.err_b
//define buzz1 buzz_flags.buzz.BIT.b1

struct time_f {  //timing flags
	union {
    	unsigned char BYTE;
        struct {
           	unsigned char mb:6;		//for alignment
           	unsigned char fhus:1;	//five hundred uS
           	unsigned char osf:1;	//one_secf
           	unsigned char tsf:1;	//tenth_secf
			unsigned char fmsf:1;	//fifty mS
			unsigned char omsf:1;	//one mS
		}   BIT;
	} timing;
} timing_flags;

#define one_secf timing_flags.timing.BIT.osf
#define tenth_secf timing_flags.timing.BIT.tsf
#define five_hundred_micro_seconds timing_flags.timing.BIT.fhus
#define fifty_mili_secf timing_flags.timing.BIT.fmsf
#define five_hundred_usecf timing_flags.timing.BIT.fhus
#define one_msecf	timing_flags.timing.BIT.omsf

void delay( unsigned int t )
{
	unsigned char t0;
	
	if( !t )
		return;
	
	while( t>255 )
	{	
		M8C_ClearWDT;  //RESET_WDT; //feed the watch dog	
		check_uart();
		t0 = timer + 255;
		while( t0!=timer )			
		t-=255;
	}
		
	t0 = timer + t;
	while( t0!=timer )
	{
		M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
		check_uart();
	}
}
 
//void interrupt main_timer_isr(void)@0x00  //This is for the Hi-Tech compiler see section 3.8.1
void main_timer_isr(void)		//This is for image craft.  #pragma interrupt_handler main_timer_isr in Timer8_ISR.h 
{
	
	static unsigned char	buzzer_bit_count;
/*********************250uS********************************/	
	//ATT	pwm_interrupt();						//
	//ADD BACK IN LATER		tach_interrupt();		//
	//ATT	buzzer_interrupt();						// 250 uS
	//ATT	fan_interrupt();						//

		update_dmk();			//
		update_tach();
		//012 random_seed ++; //adjust random seed	//
		//043	update_incline_interrupt();			
		timer++;									//
		if( tmrMotor )								//	1 mS	(every 2nd 125 uS)
			tmrMotor--;								//
		
	//ATT	pulse_interrupt();						//
	
	
/*********************250uS********************************/	
	timer_250us_count++;
	if(timer_250us_count == 2)
	{
	 	timer_250us_count = 0;
		timer_500us_count++;
	 	//***************************************** 500 uS
		five_hundred_usecf = 1;				//*
		//***************************************** 500 uS		
		if(timer_500us_count == 2)
		{
			timer_500us_count = 0;
			timer_1ms_count++;
			//***************************************** 1 mS
					update_incline_interrupt();	
					one_msecf = 1;
			//***************************************** 1 mS
			if(timer_1ms_count == 50)
			{
				timer_1ms_count = 0;
				timer_50ms_count++;
				//***************************************** 50 mS
					fifty_mili_secf = 1;				//*
				//***************************************** 50 mS
				if(timer_50ms_count == 2)
				{
					timer_50ms_count = 0;
					timer_100ms_count++;
					//***************************************** 100 mS
					tenth_secf = 1;							//* 100 mS
					//***************************************** 100 mS
					if(timer_100ms_count == 10)
					{
						timer_100ms_count = 0;
						timer_1s_count++;
						//***************************************** 1 S
						one_secf = 1;							//* 1 S
						//***************************************** 1 S
					}
				}
			}
		}
	}
	
	return;
}


/**************************************
 *		tenth_delay
 *		argument is number of tenths of seconds to delay
 **************************************/
void tenth_delay(unsigned char tenth_seconds)
{
	while (tenth_seconds != 0)
	{
		M8C_ClearWDT;  //RESET_WDT;
		check_uart();
		if( tenth_secf )
		{
			tenth_seconds --;
			tenth_secf = 0;
		}
	}
}

/**************************************
 *		get_tenth_secf
 **************************************/
unsigned char get_tenth_secf(void)
{
	return(tenth_secf);
}

/*************************************************
 *  set_tenth_secf
 *************************************************/
void set_tenth_secf(unsigned char tsf)
{
	tenth_secf = tsf;
}

//012/*************************************************
//012 *  get_mode
//012 *
//012 *************************************************/
//012unsigned char get_mode(void)
//012{
//012	return(mode);
//012}

//012/*************************************************
//012 *  set_mode
//012 *
//012 *************************************************/
//012void set_mode(unsigned char m)
//012{
//012	//turn off treadmill belt
//012	//turn off fan
//012	//stop incline
//012	mode = m;
//012}

/**************************************
 *		get_tenth_sec_count
 **************************************/
unsigned char get_tenth_sec_count(void)
{
	return(timer_100ms_count);
}


/*****************************************************************************
Name: system_idle
Parameters:  none
Returns:     none
Description: The system is updated using the timerA0 interrupt and running
			 throgh the these while loops.
*****************************************************************************/
void system_idle(void)
{
//	BYTE test_do_once=1;
	BYTE my_oneS_timer=0;
	signed char temp_s_char;
	while(1)
	{
		M8C_ClearWDT;
		//************RUN INCLINE*******************
		//test_incline_switches();
		//******************************************
				
//		#ifdef DELTA_MOTOR_CONTROLLER
//		if( !tmrMotor ) 
//ADD BACK IN LATER			vfd_m_update();	//BJD
//		#endif			
//			if (test_do_once==1)
//			{
//				test_do_once=0;
//			//	update_uart(0);
//			}
		if(get_calibrate_incline_now())
		{
			set_calibrate_incline_now(0);
//041			nv_ram.current_incline = 0xFE;		
//041			nv_ram.trans_max = 0x0FFFE;		
			calibrate_incline();
		}
		
		update_incline();
		check_uart();
		
		if (timer_do_once)
		{
			if (get_current_incline())	//don't do anything until incline reaches 0
			{
				set_desired_incline(0);
			}
			else
			{
				timer_do_once = 0;
				set_desired_incline(6);
			}
		}
//*************LEDS*******************
	//LED1 is the ins  it is taken care of in incline.c about line 306 & 312
	//LED2 inverts when you load a character into the fifo in uart0_rx_isr
	//LED3=calibrating
	/*	if (get_calibrating_incline())
		{
			LED3_Switch(1);
		}
		else
		{
			LED3_Switch(0);
		}*/
	//LED3	on = dmk disabled
	//LED3 off = dmk enabled  changed to dmk enabled in 
	if (nv_ram.dmk_disabled)
	{
		LED3_Switch(1);
	}
	else
	{
		LED3_Switch(0);
	}
	//LED4
	if( !(DMK_Data_ADDR & DMK_MASK) )
		LED4_Switch(0); //DMK IS OUT
	else
		LED4_Switch(1); //DMK IS IN
	//LED5  Toggles everytime a valid command is received
	//LED6	is the speed tach.
		
//*************LEDS*******************
		
		if (five_hundred_usecf )
		{
			five_hundred_usecf = 0;
		}
		if (one_msecf)
		{
			one_msecf = 0;
//			check_uart();
		}
		if( fifty_mili_secf )
		{
			fifty_mili_secf = 0;
//			check_uart();
		}
		if( tenth_secf )
		{
			tenth_secf = 0;
			update_incline_tenth();
			//test_uart();
	
//ADD BACK IN LATER			hostcomm_tenth();
//ADD BACK IN LATER			update_cspeed();
		}
		if( one_secf )
		{
			one_secf = 0;
			PRT1DR &= ~RS485_DE_MASK;	//SET LOW
			PRT1DR &= ~RS485_RE_MASK;	//SET LOW
	
			
			my_oneS_timer++;
			if (my_oneS_timer==10)
			{
			//042	if (timer_do_once)
			//042	{
			//042		timer_do_once = 0;
			//042		set_desired_incline(6);
			//042	}
				//move incline to max
				//set_desired_incline(machine_table->max_incline);
			}  
			if (my_oneS_timer==40)
			{
				//move incline to max
				//set_desired_incline(0x0A);
				//ATT VER008	set_desired_incline(60);
				
			}  
			if (my_oneS_timer==100)
			{
				my_oneS_timer=0;
			}  
//ADD BACK IN LATER			adjust_intercept();

			temp_s_char = update_nonvolatile();	//write to nonvolatile if flag is set
			set_nv_bError(temp_s_char);		
		
					//check_max_speed();
		}
	}
}


/*void test_incline_switches (void)
{
	if ((PRT0DR & INS_MASK)==0)
	{
		if (PRT0DR & TACH_MASK)
		{
			Port_2_Data_SHADE &= ~INCL_DN_MASK;     //Set low first 
			Port_2_Data_SHADE |= INCL_UP_MASK;		//set INCL_UP high to turn on incline

		}
		else
		{
			Port_2_Data_SHADE &= ~INCL_DN_MASK;  //Set low so the incline won't go
			Port_2_Data_SHADE &= ~INCL_UP_MASK;  //Set low so the incline won't go

		}
	}
	else if ((PRT0DR & TACH_MASK)==0) 
	{
		if (PRT0DR & INS_MASK)
		{
			Port_2_Data_SHADE &= ~INCL_UP_MASK;     //Set low first 
			Port_2_Data_SHADE |= INCL_DN_MASK;		//set INCL_DN high to turn on incline

		}
		else
		{
			Port_2_Data_SHADE &= ~INCL_DN_MASK;  //Set low so the incline won't go
			Port_2_Data_SHADE &= ~INCL_UP_MASK;  //Set low so the incline won't go

		}
	}
	else
	{
		Port_2_Data_SHADE &= ~INCL_DN_MASK;  //Set low so the incline won't go
		Port_2_Data_SHADE &= ~INCL_UP_MASK;  //Set low so the incline won't go

	}
	PRT2DR = Port_2_Data_SHADE;			//Added in version 005
} */


//void test_idle(void)
//{
//	BYTE my_oneS_timer=0;
//	while(1)
//	{
//		M8C_ClearWDT;
//		//************RUN INCLINE*******************
//		//test_incline_switches();
//		//******************************************
//		if( tenth_secf )
//		{
//			tenth_secf = 0;
//		}
//		if( one_secf )
//		{
//			one_secf = 0;
//			
//			//my_oneS_timer++;
//			/*if (my_oneS_timer==30)
//			{
//
//			}  
//			if (my_oneS_timer==60)
//			{
//
//			}  
//			if (my_oneS_timer==90)
//			{
//				my_oneS_timer=0;
//
//			}  */
//		}
//	}
//}

void update_tach(void)
{
	if ((PRT0DR & TACH_MASK)==0)					//Look at the pin and set the variable to match
	{
		LED6_Switch(1);	//041

	}
	else
	{
		LED6_Switch(0);	//041

	}


}

void set_timer_do_once (void)
{
	timer_do_once = 1;
}

BYTE get_timer_do_once (void)
{
	return timer_do_once;
}
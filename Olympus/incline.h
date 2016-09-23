/*************************************************
 *  incline.h
 *  
 *************************************************/

#ifndef INCLINE
#define INCLINE

void set_stop_incline(unsigned char);
void update_incline(void);
void update_incline_tenth(void);
void update_incline_interrupt(void);
void calibrate_incline(void);
unsigned char get_max_incline_config(void);
unsigned char get_desired_incline(void);
unsigned char get_calibrating_incline(void);
unsigned int get_trans_max(void);
//unsigned char get_min_incline(void);
//#define get_min_incline() ((unsigned char)CONFIG_MIN_INCLINE)
#define INCLINE_OFFSET	12
void set_desired_incline(unsigned char);
void incline_up_key(void);
void incline_down_key(void);
void stop_inc(void);
unsigned char get_current_incline(void);
void set_trans_max(unsigned int my_trans_max);
void no_calibration_initialize (void);
void true_calibrate_incline (void);


void quick_incline_key(unsigned char);
void quick_incline_zero_key(void);
void quick_incline_three_key(void);
void quick_incline_six_key(void);
void quick_incline_nine_key(void);
void quick_incline_twelve_key(void);
void quick_incline_fifteen_key(void);
void quick_incline_eighteen_key(void);
void quick_incline_twentyone_key(void);
void quick_incline_twentyfour_key(void);
void quick_incline_twentyseven_key(void);
void quick_incline_thirty_key(void);
void load_incline_defaults (void);
unsigned int get_trans_count(void);
unsigned int get_trans_des(void);
void reset_inc_timers (void);
void set_turn_around_delay (unsigned char f);
void set_incline_off_pins (void);
void set_incline_up_pins (void);
void set_incline_dn_pins (void);
unsigned int get_no_trans_count(void);


extern unsigned char cal_flag;
extern unsigned char iuen;	
extern unsigned char iden;	
#endif
/*************************************************
 *  Timer8_ISR.h
 *  
 *************************************************/
#pragma	interrupt_handler main_timer_isr
void main_timer_isr(void);
//For HI-TECH void interrupt main_timer_isr(void);
void delay( unsigned int );
void tenth_delay( unsigned char );
unsigned char get_tenth_secf( void );
void set_tenth_secf( unsigned char );
unsigned char get_mode(void);
//012 void set_mode(unsigned char);
unsigned char get_tenth_sec_count(void);
void system_idle(void);
void test_incline_switches (void);

volatile extern unsigned char timer;
volatile extern unsigned char tmrMotor;

void test_idle(void);
void update_tach(void);
void set_timer_do_once (void);
BYTE get_timer_do_once (void);
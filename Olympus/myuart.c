/*************************************************
 *  myuart.c
 *  
 *************************************************/
#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "main.h"
#include "constants.h"
#include "PSoCGPIOINT.h"
#include "fifo.h"
#include "incline.h"
#include "nonvolatile.h"
#include "myuart.h"
#include "Timer8_ISR.h"

//#include "interrupts.h"
//#include "serial.h"
//#include <stdlib.h>
/*#define FIFO_SIZE 32

#define FIFO_OVERRUN 01

#define COMMAND_SIZE 8
//COMMAND BYTE 1
#define UART_ADDRESS 0x07
//COMMAND BYTE 2
#define UART_READ 0x03
#define UART_WRITE 0x06
//COMMAND BYTE 3
#define D_INC_HIGH 0x0A
//COMMAND BYTE
#define D_INC_LOW 0x0B
//COMMAND BYTE 5  Value High Byte
#define VAL_HIGH_B 0x00
*/

//To communicate with delta controller
//Bits per second 38400
//Data bits 7
//Parity None
//Stop Bits 2
//unsigned char gbl_transmit;
//#define ADDRESS_GOOD 0x10
//#define READ_VALID	 0x08
//#define WRITE_VALID  0x04
//#define TO_INC_VALID 0x02
//#define CRC_VALID    0x01

//#define CRC_ERROR    0X01
#define CRC_ERR     0x01
#define COMMAND_ERR 0X02
#define CAL_INC		0x04
#define UNRECOGNIZED_COMMAND 0x08
#define BAD_CALIB_COMMAND 0x10
#define TM_NV_ERROR 0x20
#define MI_NV_ERROR 0x40
#define CAL_ERROR 0x80
#define BOTTOM_SEEKING 0x100

#define MAX_TRANS_MAX 500
#define MIN_TRANS_MAX 20

#define MAX_MAX_INC	180
#define MIN_MAX_INC 16

// Variables accesible to this file
BYTE command_buffer[COMMAND_SIZE];	//store the command here to check the CRC16
BYTE uart_fifo[FIFO_SIZE];
//BYTE test_array[FIFO_SIZE];
//BYTE temp_d_inc;
//BYTE temp_write_cnt;
BYTE ch;
FIFO rxFifo;
unsigned int com_err;
BYTE rx_isr_timer;
BYTE calibrate_incline_now;
BYTE calib_error_count = 0;
//static BYTE temp_ch=0;
//BYTE done_transmitting;
//BYTE old_incline;

//void transmitted_byte( BYTE thisbyte )
//{
//	//This byte containst the UART_TX_CONTROL_REG
//
//}

void myuart_init ( void )
{
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	fifo_init( &rxFifo,	uart_fifo, sizeof(uart_fifo));
}


void check_uart ( void )
{
	BYTE i, test_ch, test_crc_hb, test_crc_lb, checksum_err, respond, temp_lb, temp_hb;
	unsigned int test_crc,temp_int;
	
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	//******************CLEAR_BUFFER & CRC****************************//	
	for (i=0;i<COMMAND_SIZE;i++)	//clear the command_buffer
	{
		command_buffer[i]=0;
	}
	test_crc=0;						//clear test_crc
	com_err=0;						//added in ver 014
	
	if (fifo_peek( &rxFifo, 0 )==UART_ADDRESS)	
	{
		if ( rxFifo.bytes >= COMMAND_SIZE )	//see if there are enough bytes in the fifo to contain a command
		{
			//******************LOAD THE COMMAND_BUFFER****************************//
			for (i=0;i<COMMAND_SIZE;i++)	//load a command byte in the buffer
			{
				command_buffer[i]=fifo_peek( &rxFifo, i );
			}
			
			//nv_ram.f1=command_buffer[0];
			//nv_ram.f2=command_buffer[1];
			//nv_ram.f3=command_buffer[2];
			//nv_ram.f4=command_buffer[3];
			//nv_ram.f5=command_buffer[4];
			//nv_ram.f6=command_buffer[5];
			//nv_ram.f7=0xAA;
			//nv_ram.f8=0xAA;
			//******************CALCULATE CRC OF WHAT WAS RECEIVED*****************//
			test_crc = CRC(command_buffer,COMMAND_SIZE-2);
			//nv_ram.f9=test_crc;
			test_crc_hb = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
			test_crc_lb = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
			//nv_ram.f10=test_crc_hb;
			//nv_ram.f11=test_crc_lb;
			//******************CHECK FOR A VALID COMMAND**************************//
			//if (( command_buffer[1]!=C_UART_WRITE ))// || (command_buffer[1]!=C_UART_READ ))
			if (( command_buffer[1]!=C_UART_WRITE ) && (command_buffer[1]!=C_UART_READ ))	//added in ver 014
			{
				com_err |= COMMAND_ERR;		//Address was right, but next byte was not a read or write.
			}
			if ((command_buffer[6]!=test_crc_hb)) //&& command_buffer[7]==test_crc_lb))
			{	
				com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
			}
			if ((command_buffer[7]!=test_crc_lb)) //&& command_buffer[7]==test_crc_lb))
			{	
				com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
			}
			if (get_calibrating_incline())
			{
				com_err |= CAL_INC;
			}
			else if (nv_ram.calib_error_code != NO_ERROR)
			{
				com_err |= CAL_ERROR;
				calib_error_count = calib_error_count+1;
			}
			if (get_timer_do_once())
			{
				com_err |= BOTTOM_SEEKING;	
			}
			
			
			
			if(!com_err)
			//******************RECEIVED VALID COMMAND WITH VALID CRC****************************//
			{
				if (command_buffer[1]==C_UART_WRITE)
				{
				//***********************************************************************************
				//***********************************************************************************
				//							WRITE
				//***********************************************************************************
				//***********************************************************************************
					if(command_buffer[2]=='d' && command_buffer[3]=='i'){	
					//******************WRITE DESIRED INCLINE************************************//
						temp_int = get_trans_max();
						if((temp_int < MIN_TRANS_MAX) || (temp_int > MAX_TRANS_MAX)){
							com_err |= TM_NV_ERROR;
						}
						if((nv_ram.max_incline_config < MIN_MAX_INC) || (nv_ram.max_incline_config > MAX_MAX_INC)){
							com_err |= MI_NV_ERROR;
						}
						if(!com_err){
							command_buffer[4] = get_current_incline();		//added in ver 014
							if (get_desired_incline() != command_buffer[5]){
								if (get_desired_incline() != get_current_incline())  //if I haven't reached my destination then set delay
								{
									set_turn_around_delay(SWITCH_DIR);
								}
								set_desired_incline(command_buffer[5]);
							}
						}
					}
					else if(command_buffer[2]=='s' && command_buffer[3]=='i'){ 
					//******************STOP INCLINE*********************************************//
						set_stop_incline(1);
					}
					else if(command_buffer[2]=='m' && command_buffer[3]=='i'){
					//******************WRITE MAX INCLINE****************************************//
						nv_ram.max_incline_config  = command_buffer[5];
					}
					else if(command_buffer[2]=='a' && command_buffer[3]=='i'){
					//******************WRITE ACTUAL INCLINE*************************************//
						nv_ram.actual_incline_config  = command_buffer[5];
					}
					else if(command_buffer[2]=='c' && command_buffer[3]=='i'){
					//******************WRITE CURRENT INCLINE****************************************//
						nv_ram.current_incline = command_buffer[5];
					}					
					else if(command_buffer[2]=='t' && command_buffer[3]=='m'){
					//******************WRITE TRANS_MAX******************************************//
						temp_hb = command_buffer[4];
						temp_lb = command_buffer[5];
						temp_int = convert_two_7bit_to_int(temp_hb, temp_lb);
						set_trans_max(temp_int);
						//add in to see if transmax is really getting written
						temp_int = get_trans_max();
						temp_hb = high7bits_of_14bit(temp_int);
						temp_lb = low7bits_of_14bit(temp_int);
						command_buffer[4]=temp_hb;
						command_buffer[5]=temp_lb;
						//end add in to see if transmax is really getting written
						set_update_nv(1);
						no_calibration_initialize();
						set_init_complete(1);
						
					}
					else if(command_buffer[2]=='c' && command_buffer[3]=='a'){
					//******************CALIBRATE INCLINE******************************************//
						if(command_buffer[4]=='l' && command_buffer[5]=='i'){
							set_calibrate_incline_now(1);
							com_err |= CAL_INC;
						}
						else{
							com_err |= BAD_CALIB_COMMAND;
						}
					}
					else if(command_buffer[2]=='d' && command_buffer[3]=='k'){
					//******************WRITE DISABLE DMK*************************************//
						nv_ram.dmk_disabled  = command_buffer[5];  //0=enabled non zero=disabled
					}
					else if(command_buffer[2]=='c' && command_buffer[3]=='e'){
					//******************WRITE CLEAR ERRORS*************************************//
						command_buffer[1]='n';
						command_buffer[2]='o';
						command_buffer[3]='e';
						command_buffer[4]='r';
						command_buffer[5]='r';
						com_err = NO_ERROR; //no error
						nv_ram.calib_error_code = NO_ERROR; //no error
					}
					else{  
					//******************UNRECOGNIZED WRITE COMMAND*******************************//
						com_err |= UNRECOGNIZED_COMMAND;
					}
				}
				else if (command_buffer[1]==C_UART_READ){		//changed to else if in ver 014
				//***********************************************************************************
				//***********************************************************************************
				//							READ
				//***********************************************************************************
				//***********************************************************************************
					//******************READ DESIRED INCLINE************************************//
					if(command_buffer[2]=='d' && command_buffer[3]=='i'){	
						command_buffer[4]=0;
						command_buffer[5]=get_desired_incline();
					}
					//******************READ TRANS_MAX******************************************//
					else if(command_buffer[2]=='t' && command_buffer[3]=='m'){	
						temp_int = get_trans_max();
						temp_hb = high7bits_of_14bit(temp_int);
						temp_lb = low7bits_of_14bit(temp_int);
						command_buffer[4]=temp_hb;
						command_buffer[5]=temp_lb;
					}
					//******************READ CURRENT INCLINE************************************//
					else if(command_buffer[2]=='c' && command_buffer[3]=='i'){   
						command_buffer[4]=0;
						command_buffer[5]=get_current_incline();
					}
					else if(command_buffer[2]=='m' && command_buffer[3]=='i'){
					//******************READ MAX INCLINE****************************************//
						command_buffer[4]=0;
						command_buffer[5]=nv_ram.max_incline_config;
					}
					else if(command_buffer[2]=='a' && command_buffer[3]=='i'){
					//******************READ ACTUAL INCLINE*************************************//
						command_buffer[4]=0;
						command_buffer[5]=nv_ram.actual_incline_config;
					}
					//******************READ CODE VERSION***************************************//
					else if(command_buffer[2]=='c' && command_buffer[3]=='v'){   
						temp_int = nv_ram.version;
						temp_hb = high7bits_of_14bit(temp_int);
						temp_lb = low7bits_of_14bit(temp_int);
						command_buffer[4]=temp_hb;
						command_buffer[5]=temp_lb;
					}
					else if(command_buffer[2]=='d' && command_buffer[3]=='k'){
					//******************READ DISABLE KEY*****************************************//
						command_buffer[4]=0;
						command_buffer[5]=nv_ram.dmk_disabled;	//0=enabled non zero=disabled
					}
					//******************UNRECOGNIZED READ COMMAND********************************//
					else{  //unrecognize command
						com_err |= UNRECOGNIZED_COMMAND;
					}
					
				}
			}
			else
			{
					if ((command_buffer[1]==  'c') && 
				    	 command_buffer[2] == 'l' && 
					 	 command_buffer[3] == 'e' && 
					 	 command_buffer[4] == 'a' &&
					 	 command_buffer[5] == 'r')
					{
						com_err = NO_ERROR; //no error
						nv_ram.calib_error_code = NO_ERROR; //no error
					}
			}
		
			//RESPONSE
			if(!com_err){
			//******************RECEIVED VALID COMMAND********************************//
				respond = 1;
			}
			else if (com_err & COMMAND_ERR){
			//******************ADDRESS NOT FOLLOWED BY READ OR WRITE*****************//
				fifo_pop( &rxFifo, &test_ch );	//pop one byte and try again
				//fifo_pop( &rxFifo, &test_ch );	//pop one byte and try again
				respond=0;						//just pop 2 bytes and don't respond
			}
			else if (com_err & TM_NV_ERROR){
			//******************BAD INITIALIZATION***********************************//
			//not valid transmax or not valid max incline
				command_buffer[1]='b';	
				command_buffer[2]='i';
				command_buffer[3]='n';
				command_buffer[4]='i';
				command_buffer[5]='t';
				respond=1;
			}
			else if (com_err & MI_NV_ERROR){
			//******************BAD INITIALIZATION***********************************//
			//not valid transmax or not valid max incline
				command_buffer[1]='b';	
				command_buffer[2]='i';
				command_buffer[3]='n';
				command_buffer[4]='i';
				command_buffer[5]='t';
				respond=1;
			}
			else if (com_err & CAL_INC){
			//******************UNIT IS CALIBRATING***********************************//
				command_buffer[1]='c';
				command_buffer[2]='a';
				command_buffer[3]='l';
				command_buffer[4]='i';
				command_buffer[5]='b';
				respond=1;
			}
			else if (com_err & CRC_ERR){
			//******************CRC WAS NOT A MATCH***********************************//
				command_buffer[2]='n';
				command_buffer[3]='v';
				command_buffer[4]=test_crc_hb;
				command_buffer[5]=test_crc_lb;
				respond=1;
			}
			else if (com_err & UNRECOGNIZED_COMMAND){
			//******************UNRECOGNIZED COMMAND***********************************//
				command_buffer[1]='u';	//un-
				command_buffer[2]='r';	//recognized
				command_buffer[3]='c';	//command
				command_buffer[4]='m';
				command_buffer[5]='d';
				respond=1;
			}
			else if (com_err & BAD_CALIB_COMMAND){
			//******************BAD CALIBRATION COMMAND***********************************//
				command_buffer[1]='b';	//received I w c a
				command_buffer[2]='a';	//but did not receive l i after that
				command_buffer[3]='d';	
				command_buffer[4]='c';
				command_buffer[5]='a';
				respond=1;
			}
			else if (com_err & CAL_ERROR){
			//******************EXITED CALIBRATION WITH ERROR**************************//
					command_buffer[1]='c';	//calibration error
					command_buffer[2]='e';
				if (nv_ram.calib_error_code = DMK_PULLED){
					command_buffer[3]='d';	//calibration error dmk pulled
					command_buffer[4]='m';
					command_buffer[5]='k';
				}
				else if (nv_ram.calib_error_code = TENTHS_LOW){
					command_buffer[3]='t';	//calibration error tenths low
					command_buffer[4]='l';
					command_buffer[5]='o';
				}
				else if (nv_ram.calib_error_code = ZERO_TRANS_MAX){
					command_buffer[3]='z';	//calibration error tenths low
					command_buffer[4]='r';
					command_buffer[5]='o';
				}
				else{
					command_buffer[3]='r';	//general calibration error
					command_buffer[4]='r';
					command_buffer[5]=' ';
				}
				if (calib_error_count == 10)
				{
					calib_error_count = 0;
					com_err = NO_ERROR; //no error
					nv_ram.calib_error_code = NO_ERROR; //no error
				}	
				respond=1;
			}
			else if (com_err & BOTTOM_SEEKING){
			//******************BAD CALIBRATION COMMAND***********************************//
				command_buffer[1]='b';	//received I w c a
				command_buffer[2]='o';	//but did not receive l i after that
				command_buffer[3]='t';	
				command_buffer[4]='t';
				command_buffer[5]='m';
				respond=1;
			}
			else{
			//******************UNRECOGNIZED ERROR*************************************//
				command_buffer[1]='u';	//un-
				command_buffer[2]='r';	//recognized
				command_buffer[3]='e';	//error
				command_buffer[4]='r';
				command_buffer[5]='r';
				respond=1;
			}
			
			if (respond)
			{
			//******************RESPONSE***********************************************//
				LED5_Invert();
				for(i=0;i<COMMAND_SIZE;i++)
				{
					fifo_pop( &rxFifo, &test_ch );
				}
				for(i=0;i<COMMAND_SIZE-2;i++)
				{
					command_buffer[i] &= ~0x80;
				}
				test_crc = CRC(command_buffer,COMMAND_SIZE-2);
				command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
				command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
				my_uart_write(command_buffer, COMMAND_SIZE);
				
				//-----------------------------------------037 added for diagnostic-----------------------------------------------
/*				command_buffer[0]='D';
				command_buffer[1]='B';
				
				temp_int = get_trans_count();
				temp_hb = high7bits_of_14bit(temp_int);
				temp_lb = low7bits_of_14bit(temp_int);
				command_buffer[2]=temp_hb;
				command_buffer[3]=temp_lb;
				
				temp_int = get_trans_des();
			//	temp_int = get_no_trans_count();
				temp_hb = high7bits_of_14bit(temp_int);
				temp_lb = low7bits_of_14bit(temp_int);
				command_buffer[4]=temp_hb;
				command_buffer[5]=temp_lb;
				
				for(i=0;i<COMMAND_SIZE;i++)
				{
					fifo_pop( &rxFifo, &test_ch );
				}
				for(i=0;i<COMMAND_SIZE-2;i++)
				{
					command_buffer[i] &= ~0x80;
				}
				test_crc = CRC(command_buffer,COMMAND_SIZE-2);
				command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
				command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
				my_uart_write(command_buffer, COMMAND_SIZE);
				
				//-----------------------------------------------added for diagnostic----------------------------------------------
*/
			}
		}
	}
	else {//didn't get the right address
		fifo_pop( &rxFifo, &test_ch );	//pop one byte and try again
	}
}

//	if ( rxFifo.bytes >= COMMAND_SIZE )	//see if there are enough bytes in the fifo to contain a command
//	{
//		if (fifo_peek( &rxFifo, 0 )==UART_ADDRESS)	//peak in the fifo and see if the next byte is the address
//		{
//			//check for valid commands
//			for (i=0;i<COMMAND_SIZE;i++)	//load a command byte in the buffer
//			{
//				command_buffer[i]=fifo_peek( &rxFifo, i );
//			}
//			
//			test_crc = CRC(command_buffer,COMMAND_SIZE-2);
//			test_crc_hb = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
//			test_crc_lb = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
//						
//			if (command_buffer[0]==UART_ADDRESS && 
//				command_buffer[1]==C_UART_WRITE	&&
//				command_buffer[2]=='d'          &&
//				command_buffer[3]=='i'    		)
//			{
//				if(	command_buffer[6]==test_crc_hb	&&  
//					command_buffer[7]==test_crc_lb	)
//				//ignore command_buffer[4]
//				{	//valid write desired incline
//					if(get_desired_incline()!= command_buffer[5])
//					{
//						set_desired_incline(command_buffer[5]);
//					}
//					com_err = 0;			//CRC is valid and command is valid
//				}
//				else//checksum is bad
//				{
//					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
//				}
//			}
//			/*else if (command_buffer[0]==UART_ADDRESS && 
//					 command_buffer[1]==C_UART_READ	 &&
//				     command_buffer[2]=='d'          &&
//				     command_buffer[3]=='i'    		 )
//			{
//				if(	command_buffer[6]==test_crc_hb	&&  
//					command_buffer[7]==test_crc_lb	)
//				{	//valid read desired incline
//					command_buffer[4]=0;
//					command_buffer[5]=get_desired_incline();
//					//end put code here
//					checksum_err=0;
//					
//				}
//				else//checksum is bad
//				{
//					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
//				}
//			}*/
//			else
//			{
//				//didn't get the right first four bytes or didn't get a valid command
//				com_err |= COMMAND_ERR;
//			}
//			//RESPONSE
//			if(com_err)
//			{
//				if(com_err & COMMAND_ERR)			
//				{	//didn't get the right first 4 bytes
//					//ERROR CONDITION
//					fifo_pop( &rxFifo, &test_ch );  //pop one byte and try again
//				}
//				else if (com_err & CRC_ERR)
//				{	//CRC didn't match, but first 4 bytes did.
//					//ERROR CONDITION
//					fifo_pop( &rxFifo, &test_ch );  //pop one byte and try again
//					command_buffer[2]='n';//'d'
//					command_buffer[3]='v';//'i'
//					command_buffer[4]=test_crc_hb;//hb crc
//					command_buffer[5]=test_crc_lb;//lb crc
//					test_crc = CRC(command_buffer,COMMAND_SIZE-2);
//					command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
//					command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
//					my_uart_write(command_buffer, COMMAND_SIZE);
//				}
//			}
//			else if (com_err==0)
//			{	//received the command, pop 8 bytes and get ready for the next command
//				//RECEIVED THE VALID COMMAND
//				for(i=0;i<COMMAND_SIZE;i++)
//				{
//					fifo_pop( &rxFifo, &test_ch );
//				}
//				test_crc = CRC(command_buffer,COMMAND_SIZE-2);
//				command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
//				command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
//				my_uart_write(command_buffer, COMMAND_SIZE);
//				LED3_Invert();		//Toggle every time a valid command is received.
//			}
//		}
//		else
//		{	//didn't get the right address
//			fifo_pop( &rxFifo, &test_ch );	//pop one byte and try again
//		}
//	}



/*void check_uart ( void )
{
	BYTE i, test_ch, test_crc_hb, test_crc_lb, checksum_err;
	unsigned int test_crc,temp_int;
	
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
		
	for (i=0;i<COMMAND_SIZE;i++)	//clear the command_buffer
	{
		command_buffer[i]=0;
	}
	test_crc=0;						//clear test_crc
		
	if ( rxFifo.bytes >= COMMAND_SIZE )	//see if there are enough bytes in the fifo to contain a command
	{
		
		if (fifo_peek( &rxFifo, 0 )==UART_ADDRESS)	//peak in the fifo and see if the next byte is the address
		{
			//check for valid commands
			for (i=0;i<COMMAND_SIZE;i++)	//load a command byte in the buffer
			{
				command_buffer[i]=fifo_peek( &rxFifo, i );
			}
			
			test_crc = CRC(command_buffer,COMMAND_SIZE-2);
			test_crc_hb = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
			test_crc_lb = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
						
			if (command_buffer[0]==UART_ADDRESS && 
				command_buffer[1]==C_UART_WRITE	&&
				command_buffer[2]=='d'          &&
				command_buffer[3]=='i'    		)
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid write desired incline
					//ATT if (get_calibrating_incline())
					//ATT {
					//ATT	com_err |= CAL_INC;			//calibrating incline
					//ATT}
					//ATTelse
					//ATT{
						if(get_desired_incline()!=command_buffer[5])
						{
							set_desired_incline(command_buffer[5]);
						}
						com_err =0;			//CRC is valid and command is valid
					//ATT}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_READ	 &&
				     command_buffer[2]=='d'          &&
				     command_buffer[3]=='i'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					//ATTif (get_calibrating_incline())
					//ATT{
					//ATT	com_err |= CAL_INC;			//calibrating incline
					//ATT}
					//ATTelse
					//ATT{
						command_buffer[4]=0;
						command_buffer[5]=get_desired_incline();
					//end put code here
						checksum_err=0;
					
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
		/*	else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_WRITE &&
				     command_buffer[2]=='s'          &&
				     command_buffer[3]=='i'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					if (get_calibrating_incline())
					{
						com_err |= CAL_INC;			//calibrating incline
					}
					else
					{
						stop_inc();
					//end put code here
						checksum_err=0;
					}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			
			else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_READ	 &&
				     command_buffer[2]=='c'          &&
				     command_buffer[3]=='i'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					if (get_calibrating_incline())
					{
						com_err |= CAL_INC;			//calibrating incline
					}
					else
					{
						command_buffer[4]=0;
						command_buffer[5]=get_current_incline();
					//end put code here
						checksum_err=0;
					}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_WRITE &&
				     command_buffer[2]=='t'          &&
				     command_buffer[3]=='m'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					if (get_calibrating_incline())
					{
						com_err |= CAL_INC;			//calibrating incline
					}
					else
					{
						temp_int  = command_buffer[4];
						temp_int <<= 8;
						temp_int += command_buffer[5];
						//this should force a calibration if it gets a -2
					//end put code here
						checksum_err=0;
					}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_READ	 &&
				     command_buffer[2]=='t'          &&
				     command_buffer[3]=='m'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					if (get_calibrating_incline())
					{
						com_err |= CAL_INC;			//calibrating incline
					}
					else
					{
						command_buffer[4]= (BYTE)get_trans_max()>>8;
						command_buffer[5]= (BYTE)get_trans_max();
					//end put code here
						checksum_err=0;
					}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			else if (command_buffer[0]==UART_ADDRESS && 
					 command_buffer[1]==C_UART_READ	 &&
				     command_buffer[2]=='c'          &&
				     command_buffer[3]=='v'    		 )
			{
				if(	command_buffer[6]==test_crc_hb	&&  
					command_buffer[7]==test_crc_lb	)
				{	//valid read desired incline
					//put code here
					if (get_calibrating_incline())
					{
						com_err |= CAL_INC;			//calibrating incline
					}
					else
					{
						command_buffer[4]=0;
						command_buffer[5]=VERSION;	//Return the code version
					//end put code here
						checksum_err=0;
					}
				}
				else//checksum is bad
				{
					com_err |= CRC_ERR;			//CRC is not valid so set the error bit high
				}
			}
			else
			{
				//didn't get the right first four bytes or didn't get a valid command
				com_err |= COMMAND_ERR;
			}
			//RESPONSE
			if(com_err)
			{
				if(com_err & COMMAND_ERR)			
				{	//didn't get the right first 4 bytes
					//ERROR CONDITION
					fifo_pop( &rxFifo, &test_ch );  //pop one byte and try again
				}
				else if (com_err & CRC_ERR)
				{	//CRC didn't match, but first 4 bytes did.
					//ERROR CONDITION
					fifo_pop( &rxFifo, &test_ch );  //pop one byte and try again
					command_buffer[2]='n';//'d'
					command_buffer[3]='v';//'i'
					command_buffer[4]=test_crc_hb;//hb crc
					command_buffer[5]=test_crc_lb;//lb crc
					test_crc = CRC(command_buffer,COMMAND_SIZE-2);
					command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
					command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
					my_uart_write(command_buffer, COMMAND_SIZE);
				}
				/*else if (com_err & CAL_INC)
				{	//got a good command but I am calibrating incline
					for(i=0;i<COMMAND_SIZE;i++)
					{
						fifo_pop( &rxFifo, &test_ch );
					}
					command_buffer[1]='c';
					command_buffer[2]='a';//'d'
					command_buffer[3]='l';//'i'
					command_buffer[4]='i';
					command_buffer[5]='b';
					test_crc = CRC(command_buffer,COMMAND_SIZE-2);
					command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
					command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
					my_uart_write(command_buffer, COMMAND_SIZE);
				
				}
			}
			else if (com_err==0)
			{	//received the command, pop 8 bytes and get ready for the next command
				//RECEIVED THE VALID COMMAND
				for(i=0;i<COMMAND_SIZE;i++)
				{
					fifo_pop( &rxFifo, &test_ch );
				}
				test_crc = CRC(command_buffer,COMMAND_SIZE-2);
				command_buffer[6] = ((BYTE)(test_crc>>8)) & ~0x80;	//clear the top bit because I only have 7bit communication
				command_buffer[7] = ((BYTE)(test_crc)) & ~0x80;	//clear the top bit because I only have 7bit communication
				my_uart_write(command_buffer, COMMAND_SIZE);
				LED3_Invert();		//Toggle every time a valid command is received.
			}
		}
		else
		{	//didn't get the right address
			fifo_pop( &rxFifo, &test_ch );	//pop one byte and try again
		}
	}
} */





/*void test_uart ( void )
{
	BYTE test_ch;
	BYTE original_number_of_bytes;
	BYTE i;

	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	
	if ( rxFifo.bytes >= COMMAND_SIZE )
	{
		//original_number_of_bytes =rxFifo.bytes;
		
		i=32;
		while (i)
		{
			M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
			if (!fifo_empty( &rxFifo ))
			{
				fifo_pop( &rxFifo, &test_ch );
				test_array[32-i]=test_ch;// | 0x80;
				//test_array[32-i]=rxFifo.bytes | 0x80;
				//test_array[32-i]=0x80;
				i--;
			}
			else
			{
				//test_array[32-i]=0x01 | 0x80;
				//test_array[32-i]=rxFifo.bytes | 0x80;
				test_array[32-i]=0;//0x80;		//This is a 0
				i--;
			}
		}
		//test_array[0]=original_number_of_bytes | 0x80;
		//test_array[0]=rxFifo.bytes | 0x80;
		//PRT1DR |= RS485_DE_MASK;	//SET HIGH
		//PRT1DR |= RS485_RE_MASK;	//SET HIGH
		//UART_Write(test_array, sizeof(test_array)) ;
		//my_uart_write(test_array, sizeof(test_array)) ;
		my_uart_write(test_array, sizeof(test_array));
		//UART_Write(test_array, 8) ;
		//PRT1DR &= ~RS485_DE_MASK;	//SET LOW
		//PRT1DR &= ~RS485_RE_MASK;	//SET LOW

	}
	
}		*/

void my_uart_write( BYTE * fsend_array, BYTE muw_bcnt ) 
{
	BYTE i;
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	
	UART_IntCntl(UART_DISABLE_RX_INT);   // Disable RX interrupts  
	
	PRT1DR |= RS485_DE_MASK;	//SET HIGH
	PRT1DR |= RS485_RE_MASK;	//SET HIGH
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	//UART_Write(fsend_array, bcnt) ;
	for (i=0 ; i < muw_bcnt ; i++)
	{
		//UART_SendData( *(fsend_array+i) | 0x80 );	//set the top bit high
		UART_SendData( (fsend_array[i]) | 0x80 );	//set the top bit high
		//UART_SendData( 0 );	//set the top bit high
		while(!(UART_bReadTxStatus() & UART_TX_COMPLETE))
			M8C_ClearWDT;  //RESET_WDT; //feed the watch dog; 
		//while((UART_bReadTxStatus() && (UART_TX_BUFFER_EMPTY | UART_TX_COMPLETE))!= 0x30)		//jump out of this when the stop bit is sent
		//{
		//	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
		//}
	}
	PRT1DR &= ~RS485_DE_MASK;	//SET LOW
	PRT1DR &= ~RS485_RE_MASK;	//SET LOW
	UART_IntCntl(UART_ENABLE_RX_INT);   // Enable RX interrupts  \
	
}


/*
BYTE check_address ( FIFO * fifo, BYTE * command_buffer_ptr )
{ //returns true if the first item on the fifo is a valid address
  //pops a value off the stack.
  	BYTE ch;
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	fifo_pop( fifo, &ch );	
	if (ch==UART_ADDRESS)
	{
		*(command_buffer_ptr + 0) = ch;

	//	return ADDRESS_GOOD;
		return 1;
		
	}
	else
	{
		*(command_buffer_ptr + 0) = ch;
		return 0;
	}
}
  


BYTE check_read_write ( FIFO * fifo, BYTE * command_buffer_ptr )
{
	BYTE ch;
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	if (fifo_peek( fifo, 0 )==C_UART_READ)
	{
		fifo_pop( fifo, &ch );
		*(command_buffer_ptr + 1) = ch;
		//return READ_VALID;
		return 1;
		
	}
	else if (fifo_peek( &rxFifo, 0 )==C_UART_WRITE)
	{
		fifo_pop( fifo, &ch );
		*(command_buffer_ptr + 1) = ch;
		//return WRITE_VALID;
		return 1;
	}
	else
	{
		return 0;
	}
}
BYTE check_if_incline ( FIFO * fifo, BYTE * command_buffer_ptr )
{//check if it is for incline,  if it is store the value too
//	BYTE ch;
//	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
//	if ((D_INC_HIGH == fifo_peek( &rxFifo, 0 )) &&
//	    (D_INC_LOW  == fifo_peek( &rxFifo, 1 )) )
//	{
//		fifo_pop( fifo, &ch );			
//		*(command_buffer_ptr + 2) = ch;	
//		fifo_pop( fifo, &ch );
//		*(command_buffer_ptr + 3) = ch;
//		fifo_pop( fifo, &ch );			
//		*(command_buffer_ptr + 4) = ch;	//GET THE VALUE
//		fifo_pop( fifo, &ch );
//		*(command_buffer_ptr + 5) = ch;	//GET THE VALUE
//		return TO_INC_VALID;
return 0;

}

BYTE check_crc_valid (FIFO * fifo, BYTE * command_buffer_ptr )
{//verify the crc16 matches
	//do the crc if it matches what was sent then return CRC_VALID
	BYTE ch;
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	/**************THIS IS JUST FOR TEST ATT***************/
/*	fifo_pop( fifo, &ch );			
	*(command_buffer_ptr + 6) = ch;	
	fifo_pop( fifo, &ch );
	*(command_buffer_ptr + 7) = ch;
	//return CRC_VALID;
	return 1;
	/**************THIS IS JUST FOR TEST ATT***************/
	
//}*/

unsigned int CRC(unsigned char *data, unsigned int len)
{
	unsigned int i, crc=0xFFFF;
	
	while( len-- )
	{
		crc ^= *data++;
		for( i=0; i<8; ++i )
		{
			M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
			if( crc&0x01 )
				crc = (crc>>1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}
	
	return crc;
}

// --------------------------------------------------------------
// Interrupt handlers below
// --------------------------------------------------------------

void uart0_rx_isr( void )		//#pragma interrupt_handler uart0_rx_isr  in myuart.h
{
	BYTE	ch;
	
	M8C_ClearWDT;  //RESET_WDT; //feed the watch dog
	
	ch = UART_bReadRxData();
	//if (UART_bReadRxStatus() & UART_RX_REG_FULL)	//is there really something to get
	//{

		ch &= ~0x80;			//clear the top bit
		if (!fifo_full(&rxFifo))
		{
			
			if( ch == UART_ADDRESS )
			{
				rx_isr_timer=COMMAND_SIZE;
			}
			if( rx_isr_timer )
			{
				fifo_push(&rxFifo, ch);
				if( rx_isr_timer > 0)
					rx_isr_timer--;
				LED2_Invert();
			}
			//fifo_push(&rxFifo, 0x01);
			
		}
	//	else
	//	{
	//		LED2_Invert();
	//	}
	//Look at disabling receive for a while till you think there should be another command???
		//else
		//{
		//	LED1_Invert();
		//	LED2_Invert();
		//	LED3_Invert();
		//	LED4_Invert();
		//	LED6_Invert();
		//	
		//}

		//fifo_push( &rxFifo, ch );

	//}
	
	return;
}

unsigned char high7bits_of_14bit(unsigned int f_int)		//strips off the high 7 bits of the 14 bit number and returns them as a byte
{
	BYTE result;								//the top two bits of this 16 bit number have to be 0
	result = f_int>>7;							//the high byte is bits 14 through 8 of the 16bit integer
	result &=  ~0x80;							//make sure the top bit is low
	return result;
}

unsigned char low7bits_of_14bit(unsigned int f_int)		//strips off the low 7 bits of the 14 bit number and returns them as a byte
{
	BYTE result;								//the top two bits of this 16 bit number have to be 0
	result = f_int & ~0x80;						//the low byte is bits 7 through 1 of the 16bit integer
												//set the top bit low.
	return result;
}

unsigned int convert_two_7bit_to_int(unsigned char f_hb, unsigned char f_lb)
{
	unsigned int f_int=0;
	f_lb &= ~0x80;								//make sure the top bit is 0
	f_int = f_hb;
	f_int <<= 7;
	f_int += f_lb;
	return f_int;
}


/*************************************************
 *  get_calibrate_incline_now
 *  
 *************************************************/
unsigned char get_calibrate_incline_now(void)
{
	return calibrate_incline_now;
}

/*************************************************
 *  set_calibrate_incline_now
 *  
 *************************************************/
void set_calibrate_incline_now(BYTE f_value)
{
	calibrate_incline_now = f_value;
}
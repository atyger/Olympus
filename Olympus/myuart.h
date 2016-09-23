/*************************************************
 *  uart.h
 *  
 *************************************************/


//void unsigned char gbl_transmit;
//extern BYTE uart_fifo[];
//extern BYTE read_cnt;
//extern BYTE write_cnt;
#include "fifo.h"

#define FIFO_SIZE 16

#define FIFO_OVERRUN 01

#define COMMAND_SIZE 8
//COMMAND BYTE 1
#define UART_ADDRESS 'I'
//COMMAND BYTE 2
#define C_UART_READ 'r'
#define C_UART_WRITE 'w'
//COMMAND BYTE 3
//#define D_INC_HIGH 0x0
//COMMAND BYTE 4
//#define D_INC_LOW 0x0B
//COMMAND BYTE 5  Value High Byte
#define VAL_HIGH_B 0x00
//COMMAND BYTE 6  Value Low Byte
//COMMAND BYTE 7  CRC16 High Byte
//COMMAND BYTE 8  CRC16 Low Byte
#pragma interrupt_handler uart0_rx_isr
void uart0_rx_isr( void );

void myuart_init ( void );
void check_uart ( void );
//void test_uart ( void );
BYTE check_address ( FIFO *, BYTE *);
BYTE check_read_write ( FIFO *, BYTE *);
BYTE check_if_incline ( FIFO *, BYTE *);
BYTE check_crc_valid ( FIFO *, BYTE *);
void received_byte(BYTE thisbyte);
void my_uart_write( BYTE * , BYTE ) ;
unsigned int CRC( unsigned char *, unsigned int );

unsigned char high7bits_of_14bit(unsigned int);
unsigned char low7bits_of_14bit(unsigned int);
unsigned int convert_two_7bit_to_int(unsigned char, unsigned char);

unsigned char get_calibrate_incline_now(void);
void set_calibrate_incline_now(BYTE);




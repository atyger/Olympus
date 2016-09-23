#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "PSoCGPIOINT.h"
#include "fifo.h"
#include "constants.h"



void fifo_init( FIFO * fifo, BYTE  *  buffer, WORD size )
{
	fifo->buffer = (BYTE  *) buffer;
	fifo->size = size;
	fifo->head = 0;
	fifo->tail = 0;
	fifo->bytes = 0;
}


BOOL fifo_empty( FIFO * fifo )
{
	if (fifo->bytes == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL fifo_full( FIFO * fifo )
{
	if (fifo->bytes >= fifo->size)
		return TRUE;
	else
		return FALSE;
}


void fifo_push( FIFO * fifo, BYTE ch )
{
	M8C_ClearWDT;
	if ((fifo->bytes >= fifo->size))	//fifo is full
	{
		//cannot push
	}
	else								//fifo is not full
	{
		M8C_ClearWDT;
		fifo->buffer[fifo->tail] = ch;
	//	M8C_DisableGInt;//DISABLE_INTERRUPTS;
		fifo->bytes++;
	//	M8C_EnableGInt; //RESTORE_INTERRUPTS;
		fifo->tail++;
		if ( fifo->tail >= fifo->size )
			fifo->tail = 0;
	}
}


BOOL fifo_pop( FIFO * fifo, BYTE * return_ch )
{
	if (fifo->bytes == 0)
	{
		return FALSE;
	}
	else
	{
		*return_ch = fifo->buffer[fifo->head];
	//	M8C_DisableGInt;//DISABLE_INTERRUPTS;
		fifo->bytes--;
	//	M8C_EnableGInt; //RESTORE_INTERRUPTS;
		fifo->head++;
		if ( fifo->head >= fifo->size )
			fifo->head = 0;
		return TRUE;
	}
}


BYTE fifo_peek( FIFO * fifo, WORD position )
{
	WORD	w_pos;
	
	if ( (position >= fifo->bytes) )
		return 0x00;
	
	w_pos = fifo->head + position;
	w_pos %= fifo->size;
		
	return fifo->buffer[w_pos];
}

//void fifo_init( FIFO * fifo, BYTE  *  buffer, WORD size )
//{
//	fifo->buffer = (BYTE  *) buffer;
//	fifo->size = size;
//	fifo->head = 0;
//	fifo->tail = 0;
//	fifo->bytes = 0;
//}
//
//
//BOOL fifo_empty( FIFO * fifo )
//{
//	if ( NULL == fifo->buffer )
//		return TRUE;
//	
//		
//	return ( 0 == fifo->bytes );
//}
//
//
//BOOL fifo_full( FIFO * fifo )
//{
//	if ( NULL == fifo->buffer )
//		return FALSE;
//		
//	return ( fifo->bytes >= fifo->size );
//}
//
//
//void fifo_push( FIFO * fifo, BYTE ch )
//{
//	M8C_ClearWDT;
//	if ( NULL != fifo->buffer )
//	{
//		M8C_ClearWDT;
//		fifo->buffer[fifo->tail] = ch;
////ra		M8C_DisableGInt;//DISABLE_INTERRUPTS;
//		fifo->bytes++;
////ra		M8C_EnableGInt; //RESTORE_INTERRUPTS;
//		fifo->tail++;
//		if ( fifo->tail >= fifo->size )
//			fifo->tail = 0;
//	}
//}
//
//
//BOOL fifo_pop( FIFO * fifo, BYTE * return_ch )
//{
//	if ( (NULL == fifo->buffer) || (0 == fifo->bytes) )
//		return FALSE;
//	else
//	{
//		*return_ch = fifo->buffer[fifo->head];
////ra		M8C_DisableGInt;//DISABLE_INTERRUPTS;
//		fifo->bytes--;
//		//ATT if ( fifo->bytes == 0xFFFF )
//		//ATT 	DEBUG_TRAP;
////ra		M8C_EnableGInt; //RESTORE_INTERRUPTS;
//		fifo->head++;
//		if ( fifo->head >= fifo->size )
//			fifo->head = 0;
//		return TRUE;
//	}
//}
//
//
//BYTE fifo_peek( FIFO * fifo, WORD position )
//{
//	WORD	w_pos;
//	
//	if ( (NULL == fifo->buffer) || (position >= fifo->bytes) )
//		return 0x00;
//	
//	w_pos = fifo->head + position;
//	w_pos %= fifo->size;
//		
//	return fifo->buffer[w_pos];
//}
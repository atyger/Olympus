//----------------------------------------------------------------------/
//  FILE        :fifo.h													/
//  DESCRIPTION :														/
//----------------------------------------------------------------------/

#ifndef _FIFO_H
#define _FIFO_H

#include "m8c.h"
//#include "rtetypes.h"
//#include "usb_common.h"


typedef	struct {
	BYTE	*  buffer;
	WORD	size;
	WORD	head, tail;
	WORD	bytes;
} FIFO;


void fifo_init( FIFO * fifo, BYTE * buffer,	WORD size );
BOOL fifo_empty( FIFO * fifo );
BOOL fifo_full( FIFO * fifo );
void fifo_push( FIFO * fifo, BYTE ch );
BOOL fifo_pop( FIFO * fifo, BYTE * return_ch );
BYTE fifo_peek( FIFO * fifo, WORD position );

#endif	// #define _FIFO_H

/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *	Module :  EGA.SYS Device driver interface		
 *         
 *	$Log:   C:/c/gw/vcs/egasys.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:10   dz
 *      Initial revision.
 *                                                     
 *	   Rev 1.0	03 Jan 90            			
 *	Initial revision.				
 *                                                              
 *								
\**********/

#include <style.h>

#include "ega.h"

#include <dos.h>

/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/

typedef	union REGS	regs;
typedef struct SREGS	sregs;

#define	ecall( f, r )		((r)->h.ah = f, int86( 0x10, (r), (r) ))
#define	ecallx( f, r, s )	((r)->h.ah = f, int86x( 0x10, (r), (r), (s) ))

#define	assert_att		if( !Ega_Sys_attached )	return

bool	Ega_Sys_attached	= No;

bool
es_attach( void ) {
	regs	r;

	r.x.bx = 0;		/*	Clear version		*/
	ecall( ES_GETVER, &r );	/*	Get version number	*/

	if( r.x.bx == 0 ) 	/*	No driver found		*/
		return Err;

	Ega_Sys_attached = Yes;	/*	Ok, driver exist.	*/
	return Ok;
	}




/****************************************************************************
			Registers access
****************************************************************************/


void
es_get_port( char far *buf, uint port, uint from, uint num ) {
	regs	r;
	sregs	s;

	assert_att;

	r.h.ch = from;
	r.h.cl = num;
	r.x.dx = port;

	segread( &s );
	s.es = FP_OFF( buf );
	r.x.bx = FP_SEG( buf );

	ecallx( ES_GETPORT, &r, &s );
	}




void
es_put_port( char far *buf, uint port, uint from, uint num ) {
	regs	r;
	sregs	s;

	assert_att;

	r.h.ch = from;
	r.h.cl = num;
	r.x.dx = port;

	segread( &s );
	s.es = FP_OFF( buf );
	r.x.bx = FP_SEG( buf );

	ecallx( ES_PUTPORT, &r, &s );
	}






































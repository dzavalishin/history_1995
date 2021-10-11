/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *	Module : Mouse driver interface.                          
 *             
 *	$Log:   C:/c/gw/vcs/mouse.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:36   dz
 *      Initial revision.
 *                                                  
 *	   Rev 1.0     06.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\*************/


#include <stdlib.h>
#include <graphics.h>
#include <dos.h>


#include "style.h"



#define MOUSE_C
#include "mouse.h"


void	mstdcurs( void );


int		Nbuttons = 0;


char	Mouse_attached = No;


static void
mi( union REGS *r, enum mouse_commands cmd ) {
	if( !Mouse_attached )	return;
	r->x.ax = cmd;
	int86( MOUSE_INT, r, r );
	}


/**Doc************************************************************************
 *	Function: mreset()      		Header: mouse.h
 *
 *	Parameters:			None.
 *
 *	Return value:
 *      		Ok	- Mouse driver detected.
 *			Err	- Mouse driver not found.
 *
 *	Description:
 *		Looks for a mouse driver, initializes it
 *		and sets up two global variables :
 *			Mouse_attached	- 'Yes' if attached, else 'No'.
 *			Nbuttons	- number of mouse buttons ( 2 or 3 ).
 *
**/


bool
mreset( void ) {
	void interrupt	(*mint)();
	union REGS	r;

	mint = getvect( MOUSE_INT );

	/* Empty or pointing to IRET vector ?	*/
	if( mint == NULL || *((char *)mint) == 0xCF )
		return Err;

	Mouse_attached = Yes;

	r.x.ax = 0;
	mi( &r, Mouse_reset );

	if( r.x.ax == 0 ) {
		Mouse_attached = No;
		return Err;
		}

	Nbuttons = r.x.bx;

	return Ok;
	}



void
mtextcursor( unsigned char chr, unsigned char color ) {
	union REGS	r;

	r.x.bx = 0;
	if( color == 0 ) {
		r.h.ch = 0xFF;
		r.h.dh = 0x7F;
		}
	else {
		r.h.ch = 0x00;
		r.h.dh = color;
		}


	if( chr == 0 ) {
		r.h.cl = 0xFF;
		r.h.dl = 0x00;
		}
	else {
		r.h.cl = 0;
		r.h.dl = chr;
		}

	mi( &r, Mouse_marker );

	}






void
mdisplay( bool on ) {
	union REGS	r;

	mi( &r, on ? Mouse_disp_on : Mouse_disp_off );
	}

bool
mcheckin( int x1, int y1, int x2, int y2 ) {
	union REGS	r;

	if( !Mouse_attached )	return No;

	mi( &r, Mouse_get_pos );

	if(
		r.x.cx < x1 ||
		r.x.cx > x2 ||
		r.x.dx < y1 ||
		r.x.dx > y2
		)		return No;
	return Yes;
	}

void
mgetpos( int *x, int *y, int *bat ) {
	union REGS	r;

	if( !Mouse_attached ) {
		*bat = 0;
		*x = 0;
		*y = 0;
		return;
		}

	mi( &r, Mouse_get_pos );
	*x = r.x.cx;
	*y = r.x.dx;
	*bat = r.x.bx;
	}



void
msetpos( int x, int y ) {
	union REGS	r;

	r.x.cx = x;
	r.x.dx = y;
	mi( &r, Mouse_set_pos );
	}

void
mxregion( int x1, int x2 ) {
	union REGS	r;

	r.x.dx = x1;
	r.x.cx = x2;
	mi( &r, Mouse_X_region );
	}



void
myregion( int y1, int y2 ) {
	union REGS	r;

	r.x.dx = y1;
	r.x.cx = y2;
	mi( &r, Mouse_Y_region );
	}


void
mdelta( int *x, int *y ) {
	union REGS	r;

	mi( &r, Mouse_delta );
	*x = r.x.cx;
	*y = r.x.dx;
	}




bool
mpress( int button, int *x, int *y, int *but ) {
	union REGS	r;

	if( !Mouse_attached ) {
		*but = 0;
		*x = 0;
		*y = 0;
		return No;
		}

	if( button+1 > Nbuttons )
		return No;

	r.x.bx = button;
	mi( &r, Mouse_press_latch );

	if( r.x.bx == 0 )	return No;

	*x = r.x.cx;
	*y = r.x.dx;

	*but = r.x.bx;

	return Yes;
	}


bool
mrelease( int button, int *x, int *y, int *but ) {
	union REGS	r;

	if( !Mouse_attached ) {
		*but = 0;
		*x = 0;
		*y = 0;
		return No;
		}

	if( button+1 > Nbuttons )
		return No;

	r.x.bx = button;
	mi( &r, Mouse_release_latch );

	if( r.x.bx == 0 )	return No;

	*x = r.x.cx;
	*y = r.x.dx;

	*but = r.x.bx;

	return Yes;
	}




void
msetfunc( void far (*func)(), uchar mask ) {
	union REGS	r;
	struct SREGS	s;


	if(!Mouse_attached)	return;



	r.x.ax = Mouse_func;
	r.x.cx = mask;

	r.x.dx = FP_OFF(func);

	s.ds = s.es = FP_SEG(func);

	int86x( MOUSE_INT, &r, &r, &s );
	}



void
reset_latches( void ) {
	int d;

	mpress( 0, &d, &d, &d );
	mpress( 1, &d, &d, &d );
	mpress( 2, &d, &d, &d );
	mrelease( 0, &d, &d, &d );
	mrelease( 1, &d, &d, &d );
	mrelease( 2, &d, &d, &d );
	}








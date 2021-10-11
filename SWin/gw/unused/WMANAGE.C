/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Central module of windows.                       
 *
 *	$Log:   C:/c/gw/vcs/wmanage.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:42   dz
 *      Initial revision.
 *                                                               
 *     Revision 1.1     20.08.89                                 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/


#include "local.h"
#include "ega.h"

#include <mem.h>
#include <dos.h>

#include "mouse.h"



bool
gwregister( gwin *w ) {
	assert_init;

	if( !w->nobuf ) {
		if(_EGA_alloc( w->buf, w->xs, w->ys, &(w->planesize) ))
			return Err;
		w->balloc = True;
		}

	w->nactive = 0;
	w->must_be_displayed = False;
	w->is_displayed = False;
	w->haveimage = False;

	return Ok;
	}



gwin *
gwcreate( int x, int y, int xs, int ys ) {
	gwin *new;

	if( (new = malloc( sizeof( gwin ) + 40)) == NULL )
		return NULL;

	new->salloc = True;

	new->xb = x;
	new->yb = y;
	new->xs = xs;
	new->ys = ys;

	new->nobuf = False;

	if( gwregister( new ) == Err ) {
		free( new );
		return NULL;
		}

	return new;
	}

bool
gwdestroy( gwin *w ) {
	assert_init;

	if( w->nactive )	return Err;


	if( w->balloc )
		_EGA_free( w->buf );
	w->balloc = False;


	if( w->salloc )
		free( w );

	w->salloc = False;


	return Ok;
	}



/**
 *	The next function can be called only before gwregister().
**/


void
gwnobuf( gwin *w ) {
	w->nobuf = True;
	}


/**
 *	void	gw_swap_images( gwin *w, bool restore_only );
 *
 *	This function swaps image saved in the buffer with
 *	the contents of the given window. If buffer has no
 *	image saved in it this function moves  image  only
 *	from screen to buffer. If parameter 'restore_only'
 *	is equal to 'True', function moves image only from
 *	buffer to screen. This mode is used in the 'gwperm-
 *	off' function ( wonoff.c ).
**/



void
gw_swap_images( gwin *w, bool restore_only ) {
/*	int	i;	*/

	if( w->nobuf )	return;

	mouse_off();

	if( restore_only ) {
		_EGA_put( w->buf, w->xb, w->yb, w->xs, w->ys );
		w->haveimage = False;
		}
	else if( w->haveimage ) {
		_EGA_xchg( w->buf,  w->xb, w->yb, w->xs, w->ys );
#if 0
		_EGA_get( XCbuf,  w->xb, w->yb, w->xs, w->ys );
		_EGA_put( w->buf, w->xb, w->yb, w->xs, w->ys );

		for( i = 0; i < 4; i++ )
			movedata( FP_SEG(XCbuf[i]), FP_OFF(XCbuf[i]), FP_SEG(w->buf[i]), FP_OFF(w->buf[i]), w->planesize );
#endif
		w->haveimage = True;
		}
	else {
		_EGA_get( w->buf, w->xb, w->yb, w->xs, w->ys );
		w->haveimage = True;
		}

	mouse_on();

	}

void
gw_set_win_pars( gwin *w ) {

	setviewport(
		 Xbase = w->xb,
		 Ybase = w->yb,
		 w->xb + ( Xend = w->xs) - 1,
		 w->yb + ( Yend = w->ys) - 1,
		 True
		 );

	}





















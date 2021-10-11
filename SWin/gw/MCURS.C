/***********************************************************************
 *
 *
 *	Font Editor      Copyright (C) 1989 by [DZ]
 *
 *      Graphics mouse cursor emulation.
 *
 *	$Log:   C:/c/gw/vcs/mcurs.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:16   dz
 *      Initial revision.
 *
 *	   Rev 1.0	27.08.89
 *	Initial revision.
 *
**/

#include <dos.h>

#include <stdlib.h>
#include <graphics.h>

#include "style.h"

#include "font.h"
#include "mouse.h"


static void	far mouse_cursor( void );

static int	ox = -1,	oy = -1;
static int	mx,		my;

static uchar	current_mouse_image = 0x41;
static uchar	old_mouse_image;

static int	ndisplay = -1;
static bool	on_screen = False;






static void pascal
out_mouse( int x, int y, uchar sym ) {
	extern font	_mouseimg_font;

	putsymbol( x, y, &_mouseimg_font,
			 sym,
			 3,
			 LIGHTGRAY
			 );
	}

/**Doc************************************************************************
 *	Function: reset_mouse_cursor() 		Header: mouse.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Turns off GW mouse cursor driver.
 *
**/

void
reset_mouse_cursor( void ) {
	msetfunc( NULL, 0 );
	mreset();
	}

/**Doc************************************************************************
 *	Function: init_mouse_cursor()      		Header: mouse.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *              Initializes GW mouse cursor driver.
 *
**/


void
init_mouse_cursor( void ) {
	msetfunc( mouse_cursor, 0xFF );
	atexit( reset_mouse_cursor );
	}

static void
redraw_cursor( int x, int y ) {
	disable();

	if( ox == x && oy == y &&
		old_mouse_image == current_mouse_image ) {
			enable();
			return;
			}

	if( on_screen ) {
		out_mouse( ox, oy, old_mouse_image );
		on_screen = False;
		ox = oy = -1;
		}

	if( ndisplay )  {
		enable();
		return;
		}

	out_mouse( x, y, current_mouse_image );

	ox = x;
	oy = y;
	old_mouse_image = current_mouse_image;
	on_screen = True;
	enable();
	}


/**Doc************************************************************************
 *	Function: mouse_on()      		Header: mouse.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Turns mouse cursor on.
 *
**/


void
mouse_on( void ) {
	int b;

	if( !Mouse_attached )	return;

	disable();

	ndisplay++;

	if( ndisplay == 0 ) {
		ox = oy = -1;
		mgetpos( &mx, &my, &b );
		redraw_cursor( mx, my );
		}
	enable();
	}

/**Doc************************************************************************
 *	Function: mouse_off()      		Header: mouse.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Turns mouse cursor off. If mouse_off was
 *		called 5 times, you must call mouse_on 5
 *		times to turn it on. Mouse must be turned
 *		off each time you are going to draw some-
 *		thing on the screen and overdraw cursor.
 *
**/


void
mouse_off( void ) {
	if( !Mouse_attached )	return;

	disable();
	ndisplay--;
	redraw_cursor( 0, 0 );
	enable();
	}

#ifdef __HUGE__
#	pragma saveregs
#endif

static void far
mouse_cursor( void ) {
	static unsigned char	cause;

#ifndef __HUGE__
	asm	push ds

	asm	push ax
	asm	mov ax, DGROUP
	asm	mov ds, ax
	asm	pop ax
#endif

	cause = _AX;
	mx = _CX;
	my = _DX;

	if( Mouse_attached ) {

		if( /*cause & */0x01 )		/* Cursor pos. changed		*/
			redraw_cursor( mx, my );
		}
#ifndef __HUGE__
	asm	pop dx
#endif


	}

/**Doc************************************************************************
 *	Function: set_mouse_cursor()   		Header: mouse.h
 *
 *	Parameters:
 *		unsigned char		mouse_image_number
 *
 *	Return value:			None.
 *
 *	Description:
 *		Selects a new image for the mouse cursor.
 *		Mouse cursor images are stored in the font
 *		with the name 'mouseimg' linked into the
 *		GW library. Mouse images starting from
 *		number 0x40.
 *
 *
**/


void
set_mouse_cursor( uchar new ) {
	int b;

	disable();
	current_mouse_image = new;
	mgetpos( &mx, &my, &b );
	redraw_cursor( mx, my );
	enable();
	}



















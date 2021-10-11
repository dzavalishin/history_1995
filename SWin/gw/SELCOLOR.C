/**********************   Journal Maker   ************************\
 *
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *     Module : Color menu.
 *
 *	$Log:   C:/c/gw/vcs/selcolor.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:00   dz
 *      Initial revision.
 *
 *	   Rev 1.1     15 Dec 89 [DZ]
 *	Mouse support added.
 *
 *	   Rev 1.0     7 Oct 89
 *	Initial revision.
 *
 *
\***********/


#include "gw.h"
#include "mouse.h"
#include <bios.h>
#include <key.h>



static void
draw_colors( void ) {
	int	i,x,y;

	mouse_off();
	setlinestyle( SOLID_LINE, 0, 0 );
	for( i = 0; i < 16; i++ ) {
		x = ((i%8)+1)*20;
		y = i > 7 ? 40 : 20;
		setfillstyle( SOLID_FILL, i );
		bar( x, y, x+10, y+10 );
		setcolor( WHITE );
		rectangle( x-1, y-1, x+10, y+10 );
		}
	mouse_on();
	}

static void
bord( int pos, int col ) {
	int	x,y;

	mouse_off();
	setlinestyle( SOLID_LINE, 0, 0 );
	x = ((pos%8)+1)*20;
	y = pos > 7 ? 40 : 20;
	setcolor( col );
	rectangle( x-1, y-1, x+10, y+10 );
	mouse_on();
	}

static int	col;
static int	oldcol;

static bool	leave_ok;
static bool	leave_esc;


static void
disp_mark( void ) {
		bord( oldcol, WHITE );
		bord( col, BLACK );
		oldcol = col;
	}





static bool
track_mouse( int x, int y ) {
	bool	good;
	int	i;

	for( i = 0; i <= 15; i++ ) {
		int	cx, cy;

		cx = ((i%8)+1)*20;
		cy = i > 7 ? 40 : 20;

		if( x >= cx && x <= cx+10 && y >= cy && y <= cy+10 ) {
			if( col != i ) {
				col = i;
				disp_mark();
				}
			good = Yes;
			break;
			}
		}

	if( i > 15 )	good = No;

	return good;
	}


static bool
mouse_action( int wx, int wy ) {
	int	x, y, b;
	bool	act = No;

	if( !Mouse_attached )		/* No mouse ?			*/
		return No;		/* Nothing to do at all		*/




	if( mpress( 0, &x, &y, &b ) ) {
		static long	prevtime;
		long		curtime;

		if( !track_mouse( x-wx, y-wy ))
			return No;

		curtime = biostime( 0, 0 );
		if( curtime-prevtime < 9 )
			act = leave_ok = Yes;
		else {
			prevtime = curtime;
			while( mgetpos( &x, &y, &b ), b )
				track_mouse( x-wx, y-wy );
			}
		reset_latches();
		}

	if( mpress( 2, &x, &y, &b ) )
		act = leave_esc = Yes;

	return act;
	}





static bool
keyb_action( void ) {
	if( !bioskey(1) )
		return No;		/*	Nothing is done		*/


	switch( bioskey(0) >> 8 ) {
		case 0x01:		/*	Esc		*/
			leave_esc = Yes;
			break;

		case 0x1C:		/*	Enter		*/
			leave_ok = Yes;
			break;

		case UP:
			col -= 8;
			break;
		case DOWN:
			col += 8;
			break;
		case LEFT:
			col--;
			break;
		case RIGHT:
			col++;
			break;
		}
	return Yes;
        }



/**Doc************************************************************************
 *	Function: bool sel_color()      	Header: gw.h
 *
 *	Parameters:
 *		int *			color 	- Color number to edit.
 *
 *	Return value:
 *		Err			Colour not changed.
 *		Ok			Colour changed.
 *
 *	Description:
 *		Draws a palette of 16 colors, and lets user
 *		to pick one. If *color is less than 0 or
 *		greater than 15, function wremembers last
 *		color selected.
 *
**/


bool
sel_color( int *ucolor ) {
	gwin w = { 160, 60, 200, 75 };


	reset_latches();		/*	Reset mouse latches	*/

	oldcol = 0;
	if( *ucolor >= 0 && *ucolor <= 15 )
		col = *ucolor;

	if( gwregister( &w ) || gwattach( &w )) {
		errmsg("Не могу создать окно");
		return Err;
		}

	gwborder3d( 3, LIGHTGRAY, 3, DARKGRAY, CYAN );

	draw_colors();

	push_bar(" \x18\x19\x1B\x1A - Выбор    Enter - Взять                              ESC - Отказ");

	leave_ok = leave_esc = No;


	while( (!leave_ok) && (!leave_esc) ) {
		disp_mark();
		while(1) {
			if( mouse_action( w.xb, w.yb ) )
				break;

			if( keyb_action() )
				break;
			}

		while( col < 0 )	col += 16;
		while( col > 15 )	col -= 16;

		}

	if( !leave_esc )			/* Not an escape ?	*/
		*ucolor = col;			/* Send him his color	*/

	reset_latches(); /* Reset mouse latches	*/
	pop_bar();

	gwpermdetach();
	gwdestroy( &w );

	return leave_esc ? Err : Ok;
	}







































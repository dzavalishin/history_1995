/**--------------------   Journal Maker   ----------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : Zone selection.
 *
 *	$Log:   C:/c/gw/vcs/zone.c_v  $
 *      
 *         Rev 1.4   29 Apr 1990 16:19:20   dz
 *      Cosmetical changes.
 *      
 *         Rev 1.3   29 Apr 1990 15:27:42   dz
 *      Now mouse moves to corner marker at first click.
 *	Field size enlarged to whole screen hor. and
 *	0 to 327 vert.
 *
 *	   Rev 1.2   15 Dec 1989	    dz
 *	Mouse support added.
 *
 *	   Rev 1.1			    dz
 *	Initial revision.
 *
 *
\*******/


#include "gw.h"
#include "mouse.h"
#include <bios.h>
#include <key.h>

/*#include <>				*/
/*#include <>				*/
/*#include <>				*/

/*	Mark Size	*/
#define	MS	8
#define	ADD	0


static int	x, y, xs, ys;
static bool	fixed;		/*	Fixed size 			*/
static bool	ch_size;	/*	Changing size			*/

#define	SAVESIZE	100

static char	*save[4];

const	x_max = 639,
	x_min = 0,
	y_max = 327,
	y_min = 0;



static void
disp_wins( void ) {

	if( x < x_min )		x = x_min;
	if( x > x_max )		x = x_max;
	if( y < y_min )		y = y_min;
	if( y > y_max )		y = y_max;

	if( x+xs > x_max )	xs = x_max - x;
	if( y+ys > y_max )	ys = y_max - y;

	if( xs < MS )	xs = MS;
	if( ys < MS )	ys = MS;


	mouse_off();
	setcolor( ch_size ? WHITE : YELLOW );

	getimage( x, 		y,	x+MS+ADD, y+MS+ADD, &(save[0][0]) );
	moveto( x, y );		linerel( MS, 0  );
	moveto( x, y );		linerel( 0, MS  );


	setcolor( YELLOW );

	getimage( x+xs-MS,	y,	x+xs+ADD, y+MS+ADD, &(save[1][0]) );
	moveto( x+xs, y );	linerel( -MS, 0 );
	moveto( x+xs, y );	linerel( 0,  MS );

	getimage( x+xs-MS,	y+ys-MS, x+xs+ADD, y+ys+ADD, &(save[2][0]) );
	moveto( x+xs, y+ys );	linerel( -MS, 0 );
	moveto( x+xs, y+ys );	linerel( 0, -MS );

	getimage( x,		y+ys-MS, x+MS+ADD, y+ys+ADD, &(save[3][0]) );
	moveto( x, y+ys );	linerel( MS, 0  );
	moveto( x, y+ys );	linerel( 0, -MS );


	mouse_on();
	}


static void
rm_wins( void ) {
	mouse_off();
	putimage( x,		y+ys-MS,&(save[3][0]), COPY_PUT );
	putimage( x+xs-MS,	y+ys-MS,&(save[2][0]), COPY_PUT );
	putimage( x+xs-MS,	y,	&(save[1][0]), COPY_PUT );
	putimage( x, 		y,	&(save[0][0]), COPY_PUT );
	mouse_on();
	}

static bool	run;
static bool	accept;


static bool
keyb_action( void ) {
	int	step;
	uint	key;

	if( !bioskey(1) )	/*	Some key pressed ?		*/
		return No;	/*	Nothing to do			*/

	key = bioskey(0);
	step = (bioskey(2) & 3) ? 1:10;	/*	Shift ?			*/

	rm_wins();

	switch( key >> 8 ) {
		default:		/*	Bad key			*/
			disp_wins();
			return No;

		case 0x01:		/*	ESC			*/
			run = No;
			accept = No;
			break;

		case 0x1C:		/*	Return			*/
			run = No;
			accept = Yes;
			break;

		case UP:
			if( ch_size )	ys -= step;
			else		y  -= step;
			break;

		case DOWN:
			if( ch_size )	ys += step;
			else		y  += step;
			break;

		case LEFT:
			if( ch_size )	xs -= step;
			else		x  -= step;
			break;

		case RIGHT:
			if( ch_size )	xs += step;
			else		x  += step;
			break;

		case HOME:
			if( ch_size )	xs = MS;
			else		x = x_min;
			break;

		case END:
			if( ch_size )	xs = x_max - x;
			else		x = x_max - MS;
			break;

		case PGUP:
			if( ch_size )	ys = MS;
			else		y = y_min;
			break;

		case PGDN:
			if( ch_size )	ys = y_max - y;
			else		y = y_max - MS;
			break;



		case F7:		/*	Center X	*/
			x = (640 - xs)/2;
			break;

		case F8:		/*	Center Y	*/
			y = (350 - ys)/2;
			break;


		case 0x0F:		/*	Tab		*/
			if( fixed )	ch_size = No;
			else		ch_size = !ch_size;
			break;

		case F1:
			help( "selzone" );
			break;

		} /* switch */

	disp_wins();

	return Yes;
	}


static void
track_mouse( int mx, int my ) {

	if( ch_size ) {
		int	nxs, nys;

		nxs = mx - x;
		nys = my - y;

		if( nxs == xs && nys == ys )
			return;

		rm_wins();
		xs = nxs;
		ys = nys;
		disp_wins();
		}
	else {
		if( x == mx && y == my )
			return;
		rm_wins();
		x = mx;
		y = my;
		disp_wins();
		}

	}


mouse_action( void ) {
	int	lx, ly, b;
	bool	act = No;

	if( !Mouse_attached )		/* No mouse ?			*/
		return No;		/* Nothing to do at all		*/

	if( mpress( 0, &lx, &ly, &b ) ) {
		static long	prevtime;
		long		curtime;

		if( ch_size )
			msetpos( x+xs, y+ys );
		else
			msetpos( x, y );


/*		track_mouse( lx, ly );		*/

		curtime = biostime( 0, 0 );
		if( curtime-prevtime < 9 ) {
			act = accept = Yes;
			run = No;
			}
		else {
			prevtime = curtime;
			while( mgetpos( &lx, &ly, &b ), b )
				track_mouse( lx, ly );
			}
		reset_latches();
		}

	if( mpress( 2, &lx, &ly, &b ) ) {
		act = Yes;
		run = accept = No;
		}

	if( mpress( 1, &lx, &ly, &b ) ) {
		if( fixed )	ch_size = No;
		else		ch_size = !ch_size;
		rm_wins();
		disp_wins();
		}

	return act;
	}




static void
loop( void ) {
	disp_wins();
	while( run ) {

		if( keyb_action() )
			continue;

		if( mouse_action() )
			continue;
		}
	rm_wins();
	return;
	}





bool
select_zone( int *xbase, int *ybase, int *xsize, int *ysize, bool fix ) {
	int	i;

	for( i = 0; i < 4; i++ ) {
		if( (save[i] = malloc( SAVESIZE )) == NULL ) {
			for( --i; i >= 0; i-- )
				free( save[i] );
			return No;
			}
		}
	


	if( *ysize || *xsize ) {
		xs = *xsize;
		ys = *ysize;
		x  = *xbase;
		y  = *ybase;
		}

	fixed = fix;

	ch_size = No;
	accept = No;
	run = Yes;

	loop();

	if( accept ) {
		*xbase = x;
		*ybase = y;
		*xsize = xs;
		*ysize = ys;
		}

	for( i = 0; i < 4; i++ )
		free( save[i] );

	return accept;
	}































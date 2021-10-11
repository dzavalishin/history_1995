/*****************************************************************
 *
 * 	DZ screen interface, Copyright (C) 1989 by DZ
 *
 *	Module :	Help bar handling logic.
 *
 *      Modification history :
 *
 *	$Log:   C:/c/gw/vcs/bar.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:12   dz
 *      Initial revision.
 *
 *
 *      Revision 1.1		08.06.89
 *      Initial revision.
 *
 *
 *****************************************************************/

#include <graphics.h>
#include <stdlib.h>
#include <string.h>

#include "gw.h"

#include "font.h"





/*
 *	Here is two entries :
 *		push_bar( char *help )
 *		pop_bar();
 */

/**Doc************************************************************************
 *	Function: push_bar()      		Header: gw.h
 *
 *	Parameters:
 *		char *			help_string
 *
 *	Return value:			None.
 *
 *	Description:
 *		This function creates small but wide window
 *		at the bottom of the screen and puts 'help_string'
 *		into this window, replacing the previous one.
 *
 *
**/


/**Doc************************************************************************
 *	Function: pop_bar()      		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Deletes help string from the bottom of the screen,
 *		restoring the previous one, if it was.
 *
**/


static gwin win = {
	10, 324, 620, 24,
	};

#define	MAX_BAR	7

static char	*barstack[ MAX_BAR ];
static int	bsp = 0;

static bool	inited = No;


uint	BAR_TEXT =	WHITE;
uint	BAR_BORD =	RED;
uint	BAR_BACK =	BLUE;



cstyle  msgbar_style = {
	&_8x16t_font, 0,
	0, 2, 2,		/*      Shadow parameters       */
	0,			/*      Bold factor             */
	0,			/*      Italics skew            */
	0, 0,
	0, 0,
	0,			/*      Back color              */
	{ 0 },
	/* Shadow */1, /*Bold*/0, /*Italics*/0, 0, 0,
	0,			/*      copy put                */
	0			/*	Back fill off.		*/
	};




void
push_bar( char *txt ) {

	if( !inited ) {

		msgbar_style.c_color = BAR_TEXT;
		msgbar_style.s_color = BAR_BACK;

		if( gwregister( &win ))
			return;
		if( gwon( &win ))
			return;
		inited = Yes;
		bsp = 0;
		}

	if( bsp >= MAX_BAR ) {
		errmsg("Bar stack overflow");
		bsp = 0;
		return;
		}

	barstack[bsp++] = txt;

	gwattach( &win );

	gwshadow( 2, BAR_BORD, 2, BLACK, BAR_BACK );

	moveto( 4, 15 );
	gwsputs( &msgbar_style, txt );

	gwdetach();
	}



void
pop_bar( void ) {
	if( !inited )
		return;

	if( bsp <= 0 ) {
		errmsg("Bar stack underflow");
		bsp = 0;
		return;
		}

	if( (--bsp) <= 0 ) {
		gwoff( &win );
		gwdestroy( &win );
		inited = No;
		return;
		}


	gwattach( &win );

	gwshadow( 2, BAR_BORD, 2, BLACK, BAR_BACK );

	moveto( 4, 15 );
	gwsputs( &msgbar_style, barstack[bsp-1] );

	gwdetach();
	}









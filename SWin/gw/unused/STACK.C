/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Windows stack management.                        
 *                                                               
 *	$Log:   C:/c/gw/vcs/stack.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:02   dz
 *      Initial revision.
 *
 *     Revision 1.1     05.09.89                                 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\**********/


#include "local.h"
#include "ega.h"


/*#include <>			/*		*/
/*#include <>			/*		*/
/*#include <>			/*		*/
/*#include <>			/*		*/
/*#include <>			/*		*/
/*#include <>			/*		*/


static bool	Digged = No;
#define	assert_digout	if( Digged )	return Err


#define		MAX_GWINDOWS	30

static gwin	*wstack[ MAX_GWINDOWS ];
static int	wsp = 0;

static void
wpush( gwin *w ) {
	if( wsp >= MAX_GWINDOWS )		/* Out of stack		*/
/*		return Err;	*/
		gwfatal("Windows stack overflow");

	wstack[wsp++] = w;
	}

static void
wpop( void ) {

	/**
	 *	Really, this error 'cannot happen',
	 *	because the very firs window is an
	 *	automatically attached at initialization
	 *	time 'full_screen_window', and gwdetach()
	 *	won't let anybody to detach it.
	**/

	if( wsp <= 0 )				/* Underflow ??		*/
		gwfatal("Windows stack underflow");

/*	wstack[wsp--] = 0;	*/ wsp--;
	}

/**Doc************************************************************************
 *	Function: bool gwattach()      		Header: gw.h
 *
 *	Parameters:
 *		gwin *			window	- Window to attach.
 *
 *	Return value on error:		Err
 *
 *	Normal return value:		Ok
 *
 *	Description:
 *		Attaches ( turns into active state ) given window.
 *		Saves overlayed screen part, restores old window
 *		contents ( if window was attached previosly ).
 *
**/


bool
gwattach( gwin *w ) {
	assert_init;
	assert_digout;

	w->nactive++;
	wpush( w );

	if( ! w->is_displayed ) {
		gw_swap_images( w, False );
		w->is_displayed = Yes;
		}

	gw_set_win_pars( w );
	return Ok;
	}

#define	curwin	wstack[wsp-1]

/**Doc************************************************************************
 *	Function: bool gwdetach()      		Header: gw.h
 *
 *	Parameters:				None.
 *
 *	Return value on error:			Err
 *
 *	Normal return value:			Ok
 *
 *	Description:
 *		Detaches the LAST window attached, deactivating it.
 *
 *
 *
**/


bool
gwdetach( void ) {
	assert_init;
	assert_digout;

	if( wsp <= 1 )	/* The very first window is a whole screen	*/
		gwfatal("Attempt made to detach main screen");


	curwin->nactive--;

	if( (! curwin->must_be_displayed ) && ( curwin->nactive == 0 ) ) {
		gw_swap_images( curwin, False );	/* Hide it.		*/
		curwin->is_displayed = No;
		}

	wpop();		/*	Forget it				*/

	gw_set_win_pars( curwin );	/* Reattach previous.		*/
	return Ok;
	}

/**Doc************************************************************************
 *	Function: bool gwpermdetach()      	Header: gw.h
 *
 *	Parameters:				None.
 *
 *	Return value on error:			Err
 *
 *	Normal return value:			Ok
 *
 *	Description:
 *		Detaches the LAST window attached, deactivating it.
 *		This kind of gwattach isn't saves window contents,
 *		making a quickest detach pssible.
 *
**/


bool
gwpermdetach( void ) {
	assert_init;
	assert_digout;

	if( wsp <= 1 )	/* The very first window is a whole screen	*/
		gwfatal("Attempt made to detach main screen");


	curwin->nactive--;

	if( (! curwin->must_be_displayed ) && ( curwin->nactive == 0 ) ) {
		gw_swap_images( curwin, True );	/* Hide it.		*/
		curwin->is_displayed = No;
		}

	wpop();		/*	Forget it				*/

	gw_set_win_pars( curwin );	/* Reattach previous.		*/
	return Ok;
	}






static int	dig_wsp;	/*	Digged-out window		*/

bool
gwdigout( gwin *w ) {
	extern gwin	full_screen_window;

	assert_init;
	assert_digout;


	if( w == NULL )		w = &full_screen_window;


	for( dig_wsp = wsp-1; dig_wsp >= 0; dig_wsp-- ) {
		if( wstack[dig_wsp] == w ) {
			gw_set_win_pars( w );
			Digged = Yes;
			return Ok;
			}

		if( dig_wsp <= 0 )	/*	Fullscreen ??		*/
			break;

		if( wstack[dig_wsp]->is_displayed ) {
			wstack[dig_wsp]->is_displayed = No;
			gw_swap_images( wstack[dig_wsp], False );
			}
		}

	gwfatal("Digout : Window lost");
	/* NOTREACHED */
	return Err;
	}

bool
gwdigin( void ) {
	assert_init;
	if( Digged != Yes )	return Err;

	for(; dig_wsp < wsp; dig_wsp++ ) {
		if( !wstack[dig_wsp]->is_displayed ) {
			wstack[dig_wsp]->is_displayed = Yes;
			gw_swap_images( wstack[dig_wsp], False );
			}
		}

	gw_set_win_pars( wstack[wsp-1] );
	Digged = No;
	return Ok;
	}

































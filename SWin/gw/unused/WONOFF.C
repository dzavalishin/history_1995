/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Functions for turning window to be               
 *              displayed or hidden.                             
 *
 *	$Log:   C:/c/gw/vcs/wonoff.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:44   dz
 *      Initial revision.
 *                                                               
 *     Revision 1.1     05.09.89                                 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/


#include "local.h"
/*#include "ega.h"*/


bool
gwon( gwin *w ) {
	assert_init;

	w->must_be_displayed = True;

	if( w->is_displayed )	/*	Allways dispayed ?		*/
		return Ok;	/*	Ok, nothing to do.		*/

	gw_swap_images( w, False );	/*	Display it.		*/
	return Ok;		/*	Done.				*/
	}

bool
gwoff( gwin *w ) {
	assert_init;

	w->must_be_displayed = True;

	if( w->is_displayed == False ||		/* Isn't diplayed ...	*/
		w->nactive )			/* ... or active ?	*/
			return Ok;		/* That's all, folks.	*/

	gw_swap_images( w, False );		/* Temporarily hide.	*/

	return Ok;		/*	Done.				*/
	}




bool
gwpermoff( gwin *w ) {
	assert_init;

	w->must_be_displayed = True;

	if( w->is_displayed == False ||		/* Isn't diplayed ...	*/
		w->nactive )			/* ... or active ?	*/
			return Ok;		/* That's all, folks.	*/

	gw_swap_images( w, True );		/* Hide forever.	*/

	return Ok;		/*	Done.				*/
	}




























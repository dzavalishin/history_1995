/**-------------------   Graphics Windows   -------------------**\
 *                                                              *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		*
 *                                                              *
 *	Module :	Palette save/restore functions.		*
 *                                                              
 *	$Log:   C:/c/gw/vcs/savepal.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:10   dz
 *      Initial revision.
 *
 *	Revision 1.1	3 Jan 90            			*
 *		Initial revision.				*
 *                                                              *
 *								*
\**------------------------------------------------------------**/

#include <style.h>
#include "ega.h"


/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/




static char	std_pal[17] = {
	0,	1,	2,	3,		/*	Palette		*/
	4,	5,	20,	7,
	56,	57,	58,	59,
	60,	61,	62,	63,
	0					/*	Overscan	*/
	};

static char	old_pal[17];

void
save_palette( void ) {
	if( !Ega_Sys_attached )		return;

	es_get_port( old_pal,    ES_ATT, 0, 16 );	/* Get palette	*/
	es_get_port( old_pal+16, ES_ATT, 0x11, 1 );	/* Get overscan	*/
	}

void
rest_palette( void ) {
	if( Ega_Sys_attached ) {
		es_put_port( old_pal,    ES_ATT, 0, 16 );	/* Set palette	*/
		es_put_port( old_pal+16, ES_ATT, 0x11, 1 );	/* Set overscan	*/
		}
	else {
		es_put_port( std_pal,    ES_ATT, 0, 16 );	/* Set palette	*/
		es_put_port( std_pal+16, ES_ATT, 0x11, 1 );	/* Set overscan	*/

puts("No EGA.SYS available\r\n");

		}
	}








































/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *     Module : Create font function.				
 *	
 *	$Log:   C:/c/gw/vcs/creatfnt.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:22   dz
 *      Initial revision.
 *							
 *	Revision 1.2	8 Feb 90				
 *		Proportional fonts support added.		
 *								
 *	Revision 1.1     22.08.89                               
 *		Initial revision.                               
 *                                                              
 *                                                              
\*********/

#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"



/**Doc************************************************************************
 *	Function: bool creat_font()			Header: font.h
 *
 *	Parameters:
 *		font *		font	- Font structure pointer
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Creates a new emty font. Dimensions and 'prop'
 *		flag must be set in the font structure.
 *
**/



bool
creat_font( font *font ) {

	if( font->allc ) {
		free( font->fontptr );
		font->allc = False;
		}

	if( font->pallc ) {
		free( font->propptr );
		font->pallc = False;
		}

	font->bpl = (font->hsize % 8) ? (font->hsize/8 + 1) : (font->hsize/8);
	font->bpl *= font->vsize;


	font->fontptr = calloc( font->bpl*256, 1 );
	if( font->fontptr == NULL ) {
		font->allc = False;
		errno = ENOMEM;
		return Err;
		}
	font->allc = True;


	if( font->prop ) {
		register i;

		font->propptr = calloc( 256, 1 );
		if( font->propptr == NULL ) {
			font->pallc = False;
			errno = ENOMEM;
			return Err;
			}
		for( i = 0; i < 256; i++ )	/* Init prop width tab.	*/
			font->propptr[i] = font->hsize;
		font->pallc = True;
                }

	font->baseline = ( font->vsize * 3 ) / 4;

	return Ok;
	}


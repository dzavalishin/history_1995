/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : Font load/save functions.                        
 *		Superfile block descriptor version.		 
 *                                                               
 *	$Log:   C:/c/gw/vcs/bdfont.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:16   dz
 *      Initial revision.
 *
 *	   Rev 1.1	8 Feb 90				 
 *	Proportional fonts support added		 
 *                                                               
 *	   Rev 1.0     23 Dec 89                                
 *	Initial revision.                                       
 *                                                               
 *                                                               
\*******/

#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"
#include "sfile.h"


/*****************************************************************************/
/*			Super-file support				     */
/*****************************************************************************/


/**Doc************************************************************************
 *	Function: bool sf_load_font_bd()      	Header: font.h
 *
 *	Parameters:
 *		int		sfd	- Fonts SuperFile descriptor.
 *		int		bd	- Font block descriptor.
 *		font *		font	- Buffer to load font in.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Loads font from the given superfile. ( Usually jfonts.sfs )
 *
**/

static int s_sfd, s_bd;

static int
sf_read( int len, long base, void *buf ) {
	return sfread( s_sfd, s_bd, len, base, buf );
	}


bool
sf_load_font_bd( int sfd, int bd, font *font ) {
	s_sfd = sfd;
	s_bd = bd;
	return g_load_font( sf_read, font );
	}


static int
sf_write( int len, long base, void *buf ) {
	int	r;
	r = sfwrite( s_sfd, s_bd, len, base, buf );
	/*	warnmsg("sf_write: asked %d, got %d, errno = %d, base = %ld", len, r, errno, base );	*/
	return r;
	}


bool
sf_save_font_bd( int sfd, int bd, font *font ) {
	s_sfd = sfd;
	s_bd = bd;
	return g_save_font( sf_write, font );
	}






#if 0		/* Version 0 load - unused now.		*/
bool
sf_load_font_bd( int sfd, int bd, font *font ) {

	if( font->allc ) {
		free( font->fontptr );
		font->allc = False;
		}
	if( font->pallc ) {
		free( font->propptr );
		font->pallc = True;
		}

	if( sfread( sfd, bd, FONT_HEADER_SIZE, 0L, font ) != FONT_HEADER_SIZE ) {
errmsg("Can't read header");
		return Err;
		}

	font->fontptr = malloc( font->bpl*256 );
	if( font->fontptr == NULL ) {
		font->allc = False;
		errno = ENOMEM;
outofmem_msg();
		return Err;
		}

	font->allc = True;

	if( sfread( sfd, bd, font->bpl*256, FONT_SF_POS, font->fontptr )
			!= font->bpl*256 ) {
errmsg("Can't read image");
		return Err;
		}

	if( !font->non0v ) {		/* Version 0			*/
		font->vers = 0;
		font -> prop = 0;
		font -> pack_meth = 0;
		font -> propptr = NULL;
		}

warnmsg("Version : %d", font->vers );


	if( font->prop ) {

		font->propptr = malloc( 256 );
		if( font->propptr == NULL ) {
			font->pallc = False;
			errno = ENOMEM;
errmsg("Out of memory loading prop. table");
			return Err;
			}

		font->pallc = True;


		if( sfread( sfd, bd, 256,
				FONT_SF_POS+( font->bpl*256 ),
				font->propptr ) != 256
				) {
errmsg("Error reading prop table");
			return Err;
			}
		}

	return Ok;
	}
#endif
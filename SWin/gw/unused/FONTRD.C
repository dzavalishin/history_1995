/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *     Copyright (C) 1989,1990 by DZ - Moscow, 110.6728		
 *                                                              
 *     Module : Generic Font Load function.			
 *                                                              
 *	$Log:   C:/c/gw/vcs/fontrd.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:14   dz
 *      Initial revision.
 *
 *	   Rev 1.1	8 Feb 90				
 *	Proportional fonts support added		
 *                                                              
 *	   Rev 1.0     23 Dec 89				
 *	Initial revision.				
 *                                                              
 *                                                              
\**********/

#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"


/*****************************************************************************\
			Generic font load function
\*****************************************************************************/


/**Internal************************************************************************
 *	Function: bool g_load_font()      	Header: font.h
 *
 *	Parameters:
 *		int		readf	- Generic user supplied read func.
 *		font *		font	- Buffer to load font in.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Loads font from anywhere. Caller must supply a function
 *		to read through.
 *
**/

#define	FH_SIZE		sizeof( struct Font_item_header )

static bool
read_item( 	font *font, ulong *ibase,
		int (*readf)( int len, long base, void *buf )) {
	struct Font_item_header		h;
	void				*buf;

	if( (*readf)( FH_SIZE, *ibase, &h ) != FH_SIZE )
		return Err;

	switch( h.type ) {
		case Font_layout:
			if( (buf = calloc( font->bpl, 256 )) == NULL )
				return Err;
			font->allc = Yes;
			font->fontptr = buf;
			break;

		case Font_prop_table:
			if( (buf = calloc( 256, 1 )) == NULL )
				return Err;
			font->pallc = Yes;
			font->prop  = Yes;
			font->propptr = buf;
			break;

		default:	/* Ignore unknown item			*/
			(*ibase) += h.size + FH_SIZE;
			return Ok;

		}

	if( (*readf)( (uint)h.size, *ibase + FH_SIZE, buf ) != h.size )
		return Err;

	(*ibase) += h.size + FH_SIZE;
	return Ok;
	}



bool
g_load_font( int (*readf)( int len, long base, void *buf ), font *ufnt ) {
	int	ni;
	ulong	base;
	font	font;

	font.allc	= No;
	font.pallc	= No;

	if( readf( FONT_HEADER_SIZE, 0L, &font ) != FONT_HEADER_SIZE ) {
/*	errmsg("Header read error");	/*				*/
		errno = EIO;
		return Err;
		}

	if( !font.non0v ) {		/* Version 0 support		*/
		font.vers = 0;
		font.prop = 0;
		font.pack_meth = 0;
		font.propptr = NULL;

/*	warnmsg("Loading V.0 font");		/*			*/
		font.fontptr = malloc( font.bpl*256 );
		if( font.fontptr == NULL ) {
			font.allc = False;
			errno = ENOMEM;
/*	errmsg("Out of memory");		/*			*/
			return Err;
			}

		font.allc = True;

		if( readf( font.bpl*256, FONT_LL_POS, font.fontptr )
				!= font.bpl*256 ) {
/*	errmsg("Read error");			/*			*/
			errno = EIO;
			return Err;
			}

                }
	else {
/*	warnmsg("Loading V.1 font");		/*			*/
		base = FONT_LL_POS;
		for( ni = font.n_item; ni--; ) {
			if( read_item( &font, &base, readf ))
				return Err;
			}
		}


/***************** Correctify version-dependent fields *********************/

	if( font.vers < 2 )
		font.baseline = (font.vsize * 3 ) / 4;

/****************************************************************************
		All right, copy it into user's font.
****************************************************************************/



	if( ufnt->allc ) {			/* Free old image	*/
		free( ufnt->fontptr );
		ufnt->allc = False;
		}
	if( ufnt->pallc ) {			/* Free old prop. tab	*/
		free( ufnt->propptr );
		ufnt->pallc = True;
		}


	*ufnt = font;

	return Ok;
	}


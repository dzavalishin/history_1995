/**-------------------   Graphics Windows   -------------------**\
 *								
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *								
 *     Module : Generic Font Save function.			
 *		
 *	$Log:   C:/c/gw/vcs/fontwr.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:50   dz
 *      Initial revision.
 *						
 *	Revision 1.1	11 Feb 90				
 *		Font format changed.				
 *								
 *	Revision 1.0     22.08.89				
 *		Initial revision.				
 *								
 *								
\***************/

#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"



/**Doc************************************************************************
 *	Function: bool g_save_font()			Header: font.h
 *
 *	Parameters:
 *		int		writef	- Generic user-supplied write func
 *		font *		font	- Font structure pointer
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Saves FED font to anywhere. Caller must supply a
 *		function to write through.
 *
**/

#define	FH_SIZE		sizeof( struct Font_item_header )

static bool
write_item( 	void *item, enum Font_item_type itype,
		uint isize, ulong *ibase,
		int (*writef)( int len, long base, void *buf )) {
	struct Font_item_header		h;

	h.size = isize;
	h.type = itype;

	if( (*writef)( FH_SIZE, *ibase, &h ) != FH_SIZE ) {
/*	errmsg("Error writing item header, errno = %d", errno );	/**/
		return Err;
		}

	if( (*writef)( isize, *ibase + FH_SIZE, item ) != isize ) {
/*	errmsg("Error writing item");					/**/
		return Err;
		}

	(*ibase) += isize + FH_SIZE;
	return Ok;
	}




bool
g_save_font( int (*writef)( int len, long base, void *buf ), font *font ) {
	ulong	base;

	font->vers	= FONT_FORMAT_VER; /* Format version number	*/
	font->id	= FONT_MAGIC;	   /* Magic number (ident.)	*/
	font->non0v	= Yes;		   /* Version is nonzero	*/
	font->pack_meth	= 0;		   /* Layout is not compressed	*/
	font->n_item	= font->prop ? 2 : 1;

	if( (*writef)( FONT_HEADER_SIZE, 0, font ) != FONT_HEADER_SIZE ) {
/*	errmsg("Error writing header");			/*		*/
		return Err;
		}

	base = FONT_LL_POS;
	if( write_item(	font->fontptr, Font_layout, font->bpl*256,
			&base, writef ))
		return Err;

	if( font->prop && write_item( font->propptr, Font_prop_table, 256,
			&base, writef ))
		return Err;


	return Ok;
	}






















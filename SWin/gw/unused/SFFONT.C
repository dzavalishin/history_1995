/**********************   Graphics Windows   *********************\
 *
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *     Module : Font load/save functions.
 *		Superfile version.
 *
 *	$Log:   C:/c/gw/vcs/sffont.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:30   dz
 *      Initial revision.
 *
 *     Revision 1.1     22.08.89
 *       Initial revision.
 *
 *
\*********/

#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"
#include "sfile.h"


/*****************************************************************************/
/*			Super-file support				     */
/*****************************************************************************/

/***************************
 *
 *	This function assumes directory exist in the 0th
 *	block of superfile, and searches it for occurence
 *	of the 'name' to figure out what block to read.
**/

/**Doc************************************************************************
 *	Function: bool sf_load_font()      	Header: font.h
 *
 *	Parameters:
 *		int		sfd	- Fonts SuperFile descriptor.
 *		font *		font	- Buffer to load font in.
 *		char *		name	- Font name.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Loads font from the given superfile. ( Usually jfonts.sfs )
 *
**/


bool
sf_load_font( int sfd, font *font, char *name ) {
	int	bd;

	bd = read_name_dir( sfd, 0, -1, name );
	if( bd == Err ) 	return Err;

	return sf_load_font_bd( sfd, bd, font );
	}

/**Doc************************************************************************
 *	Function: bool sf_save_font()      	Header: font.h
 *
 *	Parameters:
 *		int		sfd	- Fonts SuperFile descriptor.
 *		font *		font	- Buffer to save font from.
 *		char *		name	- Font name.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Saves font to the given superfile. ( Usually jfonts.sfs )
 *
**/


bool
sf_save_font( int sfd, font *font, char *name ) {
	int	bd;

	if((bd = read_name_dir( sfd, 0, -1, name )) == Err ) {
		if( errno != ENOENT )
			return Err;
		}
	else {
		if(freebsf( sfd, bd ) == Err )
			return Err;
		if(del_name_dir( sfd, 0, -1, name ) == Err )
			return Err;
		}


	bd = allocbsf( sfd, /*(long)(FONT_LL_POS+sizeof(struct Font_item_header)+(font->bpl*256)+(font->prop?256:0))*/ 0);
	if( bd == Err )
		return Err;

	if(write_name_dir( sfd, 0, bd, name ) == Err)
		return Err;


	return sf_save_font_bd( sfd, bd, font );
	}




/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : Font load/save functions.                        
 *      
 *	$Log:   C:/c/gw/vcs/font.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:54   dz
 *      Initial revision.
 *                                                         
 *	   Rev 1.0     22.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\************/

#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"



/**Doc************************************************************************
 *	Function: bool load_font()			Header: font.h
 *
 *	Parameters:
 *		font *		font	- Font structure pointer
 *					  ( Where to load )
 *		char *		file	- File name.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Loads FED font from file ( usually *.JF ).
 *
**/


static int	ffd;

static int
fl_read( int len, long base, void *buf ) {
	lseek( ffd, base, SEEK_SET );
	return read( ffd, buf, len );
	}



bool
load_font( font *fnt, char *file ) {
	bool	ret;

	if((ffd = open( file, O_RDONLY|O_BINARY )) < 0 )
		return Err;


	ret = g_load_font( fl_read, fnt );

#if 0
	lseek( fd, 0, 0 );
	if( read( fd, &tf, FONT_HEADER_SIZE ) != FONT_HEADER_SIZE ) {
		close( fd );
		return Err;
		}

	tf.fontptr = malloc( tf.bpl*256 );
	if( tf.fontptr == NULL ) {
		close( fd );
		errno = ENOMEM;
		return Err;
		}

	tf.allc = True;

	lseek( fd, FONT_LL_POS, 0 );
	if( read( fd, tf.fontptr, tf.bpl*256 )
			!= tf.bpl*256 ) {
		close( fd );
		return Err;
		}

	close( fd );

	if( fnt->allc ) {
		free( fnt->fontptr );
		fnt->allc = False;
		}

	*fnt = tf;
#endif
	close( ffd );

	return ret;
	}


/**Doc************************************************************************
 *	Function: bool save_font()			Header: font.h
 *
 *	Parameters:
 *		font *		font	- Font structure pointer
 *		char *		file	- File name.
 *
 *	Return value on error:	Err.
 *
 *	Normal return value:	Ok.
 *
 *	Description:
 *		Saves FED font to file ( usually *.JF ).
 *
**/


static int
fl_write( int len, long base, void *buf ) {
	lseek( ffd, base, SEEK_SET );
	return write( ffd, buf, len );
	}




bool
save_font( font *font, char *file ) {
	bool	ret;

	if((ffd = open( file, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666 )) < 0 )
		return Err;


	ret = g_save_font( fl_write, font );

#if 0
	lseek( fd, 0, 0 );
	if( write( fd, font, FONT_HEADER_SIZE ) != FONT_HEADER_SIZE ) {
		close( fd );
		unlink( file );
		return Err;
		}

	lseek( fd, FONT_LL_POS, 0 );
	if( write( fd, font->fontptr, font->bpl*256 )
			!= font->bpl*256 ) {
		close( fd );
		unlink( file );
		return Err;
		}
#endif

	close( ffd );
	return ret;
	}






















/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : File search functions header.
 *
 *	$Log:   C:/c/gw/vcs/futil.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:21:54   dz
 * Initial revision.
 *
 *
 *
\******/

#ifndef FUTIL_H
#define	FUTIL_H


/*      srchfile.c		*/

#ifndef __STDIO_DEF_
#include <stdio.h>
#endif

char	*search_file( const char *file_name, const char *env_name );
FILE 	*search_fopen( const char *fname, const char *env_name, const char *mode );
int	search_open( const char *fname, const char *env_name, int access, int mode );


#endif FUTIL_H
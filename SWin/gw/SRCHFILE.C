/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *	Module : File search/open service.			
 *                  
 *	$Log:   C:/c/gw/vcs/srchfile.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:08   dz
 *      Initial revision.
 *                                            
 *	Revision 1.0	19 Jan 90            			
 *		Initial revision.				
 *                                                              
 *								
\*********/

#include <style.h>
#include "futil.h"

#include <dir.h>
#include <io.h>
/*#include <fcntl.h>*/
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>


/**Doc************************************************************************
 *	Name:	search_file			Header: futil.h
 *
 *	Synopsis: char *search_file( char *file_name, char *env_name );
 *
 *	Parameters:
 *		file_name		full or local file name
 *
 *	Return value:
 *		Pointer to local buffer, containing file name found.
 *		Returns NULL if no valid file found.
 *
 *	Description:
 *		Searches for file in:
 *                      þ Current directory.
 *			þ Directory program started from.
 *			þ Dirs listed in given env. variable.
 *			þ Path dirs.
 *
 *	See also:
 *
**/


bool		/*	Exist ?		*/
ex_name( char *name, char *ddrv, char *ddir, char *nname, char *next ) {
	fnmerge(name, ddrv, ddir, nname, next );
	return !access( name, 0 );
	}

#pragma warn -pia

char *
search_file( const char *fname, const char *env_name ) {
	char	name[80];			/* Output name		*/

	char    ddrv[5];			/* Drive		*/
	char	ddir[70];			/* Directory		*/

	char	nname[10];			/* Name			*/
	char	next[5];			/* Extension		*/

	char	dummy[80];

	char	*p;				/* Common purpose	*/
	char	*edir, *eptr;			/* Env. search pointers	*/

	if( !access( fname, 0 ) ) {		/* Is on the place.	*/
		strncpy( name, fname, 80 );
		return name;
		}

					/* Get name & extension		*/
	fnsplit( fname, dummy, dummy, nname, next );

					/* Look in the current dir.	*/
	if( ex_name( name, "", "", nname, next ))
		return name;

					/* Get dir we are started from	*/
	fnsplit( _argv[0], ddrv, ddir, dummy, dummy );


	if( ex_name( name, ddrv, ddir, nname, next ))
		return name;


				/* Scan given env. variable		*/
	eptr = getenv( env_name );

	while ( eptr && *(edir = eptr) ) {
		if( (p = strchr( eptr, ';' ))) {
			eptr = p + 1;
			*p = '\0';
			}
		else {
			eptr = NULL;
			}


		fnsplit( edir, ddrv, ddir, dummy, dummy );
		if( ex_name( name, ddrv, ddir, nname, next ))
			return name;
		}

				/*	Last - search in path		*/
	fnmerge( name, "", "", nname, next );
	if( p = searchpath( name )) {
		strncpy( name, p, 80 );
		return name;
		}

	return NULL;
	}


FILE *
search_fopen( const char *fname, const char *env_name, const char *mode ) {
	char	*name;

	name = search_file( fname, env_name );
	if( name == NULL )
		return NULL;

	return fopen( name, mode );
	}


int
search_open( const char *fname, const char *env_name, int access, int mode ) {
	char	*name;

	name = search_file( fname, env_name );
	if( name == NULL )
		return -1;

	return open( name, access, mode );
	}




































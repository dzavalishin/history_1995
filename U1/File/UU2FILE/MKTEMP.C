/************************ UU2 UU2File ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Create & open temp. file
 *
 *      $Log:   Q:/news/batch/vcs/mktemp.c_v  $
 *      
 *
 *
\*/

#include    "uu2file.h"
#include    "version.h"
#include	<dir.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<errno.h>


/****************************************************************************
						Create temp file
****************************************************************************/


#define	MAX_TRY		20

FILE *
mk_temp_file( char *o_fn )
	{
	int			fd;
	char		fn[80];
	int			ntry = 0;

	do
		{
        sprintf( fn, "%s\\uu2tmpXXXXXX", (const char *)conf.netfile_dir );
		mktemp( fn );
		fd = open( fn, O_CREAT|O_EXCL|O_BINARY|O_RDWR|O_DENYALL, 0666 );

		if( ntry++ > MAX_TRY )
			{
			error( EI_Full, "Too many tries - can't create temp file (%s)", fn );
			return NULL;
			}
		}
	while( fd < 0 );


	FILE	*of = fdopen( fd, "rb+" );
	if( of == NULL )
		return NULL;

	strcpy( o_fn, fn );
	return of;
	}




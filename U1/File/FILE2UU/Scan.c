/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Scan outfile spool for TICs
 *
 *      $Log:   Q:/file/file2uu/vcs/scan.c_v  $
 *      
 *         Rev 1.0   29 Dec 1992 01:30:34   dz
 *      Initial revision.
 *      
 *
\*/

#include    "file2uu.h"
#include	<dirent.h>

static bool		match_tic( const char *name );

bool
scan_tics( void )
	{
	DIR		*d =	opendir( (char *)(const char *)conf.fileout_dir );

	if( d == NULL )
		{
		error(EI_Full, "Can't open directory %s for scanning", (const char *)conf.fileout_dir );
		return Err;
		}

	dirent 		*ent;

	while( (ent = readdir( d )) != NULL )
		{
		log( "#", "Dirent: %s", ent->d_name );

		if( match_tic( ent->d_name ) )
			{
			char	name[105];
			sprintf( name, "%.80s\\%.20s",
				(const char *)conf.fileout_dir,
				ent->d_name );

			log( "f", "Processing %s", name );
			fprintf( stderr, "Processing %s\n", name );

			if( process_tic( name ) != Err )
				{
				chmod( name, 0666 );
				if( unlink( name ) )
					error( EI_Full, "Can't delete %s", name );
				}

			}
		}

	closedir( d );
	return Ok;
	}



static bool
match_tic( const char *name )
	{
	char	u[100], ext[6];

	fnsplit( name, u, u, u, ext );

	if( strcmpi( ext, ".tic" ) == 0 )
		return Yes;

	if( strcmpi( ext, "tic" ) == 0 )
		return Yes;

	return No;
	}

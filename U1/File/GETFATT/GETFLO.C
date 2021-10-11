/************************ UU2 GetFatt ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FLO parser
 *
 *      $Log:$
 *
 *
\*/

#include		"getfatt.h"

#include		<share.h>
#include		<uu2io.h>

static bool		process_flo( FILE *flo );

// Read FLO line by line, copy/move files
// listed, kill original files, if needed.
//
// If copy/move fails, mark corresponding
// FLO line with '~'.


bool
get_flo( const char *flo_name )
	{
	bool	ret;

	log( "f", "Processing %s", flo_name );
	fprintf( stderr, "Processing %s...\n", flo_name );

	FILE	*flo = _fsopen( flo_name, "rt+", SH_DENYRW );
	if( flo == NULL )
		{
		error( EI_Full, "Can't open FLO: %s", flo_name );
		return Err;
		}

	ret = process_flo( flo );

	if( ret != Err )
		{
		fseek( flo, 0L, SEEK_SET );
		fflush( flo );
		if( chsize( fileno( flo ), 0L ) != 0 )
			error( EI_Full, "Can't truncate %s", flo_name );
		}

	if( ferror( flo ) )
		error( EI_Full, "Error working with %s", flo_name );

	if( fclose( flo ) != 0 )
		error( EI_Full, "Can't close %s", flo_name );

	return ret;
	}




static bool
process_flo( FILE *flo )
	{
	bool	err = Ok;

	while( !feof( flo ) )
		{
		const	bufl = 200;
		char	buf[bufl];
		char	*cp;

		long	pos = ftell( flo );			// Where to write '~'

		if( pos < 0 )
			{
			error( EI_Full, "Can't ftell!" );
			return Err;
			}

		if( fgets( buf, bufl, flo ) == NULL )
			{
			if( ferror( flo ) )
				{
				error( EI_Full, "Read problem" );
				return Err;
				}
			break;
			}

		cp = strpbrk( buf, "\r\n" );
		if( cp != NULL )
			*cp = '\0';

		get_mode	m;
		const char	*a;

		switch( buf[0] )
			{
		case '#':
			m = GMove;
			a = buf+1;
			log( "f", "Moving file %s", a );
			fprintf( stderr, "Moving file %s\n", a );
			break;

		case '^':
			m = GTrunc;
			a = buf+1;
			log( "f", "Copying+truncating file %s", a );
			fprintf( stderr, "Copying+truncating file %s\n", a );
			break;

		case '~':								// Invalid line!
			log( "f", "Skipping file %s", buf );
			fprintf( stderr, "Skipping file %s\n", buf );
			continue;

		default:
			m = GCopy;
			a = buf;
			log( "f", "Copying file %s", a );
			fprintf( stderr, "Copying file %s\n", a );
			break;
			}

		if( get_file( m, a ) != Err )
			{
			err = Err;								// Don't kill FLO!

			long	pos2 = ftell( flo );			// To return here

			if( pos2 < 0 )
				{
				error( EI_Full, "Can't ftell!" );
				return Err;
				}

			if(
					fseek( flo, pos, SEEK_SET ) ||
					fputc( '~', flo ) == EOF ||
					fseek( flo, pos2, SEEK_SET )
			  ) {
				error( EI_Full, "Can't write marker (~)" );
				return Err;
				}
			}
		}

	return err;
	}









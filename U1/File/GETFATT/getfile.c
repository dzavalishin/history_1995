/************************ UU2 GetFatt ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Get one file
 *
 *      $Log:$
 *
 *
\*/

#include	"getfatt.h"

#include	<alloc.h>
#include	<uu2io.h>
#include	<share.h>
#include	<strng.h>



// 	Implementation note:
//
// 	Copy+truncate is an atomic operation
//



static bool		dest_name( String &dst, const char *src );
static bool		move_file( const char *to, const char *from );
static bool		copy_file( const char *to, const char *from, get_mode mode );
static bool		kill_file( const char *fname );
//static bool	trunc_file( const char *fname );

bool
get_file( get_mode mode, const char *src )
	{
	String	dst;

	if( dest_name( dst, src ) == Err )
		return Err;

	switch( mode )
		{
	case GMove:
		if( move_file( dst, src ) == Err )
			{
			log( "#", "Can't move %s to %s, trying copy+kill", src, (const char *)dst );

			if( copy_file( dst, src, GCopy ) == Err )
				return Err;

			if( kill_file( src ) == Err )
				return Err;
			}
		return Ok;

	case GCopy:
		if( copy_file( dst, src, mode ) == Err )
			return Err;

		return Ok;

    case GTrunc:
		if( copy_file( dst, src, mode ) == Err )
			return Err;

		return Ok;

	default:
		error( EI_None, "Unknown mode (%d) in get_file()", (int)mode );
		return Err;
		}

	}



	/*************************************************************
						   Helper
	*************************************************************/



static bool
dest_name( String &dst, const char *src )
	{
	char	u[100];
	char	name[15], ext[5];
	char	all[200];

	if( access( src, 04 ) )
		{
		error( EI_Full, "File %s is not accessible", src );
		return Err;
		}

	_splitpath( src, u, u, name, ext );
	_makepath( all, NULL, conf.fileout_dir, name, ext );

	dst = (const /*signed*/ char *)all;

	log( "f", "%s --> %s", src, (const char *)dst );
	fprintf( stderr, "File %s goes to %s\n", src, (const char *)dst );
	return Ok;
	}


	/*************************************************************
					   File processing
	*************************************************************/



static bool
move_file( const char *to, const char *from )
	{
	if( rename( from, to ) != 0 )
		{
		error( EI_Full, "Can't rename %s to %s", from, to );
		return Err;
		}
	return Ok;
	}

static bool
copy_file( const char *to, const char *from, get_mode mode )
	{
	FILE	*ff, *tf;
	bool	ret = Ok;
	const 	blen = 512;
	char	buf[blen+1];
	const	vbufs = 8 * 1024;				// File buffers size


	ff = _fsopen( from, "rb", SH_DENYRW );
	if( ff == NULL )
		{
		error( EI_Full, "Can't open %s", from );
		return Err;
		}

	tf = _fsopen( to, "wb", SH_DENYRW );
	if( ff == NULL )
		{
		error( EI_Full, "Can't create %s", to );
		fclose( ff );
		return Err;
		}

#ifndef __OS2__
	if( coreleft() > vbufs * 4 )
#endif
		{
		if(
			setvbuf( ff, NULL, _IOFBF, vbufs ) != 0 ||
			setvbuf( tf, NULL, _IOFBF, vbufs ) != 0
		  ) {
			fclose( ff );
			fclose( tf );
			error( EI_None, "Out of memory (setvbuf) !" );
			return Err;
			}
		}

	int		l;
	while( (l = fread( buf, 1, blen, ff )) > 0 )
		if( fwrite( buf, 1, l, tf ) != l )
			{
			error( EI_Full, "Error writing to %s", to );
			ret = Err;
			break;
			}

	if( mode == GTrunc )
		{
		fflush( ff );
		if( chsize( fileno( ff ), 0L ) != 0 )
			{
			error( EI_Full, "Can't truncate %s", from );
			ret = Err;
			}
		}


	if( ferror( ff ) )
		{
		error( EI_Full, "Error reading %s", from );
		ret = Err;
		}

	if( fclose( ff ) || fclose( tf ) )
		{
		error( EI_Full, "Error closing %s or %s", from, to );
		ret = Err;
		}

	if( ret == Err )					// Can't copy?
		kill_file( to );				// Don't leave junk...

	return ret;
	}

static bool
kill_file( const char *fname )
	{
	if( chmod( fname, 0666 ) != 0 )
		{
		error( EI_Full, "Can't change %s mode to 0666", fname );
		}

	if( unlink( fname ) != 0 )
		{
		error( EI_Full, "Can't delete %s", fname );
		return Err;
		}

	return Ok;
	}


/*
static bool
trunc_file( const char *fname )
	{
	}
*/




























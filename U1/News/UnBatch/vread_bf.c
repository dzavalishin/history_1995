/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1993 by Infinity Soft
 *
 *	Module 	:	Virtual file reader - buffered file
 *
 *      $Log: vread_bf.c $
 * Revision 1.3  1995/04/12  17:32:19  dz
 * Bug fixes
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   05 Mar 1993 00:05:34   dz
 *      Initial revision.
 *
 *
\*/

#include	<stddef.h>
#include	<share.h>
#include	<fcntl.h>

#include	"vread.h"


VReadBFile::
VReadBFile( void )
	{
	xfp = NULL;
	do_close = No;
	}

VReadBFile::
~VReadBFile( void )
	{
	close();
	}

	/*************************************************************
						   Open/Close
	*************************************************************/


bool VReadBFile::
open( FILE * fp )
	{
	close();
	xfp = fp;
	do_close = No;

	setvbuf( xfp, NULL, _IOFBF, 16384 );

	return Ok;
	}

bool VReadBFile::
open( const char *name )
	{
	close();

	int xfd = ::_sopen( name, O_RDONLY|O_BINARY, SH_DENYWR );
	if( xfd < 0 )
		return Err;

	xfp = fdopen( xfd, "rb" );
	if( xfp == NULL )
		{
		::close( xfd );
		return Err;
		}

	setvbuf( xfp, NULL, _IOFBF, 16384 );

	do_close = Yes;
	return Ok;
	}

bool VReadBFile::
close( void )
	{
	if( !do_close )
		return Ok;

	do_close = No;
	if( ::fclose( xfp ) != 0 )
		return Err;

	return Ok;
	}

	/*************************************************************
						   Access
	*************************************************************/


int VReadBFile::
read( void *buf, int blen )
	{
	return ::fread( buf, 1, blen, xfp );
	}

bool VReadBFile::
gets( uchar *buf, int blen )
	{
	return ::fgets( (char *)buf, blen, xfp ) == NULL ? Err : Ok;
	}



long VReadBFile::
seekg( long pos )
	{
	return ::fseek( xfp, pos, SEEK_SET );
	}



long VReadBFile::
tellg( void )
	{
	return ::ftell( xfp );
	}


long VReadBFile::
size( void )
	{
	fflush( xfp );
	return filelength( fileno( xfp ));
	}


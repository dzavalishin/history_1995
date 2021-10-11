/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Save contents to .TIC file
 *
 *      $Log: Tic_Save.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   29 Dec 1992 01:32:34   dz
 *      Initial revision.
 *
 *
\*/

#include    "tic.h"
#include	"..\..\version.h"
#include	<log.h>

#ifdef __IBMCPP__
#	define	TEXT_MODE
#else
#	define	TEXT_MODE "t"
#endif



static void
print_fa( FILE *f, fido_addr &fa, char *kw )
	{

	if( !fa.valid() )
		return;

	if( strlen( fa.domain() ) )
		fprintf( f, "%s: %s>", kw, fa.domain() );
	else
		fprintf( f, "%s: ", kw );

	fprintf( f, "%u:%u/%u", fa.zone(), fa.net(), fa.node() );

	if( fa.point() )
		fprintf( f, ".%u\n", fa.point() );
	else
		fprintf( f, "\n" );
	}


bool tic_file::
save_to( FILE *f )
	{
	fprintf( f, "File: %s\n", name_v );
	fprintf( f, "Area: %s\n", area_v );

	if( size_v )				fprintf( f, "Size: %ld\n", size_v );
	if( crc_v )					fprintf( f, "Crc: %lx\n", crc_v );
	if( strlen( passwd_v ) )	fprintf( f, "Pw: %s\n", passwd_v );
	if( date_v )				fprintf( f, "Date: %ld\n", (long)date_v );
	if( release_v )				fprintf( f, "Release: %ld\n", (long)release_v );

	print_fa( f, source_v,	"Source" );
	print_fa( f, from_v,		"From" );
	print_fa( f, origin_v,	"Origin" );

	if( send_receipt_v )
		print_fa( f, receipt_v,	"RcRcpt" );


		{
		ContainerIterator	&di = replaces_v.initIterator();
		for( ; (int)di; di++ )
			fprintf( f, "Replaces: %s\n", (const char *)(String&)di.current() );
		}

		{
		ContainerIterator	&di = desc_v.initIterator();
		for( ; (int)di; di++ )
			fprintf( f, "Desc: %s\n", (const char *)(String&)di.current() );
		}

		{
		ContainerIterator	&di = path_v.initIterator();
		for( ; (int)di; di++ )
			fprintf( f, "Path: %s\n", (const char *)(String&)di.current() );
		}

		{
		ContainerIterator	&di = seenby_v.initIterator();
		for( ; (int)di; di++ )
			fprintf( f, "SeenBy: %s\n", (const char *)(String&)di.current() );
		}


	if( ferror( f ) )
		{
		error( EI_Full, "Can't write to TIC file" );
		return Err;
		}

	return Ok;
	}


bool tic_file::
save_to( const char *fn )
	{
	FILE	*f = fopen( fn, "w" TEXT_MODE );
	if( f == NULL )
		{
		error( EI_Full, "Can't create TIC file: %s", fn );
		return Err;
		}

	if( save_to( f ) == Ok && fclose( f ) != EOF )
		return Ok;

	error( EI_Full, "Can't write to TIC file: %s", fn );

	return Err;
	}




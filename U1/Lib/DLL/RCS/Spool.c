head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/****************************** UU2 Snews *********************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	spool directory management
 *
 *      $Log: Spool.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.5   07 Jun 1993 17:54:42   dz
 *      OS/2 port: format of file names changed to hex
 *      Lot of code rewritten
 *      
 *         Rev 1.4   17 Nov 1992 04:03:52   dz
 *      Based on Object
 *      
 *         Rev 1.3   17 Nov 1992 03:45:04   dz
 *      Rewritten in C++ :)
 *      
 *         Rev 1.1   18 Sep 1992 03:11:26   dz
 *      removed call to mktemp: major speedup
 *      
 *         Rev 1.0   18 Jun 1992 11:11:56   dz
 *      Initial revision.
 *
 *
\*/

#include	<stdlib.h>
#include	"spool.h"
#include	"uu2io.h"
#include	"..\..\version.h"
#include	"..\..\u1.h"
#include	"DLLInits.h"

static FILE		*lfp;					// Pointer to spool list file
static bool		add_list_info( const char *name, const char *info, const char *path );

#ifdef __IBMCPP__
#	define	TEXT_MODE
#else
#	define	TEXT_MODE "t"
#endif


news_out_spool_file::
news_out_spool_file( void )
	{
	fp_v = NULL;
	posted = No;
	valid = No;
	created = No;
	name[0] = '\0';
	}

news_out_spool_file::
~news_out_spool_file( void )
	{
	abort();
	}


bool  news_out_spool_file::
create( void )
	{
	static uint		fno = 0;

	if( fno == 0 )
		fno = (uint) time(NULL);

	abort();
	valid = No;
	posted = No;

//	if( strlen( conf.newsout_dir ) == 0 )
	if( conf.newsout_dir.length() == 0 )
		fatal( EC_Incorrect_Setup, EI_None, "No outbound spool directory specified" );

	while( 1 )
		{
		sprintf( name, "%.60s\\%x.art", (const char *)conf.newsout_dir.c_str(), fno++ );

		if( (fp_v = fopen( name, "wb" )) != NULL )
			break;

		if( errno != EEXIST )
			fatal( EC_IO_Error, EI_Full, "Can't create spool file '%s'", name );

		fno += 21;
		}

	setvbuf( fp_v, NULL, _IOFBF, 16384 );

	created = Yes;
	return Ok;
	}


bool news_out_spool_file::
abort( void )
	{
	if( posted )
		return Ok;

	if( fp_v != NULL )
		close();

	if( created )
		{
		debug( "Aborting spool file '%s'", name );
		chmod( name, 0666 );
		unlink( name );
		}

	created = No;
	valid = No;
	posted = No;

	return Ok;
	}

bool news_out_spool_file::
close( void )
	{
	if( fp_v == NULL )
		return Ok;

	if( fclose( fp_v ) != 0 )
		{
		fp_v = NULL;
		return Err;
		}

	fp_v = NULL;
	valid = Yes;
	return Ok;
	}

bool news_out_spool_file::
post( const char *list_info, const char *path )
	{
	if( close() || !created )
		return Err;

	if( posted )
		return Ok;

	if( add_list_info( name, list_info, path ) == Err )
		{
		error( EI_None, "Can't add spool file (%s) to list!", name );
		abort();
		return Err;
		}

	posted = Yes;

	return Ok;
	}

	/*************************************************************
				   ClassLib compartibility
	*************************************************************/

#if 0
int news_out_spool_file::
isEqual( const Object _FAR &r ) const
	{
	if( this == &r )
		return 1;

	return 0;
	}

void news_out_spool_file::
printOn( ostream _FAR &o ) const
	{
	o << "{news_out_spool_file: " << name << "}\n";
	}
#endif


/****************************************************************************
						Spool list management
****************************************************************************/

extern "SYSTEM" void
close_list( void )
	{
	if( lfp == NULL )
		return;

	fclose( lfp );
	}


static bool
open_list( void )
	{
	char	name[80];
	static int	opened = 0;

	if( opened == 0 )
		{
//		at exit( close_list );
		opened = 1;
		}

	for( int i = 0; i < 10; i++ )
		{
		sprintf( name, "%s\\list%d", (const char *)conf.newsout_dir.c_str(), i );
		if( (lfp = fopen( name, "a" TEXT_MODE )) == NULL )
			continue;

		return Ok;
		}

	error( EI_Full, "Can't create spool list file!" );
	return Err;
	}


//#pra gma exit close_list 65

static bool
add_list_info( const char *name, const char *info, const char *path )
	{
	if( lfp == NULL && open_list() == Err )
		return Err;

	fprintf( lfp, "%s %s", name, info );
	if( path && *path )
		fprintf( lfp, " %s", path );
	fprintf( lfp, "\n" );

	return ferror( lfp ) ? Err : Ok;
	}

/*
test( void )
	{
	news_out_spool_file		f;
	}
*/
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/spool.c_v  $
d34 1
d39 1
d44 7
d97 2
d191 9
d205 7
d216 1
a216 1
		if( (lfp = fopen( name, "at" )) == NULL )
d227 1
a227 11
static void
close_list( void )
	{
	if( lfp == NULL )
		return;

	fclose( lfp );
	}


#pragma exit close_list 65
@

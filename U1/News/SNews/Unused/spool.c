/****************************** UU2 Snews *********************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	spool directory management
 *
 *      $Log:   Q:/news/snews/vcs/spool.c_v  $
 *      
 *         Rev 1.1   18 Sep 1992 03:11:26   dz
 *      removed call to mktemp: major speedup
 *      
 *         Rev 1.0   18 Jun 1992 11:11:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"
#include	"..\..\uu2.h"
//#include	"fido_pkt.h"

static char		temp_name[80];			// Name of current spool file
static FILE		*sfp;					// Pointer to current spool file
static FILE		*lfp;					// Pointer to spool list file


static bool		add_list_info( char *name, char *info );





FILE *
mk_spool_file( void )
	{
	static uint		fno = 0;

	if( fno == 0 )
		fno = (uint) time(NULL);

	if( sfp != NULL )
		abort_spool_file();

	if( strlen( conf.newsout_dir ) == NULL )
		fatal( EC_Incorrect_Setup, "No outbound spool directory specified" );

	while( 1 )
		{
		sprintf( temp_name, "%s\\%u.art", conf.newsout_dir, fno++ );

		if( (sfp = fopen( temp_name, "wb" )) != NULL )
			break;

		if( errno != EEXIST )
			fatal( EC_IO_Error, "Can't create spool file '%s'", temp_name );

		fno += 21;
		}

	return sfp;
	}


bool
abort_spool_file( void )
	{
	if( sfp == NULL )
		{
		error( "Aborting unopened spool file!" );
		return Ok;
		}

	debug( "Aborting spool file '%s'", temp_name );
	fclose( sfp );
	chmod( temp_name, 0666 );
	unlink( temp_name );
	sfp = NULL;
	return Ok;
	}

bool
close_spool_file( char *list_info )
	{

	if( add_list_info( temp_name, list_info ) == Err )
		{
		error("Can't add spool file (%s) to list!", temp_name );
		abort_spool_file();
		return Err;
		}

	fclose( sfp );
	sfp = NULL;
	return Ok;
	}


/****************************************************************************
						Spool list management
****************************************************************************/


static bool
open_list( void )
	{
	char	name[80];

	for( int i = 0; i < 10; i++ )
		{
		sprintf( name, "%s\\list%d", conf.newsout_dir, i );
		if( (lfp = fopen( name, "at" )) == NULL )
			continue;

		return Ok;
		}

	error("Can't create spool list file!");
	return Err;
	}


static void
close_list( void )
	{
	if( lfp == NULL )
		return;

	fclose( lfp );
	}


#pragma exit close_list 65

static bool
add_list_info( char *name, char *info )
	{
	if( lfp == NULL && open_list() == Err )
		return Err;

	fprintf( lfp, "%s %s\n", name, info );

	return ferror( lfp ) ? Err : Ok;
	}



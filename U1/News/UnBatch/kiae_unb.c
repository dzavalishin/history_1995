/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Run KIAE uunpack, split resulting
 *				mailbox-like file into the articles
 *
 *      $Log: kiae_unb.c $
 *      Revision 1.4  1995/08/07 14:55:01  dz
 *      Using class App
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 *
 *
\*/

#include    "unbatch.h"
#include    "..\..\version.h"
#include	<process.h>
#include	"recode.h"



bool
UnbatchApp::run_uunpack( const char *fn )
	{
	int     ret;

	log( "#", "Running uunpack %s", fn );

	ret = spawnlp( P_WAIT, "uunpack", "uunpack", fn, NULL );

	if( ret != 0 )
		{
		error( EI_Full, "Can't unpack '%s'", fn );
		change_ext( fn, ".bp" );
		}

	return ret == 0 ? Ok : Err;
	}


bool
UnbatchApp::unbatch_kiae( const char *fn, bool recode_back )
	{
	FILE		*fp;
	int			ofd = -1;
	bool		get = No;
	char		buf[200];
	recoder		rec( "CP866-KOI8" );			// Get codetable

	if( !rec.valid() )							// Can't init recoder
		fatal( EC_Incorrect_Setup, EI_None, "Can't find codetable 'CP866-KOI8'" );

	if( run_uunpack( fn ) == Err )
		return Err;

	debug("splitting up resulting mailbox");

	if( (fp = fopen( fn, "rb" )) == NULL )
		{
		error( EI_Full, "Can't open mailbox %s", fn );
		change_ext( fn, ".bm" );
		return Err;
		}

	while( !feof( fp ) )
		{

		if( fgets( buf, 200, fp ) == NULL )
			break;

		if( recode_back )
			rec.rs( (uchar *)buf );

		if( strncmp( buf, "From", 4 ) == 0 && buf[4] != ':' )
			{
			if( ofd >= 0 )
				close( ofd );

			get = Yes;
			maketemp( conf.newsin_dir.c_str(), ".A", &ofd );
			continue;
			}

		if( get )
			{
			if( write( ofd, buf, strlen( buf ) ) < 0 )
				{
				error( EI_Full, "Can't write to a temp file" );
				break;
				}
			}

		}

	if( get )
		close( ofd );

	if( ferror( fp ) )
		error( EI_Full, "Error reading mailbox %s", fn );

	fclose( fp );

	return Ok;										// Let us kill it anyway
	}


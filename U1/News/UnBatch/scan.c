/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Scan directory for batches, unbatch & kill 'em
 *
 *
 *      $Log: scan.c $
 *      Revision 1.5  1995/08/07 14:55:01  dz
 *      Using class App
 *
 * Revision 1.4  1995/04/12  17:32:19  dz
 * Bug fixes
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *
 *
\*/

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>


#include    "unbatch.h"
#include    "..\..\version.h"

#define ATTRS2FIND	0x27


static void     kill( const char *dir, const char *fn );




bool
UnbatchApp::process_all( const char *batch_dir )
	{
//	ffblk		ff;
	char		mask[100];
	HDIR		h = HDIR_SYSTEM;
	FILEFINDBUF3	ff = { 0 };
	ULONG		count = 1;
	APIRET		rc;

	sprintf( mask, "%s\\*.*", batch_dir );
	debug("Search mask: `%s'", mask );


	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
	if( rc != NO_ERROR )
		{
		if( rc == ERROR_NO_MORE_FILES )
			{
			log( "#", "Nothing to do - no batches found" );
			return No;
			}

		fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
		}


	do  {

//		if( ff.attrFile & FA_DIREC )
//			continue;

		if( z_ext(ff.achName) )
			{
			if( decompress( batch_dir, ff.achName ) == Ok )
				{
				kill( batch_dir, ff.achName );
				fprintf( stderr, "Decompressed: %s\n", ff.achName );
				log( "cn", "Decompressed: %s", ff.achName );
				}
			}
		else
		if( unbatch( batch_dir, ff.achName ) == Ok )
			{
			kill( batch_dir, ff.achName );
			log( "#", "Unbatch done: %s", ff.achName );
			}

		count = 1;
		rc = DosFindNext( h, &ff, sizeof(ff), &count );
		} while( rc == NO_ERROR );

	if( rc != ERROR_NO_MORE_FILES )
		fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

	if( DosFindClose( h ) != NO_ERROR )
		error( EI_Full, "FindClose rc = %d", rc );
	}


void
UnbatchApp::scan( const char *batch_dir )
	{

	process_all( batch_dir );

	//------------- And again, to clean up everything first pass made

	process_all( batch_dir );

    // Note that if compressed file had extension other than .z, we
    // have a chance to leave something in inbound spool even after
    // two passes. It doesn't matter too much since we'll eat it next
    // time we run.

	}


/****************************************************************************
                                 Delete file
****************************************************************************/

static void
kill( const char *dir, const char *fn )
	{
	char    f[150];

	sprintf( f, "%s\\%s", dir, fn );

	chmod( f, 0666 );
	unlink( f );
//	debug("%s deleted", f );
	}





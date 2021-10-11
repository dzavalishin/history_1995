head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.04.12.17.32.19;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.5
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Scan directory for batches, unbatch & kill 'em
 *
 *
 *      $Log: scan.c $
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




@


1.4
log
@Bug fixes
@
text
@d9 3
d42 2
a43 2
static bool
process_all( const char *batch_dir )
d103 1
a103 1
scan( const char *batch_dir )
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d9 3
d76 1
a76 1
				fprintf( stderr, "Decompressed: %s", ff.achName );
@


1.2
log
@rewriting for C Set
@
text
@a6 1
 *  Todo	:	join process_z & process_all
d9 3
a15 29
 *         Rev 1.9   03 Mar 1993 20:19:56   dz
 *      Port to OS/2 - ATTR argument of findfirst fixed
 *      
 *         Rev 1.8   28 Dec 1992 21:40:24   dz
 *      Added second pass of .z processing
 *      
 *         Rev 1.7   28 Nov 1992 22:30:32   dz
 *      consts
 *      
 *         Rev 1.6   23 Oct 1992 15:01:40   dz
 *      fatal()
 *      
 *         Rev 1.5   06 Jul 1992 23:59:26   dz
 *      fatal() - exit codes
 *      
 *         Rev 1.4   29 Jun 1992 22:42:14   dz
 *      Added second unbatch loop to be sure everything was processed
 *
 *         Rev 1.3   17 May 1992 11:05:58   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.2   21 Feb 1992 12:09:04   dz
 *      Decompressor starter written
 *      
 *         Rev 1.1   21 Feb 1992 11:21:24   dz
 *      Debugging...
 *      
 *         Rev 1.0   21 Feb 1992 11:07:44   dz
 *      Initial revision.
d20 5
a26 2
//#include	<dir.h>
//#include	<dos.h>
d28 1
a28 1
#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH
a33 38
#if 0
static void
process_z( const char *batch_dir )
	{
	ffblk		ff;
	char		mask[100];

	sprintf( mask, "%s\\*.z", batch_dir );
	debug("Search mask: `%s'", mask );

    if( findfirst( mask, &ff, ATTRS2FIND ) )
		{
		if( errno == ENOENT )
			log( "#", "No .z files found" );
		else
			fatal( EC_OS_Error, EI_Full, "FindFirst" );
		}
	else
		{

		do  {

			if( ff.ff_attrib & FA_DIREC )
				continue;

			if( decompress( batch_dir, ff.ff_name ) == Ok )
				{
				kill( batch_dir, ff.ff_name );
				log( "cn", "Decompressed: %s", ff.ff_name );
				}

			} while( findnext(&ff) == 0 );

		if( errno != ENOENT )
			fatal( EC_OS_Error, EI_Full, "FindNext" );
		}
	}
#endif
d36 1
a36 1
static void
d39 1
a39 1
	ffblk		ff;
d41 4
d49 3
a51 1
    if( findfirst( mask, &ff, ATTRS2FIND ) )
d53 1
a53 1
		if( errno == ENOENT )
d56 1
a56 1
			return;
d59 1
a59 1
		fatal( EC_OS_Error, EI_Full, "FindFirst" );
d65 2
a66 2
		if( ff.ff_attrib & FA_DIREC )
			continue;
d68 1
a68 1
		if( z_ext(ff.ff_name) )
d70 1
a70 1
			if( decompress( batch_dir, ff.ff_name ) == Ok) )
d72 3
a74 3
				kill( batch_dir, ff.ff_name );
				fprintf( stderr, "Decompressed: %s", ff.ff_name );
				log( "cn", "Decompressed: %s", ff.ff_name );
a75 1
			continue;
d77 2
a78 2

		if( unbatch( batch_dir, ff.ff_name ) == Ok )
d80 2
a81 3
			kill( batch_dir, ff.ff_name );
			log( "#", "Unbatch done: %s", ff.ff_name );
			continue;
d84 3
a86 1
		} while( findnext(&ff) == 0 );
d88 5
a92 2
	if( errno != ENOENT )
		fatal( EC_OS_Error, EI_Full, "FindNext" );
a99 6
	//------------- 1. Find and unpack all the compressed ones

//	process_z( batch_dir );

	//------------- 2. Scan through all the files, find & process batches

d102 1
a102 1
	//------------- And again, to clean up everything first two phases made
a103 1
//	process_z( batch_dir );			// Process_all() might create .Z ones
d115 1
a115 1
						Delete file
@


1.1
log
@Initial revision
@
text
@d9 4
a12 1
 *      $Log:   Q:/news/unbatch/vcs/scan.c_v  $
d49 2
a50 2
#include	<dir.h>
#include	<dos.h>
d57 2
d95 1
d124 11
d139 1
d155 1
a155 1
	process_z( batch_dir );
d163 1
a163 1
	process_z( batch_dir );			// Process_all() might create .Z ones
@

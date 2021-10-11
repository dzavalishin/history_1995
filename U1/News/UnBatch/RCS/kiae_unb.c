head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.14.55.01;	author dz;	state Exp;
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


1.4
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Run KIAE uunpack, split resulting
 *				mailbox-like file into the articles
 *
 *      $Log: kiae_unb.c $
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

@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d9 6
d29 2
a30 2
static bool
run_uunpack( const char *fn )
d49 1
a49 1
unbatch_kiae( const char *fn, bool recode_back )
@


1.2
log
@rewriting for C Set
@
text
@d9 2
a10 2
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
a11 24
 *      
 *         Rev 1.7   28 Nov 1992 22:31:28   dz
 *      consts
 *      
 *         Rev 1.6   23 Oct 1992 15:02:54   dz
 *      error()
 *      
 *         Rev 1.5   10 Oct 1992 01:43:28   dz
 *      Bug caught: there was no fclose!
 *      
 *         Rev 1.4   07 Jul 1992 00:00:24   dz
 *      fatal() - exit codes
 *      
 *         Rev 1.3   18 Jun 1992 11:16:20   dz
 *      Recoding fixed, bug with from/From confusion fixed
 *      
 *         Rev 1.2   24 May 1992 18:46:36   dz
 *      Recoding to KOI cause the following stage will recode to ALT :)
 *      
 *         Rev 1.1   24 May 1992 18:43:48   dz
 *      bugs fixed
 *      
 *         Rev 1.0   17 May 1992 11:06:48   dz
 *      Internal packetizer, kiae-specific unbatcher
@


1.1
log
@Initial revision
@
text
@d8 4
a11 1
 *      $Log:   Q:/news/unbatch/vcs/kiae_unb.c_v  $
d97 1
a97 1
			rec.rs( buf );
@

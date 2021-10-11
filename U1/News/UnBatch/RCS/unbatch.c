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
 *  Module  :   Cut batch into the pieces
 *
 *      $Log: unbatch.c $
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
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

#include	"unbatch.h"
#include	"uuparse.h"
#include	"..\..\version.h"
#include	"..\..\u1.h"
#include	<fcntl.h>
#include	<malloc.h>

#include	"vread.h"


static bool		bheader( const char *h, long *l );
static bool		zheader( const char *h );
//static bool		do_send_article( VReadBFile & in, const char *fn );
static bool		art_ext( const char *name );


bool
UnbatchApp::do_unbatch( const char *bn, VReadBFile & in )
	{
	char	hdr[20];
	bool	resync = No;
	bool	first_pass = Yes;
	long	len;

	while( !in.eof() )
		{

		if( getheader( hdr, 19, in ) == Err )
			{
			error( EI_Full, "Can't read batch header");
// Can't - file is open!!
//            change_ext( bn, ".bb" );
			return Err;
			}

		if( first_pass && zheader( hdr ) == Yes )
			{
//			error( EI_None, "Compressed batch %s", bn );
			in.close();
			change_ext( bn, ".z" );
			return Err;
			}

		if( bheader( hdr, &len ) == Yes )
			{
			debug("Art: %ld bytes", len );

			if( get_art( in, len ) == Err )
				{
				change_ext( bn, ".bb" );
				return Err;
				}

			resync = No;
			first_pass = No;
			continue;
			}

		if( first_pass )
			{
			log( "#", "Not a batch %s", bn );
			return Err;
			}

		if( resync )
			continue;

		if( strlen( hdr ) == 0 )
			break;

		error( EI_None, "No article header, resync...");
		}

	return Ok;
	}



bool
UnbatchApp::unbatch( const char *dir, const char *fn )
	{
	char		bn[80];
	bool		ret;
	VReadBFile	in;

	sprintf( bn, "%s\\%s", dir, fn );

	if( in.open( bn ) == Err )
		{
		error( EI_Full, "Can't open batch %s", bn );
		return Err;
		}

	if( art_ext( fn ) )
		{
		log( "BN", "Sending article %s", bn );
		ret = do_send_article( in, bn );
		}
	else
		{
		log( "bn", "Unbatching %s", bn );
		ret = do_unbatch( bn, in );
		}

	return ret;
	}




/*-------------------------------------------------------------------------*/


static bool
bheader( const char *h, long *l )
	{

	if( 1 != sscanf( h, "#! rnews %ld", l ) )
		return No;

	return Yes;
	}


static bool
zheader( const char *h )
	{
    if( h[0] != '' )               return No;
    if( h[1] != 'ù' )               return No;
    if( h[2] < 'å' || h[2] > 'ê' )  return No;  // 12 to 16 bit
    return Yes;
	}

/*-------------------------------------------------------------------------*/



bool
UnbatchApp::do_send_article( VReadBFile & in, const char *fn )
	{
	uuinfo		ui;								// uuparse stores result here

	/*************************************************************
						Analyze UUCP letter
	*************************************************************/

	if( uuparse( in, &ui ) == Err )					// Parse headlines
		return Err;

	ui.textsize = in.size();

	strlwr( ui.x_batch );
	if( strstr( ui.x_batch, "compress" ) )
		{
		in.close();						// Kiae unbatcher will need it unlocked?
		return unbatch_kiae( fn, Yes );	// Recode results back to koi-8
		}
	else if( strstr( ui.x_batch, "pack" ) )
		{
		in.close();						// Kiae unbatcher will need it unlocked?
		return unbatch_kiae( fn, No );	// Result will be in koi-8 without additional recodind
		}
	else
		return process_art( in, ui );
	}



const char *
get_ext( const char *name )
	{
	const char *p;

	p = strrchr( name, '/' );
	if( p == NULL ) p = name;

	p = strrchr( p, '\\' );
	if( p == NULL ) p = name;

	p = strrchr( p, '.' );

	if( p == NULL )
		return "";
	else
		return p;
	}


static bool
art_ext( const char *name )
	{
	const char *ext = get_ext( name );

	if( stricmp( ext, ".a" ) == 0 )
		{
		debug("File %s is an article", name );
		return Yes;
		}

	return No;
	}

bool
UnbatchApp::z_ext( const char *name )
	{
	const char *ext = get_ext( name );

	if( stricmp( ext, ".z" ) == 0 )
		{
		debug("File %s is an compressed batch", name );
		return Yes;
		}

	return No;
	}
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d37 1
a37 1
static bool		do_send_article( VReadBFile & in, const char *fn );
d41 2
a42 2
static bool
do_unbatch( const char *bn, VReadBFile & in )
d104 1
a104 1
unbatch( const char *dir, const char *fn )
d162 2
a163 2
static bool
do_send_article( VReadBFile & in, const char *fn )
d228 1
a228 1
z_ext( const char *name )
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a24 1
//#include	<dir.h>
d193 2
d196 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 25
 *      $Log:   Q:/news/unbatch/vcs/unbatch.c_v  $
 *      
 *         Rev 1.11   10 Mar 1993 14:03:42   dz
 *      Additional fixes of direct unbatching - now closing
 *      article file before calling KIAE-style unbatcher to
 *      prevent sharing violation.
 *      
 *         Rev 1.10   05 Mar 1993 03:25:36   dz
 *      Cleanup
 *      
 *         Rev 1.9   05 Mar 1993 03:04:02   dz
 *      Direct unbatching
 *      
 *         Rev 1.8.1.0   03 Mar 1993 20:21:40   dz
 *      Temp: trying to implement direct (no temp files)
 *      unbatching scheme.
 *      
 *         Rev 1.8   28 Dec 1992 21:36:56   dz
 *      Bug fix: now recognizes 12-16 bit compression headers.
 *      
 *         Rev 1.7   28 Nov 1992 22:30:58   dz
 *      consts
 *      
 *         Rev 1.6   23 Oct 1992 15:01:58   dz
 *      error()
a11 18
 *         Rev 1.5   11 Oct 1992 05:48:26   dz
 *      open/fdopen/close problem hacked off
 *      
 *         Rev 1.4   23 Jun 1992 18:43:12   dz
 *      Ignoring empty files
 *      
 *         Rev 1.3   18 Jun 1992 11:14:54   dz
 *      textsize bug fixed
 *      code conversion for kiae-batches fixed
 *      
 *         Rev 1.2   17 May 1992 11:05:44   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.1   21 Feb 1992 12:08:32   dz
 *      Art. length: int -> long!
 *      
 *         Rev 1.0   21 Feb 1992 11:22:06   dz
 *      Initial revision.
d21 2
a22 2
#include	<alloc.h>
#include	<dir.h>
d54 2
a55 1
			error( EI_None, "Compressed batch %s", bn );
d185 17
d205 1
a205 3
	char	uu[80], ext[5];

	fnsplit( name, uu, uu, uu, ext );
d207 1
a207 1
	if( stricmp( ext, ".a" ) == 0 || stricmp( ext, "a" ) == 0 )
d216 13
@

/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Cut batch into the pieces
 *
 *      $Log: unbatch.c $
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

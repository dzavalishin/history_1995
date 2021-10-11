/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991-1994 by Infinity Soft
 *
 *	Module 	:	Put article to batch spool for sending back
 *
 *      $Log: ToSpool.c $
 *      Revision 1.4  1995/08/07 14:55:01  dz
 *      Using class App
 *
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
 *         Rev 0.0   28 Feb 1994 11:22:38   dz
 *      
 *
\*/

#include	"unbatch.h"
#include	"vread.h"
#include	<spool.h>
#include	<recode.h>

static bool     is_first_from	( uchar *s );
static bool     is_new_hl		( uchar *s );
static bool     is_cont_hl		( uchar *s );


bool
UnbatchApp::art_to_spool( VReadBase & in, uuinfo &ui )
	{
	news_out_spool_file		spf;

//fprintf( stderr, "art_to_spool() called\n");

	if( spf.create() == Err )
		{
		error( EI_None, "art_to_spool: Can't create spool entry");
		return Err;
		}

	FILE			*out = spf.fp();

	fprintf( out, "Path: %s!%s\n", (const char *)conf.news_path.c_str(), (const char *)ui.path );

	in.seekg( 0L );					// return to the beginning


	const			BUFS = 8 * 1024;
	char			buf[ BUFS ];
	bool			header	= Yes;
	bool			have_hl	= No;
	bool			err = No;

	recoder		r( conf.code_uf.c_str() );
	if( !r.valid() )
		error( EI_None, "Codetable '%s' not found", (const char *)conf.code_uf.c_str() );

	while( 1 )
		{
		char	*ee;

		if( in.gets( (uchar *)buf, BUFS  ) == Err )		// Next line
			{
			if( in.ferr() )
				{
				error( EI_Full, "Problems reading input" );
				err = Yes;
				}

			break;
			}


		r.rs( (uchar *)buf );

		if( (ee = strpbrk( buf, "\r\n" )) != NULL )	// Kill any CRLFs
			*ee = '\0';

		if( header && is_first_from( (uchar *)buf ) )			// From[^:] ??
			continue;					// Kill it

		if( header && is_cont_hl( (uchar *)buf ) )			// headline continuation?
			{
			if( !have_hl )
				{
				error( EI_None, "Continuation withno headline!!");
				err = Yes;
				break;
				}
			fprintf( out, "%s\n", buf );
			continue;
			}

		if( header && is_new_hl( (uchar *)buf ) )			// Oh, new headline
			{
			if( 0 != strnicmp( buf, "path:", 5 ) )
				fprintf( out, "%s\n", buf );
			have_hl = Yes;							// Mark - we have one
			continue;
			}

		header = No;								// End of hdr reached

		fprintf( out, "%s\n", buf );
		}

        if( err )
                {
                spf.abort();
		error( EI_None, "Can't put article to CNews outbound spool" );
                return Err;
                }
        else
                {
                if( spf.close() == Err )
                        return Err;
                if( spf.post( (const char *)ui.newsgroups, (const char *)ui.path ) == Err )
                        return Err;
//fprintf( stderr, "Added as group %s\n", (const char *)ui.newsgroups );
                }


	return Ok;
	}


/****************************************************************************
						Headline detectors
****************************************************************************/

bool
is_first_from( uchar *s )					// Message leader?
	{
	if( strncmp( (const char*)s, "From", 4 ) )			// `From' withno `:'
		return No;
	return ( s[4] == ':' ) ? No : Yes;
	}

bool
is_new_hl( uchar *s )						// Headline?
	{
	while( *s )
		{
		if( isalpha( *s ) || *s == '-' )
			{
			s++;
			continue;
			}

		return ( *s == ':' ) ? Yes : No;
		}

	return No;
	}


bool
is_cont_hl( uchar *s )						// Headline continuation ?
	{
	return ( *s == ' ' || *s == '\t' ) ? Yes : No;
	}







/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUCP header parser
 *
 *      $Log: uuparse.c $
 *      Revision 1.4  1995/08/02 13:46:47  dz
 *      hlget(), new items added
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
 *         Rev 1.7   05 Mar 1993 03:11:38   dz
 *      Direct unbatching
 *      
 *         Rev 1.6.1.0   03 Mar 1993 20:35:02   dz
 *      Temp: trying to implement direct unbatching.
 *      
 *         Rev 1.6   28 Nov 1992 22:38:12   dz
 *      String to const char *, path headline buffer has it's own size.
 *      
 *         Rev 1.5   23 Oct 1992 15:08:08   dz
 *      Support for 'Control' headline.
 *      
 *         Rev 1.4   11 Oct 1992 05:49:56   dz
 *      Support for 'Comment-To' headline
 *      
 *         Rev 1.3   06 Jul 1992 23:55:46   dz
 *      recoder class used
 *      
 *         Rev 1.2   06 Jul 1992 23:53:36   dz
 *      Storing long subject
 *      
 *         Rev 1.1   18 Jun 1992 11:20:26   dz
 *      Nested () in realname
 *      
 *         Rev 1.0   17 May 1992 11:06:12   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *
 *
\*/

#include	"uuparse.h"
#include	"unbatch.h"

#include	"..\..\version.h"
#include	<datec.h>
#include	<recode.h>

#include	<ctype.h>
#include	<stdio.h>

#include	"vread.h"



#define		BUFS	500

				/*********************************************
							   Local functions
				*********************************************/


static bool			is_first_from	( uchar *s );
static bool			is_new_hl		( uchar *s );
static bool			is_cont_hl		( uchar *s );

static void			hlcpy			( char *to, char *from, int n );
static void			hlget			( string &to, char *from );

static void			process_hl		( char *hl, uuinfo *ui );

static bool			uuencoded		( void );




bool
uuparse( VReadBase & in, uuinfo *ui )
	{
	char			buf[ BUFS ];
	char			hl[ BUFS * 2 ];
	bool			have_hl	= No;

	// Clean structure

	char *cp = (char *) ui;
	for( int n = sizeof(uuinfo); n--; )
		*cp++ = '\0';



	// Start from the beginning of message

	in.seekg( 0L );

	while( 1 )
		{
		char	*ee;

		if( in.gets( (uchar *)buf, BUFS  ) == Err )        // Next line
			{

			if( in.ferr() )
				error( EI_Full, "Problems reading article" );

			if( have_hl )
				process_hl( hl, ui );					// Headline in buf

			break;
			}

		recoder		r( conf.code_uf.c_str() );
		if( !r.valid() )
			error( EI_None, "Codetable '%s' not found", (const char *)conf.code_uf.c_str() );

		r.rs( (uchar *)buf );


		if( (ee = strpbrk( buf, "\r\n" )) != NULL )	// Kill any CRLFs
			*ee = '\0';



		if( is_first_from( (uchar *)buf ) )                  // From[~:] ??
			{
			strcpy( hl, buf );						// Get beg. of headline
			have_hl = Yes;							// We have one
			continue;
			}

		if( is_cont_hl( (uchar *)buf ) )                     // headline continuation?
			{
			char	*pp = buf;

			if( !have_hl )
				{
				error( EI_None, "Continuation withno headline!!");
				continue;
				}

			while( *pp == ' ' || *pp == ' ' )		// Skip ws
				pp++;

			strcat( hl, " " );						// Delimiter
			strcat( hl, pp );						// Add continuation
			continue;
			}

		if( have_hl )                               // Not a continuation
			{
			process_hl( hl, ui );                   // Process saved one
			have_hl = No;							// Forget it
			}

		if( is_new_hl( (uchar *)buf ) )                      // Oh, new headline
			{
			strcpy( hl, buf );						// Save it
			have_hl = Yes;							// Mark - we have one
			continue;
			}


		if( uuencoded() )
			ui->uuencoded = Yes;
		else
			ui->uuencoded = No;

		break;
		}

	return Ok;
	}


// rn_dst - where to put realname

static void
decode_from( char *rn_dst, char *dst, char *src )
	{
	char	*cp, *op;

	*rn_dst = '\0';										// Clear realname

	op = dst;

	if( (cp = strchr( src, '<' )) != NULL )             // <addr> form
		{
		char	*lnsp = rn_dst;							// Last non-space + 1
		char	*rnp = src;								// Realname must be here

		while( isspace( *rnp ) )
			rnp++;

		while( rnp < cp )								// Get realname
			{
			char c;

			*rn_dst++ = c = *rnp++;
			if( !isspace( c ) )							// Catch last non-space
				lnsp = rn_dst;
			}

		*lnsp = '\0';									// Kill trail. spaces
		*rn_dst = '\0';

		cp++;											// Skip '<'
		while( *cp )
			{
			if( *cp == '>' )                            // Got it
				{
				*op = '\0';
				return;
				}

			if( *cp < ' ' )
                {
				cp++;
				continue;
				}

//			if( op - dst > 34 )                         // Too long
			if( op - dst > (HLSIZE-1) )					// Too long
				{
//				*dst = '\0';							// Clear
				*op = '\0';								// Close
				return;
				}

			*op++ = *cp++;
			}

		*dst = '\0';									// No '>', clear
		return;
		}

	if( (cp = strchr( src, '(' )) != NULL )				// Have () realname
		{
		if( strchr( cp, ')' ) != NULL )
			{
			char	*lnsp;								// Last non-space + 1

			cp++;
			while( isspace( *cp ) )						// Skip leading spcs
				cp++;

			lnsp = rn_dst;

			int	n_par = 1;								// One '(' found

			while( *cp )								// up to closing ')'
				{
				char c;

				if( *cp == '(' )						// One more '('
					n_par++;

				if( *cp == ')' )						// One more closing
					n_par--;

				if( n_par <= 0 )						// That was the last
					break;

				*rn_dst++ = c = *cp++;
				if( !isspace( c ) )						// Catch last non-space
					lnsp = rn_dst;
				}

			*lnsp = '\0';								// Kill trailing spaces

			*rn_dst = '\0';
			}
		}

	cp = src;
	while( *cp && *cp < ' ' )							// Skip ws
		cp++;

	while( *cp > ' ' )
		{
//        if( op - dst > 34 )                             // Too long
		if( op - dst > (HLSIZE-1) )						// Too long
			{
//			*dst = '\0';								// Clear
			*op = '\0';									// Close
			return;
			}

		*op++ = *cp++;
		}

	*op = '\0';											// Done;
	}




void
process_hl( char *hl, uuinfo *ui )
    {

	// Specially interested in this ones

	if( !strnicmp( hl, "X-FTN-MsgId:", 8 ))
		hlget( ui->ftn_msgid, hl );

	if( !strnicmp( hl, "X-FTN-Reply:", 8 ))
		hlget( ui->ftn_reply, hl );

	if( !strnicmp( hl, "X-FTN-To:", 8 ))
		hlget( ui->ftn_to, hl );

	if( !strnicmp( hl, "Subject:", 8 ))
		hlcpy( ui->subject, hl, HLSIZE );

	if( !strnicmp( hl, "Control:", 8 ))
		hlcpy( ui->control, hl, HLSIZE );

	if( !strnicmp( hl, "X-Batch:", 8 ))
		hlcpy( ui->x_batch, hl, 20 );

	if( !strnicmp( hl, "Message-ID:", 11 ))
		hlcpy( ui->message_id, hl, HLSIZE );

	if( !strnicmp( hl, "Date:", 5 ))
		{
		hlcpy( ui->date, hl, 100 );
		ui->gm_time = getindate( ui->date );	// Parse date/time
		if( ui->gm_time < 0L )					// Parsing error
			error( EI_None, "Can't parse date field '%s'", ui->date );
		}

	if( !strnicmp( hl, "From:", 5 ))
		{
		char	buf[101], fake[101];
		hlcpy( buf, hl, 100 );

		if( strlen( ui->realname ) )
			decode_from( fake, ui->from, buf );
		else
			decode_from( ui->realname, ui->from, buf );
		}

    if( !strnicmp( hl, "Comment-To:", 11 ))
		{
		char	buf[101], fake[101];
		hlcpy( buf, hl, 100 );

        decode_from( ui->comment_to, fake, buf );
        }

	if( !strnicmp( hl, "Reply-To:", 5 ))
		{
		char	buf[101], fake[101];
		hlcpy( buf, hl, 100 );

		if( strlen( ui->realname ) )
			decode_from( fake, ui->replyto, buf );
		else
			decode_from( ui->realname, ui->replyto, buf );
		}


	if( !strnicmp( hl, "Newsgroups:", 11 ))
		hlcpy( ui->newsgroups, hl, HLSIZE-1 );

	if( !strnicmp( hl, "Organization:", 13 ))
		hlcpy( ui->organization, hl, HLSIZE-1 );

	if( !strnicmp( hl, "Path:", 5 ))
        hlcpy( ui->path, hl, PATHHLSIZE-1 );


	hl[0] = '\0';
	}



/****************************************************************************
						Utilitary functions
****************************************************************************/


void
hlcpy( char *to, char *from, int n )        // Extract headline body
    {
	while( *from && *from != ':' )
		from++;

    if( *from != ':' )
        {
		strcpy( to, "Incorrect or empty" );
		return;
		}

	from++;

	while( *from > 0 && *from <= ' ' )
		from++;

	strncpy( to, from, n );
	to[n-1] = '\0';
	}


void
hlget( string &to, char *from )
	{
	while( *from && *from != ':' )
		from++;

	if( *from != ':' )
		{
		to="";
		return;
		}

	from++;

	while( *from > 0 && *from <= ' ' )
		from++;

	to = from;
	}



/****************************************************************************
						Headline detectors
****************************************************************************/

bool
is_first_from( uchar *s )                   // Message leader?
	{
	if( strncmp( (char *)s, "From", 4 ) )			// `From' withno `:'
		return No;

	if( s[4] == ':' )
		return No;

	return Yes;
	}

bool
is_new_hl( uchar *s )                       // Headline?
	{
	while( *s )
		{
		if( isalpha( *s ) || *s == '-' )
			{
			s++;
			continue;
			}

		if( *s == ':' )
			return Yes;

		return No;
		}

	return No;
	}


bool
is_cont_hl( uchar *s )                      // Headline continuation ?
	{
	if( *s == ' ' || *s == '\t' )
			return Yes;

	return No;
	}



	/*************************************************************
					 Detect UUencoded text presence
	*************************************************************/



static bool
uuencoded( void )
	{
	return No;
	}


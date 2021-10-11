head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.14.18.06.24;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.12.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@First RFC_Msg-based version, that compiles and does something
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       UUCP header parser
 *
 *      $Log: UUParse.c $
 *      Revision 1.4  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.3  1995/08/14 18:06:24  dz
 *      Cleanup
 *
 *      Revision 1.2  1995/08/02 13:12:11  dz
 *      RRq support
 *      C Set fixes
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *
 *         Rev 1.11   12 Apr 1993 12:08:30   dz
 *      stdin replaced with in - support for uupc 5.06
 *
 *         Rev 1.10   28 Nov 1992 23:21:12   dz
 *      String to const char *
 *
 *         Rev 1.9   23 Oct 1992 14:46:02   dz
 *      error()
 *
 *         Rev 1.8   18 Sep 1992 02:57:12   dz
 *      using class recoder
 *
 *         Rev 1.7   17 May 1992 12:22:12   dz
 *      Realname parsing, X-Batch headline
 *
 *         Rev 1.6   17 Apr 1992 11:41:46   dz
 *      34 bytes limit on From/Reply-To fields removed.
 *      Now limiting FIDO fileld lenghths elvewhere.
 *
 *         Rev 1.5   07 Apr 1992 12:26:10   dz
 *      Reply-To parsing
 *      conditional decoding
 *
 *         Rev 1.4   09 Feb 1992 11:38:32   dz
 *      Path: added, headline lengths fixed
 *
 *         Rev 1.3   08 Nov 1991 23:11:16   dz
 *      Date was truncated before parsing - fixed.
 *
 *         Rev 1.2   21 Oct 1991 05:14:10   dz
 *      Internet date string parsing added, see datec.lib
 *
 *         Rev 1.0   11 Oct 1991 05:39:24   dz
 *      Initial revision.
 *
 *
 *
\*/

#if 0

#include        "uu2fido.h"




#define         BUFS    500

                                /*********************************************
                                                           Local functions
                                *********************************************/


static bool                     is_first_from   ( uchar *s );
static bool                     is_new_hl               ( uchar *s );
static bool                     is_cont_hl              ( uchar *s );

static void                     hlcpy                   ( char *to, char *from, int n );

static void                     process_hl              ( char *hl, uuinfo *ui );

static bool                     uuencoded               ( void );





bool		
uuparse( const RFC_Msg &msg, uuinfo *uu_info )
    {
#if 0
    char                    buf[ BUFS ];
    char                    hl[ BUFS * 2 ];
    bool                    have_hl = No;
    recoder                 r( conf.code_uf.c_str() );

    if( use_recoder && (!r.valid()) )
        error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );

    // Clean structure

    char *cp = (char *) ui;
    for( int n = sizeof(uuinfo); n--; )
        *cp++ = '\0';

    // Start from the beginning of message

    rewind_to_text();

    while( 1 )
        {
        char    *ee;

        if( fgets( buf, BUFS, in  ) == NULL )        // Next line
            {

            if( ferror( in ) )
                error( EI_Full, "Problems reading stdin");

            if( have_hl )
                process_hl( hl, ui );                // Headline in buf

             break;
             }

         if( use_recoder )
             r.rs( (uchar *)buf );                                                           // Recode


         if( (ee = strpbrk( buf, "\r\n" )) != NULL )     // Kill any CRLFs
             *ee = '\0';

        if( is_first_from( (uchar *)buf ) )                  // From[~:] ??
            {
            strcpy( hl, buf );                               // Get beg. of headline
            have_hl = Yes;                                   // We have one
            continue;
            }

        if( is_cont_hl( (uchar *)buf ) )                     // headline continuation?
            {
            char    *pp = buf;

            if( !have_hl )
                fatal( EC_Got_Insane, EI_None, "Continuation withno headline!!");

            while( *pp == ' ' || *pp == ' ' )                // Skip ws
                pp++;

            strcat( hl, " " );                               // Delimiter
            strcat( hl, pp );                                // Add continuation
            continue;
            }

        if( have_hl )                               // Not a continuation
            {
            process_hl( hl, ui );                   // Process saved one
            have_hl = No;                                                   // Forget it
            }

        if( is_new_hl( (uchar *)buf ) )                      // Oh, new headline
            {
            strcpy( hl, buf );                                              // Save it
            have_hl = Yes;                                                  // Mark - we have one
            continue;
            }


        if( uuencoded() )
            ui->uuencoded = Yes;
        else
            ui->uuencoded = No;

        break;
        }

#endif

    return Ok;
    }




// rn_dst - where to put realname

static void
decode_from( char *rn_dst, char *dst, char *src )
        {
        char    *cp, *op;

        *rn_dst = '\0';                                                                         // Clear realname

        op = dst;

        if( (cp = strchr( src, '<' )) != NULL )             // <addr> form
                {
                char    *lnsp = rn_dst;                                                 // Last non-space + 1
                char    *rnp = src;                                                             // Realname must be here

                while( isspace( *rnp ) )
                        rnp++;

                while( rnp < cp )                                                               // Get realname
                        {
                        char c;

                        *rn_dst++ = c = *rnp++;
                        if( !isspace( c ) )                                                     // Catch last non-space
                                lnsp = rn_dst;
                        }

                *lnsp = '\0';                                                                   // Kill trail. spaces
                *rn_dst = '\0';

                cp++;                                                                                   // Skip '<'
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

//                      if( op - dst > 34 )                         // Too long
                        if( op - dst > (HLSIZE-1) )                                     // Too long
                                {
//                              *dst = '\0';                                                    // Clear
                                *op = '\0';                                                             // Close
                                return;
                                }

                        *op++ = *cp++;
                        }

                *dst = '\0';                                                                    // No '>', clear
                return;
                }

        if( (cp = strchr( src, '(' )) != NULL )                         // Have () realname
                {
                if( strchr( cp, ')' ) != NULL )
                        {
                        char    *lnsp;                                                          // Last non-space + 1

                        cp++;
                        while( isspace( *cp ) )                                         // Skip leading spcs
                                cp++;

                        lnsp = rn_dst;
                        while( *cp && *cp != ')' )                                      // up to closing ')'
                                {
                                char c;

                                *rn_dst++ = c = *cp++;
                                if( !isspace( c ) )                                             // Catch last non-space
                                        lnsp = rn_dst;
                                }

                        *lnsp = '\0';                                                           // Kill trailing spaces

                        *rn_dst = '\0';
                        }
                }

        cp = src;
        while( *cp && *cp < ' ' )                                                       // Skip ws
                cp++;

        while( *cp > ' ' )
                {
//        if( op - dst > 34 )                             // Too long
                if( op - dst > (HLSIZE-1) )                                             // Too long
                        {
//                      *dst = '\0';                                                            // Clear
                        *op = '\0';                                                                     // Close
                        return;
                        }

                *op++ = *cp++;
                }

        *op = '\0';                                                                                     // Done;
        }




void
process_hl( char *hl, uuinfo *ui )
    {

        // Specially interested in this ones

        if( !strnicmp( hl, "Subject:", 8 ))
                hlcpy( ui->subject, hl, 71 );

    if( !strnicmp( hl, "X-Batch:", 8 ))
                hlcpy( ui->x_batch, hl, 20 );

    if( !strnicmp( hl, "Date:", 5 ))
        {
                hlcpy( ui->date, hl, 100 );
                ui->gm_time = getindate( ui->date );    // Parse date/time
                if( ui->gm_time < 0L )                                  // Parsing error
                        error( EI_None, "Can't parse date field '%s'", ui->date );
                }

    if( !strnicmp( hl, "From:", 5 ))
        {
                char    buf[101], fake[101];
                hlcpy( buf, hl, 100 );

                if( strlen( ui->realname ) )
                        decode_from( fake, ui->from, buf );
                else
                        decode_from( ui->realname, ui->from, buf );
                }

        if( !strnicmp( hl, "Reply-To:", 5 ))
                {
                char    buf[101], fake[101];
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
                hlcpy( ui->path, hl, HLSIZE-1 );


        if( !strnicmp( hl, "Return-Receipt-To:", 18 ))
                ui->has_return_receipt_to = Yes;

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


/****************************************************************************
                                                Headline detectors
****************************************************************************/

bool
is_first_from( uchar *s )                   // Message leader?
    {
        if( strncmp( (const char *)s, "From", 4 ) )                     // `From' withno `:'
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



#endif // 0

@


1.4
log
@Before switching to rfc_msg
@
text
@d8 3
d61 2
d89 2
a90 2
bool
uuparse( uuinfo *ui )
d443 4
@


1.3
log
@Cleanup
@
text
@d8 3
d87 1
d172 2
@


1.2
log
@RRq support
C Set fixes
@
text
@d5 1
a5 1
 *	Module 	:	UUCP header parser
d8 7
a14 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
a15 1
 *      
d18 1
a18 1
 *      
d21 1
a21 1
 *      
d24 1
a24 1
 *      
d27 1
a27 1
 *      
d30 1
a30 1
 *      
d34 1
a34 1
 *      
d38 1
a38 1
 *      
d41 1
a41 1
 *      
d44 1
a44 1
 *      
d47 1
a47 1
 *      
d55 1
a55 1
#include	"uu2fido.h"
d60 1
a60 1
#define		BUFS	500
d62 3
a64 3
				/*********************************************
							   Local functions
				*********************************************/
d67 3
a69 3
static bool			is_first_from	( uchar *s );
static bool			is_new_hl		( uchar *s );
static bool			is_cont_hl		( uchar *s );
d71 1
a71 1
static void			hlcpy			( char *to, char *from, int n );
d73 1
a73 1
static void			process_hl		( char *hl, uuinfo *ui );
d75 1
a75 1
static bool			uuencoded		( void );
d83 5
a87 5
	{
	char			buf[ BUFS ];
	char			hl[ BUFS * 2 ];
	bool			have_hl	= No;
	recoder			r( conf.code_uf.c_str() );
d89 2
a90 2
	if( use_recoder && (!r.valid()) )
		error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
d92 1
a92 1
	// Clean structure
d94 3
a96 3
	char *cp = (char *) ui;
	for( int n = sizeof(uuinfo); n--; )
		*cp++ = '\0';
d98 1
d100 1
d102 1
a102 5
	// Start from the beginning of message

	rewind_to_text();

	while( 1 )
d104 1
a104 1
		char	*ee;
d106 1
a106 1
		if( fgets( buf, BUFS, in  ) == NULL )        // Next line
d109 2
a110 8
			if( ferror( in ) )
				error( EI_Full, "Problems reading stdin");

			if( have_hl )
				process_hl( hl, ui );					// Headline in buf

			break;
			}
d112 2
a113 13
		if( use_recoder )
			{
#if 0
			for( p = buf; *p; p++ )                     // KOI8 to alt. code
				{
				*p = koi8_to_pc_tab[*p];
				if( *p == 'Œ'+1 )                       // Russian 'H'
					*p = 'H';
				}
#else
			r.rs( (uchar *)buf );								// Recode
#endif
			}
d115 2
d118 2
a119 2
		if( (ee = strpbrk( buf, "\r\n" )) != NULL )	// Kill any CRLFs
			*ee = '\0';
d122 2
d127 4
a130 4
			strcpy( hl, buf );						// Get beg. of headline
			have_hl = Yes;							// We have one
			continue;
			}
d134 1
a134 1
			char	*pp = buf;
d136 2
a137 2
			if( !have_hl )
				fatal( EC_Got_Insane, EI_None, "Continuation withno headline!!");
d139 2
a140 2
			while( *pp == ' ' || *pp == ' ' )		// Skip ws
				pp++;
d142 4
a145 4
			strcat( hl, " " );						// Delimiter
			strcat( hl, pp );						// Add continuation
			continue;
			}
d150 2
a151 2
			have_hl = No;							// Forget it
			}
d155 10
a164 10
			strcpy( hl, buf );						// Save it
			have_hl = Yes;							// Mark - we have one
			continue;
			}


		if( uuencoded() )
			ui->uuencoded = Yes;
		else
			ui->uuencoded = No;
d166 2
a167 2
		break;
		}
d169 2
a170 2
	return Ok;
	}
d179 4
a182 2
	{
	char	*cp, *op;
d184 1
a184 1
	*rn_dst = '\0';										// Clear realname
d186 4
a189 1
	op = dst;
d191 2
a192 28
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
d194 43
a236 1
			if( *cp < ' ' )
d238 27
a264 48
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
			while( *cp && *cp != ')' )					// up to closing ')'
				{
				char c;

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
d266 2
a267 2
	while( *cp > ' ' )
		{
d269 6
a274 6
		if( op - dst > (HLSIZE-1) )						// Too long
			{
//			*dst = '\0';								// Clear
			*op = '\0';									// Close
			return;
			}
d276 2
a277 2
		*op++ = *cp++;
		}
d279 2
a280 2
	*op = '\0';											// Done;
	}
d289 1
a289 1
	// Specially interested in this ones
d291 2
a292 2
	if( !strnicmp( hl, "Subject:", 8 ))
		hlcpy( ui->subject, hl, 71 );
d295 1
a295 1
		hlcpy( ui->x_batch, hl, 20 );
d299 5
a303 5
		hlcpy( ui->date, hl, 100 );
		ui->gm_time = getindate( ui->date );	// Parse date/time
		if( ui->gm_time < 0L )					// Parsing error
			error( EI_None, "Can't parse date field '%s'", ui->date );
		}
d307 2
a308 2
		char	buf[101], fake[101];
		hlcpy( buf, hl, 100 );
d310 5
a314 5
		if( strlen( ui->realname ) )
			decode_from( fake, ui->from, buf );
		else
			decode_from( ui->realname, ui->from, buf );
		}
d316 4
a319 4
	if( !strnicmp( hl, "Reply-To:", 5 ))
		{
		char	buf[101], fake[101];
		hlcpy( buf, hl, 100 );
d321 5
a325 5
		if( strlen( ui->realname ) )
			decode_from( fake, ui->replyto, buf );
		else
			decode_from( ui->realname, ui->replyto, buf );
		}
d328 2
a329 2
	if( !strnicmp( hl, "Newsgroups:", 11 ))
		hlcpy( ui->newsgroups, hl, HLSIZE-1 );
d331 2
a332 2
	if( !strnicmp( hl, "Organization:", 13 ))
		hlcpy( ui->organization, hl, HLSIZE-1 );
d334 2
a335 2
	if( !strnicmp( hl, "Path:", 5 ))
		hlcpy( ui->path, hl, HLSIZE-1 );
d338 2
a339 2
	if( !strnicmp( hl, "Return-Receipt-To:", 18 ))
		ui->has_return_receipt_to = Yes;
d341 2
a342 2
	hl[0] = '\0';
	}
d347 1
a347 1
						Utilitary functions
d354 2
a355 2
	while( *from && *from != ':' )
		from++;
d359 12
a370 12
		strcpy( to, "Incorrect or empty" );
		return;
		}

	from++;

	while( *from > 0 && *from <= ' ' )
		from++;

	strncpy( to, from, n );
	to[n-1] = '\0';
	}
d374 1
a374 1
						Headline detectors
d380 5
a384 2
	if( strncmp( (const char *)s, "From", 4 ) )			// `From' withno `:'
		return No;
d386 2
a387 5
	if( s[4] == ':' )
		return No;
		
	return Yes;
	}
d396 3
a398 3
			s++;
			continue;
			}
d400 2
a401 2
		if( *s == ':' )
			return Yes;
d403 2
a404 2
		return No;
		}
d406 2
a407 2
	return No;
	}
d414 1
a414 1
			return Yes;
d416 2
a417 2
	return No;
	}
d421 3
a423 3
	/*************************************************************
					 Detect UUencoded text presence
	*************************************************************/
d430 2
a431 2
	return No;
	}
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/uuparse.c_v  $
d126 1
a126 1
			r.rs( buf );								// Recode
d136 1
a136 1
        if( is_first_from( buf ) )                  // From[~:] ??
d143 1
a143 1
        if( is_cont_hl( buf ) )                     // headline continuation?
d164 1
a164 1
        if( is_new_hl( buf ) )                      // Oh, new headline
d302 1
a302 1
	if( !strncmpi( hl, "Subject:", 8 ))
d305 1
a305 1
    if( !strncmpi( hl, "X-Batch:", 8 ))
d308 1
a308 1
    if( !strncmpi( hl, "Date:", 5 ))
d316 1
a316 1
    if( !strncmpi( hl, "From:", 5 ))
d327 1
a327 1
	if( !strncmpi( hl, "Reply-To:", 5 ))
d339 1
a339 1
	if( !strncmpi( hl, "Newsgroups:", 11 ))
d342 1
a342 1
	if( !strncmpi( hl, "Organization:", 13 ))
d345 1
a345 1
	if( !strncmpi( hl, "Path:", 5 ))
d349 3
d391 1
a391 1
	if( strncmp( s, "From", 4 ) )			// `From' withno `:'
@

/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       UUCP header parser
 *
 *      $Log: UUParse.c $
 *      Revision 1.5  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
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


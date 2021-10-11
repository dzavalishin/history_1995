head	1.3;
access;
symbols;
locks;
comment	@ *  @;


1.3
date	95.11.04.18.32.53;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *  Module  :   RFC Message Class: Loading
 *
 *      $Log: RFC_Load.c $
 *      Revision 1.2  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.1  1995/11/02 14:25:20  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include    <log.h>
#include    <ctype.h>

#include    "hl.h"
#include    "RFC_Msg.h"

#include    <setup.h>
#include    "..\..\version.h"




// Headline continuation ?
static bool
is_cont_hl( const string &ss )
    {
    const char *s = ss;

    if( *s == ' ' || *s == '\t' )
            return Yes;

    return No;
    }


bool
RFC_Msg::load( FILE *fp )
    {
    string  s, hl;
    bool    status;
    bool    in_header = Yes;
    bool    have_hl = No;
    
    froms_invalid_v = Yes;
    
    size_v = 0;
    
    if( lr != 0 )
        {
        if( !lr->valid() )
            error( EI_None, "Code table is not valid in RFC_Msg::load" );
        }
    
        {
        char buf[2000];
        sprintf( buf, "Received: by %s (UU2Fido %s); %s",
                (const char *)conf.def_domain.c_str(),
                U1_VER_STR,
                timeline( time(NULL), (const uchar *)conf.tz.c_str() )
               );
        add_headline( buf );
        }
    
    
    // load headlines & from_, iof present
    while( (status = s.load( fp )) == Ok )
        {
        
        size_v += s.length();

        s.strip_crlf();

        if( lr != 0 )
            lr->rs( s );              // Recode

        if(  s.is_empty() )
            break;

        if( is_cont_hl( s ) )       // headline continuation?
            {
            if( !have_hl )
                error( EI_None, "Continuation withno headline: '%s'", (const char *)s);

            s.strip_leading_ws();

            hl += string(" ");                      // Delimiter
            hl += s;                                // Add continuation
            continue;
            }

        if( have_hl )                               // Not a continuation
            {
            add_headline( hl );
            have_hl = No;                           // Forget it
            }

        hl = s;                                     // Save it
        have_hl = Yes;                              // Mark - we have one
        }

    if( have_hl )
        add_headline( hl );

    if( ferror( fp ) )
        return Err;

    // load text
    while( (status = s.load( fp )) == Ok )
        {
        size_v += s.length();

        s.strip_crlf();
        
        if( lr != 0 )
            lr->rs( s );              // Recode

        text.add( s );
        }

    if( ferror( fp ) )
        return Err;

    return Ok;
    }


bool
RFC_Msg::save( FILE *fp )
    {
    hl_Cursor hc( headline );
    string s;

    if( sr != 0 )
        if( !sr->valid() )
            error( EI_None, "Code table is not valid in RFC_Msg::save" );


    for( hc.setToFirst(); hc.isValid(); hc.setToNext() )
        {
        const char *k = hc.element().key();
        const hl_val_Bag &vb = hc.element().val();

        hl_val_Cursor hvc( vb );

        for( hvc.setToFirst(); hvc.isValid(); hvc.setToNext() )
            {
            s = hvc.element();

            if( sr != 0 )
                sr->rs( s );              // Recode

            fprintf( fp, "%s: %s\n", k, (const char *)s );
            }


        }

    fprintf( fp, "\n" );

    txt_Cursor tc( text );

    for( tc.setToFirst(); tc.isValid(); tc.setToNext() )
        {
        s = tc.element();

        if( sr != 0 )
            sr->rs( s );              // Recode

        fprintf( fp, "%s\n", (const char *)s );
        }

    if( ferror( fp ) )
        return Err;

    return Ok;
    }


bool
RFC_Msg::save( RFC_Msg &dest )
    {
    hl_Cursor hc( headline );
    string s;

    if( sr != 0 )
        if( !sr->valid() )
            error( EI_None, "Code table is not valid in RFC_Msg::save" );


    for( hc.setToFirst(); hc.isValid(); hc.setToNext() )
        {
        const string &k = hc.element().key();
        const hl_val_Bag &vb = hc.element().val();

        hl_val_Cursor hvc( vb );

        for( hvc.setToFirst(); hvc.isValid(); hvc.setToNext() )
            {
            s = hvc.element();
            if( sr != 0 ) sr->rs( s );              // Recode
            dest.text.add( k + string(": ")+s );
            }
        }

    dest.text.add( "" );

    txt_Cursor tc( text );

    for( tc.setToFirst(); tc.isValid(); tc.setToNext() )
        {
        s = tc.element();
        if( sr != 0 )  sr->rs( s );              // Recode
        dest.text.add( s );
        }

    return Ok;
    }




//
// rn_dst - where to put realname
// dst    - address
// src    - input mess
//

#define HLSIZE   (10*1024)

static void
do_decode_from( char *rn_dst, char *dst, const char *src )
    {
    const char    *cp;
    char          *op;

    *rn_dst = '\0';                                                                         // Clear realname

    op = dst;

    if( (cp = strchr( src, '<' )) != NULL )             // <addr> form
        {
        char    *lnsp = rn_dst;                                                 // Last non-space + 1
        const char    *rnp = src;                                                             // Realname must be here

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


void RFC_Msg::
decode_from( string &realname_out, string &address_out, const string &in )
    {
    char  rn_out[HLSIZE+1], ad_out[HLSIZE+1];
    
    do_decode_from( rn_out, ad_out, in );
    
    realname_out = rn_out;
    address_out = ad_out;
    }

    


void RFC_Msg::             
parse_froms()
    {
    string temp;
    string replyto_rn, replyto_ad;
    string from_rn, from_ad;
    
    froms_invalid_v = No;
    
    reply_address_v = "/dev/null";
    from_realname_v = "Nobody";

    bool has_replyto = headline.get( temp, "Reply-To" );
    if (has_replyto) 
        {
        decode_from( replyto_rn, replyto_ad, temp );
        reply_address_v = replyto_ad;
        from_realname_v = replyto_rn;
        }
    
    bool has_from = headline.get( temp, "From" );
    if (has_from) 
        {
        decode_from( from_rn, from_ad, temp );
        if (!has_replyto) 
            {
            reply_address_v = from_ad;
            from_realname_v = from_rn;
            }
        }
    
    errors_reply_address_v = reply_address_v;
    
    if(headline.get( temp, "Errors-To" ))
        {
        string dummy;
        decode_from( errors_reply_address_v, dummy, temp );
        }
    
    }

@


1.2
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
d64 1
a64 1
        sprintf( buf, "Received: by %s (UU2Fido %s) %s",
d118 1
d121 3
d187 39
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: RFC_Msg.c $
d17 1
d22 5
d48 5
a52 1

d54 1
d56 14
a69 2
            error( EI_None, "Code table is not valid" );

d73 2
d132 6
d147 8
a154 1
            fprintf( fp, "%s: %s\n", k, (const char *)hvc.element() );
d164 8
a171 1
        fprintf( fp, "%s\n", (const char *)tc.element() );
d179 174
@

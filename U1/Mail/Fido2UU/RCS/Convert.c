head	1.9;
access;
symbols;
locks;
comment	@ * @;


1.9
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.03.09.11.26.14;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.11.05.18.53.26;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.05.12.56.20;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.13.33;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.9
log
@current
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Convertor
 *
 *      $Log: Convert.c $
 *      Revision 1.8  1996/03/09 11:26:14  dz
 *      Fixed formatter. (Added \n between lines).
 *
 *      Revision 1.7  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.6  1995/11/05 18:53:26  dz
 *      Before moving to FTN_Msg
 *
 *      Revision 1.5  1995/11/05 12:56:20  dz
 *      Save of current (working?) code
 *
 *      Revision 1.4  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.3  1995/08/02 13:13:33  dz
 *      appearance of Received: line
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *      
 *         Rev 1.20   07 Jun 1993 16:40:24   dz
 *      CC system rewritten
 *      
 *         Rev 1.19.1.0   25 Mar 1993 04:19:20   dz
 *      Temp - changing rmail interface
 *      
 *         Rev 1.19   28 Nov 1992 23:10:34   dz
 *      String to const char *
 *      
 *         Rev 1.18   11 Nov 1992 05:55:28   dz
 *      Using uucp_addr
 *      
 *         Rev 1.17   23 Oct 1992 14:35:02   dz
 *      from_, return-receipt-to
 *      
 *         Rev 1.16   18 Sep 1992 02:46:38   dz
 *      exit codes modified
 *      writing From_ only if not ache_uupc
 *
 *         Rev 1.15   29 Jun 1992 15:09:20   dz
 *      Code for ECHO2UU support removed,
 *      Temp. file creation logic rewritten - now creating
 *      unique file, so two or more copies of FIDO2UU can
 *      work simulteoncly innetwork or mtask environment.
 *
 *         Rev 1.14.1.0   29 Jun 1992 11:46:58   dz
 *      tempnam() used
 *
 *         Rev 1.14   29 Jun 1992 04:39:10   dz
 *      From_ line generation written
 *
 *         Rev 1.13   18 Jun 1992 10:36:06   dz
 *      Cc: address conversion
 *
 *         Rev 1.12   03 Mar 1992 19:29:34   dz
 *      Diagnostics moved to show From/To as early as possible
 *
 *         Rev 1.10   09 Feb 1992 00:56:52   dz
 *      Realname conversion fixed - space & dash now allowed to pass
 *
 *         Rev 1.9   09 Feb 1992 00:49:34   dz
 *      Domains checking added for non-registered users
 *
 *         Rev 1.8   09 Feb 1992 00:27:20   dz
 *      Cc: feature added
 *
 *         Rev 1.7   04 Feb 1992 00:13:36   dz
 *      rmail.exe interface generalized, external code table
 *      translation connected.
 *      NB!! Echo2UU now generates Message-Id itself.
 *
 *         Rev 1.6   27 Jan 1992 23:18:48   dz
 *      Attachments encoding engine connected
 *      Gate control lines parser started
 *      system() stuff removed
 *
 *         Rev 1.5   11 Oct 1991 05:22:50   dz
 *      Date: format corrected to be compartible with RFC822
 *
 *         Rev 1.4   10 Oct 1991 19:31:32   dz
 *      ECHO2UU support added (No From: line added in this case)
 *
 *         Rev 1.3   24 Sep 1991 12:24:56   dz
 *      Optional headlines recognition/processing added
 *
 *         Rev 1.2   29 Aug 1991 07:03:36   dz
 *      'Received:' header line format fixed.
 *
 *         Rev 1.1   29 Aug 1991 06:42:12   dz
 *      Replaced system() with spawn()
 *      Logging rearranged
 *      Minor bug fixes
 *
 *         Rev 1.0   27 Aug 1991 02:47:14   dz
 *      Initial revision.
 *
 *
\*/


#include    "fido2uu.h"
#include    "..\..\version.h"

#include    "datec.h"
#include    "rfc_msg.h"

#include    "encode.h"
#include    "recode.h"
#include    "freedom.h"
#include    "stat.h"

#include    <ctype.h>
#include    <share.h>
#include    <time.h>
#include    <io.h>
#include    <fcntl.h>

#include    "cc_bag.h"



static bool      is_headline( string &key, string &val, const char *s );
//static bool	 is_headline( const char *s );
static bool      is_to_headline( string s );
static bool      is_xcc_headline( string &, string s );
static bool      is_cc_headline( string &, string s );

static void      format( RFC_Msg &msg, string &ss );

// TODO: Skip flag must be redone - if set at the beginning,
// most of the following code will fail!



void
Fido2uuApp::convert( FTN_Msg &fm, bool &r_report, bool &r_err )
    {
    //FILE        *fp;
    RFC_Msg      msg;
    time_t       t;
    char         uu_to[100];                     // Send to this address
    uucp_addr    uu_from;                        // Send from this address
    bool	 reg_user;                       // Registered user
    bool         skip = No;
    fido_user    fido_from;
    fmsg_attr    at;
    recoder      rec( conf.code_fu.c_str() );     // Get codetable
    char         temp_name[100];
    long         bytes_sent;
    long         files_sent;
    char         buf[200];

    CC_Bag       deliver_cc;                      // List of rmail args (cc:)
    CC_Bag       list_cc;      	                  // List of things to put to cc headline

    if( !rec.valid() )	                          // Can't init recoder
        fatal( EC_Incorrect_Setup, EI_None, "Can't find codetable '%s'", (const char *)conf.code_fu.c_str() );
    
    msg.set_saver_recoder( &rec );
    
    time( &t );
    fm.get_from( fido_from );
    fm.get_attr( at );
    
    /*
    for( int i = 10; i; i-- )
        {
        char	*tf = tempnam( getenv("TEMP"), "UU2TF" );
        if( tf == NULL )
            fatal( EC_OS_Error, EI_Full, "Can't find unique file name!" );

        int fd = _sopen( tf, O_RDWR|O_CREAT|O_EXCL|O_BINARY, SH_DENYWR, 0666 );			// Open temp file

        fp = fdopen( fd, "w+b" );

        if( fd < 0 || fp != NULL )
            {
            strncpy( temp_name, tf, sizeof(temp_name)-1 );
            free( tf );
            break;
            }
        }

    if( fp == NULL )				// Can't ?
        fatal( EC_IO_Error, EI_Full, "Can't create temporary file" );
    */

    if( get_address( uu_to, fm ) == Err )
        skip = Yes;

    if( skip || (get_from( &reg_user, uu_from, fido_from ) == Err) )
        skip = Yes;
    else
        if( strlen( (const char *)uu_from ) == 0 )
            fatal( EC_UU2_Error, EI_None, "Can't build 'From:'???" );
    
        {
        char from_[240], *p, buf[200];
        
        string fulla = (string &)uu_from, aname, adomain;
        
        if( !fulla.parse( aname, '@@', adomain ) ) 
            error( EI_None, "There MUST be @@: '%s'!", (const char *)fulla );
        
        sprintf( from_, "%.90s!%.90s %s",
                (const char *) adomain,
                (const char *) aname,
                timeline( t, (const uchar *)conf.tz.c_str() ));
        /*
        if( conf.at_to_percent_in_from_ )
            {
            while( (p = strchr( from_, '@@' )) != NULL )
                *p = '%';
            }
        */
        
        buf[0]= '\0';
        switch( conf.uucp ) {
        case UUCP_Ache30:
            break;
        case UUCP_Waffle:
            sprintf( buf, "%s", from_ );
            break;
        default:
            sprintf( buf, "%s remote from %s", from_, (const char *)conf.def_domain.c_str() );
            break;
        }
        debug("from_: %s", buf );
        msg.envelope.add_from( buf );
        }
    
        {
        char buf[200];
        sprintf( buf, "by %s (FIDO2UU %s) with FTN; %s",
                (const char *)conf.def_domain.c_str(),
                U1_VER_STR,
                timeline( t, (const uchar *)conf.tz.c_str() )
               );
        msg.headline.add_hl( "Received", buf );
        }

    uchar	rep_fido[40];
    uchar	*cp;

    strncpy( (char *)rep_fido, fido_from.name(), 37 );
    rep_fido[36] = '\0';
    rec.rs( rep_fido );
    for( cp = rep_fido; *cp; cp++ )
        {
        if( (*cp < 'A') && (*cp != ' ') && (*cp != '-') )
                *cp = '.';
        }

    if( at & FF_ReturnReceiptRequest )   // Confirmation Request
        {
        log("#", "Adding return-receipt-to field" );
        msg.headline.add_hl( "Return-Receipt-To", (string)uu_from );
        }


    if( !skip )
        {
        string   subj;

        fm.get_subj( subj );

        fprintf( stderr, "From: %s\n", rep_fido );
        fprintf( stderr, "To  : %s\n", uu_to );

        log( "ua", "To: %s", uu_to );
        log( "ua", "From: %s <%s>", rep_fido, (const char *) uu_from );
        log( "u", "Subject: %s", subj );
        log( "u", "Date: %s", timeline( t, (const uchar *)conf.tz.c_str() ) );

        //fprintf( fp, "To: %s\n", uu_to );
        msg.headline.add_hl( "To", uu_to );
        
        sprintf( buf, "%s <%s>", rep_fido, (const char *) uu_from );
        msg.headline.add_hl( "From", buf );
        msg.headline.add_hl( "Reply-To", buf );
        
        sprintf( buf, "<%lX@@%s>", t, (const char *)conf.def_domain.c_str() );
        msg.headline.add_hl( "Message-Id", buf );
        
        //rec.rs( subj );
        //fprintf( fp, "Subject: %s\n", subj );
        msg.headline.add_hl( "Subject", subj );

        //fprintf( fp, "Date: %s\n", (const char *)timeline( t, (const uchar *)conf.tz.c_str() ) );
        msg.headline.add_hl( "Date", timeline( t, (const uchar *)conf.tz.c_str() ) );
        }

    if( (!reg_user) && (!is_free_address( uu_to )))
        {
        error( EI_None, "Access to '%s' denied at this gate, sorry.", uu_to );
        skip = Yes;
        }

    if( !skip )
        {
        string  buf;
        bool    header = Yes;

        fm.rewind();
        while( !fm.gets( buf ) )
            {
            string     morecc;
            string     fidocc;  // FIDO cc
            string hl_key, hl_val;
            //rec.rs( buf );

            if( header && is_headline( hl_key, hl_val, buf ) )
                {
                if( is_gate_control( buf ) == Yes )	// Gate control line
                        continue;

                if( is_xcc_headline( morecc, buf ) )
                    {
                    if( !is_free_address( morecc ) )
                        {
                        error( EI_None, "Access to %s denied at this gate", (const char *)morecc );
                        r_report = Yes;
                        }
                    else
                        {
                        deliver_cc.add( morecc );
                        list_cc.add( morecc );
                        log( "ua", "Cc: %s", (const char *)morecc );
                        }
                    continue;
                    }

                if( is_cc_headline( fidocc, buf ) )
                    {
                    uucp_addr	convcc;	// Converted to uucp form FIDO cc

                    if( fidoaddr2uuaddr( convcc, fidocc ) == Err )
                        log( "#", "Can't convert FIDO Cc: %s", (const char *)fidocc );
                    else
                        {
                        list_cc.add( (String &)convcc );
                        log( "ua", "FIDO Cc: %s", (const char *)(((String &)convcc).c_str()) );
                        }
                    continue;
                    }

                if( !is_to_headline( buf ) )
                    {
                    //fprintf( fp, "%s\n", (const char *)buf );		// Head line
                    msg.headline.add_hl( hl_key, hl_val );
                    debug("Headline (%s: %s)", (const char *)hl_key, (const char *)hl_val );
                    }

                continue;
                }

            if( header )
                {
                header = No;

                int	ncc;

                if( (ncc = list_cc.numberOfElements()) > 0 )
                    {
                    //fprintf( fp, "Cc: " );

                    CC_BagIterator i( list_cc );
                    int n;

                    for( n = 0, i.setToFirst(); i.isValid(); i.setToNext(), n++ )
                        {
                        String &cc = (String &)list_cc.elementAt(i);

                        if( n > 4 )
                            {
                            n = 0;
                            //fprintf( fp, "\n    " );
                            }
                        /*
                        fprintf( fp, "%s%s",
                                (const char *)cc.c_str(),
                                --ncc ? ", " : "\n"
                                );
                        */
                        msg.headline.add_hl( "Cc", cc );
                        }
                    }

                //fprintf( fp, "\n" );				// Empty line - end of hdr
                }
            
            /*
            if( strnicmp( buf, "From", 4 ) == 0 )
            fprintf( fp, ">" );
            */
//            fprintf( fp, "%s\n", buf );				// Text line
            format( msg, buf );	      		// Text line
            //fprintf( fp, "\n" );		// Empty line
            }

        //fprintf( fp, "\n" );						// Empty line
        msg.text.add( "" );
        
        encode_attaches( fm, msg, &files_sent );  // Put in attached files

        }
    
    /*
    if( ferror( fp ) )
        {
        error( EI_Full, "Can't write msg to temp file - msg skipped" );
        fclose( fp );
        r_err = Yes;
        return;
        }


    fflush( fp );
    */
    
    
    //bytes_sent = filelength( fileno( fp ));
    bytes_sent = 0;

    if( !skip )
        {
        if( run_uusmail( uu_from, uu_to, deliver_cc, msg ) == Err )
            skip = Yes;
        else
            {
            add_stat_fido( fido_from, (const unsigned char *)uu_to, No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );

            CC_BagIterator i( list_cc );

            for( i.setToFirst(); i.isValid(); i.setToNext() )
                {
                String &cc = (String &)list_cc.elementAt(i);
                add_stat_fido( fido_from, (const uchar *)cc.c_str(), No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );
                }
            }
        }
    
    /*
    if( fclose( fp ) )
        {
        error( EI_Full, "Can't close temp. file" );
        r_err = Yes;
        return;
        }

    if( unlink( temp_name ) )
        error( EI_Full, "Can't delete temp '%s'", temp_name );
    */
    
    if( skip )
        r_err = Yes;

    }







		/*********************************************
                          Headlines reckognition
		*********************************************/

// Headline?
bool is_headline( string &key, string &val, const char *s )
    {
    const char *orig = s;
    while( *s )
        {
        if( isalpha( *s ) || *s == '-' )
            {
            s++;
            continue;
            }

        if( *s == ':' )
            {
            key = orig;
            key = key.substr( 0, s-orig );
            s++;
            while( isspace(*s) ) s++;
            val = s;
            return Yes;
            }

        return No;
        }

    return No;
    }


// 'To:' ?
bool is_to_headline( string s )
    {
    s.strip_leading_ws();  // Skip spaces, tabs...

    if( strnicmp( s, "to", 2 ) )        return No;
    s.strip_leading( 2 );  // Delete 'to'

    s.strip_leading_ws();  // Skip spaces, tabs...

    if( s[0] != ':' )                   return No;
    return Yes;
    }



static bool
is_xcc_headline( string &morecc, string s )
    {
    s.strip_leading_ws();  // Skip spaces, tabs...
    
    if( strnicmp( s, "xcc", 3 ) )        return No;
    s.strip_leading( 3 );  // Just delete 'cc'

    s.strip_leading_ws();  // Skip spaces, tabs...
    if( s[0] != ':' )                    return No;
    s.strip_leading( 1 );  // Delete ':'
    s.strip_leading_ws();  // Skip spaces, tabs...
    morecc = s;
    
    return Yes;
    }

static bool
is_cc_headline( string &ccdata, string s )
    {
    s.strip_leading_ws();  // Skip spaces, tabs...

    if( strnicmp( s, "cc", 2 ) )  return No;
    s.strip_leading( 2 );  // Just delete 'cc'
    s.strip_leading_ws();  // Skip spaces, tabs...

    if( s[0] != ':' )             return No;
    s.strip_leading( 1 );  // Delete ':'
    s.strip_leading_ws();  // Skip spaces, tabs...
    
    ccdata = s;
    return Yes;
    }




/****************************************************************************
							Cc: list checker
****************************************************************************/

/*
static bool
bad_cc_list( uchar *ccl )
	{
	char	*p, *cclc;

	cclc = strdup( ccl );
	if( cclc == NULL )
		{
		error( EI_None, "Out of memory checking cc list" );
		return Yes;
		}

	p = strtok( cclc, " \t\r\n" );
	while( p )
		{
		debug("Testing cc: %s", p );
		if( !is_free_address( p ) )
			{
			free( cclc );
			error( EI_None, "Access denied: Bad cc: address '%s'", p );
			return Yes;
			}
		p = strtok( NULL, " \t\r\n" );
		}

	free( cclc );
	return No;
	}

*/


const right_margin = 75;

static void format( RFC_Msg &msg, string &ss )
    {
    const char *s = ss;    // Begin of current line
    const char *scan;      // Scan pointer
    const char *end;       // Rightmost possible end, this character excluded
    
    if( *s == 0 )
        {
        msg.text.add( "" );
        return;
        }
        
    
    while( *s )
        {
        end = s;
        for( scan = s; scan-s < right_margin; scan++ )
            {
            if( *scan == 0 )                  // End of everything?
                {
                end = scan;
                break;
                }
            if( *scan <= ' ' && *scan > 0 )   // Any control or space
                end = scan+1;
            }
        
        // Here we have a piece of line shorter than right_margin characters
        // or nothing. If nothing, we'll cut exactly right_margin characters
        // and print it out.
        // Start of piece is 's', end - 'end'.
        if( end <= s+1 ) end = s+right_margin+1;

        //fwrite( s, 1, end-s, fp );
        //if( !(*end == 0) ) fwrite( "\n", 1, 1, fp );
        string str = s;
        msg.text.add( str.substr( 0, end-s ) );
        
        // Start from first non-written character
        s = end;
        }
    
    }










@


1.8
log
@Fixed formatter. (Added \n between lines).
@
text
@d8 3
d112 2
a113 2
#include	"fido2uu.h"
#include	"..\..\version.h"
d115 2
a116 1
#include	"datec.h"
d118 4
a121 4
#include	"encode.h"
#include	"recode.h"
#include	"freedom.h"
#include	"stat.h"
d123 5
a127 5
#include	<ctype.h>
#include	<share.h>
#include	<time.h>
#include	<io.h>
#include	<fcntl.h>
d129 1
a129 1
#include	"cc_bag.h"
d133 5
a137 4
static bool		is_headline( const char *s );
static bool		is_to_headline( string s );
static bool		is_xcc_headline( string &, string s );
static bool		is_cc_headline( string &, string s );
d139 1
a139 1
static void             format( FILE *fp, string &ss );
d149 2
a150 1
    FILE        *fp;
d162 1
d169 3
a171 1

d175 2
a176 1

d197 1
a197 1

d207 1
a207 6

    char	from_[120], *p;

    sprintf( from_, "%.90s %s", (const char *) uu_from, timeline( t, (const uchar *)conf.tz.c_str() ));

    if( conf.at_to_percent_in_from_ )
d209 29
a237 2
        while( (p = strchr( from_, '@@' )) != NULL )
            *p = '%';
d239 12
a250 10

    switch( conf.uucp ) {
    case UUCP_Ache30:
        break;
    case UUCP_Waffle:
        fprintf( fp, "From %s\n", from_ );
        break;
    default:
        fprintf( fp, "From %s remote from %s\n", from_, (const char *)conf.def_domain.c_str() );
        break;
a252 6
    fprintf( fp, "Received:  by %s (FIDO2UU %s) with FTN; %s\n",
            (const char *)conf.def_domain.c_str(),
            U1_VER_STR,
            timeline( t, (const uchar *)conf.tz.c_str() )
            );

d268 1
a268 1
        fprintf( fp, "Return-Receipt-To: %s\n", (const char *) uu_from );
d286 16
a301 7
        fprintf( fp, "To: %s\n", uu_to );
        fprintf( fp, "From: %s <%s>\n", rep_fido, (const char *) uu_from );
        fprintf( fp, "Reply-To: %s <%s>\n", rep_fido, (const char *) uu_from );
        fprintf( fp, "Message-Id: <%lX@@%s>\n", t, (const char *)conf.def_domain.c_str() );
        rec.rs( subj );
        fprintf( fp, "Subject: %s\n", subj );
        fprintf( fp, "Date: %s\n", (const char *)timeline( t, (const uchar *)conf.tz.c_str() ) );
d320 2
d323 1
a323 3
            rec.rs( buf );

            if( header && is_headline( buf ) )
d360 3
a362 2
                    fprintf( fp, "%s\n", (const char *)buf );		// Head line
                    debug("Headline %s", (const char *)buf );
d376 1
a376 1
                    fprintf( fp, "Cc: " );
d388 1
a388 1
                            fprintf( fp, "\n    " );
d390 1
a390 1

d395 2
d400 1
a400 1
                fprintf( fp, "\n" );				// Empty line - end of hdr
d402 2
a403 1

d405 2
a406 1
                fprintf( fp, ">" );
d408 2
a409 2
            format( fp, buf );	      		// Text line
            fprintf( fp, "\n" );		// Empty line
d412 4
a415 3
        fprintf( fp, "\n" );						// Empty line

        encode_attaches( fm, fp, &files_sent );				// Put in attached files
d418 2
a419 1

d430 5
a434 1
    bytes_sent = filelength( fileno( fp ));
d438 1
a438 1
        if( run_uusmail( uu_from, uu_to, deliver_cc, fileno( fp ) ) == Err )
d453 2
a454 1

d464 2
a465 1

d481 2
a482 2

bool is_headline( const char *s )             // Headline?
d484 1
d494 6
d501 1
d602 1
a602 1
static void format( FILE *fp, string &ss )
d608 7
a632 1
//printf("1 end-s = %d, scan-s = %d\n", end-s, scan-s );
d634 5
a638 4
//printf("2 end-s = %d, scan-s = %d\n", end-s, scan-s );
//printf("'%.*s'\n", end-s, s );
        fwrite( s, 1, end-s, fp );
        if( !(*end == 0) ) fwrite( "\n", 1, 1, fp );
@


1.7
log
@Using FTN_Msg! Seems to be working.
@
text
@d8 3
d568 1
a568 1
        
d570 2
a571 1
        
d573 1
@


1.6
log
@Before moving to FTN_Msg
@
text
@d8 3
d126 6
a131 4
static bool		is_headline( uchar *s );
static bool		is_to_headline( uchar *s );
static bool		is_xcc_headline( uchar *morecc, int len, uchar *s );
static bool		is_cc_headline( uchar *ccdata, int len, uchar *s );
d139 1
a139 1
Fido2uuApp::convert( fido_msg &fm, bool &r_report, bool &r_err )
d186 1
a186 1
    if( get_address( uu_to, &fm ) == Err )
d243 1
a243 1
        char		subj[80];
d259 1
a259 1
        rec.rs( (uchar *)subj );
d272 2
a273 2
        uchar			buf[1000];
        bool			header = Yes;
d276 1
a276 1
        while( !fm.getl( (char *)buf ) )
d278 2
a279 2
            char	morecc[80];
            char	fidocc[80];		// FIDO cc
d288 1
a288 1
                if( is_xcc_headline( (uchar *)morecc, 80, buf ) )
d292 1
a292 1
                        error( EI_None, "Access to %s denied at this gate", morecc );
d297 2
a298 2
                        deliver_cc.add( String( (const char *)morecc ) );
                        list_cc.add( String( (const char *)morecc ) );
d304 1
a304 1
                if( is_cc_headline( (uchar *)fidocc, 80, buf ) )
d309 1
a309 1
                        log( "#", "Can't convert FIDO Cc: %s", fidocc );
d320 2
a321 2
                    fprintf( fp, "%s\n", buf );		// Head line
                    debug("Headline %s", buf );
d360 1
a360 1
            if( strnicmp( (const char *)buf, "From", 4 ) == 0 )
d362 3
a364 1
            fprintf( fp, "%s\n", buf );				// Text line
d369 1
a369 1
        encode_attaches( &fm, fp, &files_sent );				// Put in attached files
d425 1
a425 1
					Headlines reckognition
d429 1
a429 2
bool
is_headline( uchar *s )						// Headline?
d449 2
a450 2
bool
is_to_headline( uchar *s )							// 'To:' ?
d452 1
a452 7
    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    if( strnicmp( (const char*)s, "to", 2 ) )					// to?
        return No;

    s += 2;											// skip "to"
d454 2
a455 2
    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;
d457 1
a457 2
    if( *s != ':' )									// ':' ?
        return No;
d459 1
d466 1
a466 1
is_xcc_headline( uchar *morecc, int len, uchar *s )
d468 11
a478 27
    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    if( strnicmp( (const char *)s, "xcc", 3 ) )					// Cc?
        return No;

    s += 3;											// skip "to"

    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    if( *s != ':' )									// ':' ?
        return No;

    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    s++;
    while( *s )
        {
        *morecc++ = *s++;
        if( --len <= 0 )
            break;
        }

    *morecc = '\0';

d483 1
a483 1
is_cc_headline( uchar *ccdata, int len, uchar *s )
d485 1
a485 23
    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    if( strnicmp( (const char *)s, "cc", 2 ) )                // Cc?
        return No;

    s += 2;                                                                                 // skip "to"

    while( *s == ' ' || *s == '\t' )				// Skip ws
        s++;

    if( *s != ':' )									// ':' ?
        return No;

    s++;
    while( *s )
        {
        *ccdata++ = *s++;
        if( --len <= 0 )
            break;
        }

    *ccdata = '\0';
d487 9
d539 1
d541 34
@


1.5
log
@Save of current (working?) code
@
text
@d8 3
d17 2
a18 5
 * Revision 1.2  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
 *
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
d20 2
a122 2
#define		MAXCC		400

a126 1
//static bool		bad_cc_list( uchar *cc_list );
d135 75
a209 78
	{
	FILE		*fp;
	time_t		t;
	char		uu_to[100];						// Send to this address
//	char		uu_cc[MAXCC];					// Carbon copies
	uucp_addr	uu_from;						// Send from this address
	bool		reg_user;						// Registered user
	bool		skip = No;
	fido_user	fido_from;
	fmsg_attr   at;
	recoder     rec( conf.code_fu.c_str() );            // Get codetable
	char		temp_name[100];
	long 		bytes_sent;
	long		files_sent;

	CC_Bag			deliver_cc;						// List of rmail args (cc:)
	CC_Bag			list_cc;						// List of things to put to cc headline

	if( !rec.valid() )							// Can't init recoder
		fatal( EC_Incorrect_Setup, EI_None, "Can't find codetable '%s'", (const char *)conf.code_fu.c_str() );

//	uu_cc[0] = '\0';							// Clear Cc field

	time( &t );
	fm.get_from( fido_from );
	fm.get_attr( at );

	for( int i = 10; i; i-- )
		{
		char	*tf = tempnam( getenv("TEMP"), "UU2TF" );
		if( tf == NULL )
			fatal( EC_OS_Error, EI_Full, "Can't find unique file name!" );

		int fd = _sopen( tf, O_RDWR|O_CREAT|O_EXCL|O_BINARY, SH_DENYWR, 0666 );			// Open temp file

		fp = fdopen( fd, "w+b" );

		if( fd < 0 || fp != NULL )
			{
			strncpy( temp_name, tf, sizeof(temp_name)-1 );
			free( tf );
			break;
			}
		}

	if( fp == NULL )				// Can't ?
		fatal( EC_IO_Error, EI_Full, "Can't create temporary file" );


	if( get_address( uu_to, &fm ) == Err )
		skip = Yes;

	if( skip || (get_from( &reg_user, uu_from, fido_from ) == Err) )
		skip = Yes;
	else
		if( strlen( (const char *)uu_from ) == 0 )
			fatal( EC_UU2_Error, EI_None, "Can't build 'From:'???" );

	char	from_[120], *p;

	sprintf( from_, "%.90s %s", (const char *) uu_from, timeline( t, (const uchar *)conf.tz.c_str() ));

	if( conf.at_to_percent_in_from_ )
		{
		while( (p = strchr( from_, '@@' )) != NULL )
			*p = '%';
		}

	switch( conf.uucp ) {
	case UUCP_Ache30:
		break;
	case UUCP_Waffle:
		fprintf( fp, "From %s\n", from_ );
		break;
	default:
		fprintf( fp, "From %s remote from %s\n", from_, (const char *)conf.def_domain.c_str() );
		break;
		}
d212 90
a301 108
			(const char *)conf.def_domain.c_str(),
			U1_VER_STR,
			timeline( t, (const uchar *)conf.tz.c_str() )
			);

	uchar	rep_fido[40];
	uchar	*cp;

	strncpy( (char *)rep_fido, fido_from.name(), 37 );
	rep_fido[36] = '\0';
	rec.rs( rep_fido );
	for( cp = rep_fido; *cp; cp++ )
		{
		if( (*cp < 'A') && (*cp != ' ') && (*cp != '-') )
			*cp = '.';
		}

	if( at & FF_ReturnReceiptRequest )   // Confirmation Request
		{
		log("#", "Adding return-receipt-to field" );
		fprintf( fp, "Return-Receipt-To: %s\n", (const char *) uu_from );
		}


	if( !skip )
		{
		char		subj[80];

		fm.get_subj( subj );

		fprintf( stderr, "From: %s\n", rep_fido );
		fprintf( stderr, "To  : %s\n", uu_to );

		log( "ua", "To: %s", uu_to );
		log( "ua", "From: %s <%s>", rep_fido, (const char *) uu_from );
		log( "u", "Subject: %s", subj );
		log( "u", "Date: %s", timeline( t, (const uchar *)conf.tz.c_str() ) );

		fprintf( fp, "To: %s\n", uu_to );
		fprintf( fp, "From: %s <%s>\n", rep_fido, (const char *) uu_from );
		fprintf( fp, "Reply-To: %s <%s>\n", rep_fido, (const char *) uu_from );
		fprintf( fp, "Message-Id: <%lX@@%s>\n", t, (const char *)conf.def_domain.c_str() );
		rec.rs( (uchar *)subj );
		fprintf( fp, "Subject: %s\n", subj );
		fprintf( fp, "Date: %s\n", (const char *)timeline( t, (const uchar *)conf.tz.c_str() ) );
		}

	if( (!reg_user) && (!is_free_address( uu_to )))
		{
		error( EI_None, "Access to '%s' denied at this gate, sorry.", uu_to );
		skip = Yes;
		}

	if( !skip )
		{
		uchar			buf[1000];
		bool			header = Yes;

		fm.rewind();
		while( !fm.getl( (char *)buf ) )
			{
			char	morecc[80];
			char	fidocc[80];		// FIDO cc

			rec.rs( buf );

			if( header && is_headline( buf ) )
				{
				if( is_gate_control( buf ) == Yes )	// Gate control line
					continue;

				if( is_xcc_headline( (uchar *)morecc, 80, buf ) )
					{
//					if( strlen( uu_cc ) > MAXCC )
//						continue;

//					fprintf( fp, "Cc: %s\n", morecc );
//					if( strlen( uu_cc ) )
//						strcat( uu_cc, " " );
//					strcat( uu_cc, morecc );

					if( !is_free_address( morecc ) )
						{
						error( EI_None, "Access to %s denied at this gate", morecc );
						r_report = Yes;
						}
					else
						{
//						String *cc = new String( (const char *)morecc );
//						if( cc == NULL )
//							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );

						deliver_cc.add( String( (const char *)morecc ) );

//						cc = new String( (const char *)morecc );
//						if( cc == NULL )
//							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );

						list_cc.add( String( (const char *)morecc ) );

						log( "ua", "Cc: %s", (const char *)morecc );
						}
					continue;
					}

				if( is_cc_headline( (uchar *)fidocc, 80, buf ) )
					{
					uucp_addr	convcc;	// Converted to uucp form FIDO cc
d303 1
a303 2
					if( fidoaddr2uuaddr( convcc, fidocc ) == Err )
//                        error( EI_None, "Can't convert FIDO Cc: %s", fidocc );
d305 97
a401 109
					else
						{
//						String *cc = new String( (String &)convcc );
//						if( cc == NULL )
//							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );

						list_cc.add( (String &)convcc );

						log( "ua", "FIDO Cc: %s", (const char *)(((String &)convcc).c_str()) );
						}

					continue;
					}

				if( !is_to_headline( buf ) )
					{
					fprintf( fp, "%s\n", buf );		// Head line
					debug("Headline %s", buf );
					}

				continue;
				}

			if( header )
				{
				header = No;

				int	ncc;

				if( (ncc = list_cc.numberOfElements()) > 0 )
					{
					fprintf( fp, "Cc: " );

					CC_BagIterator i( list_cc );
					int n;

					for( n = 0, i.setToFirst(); i.isValid(); i.setToNext(), n++ )
						{
						String &cc = (String &)list_cc.elementAt(i);

						if( n > 4 )
							{
							n = 0;
							fprintf( fp, "\n    " );
							}

						fprintf( fp, "%s%s",
							(const char *)cc.c_str(),
							--ncc ? ", " : "\n"
							);
						}
					}

				fprintf( fp, "\n" );				// Empty line - end of hdr
				}

			if( strnicmp( (const char *)buf, "From", 4 ) == 0 )
				fprintf( fp, ">" );
			fprintf( fp, "%s\n", buf );				// Text line
			}

		fprintf( fp, "\n" );						// Empty line

		encode_attaches( &fm, fp, &files_sent );				// Put in attached files

		}

	if( ferror( fp ) )
		{
		error( EI_Full, "Can't write msg to temp file - msg skipped" );
		fclose( fp );
		r_err = Yes;
		return;
		}


	fflush( fp );
	bytes_sent = filelength( fileno( fp ));
/*
	if( (!reg_user) && bad_cc_list( uu_cc ))
		skip = Yes;
*/

	if( !skip )
		{
		if( run_uusmail( uu_from, uu_to, deliver_cc, fileno( fp ) ) == Err )
			skip = Yes;
		else
			{
			add_stat_fido( fido_from, (const unsigned char *)uu_to, No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );

			CC_BagIterator i( list_cc );

        		for( i.setToFirst(); i.isValid(); i.setToNext() )
				{
				String &cc = (String &)list_cc.elementAt(i);
//				add_stat_fido( (const unsigned char*)(((String&)fido_from).c_str()), (const char *)cc.c_str(), No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );
				add_stat_fido( fido_from, (const uchar *)cc.c_str(), No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );
				}
			}

		}

	if( fclose( fp ) )
		{
		error( EI_Full, "Can't close temp. file" );
		r_err = Yes;
		return;
		}
d403 2
a404 2
	if( unlink( temp_name ) )
		error( EI_Full, "Can't delete temp '%s'", temp_name );
d406 2
a407 2
	if( skip )
		r_err = Yes;
d409 1
a409 1
	}
d424 8
a431 8
	{
	while( *s )
		{
		if( isalpha( *s ) || *s == '-' )
			{
			s++;
			continue;
			}
d433 2
a434 2
		if( *s == ':' )
			return Yes;
d436 2
a437 2
		return No;
		}
d439 2
a440 2
	return No;
	}
d445 3
a447 3
	{
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d449 2
a450 2
	if( strnicmp( (const char*)s, "to", 2 ) )					// to?
		return No;
d452 1
a452 1
	s += 2;											// skip "to"
d454 2
a455 2
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d457 2
a458 2
	if( *s != ':' )									// ':' ?
		return No;
d460 2
a461 2
	return Yes;
	}
d467 3
a469 3
	{
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d471 2
a472 2
	if( strnicmp( (const char *)s, "xcc", 3 ) )					// Cc?
		return No;
d474 1
a474 1
	s += 3;											// skip "to"
d476 2
a477 2
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d479 2
a480 2
	if( *s != ':' )									// ':' ?
		return No;
d482 2
a483 2
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d485 7
a491 7
	s++;
	while( *s )
		{
		*morecc++ = *s++;
		if( --len <= 0 )
			break;
		}
d493 1
a493 1
	*morecc = '\0';
d495 2
a496 2
	return Yes;
	}
d500 3
a502 3
	{
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d504 2
a505 2
		if( strnicmp( (const char *)s, "cc", 2 ) )                // Cc?
		return No;
d507 1
a507 1
		s += 2;                                                                                 // skip "to"
d509 2
a510 2
	while( *s == ' ' || *s == '\t' )				// Skip ws
		s++;
d512 2
a513 2
	if( *s != ':' )									// ':' ?
		return No;
d515 7
a521 7
	s++;
	while( *s )
		{
		*ccdata++ = *s++;
		if( --len <= 0 )
			break;
		}
d523 1
a523 1
	*ccdata = '\0';
d525 2
a526 2
	return Yes;
	}
@


1.4
log
@App class used
@
text
@d8 3
d256 1
@


1.3
log
@appearance of Received: line
@
text
@d8 3
d132 1
a132 1
convert( fido_msg &fm, bool &r_report, bool &r_err )
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 3
d209 1
a209 2
	fprintf( fp, "Received:  by %s (FIDO2UU %s);\n"
				 "           %s\n",
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/convert.c_v  $
d105 2
d160 3
a162 1
		fp = _fsopen( tf, "w+b", SH_DENYWR );			// Open temp file
d164 1
a164 1
		if( fp != NULL )
d187 1
a187 1
	sprintf( from_, "%.90s %s", (const char *) uu_from, timeline( t, (const char *)conf.tz.c_str() ));
d189 5
a193 2
	while( (p = strchr( from_, '@@' )) != NULL )
		*p = '%';
d210 1
a210 1
			timeline( t, (const char *)conf.tz.c_str() )
d216 1
a216 1
	strncpy( rep_fido, fido_from.name(), 37 );
d244 1
a244 1
		log( "u", "Date: %s", timeline( t, (const char *)conf.tz.c_str() ) );
d249 1
a249 1
		rec.rs( subj );
d251 1
a251 1
		fprintf( fp, "Date: %s\n", (const char *)timeline( t, (const char *)conf.tz.c_str() ) );
d262 1
a262 1
		char			buf[1000];
d266 1
a266 1
		while( !fm.getl( buf ) )
d278 1
a278 1
				if( is_xcc_headline( morecc, 80, buf ) )
d295 3
a297 3
						String *cc = new String( (const char *)morecc );
						if( cc == NULL )
							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );
d299 1
a299 1
						deliver_cc.Add( *cc );
d301 3
a303 3
						cc = new String( (const char *)morecc );
						if( cc == NULL )
							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );
d305 1
a305 1
						list_cc.Add( *cc );
d307 1
a307 1
						log( "ua", "Cc: %s", (const char *)((*cc).c_str()) );
d312 1
a312 1
				if( is_cc_headline( fidocc, 80, buf ) )
d321 3
a323 3
						String *cc = new String( (String &)convcc );
						if( cc == NULL )
							fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );
d325 1
a325 1
						list_cc.Add( *cc );
d327 1
a327 1
						log( "ua", "FIDO Cc: %s", (const char *)((*cc).c_str()) );
d348 1
a348 1
				if( (ncc = list_cc.GetItemsInContainer()) > 0 )
d353 1
d355 1
a355 1
					for( int n = 0; (int)i; i++, n++ )
d357 1
a357 1
						String &cc = (String &)i.Current();
d375 1
a375 1
			if( strncmpi( buf, "From", 4 ) == 0 )
d412 1
a412 1
			for( ; (int)i; i++ )
d414 3
a416 2
				String &cc = (String &)i.Current();
				add_stat_fido( fido_from, (const char *)cc.c_str(), No /* BUG! */,  files_sent ? Yes : No, No, bytes_sent );
d475 1
a475 1
	if( strnicmp( s, "to", 2 ) )					// to?
d497 1
a497 1
	if( strnicmp( s, "xcc", 3 ) )					// Cc?
d530 1
a530 1
		if( strnicmp( s, "cc", 2 ) )                // Cc?
@

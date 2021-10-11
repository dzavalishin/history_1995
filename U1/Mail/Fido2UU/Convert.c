/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Convertor
 *
 *      $Log: Convert.c $
 *      Revision 1.9  1996/03/27 10:46:53  dz
 *      current
 *
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
#include    "encode.h"
#include    "recode.h"
#include    "freedom.h"
#include    "stat.h"

#include    <ctype.h>
#include    <time.h>



static bool      is_headline( string &key, string &val, const char *s );
//static bool	 is_headline( const char *s );
static bool      is_to_headline( string s );
static bool      is_xcc_headline( string &, string s );
static bool      is_cc_headline( string &, string s );
static bool      is_forbidden_hl( string s );

static void      format( RFC_Msg &msg, string &ss );

// TODO: Skip flag must be redone - if set at the beginning,
// most of the following code will fail!



void
Fido2uuApp::convert( FTN_Msg &fm, bool &r_report, bool &r_err )
    {
    RFC_Msg      msg;
    time_t       right_now = time(NULL);
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
    
    time( &right_now );
    fm.get_from( fido_from );
    fm.get_attr( at );

    if( get_address( uu_to, fm ) == Err )
        skip = Yes;

    if( get_from( &reg_user, uu_from, fido_from ) == Err )
        skip = Yes;
    else
        if( strlen( (const char *)uu_from ) == 0 )
            fatal( EC_UU2_Error, EI_None, "Can't build 'From:'???" );
    
        {
        char from_[240], *p, buf[200];
        
        string fulla = (string &)uu_from, aname, adomain;
        
        if( !fulla.parse( aname, '@', adomain ) ) 
            error( EI_None, "There MUST be @: '%s'!", (const char *)fulla );
        
        sprintf( from_, "%.90s!%.90s %s",
                (const char *) adomain,
                (const char *) aname,
                timeline( right_now, (const uchar *)conf.tz.c_str() ));
        /* if( conf.at_to_percent_in_from_ )
            { while( (p = strchr( from_, '@' )) != NULL ) *p = '%'; } */
        
        buf[0]= '\0';
        switch( conf.uucp ) 
            {
        case UUCP_Ache30:            break;
        case UUCP_Sendmail:          break;

        case UUCP_Waffle:            sprintf( buf, "%s", from_ );
                                     break;
        default:                     sprintf( buf, "%s remote from %s", from_, (const char *)conf.def_domain.c_str() );
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
                timeline( right_now, (const uchar *)conf.tz.c_str() )
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
        fprintf( stderr, "From: %s\n", rep_fido );
        fprintf( stderr, "To  : %s\n", uu_to );
        
        time_t fm_date = fm.get_date();
        
        log( "ua", "To: %s", uu_to );
        log( "ua", "From: %s <%s>", rep_fido, (const char *) uu_from );
        log( "u", "Date: %s", timeline( fm_date, (const uchar *)conf.tz.c_str() ) );

        msg.headline.add_hl( "To", uu_to );
        
        sprintf( buf, "<%lX@%s>", right_now, (const char *)conf.def_domain.c_str() );
        msg.headline.add_hl( "Message-Id", buf );
        msg.headline.add_hl( "Date", timeline( fm_date, (const uchar *)conf.tz.c_str() ) );
        }

    if( (!reg_user) && (!is_free_address( uu_to )))
        {
        error( EI_None, "Access to '%s' denied at this gate, sorry.", uu_to );
        skip = Yes;
        }
    
    if( skip )
        {
        r_err = Yes;
        return;
        }
    
    
    
    string  line;
    //bool    header = Yes;
    
    bytes_sent = 0;

    fm.rewind();
    while( !fm.gets( line ) )
        {
        string     morecc;
        string     fidocc;  // FIDO cc
        string hl_key, hl_val;
        
//log( "#", "gets: %s", (const char *)line );
        
        if( !is_headline( hl_key, hl_val, line ) )   // End of header
            {
            format( msg, line );	      		// Text line
            bytes_sent += strlen( line );
            break;
            }
        
        if( is_gate_control( line ) == Yes )	// Gate control line
            continue;
        
        if( is_xcc_headline( morecc, line ) )
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
        
        if( is_cc_headline( fidocc, line ) )
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
        
        if( !is_to_headline( line ) )
            {
            if( is_forbidden_hl( hl_key ) )
                warning( EI_None, "Attempted to add forbidden headline: '%s: %s'", (const char *)hl_key, (const char *)hl_val );
            else
                {
                msg.headline.add_hl( hl_key, hl_val );
                debug("Headline (%s: %s)", (const char *)hl_key, (const char *)hl_val );
                }
            }
        
        }
    
    
    if( list_cc.numberOfElements() > 0 )
        {
        CC_BagIterator i( list_cc );
        for( i.setToFirst(); i.isValid(); i.setToNext() )
            {
            String &cc = (String &)list_cc.elementAt(i);
            msg.headline.add_hl( "Cc", cc );
            }
        }
    
        {
        sprintf( buf, "%s <%s>", rep_fido, (const char *) uu_from );
        bool need_sender = No;
    
        if( msg.headline.contains("From") )        need_sender = Yes;
        else                                       msg.headline.add_hl( "From", buf );
    
        if( msg.headline.contains("Reply-To") )    need_sender = Yes;
        else                                       msg.headline.add_hl( "Reply-To", buf );

        if(need_sender)                            msg.headline.add_hl( "Sender", buf );
        msg.headline.add_hl( "Errors-To", (const string &)uu_from );
        }
    
    if( !msg.headline.contains("Subject") )
        {
        string   subj;
        fm.get_subj( subj );
        log( "u", "Subject: %s", subj );
        msg.headline.add_hl( "Subject", subj );
        }
    
    
    while( !fm.gets( line ) )
        {
//log( "#", "gets text: %s", (const char *)line );

        format( msg, line );	      		// Text line
        bytes_sent += strlen( line );
        }
    
    msg.text.add( "" );
    
    encode_attaches( fm, msg, &files_sent );  // Put in attached files
    
    

    
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


static bool is_forbidden_hl( string s )
    {
    s.lcase();
    if( 
       s == "sender" || s == "errors-to" ||
       s == "message-id" || s == "date"
      )
        return Yes;
    return No;
    }


/****************************************************************************
                                  Formatter
****************************************************************************/


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
        //if( end <= s+1 ) end = s+right_margin+1;
        if( end <= s ) end = s+right_margin+1;

        //fwrite( s, 1, end-s, fp );
        //if( !(*end == 0) ) fwrite( "\n", 1, 1, fp );
        string str = s;
        msg.text.add( str.substr( 0, end-s ) );
        
        // Start from first non-written character
        s = end;
        }
    
    }











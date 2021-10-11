head	1.1;
access;
symbols;
locks;
comment	@ *      @;


1.1
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ FIDO MSG file class ***************************\
 *
 *    Copyright (C) 1991 by Infinity Soft
 *
 *    Module     :        Attach/detach
 *
 *      $Log: Msg_Att.c $
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *
 *      
 *         Rev 1.8   09 Apr 1993 10:09:14   dz
 *      Extended flags support
 *      
 *         Rev 1.7   26 Nov 1992 21:27:58   dz
 *      Reorganization
 *      
 *         Rev 1.6   06 Nov 1992 19:19:52   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.5   23 Oct 1992 15:19:18   dz
 *      error()
 *      
 *         Rev 1.4   17 Apr 1992 12:53:38   dz
 *      FSC35 additions, first stage.
 *      
 *         Rev 1.3   04 Feb 1992 22:04:34   dz
 *      MSGID generator written, XFM/XTO support added
 *      
 *         Rev 1.2   11 Oct 1991 05:05:12   dz
 *      default zone number assignment fixed
 *      
 *         Rev 1.1   12 Sep 1991 01:26:28   dz
 *      Bug with kludges loader fixed
 *      Reserved fields (zone/point) support added
 *      Default zone number assignment added
 *      
 *         Rev 1.0   28 Aug 1991 00:59:42   dz
 *      Initial revision.
 *
 *
\*/

#include    <style.h>
#include    <uu2io.h>
#include    <fcntl.h>
#include    <share.h>
#include    <string.h>
#include    <stdlib.h>
#include    <time.h>

#include    "FTN_Msg.h"
#include    <log.h>
#include    "..\..\version.h"
#include    <setup.h>


static ulong
get_msgid( void )
    {
    static int    c = 0;
    return (time(0) << 2) + (c++);
    }


bool
FTN_Msg::attach( const string &dir, int num )
    {
    char snum[30];
    sprintf( snum, "\\%d.msg", num );
    fname = dir + string(snum);
    return do_attach();
    }

bool    
FTN_Msg::attach( const string &file )
    {
    fname = file;
    return do_attach();
    }


bool    
FTN_Msg::do_attach( void )
    {
    int        fd;

    detach();
    clear_hdr();

    error = No;

//    sprintf( fname, "%s\\%d.msg", dir, num );

    fd    = _sopen( fname, O_RDWR|O_BINARY, SH_DENYRW, 0666 );

    if( fd < 0 )
        {
        error = Yes;
        return Err;
        }

    if( (fp = fdopen( fd, "rb+" )) == NULL )
        {
        close( fd );
        error = Yes;
        return Err;
        }

    fseek( fp, 0L, SEEK_SET );

    // Read header in
    if( fread( &hdr, sizeof(hdr), 1, fp ) != 1 )
        {
        error = Yes;
        return Err;
        }

    from.net ( hdr.origNet  );
    from.node( hdr.origNode );
    to.net     ( hdr.destNet  );
    to.node     ( hdr.destNode );

    from.name( hdr.from );
    to.name( hdr.to );

    flags        = hdr.attr;
    
    string s;
    
    // load text
    while( s.load( fp ) == Ok )
        {
        s.strip_crlf();
        text.add( s );
        }

    if( ferror( fp ) )
        return Err;
    
    
    load_kludges();
    
    error = No;

    return Ok;
    }


#include <time.h>

// Via 2:51/2@@fidonet @@19950821.200818.91 Itrack+ 1.2 EB000001
// ^AVia addr @@YYYYmmDD.hhMMss.uu Name Ver/subver

bool
FTN_Msg::put_via()
    {
    char date[40];

    if( via_addr.is_empty() ) return Ok;

    time_t t = time(NULL);

    strftime( date, 39, "%Y%m%d.%H%M%S", localtime(&t) );

    fprintf( fp, "Via %.32s @@%s U1 " U1_VER_STR "\r\n",
            (const char *)via_addr, 
            date 
           );
    return Ok;
    }

bool
FTN_Msg::rewrite_msg( void )
    {
/*
    char    buf[301];
    FILE    *tf    = tmpfile();

    if( tf == NULL )
        {
        ::error( EI_Full, "Can't create temp file (out of memory?)");
        return Err;
        }


//
//    Copy text from our msg to temp file
//

    rewind();
    while( gets( buf ) == Ok )
        {
        char *p;
        if( (p = strpbrk( buf, "\r\n" )) != NULL )
            *p = '\0';
        fputs( buf, tf );
        fputs( "\n", tf );
        }


    clear_txt();
    rewind();
    */
    
    if( error )		return Err;
    if( fseek( fp, (long)(sizeof( hdr )), SEEK_SET ) )
        return Err;
    
    if( fflush( fp ) == EOF )
        return Err;

    if( chsize( fileno( fp ), sizeof( hdr ) ) )
        return Err;
    
    if( from.has_domain() && to.has_domain() )
        fprintf( fp, "\1DOMAIN %s %d:%d/%d.%d %s %d:%d/%d.%d\r\n",
                to.domain(),    to.zone(),        to.net(),    to.node(),    to.point(),
                from.domain(),    from.zone(),    from.net(),    from.node(),    from.point()
               );

    if( to.zone() || from.zone() )
        fprintf( fp, "\1INTL %d:%d/%d %d:%d/%d\r\n",
                to.zone(),        to.net(),        to.node(),
                from.zone(),    from.net(),        from.node()
               );

    if( to.point() )
        fprintf( fp, "\1TOPT %d\r\n", to.point() );

    if( from.point() )
        fprintf( fp, "\1FMPT %d\r\n", from.point() );

    if( (!conf.no_flags_kludge) && (flags & FLAGS_KLUDGE_BITS) )
        {
        string buf;
        print_flags( buf, flags );
        fprintf( fp, "\1%s\r\n", (const char *)buf );
        }


    if( from.zone() )
        fprintf( fp, "\1MSGID: %d:%d/%d.%d%s%s %lX\r\n",
            from.zone(), from.net(), from.node(), from.point(),
            from.has_domain() ? "@@" : "",
            from.domain(),
            get_msgid()
            );
    else
        fprintf( fp, "\1MSGID: %d/%d.%d%s%s %lX\r\n",
            from.net(), from.node(), from.point(),
            from.has_domain() ? "@@" : "",
            from.domain(),
            get_msgid()
            );


    if( strlen( kl_reply ) )
        fprintf( fp, "\1REPLY: %s\r\n", kl_reply );

    if( strlen( fsc35_reply_addr ) )
        {
        fprintf( fp, "\1REPLYADDR %s\r\n", fsc35_reply_addr );

        if( fsc35_reply_to.valid() && fsc35_reply_to.zone() != 0 )
            fprintf( fp, "\1REPLYTO %d:%d/%d.%d%s%s %s\r\n",
                fsc35_reply_to.zone(),
                fsc35_reply_to.net(),
                fsc35_reply_to.node(),
                fsc35_reply_to.point(),

                fsc35_reply_to.has_domain() ? "@@" : "",
                fsc35_reply_to.domain(),

                fsc35_reply_to.name()
                );
        }


    if( strlen( kl_pid ) )
        fprintf( fp, "\1PID: %s\r\n", kl_pid );
    else
        fprintf( fp, "\1PID: U1 " U1_VER_STR "\r\n" );


//
//    Copy text back from temp file to our msg
//
/*
    fseek( tf, 0L, SEEK_SET );
    while( fgets( buf, 300, tf ) != NULL )
        {
        char *p;
        if( (p = strpbrk( buf, "\r\n" )) != NULL )
            *p = '\0';
        puts( buf );
        }
        */
    
    FTN_Cursor tc( text );

    for( tc.setToFirst(); tc.isValid(); tc.setToNext() )
        if( !is_known_kludge( tc.element() ) )
           fprintf( fp, "%s\r\n", (const char *)(tc.element()) );

    put_via();

//    fseek( fp, 0L, SEEK_END );    Slooooow!
    fwrite( "", 1, 1, fp );                            // NULL-terminate msg

//    fclose( tf );
//    chmod( tempname, 0666 );
//    unlink( tempname );

    if( ferror( fp ) )
        return Err;

    return Ok;
    }












static hlc        hltab[] = {
    {    "FMPT",        Yes,    No,        No,        No,        No,        No,        No,        No,        No    },
    {    "TOPT",        No,        Yes,    No,        No,        No,        No,        No,        No,        No    },
    {    "INTL",        No,        No,        Yes,    No,        No,        No,        No,        No,        No    },
    {    "FLAGS",    No,        No,        No,        Yes,    No,        No,        No,        No,        No    },

    {    "REPLY:",    No,        No,        No,        No,        Yes,    No,        No,        No,        No    },
    {    "PID:",        No,        No,        No,        No,        No,        Yes,    No,        No,        No    },
    {    "REPLYADDR",No,        No,        No,        No,        No,        No,        Yes,    No,        No    },
    {    "REPLYTO",    No,        No,        No,        No,        No,        No,        No,        Yes,    No    },
    {    "DOMAIN",    No,        No,        No,        No,        No,        No,        No,        No,        Yes    },
    };

const int maxhl = (sizeof(hltab)/sizeof(hlc));


bool FTN_Msg::
is_known_kludge( const string &k )
    {
    for( int i = 0; i < maxhl; i++ )
        {
        int            cl = strlen( hltab[i].fido );
        if( 0 == strncmp( hltab[i].fido, k, cl ) )
            return Yes;
        }
    
    return No;
    }



void FTN_Msg::
get_kludge( const string &buf, hlc &state )
    {
//    int    u;

    for( int i = 0; i < maxhl; i++ )
        {
        int            cl = strlen( hltab[i].fido );
        const char    *val = ((const char *)buf)+cl;

        if( strncmp( hltab[i].fido, buf, cl ) )
            continue;

        if( hltab[i].pid )              kl_pid = val;
        if( hltab[i].reply )            kl_reply = val;
        if( hltab[i].flags )            flags |= scan_flags( val );

        if( hltab[i].fmpt )
            {
            from.point(    atoi(val) );
            state.fmpt = Yes;
            }

        if( hltab[i].topt )
            {
            to.point( atoi(val) );
            state.topt = Yes;
            }

        if( hltab[i].intl && !state.domain )
            {
            int    fzone = 0, fnet = 0, fnode = 0, fpoint = 0;
            int    tzone = 0, tnet = 0, tnode = 0, tpoint = 0;
            char    fdomain[38], tdomain[36];

            fdomain[0] = tdomain[0] = '\0';

            state.intl = Yes;
            if( sscanf( val, "%d:%d/%d %d:%d/%d",
            &tzone, &tnet, &tnode, &fzone, &fnet, &fnode ) != 6 )

            if( sscanf( val, "%d:%d/%d.%d %d:%d/%d.%d",
            &tzone, &tnet, &tnode, &tpoint, &fzone, &fnet, &fnode, &fpoint ) != 8 )

            if( sscanf( val, "%d:%d/%d@@%s %d:%d/%d@@%s",
            &tzone, &tnet, &tnode, tdomain, &fzone, &fnet, &fnode, fdomain ) != 8 )

            if( sscanf( val, "%d:%d/%d.%d@@%s %d:%d/%d.%d@@%s",
            &tzone, &tnet, &tnode, &tpoint, tdomain, &fzone, &fnet, &fnode, &fpoint, fdomain ) != 10 )
                {
                ::error( EI_None, "Can't parse INTL: %s", val );
                state.intl = No;
                continue;
                }

            from.zone ( fzone );
            to.zone      ( tzone );
            from.net  ( fnet  );
            to.net      ( tnet  );
            from.node ( fnode );
            to.node      ( tnode );

            if( fpoint && !state.fmpt )        from.point( fpoint );
            if( tpoint && !state.topt )        to.point  ( tpoint );

            if( strlen( fdomain ) )
                from.domain( fdomain );

            if( strlen( tdomain ) )
                to.domain( tdomain );

            }


        if( hltab[i].domain )
            {
            int    fzone = 0, fnet = 0, fnode = 0, fpoint = 0;
            int    tzone = 0, tnet = 0, tnode = 0, tpoint = 0;
            char    fdomain[38], tdomain[36];

            int     n = 0;
            bool    err = No;

            fdomain[0] = tdomain[0] = '\0';

            state.domain = Yes;

            if( 1 != sscanf( val, "%s%n", tdomain, &n ) )
                err = Yes;
            else
                val += n;

            if( err || 3 != sscanf( val, "%d:%d/%d%n",
                    &tzone, &tnet, &tnode,
                    &n ))
                err = Yes;
            else
                val += n;

            if( (!err) && 1 == sscanf( val, ".%d%n", &tpoint, &n ) )
                val += n;

            if( 1 != sscanf( val, "%s%n", fdomain, &n ) )
                err = Yes;
            else
                val += n;

            if( err || 3 != sscanf( val, "%d:%d/%d%n",
                    &fzone, &fnet, &fnode,
                    &n ))
                err = Yes;
            else
                val += n;

            if( (!err) && 1 == sscanf( val, ".%d%n", &fpoint, &n ) )
                val += n;


            if( err )
                {
                ::error( EI_None, "Can't parse DOMAIN: %s", val );
                state.domain = No;
                continue;
                }

            from.zone ( fzone );
            to.zone      ( tzone );
            from.net  ( fnet  );
            to.net      ( tnet  );
            from.node ( fnode );
            to.node      ( tnode );

            if( fpoint )    from.point ( fpoint );
            if( tpoint )    to.point   ( tpoint );

            from.domain( fdomain );
            to.domain( tdomain );
            }

        if( hltab[i].replyaddr )
            {
            if( *val == ':' ) // crosser creates 'em incorrectly
                fsc35_reply_addr = val+1;
            else
                fsc35_reply_addr = val;
            }

        if( hltab[i].replyto )
            {
            if( *val == ':' ) // crosser creates 'em incorrectly
                fsc35_reply_to.fparse( val+1 );
            else
                fsc35_reply_to.fparse( val );
            }
        }
    }




void
FTN_Msg::load_kludges( void )
    {
    string     buf;
    hlc        state;

    state.fmpt        =
    state.topt        =
    state.intl        =
    state.flags        =
    state.reply        =
    state.pid        =
    state.replyaddr =
    state.replyto    =
    state.domain    =    No;

    rewind();
    while( !geta( buf ) )
        get_kludge( buf, state );
    }




void
FTN_Msg::reset_hdr_addr( void )
    {
    hdr.destZone    = to.zone();
    hdr.origZone    = from.zone();

    hdr.destNet        = to.net();
    hdr.origNet        = from.net();

    hdr.destNode    = to.node();
    hdr.origNode    = from.node();

    hdr.destPoint    = to.point();
    hdr.origPoint    = from.point();

    strncpy( hdr.from, from.name(), 36 );
    strncpy( hdr.to, to.name(), 36 );

    hdr.attr = flags & 0xFFFFu;
    }

bool
FTN_Msg::detach( void )
    {
    if( error )        return Ok;

    bool    kill_it = ((flags & FF_Sent) && (flags & FF_KillSent)) ? Yes : No;

//    if( rewrite &&  )
    if( (!kill_it) && rewrite_msg() == Err )
        return Err;

    if( !kill_it )
        {
        fseek( fp, 0L, SEEK_SET );

        reset_hdr_addr();

        // Write header out
        if( fwrite( &hdr, sizeof(hdr), 1, fp ) != 1 )
            return Err;
        }

    fclose( fp );

    if( kill_it )
        {
        chmod( fname, 0666 );
        unlink( fname );
        }

    error = Yes;
    return Ok;
    }
@

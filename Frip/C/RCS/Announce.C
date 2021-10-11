head	1.15;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ *      @;


1.15
date	97.03.26.10.56.58;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	97.03.22.15.47.27;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	97.01.31.10.12.49;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.15
log
@datetime::now().gmtime()
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Announcer
 *
 *      $Log: Announce.C $
 *      Revision 1.14  1997/03/22 15:47:27  dz
 *      datetime
 *
 *      Revision 1.13  1997/01/31 10:12:49  dz
 *      Silence support
 *
 *      Revision 1.12  1997/01/21 08:53:51  dz
 *      Added -NA to announces from .ad
 *      Changed creator address to our address, not address
 *      of announce.
 *
 *      Revision 1.11  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.10  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.9  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.8  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.7  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.6  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "filer.h"

// There's two reasons to reannounce: ann. files changed or
// time out expired.


//static const announce_period = 14 * 24 * 60 * 60; // 14 days

#ifdef __IBMCPP__
#  include <sys/stat.h>
#endif

static bool file_is_newer( const string &fn, time_t t )
    {
    struct stat info;
    if( stat( fn, &info ) ) throw Ex_Errno( "file_is_newer", "Can't stat file "+fn, errno );
    return info.st_mtime > t ? Yes : No;
    }
    

bool need_announce()
    {
    time_t now = time(0);
    time_t last_announce = -1;
    time_t last_announce_file = -1;
    
    ifstream  la_time( conf.dir()+"/Announce.Tim" );
    if( !la_time )
        {
        Warning("No annonce time stamp found. Running for the first time?");
        return Yes;
        }

    la_time >> last_announce;
    if( !la_time.fail() )
        {
        if(
           (now > conf.announce_period() + last_announce) &&
           last_announce != -1 )
            {
            Msg( "Initiating regular announce" );
            return Yes;
            }
        }
    else
        last_announce = -1;
    
    for( filer f(conf.dir(),"*.ad"); f.valid(); f.next() )
        {
        if( file_is_newer( f.current(), last_announce ) )
            {
            Msg( "File "+f.current()+" changed, initiating announce" );
            return Yes;
            }
        }

    return No;
    }




static void mark_announce()
    {
    time_t now = time(0);
    
    ofstream  la_time( conf.dir()+"/Announce.Tim" );
    if( !la_time ) throw Ex_Errno("mark_announce","Can't create file", errno );

    la_time << now;
    if( la_time.fail() )
        Ex_Errno("mark_announce","Can't write time stamp", errno);
    }









void announce_one( const string &orig )
    {
    if(!conf.silence()) Msg( "Announcing "+orig );
    string  s, l, address;
    rip     r;
    
    r.from( conf.aka[0] );
    r.to( conf.aka[0] );
    r.hops(0);
    r.type(rip_type::AD);
    r.fminus() += "NA";  // We are announcing non-our address, make sure routing is Non-Approved.
    
    time_t tt = datetime::now().gmtime();
    orig.parse( address, s );
    
    r.ad( address );
      //r.creator( address );
    r.creator( conf.aka[0] );
    
    s.strip_ws();
    while( !s.is_empty() )
        {
        s.parse( l, s );
        s.strip_ws();
        
        string orig_mod = l;
        
        char c = l[0];
        l.strip_leading( 1 );
        
        switch( c )
            {
            case '-':
                r.add_seenby( l );
                break;
            case '!':
                    {
                    char mod;
                    long add;
                    sscanf( l, "%ld%c", &add, &mod );
                    if( add < 0 )
                        Error( "Negative time, funny: "+orig_mod);
                    else
                        {
                        if( mod == 'd' || mod == 'D' ) add *= (24l * 60 * 60);
                        else /* hours */               add *= (60 * 60);
                        
                        tt -= add;
                        }
                    }
                break;
            case '&':
                    {
                    int add = atoi( l );
                    if( add < 0 )  Error( "Negative hops, funny: "+orig_mod);
                    else           r.increment_hops( add );
                    }
                break;
            default:
                Error("Unknown modifier "+orig_mod );
            }
        }
    
    //tm t = *localtime( & );
    r.created( datetime(tt, datetime::UTC) );

    r.save_to_dir( conf.dir() );
    }


void announce_aka( const addr &ann, const addr &me )
    {
    rip     r;
    
    r.from( conf.aka[0] );
    r.to( conf.aka[0] );
    r.hops(0);
    r.type(rip_type::AD);

    r.ad( ann );
    r.creator( me );
    
    //time_t tt = time(0);
    //tm t = *localtime( &tt );
    r.created( datetime::now() );
    r.save_to_dir( conf.dir() );
    }



static void announce_file( const string &fn )
    {
    BoldMsg( "Announcing from "+fn );

    FILE *f = fopen( fn, "r" );
    if( f == NULL ) throw Ex_Errno( "announce_file","Can't open file "+fn+", announce skipped", errno );

    string l;
    while( 1 )
        {
        try { l.load( f ); } catch(Ex_EOF) { break; }

        l.strip_ws();
        l.strip_crlf();

        if(l[0] == ';' || l.is_empty() )
            continue;

        try { announce_one(l); }
        catch( General_Ex ex )
            {
            Error("Can't announce for "+l);
            ex.print();
            }
        }

    fclose( f );
    }


void announce()
    {
    for( int an = 0; an < conf.aka.count(); an++ )
        {
        const addr &aka = conf.aka[an];
        
        if(!conf.silence()) Msg( "Announcing our address: "+((string)aka) );
        try { announce_aka( aka, aka ); }
        catch( General_Ex ex )     { ex.print(); }

        
        if( aka.node() == 0 && aka.point() == 0 )
            {
#if 0
            addr host = aka;
            host.node( WILD );
            Msg( "Announcing host as default for net: "+((string)host) );
            
            try { announce_aka( host, aka ); }
            catch( General_Ex ex )     { ex.print(); }
#else
            if(!conf.silence()) BoldMsg( "Shouldn't you announce default for net "+((string)aka)+"?" );
#endif
            }
        
        }
    
    if( conf.announce_from_ad_files() )
        for( filer f(conf.dir(),"*.ad"); f.valid(); f.next() )
            announce_file( f.current() );

    mark_announce();
    }


@


1.14
log
@datetime
@
text
@d8 3
d142 1
a142 1
    time_t tt = time(0);
@


1.13
log
@Silence support
@
text
@d8 3
d190 2
a191 2
    tm t = *localtime( &tt );
    r.created( t );
d209 3
a211 5
    time_t tt = time(0);
    
    tm t = *localtime( &tt );
    r.created( t );

@


1.12
log
@Added -NA to announces from .ad
Changed creator address to our address, not address
of announce.
@
text
@d8 5
d126 1
a126 1
    Msg( "Announcing "+orig );
d252 1
a252 1
        Msg( "Announcing our address: "+((string)aka) );
d267 1
a267 1
            BoldMsg( "Shouldn't you announce default for net "+((string)aka)+"?" );
@


1.11
log
@Before changing
@
text
@d8 3
d129 2
a130 1

d135 2
a136 1
    r.creator( address );
@


1.10
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d209 1
a209 1
    
d217 1
a217 1
        
d220 1
a220 1
        
@


1.9
log
@Version 25
@
text
@d8 3
a89 25
bool need_ping()
    {
    time_t now = time(0);
    time_t last_ping = -1;
    time_t last_ping_file = -1;
    
    ifstream  lp_time( conf.dir()+"/Ping.Tim" );
    if( !lp_time )
        {
        Warning("No ping time stamp found. Running for the first time?");
        return Yes;
        }
    
    lp_time >> last_ping;
    if( !lp_time.fail() )
        {
        if( (now > conf.ping_period() + last_ping) && last_ping != -1 )
            {
            Msg( "Initiating regular pinging" );
            return Yes;
            }
        }
    
    return No;
    }
a103 15

void mark_ping()
    {
    time_t now = time(0);
    
    ofstream  lp_time( conf.dir()+"/Ping.Tim" );
    if( !lp_time ) throw Ex_Errno("mark_ping","Can't create file",errno);
    
    lp_time << now;
    if( lp_time.fail() )
        throw Ex_Errno("mark_ping","Can't write time stamp",errno);
    }



@


1.8
log
@Exceptions added, not compiled
@
text
@d8 3
d28 1
d43 1
a43 1
    if( stat( fn, &info ) ) throw Ex_Errno( "file_is_newer", "Can't stat file", fn );
d120 1
a120 1
    if( !la_time ) throw Ex_Errno("mark_announce","Can't create file","");
d124 1
a124 1
        Ex_Errno("mark_announce","Can't write time stamp","");
d132 1
a132 1
    if( !lp_time ) throw Ex_Errno("mark_ping","Can't create file","");
d136 1
a136 1
        throw Ex_Errno("mark_ping","Can't write time stamp","");
d162 1
a162 1
    parse( orig, address, s );
d170 1
a170 2
        parse( s, l, s );
        
d245 1
a245 1
    if( f == NULL ) throw Ex_Errno( "announce_file","Can't open file, announce skipped", fn );
d248 1
a248 1
    while( l.load( f ) == Ok )
d250 2
d291 1
a291 1
            BoldMsg( "Shouldn't you announce default for net "+((string)host)+"?" );
@


1.7
log
@Before exceptions.
@
text
@d8 3
d39 1
a39 10
    
    if( stat( fn, &info ) )
        Error( "Can't stat "+fn );
    
    /*
    fprintf( stderr, "file (%s) %ld is newer then %ld?\n",
            (const char *)fn,
            (long)info.st_mtime, (long)t );
    */
    
d56 3
a58 1
    else
d60 3
a62 2
        la_time >> last_announce;
        if( !la_time.fail() )
d64 2
a65 7
            if( 
               (now > conf.announce_period() + last_announce) &&
               last_announce != -1 )
                {
                Msg( "Initiating regular announce" );
                return Yes;
                }
a66 2
        else
            last_announce = -1;
d68 2
a69 1
        
a82 24
bool mark_ping()
    {
    time_t now = time(0);
    
    ofstream  lp_time( conf.dir()+"/Ping.Tim" );
    if( !lp_time )
        {
        Error("Can't create ping time stamp!");
        return Err;
        }
    else
        {
        lp_time << now;
        if( lp_time.fail() )
            {
            Error("Can't write ping time stamp!");
            return Err;
            }
        }
        
    return Ok;
    }


d95 3
a97 1
    else
d99 1
a99 2
        lp_time >> last_ping;
        if( !lp_time.fail() )
d101 2
a102 7
            if( 
               (now > conf.ping_period() + last_ping) &&
               last_ping != -1 )
                {
                Msg( "Initiating regular pinging" );
                return Yes;
                }
a103 2
        else
            last_ping = -1;
a104 1
        
d111 11
d123 1
a123 1
static bool mark_announce()
d127 6
a132 17
    ofstream  la_time( conf.dir()+"/Announce.Tim" );
    if( !la_time )
        {
        Error("Can't create announce time stamp!");
        return Err;
        }
    else
        {
        la_time << now;
        if( la_time.fail() )
            {
            Error("Can't write announce time stamp!");
            return Err;
            }
        }
        
    return Ok;
d136 11
a146 1
bool announce_one( const string &orig )
a157 8
//    bool    tail = Yes;
    
//    if( !parse( orig, address, s ) )
//        {
        //  Error(".ad line incorrect: "+orig );
        //        return Err;
//        tail = No;
//        }
a160 1
    //r.creator( conf.aka[0] );
a162 3
    
    //debug( "Address is "+address );
    
d211 1
a211 1
    return r.save_to_dir( conf.dir() );
d215 1
a215 1
bool announce_aka( const addr &ann, const addr &me )
a216 1
//    string  s, l, address;
a224 1
    //r.creator( conf.aka[0] );
d232 1
a232 1
    return r.save_to_dir( conf.dir() );
d237 1
a237 1
static bool announce_file( const string &fn )
d242 1
a242 6
    
    if( f == NULL )
        {
        Error( "Can't open "+fn+", announce skipped");
        return Err;
        }
a244 1
    
d253 2
a254 1
        if( announce_one(l) == Err )
d257 1
a257 2
            //fclose( bf );
            //return Err;
a261 1
    return Ok;
d265 1
a265 1
bool announce()
a266 1
    bool allright = Yes;
d272 3
a274 2
        if( announce_aka( aka, aka ) != Ok )
            allright = No;
d278 1
d283 5
a287 2
            if( announce_aka( host, aka ) != Ok )
                allright = No;
d294 1
a294 7
            {
            if( announce_file( f.current() ) != Ok )
                allright = No;
            }
    
    
    if( allright ) mark_announce();
d296 1
a296 1
    return Ok;
a297 1

@


1.6
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: Announce.c $
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d25 4
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d82 59
a171 1
    r.creator( conf.aka[0] );
d175 1
d189 2
d247 24
d309 1
a309 1
    for( filer f(conf.dir(),"*.ad"); f.valid(); f.next() )
d311 4
a314 1
        if( announce_file( f.current() ) != Ok )
d316 11
d328 9
a336 1

d341 3
@


1.3
log
@before COW strings
@
text
@d1 11
@


1.2
log
@beta 9
@
text
@d99 6
d117 1
d120 1
a120 1
    s.strip_leading_ws();
d125 1
a125 1
        s.strip_leading_ws();
d136 23
a158 1
                //debug( "-"+l );
a164 6
    r.creator( conf.aka[0] );
    r.from( conf.aka[0] );
    r.to( conf.aka[0] );
    r.hops(0);

    time_t tt = time(0);
a165 1
        
d167 1
d188 1
a188 1
        l.strip_leading_ws();
@


1.1
log
@Initial revision
@
text
@d7 1
a7 1
static const announce_period = 7 * 24 * 60 * 60; // 7 days
d16 6
d44 1
a44 1
               (now > announce_period + last_announce) && 
d58 1
a58 1
        if( file_is_newer( f.current(), now ) )
d60 1
a60 1
            Msg( "File "+f.current()+", initiating announce" );
@

/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Announcer
 *
 *      $Log: Announce.C $
 *      Revision 1.15  1997/03/26 10:56:58  dz
 *      datetime::now().gmtime()
 *
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
    
    r.add_comment("Created by announce_one");

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

    r.add_comment("Created by announce_aka");
    
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



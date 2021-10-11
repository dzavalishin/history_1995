/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Links definition
 *
 *      $Log: Links.C $
 *      Revision 1.19  1997/03/22 15:49:23  dz
 *      datetime
 *
 *      Revision 1.18  1997/01/07 10:02:56  dz
 *      Ver 29
 *
 *      Revision 1.17  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.16  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.15  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.14  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.13  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.12  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.11  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.10  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.9  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.7  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.6  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "links.h"
#include "sglist.h"

#include <ctype.h>
#include <string.h>

link::link( void )
    :
last_heard_from( 0, datetime::UTC )
    {
    clear();
    }

void link::clear( void )
    {
    for( int i = 0; i < link_n_ping; i++ )
        ping_delay[i] = 0; // 0 is invalid value
    last_heard_from = datetime( 0, datetime::UTC );
    }


void ad_filter::set_time_inc(string s)
    {
    s.ucase();
    int time_inc = 0;
    if( isdigit(s[0]) )
        {
        time_inc = atol(s);
        while(isdigit(s[0]))
            s.strip_leading(1);
        switch( s[0] )
            {
            case 'D': case 'd': time_inc *= 24;  // fall through...
            case 'H': case 'h': time_inc *= 60;  // fall through...
            case 'M': case 'm': time_inc *= 60;  // fall through...
            case 'S': case 's': break;
            default:  Error("Invalid time modifier, seconds assumed: "+s);
                break;
            }
        }
    else if( s == "WEEK" ) time_inc = 7*24L*60L*60L;      // Once a week
    else if( s == "DAY" )  time_inc = 24L*60L*60L;        // Once a day
    else if( s == "ZMH" )  time_inc = 24L*60L*60L;        // Once a day
    else if( s == "HOLD" ) time_inc = 2*24L*60L*60L;      // Once in a 2 days
    else if( s == "!CM" )  time_inc = 12L*60L*60L;        // Twice a day
    else if( s == "CM" )   time_inc = 0L;                 // Immediately
    else Error("Can't parse TIME+="+s );
    time_inc_v = datetime( time_inc, datetime::UTC );
    }


// if this rip should pass this filter
bool ad_filter::pass( const rip &r ) const
    {
    
    // 1. check address against stop/go filter
    
    stopgo::sg result = stopgo::Go;

    for( int i = 0; i < sg_v.size(); i++ )
        if( r.ad().matches( sg_v[i].mask ) )
            result = sg_v[i].kind;
    
    if( result != stopgo::Go ) return No;
    
    // 2. check whether address is wild enough for this filter
    
    return wild_v.match(r.ad());
    }



void links::add_user( string s )
    {
    string mas, has, orig = s;
    
    if( !s.parse( mas, s ) )
        throw Ex_Arg("links::add_user","Incorrect line in links.cfg",orig);
    s.parse( has, s );

    link  nl;
    nl.myaddr_v  = addr( mas );
    nl.hisaddr_v = addr( has );

    ad_filter *f = &(nl.out_v);
    
    while( s.strip_leading_ws(), !s.is_empty() )
        {
        string el;
        s.parse( el, s );
        
        string kw = el;
        kw.ucase();
        
        if( kw == "IN" )                         f = &(nl.in_v);
        else if( kw == "OUT" )                   f = &(nl.out_v);
        else if( el[0] == '-' || el[0] == '+' )  f->add_sg( stopgo( el ) );
        else if( kw.substr( 0, 5 ) == "WILD=" )  f->set_wild( kw.substr( 5 ) );
        else if( kw.substr( 0, 6 ) == "TIME+=" ) f->set_time_inc( kw.substr( 6 ) );
        else
            Msg("Unknown modifier "+el+" in "+orig);
        }
    
    l.push_back( nl );
    }

void links::add_base( string s )
    {
    string mas, has, orig = s;
    
    //debug("Link base: " + s);
    
    if( (!s.parse( mas, s )) || (!s.parse( has, s )) )
        throw Ex_Arg("links::add_base","Incorrect line in links.dat",orig);

    addr myaddr  = addr( mas );
    addr hisaddr = addr( has );

    string data;
    
    int  ping_delay[link_n_ping];

    for( int i = 0; i < link_n_ping; i++ )
        {
        if( !s.parse( data, s ) )
            throw Ex_Arg("links::add_base","Incorrect line in links.dat",orig);
        ping_delay[i] = atoi(data);
        }

    string lhf, capas, flags;
    
    s.parse( lhf, s );

    do
        {
        string temp;
        s.parse( temp, s );
        capas += temp;
        capas += " ";
        }
    while(!(s.substr(0,1) == ":"));
      //debug("capas = "+capas );
    capas.strip_ws();
    
    do
        {
        if( s.is_empty() )
            throw Ex_Arg("links::add_base","Incorrect line in links.dat",orig);
        string temp;
        s.parse( temp, s );
        flags += temp;
        flags += " ";
        }
    while(!(s == "!"));
      //debug("flags = "+flags );
    flags.strip_ws();
    
    datetime last_heard_from = datetime::from_time_t_string( lhf, datetime::UTC );

    for( i = 0; i < l.size(); i++ )
        if( l[i].myaddr() == myaddr && l[i].hisaddr() == hisaddr )
            {
            l[i].flags = flags.substr(1);
            l[i].capas = capas.substr(1);
            l[i].last_heard_from = last_heard_from;

            for( int j = 0; j < link_n_ping; j++ )
                l[i].ping_delay[j] = ping_delay[j];
            
            return;
            }

    }


void links::save_base()
    {
    FILE *bf = fopen( conf.dir()+"/links1.dat", "w" );
    
    if( bf == NULL )
        throw Ex_Errno("links::save_base","Can't create links1.dat", errno );

    for( int i = 0; i < l.size(); i++ )
        {
        fprintf
            ( bf, "%s %s ",
             (string(l[i].myaddr())).c_str(),
             (string(l[i].hisaddr())).c_str()
            );

        for( int j = 0; j < link_n_ping; j++ )
            fprintf( bf, "%d ", l[i].ping_delay[j] );
            
        fprintf
            ( bf, "%d :%s :%s !\n",
             l[i].last_heard_from,
             l[i].capas.c_str(),
             l[i].flags.c_str()
            );
            
        }
    
    fclose( bf );

    unlink(conf.dir()+"/links.dat");
    if( rename(conf.dir()+"/links1.dat",conf.dir()+"/links.dat") )
        throw Ex_Errno("links::save_base","Can't rename "+conf.dir()+"/links1.dat to"+conf.dir()+"/links.dat", errno );
    }


/*
  //void links::foreach( const rip &r, void (*one)( rip r, const link &l ) )
void links::foreach( const rip &r, link_foreach_t one )
    {
    load();

    bool err = No;
    for( int i = 0; i < l.size(); i++ )
        {
        try { one( r, l[i] ); }
        catch( General_Ex ex )
            {
            ex.print();
            err = Yes;
            }
        }
    if( err )
        throw Ex_Fail("links::foreach", "Some link failed", "" );
    }
*/

bool links::check_in( const rip &r, link &li ) const
    {
    if( !loaded ) throw Ex_Fail("links::check_in","links base is not loaded", "");

    for( int i = 0; i < l.size(); i++ )
        {
        const link &ll = l[i];
        if( ll.hisaddr() != r.from() )   continue;
        li = ll;
        return ll.will_pass_in( r );
        }
    
    BoldMsg("Got a rip from unknown node: "+(string)r.from());
    return No;
    }

bool links::is_from_link( const rip &r ) const
    {
    if( !loaded ) throw Ex_Fail("links::not_from_link","links base is not loaded", "");
    for( int i = 0; i < l.size(); i++ )
        if( (l[i]).hisaddr() == r.from() ) return Yes;
    return No;
    }


void links::load()
    {
    if( loaded ) return;
    load_user();
    loaded = Yes;
    load_base();
    //loaded = Yes;
    }

void links::load_user()
    {
    FILE *bf = fopen( conf.dir()+"/links.cfg", "r" );
    
    if( bf == NULL )
        throw Ex_Errno("links::load_user","Can't open "+conf.dir()+"/links.cfg", errno );

    string l;
    while( 1 )
        {
        // try { l.load( bf ); } catch(Ex_EOF) { break; }
        try { l.load( bf ); } catch(Ex_EOF ex) { break; }
        l.strip_leading_ws();
        l.strip_crlf();
        
        if(l[0] == ';' || l.is_empty() )
            continue;

        add_user(l);
        }

    fclose( bf );
    }

void links::load_base()
    {
    FILE *bf = fopen( conf.dir()+"/links.dat", "r" );
    if( bf == NULL )
        {
        bf = fopen( conf.dir()+"/links1.dat", "r" );
        if( bf == NULL )
            throw Ex_Errno("links::load_user","Can't open "+conf.dir()+"/links.dat", errno );
        }
    
    string l;
    while( 1 )
        {
        try { l.load( bf ); } catch(Ex_EOF) { break; }
        
        l.strip_leading_ws();
        l.strip_crlf();
        
        if(l[0] == ';' || l.is_empty() )
            continue;
        
        try { add_base(l); }
        catch( General_Ex ex )  { ex.print(); }
        }

    fclose( bf );
    }

void links::process_ping( const rip &r )
    {
    load();

    for( int i = 0; i < l.size(); i++ )
        if( l[i].myaddr() == r.to() && l[i].hisaddr() == r.from() )
            {
            
            if( !strstr(l[i].flags, "PING" ) ) l[i].flags += "PING";
            string dummy;
            r.capas().parse( dummy, l[i].capas );

            datetime now = datetime::now();
            
            l[i].last_heard_from = now;

            datetime riptime = r.created();
            datetime delay = now - riptime;

            long aver = 0;
            int  anum = 1;
            
            for( int j = 0; j < link_n_ping-1; j++ )
                {
                int val = l[i].ping_delay[j] = l[i].ping_delay[j+1];
                if(val)
                    {
                    anum++;
                    aver += val;
                    }
                }

            l[i].ping_delay[link_n_ping-1] = delay;
            aver += delay;

            aver /= anum;

            char temp[100];
            sprintf( temp, "Average ping time is %d seconds", aver );
            Msg(temp);
            
            return;
            }

    Error("Ping from undefined link");
    }




    
void deliver( const addr &a, const string &fn_a )
    {
    char fn[_MAX_PATH];
    if( NULL == _fullpath( fn, (char *)(const char *)fn_a, _MAX_PATH ) )
        throw Ex_Fail( "deliver", "Can't get full path", fn_a );
    
    bool hold = conf.hold( a );
    
    BinkAttach( fn, a, hold );
    HPFSAttach( fn, a, hold );
    TMailAttachFAT( fn, a, hold );
    TMailAttachHPFS( fn, a, hold );
    
    if( conf.deliver_command().is_empty() )
        return;

    string sfn = fn;
    string sa = (string)a;
    
    string s = conf.deliver_command();
    s.insert( &sfn, &sa );

    if( system( s ) )
        throw Ex_Errno( "deliver", "Can't run "+s, errno );
    }


/*
void FripApp::send_to_the_link( rip r, const link &l )
    {
    Msg("    Sending to "+((string)l.hisaddr()) );

    mkdir( (char *)(const char *)(conf.dir()+"/out") );
    
    string tempn = tempFileName(conf.dir()+"/out/rip?????.rip");
    
        {
        // Up to the end of this block temp will be open
        ofstream temp( tempn );
        if( !temp )
            throw Ex_Errno( "send_to_the_link", "can't create "+tempn, errno );
        
        r.from(l.myaddr());
        r.to(l.hisaddr());
        r.save_to_file( temp );
        }

    ZipList_add( tempn, l.hisaddr() );
    }
*/
    
void FripApp::send_rip_to_link( rip r, const link &l, bool do_out_checks )
    {

    if( do_out_checks )
        {
        //bool passme = l.will_pass_out( r );
    
        if( (r.type() == rip_type::AD) && (!l.will_pass_out( r )) )
            {
            Msg("    Will not come to "+((string)l.hisaddr())+" due to restriction");
            return;
            }

        if( r.seenby_matches( l.hisaddr() ) )
            {
            Msg( "    Will not come to "+((string)l.hisaddr())+" due to seenby");
            return;
            }
        }

    Msg("    Will come to "+((string)l.hisaddr()) );

    mkdir( (char *)(const char *)(conf.dir()+"/out") );
    
    string tempn = tempFileName(conf.dir()+"/out/rip?????.rip");
    
        {
        // Up to the end of this block temp will be open
        ofstream temp( tempn );
    
        if( !temp )
            throw Ex_Errno( "FripApp::send_rip_to_link", "can't create "+tempn, errno );
        
        if(r.type() == rip_type::AD)
            r.increment_time( l.out().time_inc() );
        r.from(l.myaddr());
        r.to(l.hisaddr());
        r.save_to_file( temp );
        }

    zlist.add( tempn, l.hisaddr() );
    }


void FripApp::send_rip_to_all_links( const rip &r, bool do_out_checks )
    {
    links::load();
    
    bool err = No;
    for( int i = 0; i < links::size(); i++ )
        {
        try { send_rip_to_link( r, links::operator [] (i), do_out_checks ); }
        catch( General_Ex ex )
            {
            ex.print();
            err = Yes;
            }
        }
    if( err )
        throw Ex_Fail("links::foreach", "Some link failed", "" );
    }



void FripApp::ping()
    {
    BoldMsg("Pinging...");
    rip r;
    
    r.type( rip_type::PING );
    r.from( conf.aka[0] );
    r.creator( conf.aka[0] );
    r.hops(0);
    r.created( datetime::now() );

    send_rip_to_all_links( r, No );
    }


void FripApp::send_rescan_req()
    {
    BoldMsg("Sendingh rescan requests...");
    rip r;
    
    r.type( rip_type::RRQ );
    r.from( conf.aka[0] );
    r.creator( conf.aka[0] );
    r.hops(0);
    r.created( datetime::now() );

    send_rip_to_all_links( r, No );
    }



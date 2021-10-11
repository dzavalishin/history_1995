head	1.19;
access;
symbols
	Beta_12:1.6;
locks;
comment	@ *      @;


1.19
date	97.03.22.15.49.23;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	97.01.07.10.02.56;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.01.22.19.23.05;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.01.18.00.56.53;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.01.15.03.50.54;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.13.06.46.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.19
log
@datetime
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Links definition
 *
 *      $Log: Links.C $
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


@


1.18
log
@Ver 29
@
text
@d8 3
d56 2
d66 1
a66 1
    last_heard_from = 0;
d73 1
a73 1
    time_inc_v = 0;
d76 1
a76 1
        time_inc_v = atol(s);
d81 3
a83 3
            case 'D': case 'd': time_inc_v *= 24;  // fall through...
            case 'H': case 'h': time_inc_v *= 60;  // fall through...
            case 'M': case 'm': time_inc_v *= 60;  // fall through...
d89 6
a94 6
    else if( s == "WEEK" ) time_inc_v = 7*24L*60L*60L;      // Once a week
    else if( s == "DAY" )  time_inc_v = 24L*60L*60L;        // Once a day
    else if( s == "ZMH" )  time_inc_v = 24L*60L*60L;        // Once a day
    else if( s == "HOLD" ) time_inc_v = 2*24L*60L*60L;      // Once in a 2 days
    else if( s == "!CM" )  time_inc_v = 12L*60L*60L;        // Twice a day
    else if( s == "CM" )   time_inc_v = 0L;                 // Immediately
d96 1
d206 1
a206 1
    time_t last_heard_from = atoi(lhf);
d378 1
a378 1
            time_t now = time(0);
d382 2
a383 2
            time_t riptime = mktime( &((tm&)r) );
            time_t delay = now - riptime;
d541 1
a541 1
    r.created( time(0) );
d556 1
a556 1
    r.created( time(0) );
@


1.17
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d174 1
a174 6
    if(
       (!s.parse( lhf, s )) ||
       (!s.parse( capas, s )) ||
       (!s.parse( flags, s )) ||
       s != string("!") )
        throw Ex_Arg("links::add_base","Incorrect line in links.dat",orig);
d176 24
@


1.16
log
@Version 25
@
text
@d8 3
d281 1
d283 1
a283 1
    loaded = Yes;
@


1.15
log
@Exceptions added, not compiled
@
text
@d8 3
a58 2
links llist;

d113 1
a113 1
    if( !parse( s, mas, s ) )
d115 1
a115 1
    parse( s, has, s );
d126 1
a126 1
        parse( s, el, s );
d149 1
a149 1
    if( (!parse( s, mas, s )) || (!parse( s, has, s )) )
d161 1
a161 1
        if( !parse( s, data, s ) )
d169 3
a171 3
       (!parse( s, lhf, s )) ||
       (!parse( s, capas, s )) ||
       (!parse( s, flags, s )) ||
d228 3
a230 2

void links::foreach( const rip &r, bool (*one)( rip r, const link &l ) )
d247 1
a247 1

d290 1
a290 1
    while( l.load( bf ) == Ok )
d292 2
d317 1
a317 1
    while( l.load( bf ) == Ok )
d319 2
d344 1
a344 1
            parse( r.capas(), dummy, l[i].capas );
a382 10
void send_rips( const rip &r )
    {
    llist.foreach( r, do_a_link );
    }

void load_links()
    {
    llist.load();
    }

d410 4
a413 2
    
void send_to_the_link( rip r, const link &l )
d434 1
d436 1
a436 2
    
void do_a_link( rip r, const link &l )
d438 4
a441 1
    bool passme = l.will_pass_out( r );
d443 5
a447 5
    if( (r.type() == rip_type::AD) && (!passme) )
        {
	Msg("    Will not come to "+((string)l.hisaddr())+" due to restriction");
	return;
        }
d449 5
a453 4
    if( r.seenby_matches( l.hisaddr() ) )
        {
        Msg( "    Will not come to "+((string)l.hisaddr())+" due to seenby");
        return;
d467 1
a467 1
            throw Ex_Errno( "do_a_link", "can't create "+tempn, errno );
d476 1
a476 1
    ZipList_add( tempn, l.hisaddr() );
d480 18
d500 1
a500 1
void ping()
d511 1
a511 1
    llist.foreach( r, send_to_the_link );
d515 1
a515 1
void send_rescan_req()
d526 1
a526 1
    llist.foreach( r, send_to_the_link );
@


1.14
log
@Before exceptions.
@
text
@d8 3
d48 1
a48 2
void
link::clear( void )
d72 1
a72 2
            case 'S': case 's':            
                break;
d77 7
a83 14
    else if( s == "WEEK" )
        time_inc_v = 7*24L*60L*60L;      // Once a week
    else if( s == "DAY" )
        time_inc_v = 24L*60L*60L;        // Once a day
    else if( s == "ZMH" )
        time_inc_v = 24L*60L*60L;        // Once a day
    else if( s == "HOLD" )
        time_inc_v = 2*24L*60L*60L;      // Once in a 2 days
    else if( s == "!CM" )
        time_inc_v = 12L*60L*60L;        // Twice a day
    else if( s == "CM" )
        time_inc_v = 0L;                 // Immediately
    else
        Error("Can't parse TIME+="+s );
d108 1
a108 1
bool links::add_user( string s )
a111 2
    //debug("Link: " + s);
    
d113 1
a113 5
        {
        Error("Incorrect line in links.cfg: "+orig);
        return Err;
        }
    
d115 1
a115 1
    
a116 1
    
d119 1
a119 3
    
    
    //enum { In, Out } mode = Out;
d130 5
a134 8
        if( kw == "IN" )       f = &(nl.in_v);
        else if( kw == "OUT" ) f = &(nl.out_v);
        else if( el[0] == '-' || el[0] == '+' )
            f->add_sg( stopgo( el ) );
        else if( kw.substr( 0, 5 ) == "WILD=" )
            f->set_wild( kw.substr( 5 ) );
        else if( kw.substr( 0, 6 ) == "TIME+=" )
            f->set_time_inc( kw.substr( 6 ) );
a136 1
        
a138 1
    
a139 1
    return Ok;
d142 1
a142 1
bool links::add_base( string s )
d149 1
a149 4
        {
        Error("Incorrect line in links.dat: "+orig);
        return Err;
        }
d161 1
a161 4
            {
            Error("Incorrect line in links.dat: "+orig);
            return Err;
            }
d172 1
a172 4
        {
        Error("Incorrect line in links.dat: "+orig);
        return Err;
        }
a178 1

d186 1
a186 1
            return Ok;
a188 2
    
    return Ok;
d192 1
a192 1
bool links::save_base()
d197 1
a197 4
        {
        perror("Can't create links1.dat");
        return Err;
        }
d223 1
a223 6
        {
        Error( "Can't rename "+conf.dir()+"/links1.dat to"+conf.dir()+"/links.dat" );
        return Err;
        }
    
    return Ok;
d228 1
a228 1
bool links::foreach( const rip &r, bool (*one)( rip r, const link &l ) )
d230 1
a230 1
    bool rc = Ok;
d232 1
a232 2
    if( Err == load() ) return Err;
    
d234 10
a243 4
        if( one( r, l[i] ) == Err )
            rc = Err;
    
    return rc;
d249 1
a249 3
//debug("links::check_in()" );

    //if( Err == load() ) return Err;
d254 1
a254 4
        
        if( ll.hisaddr() != r.from() )
            continue;
        
d256 1
a256 2
        
        return ll.will_pass_in( r ) == Yes ? Ok : Err;
a258 1
//debug("links::check_in() err" );
d260 1
a260 1
    return Err;
d263 1
a263 1
bool links::not_from_link( const rip &r ) const
d265 1
a265 4
      //debug("links::check_in()" );
    
      //if( Err == load() ) return Err;
    
d267 2
a268 10
        {
        const link &ll = l[i];
        
        if( ll.hisaddr() != r.from() )
            continue;
        
        return Ok;
        }
    
    return Err;
d272 1
a272 1
bool links::load()
d274 2
a275 3
    if( loaded ) return Ok;
    if( load_user() == Err ) return Err;
      //if( load_base() == Err ) return Err;
a277 1
    return Ok;
d280 1
a280 1
bool links::load_user()
d285 1
a285 4
        {
        perror("Can't open links.cfg");
        return Err;
        }
a287 1
    
d296 1
a296 6
        if( add_user(l) == Err )
            {
            perror("Can't load links");
            fclose( bf );
            return Err;
            }
a297 1
	
a299 1
    return Ok;
d302 1
a302 1
bool links::load_base()
d309 1
a309 4
            {
            perror("Can't open links.dat");
            return Err;
            }
a312 1
    
d321 2
a322 2
        if( add_base(l) == Err )
            perror("Can't load links.dat record: "+l);
a325 1
    return Ok;
d328 1
a328 1
bool links::process_ping( const rip &r )
d330 1
a330 1
    if( Err == load() ) return Err;
d369 1
a369 1
            return Ok;
a371 1

a372 1
    return Err;
d377 1
a377 1
bool send_rips( const rip &r )
d379 1
a379 2
    bool rc = llist.foreach( r, do_a_link );
    return rc;
d382 1
a382 1
bool load_links()
d384 1
a384 1
    return llist.load();
d389 1
a389 1
bool deliver( const addr &a, const string &fn_a )
d393 1
a393 4
        {
        Error("Can't get full path for "+fn_a );
        return Err;
        }
d397 4
a400 7
    bool ret = Ok;
    
    if( Err == BinkAttach( fn, a, hold ) )
        ret = Err;
    
    if( Err == HPFSAttach( fn, a, hold ) )
        ret = Err;
a401 7
    if( Err == TMailAttachFAT( fn, a, hold ) )
        ret = Err;
    
    if( Err == TMailAttachHPFS( fn, a, hold ) )
        ret = Err;
    
    //return system(string("deliver ")+fn+string(" ")+((string)a)) ? Err : Ok;
d403 1
a403 1
        return ret;
a407 2
    //debug( "sa = " + sa );

d411 2
a412 2
    //debug( "cmd = " + s );
    return system( s ) ? Err : ret;
d415 1
a415 1
bool send_to_the_link( rip r, const link &l )
a422 6
    if( tempn=="" )
        {
        Error( "rip generation aborted - no temp file name" );
        return Err;
        }

a425 1
    
d427 1
a427 4
            {
            Error( "rip generation aborted - can't create file" );
            return Err;
            }
a434 1
    return Ok;
d438 1
a438 1
bool do_a_link( rip r, const link &l )
d445 1
a445 1
	return Ok;
d451 1
a451 1
        return Ok;
a459 6
    if( tempn=="" )
        {
        Error( "rip generation aborted - no temp file name" );
        return Err;
        }

d465 1
a465 4
            {
            Error( "rip generation aborted - can't create file" );
            return Err;
            }
a473 2
    // Msg("      - delivering "+string(tempn)+" to "+((string)l.hisaddr()) );
    
a474 2
    return Ok;
//    return deliver( l.hisaddr(), tempn );
a477 38
#if 0
bool ping_a_link( rip r, const link &l )
    {
    Msg("    Sending ping to "+((string)l.hisaddr()) );


    mkdir( conf.dir()+"/out" );
    
    string tempn = tempFileName(conf.dir()+"/out/rip?????.rip");
    
    if( tempn=="" )
        {
        Error( "rip generation aborted - no temp file name" );
        return Err;
        }

        {
        // Up to the end of this block temp will be open
        ofstream temp( tempn );
    
        if( !temp )
            {
            Error( "rip generation aborted - can't create file" );
            return Err;
            }
        
        r.from(l.myaddr());
        r.to(l.hisaddr());
        r.save_to_file( temp );
        }

    // Msg("      - delivering "+string(tempn)+" to "+((string)l.hisaddr()) );
    
    ZipList_add( tempn, l.hisaddr() );
    return Ok;
//    return deliver( l.hisaddr(), tempn );
    }
#endif
d480 1
a480 1
bool ping()
d491 1
a491 2
    bool rc = llist.foreach( r, send_to_the_link );
    return rc;
d495 1
a495 1
bool send_rescan_req()
d506 1
a506 2
    bool rc = llist.foreach( r, send_to_the_link );
    return rc;
@


1.13
log
@Clean up of watcom classlib based code
@
text
@d8 3
d38 15
d114 1
a114 1
bool links::add( string s )
d163 106
d272 2
d287 2
d306 19
d328 10
d356 1
a356 1
        if( add(l) == Err )
d367 80
@


1.12
log
@FripMail written
@
text
@d8 3
d82 1
a82 1
#if USE_STL
a85 5
#else
    for( int i = 0; i < sg_v.entries(); i++ )
        if( r.ad().matches( sg_v[i]->mask ) )
            result = sg_v[i]->kind;
#endif
a109 1
#if USE_STL
a110 3
#else
    link  &nl = *new link;
#endif
a140 1
#if USE_STL
a141 3
#else
    l.insert( &nl );
#endif
a148 1
#if USE_STL
a151 5
#else
    for( int i = 0; i < l.entries(); i++ )
        if( one( r, *(l[i]) ) == Err )
            rc = Err;
#endif
a160 1
#if USE_STL
a171 13
#else
    for( int i = 0; i < l.entries(); i++ )
        {
        link &ll = *(l[i]);
        
        if( ll.hisaddr() != r.from() )
            continue;
        
        li = ll;
        
        return ll.will_pass_in( r ) == Yes ? Ok : Err;
        }
#endif
@


1.11
log
@C Set, mostly works
@
text
@d7 4
a10 1
 *      $Log: Links.c $
a243 2
bool do_a_link( rip r, const link &l );
    
d336 1
a336 1
    if( !passme )
a347 1
    
a349 1

d370 2
a371 1
        r.increment_time( l.out().time_inc() );
@


1.10
log
@C Set & STL support
@
text
@d8 3
d30 1
@


1.9
log
@Beta 15
@
text
@d8 3
d72 5
d80 1
d105 3
d109 1
d140 3
d144 1
d152 5
d160 1
d170 13
d194 1
d255 1
a255 1
    if( NULL == _fullpath( fn, fn_a, _MAX_PATH ) )
d297 1
a297 1
    mkdir( conf.dir()+"/out" );
d347 1
a347 1
    mkdir( conf.dir()+"/out" );
@


1.8
log
@wildness filter
@
text
@d22 38
d113 2
a114 2
        if( kw == "IN" ) mode = In;
        else if( kw == "OUT" ) mode = Out;
d116 1
a116 6
            {
            if( mode == Out )
                nl.out_v.add_sg( stopgo( el ) );
            if( mode == In )
                nl.in_v.add_sg( stopgo( el ) );
            }
d118 3
a120 6
            {
            if( mode == Out )
                nl.out_v.set_wild( kw.substr( 5 ) );
            if( mode == In )
                nl.in_v.set_wild( kw.substr( 5 ) );
            }
d143 1
a143 1
bool links::check_in( const rip &r )
d154 2
d256 32
d330 1
d344 1
a344 1

d381 2
d395 17
a411 1
    bool rc = llist.foreach( r, ping_a_link );
d414 1
@


1.7
log
@*** empty log message ***
@
text
@d8 3
a18 1
//#include "rip.h"
a19 1
//#include "conf.h"
d23 3
a25 2
// if this outgoing rip should pass this link
bool link::will_pass_out( const rip &r ) const  
d27 3
d31 9
a39 5
    for( int i = 0; i < sg_out_v.entries(); i++ )
        if( r.ad().matches( sg_out_v[i]->mask ) )
            result = sg_out_v[i]->kind;

    return ( result == stopgo::Go ) ? Yes : No;
a42 13
// if this incoming rip should pass this link
bool link::will_pass_in( const rip &r ) const
    {
//debug("link::will_pass_in()" );
    stopgo::sg result = stopgo::Go;
    for( int i = 0; i < sg_in_v.entries(); i++ )
        if( r.ad().matches( sg_in_v[i]->mask ) )
            result = sg_in_v[i]->kind;

//debug("link::will_pass_in() done" );
    return ( result == stopgo::Go ) ? Yes : No;
    }

a62 9
    /*
    string elem;
    while( parse( s, elem, s ) )
        nl.sg_v.insert( new stopgo( elem ) );
    
    elem.strip_leading_ws();
    if( !elem.is_empty() )
    nl.sg_v.insert( new stopgo( elem ) );
    */
d64 2
a65 1
    enum { In, Out } mode = Out;
d80 1
a80 1
                nl.sg_out_v.insert( new stopgo( el ) );
d82 8
a89 1
                nl.sg_in_v.insert( new stopgo( el ) );
d265 1
a265 16
#if 0
        temp << "from " <<    ((string)l.myaddr()).c_str()       << "\n";
        temp << "to " <<      ((string)l.hisaddr()).c_str()      << "\n";
        temp << "hops " <<    r.hops()                           << "\n";
        temp << "created " << ((const char *)r.created_s())      << "\n";
        temp << "creator " << ((string)r.creator()).c_str()      << "\n";
        temp << "ad " <<      ((string)r.ad()).c_str()           << "\n";
    
        temp << "path " << ((string)conf.aka[0]).c_str()         << "\n";
        r.put_path( temp );

        for( int i = 0; i < conf.aka.count(); i++ )
            temp << "seenby " << ((string)conf.aka[i]).c_str()   << "\n";
        r.put_seenby( temp );
        r.put_unknown( temp );
#else
a268 1
#endif
@


1.6
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d270 1
a270 1

d285 44
d338 10
d349 3
@


1.5
log
@before COW strings
@
text
@d1 11
a11 2
#include <wcvector.h>
#include <fstream.h>
d13 1
a13 1
#include "rip.h"
d15 1
a15 1
#include "conf.h"
d34 1
d40 1
d117 2
d129 1
@


1.4
log
@beta 9
@
text
@d10 2
a11 1
bool link::will_pass( const rip &r ) const  // if this rip should pass this link
d14 3
a16 4
    for( int i = 0; i < sg_v.entries(); i++ )
        //if( match_addr( r.ad(), sg_v[i]->mask ) )
        if( r.ad().matches( sg_v[i]->mask ) )
            result = sg_v[i]->kind;
d22 11
d53 1
d60 27
a86 1
        nl.sg_v.insert( new stopgo( elem ) );
d104 16
d216 1
a216 1
    bool passme = l.will_pass( r );
@


1.3
log
@beta 7
@
text
@d7 1
a7 16


stopgo::stopgo( string s )
    {
    s.strip_leading_ws();
    
    kind=Go;
    if( s[0] == '-' )
        kind = Stop;
    if( s[0] == '-' || s[0] == '+' )
        s.strip_leading(1);
    
    mask = addr( s );
    
    //debug( string("got ") + (kind==Go?"Go  : ":"Stop: ") + ((string)mask) );
    }
d106 2
a107 1
    return llist.foreach( r, do_a_link );
a118 3
    if( conf.deliver_command().is_empty() )
        return Ok;
    
d126 2
d130 1
a130 1
    if( Err == BinkAttach( fn, a ) )
d133 1
a133 1
    if( Err == HPFSAttach( fn, a ) )
d136 1
a136 1
    if( Err == TMailAttachFAT( fn, a ) )
d139 1
a139 1
    if( Err == TMailAttachHPFS( fn, a ) )
d166 1
a166 1
	Msg("    This announce will not come to "+((string)l.hisaddr())+" due to restriction");
d172 1
a172 1
        Msg( "    This announce will not come to "+((string)l.hisaddr())+" due to seenby");
d177 1
a177 1
    Msg("    This announce will come to "+((string)l.hisaddr()) );
d216 5
a220 2
    Msg("      - delivering "+string(tempn)+" to "+((string)l.hisaddr()) );
    return deliver( l.hisaddr(), tempn );
@


1.2
log
@b6
@
text
@d29 2
a30 1
        if( match_addr( r.ad(), sg_v[i]->mask ) )
d133 3
d142 29
a170 1
    return system(string("deliver ")+fn+string(" ")+((string)a)) ? Err : Ok;
@


1.1
log
@Initial revision
@
text
@d130 1
a130 1
bool deliver( const addr &a, const string &fn )
d132 6
@

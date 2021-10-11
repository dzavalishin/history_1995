head	1.26;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ *      @;


1.26
date	97.03.26.10.59.01;	author dz;	state Exp;
branches;
next	1.25;

1.25
date	97.03.22.15.49.53;	author dz;	state Exp;
branches;
next	1.24;

1.24
date	97.03.17.05.49.26;	author dz;	state Exp;
branches;
next	1.23;

1.23
date	97.03.05.20.32.06;	author dz;	state Exp;
branches;
next	1.22;

1.22
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.21;

1.21
date	97.01.31.10.14.17;	author dz;	state Exp;
branches;
next	1.20;

1.20
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.19;

1.19
date	97.01.02.10.06.18;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	97.01.01.21.31.39;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.12.28.13.42.00;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.18.00.56.53;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.14.01.06.31;	author dz;	state Exp;
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


1.26
log
@-RES, -RIF filters
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main processing loop
 *
 *      $Log: mainloop.c $
 *      Revision 1.25  1997/03/22 15:49:53  dz
 *      see diff
 *
 *      Revision 1.24  1997/03/17 05:49:26  dz
 *      RIP date sanity check
 *      Timeout routes deletion
 *
 *      Revision 1.23  1997/03/05 20:32:06  dz
 *      offs are not sending rescans back
 *
 *      Revision 1.21  1997/01/31 10:14:17  dz
 *      Silence support
 *
 *      Revision 1.20  1997/01/21 08:53:51  dz
 *      OFF dupecheck
 *      RRQ support
 *
 *      Revision 1.19  1997/01/02 10:06:18  dz
 *      Correct order of attach/message send.
 *
 *      Revision 1.18  1997/01/01 21:31:39  dz
 *      msg received time/date support
 *
 *      Revision 1.14  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.13  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.12  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.11  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.9  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.8  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

/**
 *
 * The software included, file formats and basic algorithms are
 * copyright (C) 1995 by Dmitry Zavalishin. All rights reserved.
 *
**/

#include "frip.h"
#include "rip.h"
#include "rif.h"
#include "base.h"
#include "mail.h"
#include "approve.h"
#include "filer.h"
#include "stat.h"

#include <io.h>
#include <errno.h>






//static void do_one_rip( string in );

static bool did_something = No;
static void create_processed_flag_file()
    {
    if( !did_something ) return;
    if( !conf.processed_flag() ) return;
    close( creat( conf.processed_flag_file(), 0666 ));
    }

    

void FripApp::eat_rips_in( const string &dir )
    {
    for( filer finrip(dir,"rip*.rip"); finrip.valid(); finrip.next() )
        {
        string fn = dir + "/" + finrip.current();
        debug( "Looking at: " + fn );

        try { do_one_rip( fn ); unlink( fn ); }
        catch( General_Ex ex )  { ex.print(); }
        
        if( Stop_Frip ) 
            {
            Error("Frip execution shut down by signal or ^C, unprocessed rips left.");
            break;
            }
        }
    
    const safe = 20;
    static int safety = safe;
    
    if( safety-- <= 0 )
        {
        safety = safe;
        debug("  Saving database for safety reasons" );
        db.save();
        }
    
    }



static void map_rip( const string &rip, const addr &dest )
    {
    char buf[300];
    sprintf( buf, conf.map_format(), 
            (const char *)dest.domain(),
            (const char *)dest.zone_s(),
            (const char *)dest.net_s(),
            (const char *)dest.node_s(),
            (const char *)dest.point_s(),
            (const char *)rip
           );
    string mapfn = conf.map_dir() + "/" + buf;
    debug("  Mapping to "+mapfn );
    
    for( int i = 0; i < mapfn.length(); i++ )
        if( mapfn[i] == '*' )
            mapfn[i] = '$';
    
    unlink( mapfn );
    if( rename( rip, mapfn ) == 0 )
        return;
    
    //fprintf( stderr, "errno = %d\n", errno );
    
    switch( errno )
        {
        case EXDEV:
            try
                {
                frip_copy( rip, mapfn );
                return;
                }
            catch(...)
                {
                mkpath( mapfn );
                frip_copy( rip, mapfn );
                }

        case ENOENT:
#ifndef __IBMCPP__
        case ENOTDIR:
#endif
            mkpath( mapfn );
            break;
        }
    
    frip_rename( rip, mapfn );
    }


static bool host_ad( const addr &creator, const addr &ad )
    {
    debug("Testing for host ("+((string)creator)+") routing for net "+((string)ad));
    if( 
       creator.node() != 0 ||
       ad.node() != WILD ||
       creator.net() != ad.net() ||
       creator.zone() != ad.zone() ||
       creator.domain() != ad.domain() ||
       creator.point() != 0 ||
       (ad.point() != 0 && ad.point() != WILD)
      ) return No;
    
    debug("Host routing correct");
    return Yes;
    }


void FripApp::do_ad( const string &in, rip &r )
    {
    BoldMsg("Ad "+((string)r.ad())+" from "+((string)r.from())+" by "+((string)r.creator()) );

      // check for sane creation date!

    try
        {
        datetime cr_dt( r.created(), datetime::UTC );
        if( cr_dt > (datetime::now() + datetime(24l*60l*60l,datetime::UTC)) )
            {
            Warning("RIP from future ("+cr_dt.localtime_rip_string()+"). Ignored.");
            return;
            }
        }
    catch( Ex_Arg ex )
        {
        ex.print();
        Warning("RIP ignored due to the invalid date of creation.");
        return;
        }

    // input rips filter
    
    if( 
       //r.ad().is_wild() &&
       (r.creator() != r.ad()) && (!host_ad(r.creator(), r.ad())) &&
       (!ap.is_approved( r.creator(), r.ad() ))
      )
        {
        Warning("Non-approved ad, marking as -NA");
        r.fminus() += "NA";
        }
    
    bool oom = our_aka_matches(r.from());
    //__HD__("passed oom" );
    
    link  lin;
    
    if( (!oom) && (!links::is_from_link( r )) )
        {
        stats.rip_filtered();
        throw Ex_Arg("do_ad","rip is not from our link", string(r.from()));
        }
    
    if( (!oom) && (links::check_in( r, lin ) != Yes ) )
        {
          //debug("passed llist.check_in" );
        Msg( "rip filtered out by link filter" );
        stats.rip_filtered();
        return;
        }

    if( r.fminus().has("NA") && r.fminus().has("RES") )
        {
        Msg( "rescan from obsolete version of FRIP, ignored" );
        stats.rip_filtered();
	return;
        }
    
    if( r.fminus().has("RIF") )
        {
        Msg( "Ad from obsolete (RIF-less) version of FRIP, ignored" );
        stats.rip_filtered();
	return;
        }
    
    if( db.check(r) == No )
        {
        Msg( "rip gives us no good, ignored" );
        stats.rip_no_good();
	return;
        }

    did_something = Yes;

    if( conf.map_enabled() )
        try { map_rip( in, r.ad() ); } catch( General_Ex ex ) { ex.print(); }
    
    // exec "in time+=" statement here
    r.increment_time( lin.in().time_inc() );
    
    db.update( r );

    r.increment_hops();
    
    r.add_path(conf.aka[0]);
    for( int i = 0; i < conf.aka.count(); i++ )
        r.add_seenby ( conf.aka[i] );

      //send_rips( r );
    send_rip_to_all_links( r, Yes );
    }




void FripApp::do_off( rip &r )
    {
    BoldMsg("Got routing loss for "+((string)r.off())+" from "+((string)r.from()) );

    // dupecheck

    try
        {
        msgid offid = "off" + ((string)r.off()) + " " + datetime(r.created(),datetime::Local).gmtime_time_t_string();
        debug("id string for off = '" + offid + "'");
        if( id_base.check( offid ) )
            {
            if(!conf.silence()) Msg("Dupe OFF, killed.");
            return;
            }
        }
    catch( General_Ex ex )
        {
        ex.print();
        return;
        }
    
      // input rips filter
   
    bool oam = our_aka_matches(r.from());
    
    link  lin; // Это от него мы схлопотали off

    if( (!oam) && (links::check_in( r, lin ) != Yes) )
        {
        Msg( "rip is filtered out" );
        stats.rip_filtered();
        return;
        }
    
    // do something with that rip or return

    int i;
    
    switch( db.process_off( r, oam ) )
        {

// Off-ы глючат, если им позволить генерить ресканы. Рожают петли роутинга.
        case base::Off_Not_Last:          // There is still some good routing
#if 0
            did_something = Yes;

            if( oam )
                throw Ex_Fail("do_off","Something wrong, self-generated OFF didn't kill all","");

            try { addr via = db.find_route( r.off(), No /* not for rif */ ); }
            catch( General_Ex ex )
                {
                ex.print();
                throw Ex_Fail("do_off","Can't find route though db reported we have one.", string(r.off()));
                }
                {
                rip nr;
                nr.type( rip_type::AD );
                  // BUG! Тут бы лучше анонсировать не то, что терялось,
                  // а то, на что у нас в базе реально есть анонс. Иначе
                  // мы дробим агрегаты без причины. Впрочем, это не
                  // смертельно - в следующий реанонс этого агрегата
                  // оно само устаканится.
                nr.ad( r.off() );
                nr.from( conf.aka[0] );
                nr.to( r.from() );
                nr.hops(0);
                nr.created( time(0) );
                nr.creator( conf.aka[0] );
                nr.add_path(conf.aka[0]);
                nr.fminus() += "RES"; // Mark as rescanned
                nr.fminus() += "NA"; // Mark as non-approved
                for( i = 0; i < conf.aka.count(); i++ )
                    nr.add_seenby ( conf.aka[i] );

                  // BUG! Ему нужно нарастить время в пути сообразно тому, что
                  // у нас написано про этот роут в базе.

                  // Temp. workaround - сделаем вид, что он шел месяц.
                nr.increment_time( 60*60*24*30 );
                
                //do_a_link( nr, lin );
                send_rip_to_link( nr, lin, Yes );
                }
#endif
            // case base::Off_Not_Last ends here
            return;

        case base::Off_Last:
            did_something = Yes;

            if( is_our_aka( r.off() ) )
                {
                Warning( "Will not broadcast OFF for my aka: "+string(r.off()) );
                return;
                }
            
            r.increment_hops();
            r.add_path(conf.aka[0]);
            for( i = 0; i < conf.aka.count(); i++ )
                r.add_seenby ( conf.aka[i] );
            Msg("Broadcasting off for "+string(r.off()) );
              //send_rips( r );
            send_rip_to_all_links( r, Yes );
            return;
        }

    throw Ex_Fail("do_off","Can't get here!","");
    }


static void imstupid()
    {
    if(!conf.silence()) Msg("Successfully ignored due to stupidity of this FRIP version");
    }


static void do_hi( rip &r )
    {
    if(!conf.silence()) BoldMsg("Got HI from "+((string)r.from()) );
    imstupid();
    }


static void do_hack( rip &r )
    {
    if(!conf.silence()) BoldMsg("Got HI ACK from "+((string)r.from()) );
    imstupid();
    }


static void do_rrq( rip &r )
    {
    if(!conf.silence()) BoldMsg("Got RRQ from "+((string)r.from()) );
    imstupid();
    }


static void do_rack( rip &r )
    {
    if(!conf.silence()) BoldMsg("Got RRQ ACK from "+((string)r.from()) );
    imstupid();
    }


static void do_search( rip &r )
    {
    if(!conf.silence()) BoldMsg("Got SEARCH from "+((string)r.from()) );
    imstupid();
    }



void FripApp::do_one_rip( string in )
    {
    rip r;
    
    r.load( in );

    if( !our_aka_matches(r.to()) )
        throw Ex_Arg("do_one_rip","The rip is not for us", string(r.to()));
    
    switch( r.type() )
        {
        case rip_type::AD:     do_ad    ( in, r );  break;
        case rip_type::OFF:    do_off   ( r );      break;
        case rip_type::HI:     do_hi    ( r );      break;
        case rip_type::HACK:   do_hack  ( r );      break;
        case rip_type::RRQ:    do_rrq   ( r );      break;
        case rip_type::RACK:   do_rack  ( r );      break;
        case rip_type::SEARCH: do_search( r );      break;

        case rip_type::PING:
            BoldMsg("Got PING from "+((string)r.from()) );
            links::process_ping( r );
            break;
        
        default:
            throw Ex_Arg("do_one_rip","Don't know how to chew this type of rip", in );
        }
    }


//void eat_rifs_in( const string &dir );

void FripApp::delete_timed_out_routes()
    {
    bool put_header = No;
    int fatal_timeout = conf.db_fatal_timeout();

    vector <base_data> tout;

    db.get_timed_out_routes( tout );

    if( tout.size() <= 0 ) return;
    
    BoldMsg("Looking for timed-out routes to kill 'em" );
    for( int i = 0; i < tout.size(); i++ )
        {
        base_data &bb = tout[i];
        Msg("Removing old route to "+string(bb.route_for())+" via "+string(bb.route_via()) );
        frip_do_off( bb.route_for() );
        }
    }



void FripApp::main_loop( bool force_announce, bool force_ping )
    {
    if( force_announce || need_announce() )
        {
        try                     { announce(); }
        catch( General_Ex ex )  { ex.print(); }
        }
    
    if( Stop_Frip ) return;
        
    if( force_ping || need_ping() )
        {
        try
            {
            mark_ping();
            ping();
            }
        catch( General_Ex ex )  { ex.print(); }
        }
    if( Stop_Frip ) return;
    
    const string inb ( conf.inbound() );

    // first, eat all rips to make sure unzip will not overwrite 'em
    if( !inb.is_empty() )
        {
        eat_rips_in(inb);
        unzip_in(inb);
        eat_rips_in(inb);
        }
    
    if(!Stop_Frip)
        {
        eat_rips_in(conf.dir());
        unzip_in(conf.dir());
        eat_rips_in(conf.dir());
        }

    try { delete_timed_out_routes(); } catch( General_Ex ex ) { ex.print(); }

    db.save();
    create_processed_flag_file();

    if( did_something )
        create_routes();

    if( !inb.is_empty() )
        eat_rifs_in(inb);
    
    if(!Stop_Frip)
        eat_rifs_in(conf.dir());

    }



  // -----------------------------------------------------------------
  // RIFs
  // -----------------------------------------------------------------



void FripApp::receive_one_rif( const string &in, rif &r )
    {
    StatLog("────────────────────────────────────────────────────────────────────────────");
    StatLog("Unpacking RIF from "+string(r.from())+" to "+string(r.to()));

    stats.rif_for_us();
    
    string name = r.name();
    string newname;
    
      //if( r.type() == rif_type::F )
    newname = conf.inbound()+"\\"+name;
    if( r.type() == rif_type::Mail )
        {
        if( !conf.fripmail().is_empty() )
            newname = new_msg_name( conf.fripmail() );
        else
            {
            Warning("FripMail directory is undefined, receiving mail to "+conf.inbound());
            newname = tempFileName( conf.inbound()+"\\????????.msg" );
            }
        }

      // races here. It is possible for newname to appear later :(
    if( access(newname,0) == 0 )
        throw Ex_Fail( "do_one_rif","File "+newname+" exist, delaying RIF processing",in);
    
    unpack_rif(in);
    
    if( (r.type() == rif_type::Mail) )
        {
        try { msg_mark_received(in); }
        catch( General_Ex ex ) { ex.print(); }
        }

    vector <string> history;
    
    history.push_back("Received as "+newname);
    
    int pathlen = r.path().size();
    for( int i = 0; i < pathlen; i++ )
        {
        string e = string("Via ")+string(r.path()[i])+", relayed with FRIP";
        history.push_back(e);
        }
    
    try                    { set_ea_MVMT_ASCII( in, ".HISTORY", history ); }
    catch( General_Ex ex ) { ex.print(); }
    
    if( r.type() == rif_type::Mail)
        msg_add_via_lines( in, r.path() );
    
      // move in to our inbound?
    frip_rename( in, newname );
    
    if( r.desc().length() )
        {
        try                     { set_desc( newname, r.desc() ); }
        catch( General_Ex ex )  { ex.print(); }
        }
    
    Msg("Rif "+in+" is received as "+newname);

    if( r.rrq() )
        {
        try
            {
            Msg("Sending receipt message for "+newname);
            string id = r.msgid();
            if( id.is_empty() ) id = "is not known, sorry";
            string contents =
                "Dear Sysop!\r\n"
                "\r\n"
                "Your message to "+string(r.to())+"(id "+id+") was successfully received here just now.\r\n"
                "\r\n"
                "Yours truly Frip (" FRIP_VER_STR ")\r\n"
                ;
                msg_create_new( contents, r.from(), "Frip Router", "Sysop", "Receipt message", r.msgid() );
            }

        catch( General_Ex ex ) { ex.print(); }
        }

    }



  // BUG - todo
  // small rifs are better to be sent packed in riz files

void FripApp::do_one_rif( string in )
    {
    rif r;
    r.load( in );

    addr route_to = r.to();
    if( our_aka_matches(route_to) )
        {
        Msg("Got rif for us");
        receive_one_rif( in, r );
        return;
        }

    addr route_via;
    
    try
        {
          // exception may be thrown from find_route to indicate no-route situation
        route_via = db.find_route( route_to, Yes /* for rif */ );
        }
    catch( Ex_EOF e )
        {
        Msg(e.why+", unpacking it here");
        receive_one_rif( in, r );
        return;
        }

    if( our_aka_matches(route_via) )
        {
        Msg("Got rif for us");
        receive_one_rif( in, r );
        return;
        }

      // route it

    StatLog("────────────────────────────────────────────────────────────────────────────");
    StatLog("Routing RIF from "+string(r.from())+" to "+string(route_to)+" via "+string(route_via));
    
    Msg("Got rif for "+string(route_to));
    add_path_to_rif( in );
    
    string rdir = conf.dir()+"\\routed";
    mkdir((char *)(const char *)rdir);

    string outpath = newdir( in, rdir );
    try { frip_rename( in, outpath ); }
    catch( General_Ex )
        {
        // failed? Maybe name exist, try another name
        string newone = tempFileName(rdir+"/rif?????.rif");
        Warning("Can't move "+in+" to "+outpath);
        frip_rename( in, newone );
        Msg("Renamed "+in+" to "+newone);
        outpath = newone;
        }

    deliver( route_via, outpath );

    stats.rif_forw();
    }



void FripApp::eat_rifs_in( const string &dir )
    {
    for( filer finrip(dir,"rif*.ri?"); finrip.valid(); finrip.next() )
        {
        string fn = dir + "/" + finrip.current();
        debug( "Looking at: " + fn );
        
        try { do_one_rif( fn ); }
        catch( General_Ex ex )  { ex.print(); }
     
        if( Stop_Frip ) 
            {
            Error("Frip execution shut down by signal or ^C, unprocessed rifs left.");
            break;
            }
        }
    }





@


1.25
log
@see diff
@
text
@d8 3
d242 14
@


1.24
log
@RIP date sanity check
Timeout routes deletion
@
text
@d8 4
a181 13
    //debug("Host routing (domain not checked) correct");
    
    
#if 0
    if(
       creator.domain() != ad.domain()
      ) return No;
//#else
    domain_s d1, d2;
    d1 = creator.domain();
    d2 = ad.domain();
    if( d1 != d2 ) return No;
#endif
d236 1
a236 1
        Msg( "rip filtered out" );
d294 1
a294 3
    //if( (r.creator() != r.ad()) && (!host_ad(r.creator(), r.ad())) && (!ap.is_approved( r.creator(), r.ad() ))  )
    //    { Warning("Non-approved ad, marking as -NA"); r.fminus() += "NA"; }
    
a298 3
      // do we need it??
      // Предположим, был линк, да сплыл из списка - чего бы и не принять
      // от него, бывшего, потерю рутинга? Только AD-ов не слать ему, а так...
a300 1
          //debug("passed llist.check_in" );
a305 3
    //if( db.check(r) == No )
    //    { Msg( "rip gives us no good, ignored" ); stats.no_good(); return Ok; }

d455 1
a455 1
void eat_rifs_in( const string &dir );
a465 1

d675 1
d677 10
a686 2

    frip_rename( in, outpath );
@


1.23
log
@offs are not sending rescans back
@
text
@d8 2
a9 2
 *      Revision 1.22  1997/03/05 18:12:38  dz
 *      Ver. 34
a23 9
 *      Revision 1.17  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.16  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.15  1996/09/08 19:05:31  dz
 *      Version 25
 *
a35 3
 *      Revision 1.10  1996/07/22 02:48:05  dz
 *      ver 17
 *
a41 9
 *      Revision 1.7  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.6  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
d91 3
a93 9
        try
            {
            do_one_rip( fn );
            unlink( fn );
            }
        catch( General_Ex ex )
            {
            ex.print();
            }
a115 35
void FripApp::unzip_in( const string &inb )
    {
    const string our ( conf.dir() );
    
    for( filer finriz(inb,"*.riz"); finriz.valid(); finriz.next() )
        {
        string fn1=inb+"/"+finriz.current();

        // we need to unzip "rif*.*" too!!
        //string cmd = conf.unzipcmd() +" "+ fn1 + " *.rip";
        string cmd = conf.unzipcmd() +" "+ fn1 + " ri*.ri?";
        
#if defined( __DOS__ ) || defined( __NT__ )
        int l = cmd.length();
        for( int i = 0; i <  l; i++ )
            if( cmd[i] == '/' ) cmd[i] = '\\';
#endif
        debug("Executing "+cmd );
        if( system( cmd ) )
            Error("Error executing "+cmd );
        else
            {
            debug("Deleting "+fn1 );
            if( unlink( fn1 ) )
                Error("Error deleting "+fn1 );
            }
        eat_rips_in( "." );
        if( Stop_Frip ) break;
        }
    }





d200 19
a218 1
    
d475 22
d535 3
a537 1
        
a646 1

d714 1
a714 1
        
d722 1
@


1.22
log
@Ver. 34
@
text
@d8 3
d374 2
d377 1
d418 2
@


1.21
log
@Silence support
@
text
@d8 3
d91 1
a91 1
static void do_one_rip( string in );
d416 7
d676 13
a688 2
    // exception may be thrown from find_route to indicate no-route situation
    addr route_via = db.find_route( route_to, Yes /* for rif */ );
@


1.20
log
@OFF dupecheck
RRQ support
@
text
@d8 4
d330 1
a330 1
            Msg("Dupe OFF, killed.");
d429 1
a429 1
    Msg("Successfully ignored due to stupidity of this FRIP version");
d435 1
a435 1
    BoldMsg("Got HI from "+((string)r.from()) );
d442 1
a442 1
    BoldMsg("Got HI ACK from "+((string)r.from()) );
d449 1
a449 1
    BoldMsg("Got RRQ from "+((string)r.from()) );
d456 1
a456 1
    BoldMsg("Got RRQ ACK from "+((string)r.from()) );
d463 1
a463 1
    BoldMsg("Got SEARCH from "+((string)r.from()) );
@


1.19
log
@Correct order of attach/message send.
@
text
@d8 3
d318 18
d345 1
a345 1
      // Пердположим, был линк, да сплыл из списка - чего бы и не принять
d558 1
a558 1
void FripApp::relay_one_rif( const string &in, rif &r )
d580 2
a581 1
    
d589 1
a589 1
        try                    { msg_mark_received(in); }
d592 1
a592 1
    
d620 21
d658 1
a658 1
        relay_one_rif( in, r );
d668 1
a668 1
        relay_one_rif( in, r );
d672 1
a672 1
          // route it
@


1.18
log
@msg received time/date support
@
text
@d8 3
d539 3
d631 3
@


1.17
log
@Before changing
@
text
@d8 3
d561 1
a561 1
        try                    { msg_clean_bad_flags(in); }
@


1.16
log
@Ver. 27
@
text
@d8 3
d493 2
a494 1
        
d496 2
a498 2
        
    if( !inb.is_empty() )
d500 1
d504 1
d531 1
a531 1
static bool is_for_us( const rif &r )
d533 8
a540 2
      // BUG!! Mail to announced addresses will not be unpacked here :(
    if( our_aka_matches(r.to()) )
d542 44
a585 2
        Msg("Got rif for us");
        return Yes;
d587 2
a588 2
    Msg("Got rif for "+string(r.to()));
    return No;
d592 2
d602 2
a603 1
    if( is_for_us( r ) )
d605 4
a608 4
        stats.rif_for_us();
        
        string name = r.name();
        string newname;
d610 2
a611 51
        //if( r.type() == rif_type::F )
        newname = conf.inbound()+"\\"+name;
        if( r.type() == rif_type::Mail )
            {
            if( !conf.fripmail().is_empty() )
                newname = new_msg_name( conf.fripmail() );
            else
                {
                Warning("FripMail directory is undefined, receiving mail to "+conf.inbound());
                newname = tempFileName( conf.inbound()+"\\????????.msg" );
                }
            }

        if( access(newname,0) == 0 )
            throw Ex_Fail( "do_one_rif","File "+newname+" exist, delaying RIF processing",in);

        unpack_rif(in);

        if( (r.type() == rif_type::Mail) )
            {
            try                    { msg_clean_bad_flags(in); }
            catch( General_Ex ex ) { ex.print(); }
            }

        vector <string> history;
        
        history.push_back("Received as "+newname);

        int pathlen = r.path().size();
        for( int i = 0; i < pathlen; i++ )
            {
            string e = string("Via ")+string(r.path()[i])+", relayed with FRIP";
            history.push_back(e);
            }

        try                    { set_ea_MVMT_ASCII( in, ".HISTORY", history ); }
        catch( General_Ex ex ) { ex.print(); }

        if( r.type() == rif_type::Mail)
            msg_add_via_lines( in, r.path() );
        
          // move in to our inbound?
        frip_rename( in, newname );
        
        if( r.desc().length() )
            {
            try                     { set_desc( newname, r.desc() ); }
            catch( General_Ex ex )  { ex.print(); }
            }

        Msg("Rif "+in+" is received as "+newname);
d613 4
a618 1
      // route it
d620 1
a620 3
    addr route_to = r.to();
    addr route_via = db.find_route( route_to, Yes /* for rif */ );
        //stats.rif_held();
d622 1
a653 1
    
@


1.15
log
@Version 25
@
text
@d7 4
a10 1
 *      $Log: MainLoop.C $
d527 1
a576 6
        if( r.desc().length() )
            {
            try                     { set_desc( in, r.desc() ); }
            catch( General_Ex ex )  { ex.print(); }
            }

d596 7
@


1.14
log
@Exceptions added, not compiled
@
text
@d8 3
d58 2
a64 2
base db;
approve ap;
d69 1
a69 1
static void do_one_file( string in );
d81 1
a81 1
void eat_rips_in( const string &dir )
d89 1
a89 1
            do_one_file( fn );
d118 1
a118 1
void unzip_in( const string &inb )
d145 1
a145 1
        Do_Stop_Frip();
d185 1
a185 1
            catch
d234 1
a234 1
static void do_ad( const string &in, rip &r )
d255 1
a255 1
    if( !llist.is_from_link( r ) )
d261 1
a261 1
    if( (!oom) && (llist.check_in( r, lin ) != Yes ) )
d279 1
a279 1
        try { map_rip( in, r.ad() ) } catch( General_Ex ex ) { ex.error(); }
d292 2
a293 1
    send_rips( r );
d299 1
a299 1
void static do_off( rip &r )
d314 1
a314 1
    if( (!oam) && (llist.check_in( r, lin ) != Yes) )
d335 1
a335 1
                throw("do_off","Something wrong, self-generated OFF didn't kill all","");
d341 30
a370 1
                throw Ex_Fail("Can't find route though db reported we have one.", string(r.off()));
a371 18
            rip nr;
            nr.type( rip_type::AD );
              // BUG! Тут бы лучше анонсировать не то, что терялось,
              // а то, на что у нас в базе реально есть анонс. Иначе
              // мы дробим агрегаты без причины. Впрочем, это не
              // смертельно - в следующий реанонс этого агрегата
              // оно само устаканится.
            nr.ad( r.off() );
            nr.from( conf.aka[0] );
            nr.to( r.from() );
            nr.hops(0);
            nr.creator( conf.aka[0] );
            nr.add_path(conf.aka[0]);
            nr.fminus() += "RES"; // Mark as rescanned
            for( i = 0; i < conf.aka.count(); i++ )
                nr.add_seenby ( conf.aka[i] );

            do_a_link( nr, lin );
d381 2
a382 1
            send_rips( r );
d386 1
a386 1
    throw("do_off","Can't get here!","");
d424 7
d432 1
a432 1
static void do_one_rip( string in )
d443 7
a449 6
        case rip_type::AD:   do_ad   ( in, r );  break;
        case rip_type::OFF:  do_off  ( r );      break;
        case rip_type::HI:   do_hi   ( r );      break;
        case rip_type::HACK: do_hack ( r );      break;
        case rip_type::RRQ:  do_rrq  ( r );      break;
        case rip_type::RACK: do_rack ( r );      break;
d453 1
a453 1
            llist.process_ping( r );
d457 1
a457 1
            throw Ex_Arg("do_one_rip","Dunno how to chew this type of rip", in );
d465 1
a465 1
void main_loop()
d467 1
a467 1
    if( need_announce() )
d473 1
a473 1
    Do_Stop_Frip();
d475 1
a475 1
    if( need_ping() )
d484 1
a484 1
    Do_Stop_Frip();
d537 1
a537 1
static void do_one_rif( string in )
d563 1
a563 1
            throw( "do_one_rif","File "+newname+" exist, delaying RIF processing",in);
d567 5
a571 1
        if( (r.type() == rif_type::Mail) ) msg_clean_bad_flags(in);
d575 1
a575 1
            try                     { set_desc( in, r.desc() ) }
d623 1
a623 1
void eat_rifs_in( const string &dir )
@


1.13
log
@Before exceptions.
@
text
@d8 3
d63 4
a66 1
static bool do_one_file( string in );
a70 1
    //debug("create_processed_flag_file()");
a71 1
    //debug("create_processed_flag_file(): did smthing");
a72 1
    //debug("creating "+conf.processed_flag_file());
d78 1
a78 1
bool eat_rips_in( const string &dir )
d84 3
a86 1
        if( do_one_file( fn ) == Ok )
d88 5
a110 1
    return Ok;
d115 1
a115 1
bool unzip_in( const string &inb )
a121 1
        // string fn2=our+"/"+finriz.current();
a143 2
    
    return Ok;
d150 1
a150 1
static bool map_rip( const string &rip, const addr &dest )
d162 1
a162 1
    if( conf.debug() ) Msg("  Mapping to "+mapfn );
d170 1
a170 1
        return Ok;
d177 10
a186 4
            if( Ok == frip_copy( rip, mapfn ) )
                return Ok;
            mkpath( mapfn );
            return frip_copy( rip, mapfn );
a187 1
            
d196 1
a196 5
    if( rename( rip, mapfn ) == 0 )
        return Ok;
    
    Error("Can't rename "+rip+" to "+mapfn );
    return Err;
d231 1
a231 1
static bool do_ad( const string &in, rip &r )
a234 6
    if( !our_aka_matches(r.to()) )
        {
        Error("The rip is for "+((string)r.to())+", but we have no such aka");
        return Err;
        }
    
d239 1
a239 2
       (r.creator() != r.ad()) &&
       (!host_ad(r.creator(), r.ad())) &&
d252 1
a252 1
    if( llist.not_from_link( r ) != Ok )
a253 1
        Warning( "rip is not from our link, filtered" );
d255 1
a255 1
	return Err;
d258 1
a258 1
    if( (!oom) && llist.check_in( r, lin ) != Ok )
d263 1
a263 1
        return Ok;
d270 1
a270 1
	return Ok;
d276 1
a276 1
        map_rip( in, r.ad() );
a287 1
    //debug("passed ++hops" );
a289 3
    //debug("passed send_rips" );
    
    return Ok;
d295 1
a295 1
bool do_off( rip &r )
d298 1
a298 8
      //BoldMsg("Ad "+((string)r.ad())+" from "+((string)r.from())+" by "+((string)r.creator()) );
    
    if( !our_aka_matches(r.to()) )
        {
        Error("The rip is for "+((string)r.to())+", but we have no such aka");
        return Err;
        }
    
a299 1
    
a303 1
      //__HD__("passed oom" );
d310 1
a310 1
    if( (!oam) && llist.check_in( r, lin ) != Ok )
d313 1
a313 1
        Msg( "rip is not from our link, filtered out" ); 
d315 1
a315 1
        return Ok;
a326 3
        case base::Off_Err:               // Something is wrong.
            return Err;
            
d329 6
d336 2
a337 33
                addr via;
                if( Err == db.find_route( via, r.off(), No /* not for rif */ ) )
                    {
                    Error("Strange. Can't find route to "+string(r.off())+", though db reported we have one.");
                    return Err;
                    }
                rip nr;
                nr.type( rip_type::AD );
                  // BUG! Тут бы лучше анонсировать не то, что терялось,
                  // а то, на что у нас в базе реально есть анонс. Иначе
                  // мы дробим агрегаты без причины. Впрочем, это не
                  // смертельно - в следующий реанонс этого агрегата
                  // оно само устаканится.
                nr.ad( r.off() );
                nr.from( conf.aka[0] );
                nr.to( r.from() );
                nr.hops(0);
                nr.creator( conf.aka[0] );
                nr.add_path(conf.aka[0]);
                nr.fminus() += "RES"; // Mark as rescanned
                for( i = 0; i < conf.aka.count(); i++ )
                    nr.add_seenby ( conf.aka[i] );

                if( oam )
                    Error("Something wrong, self-generated OFF didn't kill all");
                else
                    {
                    if( Err == do_a_link( nr, lin ) )
                        {
                        Error("Can't send ad to "+string(r.from()) );
                        return Err;
                        }
                    }
d339 19
a357 1
            return Ok;
d365 3
a367 8
            if( send_rips( r ) == Err )
                {
                Error("Can't broadcast off for "+string(r.off()) );
                return Err;
                }
            return Ok;

            
d370 1
a370 1
    return Err;
d380 1
a380 1
static bool do_hi( rip &r )
a383 1
    return Ok;
d387 1
a387 1
static bool do_hack( rip &r )
a390 1
    return Ok;
d394 1
a394 1
static bool do_rrq( rip &r )
a397 1
    return Ok;
d401 1
a401 1
static bool do_rack( rip &r )
a404 1
    return Ok;
d409 1
a409 1
static bool do_one_file( string in )
a410 2
//debug("do_one_file()" );

d413 1
a413 6
    if( r.load( in ) == Err )
        {
        Error("Can't load RIP "+in);
        return Err;
        }
//debug("got rip" );
d415 3
d420 6
a425 6
        case rip_type::AD:   return do_ad   ( in, r );
        case rip_type::OFF:  return do_off  ( r );
        case rip_type::HI:   return do_hi   ( r );
        case rip_type::HACK: return do_hack ( r );
        case rip_type::RRQ:  return do_rrq  ( r );
        case rip_type::RACK: return do_rack ( r );
d429 2
a430 1
            return llist.process_ping( r );
d433 1
a433 2
            Error("Dunno how to chew this "+in);
            return Err;
a434 2
    
    return Ok;
d441 1
a441 1
bool main_loop()
d444 5
a448 1
        announce();
d453 6
a458 2
        ping();
        mark_ping();
d471 2
d474 1
a487 2

    return Ok;
d513 1
a513 1
static bool do_one_rif( string in )
a514 2
      //debug("do_one_file()" );
    
d516 1
a516 6
    
    if( r.load( in ) == Err )
        {
        Error("Can't load RIF "+in);
        return Err;
        }
d539 3
a541 9
            {
            Error("File "+newname+" exist, delaying processing of "+in);
            return Err;
            }
        if( unpack_rif(in) )
            {
            Error("Can't unpack "+in);
            return Err;
            }
d543 1
a543 2
        if( (r.type() == rif_type::Mail) && msg_clean_bad_flags(in) )
            Error("Can't clean attr. bits for "+in);
d545 1
a545 3
          // move in to our inbound?
        Msg("Rif "+in+" is received as "+newname);
        if( rename( in, newname ) )
d547 2
a548 2
            Error("Can't rename "+in+" to "+newname);
            return Err;
a550 3
        if( r.desc().length() && set_desc( newname, r.desc() ) )
            Error("Can't set description for "+newname+" to "+r.desc());

d552 2
d558 1
a558 1
            string e = string("Frip relayed via ")+string(r.path()[i]);
d561 3
a563 2
        
        set_ea_MVMT_ASCII( newname, ".HISTORY", history );
d566 1
a566 1
            msg_add_via_lines( newname, r.path() );
d568 5
a572 1
        return Ok;
d577 2
a578 1
    addr route_via = r.to();
d580 1
a580 13
    if( Ok != db.find_route( route_via, route_to, Yes /* for rif */ ) )
        {
        Error("No route for "+string(route_to)+", can't forward "+in);
        stats.rif_held();
        return Err;
        }

    if( add_path_to_rif( in ) == Err )
        {
        Error("Can't add path record to "+in);
        stats.rif_held();
        return Err;
        }
d586 1
a586 6
    if( rename( in, outpath ) )
        {
        Error("Can't rename "+in+" to "+outpath);
        stats.rif_held();
        return Err;
        }
d588 1
a588 6
    if( deliver( route_via, outpath ) != Ok )
        {
        Error("Can't attach RIF "+outpath);
        stats.rif_held();
        return Err;
        }
a590 1
    return Ok;
d601 3
a603 1
        do_one_rif( fn );
@


1.12
log
@Clean up of watcom classlib based code
@
text
@d8 3
d114 4
a117 2
        
        string cmd = conf.unzipcmd() +" "+ fn1 + " *.rip";
d252 7
d261 1
a261 1
        //debug("passed llist.check_in" );
d264 1
a264 1
	return Ok;
a440 7
static bool do_ping( rip &r )
    {
    BoldMsg("Got PING from "+((string)r.from()) );
    imstupid();
    return Ok;
    }

d463 4
a466 1
        case rip_type::PING: return do_ping ( r );
d469 1
a469 1
            Error("Please eat yourself this "+in);
d585 4
d611 2
a612 1
        msg_add_via_lines( newname, r.path() );
d665 1
a665 2
        if( do_one_rif( fn ) == Ok )
            unlink( fn );
@


1.11
log
@FripMail written
@
text
@d8 3
a50 1
//#include <fcntl.h>
@


1.10
log
@ver 17
@
text
@d8 3
d44 1
d49 1
d72 1
a72 1
    for( filer finrip(dir,"*.rip"); finrip.valid(); finrip.next() )
a134 29
#include <errno.h>

#ifdef __OS2__
#    define INCL_DOSFILEMGR
#    include <os2.h>
#endif

static bool copyfile( const string & from, const string & to )
    {
#ifdef __OS2__
    if( 0 == DosCopy( from, to, DCPY_EXISTING ))
        return Ok;
#endif
    return Err;
    }

static void mkpath( const string &path )
    {
    for( int i = 0; i < path.length(); i++ )
        {
        if( path[i] != '/' && path[i] != '\\' )
            continue;
        string dir = path.substr( 0, i );
        //debug("mkpath "+dir );
        
        mkdir( (char *)(const char *)dir );
        }
            
    }
d164 1
a164 1
            if( Ok == copyfile( rip, mapfn ) )
d167 1
a167 1
            return copyfile( rip, mapfn );
d241 1
a241 1
    __HD__("passed oom" );
d249 1
a249 1
        stats.filtered();
d256 1
a256 1
        stats.no_good();
d283 4
a286 1
static void imstupid()
d288 98
a385 1
    Msg("Successfully ignored due to stupidity of this FRIP version");
d388 2
a389 1
static bool do_off( rip &r )
d391 1
a391 3
    BoldMsg("Got routing loss for "+((string)r.off())+" from "+((string)r.from()) );
    imstupid();
    return Ok;
a497 1
        {
a498 3
        }

    ZipList_send();
d506 1
a506 3
    
    stats.report();
    
a529 9
static string newdir( const char *name, string dir )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)name, ndr, ndi, nfn, nex );

    return dir+"\\"+string(nfn)+string(nex);
    }


d547 2
d550 15
a564 1
        string newname = conf.inbound()+"\\"+name;
d583 1
a583 1
        if( set_desc( newname, r.desc() ) )
d586 13
d606 1
a606 1
    if( Ok != db.find_route( route_via, route_to ) )
d609 1
d616 1
d627 1
d634 1
d638 1
d646 1
a646 1
    for( filer finrip(dir,"*.rif"); finrip.valid(); finrip.next() )
a660 1

@


1.9
log
@state save
@
text
@d7 4
a10 1
 *      $Log: MainLoop.c $
d39 1
a359 2


d372 1
a372 1
    
d392 1
a396 6
    load_links();
    Do_Stop_Frip();
    
    db.load();
    Do_Stop_Frip();
    
d421 5
a425 1
    create_routes();
d429 7
d440 132
@


1.8
log
@C Set & STL support
@
text
@d8 3
d59 31
d117 1
a122 30
    

bool eat_rips_in( const string &dir )
    {
    for( filer finrip(dir,"*.rip"); finrip.valid(); finrip.next() )
        {
        string fn = finrip.current();
        debug( "Looking at: " + fn );
        if( do_one_file( fn ) == Ok )
            unlink( fn );
        
        if( Stop_Frip ) 
            {
            Error("Frip execution shut down by signal or ^C, unprocessed rips left.");
            break;
            }
        }
    
    const safe = 20;
    static int safety = safe;
    
    if( safety-- <= 0 )
        {
        safety = safe;
        debug("  Saving database for safety reasons" );
        db.save();
        }
    
    return Ok;
    }
d240 6
@


1.7
log
@Beta 15
@
text
@d8 3
d56 3
a58 1
bool main_loop()
d60 1
a60 4
    //debug("Loading links");
    load_links();
    
    Do_Stop_Frip();
d62 1
a62 20
    //debug("Loading database");
    db.load();
    
    Do_Stop_Frip();
    
    if( need_announce() )
        announce();
    
    Do_Stop_Frip();
        
    if( need_ping() )
        {
        ping();
        mark_ping();
        }
    
    //atexit( create_processed_flag_file );
    
    const string inb ( conf.inbound() );
    if( !inb.is_empty() )
d64 2
a65 1
        const string our ( conf.dir() );
d67 1
a67 10
        for( filer finrip(inb,"*.rip"); finrip.valid(); finrip.next() )
            {
            string fn1=inb+"/"+finrip.current();
            string fn2=our+"/"+finrip.current();
            
            debug("Moving "+fn1+" to "+fn2 );
            if( rename(fn1, fn2 ) )
                Error("Can't move "+fn1+" to "+fn2 );
            Do_Stop_Frip();
            }
a68 7
        for( filer finriz(inb,"*.riz"); finriz.valid(); finriz.next() )
            {
            string fn1=inb+"/"+finriz.current();
            // string fn2=our+"/"+finriz.current();
            
            string cmd = conf.unzipcmd() +" "+ fn1 + " *.rip";
            
d70 3
a72 3
            int l = cmd.length();
            for( int i = 0; i <  l; i++ )
                if( cmd[i] == '/' ) cmd[i] = '\\';
d74 8
a81 10
            debug("Executing "+cmd );
            if( system( cmd ) )
                Error("Error executing "+cmd );
            else
                {
                debug("Deleting "+fn1 );
                if( unlink( fn1 ) )
                    Error("Error deleting "+fn1 );
                }
            Do_Stop_Frip();
d83 1
d85 4
a88 2
        
    // unzip_all();
d90 3
a92 4
    const safe = 20;
    int safety = safe;
    
    for( filer f(conf.dir(),"*.rip"); f.valid(); f.next() )
d94 1
a94 1
        string fn=f.current();
a97 1
//debug("passed do_one_file" );
a103 8
        
        if( safety-- <= 0 )
            {
            safety = safe;
            debug("  Saving database for safety reasons" );
            db.save();
            }
        
d105 10
a114 8
        
    db.save();
    create_processed_flag_file();
    create_routes();

    ZipList_send();

    stats.report();
d119 2
d146 1
a146 1
        mkdir( dir );
d186 1
d188 1
d375 1
d381 40
@


1.6
log
@beta 14
@
text
@d8 3
d99 1
a99 1
            string cmd = conf.unzipcmd() + fn1 + " *.rip";
d287 3
a289 1
    if( (!oom) && llist.check_in( r ) != Ok )
a302 1
    //debug("passed db.check" );
d308 4
a311 2
    //debug("passed map" );

a312 1
    //debug("passed db.update" );
d315 1
d406 1
a406 1
            Error("Please it yourself this "+in);
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d31 1
d39 10
d72 3
a74 1

d145 1
d282 1
a282 1
__HD__("passed oom" );
d286 1
a286 1
//debug("passed llist.check_in" );
a290 1
//__HD__("passed filter" );
d298 4
a301 2
//debug("passed db.check" );
    
d304 1
a304 1
//debug("passed map" );
d307 1
a307 1
//debug("passed db.update" );
d313 1
a313 2
    
//debug("passed ++hops" );
d316 1
a316 1
//debug("passed send_rips" );
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d25 1
d31 1
a37 1

d40 1
d45 1
d48 11
d215 2
a216 1
static bool do_one_file( string in )
d218 28
a245 1
//debug("do_one_file()" );
d247 5
a251 1
    rip r;
d253 6
a258 1
    if( r.load( in ) == Err )
d260 2
a261 2
        Error("Can't load RIP "+in);
        return Err;
a262 7
//debug("got rip" );
    
    //'    Announce of 'ansi.green||ansi.intense||rip.ad||ansi.normal||ansi.green||' from 'rip.from
    //'    created 'rip.created
//    AnnounceMsg( rip );
    
    BoldMsg("Got announce of "+((string)r.ad())+" from "+((string)r.from()) );
a263 2
    // input rips filter

d274 1
a274 1
__HD__("passed filter" );
d292 4
d302 85
@


1.3
log
@before COW strings
@
text
@d1 11
d36 2
a37 1
        
d40 44
d85 3
d91 1
a91 1
        BoldMsg( "Looking at: " + fn );
d94 15
d111 1
a112 1
    db.save();
d121 30
d152 45
d200 1
d209 1
d218 3
d222 1
a222 1
    if( (!our_aka_matches(r.from())) && llist.check_in( r ) != Ok )
d224 1
d229 1
d237 5
d244 1
d247 1
d250 1
@


1.2
log
@beta 9
@
text
@d66 9
a74 2
/* here we can filter all the incoming rips - for now skip it */

@


1.1
log
@Initial revision
@
text
@d23 1
a23 1
    debug("Loading links");
d26 1
a26 1
    debug("Loading database");
a33 4
            {
//		call SysMkdir confdir'\old'
//		'@@move 'fj' 'confdir'\old >\dev\nul'
//                if RC <> 0 then call Error "Can't move "fj' to 'confdir'\old, deleting'
a34 1
            }
d36 1
a36 1

d38 1
d40 1
a40 1
    db.save();
d42 2
d70 2
a71 1
	Msg( "rip gives us no good, ignored" );
d80 1
a80 1

@

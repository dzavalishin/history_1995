/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main processing loop
 *
 *      $Log: mainloop.c $
 *      Revision 1.26  1997/03/26 10:59:01  dz
 *      -RES, -RIF filters
 *
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

      // input rips filter
   
    bool oam = our_aka_matches(r.from());
    
    link  lin; // â® ®â ­¥£® ¬ë áå«®¯®â «¨ off

    if( (!oam) && (links::check_in( r, lin ) != Yes) )
        {
        Msg( "rip is filtered out" );
        stats.rip_filtered();
        return;
        }

    msgid offid = "off" + ((string)r.off()) + " " + datetime(r.created(),datetime::Local).gmtime_time_t_string();
    debug("id string for off = '" + offid + "'");

      // dupecheck
    try
        {
        if( id_base.is_dupe( offid ) ) // will not add it to base!
            {
            if(!conf.silence()) Msg("Dupe OFF, will not pass.");
            return;
            }
        }
    catch( General_Ex ex )
        {
        ex.print();
        return;
        }

    // do something with that rip or return

    int i;
    
    switch( db.process_off( r, oam ) )
        {

// Off-ë £«îç â, ¥á«¨ ¨¬ ¯®§¢®«¨âì £¥­¥à¨âì à¥áª ­ë. ®¦ îâ ¯¥â«¨ à®ãâ¨­£ .
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
                  // BUG! ’ãâ ¡ë «ãçè¥  ­®­á¨à®¢ âì ­¥ â®, çâ® â¥àï«®áì,
                  //   â®, ­  çâ® ã ­ á ¢ ¡ §¥ à¥ «ì­® ¥áâì  ­®­á. ˆ­ ç¥
                  // ¬ë ¤à®¡¨¬  £à¥£ âë ¡¥§ ¯à¨ç¨­ë. ‚¯à®ç¥¬, íâ® ­¥
                  // á¬¥àâ¥«ì­® - ¢ á«¥¤ãîé¨© à¥ ­®­á íâ®£®  £à¥£ â 
                  // ®­® á ¬® ãáâ ª ­¨âáï.
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

                  // BUG! …¬ã ­ã¦­® ­ à áâ¨âì ¢à¥¬ï ¢ ¯ãâ¨ á®®¡à §­® â®¬ã, çâ®
                  // ã ­ á ­ ¯¨á ­® ¯à® íâ®â à®ãâ ¢ ¡ §¥.

                  // Temp. workaround - á¤¥« ¥¬ ¢¨¤, çâ® ®­ è¥« ¬¥áïæ.
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

              // dupecheck
            try
                {
                //msgid offid = "off" + ((string)r.off()) + " " + datetime(r.created(),datetime::Local).gmtime_time_t_string();
                //debug("id string for off = '" + offid + "'");
                //if( id_base.check( offid ) )
                    //{ if(!conf.silence()) Msg("Dupe OFF, will not pass."); return; }

                id_base.check( offid ); // just add it to dupebase, ignoring res
                }
            catch( General_Ex ex )
                {
                ex.print();
                //return;
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
        throw Ex_Arg("do_one_rip","Rip from " + string(r.from()) + " is not for us", "To " + string(r.to()));
    
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
    //bool put_header = No;
    //int fatal_timeout = conf.db_fatal_timeout();

    vector <base_data> tout;

    db.get_timed_out_routes( tout );

    if( tout.size() <= 0 ) return;
    
    BoldMsg("Looking for timed-out routes to kill 'em" );
    for( int i = 0; i < tout.size(); i++ )
        {
        base_data &bb = tout[i];
        //Msg("Removing old route to "+string(bb.route_for())+" via "+string(bb.route_via()) );
        //frip_do_off( bb.route_for(), "Route timed out automatically. Was created "+bb.created().gmtime_rip_string() );
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
    StatLog("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
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

    StatLog("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
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






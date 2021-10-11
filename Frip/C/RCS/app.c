head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	97.03.28.22.01.47;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	97.03.26.10.57.20;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	97.03.22.15.47.42;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@report on stats now made here, not in d'tor
because log file is closed in static d'tor as well
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	FripApp class
 *
 *      $Log: app.c $
 *      Revision 1.6  1997/03/26 10:57:20  dz
 *      showbase
 *
 *      Revision 1.5  1997/03/22 15:47:42  dz
 *      datetime
 *
 *      Revision 1.4  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.3  1997/01/21 08:53:51  dz
 *      New flag type for fara, message id suppoer.
 *
 *      Revision 1.2  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.1  1996/09/08 19:05:31  dz
 *      Initial revision
 *
 *      
 *      
\*/

#include "frip.h"
#include "flag.h"
#include "lock.h"
#include "path.h"
#include "stat.h"



void FripApp::fara_send( string a_name, addr dest, bool kill, rif_type t, flags the_flags, string id )
    {
    addr route_via = dest;
      // BUG! In fact, class 'path' must be used here,
      // and its implementation should be OS-dependent.
    string name = make_relative_to_start_dir( a_name );
    debug("make_relative_to_start_dir( "+a_name+" ) == "+name);
    
    route_via = db.find_route( dest, Yes /* for rif */ );
    
    string rdir = conf.dir()+"\\routed";
    mkdir((char *)(const char *)rdir);
    
    rif r;
    
    r.type( t );
    r.from( conf.aka[0] );
    r.to( dest );
    r.set_flags( the_flags );
    r.msgid( id );
    
    try
        {
        r.desc( get_desc( name ) );
        }
    catch(...) {}
    
    string newname;
    
    r.create_in_dir( rdir, newname, name, kill );
    
      // We can't turn in on because frip 20 still doesn't decompress
      // *.rif or rif* from riz files
#if 0
    if( t == rif_type::Mail && file_size < conf.zipmail_v )
        ZipList_add( newname, route_via );
    else
#endif
        deliver( route_via, newname );
    
    }


void FripApp::frip_do_off( addr off )
    {
    BoldMsg("Processing route loss for "+string(off));
    
    rip r;
    
    r.created(datetime::now());
    r.hops(0);
    r.type( rip_type::OFF );
    r.creator( conf.aka[0] );
    r.from( conf.aka[0] );
    r.to( conf.aka[0] );
    r.off( off );
    //r.add_path(conf.aka[0]);
    //for( int i = 0; i < conf.aka.count(); i++ )        r.add_seenby ( conf.aka[i] );
    
    do_off( r );
    }


void FripApp::do_help()
    {
    BoldMsg("FRIP command syntax is: frip {command flags args}...");
    Msg("");
    Msg("Commands:");
    Msg("");
    Msg("    send [-kill] <filename> <addr> - sends file via FRIP links");
    Msg("                                     -kill can be abbreviated to -k");
    Msg("");
    Msg("    off <addr>                     - Report/process routing loss");
    Msg("");
    Msg("    scan                           - process incoming pips/rifs (DEFAULT)");
    Msg("");
    Msg("    announce                       - reannounce self");
    Msg("                                     Not too frequent, please!");
    Msg("                                     Frip does it automatically too.");
    Msg("");
    Msg("    ping                           - ping 'em now");
    Msg("");
    Msg("    showbase                       - print routing database");
    Msg("                                     in a user-viewable way");
    Msg("");
    Msg("    help                           - print this");
    }







void FripApp::frip( int ac, char **av )
    {
    ilock frip_lock("TheFrip.!!!");
    
    BoldMsg( "Frip - FTN Automatic Routing System, " FRIP_VER_STR );
    if(Stop_Frip) return;
    
    while(!frip_lock.we_own())
        {
        BoldMsg("Other FRIP works on this machine, sleeping...");
        frip_lock.wait(30);
        frip_lock.lock();
        if(Stop_Frip) return;
        }

    // Оно само должно по необходимости загрузиться, а не грузится. Зря.
    links::load(); 
    
    if(Stop_Frip) return;
    try { db.enable(); db.load(); } catch(General_Ex ex) { ex.print(); throw; }
    if(Stop_Frip) return;
    
    bool do_main_loop = Yes;
    bool do_main_loop_anyway = No;

    bool force_announce = No;
    bool force_ping = No;
    
    ac--; // skip argv[0]
    av++;
    
    while( ac-- )
        {
        string arg = *av++;
        arg.lcase();
        
        if( arg == "send" )
            {
            do_main_loop = No;
            string flags_s;
            
            if( ac > 0 && av[0][0] == '-' )
                {
                flags_s = (av[0]) + 1;
                flags_s.lcase();
                ac--; av++;
                }
            
            if( ac < 2 )
                {
                Error("No filename or address for SEND command given");
                add_exit_code(Exit_Cmdline_Error);
                break;
                }
            else
                {
                bool kill = (flags_s == "kill"||flags_s=="k") ? Yes : No;
                
                string arg1 = *av++; ac--;
                string arg2 = *av++; ac--;
                
                  // BUG: Here we should convert name to a fully qualified using
                  //      path to a directory frip was in when it was started.

                flags empty_f;
                try { fara_send( arg1, arg2, kill, rif_type::F, empty_f ); }
                catch( General_Ex ex )
                    {
                    Error("Send failed");
                    ex.print();
                    add_exit_code(Exit_Cmd_Error);
                    }
                }
            continue;
            }
        
        if( arg == "off" || arg == "lost" )
            {
            do_main_loop = No;
            string flags;
            
            if( ac > 0 && av[0][0] == '-' )
                {
                flags = (av[0]) + 1;
                flags.lcase();
                ac--; av++;
                }
            
            if( ac < 1 )
                {
                Error("No address for OFF command given");
                add_exit_code(Exit_Cmdline_Error);
                break;
                }
            else
                {
                string arg1 = *av++; ac--;
                
                
                try { frip_do_off( arg1 ); }
                catch( General_Ex ex )
                    {
                    Error("Off failed");
                    ex.print();
                    add_exit_code(Exit_Cmd_Error);
                    }
                }
            continue;
            }
        
        if( arg == "help" || arg == "-?" || arg == "-h" )
            {
            do_main_loop = No;
            do_help();
            continue;
            }
        
        if( arg == "showbase" )
            {
            do_main_loop = No;
            db.showbase();
            continue;
            }
        
        if( arg == "toss" || arg == "scan" || arg == "in" )
            {
            do_main_loop_anyway = Yes;
            continue;
            }
        
        if( arg == "announce" || arg == "ann" )
            {
            do_main_loop_anyway = Yes;
            force_announce = Yes;
            continue;
            }
        
        if( arg == "ping" )
            {
            do_main_loop_anyway = Yes;
            force_ping = Yes;
            continue;
            }
        
        }
    
    if( do_main_loop || do_main_loop_anyway )
        {
        try { main_loop( force_announce, force_ping ); }
        catch( General_Ex ex )
            {
            Error("Main processor failed");
            ex.print();
            add_exit_code(Exit_Fail);
            }

        try
            {
            if(!conf.fripmail().is_empty())
                eat_mail_in( conf.fripmail() );
            if(!conf.fripmail_pack().is_empty())
                eat_mail_in( conf.fripmail_pack() );
            }
        catch( General_Ex ex )
            {
            Error("Mail packer failed");
            ex.print();
            add_exit_code(Exit_Packer_Error);
            }

        }

    db.save();
    db.disable();

    stats.report();
    }




@


1.6
log
@showbase
@
text
@d8 3
d34 1
a34 1

d303 1
a303 1
    
d306 2
@


1.5
log
@datetime
@
text
@d8 3
d117 3
a128 1

d243 7
@


1.4
log
@Ver. 34
@
text
@d8 3
d75 1
a75 1
void FripApp::frip_do_off( string dest )
a76 2
    addr off = dest;
    
d81 1
a81 1
    r.created(time(0));
d88 2
a89 3
    r.add_path(conf.aka[0]);
    for( int i = 0; i < conf.aka.count(); i++ )
        r.add_seenby ( conf.aka[i] );
@


1.3
log
@New flag type for fara, message id suppoer.
@
text
@d8 3
d143 1
a143 1
    try { db.load(); } catch(General_Ex ex) { ex.print(); }
d289 2
@


1.2
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d19 1
d26 1
a26 1
void FripApp::fara_send( string a_name, addr dest, bool kill, rif_type t, string flags )
a31 1

d44 2
a45 1
    r.flags( flags );
d160 1
a160 1
            string flags;
d164 2
a165 2
                flags = (av[0]) + 1;
                flags.lcase();
d177 1
a177 1
                bool kill = (flags == "kill"||flags=="k") ? Yes : No;
d184 3
a186 2
                
                try { fara_send( arg1, arg2, kill, rif_type::F, "" ); }
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: frip.C $
d17 1
a18 34
  // BUG! In fact, class 'path' must be used here,
  // and its implementation should be OS-dependent.

string make_relative_to_start_dir( string in )
    {
    in.strip_ws();

#ifndef unix
    if( in[0] == '/' ) in[0] = '\\';
    if( in[1] == '/' ) in[1] = '\\';
    if( in[2] == '/' ) in[2] = '\\';
    
    if(
       (in[1] == ':'  && in[2] == '\\') ||
       (in[0] == '\\' && in[1] == '\\')
      )
        return in; // Pathname is absolute

    // No drive letter ?
    if( in[0] == '\\' )
        return started_in_dir.substr( 0, 3 ) + in;
    
    return started_in_dir + "\\" + in;
       
#else
#error Unix version is not written yet

    if( in[0] == '/' )
        return in; // Pathname is absolute

    return started_in_dir + "/" + in;
       
#endif
    }
d25 2
d264 9
a272 2
        
        if( !conf.fripmail().is_empty())
d274 3
a276 7
            try { eat_mail_in( conf.fripmail() ); }
            catch( General_Ex ex )
                {
                Error("Mail packer failed");
                ex.print();
                add_exit_code(Exit_Packer_Error);
                }
d278 1
@

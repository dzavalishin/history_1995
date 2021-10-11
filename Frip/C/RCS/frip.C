head	1.23;
access;
symbols
	Beta_12:1.7;
locks;
comment	@ *      @;


1.23
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.22;

1.22
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	1.21;

1.21
date	96.12.28.13.42.00;	author dz;	state Exp;
branches;
next	1.20;

1.20
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.19;

1.19
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.01.22.19.23.05;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.01.18.00.56.53;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.01.14.01.06.31;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.07.13.38.33;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.04.00.00.16;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.23
log
@Make sure we have TZ var
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main
 *
 *      $Log: frip.C $
 *      Revision 1.22  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.21  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.20  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.19  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.18  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.17  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.16  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.15  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.14  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.13  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.12  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.11  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.10  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.9  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.8  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.7  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

//#include "version.h"
#include "frip.h"
#include "app.h"
//#include "lock.h"
//#include "rif.h"

#include <signal.h>
#include <malloc.h>
#include <ctype.h>

#ifdef __WATCOM_CPLUSPLUS__
#include <dos.h>
#endif


bool volatile Stop_Frip = No;

static int frip_exit_code = 0;
void add_exit_code( exit_code e ) { frip_exit_code |= e; }


static void sig_handler( int sig );

#ifdef __IBMCPP__
#  define SIG_FUNC_T (_SigFunc)
#else
#  define SIG_FUNC_T
#endif

static void sig_setter()
    {
    signal( SIGINT,   SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/C on keyboard) is signalled
    signal( SIGBREAK, SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/BREAK on keyboard) is signalled
    signal( SIGTERM,  SIG_FUNC_T sig_handler );   //     a termination request is sent to the program
#if 0
    signal( SIGUSR1,  SIG_IGN );                  //     OS/2 process flag A via DosFlagProcess
#endif
    signal( SIGUSR2,  SIG_IGN );                  //     OS/2 process flag B via DosFlagProcess
    signal( SIGUSR3,  SIG_IGN );                  //     OS/2 process flag C via DosFlagProcess
    }

static void sig_handler( int  )
    {
    //sleep(1);
    sig_setter();
    Stop_Frip = Yes;
    }



// NB!
string started_in_dir =
#ifdef unix
    "/"
#else
    "?:\\"
#endif
    ;

void go_dir( string dir )
    {
    char *dp = (char *)(const char *)dir;
    if( dp[1] == ':' )
        {
#ifdef __WATCOM_CPLUSPLUS__
        unsigned drives = 0, drive = tolower(dp[0]) - 'a' + 1;
        _dos_setdrive( drive, &drives );
#else
        _chdrive( tolower(dp[0]) - 'a' + 1);
#endif
        }
    chdir( dp );
    }

int main( int ac, char **av )
    {
    if( 0 == getenv("TZ") )
        {
        Error("TZ variable is not set. I need to know your timezone to work correctly. Sorry :-(");
        return 33;
        }
        
    sig_setter();
    tzset();
    
    try { parse_config(); }
    catch( General_Ex ex )
        {
        cerr
            << "Setup loader failed\nException in " << ex.where.c_str()
            << ": " << ex.what.c_str()
            << " (" << ex.why.c_str() << ")\n"
            << "Can't configure self, stopped.\n";
        add_exit_code(Exit_Exception);
        return frip_exit_code;
        }

        {
        char a[200];
        getcwd( a, 200 );
        started_in_dir = a;
        }

    go_dir(conf.dir());


    try
        {
        FripApp app;
        app.frip( ac, av );
        } // app destructs here
    catch( General_Ex ex )
        {
        ex.print();
        cerr << "Unhandled exception, FRIP stopped.\n";
        add_exit_code(Exit_Exception);
        }

    /*
    try { ZipList_send(); }
    catch( General_Ex ex )
        {
        ex.print();
        add_exit_code(Exit_Exception);
        }
        */
    
#ifdef __DOS__
    go_dir(started_in_dir);
#endif

    return frip_exit_code;
    }


@


1.22
log
@Before changing
@
text
@d8 3
d136 6
@


1.21
log
@Ver. 27
@
text
@d8 3
d67 3
d90 1
a91 1
    signal( SIGTERM,  SIG_FUNC_T sig_handler );   //     a termination request is sent to the program
a118 1
#ifndef __WATCOM_CPLUSPLUS__
d120 5
a125 1
    // BUG! There should be some way under watcom to change current drive too
d127 1
d134 1
a134 5

// todo:
// после парсинга конфига и до запуска фрипа нужно разобрать командную строку,
// чтобы превратить имена файлов в полные и, соответственно, не бояться
// перехода в другой каталог
@


1.20
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d56 1
a56 1
//#include "ansi.h"
d151 1
d154 1
a154 8
    
        try { app.frip( ac, av ); }
        catch( General_Ex ex )
            {
            ex.print();
            cerr << "Unhandled exception, FRIP stopped.\n";
            add_exit_code(Exit_Exception);
            }
d156 6
a161 1

@


1.19
log
@Version 25
@
text
@d8 3
a174 7
    }



void General_Ex::print() const
    {
    Error( where+": "+what+" ("+why+")" );
@


1.18
log
@Exceptions added, not compiled
@
text
@d8 3
d48 1
a48 1
#include "version.h"
d50 4
d56 1
a56 3
#include "ansi.h"
#include "lock.h"
#include "rif.h"
d76 8
a83 12
    //     an interactive attention (CTRL/C on keyboard) is signalled
    signal( SIGINT,   SIG_FUNC_T sig_handler );
    //     an interactive attention (CTRL/BREAK on keyboard) is signalled
    signal( SIGBREAK, SIG_FUNC_T sig_handler );
    //     a termination request is sent to the program
    signal( SIGTERM,  SIG_FUNC_T sig_handler );
    //     OS/2 process flag A via DosFlagProcess
    signal( SIGUSR1,  SIG_IGN );
    //     OS/2 process flag B via DosFlagProcess
    signal( SIGUSR2,  SIG_IGN );
    //     OS/2 process flag C via DosFlagProcess
    signal( SIGUSR3,  SIG_IGN );
d88 1
d95 6
a100 36
int frip_do_send( string name, addr dest, bool kill, rif_type t, string flags )
    {
    addr route_via = dest;
    
    route_via = db.find_route( dest, Yes /* for rif */ );
    
    string rdir = conf.dir()+"\\routed";
    mkdir((char *)(const char *)rdir);

    rif r;

    r.type( t );
    r.from( conf.aka[0] );
    r.to( dest );
    r.flags( flags );

    string desc;
    if( Err != get_desc( name, desc ) )
        r.desc(desc);
    //debug("desc is "+desc);
    
    string newname;
    
    bool ret = r.create_in_dir( rdir, newname, name, kill );
    if( ret == Err )
        Error("Can't send "+name+" - I/O error");

      // We can't turn in on because frip 20 still doesn't decompress
    // *.rif or rif* from riz files
#if 0
    if( t == rif_type::Mail && file_size < conf.zipmail_v )
        {
        if( Err == ZipList_add( newname, route_via ) )
            ret = Err;
        }
    else
d102 1
a102 202
        {
        if( Err == deliver( route_via, newname ) )
            ret = Err;
        }
    
    return Err == ret ? 1 : 0;
    }


int frip_do_off( string dest )
    {
    addr off = dest;

    BoldMsg("Processing route loss for "+string(off));
    
    rip r;

    r.created(time(0));
    r.hops(0);
    r.type( rip_type::OFF );
    r.creator( conf.aka[0] );
    r.from( conf.aka[0] );
    r.to( conf.aka[0] );
    r.off( off );
    r.add_path(conf.aka[0]);
    for( int i = 0; i < conf.aka.count(); i++ )
        r.add_seenby ( conf.aka[i] );
    
    bool ret = do_off( r );
    if( ret == Err )
        Error("Can't process routing loss");
    
    return Err == ret ? 1 : 0;
    }


void do_help()
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
    Msg("    help                           - print this");
    }








void frip( int ac, char **av )
    {
    ilock frip_lock("TheFrip.!!!");
    
    BoldMsg( "Frip - FTN Networks Automatic Routing System, " FRIP_VER_STR );
    if(Stop_Frip) return;

    while(!frip_lock.we_own())
        {
        BoldMsg("Other FRIP works on this machine, sleeping...");
        frip_lock.wait(30);
        frip_lock.lock();
        if(Stop_Frip) return;
        }

    load_links();
    if(Stop_Frip) return;
    try { db.load(); } catch {}
    if(Stop_Frip) return;

    bool do_main_loop = Yes;
    bool do_main_loop_anyway = No;

    ac--; // skip argv[0]
    av++;
    
    while( ac-- )
        {
        string arg = *av++;
        arg.lcase();

        if( arg == "send" )
            {
            do_main_loop = No;
            string flags;

            if( ac > 0 && av[0][0] == '-' )
                {
                flags = (av[0]) + 1;
                flags.lcase();
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
                bool kill = (flags == "kill"||flags=="k") ? Yes : No;
                
                string arg1 = *av++; ac--;
                string arg2 = *av++; ac--;

                // BUG: Here we should convert name to a fully qualified using
                //      path to a directory frip was in when it was started.
                
                try { frip_do_send( arg1, arg2, kill, rif_type::F, "" ); }
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
        
        if( arg == "toss" || arg == "scan" || arg == "in" )
            {
            do_main_loop_anyway = Yes;
            continue;
            }
        
        }

    if( do_main_loop || do_main_loop_anyway )
        {
        try { main_loop(); }
        catch( General_Ex ex )
            {
            Error("Main processor failed");
            ex.print();
            add_exit_code(Exit_Fail);
            }

        if( !conf.fripmail().is_empty())
            {
            try { eat_mail_in( conf.fripmail() ); }
                {
                Error("Mail packer failed");
                ex.print();
                add_exit_code(Exit_Packer_Error);
                }
            }
        }
    
    }

string started_in_dir = ".";
d107 1
d110 2
d118 5
d128 1
a128 1
            << "Setup reader failed\nException in " << ex.where.c_str()
d144 16
a159 1
    try { frip( ac, av ); }
a162 1
        cerr << "Unhandled exception, FRIP stopped.\n";
d165 2
a166 1

d174 6
@


1.17
log
@Before exceptions.
@
text
@d8 3
d57 2
a58 7
void Do_Stop_Frip()
    {
    if( !Stop_Frip ) return;
    Error("Frip execution shut down by signal or ^C, bye.");
    //frip_lock.release();
    exit(0);
    }
d97 1
a97 5
    if( Ok != db.find_route( route_via, dest, Yes /* for rif */ ) )
        {
        Error("No route for "+string(dest)+", can't send "+name);
        return Err;
        }
a186 7
int
main( int ac, char **av )
    {
    sig_setter();
    
    if( parse_config() == Err )
        exit(2);
a187 1
    chdir( (char *)(const char *)conf.dir() );
d189 2
d194 1
a194 1
    Do_Stop_Frip();
d201 1
a201 1
        Do_Stop_Frip();
d205 3
a207 4
    Do_Stop_Frip();
    
    db.load();
    Do_Stop_Frip();
a210 1
    int ret_val = 0;
d235 1
d248 7
a254 2
                int ret = frip_do_send( arg1, arg2, kill, rif_type::F, "" );
                if( ret > ret_val ) ret_val = ret;
d274 1
d281 8
a288 2
                int ret = frip_do_off( arg1 );
                if( ret > ret_val ) ret_val = ret;
d309 8
a316 1
        ret_val |= main_loop() == Ok ? 0 : 4;
d318 10
a327 2
    if( (!conf.fripmail().is_empty()) && Err == eat_mail_in( conf.fripmail() ) )
        ret_val |= 8;
a328 4
    ZipList_send();
    stats.report();

    return ret_val;
d331 1
d333 1
a333 5




void Log( const char *p, const string &s )
d335 4
a338 2
    if( !conf.log() ) return;
    conf.log_stream() << p << ((const char *)s) << "\n";
d341 1
a341 2

void statistics::report()
d343 13
a355 11
    char st[280];
    sprintf( st, 
            "RIPs: %3d ignored, %3d new, %3d better, %3d timed out, %3d filtered",
            no_good_v, new_addr_v, better_repl_v, timeout_repl_v, filtered_v );
    BoldMsg( string(st) );

    sprintf( st, 
            "RIFs: %3d total, %3d for us, %3d forwarded, %3d held",
            rifs_total_v, rifs_for_us_v, rifs_forw_v, rifs_held_v );
    BoldMsg( string(st) );
    }
d357 5
a361 1
statistics stats;
d363 1
d365 7
d373 3
a375 11
void Error(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_red ansi_intense "Error:" ansi_normal
                " %s\n", (const char *)s );
    else
        fprintf( stderr, "Error: %s\n", (const char *)s );
        
    Log( "Error: ", s );
    }
d377 1
a377 22
void Warning(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_magenta ansi_intense "Warning:" ansi_normal
                " %s\n", (const char *)s );
    else
        fprintf( stderr, "Warning: %s\n", (const char *)s );
        
    Log( "Warning: ", s );
    }


void Msg(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_green ansi_intense "*** " ansi_normal
                "%s\n", (const char *)s );
    else
        fprintf( stderr, "*** %s\n", (const char *)s );
    Log( "*** ", s );
a379 11
void BoldMsg(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_brown ansi_intense "*!* " ansi_normal
                "%s\n", (const char *)s );
    else
        fprintf( stderr, "*!* %s\n", (const char *)s );
        
    Log( "*!* ", s );
    }
@


1.16
log
@Clean up of watcom classlib based code
@
text
@d8 3
d95 1
a95 1
int frip_do_send( string name, addr dest, bool kill, rif_type t )
d113 1
a113 1
    r.type( rif_type::F );
d118 1
a118 1
    debug("desc is "+desc);
d126 14
a139 2
    if( Err == deliver( route_via, newname ) )
        ret = Err;
d257 3
d261 1
a261 1
                int ret = frip_do_send( arg1, arg2, kill );
@


1.15
log
@FripMail written
@
text
@d8 3
a189 2
//#define VERSION_DEF_STRING(i) #i
    
a192 3
//#if defined(__DOS__)
    //Warning("Interlocking disabled in this version, sorry");
//#else
a199 1
//#endif
a267 2
                //bool kill = (flags == "kill"||flags=="k") ? Yes : No;
                
@


1.14
log
@ver 17
@
text
@d8 3
d36 1
a43 3
#ifndef VERDEF
#  define VERDEF "Unknown version"
#endif
d89 1
a89 1
int do_send( string name, string dest, bool kill )
d93 1
a93 1
    if( Ok != db.find_route( route_via, dest ) )
d104 1
d112 1
a112 1
    Msg("desc is "+desc);
d127 25
d163 2
d187 1
a187 1
#define VERSION_DEF_STRING(i) #i
d189 1
a189 1
    BoldMsg( "Frip - FTN Networks Automatic Routing Builder: " VERSION_DEF_STRING(VERDEF) );
d192 3
a194 3
#if defined(__DOS__)
    Warning("Interlocking disabled in this version, sorry");
#else
d202 1
a202 1
#endif
d246 1
a246 1
                int ret = do_send( arg1, arg2, kill );
d252 29
d297 7
a303 1
        ret_val = main_loop() == Ok ? 0 : 1;
d324 1
a324 1
            "Total %d rips ignored, %d new, %d better, %d timed out, %d filtered",
d326 5
@


1.13
log
@state save
@
text
@d7 4
a10 1
 *      $Log: frip.c $
d38 1
d86 60
a148 2
    ac = ac; av = av;
    
d163 1
a163 1
#if defined(__DOS__) || defined(__IBMCPP__)
d175 65
a239 1
    int val = main_loop() == Ok ? 0 : 1;
d241 1
a241 1
    return val;
@


1.12
log
@C Set, mostly works
@
text
@d8 3
d56 2
@


1.11
log
@C Set & STL support
@
text
@d8 3
d96 2
a97 2
#ifdef __DOS__
    Warning("Interlocking disabled in DOS variant of this version, sorry");
@


1.10
log
@Beta 15
@
text
@d8 3
d48 3
d55 1
a55 1
    signal( SIGINT, sig_handler );
d57 1
a57 1
    signal( SIGBREAK, sig_handler );
d59 1
a59 1
    signal( SIGTERM, sig_handler );
d61 1
a61 1
    signal( SIGUSR1, SIG_IGN ); 
d63 1
a63 1
    signal( SIGUSR2, SIG_IGN ); 
d65 1
a65 1
    signal( SIGUSR3, SIG_IGN );
d84 1
a84 1
    chdir( conf.dir() );
d87 4
a90 2

    BoldMsg( "Frip - FTN Networks Automatic Routing Builder: " VERDEF );
@


1.9
log
@beta 14
@
text
@d8 3
d75 2
a76 1
    parse_config();
@


1.8
log
@*** empty log message ***
@
text
@d8 3
a28 1

d35 1
a67 2
    ilock frip_lock("TheFrip.!!!");
    
d76 2
d81 3
d91 1
@


1.7
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d19 1
d65 1
a70 37
    //printf("brk = %dk\n", ((int)sbrk(0))/1024 );
    
    
#if TESTALLOC && 0
printf("test\n" );
heap_dump(); 
printf("string ops\n" );

heap_dump(); 
string *s = new string;
string *sa = new string( *s );
string *sb = new string;

printf("new done\n" );heap_dump();
*s = " Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world--------------------------------------------------------------------------------------------------------Hello, world-------------------------------------------------------------------------------------------------------- \r\n";
printf("*s = \"Hello, world....\n" );heap_dump();

parse( *s, *sa, ',', *sb );
printf("s.parse( sa, ',', sb );\n" );heap_dump();

//*sa = (*s).substr( 1, 10 );
//printf("*sa = s.substr( 1, 10 );\n" );heap_dump();

(*s).strip_crlf();
printf("(*s).strip_ws();\n" );heap_dump();

delete s;
delete sa;
delete sb;
printf("delete all\n" );heap_dump();


//printf("_heapmin\n" );
//_heapmin();
//heap_dump();
#endif
    
a75 4
        
    if( need_announce() )
        announce();
        
d78 8
d132 1
a132 1
                ansi_brown ansi_intense "Warning:" ansi_normal
@


1.6
log
@with memory checker
@
text
@d1 11
d15 1
a20 51
#if TESTALLOC
void heap_dump(const char *where)
          {
          struct _heapinfo h_info; 
          int heap_status; 

          int used = 0;
      
          h_info._pentry = NULL; 
          for(;;) { 
            heap_status = _heapwalk( &h_info ); 
            if( heap_status != _HEAPOK ) break; 
/*
            printf( "  %s block at %Fp of size %4.4X\n", 
              (h_info._useflag == _USEDENTRY ? "USED" : "FREE"), 
              h_info._pentry, h_info._size ); 
*/
          if( h_info._useflag == _USEDENTRY ) used += h_info._size;
          } 

          char buf [100];
          if( used >= 10240 )
              {
              sprintf( buf, "%s: total %dK of unfreed memory!", where, used/1024 );
              Error(buf);
              }
          else
              {
              sprintf( buf, "%s: total %d bytes of unfreed memory!", where, used );
              Error(buf);
              }
      
          switch( heap_status ) { 
          case _HEAPEND: 
//            printf( "OK - end of heap\n" ); 
            break; 
          case _HEAPEMPTY: 
//            printf( "OK - heap is empty\n" ); 
            break; 
          case _HEAPBADBEGIN: 
            Error( "heap is damaged" ); 
            break; 
          case _HEAPBADPTR: 
            Error( "bad pointer to heap" ); 
            break; 
          case _HEAPBADNODE: 
            Error( "bad node in heap" ); 
          } 
        } 
#endif

a143 22

#define ansi_esc "\x1b["

#define ansi_lighgray       ansi_esc"37m"
#define ansi_cyan           ansi_esc"36m"
#define ansi_magenta        ansi_esc"35m"
#define ansi_blue           ansi_esc"34m"
#define ansi_brown          ansi_esc"33m"
#define ansi_green          ansi_esc"32m"
#define ansi_red            ansi_esc"31m"
#define ansi_black          ansi_esc"30m"

#define ansi_invisible      ansi_esc"8m"
#define ansi_reverse        ansi_esc"7m"
#define ansi_darkgray_bg    ansi_esc"5m"
//#define ansi_blue           ansi_esc"4m"
#define ansi_intense        ansi_esc"1m"
#define ansi_normal         ansi_esc"0m"




a190 15


void debug(string s)
    {
    if( conf.debug() )
        {
        if( conf.ansi() )
            fprintf( stderr, ansi_magenta "Debug:"ansi_normal
                    " %s\n", (const char *)s );
        else
            fprintf( stderr, "Debug: %s\n", (const char *)s );
        Log( "Debug: ", s );
        }
    }

@


1.5
log
@Signal interception, chdir, etc.
@
text
@a8 2
#define TESTALLOC 0

d10 1
a10 1
void heap_dump()
d32 1
a32 1
              sprintf( buf, "total %dK of unfreed memory!", used/1024 ); 
d37 1
a37 1
              sprintf( buf, "total %d bytes of unfreed memory!", used ); 
d108 1
a108 1
#if TESTALLOC
@


1.4
log
@before COW strings
@
text
@d2 2
d9 89
d105 39
d145 4
a148 1
    BoldMsg( "Frip - FTN Networks Automatic Routing Builder: " VERDEF "\n");
d152 6
a157 2
    
    return main_loop() == Ok ? 0 : 1;
d162 3
d258 9
a266 2
    fprintf( stderr, "Debug: %s\n", (const char *)s );
    Log( "Debug: ", s );
@


1.3
log
@beta 9
@
text
@d3 3
a10 1
    fprintf( stderr, "Frip - FTN Networks Automatic Routing Builder: Beta 9\n");
d15 1
d36 2
a37 2
            "Total %d rips ignored, %d new, %d better, %d timed out & replaced",
            no_good_v, new_addr_v, better_repl_v, timeout_repl_v );
d69 7
a75 3
    fprintf( stderr, 
            ansi_red ansi_intense "Error:" ansi_normal
            " %s\n", (const char *)s );
d81 7
a87 3
    fprintf( stderr, 
            ansi_brown ansi_intense "Warning:" ansi_normal
            " %s\n", (const char *)s );
d94 6
a99 3
    fprintf( stderr, 
            ansi_green ansi_intense "*** " ansi_normal
            "%s\n", (const char *)s );
d105 7
a111 3
    fprintf( stderr, 
            ansi_brown ansi_intense "*!* " ansi_normal
            "%s\n", (const char *)s );
@


1.2
log
@beta 7
@
text
@d8 1
a8 1
    fprintf( stderr, "Frip - FTN Networks Automatic Routing Builder: Beta 7\n");
d27 12
@


1.1
log
@Initial revision
@
text
@d8 4
a11 2
    fprintf( stderr, "Frip - FTN Networks Automatic Routing Builder: Beta 5\n");
        
d22 5
d57 1
d65 1
d74 1
d80 1
a80 1
            ansi_brown ansi_intense "*** " ansi_normal
d82 1
d89 1
@

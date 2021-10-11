/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Config file parser
 *
 *      $Log: Config.C $
 *      Revision 1.20  1997/03/22 15:48:45  dz
 *      datetime
 *
 *      Revision 1.19  1997/03/17 07:59:51  dz
 *      DBFATALTIMEOUT
 *
 *      Revision 1.18  1997/01/31 10:13:43  dz
 *      Silence support
 *      PackDirect support
 *
 *      Revision 1.17  1997/01/21 08:53:51  dz
 *      Match aka method.
 *      Turned off keywords needed for external router.
 *
 *      Revision 1.16  1997/01/02 10:06:18  dz
 *      StatLog
 *
 *      Revision 1.15  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.14  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.13  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.12  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.11  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.10  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.9  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.8  1996/01/14 01:06:31  dz
 *      beta 14
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

config conf;


void config::open_log( const string &fn )
    {
#ifdef __IBMCPP__
    log_stream_v.open( fn, ios::app );
#else
    log_stream_v.open( fn, ios::append );
#endif
    if( !log_stream_v )
        Error("Can't create log file: "+fn);
    else
        {
        log_v = Yes;
        //time_ t t = time(NULL);
        log_stream_v << "\n--- Started: " << //ctime( &t );
            (const char *)(datetime::now().localtime_ctime_locale_string()) << "\n";
        }
    }

void config::open_stat_log( const string &fn )
    {
#ifdef __IBMCPP__
    stat_log_stream_v.open( fn, ios::app );
#else
    stat_log_stream_v.open( fn, ios::append );
#endif
    if( !stat_log_stream_v )
        Error("Can't create stat_log file: "+fn);
    else
        {
        stat_log_v = Yes;
#if 0
        //time_ t t = time(NULL);
        stat_log_stream_v << "\n--- Started: " << //ctime( &t );
            datetime::now().localtime_ctime_locale_string() << "\n";

#endif
        }
    }


static int match_aka_weight( const addr & a, const addr & aka )
    {
    if( a.domain() != aka.domain() ) return 0;
    if( a.zone  () != aka.zone  () ) return 1;
    if( a.net   () != aka.net   () ) return 2;
    if( a.node  () != aka.node  () ) return 3;
    if( a.point () != aka.point () ) return 4;
    return 5;
    }

const addr & config::match_aka( const addr & a )
    {
    int maxno = 0, maxw = 0;

    int c = aka.count();
    for( int i = 0; i < c; i++ )
        {
        int n = match_aka_weight( a, aka[i] );
        if( n > maxw )
            {
            maxw = n;
            maxno = i;
            }
        }
    return aka[maxno];
    }



static bool tobool( string v, const string &opt )
    {
    v.ucase();
    
    if( v == "YES" || v == "Y" || v == "„€" || v == "SURE" || v == "ON"
       || v == "RULEZ" || v == "2B" || v.is_empty() )
        return Yes;
    
    if( v == "NO" || v == "N" || v == "H…’" || v == "NEVER" || v == "OFF"
       || v == "SUXX" || v == "~2B" )
        return No;
    
    Error("Value ("+v+") of boolean option ("+opt+") is not 'Yes' or 'No', 'No' assumed");
    return No;
    }
    

static datetime toperiod( string val, const string &opt, const datetime &def )
    {
    int result;
    char c;
    int n = sscanf( val, "%ld%c", &result, &c );
    if( n == 1 ) c = 'd';
    if( n <= 0 )
        {
        Error("Syntax error: "+val);
        return def;
        }
    else
        {
        if( result < 0 ) 
            {
            result = -result;
            Error("Negative time in "+opt+"? Forced to be positive.");
            }
        switch(c)
            {
            default:
                Error("Unknown modifier in "+opt+", days assumed");
                // fall through
            case 'D': case 'd':
                result *= 60*60*24;
                break;
            case 'H': case 'h':
                result *= 60*60;
                break;
            case 'M': case 'm':
                result *= 60;
                break;
            case 'S': case 's':
                break;
            }
        }

    return datetime( result, datetime::UTC );
    }



void config::
do_cfg_line( string line )
    {
//    debug("do_cfg_line( "+line+" )");
    
    string verb, val;
    
    line.strip_ws();
    line.parse( verb, val );
    
    verb.strip_ws();
    verb.strip_crlf();
    verb.ucase();
    
    val.strip_ws();
    val.strip_crlf();

    if( verb == "ADDRESS" || verb == "AKA" )        aka.add( addr(val) );
    else if( verb == "ANSI" )                       ansi_v = tobool( val, verb );
    else if( verb == "DEBUG" )                      debug_v = tobool( val, verb );
    else if( verb == "SILENCE" )                    silence_v = tobool( val, verb );
    else if( verb == "ANNOUNCEFROMADFILES" )        announce_from_ad_files_v = tobool( val, verb );
    else if( verb == "DIR" )                        dir_v = val;
    else if( verb == "INBOUND" )                    inbound_v = val;
    else if( verb == "FRIPMAIL" )                   fripmail_v = val;
    else if( verb == "PACKFRIPMAIL" )               fripmail_pack_v = val;
    else if( verb == "PACKDIRECT" )                 fripmail_pack_direct_v = tobool( val, verb );
    else if( verb == "ZIPMAIL" )                    zipmail_v = atoi(val);
    else if( verb == "LOG" )                        open_log( val );
    else if( verb == "STATLOG" )                    open_stat_log( val );
#if EXT_ROUTER
    else if( verb == "STRIPDOMAIN" )                strip_domain_v = tobool( val, verb );
    else if( verb == "POINTROUTING" )               point_routing_v = tobool( val, verb );
    else if( verb == "WILDWORD" )                   wild_word_v = val;
    else if( verb == "ROUTEWILDAFTER" )             route_wild_after_v = tobool( val, verb );
    else if( verb == "ROUTEFILEPATH" )              route_file_path_v = val;
    else if( verb == "ROUTEFILENAME" )              route_file_name_v = val;
    else if( verb == "ROUTEFILEPREF" )              route_file_pref_v = val;
    else if( verb == "ROUTEFILESUFF" )              route_file_suff_v = val;
    //else if( verb == "ROUTELINEPREF" )        //    route_line_pref_v = val;
    //else if( verb == "ROUTELINESUFF" )        //    route_line_suff_v = val;
    else if( verb == "ROUTELINE" )                  route_line_v = val;
    else if( verb == "VIASELF" )                    viaself_v = tobool( val, verb );
#else // EXT_ROUTER
    else if( verb == "STRIPDOMAIN" )                Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "POINTROUTING" )               Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "WILDWORD" )                   Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTEWILDAFTER" )             Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTEFILEPATH" )              Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTEFILENAME" )              Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTEFILEPREF" )              Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTEFILESUFF" )              Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "ROUTELINE" )                  Warning("Setup string "+line+" is not needed any more. Ignored.");
    else if( verb == "VIASELF" )                    Warning("Setup string "+line+" is not needed any more. Ignored.");
#endif // EXT_ROUTER
    else if( verb == "BINKOUTBOUND" )               bink_outbound_v = val;
    else if( verb == "BINKOUTBOUNDDEFAULT" )        bink_outbound_default_v = val;
    else if( verb == "HPFSOUTBOUND" )               hpfs_outbound_v = val;
    else if( verb == "TMAILFILEBOXESFAT" )          tmail_fileboxes_fat_v = val;
    else if( verb == "TMAILFILEBOXESHPFS" )         tmail_fileboxes_hpfs_v = val;
    else if( verb == "DELIVERCOMMAND" )             deliver_command_v = val;
    else if( verb == "DBTIMEOUT" )                  db_timeout_v = datetime( atoi(val), datetime::UTC );
    else if( verb == "DBFATALTIMEOUT" )             db_fatal_timeout_v = toperiod( val, verb, def_db_fatal_timeout_v );
    else if( verb == "HOLD" )                       hold_v.add_all(val);
    else if( verb == "ANNOUNCEPERIOD" )             announce_period_v = toperiod( val, verb, def_announce_period_v );
    else if( verb == "PINGPERIOD" )                 ping_period_v = toperiod( val, verb, def_ping_period_v );
    else if( verb == "HOPPENALTY" )                 hop_penalty_v = toperiod( val, verb, datetime(def_hop_penalty_v, datetime::UTC) );
    else if( verb == "DIRECTPRIORITY" )             direct_priority_v = toperiod( val, verb, datetime(def_direct_priority_v, datetime::UTC) );

    else if( verb == "ZIPCMD" )                     zipcmd_v = val;
    else if( verb == "UNZIPCMD" )                   unzipcmd_v = val;
    else if( verb == "CMDLIMIT" )
        {
        cmdlimit_v = atoi(val);
        if( cmdlimit_v < 100 )
            {
            cmdlimit_v = 100;
            Error("Config: CMDLIMIT < 100: "+val );
            }
        }
    else if( verb == "MAP" )
        {
        map_enabled_v = Yes;
        map_dir_v = val;
        }
    else if( verb == "MAPFORMAT" )                  map_format_v = val;
    else if( verb == "PROCESSEDFLAG" )
        {
        processed_flag_v = Yes;
        processed_flag_file_v = val;
        }
    else
        Error("Frip.CFG: Unknown parameter "+verb );
    
    }


void parse_config()
    {
    const char *infn = getenv("FRIP");
    if( (!access("frip.cfg",0)) || infn == NULL ) infn = "frip.cfg";
    
    ifstream in( infn );
    if( !in )
        throw Ex_Fail( "parse_config", "Can't open config", infn );

    while(!in.eof())
        {
        const linebs = 1000;
        char  lineb[linebs];
        string line;
        
        in.getline( lineb, linebs );
        
        line = lineb;
        
        line.strip_ws();
        line.strip_crlf();
        
        if( line[0]==';' || line.is_empty() )
            continue;

        conf.do_cfg_line( line );
        }

    if( conf.aka.count() == 0 )
        throw Ex_Fail( "parse_config", "Not a single address defined!", infn );
    
    addr::default_addr = conf.aka[0];
    }


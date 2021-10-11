head	1.21;
access;
symbols
	Beta_12:1.5;
locks;
comment	@ *      @;


1.21
date	97.03.22.15.30.15;	author dz;	state Exp;
branches;
next	1.20;

1.20
date	97.03.17.08.03.42;	author dz;	state Exp;
branches;
next	1.19;

1.19
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	97.01.31.10.13.43;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	97.01.02.10.06.18;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.12.28.03.11.55;	author dz;	state Exp;
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
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.01.18.00.58.45;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.14.01.06.31;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.12.05.04.23.40;	author dz;	state Exp;
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


1.21
log
@datetime
annouce period changed to 5 days
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Config.
 *
 *      $Log: conf.h $
 *      Revision 1.20  1997/03/17 08:03:42  dz
 *      db_fatal_timeout
 *
 *      Revision 1.19  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.18  1997/01/31 10:13:43  dz
 *      Silence support
 *      PackDirect support
 *
 *      Revision 1.17  1997/01/21 08:53:51  dz
 *      Turned off keywords needed for external router
 *
 *      Revision 1.16  1997/01/02 10:06:18  dz
 *      StatLog
 *
 *      Revision 1.15  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
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
 *      Revision 1.10  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.9  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.8  1996/01/18 00:58:45  dz
 *      Beta 15
 *
 *      Revision 1.7  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.6  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.5  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef CONF_H
#define CONF_H

#include "addr.h"
#include "strng.h"
#include "sglist.h"
#include "datetime.h"

#include <vector.h>

#include <fstream.h>

class aka_list
    {
private:
    vector            <addr>       a;
    
    
public:
    
    aka_list() {}
    
    int          count() { return a.size(); }
    void         add( const addr &in ) { a.push_back( in ); }
    const addr & operator [] (int i) { return a[i];}
    
    };


//const datetime def_announce_period_v  (  7ul*24u*60u*60u, datetime::UTC ); // pаз в 7 дней pеанонс
const datetime def_announce_period_v  (  5ul*24u*60u*60u, datetime::UTC ); // pаз в 5 дней pеанонс
//const long def_ping_period_v = 2ul*24u*60u*60u; // pаз в 2 дня - пинг
const datetime def_ping_period_v      (  7ul*24u*60u*60u, datetime::UTC ); // пока - pаз в неделю
const datetime def_db_timeout_v       (  8ul*24u*60u*60u, datetime::UTC ); // 8 дней на устаpевание pоутинга
// было 13, но теперь уменьшено ввиду добавления параметра
// laziness в устареватель, коий откладывает момент еще на
// некоторое время, в среднем, думаю, на сутки-двое.
const datetime def_db_fatal_timeout_v ( 32ul*24u*60u*60u, datetime::UTC ); // 32 дня на фатальное устаpевание pоутинга

#ifdef __DOS__
#  define def_zipcmd_v   "pkzip -m -es "
#  define def_unzipcmd_v   "pkunzip -n"
#  define def_cmdlimit_v 100
#else
#  ifdef __NT__ // Fucking win95 has DOS-like command.com!
#    define def_cmdlimit_v 100
#  else
#    define def_cmdlimit_v 400
#  endif
#  define def_zipcmd_v "zip -mj1 "
#  define def_unzipcmd_v   "unzip -Cjn"
#endif


class config
    {
    string            dir_v;
    string            inbound_v;

    string            fripmail_v;
    string            fripmail_pack_v;
    bool              fripmail_pack_direct_v;
    //bool              fripmail_unpack_noroute_v; // future enh.
    int               zipmail_v; // Will zip mail messages, that LESS than this value
    
    bool              strip_domain_v;
    bool              point_routing_v;
    
    bool              ansi_v;

#if EXT_ROUTER
    
    string            wild_word_v;
    bool              route_wild_after_v;
    
    string            route_file_path_v;
    string            route_file_name_v;
    string            route_file_pref_v;
    string            route_file_suff_v;
    
    // string            route_line_pref_v;
    // string            route_line_suff_v;
    string            route_line_v;
    bool              viaself_v;
    
#endif
    
    string            bink_outbound_v;
    string            bink_outbound_default_v;
    
    string            hpfs_outbound_v;
    
    string            tmail_fileboxes_fat_v;
    string            tmail_fileboxes_hpfs_v;
    
    string            deliver_command_v;
    
    bool              log_v;
    ofstream          log_stream_v;

    bool              stat_log_v;
    ofstream          stat_log_stream_v;

    bool              silence_v; // keep silence, don't cry too much
    
    bool              processed_flag_v;
    string            processed_flag_file_v;
    
      // Чеpез сколько секунд запись в базе устаpевает
    datetime          db_timeout_v;

      // Чеpез сколько секунд запись в базе устаpевает так, что ее нужно немедленно снести
    datetime          db_fatal_timeout_v;
    
    datetime          announce_period_v;
    datetime          ping_period_v;
    
    sglist            hold_v;   // which addresses send held rips to
    
    string            zipcmd_v;
    string            unzipcmd_v;
    int               cmdlimit_v;
    
    bool              debug_v;
    
    bool              map_enabled_v;
    string            map_dir_v;
    string            map_format_v;
    
    bool              announce_from_ad_files_v;
    
    void              open_log( const string &fn );
    void              open_stat_log( const string &fn );
    
public:
    config()
        :
    db_timeout_v       ( def_db_timeout_v,       datetime::UTC ),
    db_fatal_timeout_v ( def_db_fatal_timeout_v, datetime::UTC ),
    announce_period_v  ( def_announce_period_v,  datetime::UTC ),
    ping_period_v      ( def_ping_period_v,      datetime::UTC )
        { 
        dir_v = "."; 
        inbound_v = "";
        ansi_v = Yes;

#if EXT_ROUTER
        strip_domain_v = No;
        point_routing_v = Yes;
        wild_word_v = "All";
        route_wild_after_v = Yes;
        route_file_path_v = ".";
        route_file_name_v = "routes.inc";
        // deliver_command_v = "deliver %1 %2";
        route_line_v = "%0 %1";
        viaself_v = No;
#endif
        fripmail_pack_direct_v = Yes;

        zipcmd_v = def_zipcmd_v;
        unzipcmd_v = def_unzipcmd_v;
        cmdlimit_v = def_cmdlimit_v;
        debug_v = No;
        map_enabled_v = No;
        map_dir_v = "./map";
#ifdef __DOS__
        map_format_v = "%s/%s/%s/%s/%s.%s";
#else
        map_format_v = "%s/%s#%s_%s_%s.%s";
#endif
        announce_from_ad_files_v = No;
        processed_flag_v = No;
        processed_flag_file_v = "proc.!!!"; // just for a while
        zipmail_v = 0;
        silence_v = Yes;
        }
    
    void              do_cfg_line( string line );
    
    bool              log()        { return log_v; }
    ofstream &        log_stream() { return log_stream_v; }
    
    bool              stat_log()        { return stat_log_v; }
    ofstream &        stat_log_stream() { return stat_log_stream_v; }
    
    aka_list          aka;
    
    const string & dir() const             { return dir_v;           }
    const string & inbound() const         { return inbound_v;       }
    const string & fripmail() const        { return fripmail_v;      }
    const string & fripmail_pack() const   { return fripmail_pack_v; }

    const bool     fripmail_pack_direct() const { return fripmail_pack_direct_v; }
    
#if EXT_ROUTER
    bool           strip_domain() const    { return strip_domain_v;  };
    bool           point_routing() const   { return point_routing_v; };
    const string & wild_word() const       { return wild_word_v;     }
    bool           route_wild_after() const{ return route_wild_after_v; };
    
    const string & route_file_path()       { return route_file_path_v; };
    const string & route_file_name()       { return route_file_name_v; };
    const string & route_file_pref()       { return route_file_pref_v; };
    const string & route_file_suff()       { return route_file_suff_v; };
    
    //const string & route_line_pref()       { return route_line_pref_v; };
    //const string & route_line_suff()       { return route_line_suff_v; };
    const string & route_line()            { return route_line_v;            }
    bool           viaself()               { return viaself_v;               }
#endif
    
    const string & bink_outbound()         { return bink_outbound_v;         }
    const string & bink_outbound_default() { return bink_outbound_default_v; }
    
    const string & hpfs_outbound()         { return hpfs_outbound_v;         }
    
    const string & tmail_fileboxes_fat()   { return tmail_fileboxes_fat_v;   }
    const string & tmail_fileboxes_hpfs()  { return tmail_fileboxes_hpfs_v;  }
    
    const string & deliver_command()       { return deliver_command_v;       }
    datetime       db_timeout()            { return db_timeout_v;            }
    datetime       db_fatal_timeout()      { return db_fatal_timeout_v;      }
    
    bool           hold( const addr &a )   { return hold_v.match(a, No);     }
    
    datetime       announce_period()       { return announce_period_v;       }
    datetime       ping_period()           { return ping_period_v;           }
    
    string const & zipcmd()                { return zipcmd_v;                }
    string const & unzipcmd()              { return unzipcmd_v;              }
    int            cmdlimit()              { return cmdlimit_v;              }
    
    bool           ansi()                  { return ansi_v;                  }

    bool           silence()               { return (silence_v && (!debug_v)) ? Yes : No; }
    bool           debug()                 { return debug_v;                 }
    
    bool           map_enabled()           { return map_enabled_v;           }
    string         map_dir()               { return map_dir_v;               }
    string         map_format()            { return map_format_v;            }
    
    bool           announce_from_ad_files() { return announce_from_ad_files_v;}
    
    bool           processed_flag()        { return processed_flag_v;        }
    const string & processed_flag_file()   { return processed_flag_file_v;   }
    
    const addr &   match_aka( const addr & a );
    };


extern config  conf;


void           Log( const char *p, const string &s );
void           StatLog( const string &s );

void           debug( string );
void           Error( string );
void           Warning( string );
void           Msg( string );
void           BoldMsg( string );



#endif // CONF_H
@


1.20
log
@db_fatal_timeout
@
text
@d8 3
d67 1
d90 2
a91 1
const long def_announce_period_v = 7ul*24u*60u*60u; // pаз в 7 дней pеанонс
d93 2
a94 3
const long def_ping_period_v = 7ul*24u*60u*60u; // пока - pаз в неделю
const long def_db_timeout_v = 8ul*24u*60u*60u; // 8 дней на устаpевание pоутинга
const long def_db_fatal_timeout_v = 32ul*24u*60u*60u; // 32 дня на фатальное устаpевание pоутинга
d98 1
d170 1
a170 1
    time_t            db_timeout_v;   
d173 1
a173 1
    time_t            db_fatal_timeout_v;
d175 2
a176 2
    time_t            announce_period_v;
    time_t            ping_period_v;
d196 6
a201 1
    config() 
d219 1
a219 6
        
        db_timeout_v = def_db_timeout_v;
        db_fatal_timeout_v = def_db_fatal_timeout_v;
            
        announce_period_v = def_announce_period_v;
        ping_period_v = def_ping_period_v;
d281 2
a282 2
    time_t         db_timeout()            { return db_timeout_v;            }
    time_t         db_fatal_timeout()      { return db_fatal_timeout_v;      }
d286 2
a287 2
    time_t         announce_period()       { return announce_period_v;       }
    time_t         ping_period()           { return ping_period_v;           }
@


1.19
log
@Ver. 34
@
text
@d8 3
d90 1
d164 1
a164 1
    // Чеpез сколько секунд запись в базе устаpевает
d166 3
d211 2
d277 1
@


1.18
log
@Silence support
PackDirect support
@
text
@d8 4
d114 1
@


1.17
log
@Turned off keywords needed for external router
@
text
@d8 3
d109 1
d150 2
d196 1
d216 1
d233 2
d274 2
a275 1
    
@


1.16
log
@StatLog
@
text
@d8 3
d112 2
d126 3
a168 2
    bool              viaself_v;
    
d177 3
a181 1
        ansi_v = Yes;
d188 3
a207 1
        viaself_v = No;
d226 1
d240 2
d276 1
a276 1
    bool           viaself()               { return viaself_v;               }
@


1.15
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d134 4
a137 1
    
d164 1
d206 3
d258 1
a258 1
    bool          announce_from_ad_files() { return announce_from_ad_files_v;}
d271 2
@


1.14
log
@Exceptions added, not compiled
@
text
@d8 3
d99 1
d204 1
@


1.13
log
@Before exceptions.
@
text
@d8 3
@


1.12
log
@Clean up of watcom classlib based code
@
text
@d8 3
d93 1
d184 1
@


1.11
log
@FripMail written
@
text
@d8 3
a39 1
#if USE_STL
a40 3
#else
#include <wcvector.h>
#endif
a46 1
#if USE_STL
a47 3
#else
    WCPtrOrderedVector<addr>       a;
#endif
a53 1
#if USE_STL
a56 5
#else
    int          count() { return a.entries(); }
    void         add( const addr &in ) { a.insert( new addr( in ) ); }
    addr         operator [] (int i) { return *(a[i]);}
#endif
@


1.10
log
@state save
@
text
@d8 3
d99 2
d200 8
a207 6
    const string & dir()                   { return dir_v;           }
    const string & inbound()               { return inbound_v;       }
    bool           strip_domain()          { return strip_domain_v;  };
    bool           point_routing()         { return point_routing_v; };
    const string & wild_word()             { return wild_word_v;     }
    bool           route_wild_after()      { return route_wild_after_v; };
@


1.9
log
@C Set & STL support
@
text
@d8 3
d69 1
a69 1
const long def_announce_period_v = 14ul*24u*60u*60u; // pаз в 14 дней pеанонс
d72 1
a72 1
const long def_db_timeout_v = 11ul*24u*60u*60u; // 11 дней на устаpевание pоутинга
@


1.8
log
@Beta 15
@
text
@d8 3
d31 3
d35 2
d42 3
d46 1
d51 1
a51 1
//    aka_list() {  }
d53 9
a61 2
    int        count() { return a.entries(); }
    void       add( const addr &in ) { a.insert( new addr( in ) ); }
a62 1
    addr operator [] (int i) { return *(a[i]);}
@


1.7
log
@beta 14
@
text
@d8 3
d58 1
a58 1
#  define def_unzipcmd_v   "pkunzip -n "
d67 1
a67 1
#  define def_unzipcmd_v   "unzip -Cjn "
d129 2
d164 1
d223 1
@


1.6
log
@*** empty log message ***
@
text
@d8 3
d46 2
a47 1
const long def_ping_period_v = 2ul*24u*60u*60u; // pаз в 2 дня - пинг
d103 3
d157 2
d212 5
a216 1
    bool           announce_from_ad_files() { return announce_from_ad_files_v;}
@


1.5
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d23 1
d43 6
a48 1
const long def_db_timeout_v = 13ul*24u*60u*60u; // 13 дней на устаpевание pоутинга
d54 1
a54 1
#  ifdef __NT__ // Fucking win95 has DOS-like commanc.com!
d103 1
d117 2
d137 1
d149 1
d189 2
d203 1
@


1.4
log
@before COW strings
@
text
@d1 11
d42 1
d51 1
d58 2
d98 1
d101 6
d113 1
d126 1
d128 8
d146 1
d176 1
d179 8
a186 1
    bool           ansi()                  { return ansi_v; }
d193 1
@


1.3
log
@beta 7
@
text
@d6 1
d26 16
d47 3
d75 10
d93 1
d100 4
d137 9
@


1.2
log
@b6
@
text
@d33 4
d38 18
d65 4
d72 4
a75 1

d78 24
a101 5
    const string &    dir() {return dir_v; };    // our work dir
    bool              strip_domain()  { return strip_domain_v;  };
    bool              point_routing() { return point_routing_v; };
    const string &    wild_word()     { return wild_word_v;     }
    bool              route_wild_after() { return route_wild_after_v; };
@


1.1
log
@Initial revision
@
text
@d28 7
d36 11
a46 2
    config() { dir_v = "."; }
        
d48 1
d50 4
@

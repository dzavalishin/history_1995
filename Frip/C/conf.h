/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Config.
 *
 *      $Log: conf.h $
 *      Revision 1.21  1997/03/22 15:30:15  dz
 *      datetime
 *      annouce period changed to 5 days
 *
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

// Время, на которое мы ухудшаем рип за каждый хоп сверх двух
const long     def_hop_penalty_v      ( 2ul*60u*60u ); // 2 hours
// Время, на которое мы ухудшаем рип, если он - не директный (более хопа)
const long     def_direct_priority_v  ( 8ul*60u*60u ); // 8 hours

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

    long              hop_penalty_v;
    long              direct_priority_v;

    
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

        fripmail_pack_direct_v = Yes;

        zipcmd_v = def_zipcmd_v;
        unzipcmd_v = def_unzipcmd_v;
        cmdlimit_v = def_cmdlimit_v;
        debug_v = No;
        map_enabled_v = No;
        map_dir_v = "./map";
#ifdef __DOS__
        map_format_v = "%s/%s/%s/%s/%s";
#else
        map_format_v = "%s/%s#%s_%s_%s";
#endif
        announce_from_ad_files_v = No;
        processed_flag_v = No;
        processed_flag_file_v = "proc.!!!"; // just for a while
        zipmail_v = 0;
        silence_v = Yes;


        hop_penalty_v = def_hop_penalty_v;
        direct_priority_v = def_direct_priority_v;
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

    long           hop_penalty()           { return hop_penalty_v;           }
    long           direct_priority()       { return direct_priority_v;       }
    
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

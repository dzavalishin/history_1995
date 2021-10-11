head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Ver. 34
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995-97 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Pinger
 *
 *      $Log: ping.c $
 *      Revision 1.1  1996/12/28 03:11:55  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "frip.h"
//#include "filer.h"

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
    


bool need_ping()
    {
    time_t now = time(0);
    time_t last_ping = -1;
    time_t last_ping_file = -1;
    
    ifstream  lp_time( conf.dir()+"/Ping.Tim" );
    if( !lp_time )
        {
        Warning("No ping time stamp found. Running for the first time?");
        return Yes;
        }
    
    lp_time >> last_ping;
    if( !lp_time.fail() )
        {
        if( (now > conf.ping_period() + last_ping) && last_ping != -1 )
            {
            Msg( "Initiating regular pinging" );
            return Yes;
            }
        }
    
    return No;
    }



void mark_ping()
    {
    time_t now = time(0);
    
    ofstream  lp_time( conf.dir()+"/Ping.Tim" );
    if( !lp_time ) throw Ex_Errno("mark_ping","Can't create file",errno);
    
    lp_time << now;
    if( lp_time.fail() )
        throw Ex_Errno("mark_ping","Can't write time stamp",errno);
    }













@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Announce.C $
d17 1
a17 1
#include "filer.h"
@

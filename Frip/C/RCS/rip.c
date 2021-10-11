head	1.16;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ *      @;


1.16
date	97.03.26.10.59.57;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	97.01.31.10.14.35;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.01.22.19.23.05;	author dz;	state Exp;
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
date	96.01.16.23.12.52;	author dz;	state Exp;
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


1.16
log
@datetime fixes
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Class
 *
 *      $Log: rip.c $
 *      Revision 1.15  1997/01/31 10:14:35  dz
 *      Silence support
 *
 *      Revision 1.14  1997/01/21 08:53:51  dz
 *      Bug fix: point would not send any announces to his boss.
 *
 *      Revision 1.13  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.12  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.11  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.10  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.9  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.8  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.7  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.6  1996/01/16 23:12:52  dz
 *      unknown fields code error fixed - now adding \n
 *      added code to hack around 'ADflags + -NA !' :(
 *      written void rip::increment_time( time_t t )
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "osname.h"
#include "version.h"


#if 0
string rip::created_s() const
    {
    char b[100]; const tm & c = created_v;
    sprintf( b, "%d/%d/%d %d:%d:%d", 
            c.tm_mday, c.tm_mon+1, c.tm_year, // we store month as 1-12, but tm counts it as 0-11
            c.tm_hour, c.tm_min, c.tm_sec
           );
    return b;
    }


void rip::created_s(string const &s)
    {
    string date, time;
    
    if( !s.parse( date, time ) )
        throw Ex_Arg( "rip::created_s", "RIP \"created\" line format incorrect", s );
    
    if( 3 != sscanf( date, "%d/%d/%d",
                    &created_v.tm_mday,
                    &created_v.tm_mon, 
                    &created_v.tm_year ) )
        throw Ex_Arg( "rip::created_s", "RIP date format incorrect", s );
    
    created_v.tm_mon--; // we store month as 1-12, but tm counts it as 0-11
    
    if( 3 != sscanf( time, "%d:%d:%d",
                    &created_v.tm_hour, 
                    &created_v.tm_min, 
                    &created_v.tm_sec ) )
        throw Ex_Arg( "rip::created_s", "RIP time format incorrect", s );
    }
#endif

void rip::put_path( ofstream &o )
    {
    string s;
    
    static datetime  pt(0, datetime::UTC);
    static char      pts[30];
    static char    * vd;
    
    if( pt == datetime(0, datetime::UTC) )        // Don't want to ask time all the time
        {
        pt = datetime::now();
        const tm tt(pt.localtime_tm());
        strftime( pts, 29, "%H:%M:%S %d/%m/%Y", &tt );
#ifdef FRIP_VER_STR
        vd = FRIP_VER_STR;
        if( strlen( vd ) == 0 )
#endif
            vd = "Internal build ("__DATE__ " " __TIME__")";
        }
    
    for( int i = 0; i < path_v.size(); i++ )
        {
        string pathid = " FRIP " + string(vd) + " for " OS_NAME ", " + string(pts);
        s = (path_v[i]) /* + pathid */;
        //debug("path el = "+s);
        o << "path " << ((const char *)s);
        //o << " " << vd << " for " OS_NAME ", " << pts /* << "\n" */;
        o << "\n";
        }
    
    }


void rip::put_seenby( ofstream &o )
    {
    string s;
    for( int i = 0; i < seenby_v.size(); i++ )
        {
        s = seenby_v[i];
        o << "seenby " << ((const char *)s);
        o << "\n";
        }
    }

void rip::put_unknown( ofstream &o )
    {
    for( int i = 0; i < unknown_v.size(); i++ )
        o << ((const char *)unknown_v[i]) << "\n";
    }

void rip::load( string const &s )
    {
    ifstream f( s );
    if( !f )       throw Ex_Fail("rip::load","Can't open",s);
    
    hops_v = 0;
    capas_v = "";
    
    path_v.erase  ( path_v.begin(), path_v.end() );
    seenby_v.erase( seenby_v.begin(),  seenby_v.end() );
    
    bool got_ad = No, got_off = No, got_type = No;
    bool got_created = No, got_from = No, got_creator = No;
    bool got_search = No;
    
    while( !f.eof() )
        {
        const bs = 200;
        char  buf[bs];
        
        f.getline( buf, bs );
        if( f.eof() || f.fail() )
            break;
        
        string key, val;
        
        string(buf).parse( key, val );
        key.ucase();
        
        if( key == "FROM" )
            {
            string vc = val;
            vc.lcase();
            if( vc == "myself" ) from_v = conf.aka[0];
            else                 from_v = addr( val );
            got_from = Yes;
            }
        else
        if( key == "TO" )
            to_v = addr( val );
        else
        if( key == "TYPE" )
            {
            type_v = rip_type( val );
            got_type = Yes;
            }
        else
        if( key == "HOPS" )
            hops_v = atoi( val );
        else        
        if( key == "CREATED" )
            {
            created_v = datetime::from_rip_string( val );
            got_created = Yes;
            }
        else        
        if( key == "AD" )
            {
            announce_v = addr( val );
            got_ad = Yes;
            }
        else        
        if( key == "OFF" )
            {
            off_v = addr( val );
            got_off = Yes;
            }
        else        
        if( key == "CREATOR" )
            {
            creator_v = addr( val );
            got_creator = Yes;
            }
        else        
        if( key == "PATH" )
            {
            string path, tail;
            val.parse( path, tail );
              //debug( "path = "+path+" tail = "+tail );
            addr pa = addr( path );
            path_v.push_back( pa );
            path_info[pa]= tail; // map
            }
        else        
        if( key == "FLAGS" )
            {
            string flags_s = val;
            fplus_v.extract( flags_s, '+' );
            fminus_v.extract( flags_s, '-' );
            finfo_v.extract( flags_s, '!' );
            }
        else
        if( key == "CAPAS" )
            capas_v = val;
        else
        if( key == "SEARCH" )
            {
            search_v = val;
            got_search = Yes;
            }
        else
        if( key == "SEENBY" )
            seenby_v.push_back( addr( val ) );
        else        
            unknown_v.push_back( buf );  // unknown: save as is
        }
    
    if( !got_type ) type_v = rip_type::AD;         // Совместимость с B11-
    
    if( got_created != Yes || got_from != Yes )
        throw Ex_Fail("rip::load","Incomplete RIP",s);
    
    switch( type_v )
        {
        case rip_type::AD:
            if( got_ad != Yes )  throw Ex_Fail("rip::load","No AD field in AD-type RIP",s);
            break;
        case rip_type::OFF:
            if( got_off != Yes ) throw Ex_Fail("rip::load","No OFF field in OFF-type RIP",s);
            break;
        case rip_type::SEARCH:
            if( got_search != Yes ) throw Ex_Fail("rip::load","No SEARCH field in SEARCH-type RIP",s);
            break;
        }
    
    
    if( ((rip_type::rtype)type_v) == rip_type::NOP )
        {
        if( got_ad )
            {
            Warning("There's AD field, assuming AD-type RIP: "+s );
            type_v = rip_type::AD;
            }
        else
            throw Ex_Fail("rip::load","Unknown RIP type",s);
        }
    

    if( got_creator != Yes )   // А стоит ли?
        {
        creator_v = from_v;    // Это довольно вольное пpедположение...
        Warning("There's no Creator field, assuming myself as creator: "+s );
        }

        {
        bool rif_capa = No;
        
        string tail, el;

        capas_v.parse( el, tail );

        if( el.is_empty() || (addr(el) != from_v) )
            {
            tail = "";
            if(!conf.silence()) Msg("addr mismatch, capas invalidated: "+capas_v+" (don't worry)");
            }
        
        tail.ucase();
        tail.strip_ws();
        while( tail.length() )
            {
            tail.parse( el, tail );
            if( el == "RIF" ) rif_capa = Yes;
            }

        if( rif_capa == No )
            {
            fminus_v += "RIF";
            Warning("Rip is from non-RIF capable FRIP, adding -RIF flag");
            }
        }
    }


bool rip::seenby_matches( const addr &a ) const
    {
    for( int i = 0; i < seenby_v.size(); i++ )
        {

        if( seenby_v[i].matches( a ) )
            {
              // next if is a workaround.
              // without it point will not send any announces to his boss at all
            if( a.point() == 0 ) // possibly (a) is boss of (seenby_v[i])
                {
                addr test = seenby_v[i];
                test.point(0);
                if( test == a && seenby_v[i] != a )
                    continue;
                }
            
            return Yes;
            }
        }
    return No;
    }


void rip::save_to_file( ofstream &f )
    {
    f << "type " <<    ((string)type_v).c_str()           << "\n";

    f << "from " <<    ((string)from_v).c_str()           << "\n";
    f << "capas " <<   ((string)from_v).c_str() << " " FRIP_CAPAS "\n";

      // NB! "capas" field MUST be marked with address from "from"
      // field to be treated as valid at next hop.
      // This field should not be relayed, but generated anew each time
    
    f << "to " <<      ((string)to_v).c_str()                                  << "\n";
    f << "created " << ((const char *)created_v.localtime_rip_string())        << "\n";
    f << "creator " << ((string)creator_v).c_str()                             << "\n";
    
    f << 
        "flags +"<< ((string)fplus_v).c_str() << 
        " -"<< ((string)fminus_v).c_str() <<
        " !"<< ((string)finfo_v).c_str() <<
        "\n";
    
    if( type_v == rip_type::AD )
        {
        f << "ad " <<      ((string)announce_v).c_str()   << "\n";
        f << "hops " <<    hops_v                         << "\n";
        }
    
    if( type_v == rip_type::OFF )
        {
        f << "off " <<     ((string)off_v).c_str()        << "\n";
        f << "hops " <<    hops_v                         << "\n";
        }
    
    if( type_v == rip_type::SEARCH )
        {
        f << "search " <<      search_v.c_str()           << "\n";
        f << "hops " <<    hops_v                         << "\n";
        }
    
    put_path( f );
    put_seenby( f );
    put_unknown( f );
    
    if( f.fail() ) throw Ex_Fail("rip::save_to_file","Error saving RIP","");
    }



void rip::save_to_dir( const string &dir )
    {
    string tempn = tempFileName(dir+"/rip?????.rip");
    ofstream temp( tempn );
    if( !temp )  throw Ex_Fail("rip::save_to_dir","can't create file",tempn);
    save_to_file( temp );
    }


static rip_type::rtts rttab[] =
    {
    rip_type::AD,     "AD",
    rip_type::HI,     "HI",
    rip_type::HACK,   "HACK",
    rip_type::RRQ,    "RRQ",
    rip_type::RACK,   "RACK",
    rip_type::OFF,    "OFF",
    rip_type::PING,   "PING",
    rip_type::AD,     "ADflags + -NA !",   // this is to overcome bug with missing \n
    rip_type::AD,     "ADflags + - !",   // this is to overcome bug with missing \n
    rip_type::SEARCH, "SEARCH",
    rip_type::NOP,    "NOP"        // Keep this line at the end!
    };
    

rip_type::rip_type( string a )
    {
    for( int i = 0; rttab[i].val != NOP; i++ )
        if( 0 == stricmp( rttab[i].name, a ) )
            {
            val = rttab[i].val;
            return;
            }
    
    val = NOP;
    Error("Unknown RIP type: "+a);
    }

rip_type::operator string () const
    {
    for( int i = 0; rttab[i].val != NOP; i++ )
        if( val == rttab[i].val )
            return rttab[i].name;
    
    if( val != NOP ) Error("Unknown RIP type value!");
    return "NOP";
    }

/*
void rip::created( datetime t )
    {
    created_v = t;
    }
*/


void rip::increment_time( datetime t )
    {
    //time_t tt = mktime( &created_v );
    //tt -= t;
      //created_v = *localtime( &tt );
    created_v -= t;
    }

@


1.15
log
@Silence support
@
text
@d7 4
a10 1
 *      $Log: Rip.C $
d54 2
d88 1
a88 1

d94 9
a102 8
    static time_t  pt = 0;
    static char    pts[30];
    static char  * vd;
    
    if( pt == 0 )        // Don't want to ask time all the time
        {
        pt = time(0);
        strftime( pts, 29, "%H:%M:%S %d/%m/%Y", localtime(&pt) );
d192 1
a192 1
            created_s( val );
d349 3
a351 3
    f << "to " <<      ((string)to_v).c_str()             << "\n";
    f << "created " << ((const char *)created_s())        << "\n";
    f << "creator " << ((string)creator_v).c_str()        << "\n";
d434 2
a435 1
void rip::created( time_t t )
d437 1
a437 1
    created_v = *localtime( &t );
d439 1
d442 1
a442 2

void rip::increment_time( time_t t )
d444 4
a447 3
    time_t tt = mktime( &created_v );
    tt -= t;
    created_v = *localtime( &tt );
@


1.14
log
@Bug fix: point would not send any announces to his boss.
@
text
@d8 3
d288 1
a288 1
            Msg("addr mismatch, capas invalidated: "+capas_v+" (don't worry)");
@


1.13
log
@Version 25
@
text
@d8 3
d309 1
a309 1
        // if( match_addr( *(seenby_v[i]), a ) )
d311 11
d323 1
@


1.12
log
@Exceptions added, not compiled
@
text
@d8 3
d60 1
a60 1
    if( !parse( s, date, time ) )
d141 1
d154 1
a154 1
        parse( buf, key, val );
d205 1
a205 1
            parse( val, path, tail );
d223 6
d248 3
d277 1
a277 1
        parse( capas_v, el, tail );
d289 1
a289 1
            parse( tail, el, tail );
d347 6
d362 1
a362 1
bool rip::save_to_dir( const string &dir )
d373 11
a383 10
    rip_type::AD,   "AD",
    rip_type::HI,   "HI",
    rip_type::HACK, "HACK",
    rip_type::RRQ,  "RRQ",
    rip_type::RACK, "RACK",
    rip_type::OFF,  "OFF",
    rip_type::PING, "PING",
    rip_type::AD,   "ADflags + -NA !",   // this is to overcome bug with missing \n
    rip_type::AD,   "ADflags + - !",   // this is to overcome bug with missing \n
    rip_type::NOP,  "NOP"        // Keep this line at the end!
@


1.11
log
@Before exceptions.
@
text
@d8 3
d53 1
a53 1
bool rip::created_s(string const &s)
d58 1
a58 4
        {
        Error( "RIP \"created\" line format incorrect: "+s );
        return Err;
        }
d64 1
a64 4
        {
        Error( "RIP date format incorrect: "+s );
        return Err;
        }
d72 1
a72 5
        {
        Error( "RIP time format incorrect: "+s );
        return Err;
        }
    return Ok;
d125 1
a125 1
bool rip::load( string const &s )
d128 1
a128 6
    
    if( !f )
        {
        Error( "Can't open "+s );
        return Err;
        }
d176 1
a176 2
            if( created_s( val ) == Err )
                return Err;
d202 4
a205 2
            //debug( "path = "+path+" tail = "+tail );
            path_v.push_back( addr( path ) );
d228 1
a228 4
        {
        Error("Incomplete RIP: "+s );
        return Err;
        }
d233 1
a233 5
            if( got_ad != Yes )
                {
                Error("No AD field in AD-type RIP: "+s );
                return Err;
                }
a234 1
        
d236 1
a236 5
            if( got_off != Yes )
                {
                Error("No OFF field in OFF-type RIP: "+s );
                return Err;
                }
a242 1
        Error("Unknown RIP type: "+s);
d249 1
a249 1
            return Err;
d269 1
a269 1
            debug("capas invalidated due to addr mismatch :"+el);
a285 2
    
    return Ok;
d301 1
a301 1
bool rip::save_to_file( ofstream &f )
d338 1
a338 1
    return f.fail() ? Err : Ok;
a345 7
    
    if( tempn=="" )
        {
        Error( "rip generation aborted - no temp file name" );
        return Err;
        }

d347 2
a348 8
    
    if( !temp )
        {
        Error( "rip generation aborted - can't create file: "+tempn );
        return Err;
        }
    
    return save_to_file( temp );
@


1.10
log
@FripMail written
@
text
@d8 3
d332 1
a332 1
    f << "capas " <<   ((string)from_v).c_str() << " RIF\n";
@


1.9
log
@C Set, mostly works
@
text
@d7 4
a10 1
 *      $Log: Rip.c $
d34 1
d84 3
a86 3
    static ulong pt = 0;
    char         pts[30];
    char        *vd;
d91 6
a96 6
        strcpy( pts, ctime((const time_t *)&pt) );
#define PATH_IDENT(i) #i
        vd = PATH_IDENT(VERDEF);
#undef PATH_IDENT
        if( strlen( vd ) == 0 ) 
            vd = "Internal version ("__DATE__ " " __TIME__")";
a98 1
#if USE_STL
d101 3
a103 1
        s = path_v[i];
a107 8
#else
    for( int i = 0; i < path_v.entries(); i++ )
        {
        s = *(path_v[i]);
        o << "path " << ((const char *)s);
        o << " " << vd << " for " OS_NAME ", " << pts /* << "\n" */;
        }
#endif
a114 2
    
#if USE_STL
a120 9
#else
    for( int i = 0; i < seenby_v.entries(); i++ )
        {
        s = *(seenby_v[i]);
        o << "seenby " << ((const char *)s);
        o << "\n";
        }
#endif
    
a124 1
#if USE_STL
a126 4
#else
    for( int i = 0; i < unknown_v.entries(); i++ )
        o << ((const char *)(*(unknown_v[i]))) << "\n";
#endif
d140 2
a141 1
#if USE_STL
a143 4
#else
    path_v.clearAndDestroy();
    seenby_v.clearAndDestroy();
#endif
a212 2
#if USE_STL
            //path_v.insert( path_v.begin(), addr( path ) );
a213 3
#else
            path_v.insert( new addr( path ) );
#endif
d223 4
a226 3
        
        else        
#if USE_STL
a230 8
#else
        if( key == "SEENBY" )
            seenby_v.insert( new addr( val ) );
        else        
            unknown_v.insert( new string( buf ) );  // unknown: save as is
#endif
        
        
d275 1
d277 30
a306 1
    
a313 1
#if USE_STL
a319 8
#else
    for( int i = 0; i < seenby_v.entries(); i++ )
        {
        // if( match_addr( *(seenby_v[i]), a ) )
        if( (*(seenby_v[i])).matches( a ) )
            return Yes;
        }
#endif
d327 1
d329 6
@


1.8
log
@C Set & STL support
@
text
@d8 3
d100 2
a101 1
        o << " " << vd << " for " OS_NAME ", " << pts /* << "\n" */;
d234 1
a234 1
            debug( "path = "+path+" tail = "+tail );
@


1.7
log
@Beta 15
@
text
@d8 3
d84 4
a87 2
        strcpy( pts, ctime(&pt) );
        vd = VERDEF;
d92 8
d106 1
d115 8
d129 1
d135 4
d141 1
d155 4
d161 1
d231 4
d236 1
d248 6
d258 1
d314 8
d328 1
@


1.6
log
@unknown fields code error fixed - now adding \n
added code to hack around 'ADflags + -NA !' :(
written void rip::increment_time( time_t t )
@
text
@d8 5
d24 1
a24 1

d74 13
d91 1
a91 1
        o << "\n";
d194 6
a199 1
            path_v.insert( new addr( val ) );
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d95 1
a95 1
        o << ((const char *)(*(unknown_v[i])));
d226 7
a232 1
        return Err;
d322 2
d355 10
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d109 3
a111 2
    bool got_ad = No, got_created = No, got_from = No, got_creator = No;

d138 6
d160 6
d175 9
d192 3
a194 1
    if( got_ad != Yes || got_created != Yes || got_from != Yes )
d200 27
d247 34
d300 25
a324 10
    temp << "from " <<    ((string)from_v).c_str()           << "\n";
    temp << "to " <<      ((string)to_v).c_str()             << "\n";
    temp << "hops " <<    hops_v                             << "\n";
    temp << "created " << ((const char *)created_s())        << "\n";
    temp << "creator " << ((string)creator_v).c_str()        << "\n";
    temp << "ad " <<      ((string)announce_v).c_str()       << "\n";
    
    put_path( temp );
    put_seenby( temp );
    put_unknown( temp );
d326 2
a327 1
    return temp.fail() ? Err : Ok;
d330 9
d340 4
a343 1

@


1.3
log
@beta 7
@
text
@d1 11
@


1.2
log
@b6
@
text
@d174 2
a175 1
        if( match_addr( *(seenby_v[i]), a ) )
@


1.1
log
@Initial revision
@
text
@d196 1
a196 1
        Error( "rip generation aborted - can't create file" );
@

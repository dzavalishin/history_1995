/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Class
 *
 *      $Log: rip.c $
 *      Revision 1.16  1997/03/26 10:59:57  dz
 *      datetime fixes
 *
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

void rip::put_comment( ofstream &o )
    {
    for( int i = 0; i < comment_v.size(); i++ )
        o << "Rem " << ((const char *)comment_v[i]) << "\n";
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
        if( key == "REM" )
            comment_v.push_back( val );
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
    put_comment( f );
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



void rip::increment_time( datetime t )
    {
    created_v -= t;
    }


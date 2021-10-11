head	1.17;
access;
symbols
	Beta_12:1.7;
locks;
comment	@ *      @;


1.17
date	97.03.28.22.04.49;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	97.01.02.10.06.18;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.12.28.03.11.55;	author dz;	state Exp;
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
date	96.01.22.05.03.34;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.05.23.16.13;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.13.06.46.09;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.17
log
@cleanup, cosmetics
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Misc. code
 *
 *      $Log: util.C $
 *      Revision 1.16  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.14  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
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
 *      Revision 1.7  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "filer.h"
#include "stat.h"
#include "ansi.h"
#include <ctype.h>


#ifdef __IBMCPP__

// No hidden files!
#define ATTRS2FIND	(FILE_READONLY|FILE_SYSTEM|FILE_ARCHIVED)

bool filer::do_restart( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	h = HDIR_CREATE; 

	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );

	if( rc == NO_ERROR )
		{
		name = ff.achName;
		restart_flag = No;
                opened_flag = Yes;
                valid_v = Yes;
		return Yes;
		}

	if( rc == ERROR_NO_MORE_FILES )
		{
                //close();
                valid_v = No;
		return No;
		}
        
        valid_v = No;
	throw Ex_Errno( "filer::do_restart", "FindFirst error", rc );
	}


bool filer::do_continue( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	rc = DosFindNext( h, &ff, sizeof(ff), &count );

	if( rc == NO_ERROR )
		{
                name = ff.achName;
                valid_v = Yes;
		return Yes;
		}
        
        valid_v = No;
        
	if( rc == ERROR_NO_MORE_FILES )
		return No;

        throw Ex_Errno( "filer::do_restart", "FindNext error", rc );
	}


void filer::next()
    {
    switch( restart_flag ? do_restart( result ) : do_continue( result ) )
        {
        case No:                   valid_v = No;    break;
        default:                   valid_v = Yes;   break;
        }
    }


filer::filer( string dir, string mask_a )
    {
    mask = dir + "/" + mask_a;
    valid_v = No;
    restart_flag = Yes;
    opened_flag = No;
    next();
    }

filer::filer()
    {
    valid_v = No;
    restart_flag = Yes;
    opened_flag = No;
    }


bool filer::valid()
    {
    return valid_v;
    }
    

string filer::current()
    {
    return result;
    }
    

filer::~filer()
    {
    APIRET          rc;
    
    if( opened_flag )
        {
        restart_flag = Yes;
    
        if( (rc = DosFindClose( h )) != NO_ERROR )
            Error( "FindClose error" );
        }
    
    }
#else
void filer::next()
    {
    do 
        {
        if( dirp != NULL ) direntp = readdir( dirp );
        else 
            {
            direntp = NULL;
            return;
            }
        if( direntp == 0 ) return;
        }
    while( !fnmatch( direntp->d_name, mask ) );

    }

filer::filer( string dir, string mask_a )
    {
    dirp = opendir( dir );
    direntp = 0;
    mask = mask_a;
    next();
    }


bool filer::valid()
    {
    return (dirp == 0 || direntp == 0 ) ? No : Yes;
    }
    

string filer::current()
    {
    return direntp->d_name;
    }
    

filer::~filer()
    {
    closedir( dirp );
    }
#endif




static bool match_one( const char *data, const char *mask )
    {
    const char *mcp = mask, *ncp = data;

    bool match = No;

    while( *mcp && *ncp )
        {
        if(
           (toupper(*mcp) == toupper(*ncp)) ||
           *mcp == '?'
          )
            {
            mcp++, ncp++;
            continue;
            }
    
        if( mcp[0] == '*' && mcp[1] == 0 )
            return Yes;
        if( mcp[0] == '*' )
            throw Ex_Arg("match_one","* is not last in mask",mask);
        return No;
        }
    
    return (*mcp == *ncp) ? Yes : No;
    }



bool filer::fnmatch( const char *name, const char *mask )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    char mdr[_MAX_DRIVE], mdi[_MAX_DIR], mfn[_MAX_FNAME], mex[_MAX_EXT];
    _splitpath( (char *)name, ndr, ndi, nfn, nex );
    _splitpath( (char *)mask, mdr, mdi, mfn, mex );
    
    if(0 == stricmp( nex, mex ) && 0 == strcmp(mfn,"*")) return Yes;

    return
        (match_one(nex,mex) && match_one(nfn,mfn))
        ? Yes : No;
    }





bool our_aka_matches( const addr &a )
    {

    if( conf.aka.count() == 0 )
        throw Ex_Arg( "our_aka_matches", "No our addresses defined", "" );

    for( int i = 0; i < conf.aka.count(); i++ )
        //if( match_addr(conf.aka[i],a) ) return Yes;
        if( conf.aka[i].matches(a) ) return Yes;

    return No;
    }

bool is_our_aka( const addr &a )
    {

    if( conf.aka.count() == 0 ) return No;

    for( int i = 0; i < conf.aka.count(); i++ )
        if( conf.aka[i] == a ) return Yes;

    return No;
    }


  // -----------------------------------------------------------------------

void tempFile( fstream &f, string &outname, const string &mask, int mode )
    {
    int tries = 20;
    do
        {
        if( tries-- < 0 )  throw Ex_Fail("tempFile", "Tired of trying", mask);
        outname = tempFileName(mask);
        f.open( outname, mode );
        }
    while( !f );
    }


string tempFileName( const string &mask )
    {
    long  t = time(NULL);
    int   tries = 100;
    static long int modifier = 0;
    
    modifier += 0x5005;  // Чтобы дважды одно имя не pожать даже если такого файла нет
    
    char   name[600], ts[40];
    do
        {
        t += time(NULL);
        t += modifier;
        
        strncpy( name, mask, 600 );
        sprintf( ts, "%ld", t%99999 );
        
        const char *i = ts;
        char *o = name;
        
        while( *i && *o )
            {
            if( *o == '?' ) *o++ = *i++;
            else o++;
            }
        
        while( *o )
            {
            if( *o == '?' ) *o = '0';
            o++;
            }
        
        if( o==name || (tries-- == 0) )  // No '?' in name? Out of luck?
            throw Ex_Fail( "tempFileName", "I've tried enough!", "" );
        
        }
#ifdef __IBMCPP__
    while( 0 == access(name, 0));
#else
    while( 0 == access(name, F_OK));
#endif
    
    return name;
    }

  // -----------------------------------------------------------------------


string newdir( const char *name, string dir )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)name, ndr, ndi, nfn, nex );
    
    return dir+"\\"+string(nfn)+string(nex);
    }

void mkpath( const string &path )
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



  // -----------------------------------------------------------------------

void statistics::report()
    {
      //char st[280];
    string st;

    if(no_good_v||new_addr_v||better_repl_v||timeout_repl_v||filtered_v)
        {
        /*
        sprintf( st,
                "RIPs: %3d ignored, %3d new, %3d better, %3d timed out, %3d filtered",
                no_good_v, new_addr_v, better_repl_v, timeout_repl_v, filtered_v );
                */
        st.print( "RIPs: %3d ignored, %3d new, %3d better, %3d timed out, %3d filtered",
                no_good_v, new_addr_v, better_repl_v, timeout_repl_v, filtered_v );
        BoldMsg( st );
        }

    if(rifs_total_v||rifs_for_us_v||rifs_forw_v||rifs_held_v)
        {
        /*
        sprintf( st,
                "RIFs: %3d total, %3d for us, %3d forwarded, %3d held",
                rifs_total_v, rifs_for_us_v, rifs_forw_v, rifs_held_v );
                */
        st.print( "RIFs: %3d total, %3d for us, %3d forwarded, %3d held",
                rifs_total_v, rifs_for_us_v, rifs_forw_v, rifs_held_v );
        BoldMsg( st );
        }
    }

statistics stats;

  // -----------------------------------------------------------------------

void Log( const char *p, const string &s )
    {
    if( !conf.log() ) return;
      //conf.log_stream() << p << ((const char *)s) << "\n";
    conf.log_stream() << p << s << "\n";
    }


void StatLog( const string &s )
    {
    if( !conf.stat_log() ) return;
      //conf.stat_log_stream() << ((const char *)s) << "\n";
    conf.stat_log_stream() << s << "\n";
    }



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
    }

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


void General_Ex::print() const
    {
    Error( where+": "+what+" ("+why+")" );
    }










@


1.16
log
@Ver. 34
@
text
@d8 2
a9 2
 *      Revision 1.15  1997/01/02 10:06:18  dz
 *      *** empty log message ***
a13 3
 *      Revision 1.13  1996/09/08 19:05:31  dz
 *      Version 25
 *
a22 6
 *      Revision 1.9  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.8  1996/01/22 05:03:34  dz
 *      C Set fixes
 *
a38 2
//#define ATTRS2FIND	0x27

d48 1
a48 1
	h = HDIR_CREATE; // HDIR_SYSTEM
a62 1
		//log( "#", "Nothing to do - no messages found" );
a88 9
#if 0
	// Sometimes handle goes nuts :(
	if( rc == ERROR_INVALID_HANDLE )
		{
		//log( "#", "findnext gave ERROR_INVALID_HANDLE, restarting..." );
		close();
		return do_restart( name );
		}
#endif
a98 2
    //bool	rc;
    
d359 2
a360 1
    char st[280];
d364 1
d368 4
a371 1
        BoldMsg( string(st) );
d376 1
d380 4
a383 1
        BoldMsg( string(st) );
d394 2
a395 1
    conf.log_stream() << p << ((const char *)s) << "\n";
d402 2
a403 1
    conf.stat_log_stream() << ((const char *)s) << "\n";
@


1.15
log
@*** empty log message ***
@
text
@d8 3
d48 4
a51 1
#define ATTRS2FIND	0x27
d278 13
a290 1
    
@


1.14
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d365 16
a380 9
    sprintf( st, 
            "RIPs: %3d ignored, %3d new, %3d better, %3d timed out, %3d filtered",
            no_good_v, new_addr_v, better_repl_v, timeout_repl_v, filtered_v );
    BoldMsg( string(st) );
    
    sprintf( st, 
            "RIFs: %3d total, %3d for us, %3d forwarded, %3d held",
            rifs_total_v, rifs_for_us_v, rifs_forw_v, rifs_held_v );
    BoldMsg( string(st) );
d391 7
@


1.13
log
@Version 25
@
text
@d8 3
d431 6
@


1.12
log
@Exceptions added, not compiled
@
text
@d8 3
d31 3
d207 28
a235 2
// Limitation!
// Matches extension only!
d243 5
a247 1
    return (0 == stricmp( nex, mex )) ? Yes : No;
d258 1
a258 4
        {
        Error( "Can't match ad against my address" );
        exit( 0 );
        }
d267 13
d326 3
@


1.11
log
@Before exceptions.
@
text
@d8 3
d35 1
a35 2
bool		
filer::do_restart( string &name )
a44 1

d63 1
a63 2
	Error( "FindFirst error" );
	return Err;
d67 1
a67 2
bool		
filer::do_continue( string &name )
d96 1
a96 2
	Error( "FindNext error" );
	return Err;
d102 1
a102 1
    bool	rc;
d106 1
a106 1
        case No: case Err:         valid_v = No;    break;
d269 1
a269 1
            return "";
d302 86
@


1.10
log
@FripMail written
@
text
@d8 3
@


1.9
log
@C Set, mostly works
@
text
@d7 4
a10 1
 *      $Log: util.c $
d279 23
@


1.8
log
@C Set fixes
@
text
@d7 4
a10 1
 *      $Log: Util.c $
d42 2
a43 1
		opened_flag = Yes;
d50 2
a51 1
		//close();
d54 2
a55 1

d72 2
a73 1
		name = ff.achName;
d77 1
d114 1
@


1.7
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d19 68
d88 10
d102 11
a112 4
    dirp = opendir( dir );
    direntp = 0;
    mask = mask_a;
    next();
d118 7
a124 1
    return (dirp == 0 || direntp == 0 ) ? No : Yes;
d127 15
d158 15
d183 3
d195 2
a196 2
    _splitpath( name, ndr, ndi, nfn, nex );
    _splitpath( mask, mdr, mdi, mfn, mex );
d259 3
d263 1
@


1.6
log
@before moving addr.c out
@
text
@d1 11
a15 162
static addr     addr::default_addr;

static void aatoi( bool &err, int &n, const string &a )
    {
    if( a[0] == '*' )
        {
        n = WILD;
        return;
        }
    
    if( !isdigit( a[0] ) )
        {
        Error("Non-digit in number "+a);
        err = Yes;
        n = 0;
        }
    n = atoi( a );
    }

addr::addr( const string & as )
    {
    domain_v = "";
    zone_v = 0;
    net_v = 0;
    node_v = 0;
    point_v = 0;
    
    string val, tail, s = as;
    
    bool got_domain = No, got_zone = No, got_net = No;
    bool got_node = No, got_point = No, err = No;
    
    
    if( parse( s, val, '#', tail ) )
        {
        domain_v = val;
        s = tail;
        got_domain = Yes;
        }
    if( parse( s, val, ':', tail ) )
        {
        aatoi( err, zone_v, val );
        s = tail;
        got_zone = Yes;
        }
    if( parse( s, val, '/', tail ) )
        {
        aatoi( err, net_v, val );
        s = tail;
        got_net = Yes;
        }
    if( parse( s, val, '@@', tail ) )
        {
        domain_v = tail;
        if( got_domain ) Error("More than one domain in address "+as);
        got_domain = Yes;
        s = val;
        }
    if( parse( s, val, '.', tail ) )
        {
        s = val;
        aatoi( err, point_v, tail );
        got_point = Yes;
        }
    
    if( s.is_empty() )   // no node number
        {
        if( got_domain || got_zone || got_net )
            Warning("Domain/zone/net without node number: "+as );
        }
    else
        {
        aatoi( err, node_v, s );
        got_node = Yes;
        }
    
    fill_defaults();
    
    if( err || ((!got_node) && (!got_point)) )
        {
        Error("Invalid address "+as);
        node_v=0;
        net_v=0;
        }
    }
       
       
addr::addr( const addr &a )
    {
    *this = a;
    fill_defaults();
    }


void addr::fill_defaults()
    {
    if( domain_v.is_empty() )
        domain_v = default_addr.domain_v;
    if( domain_v.is_empty() )
        domain_v = "fidonet.org";
    
    if( zone_v == 0 )
        zone_v = default_addr.zone_v;
    
    if( net_v == 0 )
        net_v = default_addr.net_v;
    
//    if( node_v == BAD_NODE )
//       node_v = default_addr.node_v;
    }

inline void p( char *o, int i, char c )
    {
    char val1[40];
    if( i == WILD )
        {
        sprintf( val1, "*%c", c );
        strcat( o, val1 );
        }
    else
        {
        sprintf( val1, "%d%c", i, c );
        strcat( o, val1 );
        }
    }

addr::operator string () const
    {
    char val[200];
    
    val[0] = 0;
    
    p( val, zone_v, ':' );
    p( val, net_v,  '/' );
    p( val, node_v, '.' );
    p( val, point_v, '@@' );
    
    strcat( val, domain_v );
    
    return string(val);
    }


addr & addr::operator = ( const addr &a )
    {
    domain_v = a.domain_v;
    zone_v = a.zone_v;
    net_v = a.net_v;
    node_v = a.node_v;
    point_v = a.point_v;
    fill_defaults();
    return *this;
    }

addr::addr()
    {
    domain_v = "";
    zone_v = 0;
    net_v = 0;
    node_v = 0;
    point_v = 0;
    }
a77 94
bool addr::dom_eq(string d1, string d2)
    {
debug("dom_eq begin" );
    if( d1.same(d2) ) return Yes;
    
debug("dom_eq lcase" );
    d1.lcase();
    d2.lcase();
    
debug("dom_eq parse" );
    string d11, d12, d21, d22;
        
    parse( d1, d11, '.', d12 );
    parse( d2, d21, '.', d22 );
        
debug("dom_eq parse done" );

    if( d12.is_empty() )     d22="";
    if( d22.is_empty() )     d12="";
debug("dom_eq normalize done" );

    if( d12 != d22 || d11 != d21 ) return No;
debug("dom_eq compare done" );
    
    return Yes;
    }




// Мы попадаем под маску a2

bool addr::matches( addr a2 ) const
    {
debug("addr::matches "+((string&)a2) );
domain_s star("*");
    if( (a2.domain() != star) && (dom_eq( domain(), a2.domain() ) == No) ) 
{
debug("addr::matches: domains passed, no" );
        return No;
}

debug("addr::matches: domains passed" );

    if( a2.zone() != WILD && zone() != a2.zone() ) return No;
    if( a2.net()  != WILD && net()  != a2.net()  ) return No;
    if( a2.node() != WILD && node() != a2.node() ) return No;

    /* node is allways a default route for its points */
    if( a2.point() != WILD && a2.point() != 0 && point() != a2.point() ) return No;

    return Yes;
    }

string addr::ntos( int v )
    {
    char bb[30];
    if( v == WILD ) return "*";
    sprintf( bb, "%d", v );
    return bb;
    }


#if 0

bool match_addr( addr a1, addr a2 )
    {

    if( a2.domain() != "*" )
        {
        string d11, d12, d21, d22;
        
	parse( a1.domain(), d11, '.', d12 );
	parse( a2.domain(), d21, '.', d22 );

	if( d12.is_empty() ) d22="";
	if( d22.is_empty() ) d12="";

	if( d12 != d22 || d11 != d21 ) return No;
        }

    if( a2.zone() != WILD && a1.zone() != a2.zone() ) return No;
    if( a2.net()  != WILD && a1.net()  != a2.net()  ) return No;
    if( a2.node() != WILD && a1.node() != a2.node() ) return No;

    /* node is allways a default route for its points */
    if( a2.point() != WILD && a2.point() != 0 && a1.point() != a2.point() ) return No;

    return Yes;
    }

#endif


a134 7
    }


bool domain_s::is_wild() const
    {
    const char *s = this->c_str();
    return ( s[0] == '*' && s[1] == 0 ) ? Yes : No;
@


1.5
log
@before COW strings
@
text
@d231 4
d238 1
d244 1
d248 1
d251 1
d263 5
a267 2
    
    if( (a2.domain() != "*") && (dom_eq( domain(), a2.domain() ) == No) ) 
d269 3
d283 7
d381 8
@


1.4
log
@beta 9
@
text
@d256 2
a257 1
    if( dom_eq( domain(), a2.domain() ) == No ) return No;
d302 1
a302 1
bool our_aka_matches( addr &a )
@


1.3
log
@beta 7
@
text
@d112 2
a113 2
    if( node_v == 0 )
        node_v = default_addr.node_v;
d323 3
d331 1
@


1.2
log
@b6
@
text
@d144 1
a144 1
    return val;
d229 42
d298 1
a298 1
    
d311 2
a312 1
        if( match_addr(conf.aka[i],a) ) return Yes;
@


1.1
log
@Initial revision
@
text
@d298 6
@

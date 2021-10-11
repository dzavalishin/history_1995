head	1.13;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ * @;


1.13
date	97.03.26.10.56.38;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	97.03.22.15.47.10;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.15.03.50.54;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.05.23.20.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.05.23.17.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.13
log
@short_string()
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Address class
 *
 *      $Log: Addr.C $
 *      Revision 1.12  1997/03/22 15:47:10  dz
 *      *** empty log message ***
 *
 *      Revision 1.11  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.10  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.8  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.7  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.6  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.5  1996/01/15 03:50:54  dz
 *      wildness class
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include <ctype.h>

const addr::Wild = -1;





// Мы попадаем под маску a2

bool addr::matches( const addr & a2 ) const
    {
    domain_s a1d = domain();
    domain_s a2d = a2.domain();
    domain_s star_d("*");

    int is_not_star = (a2d != star_d);
    int is_eq   = (a1d == a2d);

    if( is_not_star && (!is_eq) )
        return No;

    if( a2.zone() != WILD && zone() != a2.zone() ) return No;
    if( a2.net()  != WILD && net()  != a2.net()  ) return No;
    if( a2.node() != WILD && node() != a2.node() ) return No;

    /* node is allways a default route for its points */
    if( a2.point() != WILD && a2.point() != 0 && point() != a2.point() ) return No;

    return Yes;
    }


addr     addr::default_addr;

static int aatoi( const string &a )
    {
    if( a[0] == '*' )
        return WILD;
    
    if( !isdigit( a[0] ) )
        throw Ex_Arg("aatoi","Non-digit in number "+a);
    return atoi( a );
    }

addr::addr( const string & as )
    {
    domain_v = "";
    zone_v = 0;   net_v = 0;   node_v = 0;   point_v = 0;
    
    string val, tail, s = as;
    
    bool got_zone = No, got_net = No; //, err = No;
    bool got_domain = No, got_node = No, got_point = No;

    if( s.parse( val, '#', tail ) )
        {
        domain_v = val;        s = tail;        got_domain = Yes;
        }
    if( s.parse( val, ':', tail ) )
        {
        zone_v = aatoi( val ); s = tail;        got_zone = Yes;
        }
    if( s.parse( val, '/', tail ) )
        {
        net_v = aatoi( val );  s = tail;        got_net = Yes;
        }
    if( s.parse( val, '@@', tail ) )
        {
        domain_v = tail;
        if( got_domain ) Error("More than one domain in address "+as);
        got_domain = Yes;
        s = val;
        }
    if( s.parse( val, '.', tail ) )
        {
        s = val;        point_v = aatoi( tail );        got_point = Yes;
        }
    
    if( s.is_empty() )   // no node number
        {
        if( got_domain || got_zone || got_net )
            throw Ex_Arg("addr::addr( string )","Domain/zone/net without node number: "+as );
        }
    else
        {
        node_v = aatoi( s );
        got_node = Yes;
        }

    fill_defaults();
    
    if( (!got_node) && (!got_point) )
        {
        node_v=0;
        net_v=0;
        throw Ex_Arg("addr::addr( string )","Invalid address "+as );
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
        domain_v = "fidonet" /* "fidonet.org" */ ;
    
    if( zone_v == 0 )
        zone_v = default_addr.zone_v;
    
    if( net_v == 0 )
        net_v = default_addr.net_v;
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

string addr::short_string() const
    {
    char val[200];
    
    val[0] = 0;
    
    p( val, zone_v, ':' );
    p( val, net_v,  '/' );
    p( val, node_v, '.' );

    if( domain_v != default_addr.domain_v )
        {
        p( val, point_v, '@@' );
        strcat( val, domain_v );
        }
    else
        p( val, point_v, '~' );
    
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




string addr::ntos( int v )
    {
    char bb[30];
    if( v == WILD ) return "*";
    sprintf( bb, "%d", v );
    return bb;
    }



domain_s::domain_s()     {};
domain_s::domain_s( const string &s )   : string( s ) {};
domain_s::domain_s( const domain_s &s ) : string( s ) {};
domain_s::domain_s( const char *s )     : string( s ) {};


int domain_eq( const string &d1, const string &d2 )
    {
    if( d1.same(d2) ) return Yes;
    
    string d11, d12, d21, d22;
        
    d1.parse( d11, '.', d12 );
    d2.parse( d21, '.', d22 );
    
    d11.lcase();
    d21.lcase();
    d12.lcase();
    d22.lcase();
    
    if( d12.is_empty() )     d22="";
    if( d22.is_empty() )     d12="";

    if( d12 != d22 || d11 != d21 ) return No;
    
    return Yes;
    }



int domain_lt( const string &a, const string &b )
    {
    if( a.same(b) ) return No;
    
    string d11, d12, d21, d22;
        
    a.parse( d11, '.', d12 );
    b.parse( d21, '.', d22 );
    
    d11.lcase();
    d21.lcase();
    d12.lcase();
    d22.lcase();
    
    if( d12.is_empty() )     d22="";
    if( d22.is_empty() )     d12="";

    if( d11 < d21 ) return 1;
    if( d12 < d22 ) return 1;
    
    return 0;
    }



bool domain_s::is_wild() const
    {
    const char *s = this->c_str();
    return ( s[0] == '*' && s[1] == 0 ) ? Yes : No;
    }

int addr::operator    != ( const addr &a ) const
    { 
    return !(*this == a ); 
    }

int addr::operator    == ( const addr &a ) const
    {
    return 
        a.zone_v == zone_v &&
        a.net_v == net_v &&
        a.node_v == node_v &&
        a.point_v == point_v &&
        ((a.domain_v == domain_v) /* || dom_eq(a.domain_v, domain_v) */ );
    }

int addr::operator    < ( const addr &a ) const
    {
    if( domain_v <  a.domain_v ) return 1;
    if( domain_v != a.domain_v ) return 0;
    if( zone_v   <  a.zone_v   ) return 1;
    if( zone_v   >  a.zone_v   ) return 0;
    if( net_v    <  a.net_v    ) return 1;
    if( net_v    >  a.net_v    ) return 0;
    if( node_v   <  a.node_v   ) return 1;
    if( node_v   >  a.node_v   ) return 0;
    if( point_v  <  a.point_v  ) return 1;
    
    return 0;
    }


bool addr::is_wild() const
    {
    return
        (
         node_v == WILD ||
         point_v == WILD ||
         net_v == WILD ||
         zone_v == WILD ||
         domain_v.is_wild()
        ) ? Yes : No;
    }






wildness::wildness( string s )
    {
    v = None;
    s.ucase();
    
    if     ( s == "POINT"  ) v = Point;
    else if( s == "NODE"   ) v = Node;
    else if( s == "NET"    ) v = Net;
    else if( s == "ZONE"   ) v = Zone;
    else if( s == "DOMAIN" ) v = Domain;
    else
        throw Ex_Arg( "wildness::wildness( string s )", "Unknown wildness: "+s );
    }

bool wildness::match( const addr &a ) const
    {
    // Note falls through!
    switch( v )
        {
        case Domain:    if( a.domain() != string("*")) return No;
        case Zone:      if( a.zone()   != addr::Wild ) return No;
        case Net:       if( a.net()    != addr::Wild ) return No;
        case Node:      if( a.node()   != addr::Wild ) return No;
        case Point:     if( 
                           (a.point()  != addr::Wild ) &&
                           (a.point()  != 0 )
                          )                            return No;
        case None:      break;
        }
    return Yes;
    }



@


1.12
log
@*** empty log message ***
@
text
@d8 3
d189 22
@


1.11
log
@Ver. 34
@
text
@d8 3
a13 3
 *      Revision 1.9  1996/09/08 19:05:31  dz
 *      Version 25
 *
a25 3
 *      Revision 1.4  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
a45 2
//debug("addr::matches "+((string&)a2) );

a49 2
    //debug("matches("+a1d+","+a2d+")");

a50 1
    //bool is_eq   = dom_eq( domain(), a2.domain() );
d91 1
a91 3
        domain_v = val;
        s = tail;
        got_domain = Yes;
d95 1
a95 3
        zone_v = aatoi( val );
        s = tail;
        got_zone = Yes;
d99 1
a99 3
        net_v = aatoi( val );
        s = tail;
        got_net = Yes;
d110 1
a110 3
        s = val;
        point_v = aatoi( tail );
        got_point = Yes;
a153 3
    
//    if( node_v == BAD_NODE )
//       node_v = default_addr.node_v;
a227 1
//static bool dom_eq( const domain_s &d1, const domain_s &d2)
a229 2
    //d1.lcase();
    //d2.lcase();
a248 25
/*
int domain_s::operator == ( const string &b ) const
    { return dom_eq(*this,b); }

int domain_s::operator != ( const string &b ) const
    { return dom_eq(*this,b) ? No : Yes; }

int operator == ( const char *a,     const domain_s &b )
    { return dom_eq(a,b); }

int operator == ( const domain_s &a, const char *b )
    { return dom_eq(a,b); }

int operator == ( const domain_s &a, const domain_s &b )
    { return dom_eq(a,b); }

int operator != ( const char *a,     const domain_s &b )
    { return dom_eq(a,b) ? No : Yes; }

int operator != ( const domain_s &a, const char *b )
    { return dom_eq(a,b) ? No : Yes; }

int operator != ( const domain_s &a, const domain_s &b )
    { return dom_eq(a,b) ? No : Yes; }
*/
a251 2
//int domain_s::operator < ( const string &b ) const
//static int dom_less( const string &a, const string &b )
a362 28
#if 0
int domain_s::operator == ( const string &s2  ) const   { return dom_eq(*this,s2); }
int domain_s::operator != ( const string &s2   ) const  { return !dom_eq(*this,s2); }
int domain_s::operator >  ( const string &s2   ) const  { return !dom_less(s2,*this); }
int domain_s::operator <  ( const string &s2   ) const  { return !dom_less(*this,s2); }

int domain_s::operator == ( const istring &s2  ) const  { return dom_eq(*this,s2); }
int domain_s::operator != ( const istring &s2  ) const  { return !dom_eq(*this,s2); }
int domain_s::operator >  ( const istring &s2  ) const  { return !dom_less(s2,*this); }
int domain_s::operator <  ( const istring &s2  ) const  { return !dom_less(*this,s2); }

int domain_s::operator == ( const domain_s &s2 ) const  { return dom_eq(*this,s2); }
int domain_s::operator != ( const domain_s &s2 ) const  { return !dom_eq(*this,s2); }
int domain_s::operator >  ( const domain_s &s2 ) const  { return !dom_less(s2,*this); }
int domain_s::operator <  ( const domain_s &s2 ) const  { return !dom_less(*this,s2); }

int domain_eq( const string &l, const string &r )
    {
    return l.same(r) || ((int)dom_eq( l, r ));
    }

int domain_lt( const string &l, const string &r )
    {
    return (!l.same(r)) && ((int)dom_less( l, r ));
    }


#endif
@


1.10
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d47 1
a47 1
bool addr::matches( addr a2 ) const
d51 7
a57 1
    int is_not_star = (a2.domain() != domain_s("*") );
d59 1
a59 1
    int is_eq   = domain() == a2.domain();
@


1.9
log
@Version 25
@
text
@d8 3
d237 2
a238 1
static bool dom_eq( const domain_s &d1, const domain_s &d2)
d262 1
a262 1

d286 1
d289 3
a291 2

int domain_s::operator < ( const string &b ) const
d293 1
a293 1
    if( same(b) ) return No;
d297 1
a297 1
    (*this).parse( d11, '.', d12 );
d389 1
a389 1
        case Domain:    if( a.domain() != "*" ) return No;
d403 28
@


1.8
log
@Exceptions added, not compiled
@
text
@d8 3
d82 1
a82 1
    //bool got_zone = No, got_net = No, err = No;
d85 1
a85 1
    if( parse( s, val, '#', tail ) )
d91 1
a91 1
    if( parse( s, val, ':', tail ) )
d95 1
a95 1
        //got_zone = Yes;
d97 1
a97 1
    if( parse( s, val, '/', tail ) )
d101 1
a101 1
        //got_net = Yes;
d103 1
a103 1
    if( parse( s, val, '@@', tail ) )
d110 1
a110 1
    if( parse( s, val, '.', tail ) )
d130 1
a130 1
    if( err || ((!got_node) && (!got_point)) )
d242 2
a243 2
    parse( d1, d11, '.', d12 );
    parse( d2, d21, '.', d22 );
d259 1
a259 1
int domain_s::operator == ( const domain_s &b ) const
d262 1
a262 1
int domain_s::operator != ( const domain_s &b ) const
d271 2
d280 6
a285 1
int domain_s::operator < ( const domain_s &b ) const
d291 2
a292 2
    parse( *this, d11, '.', d12 );
    parse( b,     d21, '.', d22 );
@


1.7
log
@Before exceptions.
@
text
@d8 3
a41 5
#if 0
domain_s star("*");
    if( (a2.domain() != star) && (dom_eq( domain(), a2.domain() ) == No) ) 
        return No;
#else
a48 4
#endif

//debug("addr::matches: domains passed" );

d62 1
a62 1
static void aatoi( bool &err, int &n, const string &a )
d65 1
a65 4
        {
        n = WILD;
        return;
        }
d68 2
a69 6
        {
        Error("Non-digit in number "+a);
        err = Yes;
        n = 0;
        }
    n = atoi( a );
d75 1
a75 4
    zone_v = 0;
    net_v = 0;
    node_v = 0;
    point_v = 0;
d79 3
a81 4
    bool got_domain = No, got_zone = No, got_net = No;
    bool got_node = No, got_point = No, err = No;
    
    
d90 1
a90 1
        aatoi( err, zone_v, val );
d92 1
a92 1
        got_zone = Yes;
d96 1
a96 1
        aatoi( err, net_v, val );
d98 1
a98 1
        got_net = Yes;
d110 1
a110 1
        aatoi( err, point_v, tail );
d117 1
a117 1
            Warning("Domain/zone/net without node number: "+as );
d121 1
a121 1
        aatoi( err, node_v, s );
d124 1
a124 1
    
a128 1
        Error("Invalid address "+as);
d131 1
a232 1
//debug("dom_eq begin" );
a233 2
    
//debug("dom_eq lcase" );
a236 1
//debug("dom_eq parse" );
a246 2
//debug("dom_eq parse done" );

a248 1
//debug("dom_eq normalize done" );
a250 1
//debug("dom_eq compare done" );
d365 1
a365 1
        Error( "Unknown wildness, none assumed: "+s );
@


1.6
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: Addr.c $
@


1.5
log
@wildness class
@
text
@d8 3
d41 1
a41 1
    bool is_not_star = (a2.domain() != domain_s("*") );
d43 1
a43 1
    bool is_eq   = domain() == a2.domain();
d45 1
a45 1
    if( is_not_star && (is_eq == No) )
d63 1
a63 1
static addr     addr::default_addr;
@


1.4
log
@*** empty log message ***
@
text
@d8 3
d21 6
d367 38
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d29 3
a31 2
    bool is_star = (a2.domain() != "*");
    bool is_eq   = dom_eq( domain(), a2.domain() );
d33 1
a33 1
    if( is_star && (is_eq == No) ) 
d150 1
a150 1
        domain_v = "fidonet.org";
d217 15
d233 1
a233 1
bool addr::dom_eq(string d1, string d2)
d239 2
a240 2
    d1.lcase();
    d2.lcase();
d247 6
a252 1
        
d266 2
d269 2
d272 2
d275 2
a276 7
string addr::ntos( int v )
    {
    char bb[30];
    if( v == WILD ) return "*";
    sprintf( bb, "%d", v );
    return bb;
    }
d279 2
d282 2
d285 16
d302 5
a306 5
int domain_s::operator == ( const domain_s &b ) const
    { return addr::dom_eq(*this,b); }

int domain_s::operator != ( const domain_s &b ) const
    { return addr::dom_eq(*this,b) ? No : Yes; }
d330 30
@


1.2
log
@*** empty log message ***
@
text
@d1 11
d15 31
d216 1
a216 1
debug("dom_eq begin" );
d219 1
a219 1
debug("dom_eq lcase" );
d223 1
a223 1
debug("dom_eq parse" );
d229 1
a229 1
debug("dom_eq parse done" );
d233 1
a233 1
debug("dom_eq normalize done" );
d236 1
a236 1
debug("dom_eq compare done" );
a243 23
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
a253 11
#if 0

bool match_addr( addr a1, addr a2 )
    {

    if( a2.domain() != "*" )
        {
        string d11, d12, d21, d22;
        
	parse( a1.domain(), d11, '.', d12 );
	parse( a2.domain(), d21, '.', d22 );
a254 2
	if( d12.is_empty() ) d22="";
	if( d22.is_empty() ) d12="";
a255 2
	if( d12 != d22 || d11 != d21 ) return No;
        }
d257 2
a258 11
    if( a2.zone() != WILD && a1.zone() != a2.zone() ) return No;
    if( a2.net()  != WILD && a1.net()  != a2.net()  ) return No;
    if( a2.node() != WILD && a1.node() != a2.node() ) return No;

    /* node is allways a default route for its points */
    if( a2.point() != WILD && a2.point() != 0 && a1.point() != a2.point() ) return No;

    return Yes;
    }

#endif
d260 2
d271 14
@


1.1
log
@Initial revision
@
text
@a169 57
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

string filer::current()
    {
    return direntp->d_name;
    }
    

filer::~filer()
    {
    closedir( dirp );
    }



// Limitation!
// Matches extension only!
bool filer::fnmatch( const char *name, const char *mask )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    char mdr[_MAX_DRIVE], mdi[_MAX_DIR], mfn[_MAX_FNAME], mex[_MAX_EXT];
    _splitpath( name, ndr, ndi, nfn, nex );
    _splitpath( mask, mdr, mdi, mfn, mex );
    
    return (0 == stricmp( nex, mex )) ? Yes : No;
    }



a265 58
bool our_aka_matches( const addr &a )
    {

    if( conf.aka.count() == 0 )
        {
        Error( "Can't match ad against my address" );
        exit( 0 );
        }

    for( int i = 0; i < conf.aka.count(); i++ )
        //if( match_addr(conf.aka[i],a) ) return Yes;
        if( conf.aka[i].matches(a) ) return Yes;

    return No;
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
            return "";
        
        }
    while( 0 == access(name, F_OK));
    
    return name;
    }
@

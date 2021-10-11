/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Address class
 *
 *      $Log: Addr.C $
 *      Revision 1.13  1997/03/26 10:56:38  dz
 *      short_string()
 *
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
    if( s.parse( val, '@', tail ) )
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
    p( val, point_v, '@' );
    
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
        p( val, point_v, '@' );
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




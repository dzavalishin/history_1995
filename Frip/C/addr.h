/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Address class
 *
 *      $Log: addr.h $
 *      Revision 1.16  1997/03/26 10:56:38  dz
 *      short_string()
 *
 *      Revision 1.15  1997/03/22 16:04:57  dz
 *      *** empty log message ***
 *
 *      Revision 1.14  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.13  1997/01/31 10:12:17  dz
 *      Set methods added.
 *
 *      Revision 1.12  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.10  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.9  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.8  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.7  1996/01/15 03:50:54  dz
 *      wildness class
 *
 *      Revision 1.5  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef ADDR_H
#define ADDR_H

#include "strng.h"


const WILD = -1;



class addr
    {
    int             zone_v;
    int             net_v;
    int             node_v;
    int             point_v;
    domain_s        domain_v;
    
    void            fill_defaults();
    
    static string   ntos( int );
    
public:
    static const Wild;

    static addr     default_addr;
    
    addr( const addr & );
    addr( const string & );
    addr();
//    ~addr();
    
    bool            valid() const { return net_v == 0 ? No : Yes; };
    
    int             zone() const {return zone_v;}
    int             net() const {return net_v;}
    int             node() const {return node_v;}
    int             point() const {return point_v;}
    const domain_s& domain() const {return domain_v;}
    
    void            zone( int n )  { zone_v = n;}
    void            node( int n )  { node_v = n;}
    void            net( int n )   { net_v = n;}
    void            point( int n ) { point_v = n;}
    void            domain(const domain_s& s) { domain_v = s; }

    string          zone_s() const {return ntos(zone_v);}
    string          net_s() const {return ntos(net_v);}
    string          node_s() const {return ntos(node_v);}
    string          point_s() const {return ntos(point_v);}
    
    
    addr & operator = ( const addr &a );
    
    operator string () const;
    string          short_string() const;
    
    bool            matches( const addr & a2 ) const;
    
    int operator    == ( const addr &a ) const;
    int operator    != ( const addr &a ) const;
    int operator    <  ( const addr &a ) const;
    
    bool            is_wild() const;
    };




#define fido_Addr addr








class wildness
    {
    
public:
    enum ewildness { None, Point, Node, Net, Zone, Domain };
    
private:
    ewildness  v;
    
public:
    wildness( )                     : v(None) {};
    wildness( string );
    wildness( const wildness &vv )  : v(vv.v) {};
    wildness( ewildness vv )        : v(vv) {};
    
    
    int operator == ( const wildness &vv ) const
        { return v == vv.v; }
    
    operator string () const;
    
    bool match( const addr &a ) const;
    };








#endif // ADDR_H

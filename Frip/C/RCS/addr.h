head	1.16;
access;
symbols
	Beta_12:1.5;
locks;
comment	@ *      @;


1.16
date	97.03.26.10.56.38;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	97.03.22.16.04.57;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	97.01.31.10.12.17;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.15.03.50.54;	author dz;	state Exp;
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
date	96.01.07.00.29.44;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.16.00.54.44;	author dz;	state Exp;
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
@short_string()
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Address class
 *
 *      $Log: addr.h $
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
@


1.15
log
@*** empty log message ***
@
text
@d8 3
d94 1
@


1.14
log
@Ver. 34
@
text
@d8 3
a16 3
 *      Revision 1.11  1996/09/08 19:05:31  dz
 *      Version 25
 *
a28 3
 *      Revision 1.6  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
a40 1
//#pragma pack(1);
a43 2
//bool     dom_eq(string d1, string d2);

a100 14
/*
int operator == ( const char *a,     const domain_s &b );
//    { return dom_eq(a,b); }

int operator == ( const domain_s &a, const char *b );
//    { return dom_eq(a,b); }


int operator != ( const char *a,     const domain_s &b );
//    { return dom_eq(a,b) ? No : Yes; }

int operator != ( const domain_s &a, const char *b );
//    { return dom_eq(a,b) ? No : Yes; }
*/
a142 1
//#pragma pack();
@


1.13
log
@Set methods added.
@
text
@d8 3
d98 1
a98 1
    bool            matches( addr a2 ) const;
@


1.12
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d79 3
a81 1
    void            node( int n ) { node_v = n;}
@


1.11
log
@Version 25
@
text
@d8 3
a44 30
class domain_s : public string
    {
public:
    domain_s();
    domain_s( const string &s );
    domain_s( const domain_s &s );
    domain_s( const char *s );

    //const char * Wild = "*";
    
    bool is_wild() const;
    
    //virtual int operator == ( const domain_s & ) const;
    //virtual int operator != ( const domain_s & ) const;
    //virtual int operator <  ( const domain_s & ) const;
    virtual int operator == ( const string & ) const;
    virtual int operator != ( const string & ) const;
    virtual int operator <  ( const string & ) const;

    friend int operator == ( const char *,     const domain_s & );
    friend int operator == ( const domain_s &, const char * );
    friend int operator == ( const domain_s &, const domain_s & );
    friend int operator != ( const char *,     const domain_s & );
    friend int operator != ( const domain_s &, const char * );
    friend int operator != ( const domain_s &, const domain_s & );

    };


class string;
d99 1
a99 1

d112 1
a112 1

@


1.10
log
@Exceptions added, not compiled
@
text
@d8 3
d54 6
a59 3
    virtual int operator == ( const domain_s & ) const;
    virtual int operator != ( const domain_s & ) const;
    virtual int operator <  ( const domain_s & ) const;
d63 1
d66 1
@


1.9
log
@Before exceptions.
@
text
@d8 3
@


1.8
log
@FripMail written
@
text
@d8 3
@


1.7
log
@wildness class
@
text
@d8 3
d83 9
a91 7
    int             zone() const {return zone_v;};
    int             net() const {return net_v;};
    int             node() const {return node_v;};
    int             point() const {return point_v;};
    const domain_s& domain() const {return domain_v;};
    
    void            node( int n ) { node_v = n;};
@


1.6
log
@*** empty log message ***
@
text
@d8 3
d37 2
d69 2
d125 37
@


1.5
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d18 3
a20 1
//#include "strng.h"
d24 3
d30 4
a33 3
    domain_s( const string &s )   : string( s ) {};
    domain_s( const domain_s &s ) : string( s ) {};
    domain_s( const char *s )     : string( s ) {};
d39 1
a63 2
    static bool     dom_eq(string d1, string d2);
    
d79 2
d92 1
a92 14
#if 0
    int operator    == ( const addr &a ) const
        {
        return 
            a.zone_v == zone_v &&
            a.net_v == net_v &&
            a.node_v == node_v &&
            a.point_v == point_v &&
            ((a.domain_v == domain_v) || dom_eq(a.domain_v, domain_v));
        }

    int operator    != ( const addr &a ) const
        { return !(*this == a ); }
#else
d95 3
a97 1
#endif
d101 2
a102 2
int operator == ( const char *a,     const domain_s &b )
    { return addr::dom_eq(a,b); }
d104 2
a105 2
int operator == ( const domain_s &a, const char *b )
    { return addr::dom_eq(a,b); }
d108 2
a109 2
int operator != ( const char *a,     const domain_s &b )
    { return addr::dom_eq(a,b) ? No : Yes; }
d111 2
a112 2
int operator != ( const domain_s &a, const char *b )
    { return addr::dom_eq(a,b) ? No : Yes; }
d118 2
@


1.4
log
@before virtualization
@
text
@d1 11
d28 3
a30 1
    friend int operator == ( const domain_s &, const domain_s & );
a32 1
    friend int operator != ( const domain_s &, const domain_s & );
d82 1
d92 1
a92 1
#if 0
d96 1
a101 3
int operator == ( const domain_s &a, const domain_s &b )
    { return addr::dom_eq(a,b); }

a107 3

int operator != ( const domain_s &a, const domain_s &b )
    { return addr::dom_eq(a,b) ? No : Yes; }
@


1.3
log
@before COW strings
@
text
@d15 2
d39 2
d59 6
d79 6
@


1.2
log
@beta 7
@
text
@d8 15
d33 1
a33 1
    string          domain_v;
d53 1
a53 1
    const string &  domain() const {return domain_v;};
d70 22
@


1.1
log
@Initial revision
@
text
@d21 1
d23 1
d44 1
d52 1
a52 1
            a.domain_v == domain_v;
@

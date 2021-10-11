head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.14.01.06.31;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.12.01.08.49;	author dz;	state Exp;
branches;
next	;


desc
@@


1.8
log
@clear
@
text
@#include "frip.h"
#include "flag.h"


#include <iterator.h>

flags::~flags()
    {
    }


flags::flags( const string &f )
    {
    string s = f;
    string one;
    while( !s.is_empty() )
        {
        s.parse( one, ',', s );
        one.ucase();
        (*this) += one;
        }
    }
    

flags::operator string () const
    {
    string s;
    bool first = Yes;
    
    set<string, less<string> >::iterator i;
    for (i = v.begin (); i != v.end (); i++)
        {
        if( !first ) s += ",";
        first = No;
        s += *i;
        }

    return s;
    }


int flags::operator == ( const flags &f ) const
    {
    set<string, less<string> >::iterator i;
    
    for (i = v.begin (); i != v.end (); i++)
        {
        if( !f.v.count( *i ) )
            return 0;
        }
    
    for (i = f.v.begin (); i != f.v.end (); i++)
        {
        if( !v.count( *i ) )
            return 0;
        }
    
    return 1;
    }



flags & flags::operator += ( string s )
    {
    s.ucase();
    // if(!v.find(&s))  we need not check since it's a set, not just vector
    v.insert( s );
    return *this;
    }

flags & flags::operator -= ( string s )
    {
    s.ucase();
    v.erase( s );
    return *this;
    }

bool flags::has( string s ) const
    {
    s.ucase();
    return (v.count(s) == 0) ? No : Yes;
    }


void flags::extract( string &s, char key )
    {
    s.strip_ws();
    if( s[0] == key )
        {
        string fs;
        s.parse( fs, s );
        *this = flags( fs.substr( 1 ) );
        }
    }


void flags::clear()
    {
    while( v.size() )
        v.erase( v.begin() );
    }









@


1.7
log
@Version 25
@
text
@d97 5
@


1.6
log
@Exceptions added, not compiled
@
text
@d18 1
a18 1
        parse( s, one, ',', s );
d91 1
a91 1
        parse( s, fs, s );
@


1.5
log
@Before exceptions.
@
text
@@


1.4
log
@Clean up of watcom classlib based code
@
text
@@


1.3
log
@beta 14
@
text
@a3 3
#if USE_STL
#else
#endif
a4 1
#if USE_STL
a5 3
#else
#include <wchiter.h>
#endif
a8 3
#if !USE_STL
    v.clear();
#endif
a12 4
#if USE_STL
#else
                                 : v(flag_hash)
#endif
a29 1
#if USE_STL
a36 12
#else
    WCValHashSetIter<string> i;
    
    i.reset( v );
    
    while( i.operator ++ )
        {
        if( !first ) s += ",";
        first = No;
        s += i.current();
        }
#endif
a43 1
#if USE_STL
a57 18
#else
    WCValHashSetIter<string> i;
    
    i.reset( v );
    while( i.operator ++ )
        {
        if( !f.v.contains( i.current() ) )
            return 0;
        }
    
    i.reset( f.v );
    while( i.operator ++ )
        {
        if( !v.contains( i.current() ) )
            return 0;
        }
#endif
    
a73 1
#if USE_STL
a74 3
#else
    v.remove( s );
#endif
a80 1
#if USE_STL
a81 3
#else
    return (v.contains(s) == NULL) ? No : Yes;
#endif
a98 9
#if !USE_STL
unsigned flag_hash( const string &s )
    {
    int l = s.length();
    int res = 0;
    while(l--) res += s[l];
    return res;
    }
#endif
@


1.2
log
@*** empty log message ***
@
text
@d3 8
d12 1
d16 1
d18 1
d22 5
a26 1
flags::flags( const string &f ) : v(flag_hash)
a38 9
/*
 for( int i = 0; i < v.entries(); i++ )
 {
 if( !first ) s += ",";
 first = No;
 s += (*(v[i]));
 }
 */

d43 10
d63 2
a64 1
    
d71 16
d102 1
d120 3
d124 1
d131 3
d135 1
d153 1
d161 2
@


1.1
log
@Initial revision
@
text
@d3 1
d5 7
a11 1
flags::flags( const string &f )
d23 10
d37 5
a41 1
    for( int i = 0; i < v.entries(); i++ )
d45 1
a45 1
        s += (*(v[i]));
d47 1
d50 5
d56 19
d78 2
a79 2
    if(!v.find(&s)) 
        v.insert( new string( s ));
d86 1
a86 3
    string *sp = v.
        remove( &s );
    if( sp != NULL ) delete sp;
d93 1
a93 1
    return (v.find(&s) == NULL) ? No : Yes;
d111 7
@

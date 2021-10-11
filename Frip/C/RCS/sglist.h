head	1.6;
access;
symbols
	Beta_12:1.2;
locks;
comment	@ * @;


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
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.12.05.04.25.03;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Exceptions added, not compiled
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	+- lists
 *
 *      $Log: sglist.h $
 *      Revision 1.5  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.4  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.3  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.2  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef SGLIST_H
#define SGLIST_H

#include "strng.h"
#include "addr.h"

#include <vector.h>

class stopgo
    {
public:
    enum sg { Stop, Go, Error }   kind;
    addr                          mask;
    
    stopgo( string );
    stopgo( const stopgo &s ) : mask(s.mask) { kind = s.kind; }
    stopgo()                                 { kind = Error;   }
        
    
    int operator == ( stopgo const &a ) const
        { return mask==a.mask && kind == kind; };
    };



class sglist
    {
    vector            <stopgo>     sg_v;

public:
    sglist( const sglist& s) : sg_v(s.sg_v) {}
    sglist() {}
        
    void    add_one( const string &s )
        {
        sg_v.push_back( stopgo( s ) );
        }
    
    void    add_all( string s );
    
    bool    match( const addr &r, bool default_mode ) const;
    
    int operator == ( const sglist &his ) const
        {
        return sg_v == his.sg_v;
        }
    };






#endif // SGLIST_H








@


1.5
log
@Before exceptions.
@
text
@d8 3
@


1.4
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.3
log
@C Set & STL support
@
text
@d8 3
a23 1
#if USE_STL
a24 3
#else
#include <wcvector.h>
#endif
a44 1
#if USE_STL
a45 3
#else
    WCPtrOrderedVector<stopgo>     sg_v;
#endif
a52 1
#if USE_STL
a53 3
#else
        sg_v.insert( new stopgo( s ) );
#endif
@


1.2
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d21 3
d25 1
d30 3
d34 2
a36 3
    enum sg { Stop, Go }   kind;
    addr                   mask;
    
d46 3
d50 1
d53 3
d58 3
d62 1
@


1.1
log
@Initial revision
@
text
@d1 11
@

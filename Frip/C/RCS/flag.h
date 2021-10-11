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
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.22.04.36.41;	author dz;	state Exp;
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
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Flags class
 *
 *      $Log: flag.h $
 *      Revision 1.7  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.6  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.5  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.4  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.3  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.2  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/01/12 01:08:49  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef FLAG_H
#define FLAG_H

#ifdef __WATCOMC__
//#pragma warning 621 9
#pragma warning 549 9
#pragma warning 297 9
#endif

#include "strng.h"
#include "addr.h"

#include <set.h>


class flags
    {
    set<string, less<string> >    v;
    
public:
    
    flags() {};

      //flags( const flags &f );
    flags( const string &f );
    ~flags();
    
    // NB! It will clean extracted part out of s
    void extract( string &s, char key );
    
    //flags & operator = (flags &f );
    
    operator string () const;
    
    flags & operator += ( string s );
    flags & operator -= ( string s );
    
    int operator == ( const flags &f ) const;
    
    bool has( string s ) const ;
    
    bool is_empty() const { return v.empty() /* ? Yes : No */ ; }
    void clear();
    };


#endif // FLAG_H
@


1.7
log
@Exceptions added, not compiled
@
text
@d8 3
d76 1
@


1.6
log
@Before exceptions.
@
text
@d8 3
@


1.5
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.4
log
@C Set & STL support
@
text
@d8 3
a36 1
#if USE_STL
a37 3
#else
#include <wchash.h>
#endif
a38 3
#if !USE_STL
unsigned flag_hash( const string & );
#endif
a41 1
#if USE_STL
a42 3
#else
    WCValHashSet<string>        v;
#endif
a45 1
#if USE_STL
d47 2
a48 4
#else
    flags() : v(flag_hash) {};
#endif
    //flags( const flags &f );
a65 1
#if USE_STL
a66 3
#else
    bool is_empty() const { return v.entries() == 0 ? Yes : No; }
#endif
@


1.3
log
@beta 14
@
text
@d8 3
d25 1
d29 1
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d22 3
a24 1
#pragma warning 621 9
d26 1
d28 4
d33 1
d35 1
d37 1
d41 5
a45 1
    WCValHashSet<string>   v;
d49 3
d53 1
d72 3
d76 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: rip.h $
d19 2
d22 1
a22 1
#include <wcvector.h>
d24 1
a24 4
// NB! To implement flags correctly, rip must not match
// db record, which has differrent - and + flags.
// This need forces us to use set, not a vector, as it is now, to
// keep flags in class flags.
d28 1
a28 1
    WCPtrSortedVector<string>   v;
d32 1
a32 1
    flags() {};
d35 1
d47 1
a47 1
    int operator == ( const flags &f ) const { return v == f.v; }
@

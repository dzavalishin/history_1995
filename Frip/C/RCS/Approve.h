head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	approve class
 *
 *      $Log: Approve.h $
 *      Revision 1.5  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.4  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.3  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.2  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.1  1996/01/13 09:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/


#ifndef APPROVE_H
#define APPROVE_H

#include "style.h"
#include "strng.h"
#include "addr.h"
#include "conf.h"

#include <set.h>

class app_elem
    {
public:
    addr  who;
    addr  what;
    
    app_elem( const addr & awho, const addr & awhat )
        : who( awho ), what( awhat ) {};

    app_elem() {};
    app_elem( const app_elem &a ) : who(a.who), what(a.what) {};
    
    app_elem & operator = ( const app_elem &a )
        { who = a.who; what = a.what; return *this; }
    int operator == ( const app_elem &a ) const
        { return who == a.who && what == a.what; }
    
    int operator <  ( const app_elem &a ) const
        { return who < a.who || ( who == a.who && what < a.what); }
    
    };


class approve
    {
    bool   done_load;
    
    void   load();
    void   load_file( const string &file );
    void   load_line( const string &line );
    
    void   insert( const app_elem & );
    bool   lookup( const app_elem & );
    
    set <app_elem, less<app_elem> >   set_v;
    
public:
    approve() { done_load = No; };
    approve( const approve &a ) : set_v(a.set_v), done_load(a.done_load) {}
    //~approve() { release(); }
    
    bool is_approved( const addr &who, const addr &what );
    };




#endif // APPROVE_H
@


1.5
log
@Exceptions added, not compiled
@
text
@d8 3
d70 1
a70 1
    bool   insert( const app_elem & );
@


1.4
log
@Before exceptions.
@
text
@d8 3
d63 3
a65 3
    bool   load();
    bool   load_file( const string &file );
    bool   load_line( const string &line );
@


1.3
log
@Clean up of watcom classlib based code
@
text
@d8 3
d45 2
a46 2
    operator = ( const app_elem &a )
        { who = a.who; what = a.what; }
@


1.2
log
@C Set & STL support
@
text
@d8 3
a27 1
#if USE_STL
a28 3
#else
#include <wchash.h>
#endif
a51 3
#if !USE_STL
unsigned app_elem_hash( const app_elem & );
#endif
a63 1
#if USE_STL
a64 3
#else
    WCValHashSet<app_elem>            set_v;
#endif
a66 1
#if USE_STL
a67 3
#else
    approve() : set_v(&app_elem_hash) { done_load = No; };
#endif
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: frip.h $
d25 3
d29 1
a39 1
    
d42 1
d48 3
d53 1
d55 1
d67 6
a72 1
    WCValHashSet<app_elem>   set;
d75 6
a80 1
    approve() : set(&app_elem_hash) { done_load = No; };
@

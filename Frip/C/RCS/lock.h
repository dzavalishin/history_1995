head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.04.08.18.43;	author dz;	state Exp;
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


1.5
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Lock class
 *
 *      $Log: lock.h $
 *      Revision 1.4  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
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


#ifndef LOCK_H
#define LOCK_H

#include "style.h"
#include "strng.h"
#include "conf.h"

class ilock
    {
    string name_v;
    bool   we_own_the_lock_v;
    int    handle;
        
public:
    bool  we_own() {return we_own_the_lock_v;};
    void  release();
    void  lock();
    void  wait( int time );  // Slip this time or less, if resource is freed
    
    ilock() {}
    ilock( const string &name ) : name_v(name) { lock(); };
    ~ilock() { release(); }
    };




#endif // LOCK_H
@


1.4
log
@Exceptions added, not compiled
@
text
@d8 3
d42 1
a42 1
    bool  lock();
@


1.3
log
@Before exceptions.
@
text
@d8 3
@


1.2
log
@C Set & STL support
@
text
@d8 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: frip.h $
d36 1
@

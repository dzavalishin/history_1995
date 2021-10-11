head	1.7;
access;
symbols
	Beta_12:1.2;
locks;
comment	@ * @;


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
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	+- lists class
 *
 *      $Log: sglist.C $
 *      Revision 1.6  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.5  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.4  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.3  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.2  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "sglist.h"

stopgo::stopgo( string s )
    {
    s.strip_leading_ws();
    
    kind=Go;
    if( s[0] == '-' )
        kind = Stop;
    if( s[0] == '-' || s[0] == '+' )
        s.strip_leading(1);
    
    mask = addr( s );
    
    //debug( string("got ") + (kind==Go?"Go  : ":"Stop: ") + ((string)mask) );
    }


bool sglist::match( const addr &r, bool default_mode ) const
    {
    stopgo::sg result = stopgo::Stop;
    if(default_mode) result = stopgo::Go;
    
    for( int i = 0; i < sg_v.size(); i++ )
        if( r.matches( sg_v[i].mask ) )
            result = sg_v[i].kind;

    return ( result == stopgo::Go ) ? Yes : No;
    }




void sglist::add_all( string s )
    {
    string l, r;
    
    while( s.parse( l, r ) )
        {
        if( !l.is_empty() )
            add_one( l );
        s = r;
        }
    
    if( !l.is_empty() )
        add_one( l );
    }




@


1.6
log
@Exceptions added, not compiled
@
text
@d8 3
d65 1
a65 1
    while( parse( s, l, r ) )
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
@d7 4
a10 1
 *      $Log: sglist.c $
a41 1
#if USE_STL
a44 5
#else
    for( int i = 0; i < sg_v.entries(); i++ )
        if( r.matches( sg_v[i]->mask ) )
            result = sg_v[i]->kind;
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
d15 1
a16 1
#include "frip.h"
d39 5
d47 1
@


1.1
log
@Initial revision
@
text
@d1 11
@

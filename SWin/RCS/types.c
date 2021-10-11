head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.27.21.12.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.27.07.26.39;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: basic classes impl.
 *
 *      $Log: types.c $
 *      Revision 1.5  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.4  1996/09/27 21:12:46  dz
 *      fixed intersection to not to generate
 *      negative widths.
 *
 *      Revision 1.3  1996/09/27 07:26:39  dz
 *      bugfixes
 *
 *      Revision 1.2  1996/09/20 14:40:18  dz
 *      update
 *
 *      Revision 1.1  1996/09/12 01:12:51  dz
 *      Initial revision
 *
 *
 *      
 *
\*/


#include "sw_types.h"



sw_point::sw_point( int x, int y )      : x_v(x),     y_v(y)
    {}

sw_point::sw_point( const sw_point &p ) : x_v(p.x_v), y_v(p.y_v)
    {}

sw_point::sw_point( void )              : x_v(0),     y_v(0)
    {}

  // -----------------------------------------------------------------

sw_size::sw_size( int x, int y )     : xs_v(x),      ys_v(y) {}
sw_size::sw_size( const sw_size &s ) : xs_v(s.xs_v), ys_v(s.ys_v) {}
sw_size::sw_size( void )             : xs_v(0),      ys_v(0) {}


// -----------------------------------------------------------------

sw_rect::sw_rect()
    {}



void sw_rect::summ( const sw_rect& r1, const sw_rect& r2 )
    {
    int xe = max( r1.xend(), r2.xend() );
    int ye = max( r1.yend(), r2.yend() );
    x( min( r1.x(), r2.x() ) );
    y( min( r1.y(), r2.y() ) );
    xend( xe );
    yend( ye );
    }


int sw_rect::intersection( const sw_rect& r1, const sw_rect& r2 )
    {
    if(
       r1.x() >= r2.xend() ||
       r1.y() >= r2.yend() ||
       r2.x() >= r1.xend() ||
       r2.y() >= r1.yend() ||
       r1.xs() <= 0 || r2.xs() <= 0 ||
       r1.ys() <= 0 || r2.ys() <= 0
      )
        return 0;

    int xe = min( r1.xend(), r2.xend() );
    int ye = min( r1.yend(), r2.yend() );
    x( max( r1.x(), r2.x() ) );
    y( max( r1.y(), r2.y() ) );
    xend( xe );
    yend( ye );

    return 1;
    }



@


1.5
log
@Before changing event class
@
text
@d8 3
d89 2
@


1.4
log
@fixed intersection to not to generate
negative widths.
@
text
@d8 4
@


1.3
log
@bugfixes
@
text
@d8 3
d67 3
a69 1
       r2.y() >= r1.yend()
d73 2
d77 2
a78 2
    xend( min( r1.xend(), r2.xend() ) );
    yend( min( r1.yend(), r2.yend() ) );
@


1.2
log
@update
@
text
@d8 3
d49 2
d53 2
a54 2
    xend( max( r1.xend(), r2.xend() ) );
    yend( max( r1.yend(), r2.yend() ) );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: win.c $
a41 3

void sw_rect::xend( int xe ) { xs(xe-x()); }
void sw_rect::yend( int ye ) { ys(ye-y()); }
@

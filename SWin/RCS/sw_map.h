head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.27.00.33.20;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: map classes
 *
 *      $Log: sw_map.h $
 *      Revision 1.4  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.3  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.2  1996/09/27 00:33:20  dz
 *      viobuf, optimization (sort of:)
 *
 *      Revision 1.1  1996/09/12 01:12:51  dz
 *      Initial revision
 *
 *
 *      
 *
\*/

#ifndef MAP_H
#define MAP_H

//#include "vector.h"
#include "sw_types.h"


class sw_abstract_win;

class sw_map : public sw_size
    {
    private:
        sw_abstract_win **v;
        bool             *transp;
        bool              rectangular;
        
        void allocme();
        void alloc_transp();
        void freeme();
    public:
        sw_map( const sw_size &s );
        ~sw_map();
        
        virtual void xs( int _ );
        virtual void ys( int _ );
        virtual void resize( const sw_size &s );

        void set_transparent( const sw_rect &r, bool t = Yes );
        void set_transparent( const sw_point &p, bool t = Yes );
        
        void clear( sw_abstract_win * );
        void load( const sw_map &m, const sw_point &p );

        int  belongs( const sw_abstract_win *win, int x, int y ) const;
        int  belong_count( const sw_abstract_win *win, int x, int y ) const;
        int  not_belong_count( const sw_abstract_win *win, int x, int y ) const;

        void dump() const;

        const sw_abstract_win * const * mapp() const { return v; }

        sw_abstract_win * win_at(const sw_point &p) { return *(v+p.x()+(p.y()*sw_size::xs())); }
    };




#endif // MAP_H


@


1.4
log
@Before changing event class
@
text
@d8 3
d38 2
d42 1
d51 3
@


1.3
log
@preliminary mouse support done
@
text
@d8 3
@


1.2
log
@viobuf, optimization (sort of:)
@
text
@d8 3
d53 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_win.h $
d45 1
d48 2
@

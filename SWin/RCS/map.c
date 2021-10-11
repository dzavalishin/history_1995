head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.28.22.58.52;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.27.00.33.20;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.26.22.07.12;	author dz;	state Exp;
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


1.8
log
@transparency, clock
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: map classes impl.
 *
 *      $Log: map.c $
 *      Revision 1.7  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
 *      Revision 1.6  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.5  1996/09/28 22:58:52  dz
 *      menu item classes written (wrong way yet)
 *
 *      Revision 1.4  1996/09/27 00:33:20  dz
 *      viobuf, optimization (sort of:)
 *
 *      Revision 1.3  1996/09/26 22:07:12  dz
 *      faster map rebuild. not much, though.
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


#include "sw_win.h"
#include "sw_map.h"

  // --------------------------- sw_map -----------------------------


void sw_map::allocme()
    {
    int elems = sw_size::xs() * sw_size::ys();
    v = new sw_abstract_win * [ elems ];
    if( v == 0 ) throw sw_Out_Of_Memory("sw_map::allocme");
      //transp = new bool [ elems ];
    transp = 0;
    clear(0);
    }

void sw_map::alloc_transp()
    {
    if( transp ) return;
    int elems = sw_size::xs() * sw_size::ys();
    transp = new bool [ elems ];
    if( transp == 0 ) throw sw_Out_Of_Memory("sw_map::alloc_transp");
    for( bool *bp = transp; bp < transp+elems; bp++ )
        *bp = No;
    }


void sw_map::freeme()
    {
    delete [] v;
    delete [] transp;
    v = 0;
    transp = 0;
    }

sw_map::sw_map( const sw_size &_s ) : sw_size(_s)
    {
    rectangular = Yes;
    allocme();
    }

sw_map::~sw_map()
    {
    freeme();
    }

void sw_map::resize( const sw_size &delta )
    {  freeme();  sw_size::resize( delta ); allocme();  }

void sw_map::xs( int _ )
    {  freeme();  sw_size::xs( _ );         allocme();  }

void sw_map::ys( int _ )
    {  freeme();  sw_size::ys( _ );         allocme();  }


void sw_map::set_transparent( const sw_rect &r, bool t )
    {
    alloc_transp();
    sw_rect is;
    if( !is.intersection( sw_rect( sw_point(0,0), *this ), r ) )
        return;
    
    int y2x1 = is.y()*sw_size::xs();
    //int y2x2 = ((is.y()-p.y())*m.sw_size::xs())-p.x();
    int y2x1_inc = sw_size::xs();
    //int y2x2_inc = m.sw_size::xs();
    int isyend = is.yend();
    int isxend = is.xend();
    
    for( int yi = is.y(); yi < isyend; yi++ )
        {
        for( int xi = is.x(); xi < isxend; xi++ )
            transp[xi+y2x1] = t;
        y2x1 += y2x1_inc;
        //y2x2 += y2x2_inc;
        }
    rectangular = No;
    }

void sw_map::set_transparent( const sw_point &p, bool t )
    {
    alloc_transp();
    if( p.x() >= 0 && p.x() < sw_size::xs() && p.y() >= 0 && p.y() < sw_size::ys() )
        {
        transp[p.x()+p.y()*sw_size::xs()] = t;
        rectangular = No;
        }
    }



void sw_map::clear( sw_abstract_win *w )
    {
    for( int i = sw_size::xs()*sw_size::ys(); i--; )
        v[i] = w;
    
    if(!rectangular)
        {
        // nullify transparent positions
        for( int i = sw_size::xs()*sw_size::ys(); i--; )
            if( transp[i] )
                v[i] = 0;
        }
    }

void sw_map::load( const sw_map &m, const sw_point &p )
    {
    sw_rect is;

    if( !is.intersection(
                         sw_rect( sw_point(0,0), *this ),
                         sw_rect( p, m )
                        ) )
        return;

    int y2x1 = is.y()*sw_size::xs();
    int y2x2 = ((is.y()-p.y())*m.sw_size::xs())-p.x();
    int y2x1_inc = sw_size::xs();
    int y2x2_inc = m.sw_size::xs();
    int isyend = is.yend();
    int isxend = is.xend();
    
    for( int yi = is.y(); yi < isyend; yi++ )
        {
        for( int xi = is.x(); xi < isxend; xi++ )
            {
            sw_abstract_win *wp = m.v[xi+y2x2];
            // check for zero. support for non-rectangular maps. (maps with holes)
            if( wp && v[xi+y2x1] )
                v[xi+y2x1] = wp;
            }
        y2x1 += y2x1_inc;
        y2x2 += y2x2_inc;
        }
    }

void sw_map::dump() const
    {
    cerr.flags( ios::hex );
    cerr.width( 6 );
    cerr << "sw_map::dump ->\n";
    for( int yi = 0; yi < sw_size::ys(); yi++ )
        {
        cerr << " ";
        for( int xi = 0; xi < sw_size::xs(); xi++ )
            cerr << (int)v[xi+(yi*sw_size::xs())] << " ";
        cerr << "\n";
        }
    }

int sw_map::belongs( const sw_abstract_win *w, int x, int y ) const
    {
    return w == v[x+(y*sw_size::xs())];
    }

int sw_map::belong_count( const sw_abstract_win *w, int x, int y ) const
    {
    int count = 0;
    sw_abstract_win **vp = v + x + (y*sw_size::xs());
    int max = sw_size::xs()-x;
    while( max-- && w == *vp++)
        count++;
    return count;
    }

int sw_map::not_belong_count( const sw_abstract_win *w, int x, int y ) const
    {
    int count = 0;
    sw_abstract_win **vp = v + x + (y*sw_size::xs());
    int max = sw_size::xs()-x;
    while( max-- && w != *vp++)
        count++;
    return count;
    }

@


1.7
log
@Nonrectangular (partially transparent) windows
@
text
@d8 3
d57 2
@


1.6
log
@Before changing event class
@
text
@d8 3
d40 14
a53 4
    int bytes = sw_size::xs() * sw_size::ys();
    v = new sw_abstract_win * [ bytes ];
    if( v == NULL ) throw sw_Out_Of_Memory("sw_map::allocme");
    clear(NULL);
d56 1
d60 3
a62 1
    v = NULL;
d67 1
d86 35
d126 8
d156 6
a161 1
            v[xi+y2x1] = m.v[xi+y2x2];
@


1.5
log
@menu item classes written (wrong way yet)
@
text
@d8 3
a85 1
#if 1
a99 6
#else
    for( int yi = is.y(); yi < is.yend(); yi++ )
        for( int xi = is.x(); xi < is.xend(); xi++ )
            v[xi+(yi*sw_size::xs())] =
            m.v[xi-p.x()+((yi-p.y())*m.sw_size::xs())];
#endif
@


1.4
log
@viobuf, optimization (sort of:)
@
text
@d8 3
a104 19

/*
void sw_map::mark( int index, sw_win &w )
    {
    int xbegin = w.x();
    int xend = w.x() + w.xs();
    if( xend > s.xs() ) xend = s.xs();
    
    int ybegin = w.y();
    int yend = w.y() + w.ys();
    if( yend > s.ys() ) yend = s.ys();
    
    for( int yi = ybegin; yi < yend; yi++ )
        for( int xi = xbegin; xi < xend; xi++ )
            v[xi+(yi*s.xs())] = index;
    }

*/

@


1.3
log
@faster map rebuild. not much, though.
@
text
@d8 3
d147 10
@


1.2
log
@update
@
text
@d8 3
d141 3
a143 1
    while( x < sw_size::xs() && w == v[x++ +(y*sw_size::xs())])
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: win.c $
d74 16
d94 1
@

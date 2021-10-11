head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.11.18.16.16.26;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.15.13.12.27;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.01.47.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: basic types
 *
 *      $Log: sw_types.h $
 *      Revision 1.9  1996/11/18 16:16:26  dz
 *      Event class now has no type field/method, but
 *      has info() == KeyPress and delivery mode == Mouse.
 *      If event::mouse() is nonzero, pos is valid.
 *
 *      Revision 1.8  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.7  1996/11/15 16:29:28  dz
 *      mouse event generator written
 *
 *      Revision 1.6  1996/11/15 13:12:27  dz
 *      John Gladkih's additions
 *
 *      Revision 1.5  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.4  1996/09/20 14:40:18  dz
 *      update
 *
 *      Revision 1.3  1996/09/12 01:12:51  dz
 *      works a little
 *
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
 *      Revision 1.1  1996/09/09 01:47:48  dz
 *      Initial revision
 *
 *      
 *
\*/



#ifndef SW_TYPES_H
#define SW_TYPES_H

#include "sw_except.h"


class sw_point
    {
    protected:
        int x_v, y_v;

    public:
        sw_point( int x, int y );      // : x_v(x),     y_v(y) {}
        sw_point( const sw_point &p ); // : x_v(p.x_v), y_v(p.y_v) {}
        sw_point( void );              // : x_v(0),     y_v(0) {}
            
        int x() const { return x_v; }
        int y() const { return y_v; }

        virtual void x( int _ ) { x_v = _; }
        virtual void y( int _ ) { y_v = _; }

        virtual void move( int x, int y ) { x_v += x; y_v += y; }
        virtual void home() { move(0,0); }

        int operator == ( const sw_point &r ) const { return x_v == r.x_v && y_v == r.y_v; }
        
    };

class sw_size
    {
    protected:
        int xs_v, ys_v;
    
    
    public:
        sw_size( int x, int y );     //: xs_v(x),      ys_v(y) {}
        sw_size( const sw_size &s ); //: xs_v(s.xs_v), ys_v(s.ys_v) {}
        sw_size( void );             //: xs_v(0),      ys_v(0) {}

        int xs() const { return xs_v; }
        int ys() const { return ys_v; }
        
        virtual void xs( int _ ) { xs_v = _; }
        virtual void ys( int _ ) { ys_v = _; }
        
        virtual void resize( const sw_size &s ) { xs_v += s.xs_v; ys_v += s.ys_v; }
    };


class sw_rect : public sw_point, public sw_size
    {
    public:
        sw_rect(); // {}

        sw_rect( const sw_rect &r ) : sw_point(r), sw_size(r) {}

        sw_rect( const sw_point &p, const sw_size &s )
            : sw_point(p), sw_size(s) {}

        //sw_rect( const sw_size &s ) : sw_point(0,0), sw_size(s) {}
        
        sw_rect( int x, int y, int xs, int ys )
            : sw_point(x,y), sw_size(xs,ys) {}

        virtual void xend( int xe ) { xs(xe-x()); }
        virtual void yend( int ye ) { ys(ye-y()); }

        virtual int xend() const { return x()+xs(); }
        virtual int yend() const { return y()+ys(); }
        
        virtual void cut( sw_size s )
            {
            if( x() < 0 ) { resize( sw_size(x(), 0) ); x(0); }
            if( y() < 0 ) { resize( sw_size(0, y()) ); y(0); }
            if( xend() > s.xs() ) xend(s.xs());
            if( yend() > s.ys() ) yend(s.ys());
            if( x() >= s.xs() ) { xs(0); x(s.xs()-1); }
            if( y() >= s.ys() ) { ys(0); y(s.ys()-1); }
            }

        virtual void summ( const sw_rect& r1, const sw_rect& r2 );
        virtual int intersection( const sw_rect& r1, const sw_rect& r2 );

        int includes( const sw_point& p );
        
    };

inline int sw_rect::includes( const sw_point& p )
    {
    return p.x() >= x() && p.y() >= y() && p.x() <= xend() && p.y() <= yend();
    }


class sw_color
    {
    unsigned char v;
    public:
        enum color_name
            {
            Black, Blue, Green, Cyan,
            Red, Magenta, Brown, LightGray,
            DarkGray, LightBlue, LightGreen, LightCyan,
            LightRed, LightMagenta, Yellow, White
            };

        unsigned char value() const { return v; }
        
        sw_color( enum color_name n ) { v = (char)n; }
        sw_color( enum color_name f, enum color_name b )
            { v = ((char)f) | (((unsigned char)b) << 4); }
        sw_color( const sw_color &n ) { v = n.v; }

        sw_color()
            { v = ((char)LightGray) | (((unsigned char)Black) << 4); }

        void back(unsigned char i) { v |= ((i & 0x0F) << 4); }
        void fore(unsigned char i) { v |= (i & 0x0F); }

        char back(void) const      { return (v >> 4) & 0x0F; }
        char fore(void) const      { return v & 0x0F; }

        // John Gladkin's addition
        sw_color( int attrib ) { v = (unsigned char)attrib; }   //jg
        sw_color( char attrib ) { v = (unsigned char)attrib; }   //jg
        sw_color( int f, int b )      //jg
            { v = ((char)f) | (((unsigned char)b) << 4); }
        sw_color( char f, char b )      //jg
            { v = ((char)f) | (((unsigned char)b) << 4); }
        int operator == ( sw_color& t ) { return v==t.v; }   //jg

    };









#endif // SW_TYPES_H
@


1.9
log
@Event class now has no type field/method, but
has info() == KeyPress and delivery mode == Mouse.
If event::mouse() is nonzero, pos is valid.
@
text
@d8 5
d126 2
d131 4
@


1.8
log
@Before changing event class
@
text
@d8 3
d63 1
a63 1
        int operator == ( const sw_point &r ) { return x_v == r.x_v && y_v == r.y_v; }
@


1.7
log
@mouse event generator written
@
text
@d8 3
d58 1
a58 1
        virtual void home() { x_v = y_v = 0; }
@


1.6
log
@John Gladkih's additions
@
text
@d8 3
d56 2
@


1.5
log
@log control written
@
text
@d8 3
d142 10
@


1.4
log
@update
@
text
@d8 3
d134 5
@


1.3
log
@works a little
@
text
@d8 3
d86 2
a87 2
        virtual void xend( int xe );// { xs(xe-x()); }
        virtual void yend( int ye );// { ys(ye-y()); }
@


1.2
log
@update
@
text
@d8 3
d25 1
d32 3
a34 3
        sw_point( int x, int y )      : x_v(x),     y_v(y) {}
        sw_point( const sw_point &p ) : x_v(p.x_v), y_v(p.y_v) {}
        sw_point( void )              : x_v(0),     y_v(0) {}
d39 2
a40 2
        void x( int _ ) { x_v = _; }
        void y( int _ ) { y_v = _; }
d54 3
a56 3
        sw_size( int x, int y )     : xs_v(x),      ys_v(y) {}
        sw_size( const sw_size &s ) : xs_v(s.xs_v), ys_v(s.ys_v) {}
        sw_size( void )             : xs_v(0),      ys_v(0) {}
d61 2
a62 2
        void xs( int _ ) { xs_v = _; }
        void ys( int _ ) { ys_v = _; }
d64 1
a64 1
        virtual void resize( int x, int y ) { xs_v += x; ys_v += y; }
d71 1
a71 1
        sw_rect() {}
d78 1
a78 2
        sw_rect( const sw_size &s )
            : sw_point(0,0), sw_size(s) {}
d83 2
a84 2
        int xend() { return x()+xs(); }
        int yend() { return y()+ys(); }
d86 4
a89 4
        void xend( int xe ) { xs(xe-x()); }
        void yend( int ye ) { ys(ye-y()); }

        void cut( sw_size s )
d91 2
a92 2
            if( x() < 0 ) { resize( x(), 0 ); x(0); }
            if( y() < 0 ) { resize( 0, y() ); y(0); }
d99 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Tic.c $
d32 2
a33 2
        int x() { return x_v; }
        int y() { return y_v; }
d51 1
a51 1
        sw_size( const sw_size &s ) : xs_v(s.xs_v), ys_v(p.ys_v) {}
d54 2
a55 2
        int xs() { return xs_v; }
        int ys() { return ys_v; }
d74 3
d79 18
d103 1
a103 1
    char v;
d105 1
a105 3
        unsigned char value() { return v; }

        enum
d112 11
@

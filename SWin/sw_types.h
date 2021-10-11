/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: basic types
 *
 *      $Log: sw_types.h $
 *      Revision 1.10  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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

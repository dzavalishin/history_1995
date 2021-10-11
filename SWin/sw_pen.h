/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: window classes
 *
 *      $Log: sw_pen.h $
 *      Revision 1.2  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.1  1996/09/12 01:12:51  dz
 *      Initial revision
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

#include "sw_win.h"



class sw_pen : public sw_point
    {
    protected:
        sw_color col_v;
        sw_abstract_win &w;
        
    public:
        sw_pen( sw_abstract_win &_w ) : w(_w) { x( 0 ); y( 0 ); }
        
        void color( sw_color _ ) { col_v = _; }
        sw_color color( void ) { return col_v; }
        
        virtual void put( char c )        { w.put( (sw_point&)*this, col_v, c ); }
        virtual void put( const char *s ) { w.put( (sw_point&)*this, col_v, s ); }
        
        virtual void fill( char c )       { w.fill( col_v, c ); }
    };



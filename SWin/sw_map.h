/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: map classes
 *
 *      $Log: sw_map.h $
 *      Revision 1.5  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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



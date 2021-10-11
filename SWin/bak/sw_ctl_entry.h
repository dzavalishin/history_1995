/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes: entry fields
 *
 *      $Log: sw_controls.h $
 *
 *
 *      
 *
\*/

#include "sw_win.h"
#include <vector.h>

class sw_entry_base : public sw_win
    {
    protected:
        int                  pos_v;     // Current pos
        int                  shift_v;

        virtual int          size() const = 0;
        virtual void         display_line( int line ) = 0;

        virtual void         refresh(const sw_rect & a);

        virtual void         set_pos(int np);

        virtual bool         event_preprocessor( const sw_event &e );

        virtual void         prev() { set_pos( pos_v-1 ); }
        virtual void         next() { set_pos( pos_v+1 ); }
        virtual void         pgup() { set_pos( pos_v-(ys()-2) ); }
        virtual void         pgdn() { set_pos( pos_v+(ys()-2) ); }
        virtual void         home() { set_pos( 0 ); }
        virtual void         end()  { set_pos( size()-1 ); }
        
        sw_entry_base( sw_abstract_win &parent, const sw_rect &rect  )
            : pos_v(0), shift_v(0), sw_win( parent, rect )
            {}
    };

class sw_simple_list : public sw_list_base
    {
    protected:
        vector <string>    text_v;
        sw_color           lcolor_v, scolor_v;
    public:
        sw_simple_list( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc )
            : sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc)
            {}
        
        virtual int          size() const { return text_v.size(); }

        virtual void         add( const string & s, int pos = -1 );
        virtual void         del( int pos );
        virtual void         clear();
        virtual void         display_line( int line );

    };




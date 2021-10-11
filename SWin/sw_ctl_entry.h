/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes: entry fields
 *
 *      $Log: sw_ctl_entry.h $
 *      Revision 1.6  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.5  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.4  1996/10/07 19:40:27  dz
 *      new 3d entry fields
 *
 *      Revision 1.3  1996/10/07 11:15:09  dz
 *      3d entry fields, wrong way
 *
 *      Revision 1.2  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.1  1996/09/20 14:40:18  dz
 *      Initial revision
 *
 *
 *
 *      
 *
\*/

#include "sw_controls.h"
#include <vector.h>

class sw_entry_base : public sw_control
    {
    protected:
        int                  pos_v;     // Current pos
        int                  shift_v;
        
        virtual int          size() const = 0;
        virtual void         display_part( int from, int to ) = 0;
        
        virtual void         refresh(const sw_rect & a);

        virtual void         set_pos(int np);
        
        virtual bool         event_preprocessor( const sw_event &e );
        
        virtual void         prev() { set_pos( pos_v-1 ); }
        virtual void         next() { set_pos( pos_v+1 ); }
        virtual void         prev_word() = 0;
        virtual void         next_word() = 0;
        virtual void         home() { set_pos( 0 ); }
        virtual void         end()  { set_pos( size() ); }

        virtual void         insert_here( string s ) = 0;
        virtual void         delete_here() = 0;
        
        sw_entry_base( sw_abstract_win &parent, const sw_rect &rect  )
            : pos_v(0), shift_v(0), sw_control( parent, rect )
            {}
    };


class sw_entry : public sw_entry_base
    {
    protected:
        string             text_v;
        sw_color           lcolor_v, scolor_v /*, shadow_v */;
    public:
        sw_entry( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc /*, sw_color shadow_c */ );

        //virtual void           refresh(const sw_rect&a);

        virtual int            size() const { return text_v.length(); }
        
        virtual void           value( const string & s ) { text_v = s; redisplay(); }
        virtual const string & value() const             { return text_v; }
        virtual void           clear()                   { text_v.clear(); redisplay(); }

        virtual void           display_part( int from, int to );
        virtual void           redisplay() { display_part( 0, size() ); }
        virtual void           focus_change_refresh() { redisplay(); }
        virtual void           insert_here( string s );
        virtual void           delete_here();

        virtual void           prev_word();
        virtual void           next_word();
    };

class sw_3d_shadow : public sw_control
    {
    sw_color           shadow_v;
    string             lower_line;

    static sw_rect extend_rect( const sw_rect &r );
    
    public:
        sw_3d_shadow( sw_abstract_win &parent, const sw_rect &r, sw_color shadow_c )
            :
        sw_control( parent, extend_rect(r) ), shadow_v(shadow_c), lower_line('ß',r.xs())
            {
            focusable_v = No;
            }

        virtual void refresh(const sw_rect&); // redisplays self and children
    };

class sw_3d_entry : public sw_entry
    {
    private:
        sw_3d_shadow sh;
    public:
        sw_3d_entry( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc, sw_color shadow_c )
            :
        sw_entry( parent, rect, lc, sc /*, shadow_c */ ),
        sh( parent, rect, shadow_c )
            {
            sh.below(*this);
            }
    };



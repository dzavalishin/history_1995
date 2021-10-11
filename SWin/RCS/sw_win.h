head	1.18;
access;
symbols;
locks;
comment	@ * @;


1.18
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.10.07.19.40.27;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.10.03.07.43.57;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.10.02.11.01.11;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.27.04.54.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.26.22.07.12;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.26.06.06.18;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.21.13.26.57;	author dz;	state Exp;
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


1.18
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: window classes
 *
 *      $Log: sw_win.h $
 *      Revision 1.17  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.16  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.15  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.14  1996/10/07 19:40:27  dz
 *      new 3d entry fields
 *
 *      Revision 1.13  1996/10/03 07:43:57  dz
 *      buttons
 *
 *      Revision 1.12  1996/10/02 11:01:11  dz
 *      focusable
 *
 *      Revision 1.11  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.10  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.9  1996/09/27 04:54:43  dz
 *      fast fill - now we're passing fill data
 *      down to device and do actual fill there.
 *      Prepared to do same thing with border, but
 *      not sure yet I really will.
 *
 *      Revision 1.8  1996/09/26 22:07:12  dz
 *      faster map rebuild. not much, though.
 *
 *      Revision 1.7  1996/09/26 20:46:52  dz
 *      menus basically done
 *
 *      Revision 1.5  1996/09/21 13:26:57  dz
 *      generalized event model
 *
 *      Revision 1.3  1996/09/12 01:12:51  dz
 *      works a little
 *
 *      Revision 1.1  1996/09/09 01:47:48  dz
 *      Initial revision
 *
 *      
 *
\*/

#ifndef SW_WIN_H
#define SW_WIN_H

class sw_win;
class sw_abstract_win;
inline void destroy(sw_win**) {}
inline void destroy(sw_abstract_win**) {}


#include <vector.h>

#include "sw_types.h"
#include "sw_map.h"
#include "sw_event_handler.h"


  // -----------------------------------------------------------------------
class sw_abstract_win;
class sw_win;

  // see border method in win.c, values are used there
enum sw_border_t
    {
    No_Border = 0,
    Single_Border = 1, Double_Border  = 2,
    SHDV_Border   = 3, DHSV_Border    = 4,
    Block_Border  = 5, Dot_Border     = 6
    };

class sw_win_ops
    {
    protected:
        
    public:
        sw_win_ops();
          // ------ end-user methods --------
          // both advance given point
        virtual void put( const sw_point &p, sw_color c, char ch ) = 0;
        virtual void put( const sw_point &p, sw_color c, const char *s ) = 0;
        virtual void put( const sw_rect &p, const char *char_and_attr_map ) = 0;

        virtual void fill( sw_color col, char c, sw_rect r, bool excl_border = No ) = 0;
        virtual void border( sw_color col, sw_border_t t ) = 0;

        // redisplays this rec. of self and children
        virtual void refresh( const sw_rect &sq) = 0; 
        
          // ------ mostly internal use methods --------
        
          // displays a videomemory-style array - used
          // by child windows only!
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &p, const char *char_and_attr_map ) = 0;
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r ) = 0;
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t ) = 0;
        
    };

class sw_device;
class sw_win;

class sw_abstract_win
    :
public sw_size,
public sw_win_ops,
public sw_event_handler
    {
    private:
        vector <sw_win *> child;
        typedef vector<sw_win *>::iterator child_iterator;

        virtual bool pass_broadcast_to_children( const sw_event &e );
        //virtual bool pass_event_to_position( const sw_event &e );

        virtual void erase_child( const sw_win &w );
        virtual child_iterator find_child( const sw_win &w );

    protected:
        sw_map            map;
        
        sw_device &       device;
        friend            sw_win; // it needs access to our device member

        bool              focusable_v;

        sw_point          absolute_positon_v; // pos in device's coords. Used to convert mouse event coords

          // this method is called by parent window
          // with absolute coordinates of this window given.
          // It calls same child window methods in turn
        virtual void      calculate_absolute_window_posiotion() = 0;
        const sw_point &  absolute_positon() { return absolute_positon_v; }

        virtual int       our_mouse_event( sw_point &out, const sw_event &in ) const; // boolean
            /*
            {
            if( !in.mouse() ) return 0; // pos is not valid
            out.x( in.pos().x() - absolute_positon_v.x() );
            out.y( in.pos().y() - absolute_positon_v.y() );
            return ((sw_rect*)this)->includes( out );
            }
            */
    public:
          // rebuilds our map and map of child windows
        virtual void do_rebuild_map(const sw_rect&); 
        virtual void do_rebuild_map_to_root(const sw_rect&);
          // Entry point
        virtual void rebuild_map(const sw_rect&); // { do_rebuild_map_to_root(); }
        virtual void move_to_win( sw_point &p ) const = 0;

    public:
        sw_abstract_win( const sw_size &s, sw_device &dev );
        virtual ~sw_abstract_win();

        void attach_child( sw_win &w );
        void detach_child( sw_win &w );

        virtual void resize( const sw_size &s );

        void top_child( sw_win &w ); // move child to top
        void bot_child( sw_win &w ); // move child to bottom
        void above_child( sw_win &ch, const sw_win &ref );
        void below_child( sw_win &ch, const sw_win &ref );

        void get_focus_iterator(child_iterator &i);
        void increment_focus_iterator(child_iterator &i);
        void decrement_focus_iterator(child_iterator &i);
            
        virtual void focus_to_next_child();
        virtual void focus_to_prev_child();
        virtual void focus_to_left_child();
        virtual void focus_to_right_child();
        virtual void focus_to_up_child();
        virtual void focus_to_down_child();

        // return deepest focused window
        sw_abstract_win * deep_focus()
            {
            return focused_child() ? focused_child()->deep_focus() : this;
            }

        virtual void push_focus();
        virtual void pop_focus();

        virtual bool focusable() { return focusable_v; }
        
        virtual void put( const sw_point &p, sw_color c, char ch );
        virtual void put( const sw_point &p, sw_color c, const char *s );
        virtual void put( const sw_rect &p, const char *char_and_attr_map );
        virtual void fill( sw_color col, char c, sw_rect r, bool excl_border = No );
        virtual void border( sw_color col, sw_border_t t );
        
        virtual void refresh(const sw_rect&); // redisplays self and children
        
          // ------ mostly internal use methods --------
        
          // displays a videomemory-style array - used
          // by child windows only!
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &p, const char *char_and_attr_map ) = 0;
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r ) = 0;
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t ) = 0;
        virtual void focus_tree() {};


        void dump() { map.dump(); }

    };


class sw_win : public sw_abstract_win, public sw_point
    {
    protected:
        friend class sw_border; // needs access to w
        sw_abstract_win &w; // parent win
        
        virtual void do_rebuild_map_to_root(const sw_rect&);
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &p, const char *char_and_attr_map );
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r );
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t );

    public:
        virtual void move_to_win( sw_point &p ) const { p.move( x(), y() ); }
        virtual void calculate_absolute_window_posiotion();
        virtual int  our_mouse_event( sw_point &out, const sw_event &in ) const; // boolean
        
    public:
        sw_win( sw_abstract_win &parent, sw_rect pos_size );
        virtual ~sw_win();

        const sw_abstract_win &parent() const { return w; }
        
        virtual void move( int x, int y );
        virtual void resize( const sw_size &s );

        virtual void top(); // bring this window to the top of all siblings
        virtual void bot();
        virtual void above( const sw_win & );
        virtual void below( const sw_win & );

        // BUG!
        // There should be real visibility status. Here is just a workaround.
        virtual void visible( bool yn) { if( yn ) top(); else bot(); }

        virtual void focus(); //      { w.focused_child( this ); }
        virtual void focus_tree(); // { w.focused_child( this ); w.focus_tree(); }
        
        operator     sw_rect () const {return sw_rect((sw_point&)*this,(sw_size&)*this);}
    };

class sw_bordered_win : public sw_win
    {
    private:
        sw_border_t  border_type_v;
        sw_color     border_color_v;
        sw_color     fill_color_v;
        char         fill_char_v;
            
    public:
        sw_bordered_win(
                        sw_abstract_win &parent,
                        sw_rect pos_size,
                        sw_border_t border_type = Single_Border,
                        sw_color border_color = sw_color::Yellow,
                        sw_color fill_color = sw_color::Cyan,
                        char fill_char = '°'
                       );
        virtual ~sw_bordered_win();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };

class sw_border : public sw_win
    {
    private:
        sw_border_t  border_type_v;
        sw_color     border_color_v;
        //sw_color     fill_color_v;
        //char         fill_char_v;
        
    public:
        sw_border(
                  sw_win &parent,
                  //sw_rect pos_size,
                  sw_border_t border_type = Single_Border,
                  sw_color border_color = sw_color::Yellow
                    //sw_color fill_color = sw_color::Cyan,
                    //char fill_char = '°'
                 );
        virtual ~sw_border();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };



#endif // SW_WIN_H
@


1.17
log
@Before changing event class
@
text
@d8 3
d150 9
d229 1
d240 1
d263 1
a263 1
        operator     sw_rect () {return sw_rect((sw_point&)*this,(sw_size&)*this);}
d284 21
@


1.16
log
@preliminary mouse support done
@
text
@d8 3
a41 3
 *      Revision 1.6  1996/09/26 06:06:18  dz
 *      update
 *
a44 3
 *      Revision 1.4  1996/09/20 14:40:18  dz
 *      update
 *
a47 3
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
d138 9
a146 1
        
d217 1
a217 1
        sw_abstract_win &w;
d226 1
@


1.15
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
d132 2
d187 1
a187 1
        bool         focusable() { return focusable_v; }
@


1.14
log
@new 3d entry fields
@
text
@d8 3
d208 1
a208 1
    private:
a209 3

    public:
        virtual void move_to_win( sw_point &p ) const { p.move( x(), y() ); }
a210 1
    protected:
d216 3
a218 1
    
@


1.13
log
@buttons
@
text
@d8 3
d125 2
d155 2
d227 1
a227 1
        virtual void top();
d229 2
@


1.12
log
@focusable
@
text
@d8 3
a65 4

  // ----------------------------- CONSTANTS -------------------------------

const sw_max_width  = 512; // BUG! We should get rid of this limit
@


1.11
log
@dialogs, basically written
@
text
@d8 3
d120 1
d123 1
a123 1
        
d130 2
d152 4
d172 2
@


1.10
log
@log control written
@
text
@d8 3
d145 16
@


1.9
log
@fast fill - now we're passing fill data
down to device and do actual fill there.
Prepared to do same thing with border, but
not sure yet I really will.
@
text
@d8 6
d103 2
d114 1
a114 1
        
d119 3
a122 2
          // Put to the place we're displayed in
        //virtual void put_up( const sw_rect &p, const char *char_and_attr_map ) = 0;
d132 1
a132 1
        sw_abstract_win( const sw_size &s );
d190 2
a191 2
        virtual void top(); // { w.top_child( *this ); }
        virtual void bot(); // { w.bot_child( *this ); }
@


1.8
log
@faster map rebuild. not much, though.
@
text
@d8 3
d92 2
d147 2
d168 3
a170 1
        //virtual void put_up( const sw_rect &p, const char *char_and_attr_map )            { w.put_cw( *this, p, char_and_attr_map ); }
@


1.7
log
@menus basically done
@
text
@d8 3
d111 2
a112 2
        virtual void do_rebuild_map(); 
        virtual void do_rebuild_map_to_root();
d114 1
a114 1
        virtual void rebuild_map(); // { do_rebuild_map_to_root(); }
d159 1
a159 1
        virtual void do_rebuild_map_to_root();
@


1.6
log
@update
@
text
@d8 3
d171 4
@


1.5
log
@generalized event model
@
text
@d8 3
a95 3
          // wfocus_changer is called from sw_event_handler::event
          // to pass event to focus_changer class
        //virtual bool wfocus_changer( const sw_event &e ) { return focus_changer(e); }
@


1.4
log
@update
@
text
@d8 3
d96 1
a96 1
        virtual void pass_broadcast_to_children( const sw_event &e );
@


1.3
log
@works a little
@
text
@d8 3
d24 8
a31 1
#include "vector.h"
d34 1
d45 8
a52 1
enum sw_border_t { No_Border, Single_Border, Double_Border };
d81 5
a85 1
class sw_abstract_win : public sw_size, public sw_win_ops
d90 5
d133 1
d157 3
d163 2
a164 2
        virtual void top() { w.top_child( *this ); }
        virtual void bot() { w.bot_child( *this ); }
d166 4
a169 1
        operator sw_rect () {return sw_rect((sw_point&)*this,(sw_size&)*this);}
d192 1
@


1.2
log
@update
@
text
@d8 3
d18 3
d23 1
d34 2
d44 3
a46 2
        virtual void put( sw_point &p, sw_color c, char ch ) = 0;
        virtual void put( sw_point &p, sw_color c, const char *s ) = 0;
d48 2
a49 1
        virtual void fill( sw_color col, char c ) = 0;
d51 2
a52 1
        virtual void refresh() = 0; // redisplays self and children
d58 1
a58 1
        virtual void put_cw( const sw_win &who, const sw_rect &p, const char *char_and_attr_map ) = 0;
a61 15
class sw_map
    {
    private:
        char *v;
        sw_size s;
        void allocme();
        void freeme();
    public:
        sw_map( const sw_size &s );
        ~sw_map();

        void resize( const sw_size &s );
        void clear();
        void mark( int i, sw_win &w );
    };
a66 1
        sw_map            map;
d69 11
a79 2
        // Put to the place we're displayed in
        virtual void put_up( const sw_rect &p, const char *char_and_attr_map ) = 0;
d88 1
a88 3
        virtual void rebuild_map(); // rebuilds map of child windows

        virtual void resize( int x, int y );
d93 5
a97 3
        virtual void put( sw_point &p, sw_color c, char ch );
        virtual void put( sw_point &p, sw_color c, const char *s );
        virtual void fill( sw_color col, char c );
d99 1
a99 1
        virtual void refresh(); // redisplays self and children
d105 5
a109 1
        virtual void put_cw( const sw_win &who, const sw_rect &p, const char *char_and_attr_map );
d117 8
a124 3
    
        virtual void put_up( const sw_rect &p, const char *char_and_attr_map )
            { w.put_cw( *this, p, char_and_attr_map ); }
d130 1
a130 1
        virtual void resize( int x, int y );
d135 1
d138 1
a138 2

class sw_pen : public sw_point
d140 6
a145 4
    protected:
        sw_color col_v;
        sw_abstract_win &w;
        
d147 10
a156 9
        sw_pen( sw_abstract_win &_w ) : w(_w) { x( 0 ); y( 0 ); }
        
        void color( sw_color _ ) { col_v = _; }
        sw_color color( void ) { return col_v; }

        virtual void put( char c )        { w.put( (sw_point&)*this, col_v, c ); }
        virtual void put( const char *s ) { w.put( (sw_point&)*this, col_v, s ); }

        virtual void fill( char c )       { w.fill( col_v, c ); }
d160 1
a160 1

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Tic.c $
d19 8
d32 1
d38 1
a38 1
        virtual void fill( sw_color c );
d40 1
a40 1
        virtual void refresh(); // redisplays self and children
d43 6
d50 10
a59 2
          // displays a videomemory-style array
        virtual void put( sw_rect &p, const char *char_and_attr_map );
d61 3
d69 3
a72 2
        vector <sw_abstract_win *> child;

d74 1
a74 2
        virtual void put_up( sw_rect &p, const char *char_and_attr_map ) = 0;
        virtual void rebuild_map(); // rebuilds map of child windows
d77 1
a77 1
        sw_abstract_win();
d80 9
a88 14
        void attach_child( sw_abstract_win &w )
            {
            child.push_back( &w );
            rebuild_map();
            }
        void detach_child( sw_abstract_win &w )
            {
            for( int i = 0; i < child.size(); i++ )
                {
                if( child[i] == &w )
                    child.erase( child.begin() + i );
                }
            rebuild_map();
            }
d90 11
a100 1
        virtual void resize( int x, int y ) = 0;
d109 2
a110 2
        virtual void put_up( sw_rect &p, const char *char_and_attr_map )
            { w.put( p, char_and_attr_map ); }
d113 7
a119 21
        sw_win( sw_abstract_win &parent, sw_rect pos_size )
            : w(parent), sw_point(pos_size), sw_size(pos_size)
            { w.attach_child( *this );  }
        virtual ~sw_win()
            { w.detach_child( *this );  }


        virtual void resize( int x, int y )
            {
            sw_size::resize(x,y);
            w.rebuild_map();
            w.refresh();
            }

        virtual void move( int x, int y )
            {
            sw_point::move(x,y);
            w.rebuild_map();
            w.refresh();
            }
        
d124 1
a124 1
class sw_pen : public sw_point //, public sw_win_ops
d139 1
a139 1
        virtual void fill()               { w.fill( col_v ); }
@

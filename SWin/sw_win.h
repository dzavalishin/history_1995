/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: window classes
 *
 *      $Log: sw_win.h $
 *      Revision 1.18  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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

        virtual void fill( sw_color col, char c, sw_rect r /* , bool excl_border = No */ ) = 0;
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
        virtual void fill( sw_color col, char c, sw_rect r /*, bool excl_border = No */ );
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
        sw_border       *our_border;

        virtual void register_border( sw_border *b ) { our_border = b; }
        
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
        virtual void visible( bool yn );
        //virtual void visible( bool yn) { if( yn ) top(); else bot(); }

        virtual void focus(); //      { w.focused_child( this ); }
        virtual void focus_tree(); // { w.focused_child( this ); w.focus_tree(); }
        
        operator     sw_rect () const {return sw_rect((sw_point&)*this,(sw_size&)*this);}
    };


  // ------------------------- border -------------------------------------

class sw_border : public sw_win
    {
    private:
        sw_border_t  border_type_v;
        sw_color     border_color_v;
        sw_win      &bordered_window;
          //sw_color     fill_color_v;
          //char         fill_char_v;
        
    public:
        sw_border(
                  sw_win &parent,
                    //sw_rect pos_size,
                  sw_border_t border_type = Single_Border,
                  sw_color border_color = sw_color::Yellow
                    //sw_color fill_color = sw_color::Cyan,
                    //char fill_char = '�'
                 );
        virtual ~sw_border();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };


  // ------------------------- bordered_win -------------------------------------

class sw_bordered_win : public sw_win
    {
    private:
        //sw_border_t  border_type_v;
        //sw_color     border_color_v;
        sw_color     fill_color_v;
        char         fill_char_v;

        sw_border    border_v;
            
    public:
        sw_bordered_win(
                        sw_abstract_win &parent,
                        sw_rect pos_size,
                        sw_border_t border_type = Single_Border,
                        sw_color border_color = sw_color::Yellow,
                        sw_color fill_color = sw_color::Cyan,
                        char fill_char = '�'
                       );
        virtual ~sw_bordered_win();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };


#endif // SW_WIN_H

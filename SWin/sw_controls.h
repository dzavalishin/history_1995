/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes
 *
 *      $Log: sw_controls.h $
 *      Revision 1.9  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.8  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.7  1996/10/11 17:02:37  dz
 *      context menus done, menus code fixed to
 *      support lone vertical menus correctly.
 *
 *      Revision 1.6  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.5  1996/10/03 07:43:57  dz
 *      buttons
 *
 *      Revision 1.4  1996/10/02 11:01:11  dz
 *      focusable
 *
 *      Revision 1.3  1996/09/26 06:06:18  dz
 *      update
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

#ifndef SW_CONTROLS
#define SW_CONTROLS

#include "sw_win.h"

class sw_label;
class sw_menu_base;
class sw_control : public sw_win
    {
    sw_label      *label_v;
    sw_menu_base  *context_menu_v;

    // processes context menus. call from children, if overridden!
    virtual bool event_postprocessor( const sw_event &e );

    public:
        sw_control( sw_abstract_win &p, const sw_rect &r )
            :
        sw_win(p,r)
            {
            label_v = 0;
            context_menu_v = 0;
            }
        
        void del_label();
            //{ if( l ) delete l; l = 0; }

        void label( const char *text, sw_color color );
          //{ del_label(); l = new sw_label( w, sw_point( x() - strlen(text), y() ), text, color ); }

        void context_menu(sw_menu_base *);
    };

class sw_label : public sw_control
    {
    protected:
        char *         text_v;
        int            len_v;
        sw_color       color_v;
        sw_label( sw_abstract_win &parent, sw_color color );
    public:
        sw_label(
                 sw_abstract_win &parent, sw_point pos, 
                 const char *_text, sw_color color = sw_color::Yellow
                );
        virtual ~sw_label();

        virtual void text( const char *s );
        virtual void refresh(const sw_rect&a);
    };

class sw_title : public sw_label
    {
    public:
        sw_title(
                 sw_abstract_win &parent,
                 const char *_text, sw_color color = sw_color::Yellow
                )
            :
        sw_label( parent, color )
            {
            text(_text);
            }
        virtual ~sw_title() {}
        virtual void text( const char *s )
            {
            sw_label::text(s);
            int delta = ((parent().xs()/2)-(strlen(s)/2)) - x();
            if( delta ) move( delta, 0 );
            }
    
    };

class sw_lamp : public sw_label
    {
    sw_color col0, col1;
    int  lit_v;
    public:
        sw_lamp(
                 sw_abstract_win &parent, sw_point pos, 
                 char *_text,
                 sw_color color0 = sw_color(sw_color::Black,sw_color::LightGray),
                 sw_color color1 = sw_color(sw_color::White,sw_color::Red)
                )
            :
        sw_label( parent, pos, _text, color0 ), col0(color0), col1(color1)
            {
            lit_v = 0;
            }
        virtual ~sw_lamp() {}
        
        int value() { return lit_v; }
        virtual void value( int lit )
            {
            if( (!lit_v) == (!lit) ) return;
            lit_v = lit;
            color_v = lit ? col1 : col0;
            for( int i = 0; i < len_v*2; i++ )
                text_v[++i] = color_v.value();
            refresh(sw_rect(0,0,len_v,1));
            }
        
    };



class sw_button_focus_changer : virtual public sw_focus_changer
    {
    public:
        virtual void button_press() {}
        virtual bool pre_focus_changer( const sw_event &e )
            {
            if( e.fkey() == sw_key::space )
                {
                button_press();
                return Yes;
                }
            return No;
            }
        
        virtual bool post_focus_changer( const sw_event &e )
            {
            if( e.fkey() != sw_key::enter ) return No;
            button_press();
            return Yes;
            }
    };



class sw_abs_button : public sw_control, public sw_button_focus_changer
    {
    protected:
        int state_v; // On or off
    public:
        virtual void refresh(const sw_rect&) = 0;
        sw_abs_button
            (
             sw_abstract_win &p, const sw_rect &r
            ) : sw_control(p,r), state_v(0)
            {}
    

        void state( int st )
            {
            state_v = st;
            refresh(sw_rect(0,0,xs(),ys()));
            }
        virtual void focus_change_refresh()
            {
            refresh(sw_rect(0,0,xs(),ys()));
            }
        
        int state() { return state_v; }
        virtual bool event_preprocessor( const sw_event &e );
        virtual void button_press(); // { state(1); device.refresh(); state(0); }
    };


class sw_onechar_button : public sw_abs_button
    {
    protected:
        char c1[2], c2[2];
    public:
        sw_onechar_button
            (
             sw_abstract_win &parent, sw_point pos,
             sw_color color = sw_color::Yellow, char _c1 = '', char _c2 = 9
            );
        
        virtual void refresh(const sw_rect&);
    };


class sw_text_button : public sw_abs_button
    {
    protected:
        string text_v;
        sw_color c1_v, c2_v, cs_v;
        string lower_line_1;
        string lower_line_2;
        int l;
    public:
        sw_text_button
            (
             sw_abstract_win &parent, sw_point pos, const string &text,
             sw_color c1 = sw_color::Yellow, sw_color cs = sw_color::Yellow,
             sw_color c2 = sw_color::Yellow
            );
        
        virtual void refresh(const sw_rect&);
    };





#endif // SW_CONTROLS



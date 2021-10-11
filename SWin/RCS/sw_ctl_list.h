head	1.14;
access;
symbols;
locks;
comment	@ * @;


1.14
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.29.05.54.19;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.29.03.37.08;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.28.22.58.52;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.21.08.07.06;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.14
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes: lists
 *
 *      $Log: sw_ctl_list.h $
 *      Revision 1.13  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.12  1996/10/11 17:02:37  dz
 *      context menus done, menus code fixed to
 *      support lone vertical menus correctly.
 *
 *      Revision 1.11  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.10  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.9  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.8  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.7  1996/09/29 05:54:19  dz
 *      external array for list
 *
 *      Revision 1.6  1996/09/29 03:37:08  dz
 *      using sw_list_elem_base
 *
 *      Revision 1.5  1996/09/28 22:58:52  dz
 *      menu item classes written (wrong way yet)
 *
 *      Revision 1.3  1996/09/26 20:46:52  dz
 *      menus basically done
 *
 *      Revision 1.1  1996/09/20 14:40:18  dz
 *      Initial revision
 *
 *
 *
 *      
 *
\*/

#ifndef SW_CTL_LIST_H
#define SW_CTL_LIST_H

#include "sw_controls.h"
#include "sw_list_elem.h"
#include <vector.h>



class sw_list_base : public sw_control
    {
    protected:
        int                  pos_v;     // Current pos
        int                  shift_v;

        virtual int          size() const = 0;
        virtual int          visible_elems() const = 0;
        virtual void         display_elem( int line ) = 0;
        virtual void         focus_change_refresh() { display_elem( pos_v ); }

        virtual void         refresh(const sw_rect & a);

        virtual void         set_pos(int np);

        virtual bool         event_preprocessor( const sw_event &e );

        virtual void         prev() { set_pos( pos_v-1 ); }
        virtual void         next() { set_pos( pos_v+1 ); }
        virtual void         pgup() { set_pos( pos_v-(visible_elems()-2) ); }
        virtual void         pgdn() { set_pos( pos_v+(visible_elems()-2) ); }
        virtual void         home() { set_pos( 0 ); }
        virtual void         end()  { set_pos( size()-1 ); }
        
        sw_list_base( sw_abstract_win &parent, const sw_rect &rect  )
            : pos_v(0), shift_v(0), sw_control( parent, rect )
            {}
    public:
        virtual void         display_all()
            {
            int ie = shift_v+visible_elems();
            for( int i = shift_v; i < ie; i++ )
                display_elem( i );
            }
        

    };

class sw_simple_list : public sw_list_base
    {
    protected:
        vector <sw_list_elem_base*>   & elem_v;
        sw_color           lcolor_v, scolor_v;
    public:
        sw_simple_list( sw_abstract_win &parent, const sw_rect &rect,
                       vector <sw_list_elem_base*> & elems,
                       sw_color lc, sw_color sc )
            : sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc),
            elem_v(elems)
            {}
        
        virtual int          visible_elems() const { return ys(); }
        virtual int          size() const { return elem_v.size(); }
        virtual void         display_elem( int line );

    };

class sw_menu_base : public sw_list_base
    {
    protected:
        vector <sw_menu_elem_base*>    elem_v;
        sw_color                       lcolor_v, scolor_v;
          // This flag is set when we get focus and reset when we pop it
          // to prevent multiple pops
        bool                           must_pop; 

        //virtual void                   action(); // activate cur. selected one
        virtual void                   dismiss() = 0; //

    public:
        sw_menu_base( sw_abstract_win &parent, const sw_rect &rect,
                     //vector <sw_menu_elem_base*>   &elems,
                     sw_color lc, sw_color sc )
            : sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc)
            //elem_v(elems)
            {}

        virtual void         left_from_submenu() = 0; // called from vert. submenu on left
        virtual void         right_from_submenu() = 0; // called from vert. submenu on right

        virtual void         do_scan_up( int pos );
        virtual void         do_scan_dn( int pos );

        virtual void         scan_up( int pos );
        virtual void         scan_dn( int pos );

        virtual void         prev() { scan_up( pos_v-1 ); }
        virtual void         next() { scan_dn( pos_v+1 ); }
        virtual void         pgup() { scan_up( pos_v-(visible_elems()-2) ); }
        virtual void         pgdn() { scan_dn( pos_v+(visible_elems()-2) ); }
        virtual void         home() { scan_dn( 0 ); }
        virtual void         end()  { scan_up( size()-1 ); }

        virtual int          visible_elems() const = 0; 
        virtual int          size() const { return elem_v.size(); }
        virtual void         display_elem( int nelem ) = 0;
        
        virtual void         activate(); // currently selected one

        virtual void         add( sw_menu_elem_base *el );
        virtual void         del( const sw_menu_elem_base *el );
    };


class sw_hmenu : public sw_menu_base
    {
    private:
        virtual bool         event_preprocessor( const sw_event &e );
        virtual void         refresh(const sw_rect&a);
        //virtual void         action();
        virtual void         dismiss();
        virtual int          visible_elems() const; 

        //sw_win *             back_focus_v; // where to return focus on dismiss
    public:
        sw_hmenu( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc )
            :
        sw_menu_base( parent, rect, lc, sc )
            {
            visible(No);
            }

        virtual void         display_elem( int nelem );
        virtual void         left_from_submenu();
        virtual void         right_from_submenu();

        int elem_at( int xpos ) const;

        //virtual void         activate(); // currently selected one
    };





class sw_vmenu : public sw_menu_base
    {
    protected:
        sw_menu_base *       base_menu_v;
    private:
        virtual bool         event_preprocessor( const sw_event &e );
        virtual void         refresh(const sw_rect&a);
        //virtual void         action();
        virtual void         dismiss();
        virtual int          visible_elems() const;
    public:
        sw_vmenu( sw_abstract_win &parent, sw_menu_base *base_menu, const sw_rect &rect, sw_color lc, sw_color sc )
            :
        sw_menu_base( parent, rect, lc, sc ), base_menu_v(base_menu)
            {
            visible(No);
            }
        
        virtual void         display_elem( int nelem );
        virtual void         left_from_submenu();
        virtual void         right_from_submenu();

        //virtual void         activate(); // currently selected one
    };


class sw_vsubmenu : public sw_vmenu, public sw_menu_elem_base
    {
    public:
        sw_vsubmenu( const string &name, sw_abstract_win &parent, sw_menu_base &base_menu, const sw_rect &rect, sw_color lc, sw_color sc )
            :
        sw_vmenu( parent, &base_menu, rect, lc, sc ),
        sw_menu_elem_base(name,base_menu)
            {
            //base_menu_v->add(this);
            }
        //~sw_vsubmenu() { if( base_menu_v ) base_menu_v->del(this); }

        //void action()         { focus_tree(); }
        //void submenu_action() { focus_tree(); }
        void           action()         { push_focus(); }
        void           submenu_action() { push_focus(); }
        virtual int    auto_dismiss()    { return 0; }

    };



#endif // SW_CTL_LIST_H

    @


1.13
log
@Before changing event class
@
text
@d8 3
a36 3
 *      Revision 1.4  1996/09/28 14:03:17  dz
 *      update
 *
a39 3
 *      Revision 1.2  1996/09/21 08:07:06  dz
 *      update
 *
d120 3
d155 2
d184 3
a186 1
        //virtual void         back_focus( sw_win *f ) { back_focus_v = f; }
d214 2
@


1.12
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@d8 4
@


1.11
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
d221 4
a224 2
        void action()         { focus_tree(); }
        void submenu_action() { focus_tree(); }
@


1.10
log
@bugfixes - a lot of
@
text
@d8 3
d47 1
a47 1
#include "sw_win.h"
d53 1
a53 1
class sw_list_base : public sw_win
d78 1
a78 1
            : pos_v(0), shift_v(0), sw_win( parent, rect )
d161 1
a161 1
        sw_win *             back_focus_v; // where to return focus on dismiss
d174 1
a174 1
        virtual void         back_focus( sw_win *f ) { back_focus_v = f; }
@


1.9
log
@dialogs, basically written
@
text
@d8 3
d59 1
d216 1
@


1.8
log
@log control written
@
text
@d8 3
d204 1
a204 1
        sw_menu_elem_base(name)
d206 1
a206 1
            base_menu_v->add(this);
d208 1
a208 1
        ~sw_vsubmenu() { if( base_menu_v ) base_menu_v->del(this); }
@


1.7
log
@external array for list
@
text
@d8 3
d35 3
d70 9
d214 1
@


1.6
log
@using sw_list_elem_base
@
text
@d8 3
d33 1
a35 84
class sw_list_elem_base
    {
    public:
        bool         disabled;   // action is forbidden, don't call

        virtual void   display( string &s, int width ) const = 0; // put self here
        virtual string display() const = 0; // put self here
        virtual void   action()          {}
        virtual void   submenu_action()  {}

    };
inline void destroy(sw_list_elem_base**) {} // For STL
inline void destroy(sw_list_elem_base*)  {} // For STL

class sw_static_list_elem : public sw_list_elem_base
    {
    protected:
        string       text_v;
    public:
        
        int length() const { return text_v.length(); }
        
        sw_static_list_elem(const string &t) : text_v(t) {}
        void         display( string &s, int w ) const
            { s.print( "%-*.*s", w, w, text_v.c_str() ); }
        virtual string display() const { return text_v; }
    };


class sw_menu_elem_base : public sw_list_elem_base
    {
    protected:
        string       text_v;
    public:
        int          xpos_v; // not all the menus use it. mostly internal use
        bool         selectable; // for menus. non-selectables will be skipped

        int length() const { return text_v.length(); }
        
        sw_menu_elem_base(const string &t) : text_v(t) { selectable = Yes; }
        void         display( string &s, int w ) const
            { s.print( " %-*.*s ", w-2, w-2, text_v.c_str() ); }
        virtual string display() const { return text_v; }
    };

class sw_vmenu_delimiter : public sw_menu_elem_base
    {
    public:
        sw_vmenu_delimiter()
            :
        sw_menu_elem_base("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴")
            {}
    };

class sw_menu_base;
inline void destroy(sw_menu_elem_base**) {} // For STL
inline void destroy(sw_menu_elem_base*)  {} // For STL

class sw_menu_submenu : public sw_menu_elem_base
    {
    private:
        sw_menu_base &   submenu_v;
    public:
        sw_menu_submenu( const string &s, sw_menu_base &sub )
            : sw_menu_elem_base(s), submenu_v(sub) {}
        
        virtual void action();
        virtual void submenu_action(); // do an action only if this is submenu
    };

class sw_menu_func : public sw_menu_elem_base
    {
    private:
        void             (*func_v)( void * );
        void             *arg_v;
    public:
        sw_menu_func( const string &s, void (*f)( void * ), void *a )
            : sw_menu_elem_base(s), func_v(f), arg_v(a) {}
        
        virtual void action()       { func_v(arg_v); }
    };



d69 1
a69 2
          //vector <string>    text_v;
        vector <sw_list_elem_base*>    elem_v;
d72 5
a76 2
        sw_simple_list( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc )
            : sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc)
a80 8

        virtual void         add( sw_list_elem_base *el, int pos = -1 );
        // dont use these ones!
        virtual void         add( const string &s )                    { add(new sw_menu_elem_base(s)); }
        virtual void         add( const string &s, sw_menu_base &sub ) { add(new sw_menu_submenu( s, sub )); }
        //virtual void         add( const string & s, int pos = -1 );
        virtual void         del( int pos );
        virtual void         clear();
a87 1
        //vector <sw_list_elem_base*>    elem_v;
d91 1
a91 1
        virtual void                   action(); // activate cur. selected one
d95 3
a97 1
        sw_menu_base( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc )
d99 1
a99 1
            //,visible_v(0)
d120 1
d123 1
a123 11
          // dont use these ones!
        virtual void         add( const string &s )                    { add(new sw_menu_elem_base(s)); }
        virtual void         add( const string &s, sw_menu_base &sub ) { add(new sw_menu_submenu( s, sub )); }
        
        virtual void         del( int pos );
        virtual void         clear();
        sw_menu_elem_base *  operator[] (int i) { return elem_v[i]; }

        virtual void         display_elem( int nelem ) = 0;
        
        
d158 2
a164 1
        sw_menu_base *       base_menu_v;
d180 16
@


1.5
log
@menu item classes written (wrong way yet)
@
text
@d8 3
d43 16
a114 13
  /*
elem    {
    elem( string s, int i, bool _sel, sw_menu_base *submenu = 0 )
        :
    text_v(s), xpos_v(i), selectable(_sel), submenu_v(submenu) {}
    
    elem() : selectable(No), submenu_v(0) {}
    string           text_v;
    int              xpos_v;
    bool             selectable;
    sw_menu_base *   submenu_v;
    };
*/
d149 2
a150 1
        vector <string>    text_v;
d158 1
a158 1
        virtual int          size() const { return text_v.size(); }
d160 5
a164 1
        virtual void         add( const string & s, int pos = -1 );
d174 1
a186 2
        sw_menu_elem_base &  operator[] (int i) { return *(elem_v[i]); }
        
d207 1
a207 1
        // BUG!! BUG!! new!!
d213 2
d216 1
@


1.4
log
@update
@
text
@d8 3
d29 82
a110 1
// Note that this class is used by entry filed subclasses too
d123 1
a123 2
            // { for( int i = a.y(); i < a.end(); i++ ) display_line( shift_v+i, sw_pos(0,i); }
        
a161 14
    public:
        struct elem
            {
            elem( string s, int i, bool _sel, sw_menu_base *submenu = 0 )
                :
            text_v(s), xpos_v(i), selectable(_sel), submenu_v(submenu) {}
            
            elem() : selectable(No), submenu_v(0) {}
            string           text_v;
            int              xpos_v;
            bool             selectable;
            sw_menu_base *   submenu_v;
            };

d163 2
a164 3
        vector <elem>        elem_v;
        sw_color             lcolor_v, scolor_v;
        //int                visible_v;
d166 2
a167 2
        virtual void         action() = 0; // do something with currently selected one
        virtual void         dismiss() = 0; //
d175 1
a175 1
        elem &               operator[] (int i) { return elem_v[i]; }
d193 1
a193 2

        virtual int          visible_elems() const = 0; //{ return visible_v; }
d196 5
a200 1
        virtual void         add( const string & s, sw_menu_base *b = 0, int pos = -1 );
d213 1
a213 1
        virtual void         action();
d215 1
a215 1
        virtual int          visible_elems() const; // { return vs(); }
d242 1
a242 1
        virtual void         action();
d245 1
a245 1
        virtual int          visible_elems() const;// { return ys(); }
@


1.3
log
@menus basically done
@
text
@d8 3
d145 2
d158 2
d174 1
a174 1
        virtual int          visible_elems() const { return ys(); }
@


1.2
log
@update
@
text
@d8 3
d79 9
a87 3
            string         text_v;
            int            xpos_v;
            bool           selectable;
d91 6
a96 3
        vector <elem>      elem_v;
        sw_color           lcolor_v, scolor_v;
        int                visible_v;
d100 2
a101 2
            : sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc),
            visible_v(0)
d106 3
d123 1
a123 1
        virtual int          visible_elems() const { return visible_v; }
d126 1
a126 1
        virtual void         add( const string & s, int pos = -1 );
d136 6
d146 12
a157 1
            {}
d159 17
d177 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_controls.h $
d29 2
a30 1
        virtual void         display_line( int line ) = 0;
d41 2
a42 2
        virtual void         pgup() { set_pos( pos_v-(ys()-2) ); }
        virtual void         pgdn() { set_pos( pos_v+(ys()-2) ); }
d61 1
d67 50
a116 1
        virtual void         display_line( int line );
d118 10
d132 4
@

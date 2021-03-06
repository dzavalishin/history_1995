/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: list/menu elements
 *
 *      $Log: sw_list_elem.h $
 *      Revision 1.6  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
 *      Revision 1.5  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.4  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.3  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.2  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.1  1996/09/29 05:54:19  dz
 *      Initial revision
 *
 *      
 *
\*/

#ifndef SW_LIST_ELEM_H
#define SW_LIST_ELEM_H

#include "sw_win.h"
#include <vector.h>

class sw_list_elem_base
    {
    public:
        bool         disabled;   // action is forbidden, don't call
        
        virtual void   display( string &s, int width ) const = 0; // put self here
        virtual string display() const = 0; // put self here
        virtual void   action()          {}
        virtual void   submenu_action()  {}
        virtual int    auto_dismiss()    { return 1; }

          // Called by menu, that loosed focus and is trying to find out,
          // does it have some submenus, that are in focus
        //virtual bool   has_focused_submenu()    { return No; }
        
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


class sw_menu_base;

class sw_menu_elem_base : public sw_list_elem_base
    {
    protected:
        string       text_v;
    public:
        int          xpos_v; // not all the menus use it. mostly internal use
        bool         selectable; // for menus. non-selectables will be skipped
        sw_menu_base &base_menu_v;
        
        int length() const { return text_v.length(); }
        
        sw_menu_elem_base(const string &t, sw_menu_base &base_menu);
        ~sw_menu_elem_base();
        
        void         display( string &s, int w ) const
            { s.print( " %-*.*s ", w-2, w-2, text_v.c_str() ); }
        virtual string display() const { return text_v; }
    };

class sw_vmenu_delimiter : public sw_menu_elem_base
    {
    public:
        sw_vmenu_delimiter(sw_menu_base &bm)
            :
        sw_menu_elem_base("????????????????????????????????????????????????",bm)
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
        sw_menu_submenu( const string &s, sw_menu_base &bm, sw_menu_base &sub )
            : sw_menu_elem_base(s,bm), submenu_v(sub) {}
        
        virtual void action();
        virtual void submenu_action(); // do an action only if this is submenu
        virtual int  auto_dismiss()    { return 0; }
    };

class sw_menu_func : public sw_menu_elem_base
    {
    private:
        void             (*func_v)( void * );
        void             *arg_v;
    public:
        sw_menu_func( const string &s, sw_menu_base &bm, void (*f)( void * ), void *a )
            : sw_menu_elem_base(s,bm), func_v(f), arg_v(a) {}
        
        virtual void action()       { func_v(arg_v); }
    };

class sw_menu_push_focus : public sw_menu_elem_base
    {
    private:
        sw_abstract_win &new_focus;
    public:
        sw_menu_push_focus( const string &s, sw_menu_base &bm, sw_abstract_win &nf )
            : sw_menu_elem_base(s,bm), new_focus(nf) {}
        
        virtual void action()       { new_focus.push_focus(); }
    };


#endif // SW_LIST_ELEM_H


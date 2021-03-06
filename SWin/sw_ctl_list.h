/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes: lists
 *
 *      $Log: sw_ctl_list.h $
 *      Revision 1.14  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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

    
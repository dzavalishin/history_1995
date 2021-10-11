/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: events
 *
 *      $Log: event.c $
 *      Revision 1.7  1996/11/18 16:16:26  dz
 *      Event class now has no type field/method, but
 *      has info() == KeyPress and delivery mode == Mouse.
 *      If event::mouse() is nonzero, pos is valid.
 *
 *      Revision 1.6  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.5  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.4  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.3  1996/09/28 22:58:52  dz
 *      menu item classes written (wrong way yet)
 *
 *
 *      
 *
\*/


#include "sw_event_handler.h"


int sw_event::operator == ( const sw_event &e ) const
    {
    if( info_v != e.info_v ) return No;
    if( info_v == KeyPress )
        {
            return
                akey_v == e.akey_v &&
                vkey_v == e.vkey_v &&
                shifts_v == e.shifts_v;
        }
    if( mouse() )
        return pos_v == e.pos_v;
    
    return 0;
    }


  // -----------------------------------------------------------------------


void sw_event_handler::attach_hook( sw_hook_base *h )
    {
    hook_v.push_back(h);
    }

void sw_event_handler::detach_hook( sw_hook_base *h )
    {
    for( int i = 0; i < hook_v.size(); i++ )
        if( hook_v[i] == h )
            {
            hook_v.erase( hook_v.begin() + i );
            return;
            }
    }


bool sw_event_handler::event( const sw_event &e )
    {
    int nonforce = !e.forced();

    switch( e.info() )
        {
        case sw_event::InFocus:    in_focus = Yes; focus_change_refresh(); break;
        case sw_event::NotInFocus: in_focus = No;  focus_change_refresh(); break;
        }
    
    for( int i = 0; i < hook_v.size(); i++ )
        if( hook_v[i]->pre(e) && nonforce ) return Yes;

      // *_focus_changer is defined in sw_focus_changer
    if( pre_focus_changer( e )   && nonforce ) return Yes;
    if( event_preprocessor( e )  && nonforce ) return Yes;

    if( e.broad() )
        {
        if( pass_broadcast_to_children( e ) && nonforce )                return Yes;
        }
    else if( !e.mouse() )
        {
        if( pass_event_to_focus( e ) && nonforce )                       return Yes;
        }

    if( event_postprocessor( e ) && nonforce ) return Yes;
    if( post_focus_changer( e )  && nonforce ) return Yes;

    for( i = 0; i < hook_v.size(); i++ )
        if( hook_v[i]->post(e) && nonforce ) return Yes;

    return No;
    }



  // -----------------------------------------------------------------------




sw_hook_base::sw_hook_base(sw_event_handler&e) : h_v(e)
    {
    h_v.attach_hook(this);
    }

sw_hook_base::~sw_hook_base()
    {
    h_v.detach_hook(this);
    }








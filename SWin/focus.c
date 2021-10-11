
#include "sw_focus.h"
#include "sw_win.h"

void sw_focus_changer::focused_child( sw_abstract_win *f )
    {
    if(focus_v == f) return;
    if( focus_v ) focus_v->event( sw_event(sw_event::NotInFocus,sw_event::FFocus) );
    focus_v = f;
    if( focus_v ) focus_v->event( sw_event(sw_event::InFocus,sw_event::FFocus) );
    }



bool sw_focus_changer::pass_event_to_focus( const sw_event &e )
    {
    return focus_v ? focus_v->event(e) : No;
    }

bool sw_focus_changer::pre_focus_changer( const sw_event &e )
    {
    if(
       focusable() &&
       (
        e.info() == sw_event::Mouse1Press ||
        e.info() == sw_event::Mouse2Press ||
        e.info() == sw_event::Mouse3Press ||
        e.info() == sw_event::Mouse1Release ||
        e.info() == sw_event::Mouse2Release ||
        e.info() == sw_event::Mouse3Release
       )
      )
        focus_tree();
    return No;
    }

bool sw_focus_changer::post_focus_changer( const sw_event &e )
    {
    return No;
    }


  // -----------------------------------------------------------------------

bool sw_dialog_focus_changer::pre_focus_changer( const sw_event &e )
    {
    if(
       focusable() &&
       (
        e.info() == sw_event::Mouse1Press ||
        e.info() == sw_event::Mouse2Press ||
        e.info() == sw_event::Mouse3Press ||
        e.info() == sw_event::Mouse1Release ||
        e.info() == sw_event::Mouse2Release ||
        e.info() == sw_event::Mouse3Release
       )
      )
        focus_tree();
    if( e.fkey() != sw_key::tab ) return No;
    if( e.shift() )
        focus_to_prev_child();
    else
        focus_to_next_child();
    return Yes;
    }

bool sw_dialog_focus_changer::post_focus_changer( const sw_event &e )
    {
    switch( e.fkey() )
        {
        case sw_key::esc:   pop_focus();            break;
        case sw_key::left:  focus_to_left_child();  break;
        case sw_key::right: focus_to_right_child(); break;
        case sw_key::up:    focus_to_up_child();    break;
        case sw_key::down:  focus_to_down_child();  break;
        default: return No;
        }
    return Yes;
    }


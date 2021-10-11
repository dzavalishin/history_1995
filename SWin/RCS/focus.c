head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.11.18.16.16.26;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.26.06.06.18;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.21.13.26.57;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Event class now has no type field/method, but
has info() == KeyPress and delivery mode == Mouse.
If event::mouse() is nonzero, pos is valid.
@
text
@
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

@


1.6
log
@Before changing event class
@
text
@d23 1
a23 1
       focusable() && e.type() == sw_event::Mouse &&
d47 11
a57 1
    if( focusable() && e.type() == sw_event::Mouse )
@


1.5
log
@preliminary mouse support done
@
text
@d22 11
a32 1
    if( focusable() && e.type() == sw_event::Mouse )
@


1.4
log
@dialogs, basically written
@
text
@d20 12
d33 1
a33 1
// -----------------------------------------------------------------------
d37 2
@


1.3
log
@update
@
text
@d20 27
@


1.2
log
@generalized event model
@
text
@a19 6
/*
void sw_focus_changer::pass_focused_broadcast_event_to_focus( const sw_event &e )
    {
    if( focus_v ) focus_v->focused_broadcast(e);
    }
*/
@


1.1
log
@Initial revision
@
text
@d8 1
a8 1
    if( focus_v ) focus_v->focused_broadcast( sw_event(sw_event::NoFocus) );
d10 1
a10 1
    if( focus_v ) focus_v->focused_broadcast( sw_event(sw_event::Focus) );
d20 1
d25 1
a25 1

@

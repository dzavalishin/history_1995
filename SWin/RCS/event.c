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
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.28.22.58.52;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.28.14.08.21;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.21.08.07.06;	author dz;	state Exp;
branches;
next	;


desc
@last version with distinct broadcast/event entry points
@


1.7
log
@Event class now has no type field/method, but
has info() == KeyPress and delivery mode == Mouse.
If event::mouse() is nonzero, pos is valid.
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: events
 *
 *      $Log: event.c $
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







@


1.6
log
@Before changing event class
@
text
@d8 3
d31 2
a32 2
    if( type_v != e.type_v ) return No;
    switch( type_v )
a33 1
        case Key:
a37 2
        case Mouse: return 0;
        case Info: return info_v == e.info_v;
d39 4
a42 1
    return No;
d69 1
a69 1
    if( e.type() == sw_event::Info )
d71 2
a72 2
        if( e.info() == sw_event::InFocus )    { in_focus = Yes; focus_change_refresh(); }
        if( e.info() == sw_event::NotInFocus ) { in_focus = No;  focus_change_refresh(); }
d82 5
a86 1
    if( e.type() != sw_event::Mouse )
d88 1
a88 8
        if( e.broad() )
            {
            if( pass_broadcast_to_children( e ) && nonforce )                return Yes;
            }
        else
            {
            if( pass_event_to_focus( e ) && nonforce )                       return Yes;
            }
@


1.5
log
@preliminary mouse support done
@
text
@d8 3
@


1.4
log
@bugfixes - a lot of
@
text
@d8 3
d76 1
a76 1
    if( e.broad() )
d78 8
a85 7
        if( pass_broadcast_to_children( e ) && nonforce )
            return Yes;
        }
    else
        {
        if( pass_event_to_focus( e ) && nonforce )
            return Yes;
@


1.3
log
@menu item classes written (wrong way yet)
@
text
@d7 4
a10 1
 *      $Log: device.c $
d60 6
@


1.2
log
@update
@
text
@d1 12
@


1.1
log
@Initial revision
@
text
@d25 1
a25 1
void sw_event_handler::attach_hook( const sw_hook_base *h )
d30 1
a30 1
void sw_event_handler::detach_hook( const sw_hook_base *h )
d32 1
a32 1
    for( int i = 0; i < child.size(); i++ )
d43 5
d49 2
a50 7
    if( pre_focus_changer( e ) ) return Yes;
    if( event_preprocessor( e ) ) return Yes;
    if( pass_event_to_focus( e ) ) return Yes;
    if( event_postprocessor( e ) ) return Yes;
    if( post_focus_changer( e ) ) return Yes;
    return No;
    }
d52 10
a61 7
  // To all windows
void sw_event_handler::broadcast( const sw_event &e )
    {
    broadcast_preprocessor( e );
    pass_broadcast_to_children( e );
    broadcast_postprocessor( e );
    }
d63 2
a64 7
  // To all windows in focus
void sw_event_handler::focused_broadcast( const sw_event &e )
    {
    broadcast_preprocessor( e );
    pass_focused_broadcast_event_to_focus( e );
    broadcast_postprocessor( e );
    }
d66 2
a68 4
bool sw_event_handler::event_to_hooks( const sw_event &e )
    {
    for( int i = 0; i < child.size(); i++ )
        if( hook_v[i]->event(e) ) return Yes;
a71 6
void sw_event_handler::broadcast_to_hooks( const sw_event &e )
    {
    for( int i = 0; i < child.size(); i++ )
        hook_v[i].broadcast(e);
    }

a78 2


d84 1
a84 1
virtual sw_hook_base::~sw_hook_base()
@

head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.10.01.16.32.27;	author dz;	state Exp;
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
@Before changing event class
@
text
@
#ifndef SW_FOCUS
#define SW_FOCUS

#include "sw_event.h"

class sw_abstract_win;

class sw_focus_changer
    {
    private:
        sw_abstract_win *focus_v;
        
    public:
        sw_focus_changer() { focus_v = 0; }
        
        void              focused_child( sw_abstract_win *f );
        sw_abstract_win * focused_child() { return focus_v; }

        virtual bool      pass_event_to_focus( const sw_event &e );
        
        virtual bool      pre_focus_changer( const sw_event &e );
        virtual bool      post_focus_changer( const sw_event &e );

        virtual void      focus_to_next_child()  {}
        virtual void      focus_to_prev_child()  {}

        virtual void      focus_to_left_child()  {}
        virtual void      focus_to_right_child() {}
        virtual void      focus_to_up_child()    {}
        virtual void      focus_to_down_child()  {}

        virtual void      push_focus() {} // redefined in sw_abstract_win
        virtual void      pop_focus()  {} // redefined in sw_abstract_win
        virtual void      focus_tree() {} // redefined in sw_abstract_win
        virtual bool      focusable() { return No; }  // redefined in sw_abstract_win
    };

class sw_dialog_focus_changer : virtual public sw_focus_changer
    {
    public:
        virtual bool      pre_focus_changer( const sw_event &e );
        virtual bool      post_focus_changer( const sw_event &e );
    };

#endif
@


1.6
log
@preliminary mouse support done
@
text
@@


1.5
log
@bugfixes - a lot of
@
text
@d22 2
a23 2
        virtual bool      pre_focus_changer( const sw_event &e ) { return No; }
        virtual bool      post_focus_changer( const sw_event &e ) { return No; }
d35 2
@


1.4
log
@dialogs, basically written
@
text
@d20 1
a20 1
        virtual bool pass_event_to_focus( const sw_event &e );
d22 2
a23 2
        virtual bool pre_focus_changer( const sw_event &e ) { return No; }
        virtual bool post_focus_changer( const sw_event &e ) { return No; }
d25 2
a26 2
        virtual void focus_to_next_child()  {}
        virtual void focus_to_prev_child()  {}
d28 4
a31 4
        virtual void focus_to_left_child()  {}
        virtual void focus_to_right_child() {}
        virtual void focus_to_up_child()    {}
        virtual void focus_to_down_child()  {}
d33 2
a34 2
        virtual void push_focus() {} // redefined in sw_abstract_win
        virtual void pop_focus()  {} // redefined in sw_abstract_win
d40 2
a41 2
        virtual bool pre_focus_changer( const sw_event &e );
        virtual bool post_focus_changer( const sw_event &e );
@


1.3
log
@update
@
text
@d32 3
d40 2
a41 22
        virtual bool pre_focus_changer( const sw_event &e )
            {
            if( e.fkey() != sw_key::tab ) return No;
            if( e.shift() )
                focus_to_prev_child();
            else
                focus_to_next_child();
            return Yes;
            }

        virtual bool post_focus_changer( const sw_event &e )
            {
            switch( e.fkey() )
                {
                case sw_key::left:  focus_to_left_child(); break;
                case sw_key::right: focus_to_right_child(); break;
                case sw_key::up:    focus_to_up_child(); break;
                case sw_key::down:  focus_to_down_child(); break;
                default: return No;
                }
            return Yes;
            }
@


1.2
log
@generalized event model
@
text
@a17 7
            /*
            {
            if( focus_v ) focus_v->broadcast( sw_event(sw_event::NoFocus) );
            focus_v = f;
            if( focus_v ) focus_v->broadcast( sw_event(sw_event::Focus) );
            }
            */
a20 1
        //virtual void pass_focused_broadcast_event_to_focus( const sw_event &e );
@


1.1
log
@Initial revision
@
text
@d28 1
a28 1
        virtual void pass_focused_broadcast_event_to_focus( const sw_event &e );
@

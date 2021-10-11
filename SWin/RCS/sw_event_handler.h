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
date	96.09.26.06.06.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.21.13.26.57;	author dz;	state Exp;
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


1.7
log
@Before changing event class
@
text
@
#ifndef SW_EVENT_HANDLER
#define SW_EVENT_HANDLER

#include "sw_focus.h"
#include "sw_event.h"

#include <vector.h>

class sw_hook_base;
inline void destroy(sw_hook_base**) {}

class sw_event_handler : virtual public sw_focus_changer
    {
    private:
        vector <sw_hook_base*>  hook_v;

    protected:
        // THIS window is in focus (and may be some child too)
        bool         in_focus;
        virtual void focus_change_refresh() // called when we go to/from focus
            {}

    public:
        sw_event_handler()
            {
            in_focus = No;
            }
        
        virtual void attach_hook( sw_hook_base *h ); // { hook_v.push_back(h); }
        virtual void detach_hook( sw_hook_base *h );
        
          // event_preprocessor is possibly defined in inherited class and used
          // by class itself to filter messages it HAS to process himself
        virtual bool event_preprocessor( const sw_event &e ) { return No; }

          // event_preprocessor is possibly defined in inherited class and used
          // by class itself to filter messages it CAN process himself
        virtual bool event_postprocessor( const sw_event &e ) { return No; }


        virtual bool pass_broadcast_to_children( const sw_event &e )
            { return No; }

        virtual bool      pass_event_to_position( const sw_event &e )
            { return No; }

          // -------------------
          // main entry point
          // -------------------
        
        // returns yes if handled
        virtual bool event( const sw_event &e );

    };


class sw_hook_base
    {
    protected:
        sw_event_handler &h_v;
        sw_hook_base(sw_event_handler&e);
        virtual ~sw_hook_base();
        
    public:
        virtual bool pre( const sw_event &e ) = 0;
        virtual bool post( const sw_event &e ) = 0;

    };


class sw_single_hook : public sw_hook_base
    {
    sw_event         filter_v;
    virtual void action(const sw_event &e) = 0;
    public:
        sw_single_hook( sw_event_handler&e, const sw_event &filter )
            :
        sw_hook_base(e), filter_v(filter)
            {}
        
        virtual bool pre( const sw_event &e ) { return No; }
        virtual bool post( const sw_event &e ) 
            {
            if( e == filter_v )
                {
                action( e );
                return Yes;
                }
            return No;
            }
        
    };


class sw_repost_hook : public sw_single_hook
    {
    sw_event_handler &dest_v;
    const sw_event   &e_v;

    virtual void action(const sw_event &e)
        {
        dest_v.event( e_v );
        }
    
    public:
        sw_repost_hook(
                       sw_event_handler&hooked,
                       const sw_event &filter,
                       const sw_event &newev,
                       sw_event_handler &dest
                      )
            :
        sw_single_hook(hooked,filter), e_v(newev), dest_v(dest)
            {}
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
@d45 3
@


1.4
log
@update
@
text
@d17 7
d25 5
a41 1
        //virtual void broadcast_preprocessor( const sw_event &e )      {}
a43 2
        //virtual void broadcast_postprocessor( const sw_event &e )     {}

d46 1
a46 1
          // main entry points
a51 5
          // To all windows
        //virtual void broadcast( const sw_event &e );

          // To all windows in focus
        //virtual void focused_broadcast( const sw_event &e );
a59 4
            /*: h_v(e)
            {
            h_v.attach_hook(this);
            } */
a60 1
          //        {          h_v.detach_hook(this);            }
@


1.3
log
@generalized event model
@
text
@a19 2
        //virtual bool event_to_hooks( const sw_event &e );
        //virtual void broadcast_to_hooks( const sw_event &e );
@


1.2
log
@last version with distinct broadcast/event entry points
@
text
@d8 2
d18 4
a21 4
        virtual void attach_hook( const sw_hook_base *h ) { hook_v.push_back(h); }
        virtual void detach_hook( const sw_hook_base *h );
        virtual bool event_to_hooks( const sw_event &e );
        virtual void broadcast_to_hooks( const sw_event &e );
d32 4
a35 3
        virtual void broadcast_preprocessor( const sw_event &e )      {}
        virtual void pass_broadcast_to_children( const sw_event &e )  {}
        virtual void broadcast_postprocessor( const sw_event &e )     {}
d46 1
a46 1
        virtual void broadcast( const sw_event &e );
d49 1
a49 1
        virtual void focused_broadcast( const sw_event &e );
d57 2
a58 1
        sw_hook_base(sw_event_handler&e) : h_v(e)
d61 4
a64 6
            }
        virtual ~sw_hook_base()
            {
            h_v.detach_hook(this);
            }
        virtual void action(const sw_event &e) = 0;
d66 2
a67 2
        virtual bool event( const sw_event &e ) = 0;
        virtual void broadcast( const sw_event &e ) = 0;
d75 1
d77 4
a80 1
        sw_single_hook( const sw_event &filter ) : filter_v(filter) {}
d82 2
a83 1
        virtual bool event( const sw_event &e )
a92 5
        virtual void broadcast( const sw_event &e );
            {
            if( e == filter_v ) action( e );
            }

d100 6
d108 1
d114 3
a116 1
        sw_single_hook(filter), e_v(newev),
@


1.1
log
@Initial revision
@
text
@d8 2
a11 1

d13 2
d16 5
d34 4
d40 36
d78 5
a82 6
              // *_focus_changer is defined in sw_focus_changer
            if( pre_focus_changer( e ) ) return Yes;
            if( event_preprocessor( e ) ) return Yes;
            if( pass_event_to_focus( e ) ) return Yes;
            if( event_postprocessor( e ) ) return Yes;
            if( post_focus_changer( e ) ) return Yes;
d85 2
a86 3

          // To all windows
        virtual void broadcast( const sw_event &e )
d88 1
a88 3
            broadcast_preprocessor( e );
            pass_broadcast_to_children( e );
            broadcast_postprocessor( e );
a90 7
          // To all windows in focus
        virtual void focused_broadcast( const sw_event &e )
            {
            broadcast_preprocessor( e );
            pass_focused_broadcast_event_to_focus( e );
            broadcast_postprocessor( e );
            }
d92 14
@

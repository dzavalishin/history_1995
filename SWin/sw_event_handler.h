
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



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

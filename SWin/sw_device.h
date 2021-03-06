/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes
 *
 *      $Log: sw_device.h $
 *      Revision 1.11  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
 *      Revision 1.10  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.9  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.8  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.7  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.6  1996/09/27 04:54:43  dz
 *      fast fill - now we're passing fill data
 *      down to device and do actual fill there.
 *      Prepared to do same thing with border, but
 *      not sure yet I really will.
 *
 *      Revision 1.5  1996/09/27 00:33:20  dz
 *      viobuf, optimization (sort of:)
 *
 *      Revision 1.3  1996/09/12 01:12:51  dz
 *      works a little
 *
 *      Revision 1.1  1996/09/09 01:47:48  dz
 *      Initial revision
 *
 *      
 *
\*/


#ifndef SW_DEVICE_H
#define SW_DEVICE_H


#include "sw_types.h"
#include "sw_win.h"

#include <iostream.h>
#include <stack.h>



class sw_device : public sw_abstract_win
    {
    private:
        sw_rect          invalid_v;
        bool             have_invalid_v;

        stack<vector<sw_abstract_win *> > focus_stack;

    protected:
        void             invalidate( const sw_rect &r );

        virtual void     mouse_hide( const sw_rect &r );
        virtual void     mouse_show();
        
    public:
        void             flush();

    protected:
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &p, const char *char_and_attr_map ) = 0;
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r ) = 0;
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t ) = 0;

    public:
        virtual void move_to_win( sw_point &p ) const {}

    public:
        sw_device( const sw_size &s = sw_size(80,25) );
        virtual ~sw_device();
        virtual const char *name() { return "abstract"; }
        virtual void        do_flush(const sw_rect &r) {}
            
        virtual void        resize( const sw_size &s );

        virtual void        calculate_absolute_window_posiotion()
            {
              // we are device, we're allways at 0,0
            absolute_positon_v = sw_point(0,0);
            }
        
        virtual void        push_focus();
        virtual void        push_focus(sw_abstract_win &new_focus);
        virtual void        pop_focus();
        virtual void        clear_win_from_focus_stack(sw_abstract_win &w);
        
        virtual void        mouse_capture( sw_event_handler *c ); // {}
        bool                mouse_event( const sw_event &e );
        bool                mouse_event_translator( sw_event &e );
        /*
            {
            sw_abstract_win *wp = map.win_at( e.pos() );
            if( wp ) wp->event( e );
            }
        */
    };


// video memory devices with 2 bytes per cell
class sw_2b_video_memory_device : public sw_device
    {
    protected:
        char *video_memory; // video memory or vm cache

        virtual void vm_alloc() = 0;
        virtual void vm_free() = 0;
        
        sw_2b_video_memory_device(const sw_size &s) : sw_device(s)
            {
            }
        
        ~sw_2b_video_memory_device()
            {
            }

        virtual void        do_flush(const sw_rect &r) = 0;

    public:
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &p, const char *char_and_attr_map );
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r );
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t );

        virtual const char *name() { return "2b_video_memory-style"; }
    };



#ifdef __OS2__
#  ifndef SW_DEVICE_OS2_H
#    include "sw_device_os2.h"
#  endif
#endif

#ifndef SW_DEVICE_TTY_H
#  include "sw_device_tty.h"
#endif


#endif // SW_DEVICE_H


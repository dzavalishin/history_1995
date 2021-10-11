head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.10.10.10.46.53;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: OS/2-specific device classes
 *
 *      $Log: sw_device_os2.h $
 *      Revision 1.5  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.4  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.3  1996/11/15 16:29:28  dz
 *      mouse event generator written
 *
 *      Revision 1.2  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.1  1996/10/10 10:46:53  dz
 *      Initial revision
 *
 *
 *      
 *
\*/

#ifndef SW_DEVICE_OS2_H
#define SW_DEVICE_OS2_H

#ifndef SW_DEVICE_H
#  include "sw_device.h"
#endif


#include "sw_event.h"
//#include "sw_win.h"

//#include <iostream.h>
//#include <stack.h>

#include "sw_sysdep.h"

#define INCL_MOU
#define INCL_KBD
#define INCL_VIO
#define INCL_DOSPROCESS
#include <os2.h>



class sw_os2_mouse_handler : public sw_thread
    {
    sw_device &         dev_v;
    sw_event_handler *  capture_v;
    int                 h; // mouse handle

    virtual void run();
    void send_event( sw_point p, sw_event::it i );
    public:
        sw_os2_mouse_handler(sw_device &d) : dev_v(d)
            {
            capture_v = 0;
            h = 0;
            sw_thread::begin();
            }

        ~sw_os2_mouse_handler()
            {
            sw_thread::ask_to_stop();
              //sw_thread::wait();
            ::DosSleep( 65 );
            sw_thread::kill();
            }

        void hide( const sw_rect &where );
        void show();

          // Forward all mouse events until all the buttons are released
          // to this window. Call with 0 to stop capture
        void capture( sw_event_handler *c )
            {
            if( c ) c->event( sw_event::BeginOfCapture );
            else
                {
                if( capture_v )
                    capture_v->event( sw_event::EndOfCapture );
                }
            capture_v = c;
            }
    };

class sw_vio_base : public sw_2b_video_memory_device
    {
    private:
        //VIOMODEINFO vio_mi;
        //char *viobuf;
        char *saved_screen;

        void vm_alloc();
        void vm_free();

        sw_os2_mouse_handler mouse;

    protected:
        virtual void mouse_hide( const sw_rect &r );
        virtual void mouse_show();

        
    public:
        sw_vio_base();
        virtual ~sw_vio_base();
        //virtual void put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca );
        //virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r );
        //virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t );
        virtual void do_flush(const sw_rect &r);
        virtual void resize( const sw_size &s )
            {
            throw sw_Fail("sw_vio_device::resize","Resize is not implemented for this device", name() );
              // check and change video mode
            sw_abstract_win::resize(s);
            }

        virtual void mouse_capture( sw_event_handler *c ) { mouse.capture( c ); }
    };

class sw_vio_device : public sw_vio_base
    {
    //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
    public:
        sw_vio_device();
        virtual ~sw_vio_device();
        virtual const char *name(); // { return "vio"; }


    };


class sw_avio_device : public sw_vio_base
    {
    //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
    public:
        sw_avio_device();
        virtual ~sw_avio_device();
        virtual const char *name(); // { return "avio"; }
    };






#endif // SW_DEVICE_OS2_H


@


1.5
log
@Before changing event class
@
text
@d8 3
d105 5
@


1.4
log
@preliminary mouse support done
@
text
@d8 3
@


1.3
log
@mouse event generator written
@
text
@d8 3
d39 8
d66 3
a68 1
            sw_thread::wait();
d113 2
@


1.2
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
d28 1
a28 1
//#include "sw_types.h"
d34 40
d84 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_device.h $
d37 3
@

head	1.11;
access;
symbols;
locks;
comment	@ * @;


1.11
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.27.04.54.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.27.00.33.20;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.01.47.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.11
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes
 *
 *      $Log: sw_device.h $
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

@


1.10
log
@Before changing event class
@
text
@d8 3
a29 3
 *      Revision 1.4  1996/09/20 14:40:18  dz
 *      update
 *
a32 3
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
d60 1
a60 1
        
d63 4
d95 1
@


1.9
log
@preliminary mouse support done
@
text
@d8 3
d85 6
d95 4
a98 2
        virtual void        mouse_capture( sw_event_handler *c ) {}
        void                mouse_event( const sw_event &e )
d103 1
a103 1

@


1.8
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
d86 7
a92 2
        // will dispatch events as long as (*run) != 0
        //virtual void event_dispatching_loop( int *run );
@


1.7
log
@dialogs, basically written
@
text
@d8 3
d37 2
d87 2
a88 1
class sw_vio_base : public sw_device
d90 9
a98 4
    private:
        //VIOMODEINFO vio_mi;
        char *viobuf;
        char *saved_screen;
d100 1
a100 8
    public:
        sw_vio_base();
        virtual ~sw_vio_base();
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca );
        virtual void fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect r );
        virtual void border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t );
        virtual void do_flush(const sw_rect &r);
        virtual void resize( const sw_size &s )
a101 3
            throw sw_Fail("sw_vio_device::resize","Resize is not implemented for this device", name() );
              // check and change video mode
            sw_abstract_win::resize(s);
a102 9
    };

class sw_vio_device : public sw_vio_base
    {
    //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
    public:
        sw_vio_device();
        virtual ~sw_vio_device();
        virtual const char *name(); // { return "vio"; }
d104 1
a105 6
    };


class sw_avio_device : public sw_vio_base
    {
    //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
d107 1
a107 23
        sw_avio_device();
        virtual ~sw_avio_device();
        virtual const char *name() { return "avio"; }
    };

// Cache must be implemented here
class sw_tty_base
    {
    protected:
        ostream& os_v;
        void tty_write( const char *c, int count );
    public:
        sw_tty_base( ostream& s = cout); // : os_v(s); // { }
    };


class sw_ansi_device : public sw_device, public sw_tty_base
    {
    private:
        void ansi_set_color( char color );
        void ansi_gotoxy( int x, int y );
    public:
        virtual void put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca );
d111 1
a111 4
    public:
        sw_ansi_device();
        virtual ~sw_ansi_device();
        virtual const char *name(); // { return "ansi"; }
a113 1
#ifdef unix
a114 8
class sw_termcap_device : public sw_device, public sw_tty_base
    {
    //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
    public:
        sw_termcap_device();
        virtual ~sw_termcap_device();
        virtual const char *name() { return "termcap, unknown tty type"; }
    };
d116 4
d122 3
d127 1
a127 1

@


1.6
log
@fast fill - now we're passing fill data
down to device and do actual fill there.
Prepared to do same thing with border, but
not sure yet I really will.
@
text
@d8 6
d40 1
d49 3
d53 1
a53 10
        void             invalidate( const sw_rect &r )
            {
            if( have_invalid_v )
                invalid_v.summ(invalid_v,r);
            else
                {
                invalid_v = r;
                have_invalid_v = Yes;
                }
            }
d55 1
a55 6
        void flush()
            {
            if( !have_invalid_v ) return;
            do_flush(invalid_v);
            have_invalid_v = No;
            }
d71 1
a71 2
        virtual void        resize( const sw_size &s )
            { throw sw_Fail("sw_device::resize","Device is not resizeable", name() ); }
d73 4
@


1.5
log
@viobuf, optimization (sort of:)
@
text
@d8 3
d63 2
d94 2
d144 3
a146 1
        //virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
@


1.4
log
@update
@
text
@d8 3
d36 22
a58 2
    
        //virtual void put_up( const sw_rect &p, const char *char_and_attr_map ) = 0;
d60 1
d68 1
d70 1
a70 1
        virtual void resize( const sw_size &s )
d82 3
a84 1

d89 1
a89 1

@


1.3
log
@works a little
@
text
@d8 3
a28 2
#define INCL_VIO
#include <os2.h>
d47 3
a49 1
    
@


1.2
log
@update
@
text
@d8 3
d26 4
d32 1
a32 1
    private:
d34 4
a37 1
        virtual void put_up( const sw_rect &p, const char *char_and_attr_map ) = 0;
d44 1
a44 1
        virtual void resize( int x, int y )
d52 3
d58 1
d60 1
a60 1
        virtual void resize( int x, int y )
d64 1
a64 1
            sw_abstract_win::resize(x,y);
d70 1
a70 1
    virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
d82 1
a82 1
    virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
d94 1
a94 1
        int tty_write( const char *c, int count );
d96 1
a96 1
        sw_tty_base( ostream& s = cout) : os_v(s) { }
d106 2
a107 1
    virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
d118 1
a118 1
    virtual void put_up( const sw_rect &p, const char *char_and_attr_map );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Tic.c $
d21 2
d27 1
a27 1
        virtual void put_up( sw_rect &p, const char *char_and_attr_map ) = 0;
d30 2
a31 2
        sw_device();
        virtual ~sw_device() {}
d35 1
a35 1
            { throw sw_Fail("sw_device::resize","Can't resize this device", name() ); }
d40 1
a40 1
class sw_vio_base
d45 7
d54 1
a54 1
class sw_vio_device : public sw_device, public sw_vio_base
d56 1
a56 1
    virtual void put_up( sw_rect &p, const char *char_and_attr_map );
d60 3
a62 1
        virtual const char *name() { return "vio"; }
d66 1
a66 1
class sw_avio_device : public sw_device, public sw_vio_base
d68 1
a68 1
    virtual void put_up( sw_rect &p, const char *char_and_attr_map );
d75 1
d78 3
a80 1
    int handle_v;
d82 1
a82 1
        sw_tty_base( int handle ) : handle_v(handle) { }
d88 5
d96 1
a96 1
        virtual const char *name() { return "ansi"; }
d103 1
@

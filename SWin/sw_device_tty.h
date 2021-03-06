/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: tty-style device classes
 *
 *      $Log: sw_device_tty.h $
 *      Revision 1.3  1996/11/18 15:47:54  dz
 *      Before changing event class
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


#ifndef SW_DEVICE_TTY_H
#define SW_DEVICE_TTY_H

#ifndef SW_DEVICE_H
#  include "sw_device.h"
#endif


//#include "sw_types.h"
//#include "sw_win.h"

//#include <iostream.h>
//#include <stack.h>






// Cache must be implemented here
class sw_tty_base_device : public sw_2b_video_memory_device
    {
    protected:
        char *screen_copy;

        void vm_alloc();
        void vm_free();

        ostream& os_v;
        virtual void tty_write( const char *c, int count );
        virtual void tty_set_color( char color ) = 0;
        virtual void tty_gotoxy( int x, int y ) = 0;

        // compare cache with screen_copy, find nearest diff
        sw_point find_mismatch( const sw_point &) const;
        
    public:
        sw_tty_base_device( ostream& s = cout); // : os_v(s); // { }
        ~sw_tty_base_device();
    };


class sw_ansi_device : public sw_tty_base_device
    {
    protected:
        void tty_set_color( char color );
        void tty_gotoxy( int x, int y );
        
    public:
        sw_ansi_device();
        virtual ~sw_ansi_device();
        virtual const char *name(); // { return "ansi"; }
    };

#ifdef unix

class sw_termcap_device : public sw_tty_base_device
    {
    protected:
        void tty_set_color( char color );
        void tty_gotoxy( int x, int y );
        
    public:
        sw_termcap_device();
        virtual ~sw_termcap_device();
        virtual const char *name() { return "termcap, unknown tty type"; }
    };

#endif


#endif // SW_DEVICE_TTY_H


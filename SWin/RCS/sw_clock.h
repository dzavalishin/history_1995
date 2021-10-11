head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.11.22.18.18.01;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.11.22.17.37.34;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@transparency, clock
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: clock class
 *
 *      $Log: sw_clock.h $
 *      Revision 1.2  1996/11/22 18:18:01  dz
 *      ticks
 *
 *      Revision 1.1  1996/11/22 17:37:34  dz
 *      Initial revision
 *
 *
 *      
 *
\*/

#ifndef SW_CLOCK_H
#define SW_CLOCK_H

class sw_clock;
inline void destroy(sw_clock**) {}


#include <time.h>

#include "sw_roundwin.h"
#include "sw_sysdep.h"



class sw_clock : public sw_round_win, private sw_thread
    {
    private:
        sw_color      border_col, display_col, time_col, ticks_off_col, ticks_on_col;

        int           old_tick; // used in tick display code
        int           old_min;  // used in time display code

        struct tm     now;

        bool          first_refresh;
        
        
        void          put_tick();
        void          put_time();
        void          get_time();
        virtual void  run();

    public:
        sw_clock(
                 sw_abstract_win &parent, sw_point pos,
                 sw_color _border_col      = sw_color( sw_color::Cyan,       sw_color::Black ),
                 sw_color _display_col     = sw_color( sw_color::Green,      sw_color::Black ),
                 sw_color _time_col        = sw_color( sw_color::LightGreen, sw_color::Black ),
                 sw_color _ticks_off_col   = sw_color( sw_color::Red,        sw_color::Black ),
                 sw_color _ticks_on_col    = sw_color( sw_color::LightRed,   sw_color::Black )
                );
        virtual ~sw_clock();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };



#endif // SW_CLOCK_H
@


1.2
log
@ticks
@
text
@d8 3
d26 2
d29 1
d33 1
a33 1
class sw_clock : public sw_round_win
d36 6
a41 1
        sw_color   border_col, display_col, ticks_off_col, ticks_on_col;
d43 7
a49 1
        int old_tick; // used in tick display code
a50 1
        void     put_tick(int second_number);
d54 5
a58 4
                 sw_color _border_col      = sw_color( sw_color::Cyan,     sw_color::Black ),
                 sw_color _display_col     = sw_color( sw_color::Green,    sw_color::Black ),
                 sw_color _ticks_off_col   = sw_color( sw_color::Red,      sw_color::Black ),
                 sw_color _ticks_on_col    = sw_color( sw_color::LightRed, sw_color::Black )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_roundwin.h $
d30 3
a32 1
        sw_color   border_col, display_col, ticks_col;
d34 1
d38 4
a41 3
                 sw_color _border_col  = sw_color( sw_color::Cyan,  sw_color::Black ),
                 sw_color _display_col = sw_color( sw_color::Green, sw_color::Black ),
                 sw_color _ticks_col   = sw_color( sw_color::Red,   sw_color::Black )
@

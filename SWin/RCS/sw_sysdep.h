head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@transparency, clock
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes impl.
 *
 *      $Log: sw_sysdep.h $
 *      Revision 1.3  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.2  1996/11/15 16:29:28  dz
 *      mouse event generator written
 *
 *      Revision 1.1  1996/10/10 23:37:07  dz
 *      Initial revision
 *
 *
 *      
 *
\*/


//#include <io.h>

#ifndef SW_THREAD_H
#define SW_THREAD_H

class sw_thread
    {
    private:
          //friend extern "OPTLINK" void sw_thread_kicker(void *val);
        friend void _Optlink sw_thread_kicker(void *val);
        
        volatile int tid_v;
        volatile int stop_v;
    public:
        sw_thread() { tid_v = -1; stop_v = 0; }
        ~sw_thread() { ask_to_stop(); kill(); }

        void ask_to_stop() { stop_v = 1; }
        int asked_to_stop() const { return stop_v; }

        void begin();
        void kill();

        void suspend();
        void resume();

        void wait() const; // for thread to complete

        static void sleep( int msec );
        
        virtual void run() = 0;
        int tid() const { return tid_v; }
    };

struct tm;
void sysdep_getlocaltime( struct tm & );

#endif // SW_THREAD_H


@


1.3
log
@Before changing event class
@
text
@d8 3
d25 2
d50 2
d56 5
@


1.2
log
@mouse event generator written
@
text
@d8 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: device.c $
d23 5
a27 1
        int tid_v;
d29 13
a41 1
        sw_thread();
@

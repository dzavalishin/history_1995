/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes impl.
 *
 *      $Log: sw_sysdep.h $
 *      Revision 1.4  1996/11/23 10:19:59  dz
 *      transparency, clock
 *
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



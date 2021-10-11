/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Buffer class
 *
 *
 *
 *
 *
\*/

#ifndef MPBUFF_H
#define MPBUFF_H

#include "MpPort.h"
#include "MpUtil.h"

#include "Strng.h"



extern void _Optlink   sucker_run( void * ); // Kicker for sucker()

class MP_Buffer
    {
    protected:
        MP_Port &      port;
        int            size;
        volatile int   used;     // How many bytes of buf is really full
        char *         buf;
        
        MP_SpinLock    discard;  // needs to be locked by anyone changing size
        
        MP_Sem         s_under;  // reader sleeps here when buffer is empty
        volatile bool  underflow;// and this flag is true in such a case
        
        MP_Sem         s_over;   // sucker sleeps here when buffer is full
        volatile bool  overflow; // and this flag is true in such a case
        
        friend void    sucker_run( void * ); // Kicker for sucker()
        void           sucker(); // thread that pumps data from port to buffer
        int            s_tid;    // its tid
        volatile bool  stop_v;
        
        void           sleep();
        void           wakeup();
        
    public:
        MP_Buffer( MP_Port &p, int s = 65*1024 ) : port(p), size(s)
            { 
            buf = new char[size]; 
            stop_v = No;
            MP_Message("Initializing buffer sucker");
            s_tid = MP_Thread( sucker_run, this );
            }
        ~MP_Buffer() { stop(); }
        
        void     stop()
            {
            stop_v = Yes;
            s_under.post();
            s_over.post();
            while(s_tid) MP_Sleep(128);
            }
        
        operator const char *() const { return buf; }
        void operator += (int); // Dscard first n characters
        
        bool connected() { return (!stop_v && port.connected()) ? Yes : No; }
        
        int len() const { return used; }
        
        void scan( char c );
        
        void wait_for_data( int len = 1);
        
        int bufsize() const { return size; }
    };






#endif // MPBUFF_H


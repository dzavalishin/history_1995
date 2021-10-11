/************************ ZLib ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: semaphores
 *
 *      $Log: sw_sema.h $
 *      Revision 1.2  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.1  1996/09/30 14:18:41  dz
 *      Initial revision
 *
 *      Revision 1.1  1996/09/23 03:19:42  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef SW_SEMA_H
#define SW_SEMA_H

#include "sw_except.h"
//#include "zstyle.h"
#include "strng.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>






// Event semaphore, unnamed

class sw_sem
    {
    private:
        unsigned long  h;
        friend class sw_timer;
    public:
        sw_sem();
        ~sw_sem();
        void sleep(); // Wait for event
        void post();  // trigger
    
    };


// Resource access semaphore, unnamed
// Use MP_SLKey to lock - this way any exception will
// automatically unlockresources, locked in code, that generated
// exception

class sw_spinlock
    {
    private:
          //unsigned long  h;
        sw_sem           notificator;
        volatile char fast_sem;
        volatile short need_notification;
        
    public:
        sw_spinlock();
        ~sw_spinlock(); // { unlock(); }
        
    protected:
        friend class sw_slkey;
        void lock();   // Wait for resource and lock it
        void unlock(); // Release
    
    };

class sw_slkey
    {
    sw_spinlock &ll;
    public:
        sw_slkey( sw_spinlock &l );// : ll(l); // { ll.lock(); }
        ~sw_slkey(  );                      // { ll.unlock(); }
    };







#endif // SW_SEMA_H





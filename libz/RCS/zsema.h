head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	96.09.23.03.19.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ ZLib ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: semaphores
 *
 *      $Log: MpUtil.h $
 *
 *
 *
\*/

#ifndef ZSEMA_H
#define ZSEMA_H

#include "zexcept.h"
#include "zstyle.h"
#include "zstrng.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>






// Event semaphore, unnamed

class zsem
    {
    private:
        unsigned long  h;
        friend class ztimer;
    public:
        zsem();
        ~zsem();
        void sleep(); // Wait for event
        void post();  // trigger
    
    };


// Resource access semaphore, unnamed
// Use MP_SLKey to lock - this way any exception will
// automatically unlockresources, locked in code, that generated
// exception

class zspinlock
    {
    private:
          //unsigned long  h;
        zsem           notificator;
        volatile short fast_sem;
        volatile short need_notofication;;
        
    public:
        zspinlock();
        ~zspinlock(); // { unlock(); }
        
    protected:
        friend class zslkey;
        void lock();   // Wait for resource and lock it
        void unlock(); // Release
    
    };

class zslkey
    {
    zspinlock &ll;
    public:
        zslkey( zspinlock &l );// : ll(l); // { ll.lock(); }
        ~zslkey(  );                      // { ll.unlock(); }
    };







#endif // ZSEMA_H




@

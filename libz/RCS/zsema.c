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
 *	Module 	: sema impl.
 *
 *      $Log: MpUtil.c $
 *
 *
 *
 *
\*/

#include "zsema.h"

#define INCL_DOSPROCESS      // Sleep
#define INCL_DOSSEMAPHORES   /* Semaphore values */
#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSERRORS       /* DOS error values */
#include <os2.h>

#include <stddef.h>
#include <builtin.h>



        
zsem::zsem() // Wait for event
    {
      // must be shared to use timers!
    APIRET rc = DosCreateEventSem( NULL, &h, DC_SEM_SHARED, FALSE );
    if( NO_ERROR == rc )
        return;
    throw(zxErrno("zsem::zsem","DosCreateEventSem",rc));
    }

zsem::~zsem()
    {
    APIRET rc = DosCloseEventSem(h);
    if( NO_ERROR != rc )
        throw(zxErrno("zsem::zsem","DosCloseEventSem",rc));
    }


void zsem::sleep() // Wait for event
    {
    APIRET rc = DosWaitEventSem(h, (ULONG) SEM_INDEFINITE_WAIT);
    if( rc == NO_ERROR)
        {
        ULONG count;
        DosResetEventSem( h, &count );
        return;
        }
        
    throw(zxErrno("zsem::sleep","DosWaitEventSem",rc));
    }

void zsem::post()  // trigger
    {
    APIRET rc = DosPostEventSem(h);
    if( 
       rc == NO_ERROR || 
       rc == ERROR_ALREADY_POSTED || 
       rc == ERROR_TOO_MANY_POSTS
      )
        return;
        
    throw(zxErrno("zsem::post","DosWaitEventSem",rc));
    }


  // -----------------------------------------------------------------


zspinlock::zspinlock()
    {
    fast_sem = 0;
    need_notification = 0;
      /*
    APIRET rc = DosCreateMutexSem( NULL, &h, 0, 0);
    if( NO_ERROR != rc )
    throw(zxErrno("zspinlock::zspinlock","DosCreateMutexSem",rc));
    */
    }

zspinlock::~zspinlock()
    {
    /*
    APIRET rc = DosCloseMutexSem(h);
    if( NO_ERROR != rc )
    throw(zxErrno("zspinlock::~zspinlock","DosCloseMutexSem",rc));
    */
    }

void zspinlock::lock()   // Wait for resource and lock it
    {
    while(1)
        {
        int fastlock = __sxchg( &fast_sem, 1 );
        if( !fastlock ) return; // We locked it!
        need_notification = 1;
        notificator.sleep();
        }
      /*
       APIRET rc = DosRequestMutexSem(h, (ULONG) SEM_INDEFINITE_WAIT);
       if( NO_ERROR != rc )
       throw(zxErrno("zspinlock::lock","DosRequestMutexSem",rc));
       */
    }

void zspinlock::unlock() // Release
    {
    /*
    APIRET rc = DosReleaseMutexSem(h);
    if( NO_ERROR != rc )
    throw(zxErrno("zspinlock::unlock","DosReleaseMutexSem",rc));
    */

    int fastlock = __sxchg( &fast_sem, 0 );
    if(need_notification)
        {
        need_notification = 0;
        notificator.post();
        }
    }


zslkey::zslkey( zspinlock &l ) : ll(l)
    { ll.lock(); }

zslkey::~zslkey()
    { ll.unlock(); }



@

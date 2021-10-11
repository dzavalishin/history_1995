/************************ ZLib ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: sema impl.
 *
 *      $Log: sw_sema.c $
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
 *
\*/

#include "sw_sema.h"

#define INCL_DOSPROCESS      // Sleep
#define INCL_DOSSEMAPHORES   /* Semaphore values */
#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSERRORS       /* DOS error values */
#include <os2.h>

#include <stddef.h>
#include <builtin.h>



        
sw_sem::sw_sem() // Wait for event
    {
      // must be shared to use timers!
    APIRET rc = DosCreateEventSem( NULL, &h, DC_SEM_SHARED, FALSE );
    if( NO_ERROR == rc )
        return;
    throw(sw_Errno("sw_sem::sw_sem","DosCreateEventSem",rc));
    }

sw_sem::~sw_sem()
    {
    APIRET rc = DosCloseEventSem(h);
    if( NO_ERROR != rc )
        throw(sw_Errno("sw_sem::sw_sem","DosCloseEventSem",rc));
    }


void sw_sem::sleep() // Wait for event
    {
    APIRET rc = DosWaitEventSem(h, (ULONG) SEM_INDEFINITE_WAIT);
    if( rc == NO_ERROR)
        {
        ULONG count;
        DosResetEventSem( h, &count );
        return;
        }
        
    throw(sw_Errno("sw_sem::sleep","DosWaitEventSem",rc));
    }

void sw_sem::post()  // trigger
    {
    APIRET rc = DosPostEventSem(h);
    if( 
       rc == NO_ERROR || 
       rc == ERROR_ALREADY_POSTED || 
       rc == ERROR_TOO_MANY_POSTS
      )
        return;
        
    throw(sw_Errno("sw_sem::post","DosWaitEventSem",rc));
    }


  // -----------------------------------------------------------------


sw_spinlock::sw_spinlock()
    {
    fast_sem = 0;
    need_notification = 0;
      /*
    APIRET rc = DosCreateMutexSem( NULL, &h, 0, 0);
    if( NO_ERROR != rc )
    throw(sw_xErrno("sw_spinlock::sw_spinlock","DosCreateMutexSem",rc));
    */
    }

sw_spinlock::~sw_spinlock()
    {
    /*
    APIRET rc = DosCloseMutexSem(h);
    if( NO_ERROR != rc )
    throw(sw_xErrno("sw_spinlock::~sw_spinlock","DosCloseMutexSem",rc));
    */
    }

void sw_spinlock::lock()   // Wait for resource and lock it
    {
    while(1)
        {
        int fastlock = __cxchg( &fast_sem, 1 );
        if( !fastlock ) return; // We locked it!
        need_notification = 1;
        notificator.sleep();
        }
      /*
       APIRET rc = DosRequestMutexSem(h, (ULONG) SEM_INDEFINITE_WAIT);
       if( NO_ERROR != rc )
       throw(sw_xErrno("sw_spinlock::lock","DosRequestMutexSem",rc));
       */
    }

void sw_spinlock::unlock() // Release
    {
    /*
    APIRET rc = DosReleaseMutexSem(h);
    if( NO_ERROR != rc )
    throw(sw_xErrno("sw_spinlock::unlock","DosReleaseMutexSem",rc));
    */

    int fastlock = __cxchg( &fast_sem, 0 );
    if(need_notification)
        {
        need_notification = 0;
        notificator.post();
        }
    }


sw_slkey::sw_slkey( sw_spinlock &l ) : ll(l)
    { ll.lock(); }

sw_slkey::~sw_slkey()
    { ll.unlock(); }




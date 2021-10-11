/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpUtil.c $
 *      Revision 1.6  1996/07/09 15:29:52  dz
 *      SpinLock for dumper
 *
 *      Revision 1.5  1996/07/09 14:13:33  dz
 *      MAJOR BUG FIX in thread start code - using
 *      DYNAMICALLY allocated memory to pass composite parameters.
 *
 *      Revision 1.4  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/07/02 01:55:57  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include "MpUtil.h"

#define INCL_DOSPROCESS      // Sleep
#define INCL_DOSSEMAPHORES   /* Semaphore values */
#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSERRORS       /* DOS error values */
#include <os2.h>

#include <stddef.h>
#include <builtin.h>



  // NB!
  // Parameter, passed to new thread must NOT be placed on stack
  // (or it will be overwritten by owning thread in unpredictable moment)
  // or in a static memory (not reentrant), so only dynamic allocation
  // wil do.

struct thread_par
    {
    void ( *_Optlink func) ( void * );
    void *object;
    };

void _Optlink thread_catcher( void *p )
    {
    thread_par *tpp = (thread_par *)p;
    thread_par tp = *tpp;
    delete tpp;
    try
        {
        //MP_Message("thread 0x%x( 0x%x ), tp.func, tp.object );
        tp.func(tp.object);
        }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s)",  ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        //fprintf( stderr, "Exc in %s: %s (%s)",  ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }
    }


int MP_Thread( void _Optlink f( void * ), void *object )
    {
    struct thread_par tp;
    tp.func = f; 
    tp.object = object;
      //MP_Message("thread(0x%x, 0x%x)", f, object );
    int tid = _beginthread( thread_catcher, NULL, 64*1024, new thread_par(tp) );
      //MP_Message("thread = %d", tid );
    return tid;
    }

int MP_get_tid()
    {
    PTIB   ptib = NULL;
    PPIB   ppib = NULL;
    
    APIRET rc = DosGetInfoBlocks(&ptib, &ppib);
    
    if (rc != NO_ERROR)
        return 0;
    
    return ptib->tib_ptib2->tib2_ultid;
    }



  // -----------------------------------------------------------------



//        unsigned long  h;
        
MP_Sem::MP_Sem() // Wait for event
    {
      // must be shared to use timers!
    APIRET rc = DosCreateEventSem( NULL, &h, DC_SEM_SHARED, FALSE );
    //MP_Message("made sema %d", h );
    if( NO_ERROR == rc )
        return;

    //MP_Message("ctor: exc in sema %d", h );
    throw(Mp_Ex_Errno("MP_Sem::MP_Sem","DosCreateEventSem",rc));
    }

MP_Sem::~MP_Sem()
    {
    APIRET rc = DosCloseEventSem(h);
    //MP_Message("closed sema %d", h );
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_Sem::MP_Sem","DosCloseEventSem",rc));

    //MP_Message("~: exc in sema %d", h );
    }


void MP_Sem::sleep() // Wait for event
    {
    //MP_Message("sleep on sema %d", h );
    APIRET rc = DosWaitEventSem(h, (ULONG) SEM_INDEFINITE_WAIT);
    if( rc == NO_ERROR)
        {
        ULONG count;
        DosResetEventSem( h, &count );
        return;
        }
        
    //MP_Message("sleep: exc in sema %d", h );
    throw(Mp_Ex_Errno("MP_Sem::sleep","DosWaitEventSem",rc));
    }

void MP_Sem::post()  // trigger
    {
    //MP_Message("posting sema %d", h );
    APIRET rc = DosPostEventSem(h);
    if( 
       rc == NO_ERROR || 
       rc == ERROR_ALREADY_POSTED || 
       rc == ERROR_TOO_MANY_POSTS
      )
        return;
        
    //MP_Message("post: exc in sema %d", h );
    throw(Mp_Ex_Errno("MP_Sem::post","DosWaitEventSem",rc));
    }


  // -----------------------------------------------------------------

  // BUG! (Not a bug really, but ineffective impl.)
  // This stuff should use more effective semaphores

MP_SpinLock::MP_SpinLock()
    {
    APIRET rc = DosCreateMutexSem( NULL, &h, 0, 0);
    
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_SpinLock::MP_SpinLock","DosCreateMutexSem",rc));
    //MP_Message("created slock %d", h );
    }

MP_SpinLock::~MP_SpinLock()
    {
    APIRET rc = DosCloseMutexSem(h);
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_SpinLock::~MP_SpinLock","DosCloseMutexSem",rc));
    //MP_Message("deleted slock %d", h );
    }

void MP_SpinLock::lock()   // Wait for resource and lock it
    {
    //MP_Message("locking slock %d", h );
    APIRET rc = DosRequestMutexSem(h, (ULONG) SEM_INDEFINITE_WAIT);
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_SpinLock::lock","DosRequestMutexSem",rc));
    //MP_Message("locked slock %d", h );
    }

void MP_SpinLock::unlock() // Release
    {
    //MP_Message("unlocking slock %d", h );
    APIRET rc = DosReleaseMutexSem(h);
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_SpinLock::unlock","DosReleaseMutexSem",rc));
    }


MP_SLKey::MP_SLKey( MP_SpinLock &l ) : ll(l)
    { ll.lock(); }

MP_SLKey::~MP_SLKey()
    { ll.unlock(); }



// -----------------------------------------------------------------

void MP_Timer::arm_once( int usec )
    {
    disarm();
    APIRET rc = DosAsyncTimer( usec, (HSEM)sem_h, &h );
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_Timer::arm_once","DosAsyncTimer",rc));
    }
    
void MP_Timer::arm_repeated( int usec )
    {
    disarm();
    APIRET rc = DosStartTimer( usec, (HSEM)sem_h, &h );
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_Timer::arm_repeated","DosStartTimer",rc));
    }
    
void MP_Timer::disarm()
    {
    APIRET rc = DosStopTimer( h );
    if( NO_ERROR != rc && ERROR_TS_HANDLE != rc )
        throw(Mp_Ex_Errno("MP_Timer::disarm","DosStopTimer",rc));
    }




// -----------------------------------------------------------------

void MP_Sleep(int ms)
    {
    DosSleep( ms );
    }


static MP_SpinLock messager;

void MP_Message( const char *fmt, ... )
    {
    
      // Prevent reentrance. В очередь, сукины дети, в очередь.
    MP_SLKey key( messager );
    int tid = MP_get_tid();
    
    va_list argptr;
    
    va_start(argptr, fmt);
    
      //vsprintf( buf, fmt, argptr);
    
      //fprintf( stderr, "MPipe: " );
    fprintf( stderr, "%2d- ", tid );
    vfprintf( stderr, fmt, argptr);
    fprintf( stderr, "\n" );
    fflush(stderr);
    
    if( logfile != 0 )
        {
        fprintf( logfile, "%2d- ", tid );
        vfprintf( logfile, fmt, argptr);
        fprintf( logfile, "\n" );
        fflush(logfile);
        }
    
    va_end(argptr);
    }




#define MP_DEBUG 1
#define MP_DEBUG_DUMP 0

static MP_SpinLock dumper;

void MP_Dump( const char * msg, const char * buf, int len )
    {
#if MP_DEBUG && MP_DEBUG_DUMP
      // Prevent reentrance. Own spinlock, not PM_Message's
    MP_SLKey key( dumper );
    const bblen = 130*1024;
    static char bb[bblen+1];

    if( len > bblen/2 )
        {
        len = len > bblen/2;
        MP_Message("MP_Dump: truncated data!");
        }

    memcpy( bb, buf, len );

    for( int i = 0; i < len; i++ )
        if( bb[i] < ' ' ) bb[i] = '?';

    bb[len] = '\0';
    MP_Message("%s dump asc: <%s>", msg, bb );

#endif
    }

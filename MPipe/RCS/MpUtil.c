head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.07.09.15.29.52;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.09.14.13.33;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@SpinLock for dumper
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpUtil.c $
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

static MP_SpinLock dumper;

void MP_Dump( const char * msg, const char * buf, int len )
    {
#if MP_DEBUG
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
@


1.5
log
@MAJOR BUG FIX in thread start code - using
DYNAMICALLY allocated memory to pass composite parameters.
@
text
@d8 4
d56 1
d78 1
a78 1
static int MP_get_tid()
a90 45
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



void MP_I_am_crazy()
    {
    printf("MP_I_am_crazy()\n");
    }







d237 34
d274 2
d279 2
@


1.4
log
@Debugging has begone
@
text
@d8 2
a9 5
 *      Revision 1.3  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.2  1996/07/02 04:33:28  dz
 *      writing
d31 106
a136 1
 
a277 57
struct thread_par
    {
    void ( *_Optlink func) ( void * );
    void *object;
    };

static void _Optlink thread_catcher( void *p )
    {
    int tid = _getTIBvalue( offsetof( TIB, tib_ordinal ) );

    thread_par *tp = (thread_par *)p;
    try
        {
        tp->func(tp->object);
        }
    catch( Mp_Ex ex )
        {
        printf("Exc in %s: %s (%s), tid %d\n", 
               ex.where.c_str(), ex.what.c_str(), ex.why.c_str(),
               tid );
        }
    }


int MP_Thread( void _Optlink f( void * ), void *object )
    {
    struct thread_par tp;
    tp.func = f; 
    tp.object = object;
    int tid = _beginthread( thread_catcher, NULL, 64*1024, &tp );
    MP_Message("thread = %d", tid );
    return tid;
    }



static MP_SpinLock messager;

void MP_Message( const char *fmt, ... )
    {
    // Prevent reentrance. В очередь, сукины дети, в очередь.
    MP_SLKey key( messager );
    
    va_list argptr;
    
    va_start(argptr, fmt);
    
    //vsprintf( buf, fmt, argptr);
    
    fprintf( stderr, "MPipe: " );
    vfprintf( stderr, fmt, argptr);
    fprintf( stderr, "\n" );
    fflush(stderr);
    
    va_end(argptr);
    }

d284 1
a284 1
    static char bb[bblen];
d297 1
d301 1
a301 1
    }@


1.3
log
@*** empty log message ***
@
text
@d8 3
d31 2
d39 1
a39 1
    // must be shared to use timers!
d41 6
a46 2
    if( NO_ERROR != rc )
        throw(Mp_Ex_Errno("MP_Sem::MP_Sem","DosCreateEventSem",rc));
d52 1
d55 2
d62 1
d71 1
d77 1
d86 1
d91 1
a91 1
// -----------------------------------------------------------------
d93 2
a94 2
// BUG! (Not a bug realy, but ineffective impl.)
// This stuff should use more effective semaphores
d102 1
d110 1
d115 1
d119 1
d124 1
d131 7
d160 1
a160 1
    if( NO_ERROR != rc )
d175 7
d184 3
a186 1
    void ( *_Optlink f) ( void * ) = (void ( *_Optlink ) ( void * ))p;
d189 1
a189 1
        f(0);
d193 3
a195 1
        printf("Exc in %s: %s (%s)\n", ex.where, ex.what, ex.why );
d200 1
a200 1
int MP_Thread( void _Optlink f( void * ) )
d202 6
a207 1
    return _beginthread( thread_catcher, NULL, 64*1024, f );
d210 4
d216 3
d228 1
d233 13
d247 1
d249 2
d252 1
d254 2
@


1.2
log
@writing
@
text
@d8 3
d22 1
d139 6
d147 12
d160 20
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpChan.h $
d112 1
a112 1
    APIRET rc = DosAsyncTimer( usec, sem_h, &h );
d120 1
a120 1
    APIRET rc = DosStartTimer( usec, sem_h, &h );
@

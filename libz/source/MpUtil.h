/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpUtil.h $
 *      Revision 1.10  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.9  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.8  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.7  1996/07/09 01:20:26  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
 *
 *
 *
 *
 *
\*/

#ifndef MPUTIL_H
#define MPUTIL_H

#include "style.h"
#include "strng.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "MpExcept.h"

#define DEBUG 1


// logging
extern FILE *logfile;



// integral types

typedef long             MP_Off_t;
typedef long             MP_Off;

// Functions

void MP_Sleep( int msec );
void MP_Message( const char *fmt, ... );
void MP_Dump( const char * msg, const char * buf, int len );
int  MP_Thread( void _Optlink f( void * ), void *object );

void MP_I_am_crazy();



// Event semaphore, unnamed

class MP_Sem
    {
    private:
        unsigned long  h;
        friend class MP_Timer;
    public:
        MP_Sem();
        ~MP_Sem();
        void sleep(); // Wait for event
        void post();  // trigger
    
    };


// Resource access semaphore, unnamed
// Use MP_SLKey to lock - this way any exception will
// automatically unlockresources, locked in code, that generated
// exception

class MP_SpinLock
    {
    private:
        unsigned long  h;
        
    public:
        MP_SpinLock();
        ~MP_SpinLock(); // { unlock(); }
        
    protected:
        friend class MP_SLKey;
        void lock();   // Wait for resource and lock it
        void unlock(); // Release
    
    };

class MP_SLKey
    {
    MP_SpinLock &ll;
    public:
        MP_SLKey( MP_SpinLock &l );// : ll(l); // { ll.lock(); }
        ~MP_SLKey(  );                      // { ll.unlock(); }
    };

// timer

class MP_Timer
    {
    private:
        unsigned long  h;
        unsigned long  sem_h;
        
    public:
        MP_Timer( const MP_Sem & s ) : sem_h(s.h), h(0) {}
        ~MP_Timer() { disarm(); }

        void arm_once( int usec );
        void arm_repeated( int usec );

        void disarm();
    
    };







#endif // MPUTIL_H





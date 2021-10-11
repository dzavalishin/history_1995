head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.06.05.13.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@*** empty log message ***
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpExcept.h $
 *      Revision 1.1  1996/07/06 05:13:48  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#ifndef MPEXCEPT_H
#define MPEXCEPT_H

//#include "style.h"
//#include "strng.h"
//#include "stdlib.h"

class string;



// Eceptions

class Mp_Ex
    {
    public:
        string   where, what, why;
    };

class Mp_Ex_Abort : public Mp_Ex
    {
    public:
        Mp_Ex_Abort( const char* wh )
            {
            where = wh;
            what = "operation aborted";
            why = "";
            }
    };


class Mp_Ex_EOF : public Mp_Ex
    {
    public:
        Mp_Ex_EOF( const char* wh )
            {
            where = wh;
            what = "EOF";
            why = "";
            }
    };


class Mp_Ex_NoMemory : public Mp_Ex
    {
    public:
        Mp_Ex_NoMemory( const char* wh )
            {
            where = wh;
            what = "out of memory";
            why = "";
            }
    };


class Mp_Ex_Fail : public Mp_Ex
    {
    public:
        Mp_Ex_Fail( const char* wh, const char* wa, const char* wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class Mp_Ex_Errno : public Mp_Ex
    {
    public:
        Mp_Ex_Errno( const char* wh, const char* wa, long e )
            {
            where = wh;
            what = wa;
            char es[100];
            sprintf( es, "%ld", e );
            why = es;
            }
    };



class Mp_Ex_Disconnect : public Mp_Ex
    {
    public:
        Mp_Ex_Disconnect( const char* wh )
            {
            where = wh;
            what = "channel disconnect";
            why = "";
            }
    };


#endif // MPEXCEPT_H




@


1.1
log
@Initial revision
@
text
@d7 3
a9 6
 *      $Log: MpUtil.h $
 *      Revision 1.4  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
d17 2
a18 90
#ifndef MPUTIL_H
#define MPUTIL_H

#include "style.h"
#include "strng.h"
#include "stdlib.h"

//#include "vector.h"




// integral types

typedef long             MP_Off_t;
typedef long             MP_Off;

// Functions

void MP_Sleep( int msec );


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
        MP_SLKey( MP_SpinLock &l ) : ll(l) { ll.lock(); }
        ~MP_SLKey(  )                      { ll.unlock(); }
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


// Threads

//#define MP_Thread( f ) _beginthread( f, NULL, 64*1024, NULL );
d20 3
a22 4
int MP_Thread( void _Optlink f( void * ) );
//    {
//    return _beginthread( f, NULL, 64*1024, NULL );
//    }
d24 1
d110 1
a110 1
#endif // MPUTIL_H
@

head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.09.01.20.26;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.06.05.12.56;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.20.14.53;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@something goes through, framer/frame classes
are work somehow.
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpUtil.h $
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




@


1.9
log
@debugging depacketizer
@
text
@d8 3
d36 2
@


1.8
log
@Debugging has begone
@
text
@d8 3
d29 1
d35 3
d51 2
@


1.7
log
@*** empty log message ***
@
text
@d8 1
a8 1
 *      Revision 1.6  1996/07/07 10:01:14  dz
a10 6
 *      Revision 1.5  1996/07/06 05:12:56  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/07/02 04:33:28  dz
 *      writing
 *
a115 83
#if 0
// see mpexcept.h
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
#endif
@


1.6
log
@*** empty log message ***
@
text
@d8 3
d48 2
d94 2
a95 2
        MP_SLKey( MP_SpinLock &l ) : ll(l) { ll.lock(); }
        ~MP_SLKey(  )                      { ll.unlock(); }
a117 8
// Threads

//#define MP_Thread( f ) _beginthread( f, NULL, 64*1024, NULL );

int MP_Thread( void _Optlink f( void * ) );
//    {
//    return _beginthread( f, NULL, 64*1024, NULL );
//    }
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d28 2
a29 1
#include "stdlib.h"
d31 1
a31 1
//#include "vector.h"
d44 2
d125 2
d207 1
a207 1

@


1.4
log
@writing
@
text
@d8 3
d107 3
d111 4
d189 10
@


1.3
log
@writing
@
text
@d7 4
a10 1
 *      $Log: MpChan.h $
d45 1
a45 1
        
d93 1
a93 1
        MP_Timer( unsigned long sema_h ) : sem_h(sema_h), h(0) {}
@


1.2
log
@update
@
text
@d1 12
a12 1

d17 1
d19 6
d29 1
d44 2
d52 54
d117 1
a117 1
        Mp_Ex_Abort( const string & wh )
d129 13
a141 1
        Mp_Ex_EOF( const string & wh )
d144 1
a144 1
            what = "the end";
d148 28
@


1.1
log
@Initial revision
@
text
@d3 14
d41 7
a47 6
    Mp_Ex_Abort( const string & wh )
        {
        where = wh;
        what = "operation aborted";
        why = "";
        }
d53 7
a59 6
    Mp_Ex_EOF( const string & wh )
        {
        where = wh;
        what = "the end";
        why = "";
        }
d63 1
@

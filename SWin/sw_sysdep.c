/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes impl.
 *
 *      $Log: sw_sysdep.c $
 *      Revision 1.4  1996/11/23 10:19:59  dz
 *      transparency, clock
 *
 *      Revision 1.3  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.2  1996/11/15 16:29:28  dz
 *      mouse event generator written
 *
 *      Revision 1.1  1996/10/10 23:37:07  dz
 *      Initial revision
 *
 *
 *      
 *
\*/


#include "sw_sysdep.h"

#ifdef __OS2__

#define INCL_KBD
#define INCL_VIO
#define INCL_DOSPROCESS
#define INCL_DOSDATETIME
#include <os2.h>

#include <stdlib.h>
#include <time.h>



extern "OPTLINK" void sw_thread_kicker(void *val)
    {
    sw_thread *obj = (sw_thread*)val;
    obj->run();
    obj->tid_v = -1;
    }

void sw_thread::begin()
    {
    tid_v = _beginthread( sw_thread_kicker, 0, 16*1024, this );
    }

void sw_thread::kill()
    {
    if( tid_v < 0 ) return;

    if( DosKillThread( tid_v ) == 0 )
        tid_v = -1;
    }

void sw_thread::suspend()
    {
    if( tid_v < 0 ) return;
    DosSuspendThread( tid_v );
    }


void sw_thread::resume()
    {
    if( tid_v < 0 ) return;
    DosResumeThread( tid_v );
    }


void sw_thread::wait() const
    {
    if( tid_v < 0 ) return;
    TID tid = tid_v;
    DosWaitThread( &tid, DCWW_WAIT );
    }    

void sw_thread::sleep(int msec )
    {
    DosSleep( msec );
    }


  // ------------------------------- getlocaltime ---------------------------------------

void sysdep_getlocaltime( struct tm &out )
    {
    DATETIME dt;
    DosGetDateTime( &dt );
    out.tm_hour = dt.hours;
    out.tm_min = dt.minutes;
    out.tm_sec = dt.seconds;
    out.tm_mday = dt.day;
    out.tm_mon = dt.month-1;
    out.tm_year = dt.year;
    out.tm_wday = dt.weekday;
    out.tm_yday = 0; // Potential BUG!!
    }


#endif







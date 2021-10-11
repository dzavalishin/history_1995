/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies - DOS
 *
 *      $Log: dep_dos_i.c $
 *      Revision 1.6  1997/03/26 10:58:43  dz
 *      USE_INT15
 *
 *      Revision 1.5  1997/03/22 10:39:23  dz
 *      int15 based timeslice release
 *
 *      Revision 1.4  1997/03/17 08:00:10  dz
 *      sleep remade
 *
 *      Revision 1.3  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.2  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.1  1996/08/04 08:18:43  dz
 *      Initial revision
 *
 *      Revision 1.3  1996/01/22 19:25:18  dz
 *      C Set, mostly works
 *
 *      Revision 1.2  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#include <i86.h>
#include <dos.h>

#ifndef __386__
#error This code will not work in real mode
#endif


OS_Type::OS_Type( void )
    {
    v = DOS;
    name_v = "DOS/32";
    }




#define SYSDEP_ILOCK_WAIT
void  ilock::wait( int time )
    {
#if 1
    frip_sleep( time );
#else
    while( (time -= 2) > 0 )
        {
        union REGS rr;
        sleep(1);
        for( int i = 10; i--; )
            int386( 0x28, &rr, &rr );
        }
#endif
    }

#define SYSDEP_FRIP_SLEEP

#define USE_INT15 1

void frip_sleep( int sec )
    {
    while( (sec -= 2) > 0 )
        {
        union REGS rr;
#if USE_INT15
          //mov ah,86h
          //mov cx,(старшее слово длительности задержки в миллисекундах)
          //mov dx,(младшее слово)
          //int 15h
            rr.x.eax = 0x8600;
            rr.x.ecx = (2000>>16);
            rr.x.edx = 2000%0xFFFFu;
            int386( 0x15, &rr, &rr );
#else
        sleep(1);
        for( int i = 10; i--; )
            int386( 0x28, &rr, &rr );
#endif
        }
    }



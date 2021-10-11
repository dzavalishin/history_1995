head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	97.03.26.10.58.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	97.03.22.10.39.23;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	97.03.17.08.00.10;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@USE_INT15
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies - DOS
 *
 *      $Log: dep_dos_i.c $
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
            rr.x.eax = 0x86;
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


@


1.5
log
@int15 based timeslice release
@
text
@d8 3
d62 1
a62 1
#define USE_INT15
@


1.4
log
@sleep remade
@
text
@d8 3
d36 5
a51 1
#ifdef __386__
a52 3
#else
            int86( 0x28, &rr, &rr );
#endif
d58 3
d66 10
a77 1
#ifdef __386__
a78 2
#else
            int86( 0x28, &rr, &rr );
@


1.3
log
@Ver. 34
@
text
@d8 3
d36 4
a39 1
    while( time -= 2 )
d50 1
d53 15
@


1.2
log
@Exceptions added, not compiled
@
text
@d8 3
a44 4




@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: dep_dos.ic $
@

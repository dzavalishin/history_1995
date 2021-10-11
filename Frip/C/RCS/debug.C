head	1.4;
access;
symbols
	Beta_12:1.1;
locks;
comment	@ * @;


1.4
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Exceptions added, not compiled
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Debugging support
 *
 *      $Log: debug.C $
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.2  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#include "frip.h"
#include "ansi.h"
#include <malloc.h>

#if TESTALLOC
void heap_dump(const char *where)
    {
#ifdef __IBMCPP__
    _heap_check();
#else
    struct _heapinfo h_info; 
    int heap_status; 
    
    int used = 0;
    
    h_info._pentry = NULL; 
    for(;;) 
        { 
        heap_status = _heapwalk( &h_info ); 
        if( heap_status != _HEAPOK ) break; 
#if DUMPALLOC
        printf( "  %s block at %Fp of size %4.4X\n",
               (h_info._useflag == _USEDENTRY ? "USED" : "FREE"), 
               h_info._pentry, h_info._size ); 
#endif
        if( h_info._useflag == _USEDENTRY ) used += h_info._size;
        } 
    
    char buf [100];
    if( used >= 10240 )
        sprintf( buf, "%s: total %dK allocated", where, used/1024 );
    else
        sprintf( buf, "%s: total %d bytes allocated", where, used );
    
    debug(buf);
    
    switch( heap_status ) 
        { 
        case _HEAPEND:       // printf( "OK - end of heap\n" );
            break; 
        case _HEAPEMPTY:     // printf( "OK - heap is empty\n" );
            break; 
        case _HEAPBADBEGIN:  Error( "heap is damaged" );
            break; 
        case _HEAPBADPTR:    Error( "bad pointer to heap" );
            break; 
        case _HEAPBADNODE:   Error( "bad node in heap" );
            break; 
        } 
#endif
    } 
#endif


void debug(string s)
    {
    if( conf.debug() )
        {
        if( conf.ansi() )
            fprintf( stderr, ansi_magenta "Debug:"ansi_normal
                    " %s\n", (const char *)s );
        else
            fprintf( stderr, "Debug: %s\n", (const char *)s );
        Log( "Debug: ", s );
        }
    }



@


1.3
log
@Before exceptions.
@
text
@d8 3
@


1.2
log
@FripMail written
@
text
@d7 4
a10 1
 *      $Log: debug.c $
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:$
a18 4
#ifndef VERDEF
#  define VERDEF "Unknown version"
#endif

d22 3
d64 1
@

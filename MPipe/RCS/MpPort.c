head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@debugging depacketizer
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: port class
 *
 *      $Log: MpPort.c $
 *      Revision 1.2  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/07/02 01:55:57  dz
 *      Initial revision
 *
 *      Revision 1.3  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.2  1996/06/30 20:14:53  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "MpPort.h"



void MP_LoopPort::set_pair( MP_LoopPort &p )
    { 
    pair = &p;
    connected_v = Yes;
    pwait.post();
    }

int MP_LoopPort::read( char *data, int size, bool sure )
    {
    //MP_Message( "MP_LoopPort::read waits for pair" );
    while( pair == 0 )
        pwait.sleep();
    
    //MP_Message( "MP_LoopPort::read waits for read spinlock" );
    MP_SLKey key(pair->rlock);   // obtain access

    //MP_Message( "MP_LoopPort::read looks for data" );
    while( connected_v && pair->wlen <= 0 )     // no data?
        pair->rwait.sleep();
    
    //MP_Message( "MP_LoopPort::read looks for 'sure' data" );
    while( sure && connected_v && pair->wlen < size ) // not enough data?
        pair->rwait.sleep();
    
    if( !connected_v ) throw Mp_Ex_Disconnect("MP_LoopPort::read");
    
    if( pair->wlen < size ) size = pair->wlen;
    memcpy( data, (void *)pair->wdata, size );
    pair->wlen -= size;
    pair->wwait.post();

    //MP_Dump( "MP_LoopPort::read", data, size );
    
    return size;
    }

int MP_LoopPort::write( char *data, int size )
    {
    MP_SLKey key(wlock);  // Lock it up!

    //MP_Dump( "MP_LoopPort::write", data, size );
    
    wdata = data;
    wlen = size;
    
    rwait.post(); // wakeup reader
    
    while( connected_v && wlen > 0 )
        wwait.sleep();

    return size - wlen;
    }
    
    
    
void MP_LoopPort::hangup()                      // Blocking!!
    {
    connected_v = No;
    rwait.post();
    wwait.post();
    if( pair ) 
        {
        pair->connected_v = No;
        pair->rwait.post();
        pair->wwait.post();
        }
    }

@


1.2
log
@Debugging has begone
@
text
@d8 3
d41 1
d45 1
d48 1
d52 1
d62 2
d72 1
a72 1
    MP_Dump( "MP_LoopPort::write", data, size );
d75 1
a75 1
    wlen - size;
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpPort.h $
d32 1
d36 1
a36 1
int MP_LoopPort::read( char *data, int size )
d46 4
a49 1
    if( !connected_v ) return 0;
d55 2
d62 2
@

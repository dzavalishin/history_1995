head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.07.10.07.48.29;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.09.01.49.24;	author dz;	state Exp;
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
@spinlocked
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: packet queue implementation
 *
 *      $Log: MpPQueue.c $
 *      Revision 1.2  1996/07/09 01:49:24  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/07/02 01:55:57  dz
 *      Initial revision
 *
 *
 *
 *
 *
 *
\*/

#include "MpPQueue.h"


MP_PQueue::MP_PQueue()
    {
    }

MP_PQueue::~MP_PQueue() 
    {
    while(!empty())
        discard();
    }

bool MP_PQueue::empty() const
    {
    return v.empty() ? Yes : No;
    }

int MP_PQueue::queued() const
    {
      //MP_Message("%d on q", v.size());
    return v.size();
    }

MP_Packet const * MP_PQueue::peek() const
    {
    if( v.size() == 0 )
        return 0;
    return v[0];
    }



// detach & delete that one peek looks at
void MP_PQueue::discard()
    {
    MP_SLKey key(vlock);
    if( !v.empty() )
        {
        delete v.front();
        v.pop_front();
        }
    }

// detach, don't delete
MP_Packet * MP_PQueue::get()     
    {
    MP_SLKey key(vlock);
    MP_Packet *p = v.front();
    v.pop_front();
    return p;
    }

void MP_PQueue::put( MP_Packet *p )
    {
    MP_SLKey key(vlock);
    v.push_back(p);
      //MP_Message("put: %d on q", v.size());
    }



@


1.2
log
@*** empty log message ***
@
text
@d8 3
d57 1
d68 1
d76 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpPQueue.h $
d20 31
d68 7
@

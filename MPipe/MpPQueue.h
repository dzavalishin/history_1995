/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: packet queue
 *
 *      $Log: MpPQueue.h $
 *      Revision 1.5  1996/07/10 07:48:46  dz
 *      spinlocked
 *
 *      Revision 1.4  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/06/30 22:58:25  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#ifndef MP_PQUEUE_H
#define MP_PQUEUE_H

#include "MpPacket.h"
#include "deque.h"


class MP_PQueue
    {
    private:
        deque<MP_Packet *> v;
        MP_SpinLock        vlock;
        
    public:
        MP_PQueue();
        ~MP_PQueue();
        
        bool               empty() const;
        int                queued() const;
        MP_Packet const *  peek() const;
        void               discard();
        void               put( MP_Packet *p );
        MP_Packet *        get();     // detach, don't delete

    };

#endif // MP_PQUEUE_H


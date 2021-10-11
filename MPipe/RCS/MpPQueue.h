head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.07.10.07.48.46;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.09.01.49.17;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@spinlocked
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: packet queue
 *
 *      $Log: MpPQueue.h $
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

@


1.4
log
@Debugging has begone
@
text
@d8 2
a9 5
 *      Revision 1.3  1996/07/09 01:49:17  dz
 *      *** empty log message ***
 *
 *      Revision 1.2  1996/07/02 01:55:57  dz
 *      writing
d31 1
d34 2
a35 2
        MP_PQueue(); // {}
        ~MP_PQueue(); //   { while(!empty())    discard();    }
d37 5
a41 14
        bool               empty() const; // const { return v.empty() ? Yes : No; }
        int                queued() const; // const { MP_Message("%d on q", v.size()); return v.size(); }
        MP_Packet const *  peek() const; //  const { if( v.size() == 0 ) return 0; return v[0]; }
        void               discard(); // detach & delete that one peek looks at
            /*
            {
            if( !v.empty() ) 
                {
                delete v.front();
                pop_front();
                }
            }
            */
        void               put( MP_Packet *p ); // { v.push_back(p); MP_Message("put: %d on q", v.size());}
a42 8
            /*
            {
            MP_Packet *p = v.front();
            pop_front();
            return p;
            }
            */
        
@


1.3
log
@*** empty log message ***
@
text
@d8 3
d39 3
a41 3
        bool               empty(); // const { return v.empty() ? Yes : No; }
        int                queued(); // const { MP_Message("%d on q", v.size()); return v.size(); }
        MP_Packet const *  peek(); //  const { if( v.size() == 0 ) return 0; return v[0]; }
@


1.2
log
@writing
@
text
@d8 3
d33 2
a34 6
        MP_PQueue() {}
        ~MP_PQueue() 
            {
            while(!empty())
                discard();
            }
d36 3
a38 2
        bool               empty() const { return v.empty() ? Yes : No; }
        MP_Packet const *  peek()  const { if( v.size() == 0 ) return 0; return v[0]; }
d49 1
a58 1
        void               put( MP_Packet *p ) { v.push_back(p); }
@


1.1
log
@Initial revision
@
text
@d5 5
a9 1
 *	Module 	:	
a10 1
 *      $Log: MpStat.h $
d21 1
d27 1
a27 1
        vector<MP_Packet *> v;
d37 2
a38 2
        bool               empty() { return v.size() == 0 ? Yes : No; }
        MP_Packet const *  peek();
d40 17
a56 2
        MP_Packet const *  get();     // detach, don't delete
        void               put( MP_Packet * );
d58 2
@

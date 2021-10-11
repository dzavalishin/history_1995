head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.10.08.19.31;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.06.01.50.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@something goes through, framer/frame classes
are work somehow.
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Session class impl.
 *
 *      $Log: MpSess.c $
 *      Revision 1.4  1996/07/10 08:19:31  dz
 *      debugging. packetizing, packet queues
 *      and wakeup/init state machine seems to be ok.
 *
 *      Revision 1.3  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.2  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/07/06 01:50:29  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "MpSess.h"
#include "MpPacket.h"

#include <time.h> // time()

#include "MpUtil.h"


void prth_run( void *o ) 
    {
    try {  ((MP_Session*)o)->pr_thread(); }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), port reader thread",
               ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }

    ((MP_Session*)o)->pr_tid = 0;
    MP_Message( "port reader thread finished" );
    }

void pwth_run( void *o ) 
    {
    try { ((MP_Session*)o)->pw_thread(); }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), port writer thread",
                   ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }

    ((MP_Session*)o)->pw_tid = 0;
    MP_Message( "port writer thread finished" );
    }


// port reader - decodes/timestamps packets, puts them on rpq, kicks rpq_sem, blocks on port read
void MP_Session::pr_thread()
    {
    while( 1 )
        {
        if( stop_us ) break;

          // throws exception if port disconnects
        MP_Packet *p = MP_Packet::receive( inbuf );
        
        // we will be blocked in new for most of 
        // the time so check here for xfer shutdown
        if( stop_us || p == NULL ) break;
        
        switch( p->type() )
            {
            
            case MP_PT_Wakeup:
            case MP_PT_Init:
            case MP_PT_XOFF:
            case MP_PT_XON:
            case MP_PT_Query:
            case MP_PT_Ack:
            case MP_PT_Nak:
            case MP_PT_RStop1:
            case MP_PT_RStop2:
                    {
                    // gain access to sender receive q.
                    MP_SLKey key(rspq_sl); 
                    rspq.put(p);
                    s_sem.post();
                    }
                break;
                
            case MP_PT_Data:
            case MP_PT_MiniData:
            case MP_PT_EOF:
            case MP_PT_SStop:
                    {
                    // gain access to receiver receive q.
                    MP_SLKey key(rrpq_sl);
                    rrpq.put(p);
                    r_sem.post();
                    }
                break;

            case MP_PT_HBT:
                delete p;
                break;
                
            default:
                delete p;
                break;
            }
        }
    
    }
    

// port writer - encodes packets from spq, sleeps on spq_sem if none, blocks on port write
void MP_Session::pw_thread()
    {
    while(1)
        {
        if( stop_us ) break;
        spq_sem.sleep();
        //MP_Message("MP_Session::pw_thread() awaken" );
        if( stop_us ) break;
            {
            MP_SLKey key(spq_sl);
            if( spq.empty() )
                continue;
            MP_Packet const *p = spq.peek();
            //MP_Message("MP_Session::pw_thread() got & sending '%c'", p->type() );
            p->send( port );
            // Tell send state machine that we sent some data
            if( p->type() == MP_PT_Data )
                {
                s_ndata--;
                  // Sender might be blocked due to queue overflow.
                  // Wake him up - time to send some data packets.
                s_sem.post(); 
                }

            spq.discard();
            }
        }
    }

unsigned long MP_Session::get_random_challenge() const
    {
    // BUG - use random gen.
    return time(NULL) ^ 0x3ABDEC12; // Just something :)
    }

int MP_Session::read( char *data, int size, bool sure )    // Blocking!!
    {
    if( !sure )
        return rwin.read( data, size );
    
    int ret = size;
    
    while( size > 0 )
        {
        int got = rwin.read( data, size );
        if( got <= 0 )
            throw Mp_Ex_Fail("MP_Session::read","read error","rwin.read");
        data += got;
        size -= got;
        }
    
    return ret;
    }

int MP_Session::write( char *data, int size )   // Blocking!!
    {
    int r = swin.write( data, size );
      // Sender might be blocked because of no data to
      // send. Kick him.
    s_sem.post();
    return r;
    }


MP_Session::MP_Session( bool caller, MP_Port &_p ) : port(_p), inbuf(_p)
    {
    stop_us = No;
    options_valid = No;
    is_caller = caller;
    challenge = is_caller ? 0 : get_random_challenge();
    
    s_state = MP_SS_Init; // Do init
    r_state = MP_SS_Data; // Here we start
    
    connected_v = Yes;

    s_eof_known = No;
    r_eof_known = No;
    
    //MP_Message( "c'tor this = %x", this );
    
      // Make sure everthing used by threads is init'ed above!
    
    r_tid = MP_Thread( rth_run, this ); // p thread must be running first
    MP_Message("receiver state machine tid = %d", r_tid);
    s_tid = MP_Thread( sth_run, this );
    MP_Message("sender state machine tid = %d", s_tid);
    pr_tid = MP_Thread( prth_run, this );
    MP_Message("port reader/packet disassembler tid = %d", pr_tid);
    pw_tid = MP_Thread( pwth_run, this );
    MP_Message("port writer/packet assembler tid = %d", pw_tid);
    
    }

MP_Session::~MP_Session()
    {
    abort();
    MP_Sleep(0); // Give up timeslice to help them finish quickly
    while( r_tid || s_tid || pr_tid || pw_tid )
        {
        MP_Sleep(128); MP_Message( "r%d s%d pr%d pw%d", r_tid, s_tid, pr_tid, pw_tid );
        }
    }

void MP_Session::wait_for_hangup()
    {
    while(connected_v) hangup_sem.sleep();
    }

void MP_Session::abort()
    {
    stop_us = Yes;
    r_sem.post();
    s_sem.post();
    spq_sem.post();
    port.hangup();
    rwin.abort();
    swin.abort();
    inbuf.stop();
    }

void MP_Session::hangup()                        // Blocking!!
    {
    abort();
    port.hangup();
    connected_v = No;
    hangup_sem.post();
    }

  // NB! Pass new() objects only!
void MP_Session::send_pkt( MP_Packet *p )
    {
      // BUG! Need overflow sema!
      //MP_Message("going to enqueue pkt '%c'", p->type() );
    while( spq.queued() > MP_Max_Queued_For_Send ) MP_Sleep( 256 );
    //MP_Message("really enqueuing pkt '%c'", p->type() );
    spq.put( p );
    spq_sem.post();
    }






@


1.4
log
@debugging. packetizing, packet queues
and wakeup/init state machine seems to be ok.
@
text
@d8 4
d137 8
a144 1
            if( p->type() == MP_PT_Data ) s_ndata--;
d177 5
a181 1
    return swin.write( data, size );
d218 1
d221 1
a221 1
        MP_Sleep(2000); MP_Message( "r%d s%d pr%d pw%d", r_tid, s_tid, pr_tid, pw_tid );
@


1.3
log
@debugging depacketizer
@
text
@d8 3
d89 1
d101 1
d105 1
d109 1
d178 1
a178 1
    r_state = MP_SS_None; // Wait for now
d181 3
a188 1
    MP_Message("Initializing session receiver state machine");
d190 1
a190 1
    MP_Message("Initializing session sender state machine");
d192 1
a192 1
    MP_Message("Initializing session port reader/packet disassembler");
d194 1
a194 1
    MP_Message("Initializing session port writer/packet assembler");
d196 1
d205 1
a205 2
        MP_Sleep(128); MP_Message( "r%d s%d pr%d pw%d", r_tid, s_tid, pr_tid, pw_tid );
        MP_Sleep(128);
d223 1
@


1.2
log
@Debugging has begone
@
text
@d8 3
d24 2
d84 1
d95 1
d123 1
a123 1
            MP_Message("MP_Session::pw_thread() got & sending '%c'", p->type() );
d175 1
a175 1
    MP_Message( "c'tor this = %x", this );
d224 10
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpSess.h $
d19 3
d24 7
a30 1
    ((MP_Session*)o)->pr_thread();
d32 1
d37 7
a43 1
    ((MP_Session*)o)->pw_thread();
d45 1
d54 3
a56 1
        // throws exception if port disconnects
d107 1
d109 2
d116 1
d118 2
a123 2
    

d125 5
d131 1
a131 4
// ----------------------------------------------------------------------


void sucker_run( void *o )
d133 15
a147 2
    ((MP_Buffer*)o)->sucker();
    ((MP_Buffer*)o)->s_tid = 0;
d150 1
a150 2

void MP_Buffer::sleep()
d152 1
a152 3
    overflow = Yes;
    s_over.sleep();
    overflow = No;
d155 2
a156 1
void MP_Buffer::wakeup()
d158 23
a180 1
    if(overflow) s_over.post();
d183 1
a183 1
void MP_Buffer::wait_for_data( int len )
d185 2
a186 1
    while(used < len) 
d188 2
a189 4
        underflow = Yes;
        s_under.sleep();
        if( !connected() ) throw Mp_Ex_Disconnect("MP_Buffer::wait_for_data");
        underflow = No;
d193 4
d198 1
a198 2

void MP_Buffer::sucker()
d200 7
a206 27
    while(!stop_v)
        {
        const rbs = 128;
        char rbuf[rbs];
        
        int r = port.read( rbuf, rbs );
        
        if( r < 0 ) { stop_v = Yes; break; }
        
        int rbshift = 0;
        while( r > 0 )
            {
            while(used >= size) sleep();
            
                {
                MP_SLKey key( discard ); // Prevent discard from happening here
                int len = size-used;
                if( len > r ) len = r;
                memcpy( buf+used, rbuf+rbshift, len );
                r -= len;
                used += len;
                rbshift += len;
                if( underflow ) s_under.post();
                }
            }
        
        }
d209 1
a209 2

void MP_Buffer::operator += ( int len )
d211 4
a214 7
    MP_SLKey key( discard ); // Prevent sucker from messing around
    if( len <= 0 || len > used )
        len = used; // better that screw things up?
    
    used -= len;
    memcpy( buf, buf+len, used - len );
    wakeup();
a218 2
void MP_Buffer::scan( char c )
    {
a219 16
    while(1)
        {
            {
            MP_SLKey key( discard ); // Prevent sucker from messing around
            for( char *cp = buf; cp-buf < used; cp++ )
                if( *cp == c )
                    {
                    cp++;
                    (*this) += cp-buf;
                    return;
                    }
            
            // we scanned all the buffer and nothing is of interest,
            // so clear it
            used = 0;
            }
a220 1
        if( !connected() ) throw Mp_Ex_Disconnect("MP_Buffer::wait_for_data");
a221 2
        wait_for_data();
        }
a222 1
    }
@

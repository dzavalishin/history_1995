/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Session class impl.
 *
 *      $Log: MpSess.c $
 *      Revision 1.5  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
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







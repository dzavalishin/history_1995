/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:  Sender state machine
 *
 *      $Log: MpSend.c $
 *      Revision 1.10  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.9  1996/07/10 08:19:31  dz
 *      debugging. packetizing, packet queues
 *      and wakeup/init state machine seems to be ok.
 *
 *      Revision 1.8  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.7  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 * BUGS:
 *     - Sender does not join small frames whel looks for data to send.
 *       As a result feeding Session with one-byte writes results in
 *       sending of a one byte packets.
 *
 *
\*/

#include "MpSess.h"
#include "MpPacket.h"


void sth_run( void *o ) // Kicker for s_thread()
    {
    try { ((MP_Session*)o)->s_thread(); }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), sender state machine thread",
                   ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }
    
    ((MP_Session*)o)->s_tid = 0;
    MP_Message( "s_thread finished" );
    }




void MP_Session::s_process_wakeup()
    {
    MP_Message( "MP_Session::s_process_wakeup" );
    if( is_caller ) challenge = rspq.peek()->challenge();
    send_pkt( new MP_PK_Init((MP_Options)our_options_on,challenge) );
    rspq.discard();
    }


void MP_Session::s_process_options( MP_Options ioptions )
    {
    MP_Message( "MP_Session::s_process_options" );
    if( options_valid ) return;
    
    // BUG?
    options = ioptions;
    options |= our_options_on;
    options &= ~our_options_off;
    
    options_valid = Yes;
    }

void MP_Session::s_process_init()
    {
    MP_Message( "MP_Session::s_process_init" );
    long ic = rspq.peek()->challenge();
    s_process_options( rspq.peek()->options() );
    rspq.discard();
    
    if( is_caller ) challenge = ic;
    else if( challenge != ic )
        {
        MP_Message( "Invalid challenge %d instead of %d", ic, challenge );
        abort();
        return;
        }
    s_state = MP_SS_Data;
    r_state = MP_SS_Data; // Enable receiver
    r_sem.post();         // Kick receiver off his sema sleep
    }


void MP_Session::s_process_ack()
    {
    MP_Frame ack = rspq.peek()->frame();
    rspq.discard();
    
    MP_Message( "MP_Session::s_process_ack(%d, %d)", ack.off, ack.size );
    swin.mark_as_received( ack );
    }

void MP_Session::s_process_nack()
    {
    MP_Message( "MP_Session::s_process_nack" );
    swin.mark_as_non_received( rspq.peek()->frame() );
    rspq.discard();
    }


// Caller must be SURE there is something to send
void MP_Session::s_send_some_data()
    {
      // Keep not more than 5 data packets in send Q
    if( (!swin.have_non_acked()) || s_ndata >= 5 )
        return;
    
    MP_Message( "MP_Session::s_send_some_data" );
    
    MP_Frame f = swin.find_frame_to_send();
    send_pkt( new MP_PK_Data(f,swin.get(f)) );
    swin.mark_as_sent(f);
    
    s_ndata++;
    }

void MP_Session::s_query(const MP_Frame &f)
    {
    MP_Message( "MP_Session::s_query" );
    send_pkt( new MP_PK_Query(f) );
    swin.mark_as_queried(f);
    }




//
// try not to keep q locked as it can prevent
// packet receiver from feeding other q
//

void MP_Session::s_thread()
    {
    MP_Timer  timer(s_sem); // Used to kick sem after a timeout
    s_sem.post(); // Make sure on entry to this loop we'll not deadlock on sema
    
    
    while( 1 )
        {
        
        if( stop_us ) break;
        
        //MP_Message( "s_thread fall asleep in state %c", s_state );
        s_sem.sleep(); // Sleep on sema
        //MP_Message( "s_thread awaken in state %c", s_state );
        
        if( stop_us ) break;
        
        switch( s_state )
            {
            default:
                MP_Message( "Invalid s_state %d (%c)", s_state, s_state );
                abort();
                return;
                
            case MP_SS_Init:
                timer.arm_once(Wakeup_Timeout); // retrigger us after this time
                
                if( spq.queued() < 3 ) // Don't overflow queue
                    {
                    send_pkt( new MP_PK_Wakeup(challenge) );
                    if( !is_caller )
                        send_pkt( new MP_PK_Init((MP_Options)our_options_on,challenge)); // Speedup
                    }
                
                    {
                    MP_SLKey key(rspq_sl); // will access rspq
                    if( rspq.empty() ) break;
                    switch(rspq.peek()->type())
                        {
                        case MP_PT_Wakeup:    s_process_wakeup();   continue;
                        case MP_PT_Init:      s_process_init();     break;
                        default:
                            rspq.discard();
                        }
                    break;
                    }
                
                
                
            case MP_SS_Data:
                    {
                    MP_SLKey key(rspq_sl);
                    if( !rspq.empty() )
                        {
                        switch(rspq.peek()->type())
                            {
                            case MP_PT_Wakeup:     s_process_wakeup();   break;
                            case MP_PT_Init:       rspq.discard();       break;
                            case MP_PT_Ack:        s_process_ack();      break;
                            case MP_PT_Nack:       s_process_nack();     break;
                            default:               rspq.discard();       break;
                            }
                        }
                    }

                  // I don't really like this way of determining
                  // completeness of transmission. BUG?
                if( s_eof_known && !swin.have_non_acked() )
                    {
                    s_state = MP_SS_SEOF;
                    break;
                    }
                
                s_send_some_data();
                //else timer.arm_once(Wakeup_Timeout); // retrigger us after this time
                
                break;
                
                
            case MP_SS_SEOF:
                timer.arm_once(SEOF_Timeout); // retrigger us after this time
                send_pkt( new MP_PK_EOF(s_eof_pos));
                    {
                    MP_SLKey key(rspq_sl);
                    if( rspq.empty() ) break;
                    switch(rspq.peek()->type())
                        {
                        case MP_PT_RStop1:   
                            rspq.discard();
                            s_state = MP_SS_SStop;
                            break;
                            
                        default:
                            rspq.discard();
                            break;
                        }
                    }
                
                if( s_ndata < 5 ) // Keep not more than 5 data packets in send Q
                    s_send_some_data();
                    break;
                    
            case MP_SS_SStop:
                    {
                    MP_SLKey key(rspq_sl); 
                    rspq.discard(); // Ignore all that comes here
                    }
                timer.arm_once(SStop_Timeout); // retrigger us after this time
                send_pkt( new MP_PK_SStop());
                try_to_hangup();
                break;
                
            }
        }
    
    }


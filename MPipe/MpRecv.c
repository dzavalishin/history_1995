/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpRecv.c $
 *      Revision 1.9  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.8  1996/07/10 08:19:31  dz
 *      debugging. packetizing, packet queues
 *      and wakeup/init state machine seems to be ok.
 *
 *      Revision 1.7  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.6  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.5  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.3  1996/06/30 22:58:25  dz
 *      *** empty log message ***
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

#include "MpSess.h"
#include "MpPacket.h"

void rth_run( void *o ) // Kicker for r_thread()
    {
    try {  ((MP_Session*)o)->r_thread(); }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), receiver state machine thread",
                   ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }
    
    ((MP_Session*)o)->r_tid = 0;
    MP_Message( "r_thread finished" );
    }




void MP_Session::r_process_query()
    {
    MP_Frame f = rrpq.peek()->frame();
    rrpq.discard();
      // BUG: Ignoring queries yet...
    
      // Here we should send acks/nacks for all the frames in this
      // interval so that sender will know what to send and what is
      // received ok.
    }


void MP_Session::r_process_eof()
    {
    r_eof_pos = rrpq.peek()->eof();
    rrpq.discard();
    r_eof_known = Yes;
    }

/*
void MP_Session::r_process_sstop()
    {
    rrpq.discard();
    }
*/
    
void MP_Session::r_process_data()
    {
    const MP_Packet *p = rrpq.peek();
    MP_Frame f = p->frame();
    MP_Frame ack = rwin.put( f, p->data() );
    send_pkt( new MP_PK_Ack(ack) );
    MP_Message( "got data [%d, %d[, sent ack [%d, %d[",
               f.off, f.end(), ack.off, ack.end() );
    rrpq.discard();

      // Here we know which frame we got now.
      // If we have non-received frames before this one it seems
      // smart to tell sender he should resend them asap.
      // He will resend them anyway, but telling him will speed up
      // things like chat and improve protocol smoothness in
      // general.
    }






void MP_Session::r_thread()
    {
    
    while( 1 )
        {
        
        if(stop_us) break;
        
        //MP_Message( "r_thread fall asleep in state %c", r_state );
        r_sem.sleep(); // Sleep on sema
        //MP_Message( "r_thread awaken in state %c", r_state );
        
        if(stop_us) break;
        
        switch( r_state )
            {
            default:
                MP_Message( "Invalid r_state %c", r_state );
                abort();
                return;
                
            case MP_SS_None:  // receiver state machine is not started yet
                MP_Message( "Awaken in MP_SS_None. Why?.." );
                continue;
                
            case MP_SS_Data:
                
                    {
                    MP_SLKey key(rrpq_sl);
                    if( !rrpq.empty() )
                        {
                        MP_Message("receiver in data state got '%c'", rrpq.peek()->type() );
                        switch(rrpq.peek()->type())
                            {
                            case MP_PT_Data:      r_process_data();     break;
                            case MP_PT_Query:     r_process_query();    break;
                            case MP_PT_EOF:       r_process_eof();      break;
                            default:              rrpq.discard();       break;
                            }
                        }
                    }
                  // We can hang in thsi state as long, as we need, 'cause
                  // sender will keep bugging us with EOF packets until we
                  // switch to Stop1.
                  // In fact, we can hang here forever if client side will not
                  // eat all the data for some reason.
                  // This should be fixed somehow to make sure
                  // we don't hang on line too much.
                  // The reason I did not did it now is that session
                  // is completely stopped when transmission is complete
                  // ans windows discarded, possibly pereventing client to
                  // get rest of his data.
                if( r_eof_known && rwin.completed_up_to(r_eof_pos) )
                    {
                    r_state = MP_SS_RStop1;
                      // Jump to RStop1 immediately, don't wait for pkt
                    r_sem.post(); 
                    }
                
                break;
                
                
                
            case MP_SS_RStop1:
                    {
                    MP_SLKey key(rrpq_sl);
                    if( !rrpq.empty() && rrpq.peek()->type() == MP_PT_SStop )
                        {
                        r_state = MP_SS_RStop2;
                          // Jump to RStop2 immediately, don't wait for pkt
                        r_sem.post();
                        }
                    else
                        send_pkt( new MP_PK_RStop1());
                    
                    rrpq.discard();
                    }
                break;
                
            case MP_SS_RStop2:
                send_pkt( new MP_PK_RStop2());
                    {
                    MP_SLKey key(rrpq_sl);
                    rrpq.discard(); // Ignore all that comes here
                    }
                try_to_hangup();
                    break;
                    
            }
        }
    
    }


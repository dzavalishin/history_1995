head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.10.08.19.31;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.20.14.53;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@something goes through, framer/frame classes
are work somehow.
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpSend.c $
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
 *      Revision 1.6  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.5  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.4  1996/07/02 01:55:57  dz
 *      writing
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
        
        MP_Message( "s_thread fall asleep in state %c", s_state );
        s_sem.sleep(); // Sleep on sema
        MP_Message( "s_thread awaken in state %c", s_state );
        
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

@


1.9
log
@debugging. packetizing, packet queues
and wakeup/init state machine seems to be ok.
@
text
@d8 4
d105 1
a105 2
    MP_Message( "MP_Session::s_process_ack" );
    swin.mark_as_received( rspq.peek()->frame() );
d107 3
d120 1
d123 4
a127 2
    if( !swin.have_non_acked() )
        return;
d215 1
a215 4
                
                if( s_ndata < 5 ) // Keep not more than 5 data packets in send Q
                    s_send_some_data();
                
d219 1
d221 5
@


1.8
log
@debugging depacketizer
@
text
@d8 3
d43 7
a49 1
    ((MP_Session*)o)->s_thread();
d59 1
d68 1
d81 1
d101 1
d108 1
d116 1
d129 1
d153 1
d155 1
a155 1
        //MP_Message( "s_thread awaken in state %c", s_state );
@


1.7
log
@Debugging has begone
@
text
@d8 3
d42 1
a42 1
MP_Message( "s_thread finished" );
a130 2
    MP_Message( "s_thread this = %x", this );
    
d138 1
a138 1
        MP_Message( "s_thread awaken in state %c", s_state );
@


1.6
log
@*** empty log message ***
@
text
@d8 3
d39 1
d74 1
a74 1
        fprintf( stderr, "Invalid challenge %d instead of %d", ic, challenge );
d105 2
a123 5
{
MP_Timer  timer(s_sem); // Used to kick sem after a timeout
s_sem.post(); // Make sure on entry to this loop we'll not dedlock on sema

while( !stop_us )
d125 2
d128 1
a128 1
    s_sem.sleep(); // Sleep on sema
d130 2
a131 1
    switch( s_state )
d133 19
a151 15
        default:
            fprintf( stderr, "Invalid s_state %c", s_state );
            abort();
            return;
            
        case MP_SS_Init:
            timer.arm_once(Wakeup_Timeout); // retrigger us after this time
            send_pkt( new MP_PK_Wakeup(challenge) );
            if( !is_caller ) 
                send_pkt( new MP_PK_Init((MP_Options)our_options_on,challenge)); // Speedup
            
                {
                MP_SLKey key(rspq_sl); // will access rspq
                if( rspq.empty() ) break;
                switch(rspq.peek()->type())
d153 3
a155 4
                    case MP_PT_Wakeup:    s_process_wakeup();   continue;
                    case MP_PT_Init:      s_process_init();     break;
                    default:
                        rspq.discard();
d157 40
a197 2
                }
                    
d199 4
a202 5
            
        case MP_SS_Data:
                {
                MP_SLKey key(rspq_sl);
                if( !rspq.empty() )
d204 2
d208 8
a215 5
                        case MP_PT_Wakeup:     s_process_wakeup();   break;
                        case MP_PT_Init:       rspq.discard();       break;
                        case MP_PT_Ack:        s_process_ack();      break;
                        case MP_PT_Nack:       s_process_nack();     break;
                        default:               rspq.discard();       break;
d218 6
a223 20
                }
            
            if( s_ndata < 5 ) // Keep not more than 5 data packets in send Q
                s_send_some_data();
            
            // I don't really like this way of determining
            // completeness of transmission. BUG?
            if( s_eof_known && !swin.have_non_acked() )
                s_state = MP_SS_SEOF;
            
            break;


        case MP_SS_SEOF:
            timer.arm_once(SEOF_Timeout); // retrigger us after this time
            send_pkt( new MP_PK_EOF(s_eof_pos));
                {
                MP_SLKey key(rspq_sl);
                if( rspq.empty() ) break;
                switch(rspq.peek()->type())
d225 2
a226 8
                    case MP_PT_RStop1:   
                        rspq.discard();
                        s_state = MP_SS_SStop;
                        break;

                    default:
                        rspq.discard();
                        break;
d228 6
a233 16
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
        
d235 1
a236 2
    
}
@


1.5
log
@writing
@
text
@d8 3
d112 4
a139 2
            if( rspq.empty() ) break;
            switch(rspq.peek()->type())
d141 10
a150 10
                case MP_PT_Wakeup:
                    s_process_wakeup();
                    continue;
                    
                case MP_PT_Init:
                    s_process_init();
                    break;
                    
                default:
                    rspq.discard();
a151 1
            break;
a155 2
            
            if( !rspq.empty() ) 
d157 2
a158 1
                switch(rspq.peek()->type())
d160 8
a167 17
                    case MP_PT_Wakeup:
                        s_process_wakeup();
                        break;

                    case MP_PT_Init: // ignore
                        rspq.discard();
                        break;
                    
                    case MP_PT_Ack:
                        s_process_ack();
                    
                    case MP_PT_Nack:
                        s_process_nack();
                    
                    default:
                        rspq.discard();
                        break;
d181 1
a181 1
            
a184 3
        
            if( rspq.empty() ) break;
            switch(rspq.peek()->type())
d186 13
a198 8
                case MP_PT_RStop1:
                    rspq.discard();
                    s_state = MP_SS_SStop;
                    break;
                
                default:
                    rspq.discard();
                    break;
d200 1
a200 1
            
d206 4
a209 1
            rspq.discard(); // Ignore all that comes here
@


1.4
log
@writing
@
text
@d8 3
d41 1
a41 1
    P_Send_Init(challenge);
d96 1
a96 1
    P_Send_Data( f, swin.get(f), f.size );
d100 6
d129 3
a131 2
            P_Send_Wakeup(challenge);
            if( !is_caller ) P_Send_Init(challenge); // Speedup
d190 1
a190 1
            P_Send_SEOF(s_eof_pos);
d212 1
a212 1
            P_Send_SStop();
@


1.3
log
@update
@
text
@d8 3
d94 1
d103 1
a103 1

d119 1
a119 1
            set_s_sem_timer(Wakeup_Timeout); // retrigger us after this time
d170 2
d179 1
a179 1
            set_s_sem_timer(SEOF_Timeout); // retrigger us after this time
d201 1
a201 1
            set_s_sem_timer(SStop_Timeout); // retrigger us after this time
@


1.2
log
@update
@
text
@d8 3
d19 3
d23 1
a23 1
void MP_Session::sth_run( void *o ) // Kicker for s_thread()
d26 1
a26 1
    s_tid = 0;
d34 1
a34 1
    if( is_sender ) challenge = rspq.peek()->challenge();
d40 12
d55 1
a55 1
    MP_Options ioptions = rspq.peek()->options();
d57 2
a58 1
    if( is_sender ) challenge = ic;
d61 1
a61 1
        error( "Invalid challenge %d instead of %d", ic, challenge );
d67 1
a67 1
    rpq_sem.post(); // Kick receiver to make sure no deadlock
d73 1
a73 1
    mark_as_received( rspq.peek()->frame() );
d79 1
a79 1
    mark_as_non_received( rspq.peek()->frame() );
d89 2
a90 1
    P_Send_Data( find_frame_to_send() );
d110 1
a110 1
            error( "Invalid s_state %c", s_state );
d117 1
a117 1
            if( !is_sender ) P_Send_Init(challenge); // Speedup
d196 1
a196 1
            P_Send_SStop(challenge);
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d177 1
@

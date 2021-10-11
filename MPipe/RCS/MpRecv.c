head	1.9;
access;
symbols;
locks;
comment	@ * @;


1.9
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.10.08.19.31;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.04.33.28;	author dz;	state Exp;
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


1.9
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
 *      $Log: MpRecv.c $
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
    MP_Message( "sending ack for %d, %d", ack.off, ack.size );
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
        
        MP_Message( "r_thread fall asleep in state %c", r_state );
        r_sem.sleep(); // Sleep on sema
        MP_Message( "r_thread awaken in state %c", r_state );
        
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

@


1.8
log
@debugging. packetizing, packet queues
and wakeup/init state machine seems to be ok.
@
text
@d8 4
d59 1
a59 1
    rrpq.peek()->frame();
d61 5
d85 5
d91 7
@


1.7
log
@debugging depacketizer
@
text
@d8 3
d39 7
a45 1
    ((MP_Session*)o)->r_thread();
d92 1
a92 1
        //MP_Message( "r_thread fall asleep in state %c", r_state );
d115 1
@


1.6
log
@Debugging has begone
@
text
@d8 3
d83 1
a83 1
        MP_Message( "r_thread fall asleep in state %c", s_state );
d85 1
a85 1
        MP_Message( "r_thread awaken in state %c", s_state );
d92 1
a92 1
                fprintf( stderr, "Invalid s_state %c", s_state );
d97 1
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d35 1
a72 3
{

while( !stop_us )
d75 1
a75 3
    r_sem.sleep(); // Sleep on sema
    
    switch( r_state )
d77 21
a97 13
        default:
            fprintf( stderr, "Invalid s_state %c", s_state );
            abort();
            return;
            
        case MP_SS_None:  // receiver state machine is not started yet
            continue;

        case MP_SS_Data:
            
                {
                MP_SLKey key(rrpq_sl);
                if( !rrpq.empty() )
d99 2
a100 1
                    switch(rrpq.peek()->type())
d102 7
a108 4
                        case MP_PT_Data:      r_process_data();     break;
                        case MP_PT_Query:     r_process_query();    break;
                        case MP_PT_EOF:       r_process_eof();      break;
                        default:              rrpq.discard();       break;
d111 40
a150 27
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
d152 2
a153 3
                    r_state = MP_SS_RStop2;
                    // Jump to RStop2 immediately, don't wait for pkt
                    r_sem.post();
d155 4
a158 16
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
        
d160 1
a161 2
    
}
@


1.4
log
@writing
@
text
@d8 3
d39 2
a40 2
    rspq.peek()->frame();
    rspq.discard();
d46 2
a47 2
    r_eof_pos = rspq.peek()->eof();
    rspq.discard();
d54 1
a54 1
    rspq.discard();
d60 1
a60 1
    rspq.discard();
a87 1
            if( !rspq.empty() ) 
d89 2
a90 1
                switch(rspq.peek()->type())
d92 7
a98 15
                    case MP_PT_Data:
                        r_process_data();
                        break;

                    case MP_PT_Query: // ignore
                        r_process_query();
                        break;
                    
                    case MP_PT_EOF:
                        r_process_eof();
                        break;
                    
                    default:
                        rspq.discard();
                        break;
a100 1
            
a123 1
            if( !rspq.empty() && rspq.peek()->type() == MP_PT_SStop )
d125 11
a135 3
                r_state = MP_SS_RStop2;
                // Jump to RStop2 immediately, don't wait for pkt
                r_sem.post(); 
a136 4
            else
                send_pkt( new MP_PK_RStop1());
                    
            rspq.discard(); 
d141 4
a144 1
            rspq.discard(); // Ignore all that comes here
@


1.3
log
@*** empty log message ***
@
text
@d8 3
d137 1
a137 1
                P_Send_RStop1();
d143 1
a143 1
            P_Send_RStop2();
@


1.2
log
@update
@
text
@d8 3
d19 2
d22 1
a22 1
void MP_Session::rth_run( void *o ) // Kicker for r_thread()
d25 1
a25 1
    r_tid = 0;
d31 1
a31 1
void MP_Session::s_process_query()
d33 1
a33 1
    rspq.peek()->frame()
d38 1
a38 1
void MP_Session::s_process_eof()
d45 2
a46 2

void MP_Session::s_process_sstop()
d50 3
a52 2

void MP_Session::s_process_data()
d73 1
a73 1
            error( "Invalid s_state %c", s_state );
d87 1
a87 1
                        s_process_data();
d91 1
a91 1
                        s_process_query();
d94 2
a95 2
                    case MP_PT_SEOF:
                        s_process_eof();
d104 12
a115 1
            if( r_eof_known && !rwin.have_non_acked() )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d125 1
@

head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.07.06.02.43;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.17.37.36;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Debugging has begone
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Channels      
 *
 *      $Log: MpChan.h $
 *      Revision 1.5  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/07/07 06:02:43  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.2  1996/06/30 17:37:36  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "style.h"

#include "vector.h"

#include "MpUtil.h"
#include "MpPort.h"
#include "MpState.h"
#include "MpChanType.h"




extern void _Optlink   sth_run( void * ); // Kicker for s_thread()
extern void _Optlink   rth_run( void * ); // Kicker for r_thread()




class MP_Channels
    {
    private:
        char                     next_schan;
        
        bool                     is_caller;       // Am I caller, or what?
        volatile bool            stop_us;         // Shut all the things down!
        
        MP_Port &                port;
        
        vector <MP_RChannel*>    rchan;          // in a channel number order
        vector <MP_SChannel*>    schan;          // in order of priority
        
        void                     sort_schannels(); // In order of priority
        
        bool                     try_to_send( int chan );
        void                     s_thread();     // sender/packer thread
        void                     r_thread();     // receiver/unpacker thread
        
        //static void            sth_run( void * ); // Kicker for s_thread()
        //static void            rth_run( void * ); // Kicker for r_thread()
        
        int                      s_tid, r_tid;
        
        MP_State                 s_state;
        MP_State                 r_state;
        
        bool                     s_un_eof; // packer source EOF
        bool                     s_pk_eof; // packer dest. EOF
        bool                     r_pk_eof; // unpacker source EOF
        bool                     r_un_eof; // packer dest. EOF
        
        // Posted by sending channels when new data arrived
        // sender have to post this if it returned zero from
        // send method before.
        MP_Sem                   sready;
            
    protected:
        bool        schan_used(int ch);
        int         get_new_schan_no();
        
    public:
        
        MP_Channels( bool caller, MP_Port &_p );
        ~MP_Channels();

        void             abort();

        MP_SChannel &  new_schan( const char* type, int prio, const char* param );
        MP_RChannel &  new_rchan( const char* type, int num,  const char* param );
    };

@


1.5
log
@*** empty log message ***
@
text
@d8 3
d59 3
d95 1
a95 1
        MP_RChannel &  new_rchan( const char* type, int prio, const char* param );
@


1.4
log
@*** empty log message ***
@
text
@d8 3
a24 3
// Odd (1, 3, 5...) channels are transferring data from caller to answerer.
// Even ones - back.

d32 1
a33 1
//#include "stdlib.h" // _beginthread
a35 1
//#define MP_Thread( f ) _beginthread( f, NULL, 64*1024, NULL );
a42 34
class MP_Channel
    {
    private:
        int     ch_no;
    public:
        MP_Channel( int num ) : ch_no(num) {}
        
        int     number() const { return ch_no; }
    };

// Receiving channel
class MP_RChannel : public MP_Channel
    {
    public:
        MP_RChannel( int num ) : MP_Channel(num) {}
        
        // Called by multiplexer to pass received data
        void receive( const char *data, int len );
    };

// Sending one
class MP_SChannel : public MP_Channel
    {
    public:
        MP_SChannel( int num, MP_Sem &sender_ready ) : MP_Channel(num) {}
    
        // Called by multiplexer to get data to send
        int send( const char *data, int len );
    };





d51 1
a51 1
        MP_Port &                p;
d53 2
a54 2
        vector <MP_RChannel*>    schan;
        vector <MP_SChannel*>    rchan;
d75 1
a75 1
        MP_Sem                   sender_ready; 
d77 4
d83 7
a89 44
        MP_Channels( bool caller, MP_Port &_p ) : p( _p )
            {
            next_schan = 1;
            stop_us = No;
            is_caller = caller;
            
            r_tid = MP_Thread( rth_run ); // p thread must be running first
            s_tid = MP_Thread( sth_run );
            }
        
        ~MP_Channels()
            {
            abort();
            while( r_tid || s_tid )
                MP_Sleep(128);
            
            while( schan.size() ) { delete schan.back(); schan.pop_back(); }
            while( rchan.size() ) { delete rchan.back(); rchan.pop_back(); }
            }
            
        abort()
            {
            stop_us = Yes;
            }
        
        bool schan_used(int ch)
            {
            for( int i = schan.size(); i-- > 0;  )
                if( schan[i]->number() == ch )
                    return Yes;
            return No;
            }
        
        int get_new_schan_no()
            {
            int ch;
            
            do
                {
                ch = ((next_schan++) << 1) + (is_caller ? 1 : 0);
                } while( schan_used(ch) );
            return ch;
            }
        
@


1.3
log
@writing
@
text
@d5 1
a5 1
 *	Module 	:	
d8 3
d29 1
d33 1
a33 1
#include "stdlib.h" // _beginthread
d36 1
a36 1
#define MP_Thread( f ) _beginthread( f, NULL, 64*1024, NULL );
d59 3
a61 1
    
d68 1
a68 1
        MP_SChannel( int num ) : MP_Channel(num) {}
d70 2
d81 1
a81 1
        char                   next_schan;
d83 2
a84 2
        bool                   is_caller;       // Am I caller, or what?
        volatile bool          stop_us;         // Shut all the things down!
d86 1
a86 1
        MP_Port &              p;
d91 2
a92 2
        void                   s_thread();     // sender/packer thread
        void                   r_thread();     // receiver/unpacker thread
d97 1
a97 4
        int                    s_tid, r_tid;
        
        MP_State               s_state;
        MP_State               r_state;
d99 2
a100 4
        bool                   s_un_eof; // packer source EOF
        bool                   s_pk_eof; // packer dest. EOF
        bool                   r_pk_eof; // unpacker source EOF
        bool                   r_un_eof; // packer dest. EOF
d102 10
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d22 46
d80 2
a81 2
        vector <MP_Channel>    schan;
        vector <MP_Channel>    rchan;
d86 2
a87 2
        static void            sth_run( void * ); // Kicker for s_thread()
        static void            rth_run( void * ); // Kicker for r_thread()
d101 1
a101 1
        MP_Channels( bool caller, MP_Port &p ) : port( p )
d107 1
a107 1
            p_tid = MP_Thread( pth_run ); // p thread must be running first
d114 5
a118 2
            while( p_tid || s_tid || pr_tid || pw_tid )
                DosSleep(128);
d129 2
a130 2
                if( schan[i].number() == ch )
                    return yes;
a144 17
    }


class MP_Channel
    {
    private:
        int     ch_no;
    public:
        MP_Channel( int num ) : ch_no(num) {}
        
        int     number() const { return ch_no; }
    };

// Receiving channel
class MP_RChannel : public MP_Channel
    {
    
a145 7

// Sending one
class MP_SChannel : public MP_Channel
    {
    };


@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d21 1
a21 1
class MP_Session
a27 1
        volatile long          challenge;
d34 2
a35 6
        MP_LinkStat            stat;
        
        void                   s_thread();     // sender/init state machine
        void                   r_thread();     // receiver state machine
        void                   pr_thread();     // port reader - decodes/timestamps packets, puts them on rpq, kicks rpq_sem, blocks on port read
        void                   pw_thread();     // port writer - endecodes packets from spq, sleeps on spq_sem if none, blocks on port write
a38 2
        static void            prth_run( void * ); // Kicker for pr_thread()
        static void            pwth_run( void * ); // Kicker for pr_thread()
d40 1
a40 1
        int                    s_tid, r_tid, pr_tid, pw_tid;
d45 4
a48 24
        // prq/spq keep pointers to the packets
        // packets corresponding to sender/receiver state machine are
        // stored on different queues.
        MP_PQueue              rrpq;    // Queue for the received for receiver packets
        MP_PQueue              rspq;    // Queue for the received for sender packets
        MP_PQueue              spq;     // Queue for the packets to send
        
        //MP_Packet *            peek_rpq(); // Take a look at oldest p in rpq
        //bool                   have_packet(); // rpq is not empty
        //void                   discard_rpq(); // discard oldest one in rpq
        
        MP_Sem                 spq_sem; // slept on by pw_thread
        MP_Sem                 s_sem;   // slept on by s_thread
        MP_Sem                 r_sem;   // slept on by r_thread
        
        volatile int           s_ndata; // number of non-sent data packets in spq, used to flow-control s_thread
        
        bool                   s_eof_known;
        bool                   r_eof_known;
        MP_Off_t               s_eof_pos;
        MP_Off_t               r_eof_pos;
        
        MP_Send_Window         swin;
        MP_Receive_Window      rwin;
d52 1
a52 1
        MP_Session( bool caller, MP_Port &p ) : port( p )
a56 1
            challenge = is_sender ? 0 ; get_random_challenge();
a59 6
            pr_tid = MP_Thread( prth_run );
            pw_tid = MP_Thread( pwth_run );
            
            s_state = MP_SS_Init; // Do init
            r_state = MP_SS_None; // Wait for now
            
d62 1
a62 1
        ~MP_Session()
@

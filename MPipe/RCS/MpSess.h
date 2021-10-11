head	1.11;
access;
symbols;
locks;
comment	@ * @;


1.11
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.09.00.14.58;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.06.01.50.37;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.06.30.20.14.53;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.17.37.36;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.17.37.09;	author dz;	state Exp;
branches;
next	;


desc
@@


1.11
log
@debugging depacketizer
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Session class
 *
 *      $Log: MpSess.h $
 *      Revision 1.10  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.9  1996/07/09 00:14:58  dz
 *      debug
 *
 *      Revision 1.8  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.7  1996/07/06 01:50:37  dz
 *      *** empty log message ***
 *
 *      Revision 1.6  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.5  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.4  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.3  1996/06/30 20:14:53  dz
 *      update
 *
 *      Revision 1.2  1996/06/30 17:37:36  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/06/30 17:37:09  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "vector.h"

#include "MpPort.h"
#include "MpStat.h"
#include "MpState.h"
#include "MpPQueue.h"
#include "MpUtil.h"
#include "MpWin.h"
#include "MpPort.h"
#include "MpBuff.h"

#include "Strng.h"

//#include "stdlib.h"


const MP_Max_Queued_For_Send = 15; // Max packets on spq


extern void _Optlink   sth_run( void * ); // Kicker for s_thread()
extern void _Optlink   rth_run( void * ); // Kicker for r_thread()
extern void _Optlink  prth_run( void * ); // Kicker for pr_thread()
extern void _Optlink  pwth_run( void * ); // Kicker for pr_thread()
        

class MP_Session : public MP_Port
    {
    private:
        
        bool                   is_caller;       // Am I caller, or what?
        volatile bool          stop_us;         // Shut all the things down!
        volatile long          challenge;
        
        MP_Port &              port;
        MP_Buffer              inbuf;
        
        MP_LinkStat            stat;
        
        long                   options;
        bool                   options_valid;
        
        void                   s_thread();     // sender/init state machine
        void                   r_thread();     // receiver state machine
        void                   pr_thread();     // port reader - decodes/timestamps packets, puts them on rpq, kicks rpq_sem, blocks on port read
        void                   pw_thread();     // port writer - encodes packets from spq, sleeps on spq_sem if none, blocks on port write
        
        friend extern void _Optlink   sth_run( void * );
        friend extern void _Optlink   rth_run( void * );
        friend extern void _Optlink  prth_run( void * );
        friend extern void _Optlink  pwth_run( void * );
        
        volatile int           s_tid, r_tid, pr_tid, pw_tid;
        
        volatile MP_State      s_state;
        volatile MP_State      r_state;
        
        // prq/spq keep pointers to the packets
        // packets corresponding to sender/receiver state machine are
        // stored on different queues.
        MP_PQueue              rrpq;    // Queue for the received for receiver packets
        MP_PQueue              rspq;    // Queue for the received for sender packets
        MP_PQueue              spq;     // Queue for the packets to send
        
        MP_SpinLock            rrpq_sl, rspq_sl, spq_sl;
        
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
        
        MP_Sem                 hangup_sem;
        
        void                   try_to_hangup()
            {
            if( r_state == MP_SS_RStop2 && s_state == MP_SS_SStop )
                hangup();
            }
            

        
    public:
        
        MP_Session( bool caller, MP_Port &_p ); // : port(_p), inbuf(_p);
        ~MP_Session();
        
        void wait_for_hangup();
        void abort();
        
        virtual int  read( char *data, int size, bool sure );    // Blocking!!
        virtual int  write( char *data, int size );   // Blocking!!
        virtual void hangup();                        // Blocking!!
        
        
    private:
        unsigned long       get_random_challenge() const;
        
        void                r_process_data();
        void                r_process_query();
        void                r_process_eof();

        void                s_process_wakeup();
        void                s_process_init();
        void                s_process_ack();
        void                s_process_nack();

        void                s_send_some_data();
        void                s_query(const MP_Frame &f);
        
        void                s_process_options( MP_Options ioptions );
        
        // NB! Pass new() objects only!
        void                send_pkt( MP_Packet *p );
        
    };


#undef MP_Thread

@


1.10
log
@Debugging has begone
@
text
@d8 3
d167 1
a167 9
        void                send_pkt( MP_Packet *p )
            {
              // BUG! Need overflow sema!
            MP_Message("going to enqueue pkt '%c'", p->type() );
            while( spq.queued() > MP_Max_Queued_For_Send ) MP_Sleep( 256 );
            MP_Message("really enqueuing pkt '%c'", p->type() );
            spq.put( p );
            spq_sem.post();
            }
@


1.9
log
@debug
@
text
@d8 3
d56 1
a56 1

d135 2
a136 41
        MP_Session( bool caller, MP_Port &_p ) : port(_p), inbuf(_p)
            {
            stop_us = No;
            options_valid = No;
            is_caller = caller;
            challenge = is_caller ? 0 : get_random_challenge();
            
            s_state = MP_SS_Init; // Do init
            r_state = MP_SS_None; // Wait for now
            
            connected_v = Yes;
            
            MP_Message( "c'tor this = %x", this );
            
            // Make sure everthing used by threads is init'ed above!
            
            MP_Message("Initializing session receiver state machine");
            r_tid = MP_Thread( rth_run, this ); // p thread must be running first
            MP_Message("Initializing session sender state machine");
            s_tid = MP_Thread( sth_run, this );
            MP_Message("Initializing session port reader/packet disassembler");
            pr_tid = MP_Thread( prth_run, this );
            MP_Message("Initializing session port writer/packet assembler");
            pw_tid = MP_Thread( pwth_run, this );
            
            }
        
        ~MP_Session()
            {
            abort();
            while( r_tid || s_tid || pr_tid || pw_tid )
                {
                MP_Sleep(128); MP_Message( "r%d s%d pr%d pw%d", r_tid, s_tid, pr_tid, pw_tid );
                MP_Sleep(128);
                }
            }
        
        void wait_for_hangup()
            {
            while(connected_v) hangup_sem.sleep();
            }
d138 2
a139 10
        void abort()
            {
            stop_us = Yes;
            r_sem.post();
            s_sem.post();
            spq_sem.post();
            port.hangup();
            rwin.abort();
            swin.abort();
            }
d142 2
a143 11
        
        virtual int  write( char *data, int size )   // Blocking!!
            { return swin.write( data, size ); }
        
        virtual void hangup()                        // Blocking!!
            {
            abort();
            port.hangup();
            connected_v = No;
            hangup_sem.post();
            }
d166 4
d171 1
@


1.8
log
@*** empty log message ***
@
text
@d8 3
d88 1
a88 1
        int                    s_tid, r_tid, pr_tid, pw_tid;
d90 2
a91 2
        MP_State               s_state;
        MP_State               r_state;
a138 5
            r_tid = MP_Thread( rth_run ); // p thread must be running first
            s_tid = MP_Thread( sth_run );
            pr_tid = MP_Thread( prth_run );
            pw_tid = MP_Thread( pwth_run );
            
d143 14
d163 2
d166 1
d169 1
a169 1
        wait_for_hangup()
d174 1
a174 1
        abort()
d177 4
d185 1
a185 2
        virtual int  read( char *data, int size )    // Blocking!!
            { return rwin.read( data, size ); }
@


1.7
log
@*** empty log message ***
@
text
@d8 3
d43 1
a49 50
extern void _Optlink   sucker_run( void * ); // Kicker for sucker()

class MP_Buffer
    {
    protected:
        MP_Port &      port;
        int            size;
        volatile int   used;     // How many bytes of buf is really full
        char *         buf;
        
        MP_SpinLock    discard;  // needs to be locked by anyone changing size
        
        MP_Sem         s_under;  // reader sleeps here when buffer is empty
        bool           underflow;// and this flag is true in such a case
        
        MP_Sem         s_over;   // sucker sleeps here when buffer is full
        bool           overflow; // and this flag is true in such a case
        
        friend void    sucker_run( void * ); // Kicker for sucker()
        void           sucker(); // thread that pumps data from port to buffer
        int            s_tid;    // its tid
        volatile bool  stop_v;
        
        void           sleep();
        void           wakeup();
        
    public:
        MP_Buffer( MP_Port &p, int s ) : port(p), size(s) 
            { 
            buf = new char[size]; 
            stop_v = No;
            s_tid = MP_Thread( sucker_run );
            }
        ~MP_Buffer() { stop(); }
        
        void     stop() { stop_v = Yes; while(s_tid) MP_Sleep(128); }
        
        operator const char *() const;
        void operator += (int); // Dscard first n characters
        
        bool connected() { return (!stop_v && port.connected()) ? Yes : No; }
        
        int len() const { return used; }
        
        void scan( char c );
        
        void wait_for_data( int len = 1);
        
        int bufsize() const { return size; }
    };
d182 1
a182 1
        unsigned long       get_random_challenge();
@


1.6
log
@writing
@
text
@d8 3
d43 53
a95 1
#include "stdlib.h"
a97 1
#define MP_Thread( f ) _beginthread( f, NULL, 64*1024, NULL );
a104 1

d113 2
a114 1
        MP_Port &              p;
d124 1
a124 1
        void                   pw_thread();     // port writer - endecodes packets from spq, sleeps on spq_sem if none, blocks on port write
d143 2
d175 1
a175 1
        MP_Session( bool caller, MP_Port &_p ) : p(_p)
d221 1
a221 1
            p.hangup();
@


1.5
log
@writing
@
text
@d8 3
d52 1
a52 1
class MP_Session : public MP_Port, protected MP_PAssembler
d119 1
a119 1
        MP_Session( bool caller, MP_Port &_p ) : p(_p), MP_PAssembler(_p)
d182 1
d184 1
d187 6
a192 1

@


1.4
log
@update
@
text
@d8 3
a181 6
        
        void                set_s_sem_timer(int sec);
        



@


1.3
log
@update
@
text
@d8 3
d22 8
d32 6
d39 8
a46 1
class MP_Session : public MP_Port
d58 3
d66 4
a69 4
        static void            sth_run( void * ); // Kicker for s_thread()
        static void            rth_run( void * ); // Kicker for r_thread()
        static void            prth_run( void * ); // Kicker for pr_thread()
        static void            pwth_run( void * ); // Kicker for pr_thread()
d101 2
d113 1
a113 1
        MP_Session( bool caller, MP_Port &p ) : port( p )
a114 1
            next_schan = 1;
d116 1
d118 1
a118 1
            challenge = is_sender ? 0 ; get_random_challenge();
d120 1
a120 1
            p_tid = MP_Thread( pth_run ); // p thread must be running first
d134 2
a135 2
            while( p_tid || s_tid || pr_tid || pw_tid )
                DosSleep(128);
d164 7
a170 1
    }
d172 16
d190 1
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d19 2
d22 1
a22 1
class MP_Session
d74 8
d99 1
d108 6
a113 1
            
d117 2
d121 13
a133 2
        int read( char *, int ); // Blocking!
        int write( char *, int ); // Blocking!
@


1.1
log
@Initial revision
@
text
@d7 2
a8 2
 *      $Log: MpChan.h $
 *      Revision 1.1  1996/06/30 16:11:38  dz
@

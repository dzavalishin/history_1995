/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Session class
 *
 *      $Log: MpSess.h $
 *      Revision 1.11  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
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


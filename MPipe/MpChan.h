/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Channels      
 *
 *      $Log: MpChan.h $
 *      Revision 1.6  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
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


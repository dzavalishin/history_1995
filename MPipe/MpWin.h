/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpWin.h $
 *      Revision 1.7  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.6  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.5  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.4  1996/07/01 14:46:23  dz
 *      writing framer methods
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

#ifndef MPWIN_H
#define MPWIN_H

#include "MpUtil.h"
#include "MpFrame.h"

#include "vector.h"

#include "time.h"


// defaults

const MP_Def_Win_Size = 128*1024;

class MP_Window
    {
    public:
        MP_Window( int size = MP_Def_Win_Size );
        ~MP_Window();

        abort();
        
        // Set probability to 100%
        void mark_as_received(const MP_Frame & f);
            /*
            {
            MP_Message("swin:: marking as received [%d,%d[", f.off, f.end() );
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob.set_to_100_percent();
            frames.dump();
            }
            */
    
        
    protected:
        char *              wbuf;
        MP_Off_t            wsize;   // Full size of wbuf
        
        MP_Frame            wf;      // Contains win. offset and _used_ size
        MP_Framer           frames;
        MP_SpinLock         lock;    // used to serialize access to wbuf/frames
        
        bool                aborted;
        
        // Posted when lower part of receiver window
        // got new data or upper part of sender window freed up
        // and ready to be filled with new data.
        MP_Sem              client_data_ready; 
    };


class MP_Send_Window : public MP_Window
    {
    public:

        MP_Frame find_frame_to_send();  // throws MP_EOF in none found
        /*
            {
            MP_SLKey key(lock);
            MP_Prob p = frames.lowest_prob();
            return frames.next_with_prob( p );
            }
        */
        
        bool     have_non_acked();      // We have at least one non-acked frame
        /*
            {
            MP_SLKey key(lock);
            // we can't have non-acked ones if win is empty!
            if( wf.size == 0 ) return No; 
            return frames.have_non_acked(wf);
            }
        */
        // Set probability to 0%
        void mark_as_non_received( const MP_Frame & f );
        /*
            {
            MP_Message("swin:: marking as non-received [%d,%d[", f.off, f.end() );
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob.set_to_0_percent();
            frames.dump();
            }
        */
        // Increase probability, but NEVER make it 100%.
        // Use known info on packet drop rate to adjust probability
        void mark_as_sent(const MP_Frame & f);
        /*
            {
            MP_Message("swin:: marking as sent [%d,%d[", f.off, f.end() );
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob += MP_75_percent; // BUG! here we should use drop rate
            wf.sent = time(0);
            frames.dump();
            }
        */
        // we asked about this. it should trigger
        // ack/nak and so must be used to count turnaround time
        void mark_as_queried(const MP_Frame & f);
            /*
            {
            MP_Message("swin:: marking as queried [%d,%d[", f.off, f.end() );
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.sent = time(0);
            frames.dump();
            }
            */
            
    const char* get( const MP_Frame &f );
        /*{ return wbuf + (f.off - wf.off); } */
    
    
    // Client (reliable) side.
    int  write( char *data, int size );   // Blocking!!
    };





class MP_Receive_Window : public MP_Window
    {
    public:
        // Set probability to 100%
        //void mark_as_received(MP_Frame);

        // returns frame to ack
        MP_Frame put( const MP_Frame &f, const char* data );

        bool completed_up_to(MP_Off_t pos);

        //void skip( MP_Off_t ); // unused!


        // Wait until buffer has this many received data at the beginning
        //void wait( MP_Off_t ); // unused!
    
        // Client (reliable) side.
        int  read( char *data, int size );   // Blocking!!

    };


#endif  // MPWIN_H


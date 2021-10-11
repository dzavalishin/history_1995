head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.07.10.18.27.24;	author dz;	state Exp;
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
date	96.07.01.14.46.23;	author dz;	state Exp;
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


1.7
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
 *      $Log: MpWin.h $
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
        void mark_as_received(const MP_Frame & f)
            {
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob.set_to_100_percent();
            }
    
        
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

        MP_Frame find_frame_to_send()  // throws MP_EOF in none found
            {
            MP_SLKey key(lock);
            MP_Prob p = frames.lowest_prob();
            return frames.next_with_prob( p );
            }
            
        
        bool     have_non_acked()      // We have at least one non-acked frame
            {
            MP_SLKey key(lock);
            // we can't have non-acked ones if win is empty!
            if( wf.size == 0 ) return No; 
            return frames.have_non_acked(wf);
            }

        // Set probability to 0%
        void mark_as_non_received( const MP_Frame & f )
            {
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob.set_to_0_percent();
            }
    
        // Increase probability, but NEVER make it 100%.
        // Use known info on packet drop rate to adjust probability
        void mark_as_sent(const MP_Frame & f)
            {
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.prob += MP_75_percent; // BUG! here we should use drop rate
            wf.sent = time(0);
            }
    
        // we asked about this. it should trigger
        // ack/nak and so must be used to count turnaround time
        void mark_as_queried(const MP_Frame & f)
            {
            MP_SLKey key(lock);
            MP_WFrame &wf = frames.find_or_rip_or_make( f );
            wf.sent = time(0);
            }
    
    const char* get( const MP_Frame &f )
        { return wbuf + (f.off - wf.off); }
    
    
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

@


1.6
log
@writing
@
text
@d8 3
d145 2
a146 1
        void put( const MP_Frame &f, const char* data );
@


1.5
log
@writing
@
text
@d8 3
d32 1
a42 145


// classes

const MP_Prob_Max = 100;

class MP_Prob   // Probability
    {
    protected:
        unsigned char  prob_v;
    
    public:
        MP_Prob() { prob_v = 0; }
        MP_Prob(int i) { prob_v = i; }
        
        bool           is_100_percent() const { return (prob_v >= MP_Prob_Max) ? Yes : No; }
        void           set_to_100_percent()   { prob_v = MP_Prob_Max; }
        void           set_to_0_percent()   { prob_v = 0; }
        
        int            operator >  ( const MP_Prob &p ) const { return prob_v >  p.prob_v; }
        int            operator <  ( const MP_Prob &p ) const { return prob_v <  p.prob_v; }
        int            operator == ( const MP_Prob &p ) const { return prob_v == p.prob_v; }
        int            operator != ( const MP_Prob &p ) const { return prob_v != p.prob_v; }
        
        MP_Prob &      operator =  ( const MP_Prob &p )       { prob_v = p.prob_v; return *this; }
        MP_Prob &      operator += ( const MP_Prob &p );
        MP_Prob        operator +  ( const MP_Prob &p ) const; //  { MP_Prob pp = *this; pp += p; return pp; }

    };

extern const MP_Prob MP_75_percent;


class MP_Frame
    {
    public:
        MP_Off_t   off;  // NB - _absolute_, not relative offset
        MP_Off_t   size;
        
        // First byte that is _not_ ours
        MP_Off     end() const { return off+size; }
        
        bool       contains( const MP_Frame & ) const;
        bool       has_common_point_with( const MP_Frame & ) const;
        
        // Extend this frame to include arg.
        // Throw error if arg is not side by side with us
        MP_Frame & operator += ( const MP_Frame & );
        
    };

class MP_WFrame : public MP_Frame
    {
    public:
        MP_Prob       prob; // Probability of correct reception
        time_t        sent; // When last packet (data or ack/nack) was sent
        
        MP_WFrame     operator +  ( const MP_WFrame &f1 );
            //{ MP_WFrame f(*this); f += f1; return f; }
        
        MP_WFrame &   operator += ( const MP_WFrame & );
    };


// Not multithread-protected.
// Call methods IN LOCKED STATE only!
class MP_Framer
    {
    protected:
        // rip off this frame from link. Element at index known
        // for sure to contain lower part of requested range.
        // Return index.
        // Note that if resulting frame is combined from two frames,
        // it's probability is set to lowest of both and time is
        // set to highest of both. See MP_WFrame operator +.
        // this is a worker for other methods.
        int                  rip_off( MP_Frame, int index );
        
        // Split frames[i] at position off, return index of leftmost part
        int                  split( int i, MP_Off off );
    
    public:
        vector<MP_WFrame>    frames;
        
        // We have at least one frame with probability < 100%
        // in range r
        bool                 have_non_acked( MP_Frame r )
            {
            for( int i = frames.size(); i--; )
                {
                if( !frames[i].has_common_point_with(r) )
                    continue;
                if( !frames[i].prob.is_100_percent() )
                    return Yes;
                }
            return No;
            }
        
        MP_Prob              lowest_prob()
            {
            MP_Prob p;
            p.set_to_100_percent();
            for( int i = frames.size(); i--; )
                {
                if( frames[i].prob < p )
                    p = frames[i].prob;
                }
            return p;
            }
        
        MP_WFrame &          next_with_prob(const MP_Prob &p)
            {
            static int last_pos = 0;
            int end = frames.size();
            
            for( int i = last_pos; i < end; i++ )
                if( frames[i].prob == p || frames[i].prob < p )
                    {
                    last_pos = i;
                    return frames[i];
                    }
            for( i = 0; i <= last_pos; i++ )
                if( frames[i].prob == p || frames[i].prob < p )
                    {
                    last_pos = i;
                    return frames[i];
                    }
            throw(Mp_Ex_EOF("MP_WFramer::next_with_prob"));
            }
        
        // rip off and DELETE this frame from link, possibly dividing
        // some el. into a pair. 
        MP_WFrame            operator -= ( MP_Frame );
        
        // Just make framer to contatin ths frame - don't care, how.
        MP_WFrame &          find_or_rip_or_make( MP_Frame );
        bool                 empty() const { return frames.size() == 0 ? Yes : No; }
        const MP_WFrame &    first() const
            { 
            if( empty() ) throw(Mp_Ex_Fail("MP_WFrame::first","empty",""));
            return frames[0];
            }
    };


d112 10
@


1.4
log
@writing framer methods
@
text
@d8 3
d52 1
d55 12
d69 2
d75 1
a75 1
        MP_Off_t   off;  // NB - _FILE_, not window offset
d78 2
a79 1
        MP_Off     end() { return off+size; }
d90 1
a90 1
class MP_WFrame ; public MP_Frame
d96 2
a97 2
        MP_WFrame     operator +  ( const MP_WFrame &f1 ) 
            { MP_WFrame f(*this); f += f1; return f; }
d115 1
a115 1
        int                  rip_off( MP_Frame, index );
d121 1
a121 1
        vector<MP_WFrame>  frames;
d123 45
d171 1
a171 1
        MP_WFrame            oerator -= ( MP_Frame );
d173 8
a180 3
        // Just make framer to contatin ths frame -
        // don't care, how.
        const MP_WFrame &    find_or_rip_or_make( MP_Frame );
d192 9
d205 2
a206 2
        volatile MP_Frame   wf;      // Contains win. offset and _used_ size
        volatile MP_Framer  frames;
d221 24
d246 8
a253 12
    MP_Frame find_frame_to_send(); // throws MP_EOF in none found
    bool     have_non_acked();     // We have at least one non-acked frame
    
    // Set probability to 100%
    void mark_as_received(MP_Frame);
    
    // Set probability to 0%
    void mark_as_non_received(MP_Frame);
    
    // Increase probability, but NEVER make it 100%.
    // Use known info on packet drop rate to adjust probability
    void mark_as_sent(MP_Frame);   
d271 1
a271 1
        void mark_as_received(MP_Frame);
@


1.3
log
@update
@
text
@d8 3
d26 3
d59 3
a61 3
        MP_Prob    prob; // Probability of correct reception
        time_t     sent; // When last packet (data or ack/nack) was sent
    
d63 1
d71 12
d84 2
d88 13
d102 6
a107 1
        vector<MP_Frame>  frames_v;
d109 3
a111 3
        // rip off this frame from link,possibly dividing
        // some el. into a pair. Errors ignored
        operator -= ( MP_Frame );
d118 4
a121 19
        MP_Window( int size = MP_Def_Win_Size )
            {
            wbuf_v = new char [size+1]; // +1 for any case :)
            if( wbuf_v == NULL ) throw("out of memory");
            wf_v.off = 0;
            wf_v.size = size;
            aborted = No;
            }
        
        ~MP_Window()
            {
            delete [] wbuf_v;
            }
        
        abort()
            {
            aborted = No;
            client_data_ready.post();
            }
d124 2
a125 1
        char *     wbuf_v;
d127 3
a129 2
        MP_Frame   wf_v;       // Contains window offset and size
        MP_Framer  frames_v;
d131 1
a131 1
        bool       aborted;
d136 1
a136 1
        MP_Sem     client_data_ready; 
d158 1
a158 1
        { return wbuf_v + (f.off - wf_v.off); }
d162 1
a162 9
    int  write( char *data, int size )   // Blocking!!
        {
        while(1)
            {
            if( aborted ) throw(Mp_Ex_Abort("MP_Send_Window::write"));
            
            client_data_ready.sleep();
            }
        }
d165 4
d175 1
a175 7
        void put( const MP_Frame &f, const char* data )
            {
            if( !wf_v.contains( f ) ) throw("block is out of window");
            MP_Off_t woff = f.off - wf_v.off; // offset in a window
            memcpy( wbuf_v+woff, data, f.size );
            mark_as_received(f);
            }
d179 5
a183 11
    // Ask sender to skip this many bytes from the current start of window.
    // Upon the return from this method window must start from new pos.
    // How:
    // Send ack for all the data from 0 to this pos, move window
    // to a new start correcting Framer state, return.
    // Note that state machine must ack all the data up to the window
    // start on the receprion of data, which belongs to the under-the-window
    // part of stream. These acks are not depend on history and not TOO
    // critical, so it seems to be wise to send 'em no more frequent,
    // than normal data turnaround time. See MpStat.h
    void skip( MP_Off_t ); 
d185 2
a186 14
    
    // Wait until buffer has this many received data at the beginning
    void wait( MP_Off_t ); 
    
    // Client (reliable) side.
    int  read( char *data, int size )   // Blocking!!
        {
        while(1)
            {
            if( aborted ) throw(Mp_Ex_Abort("MP_Receive_Window::read"));
            
            client_data_ready.sleep();
            }
        }
@


1.2
log
@update
@
text
@d8 3
d19 6
a29 4
// integral types

typedef long             MP_Off_t;

d44 1
a44 1
        bool           is_100_percent() const { return prob_v >= MP_Prob_Max; }
d116 1
d131 4
d149 14
a162 11
    // Set probability to 100%
    void mark_as_received(MP_Frame);
    
    void put( const MP_Frame &f, const char* data )
        {
        if( !wf_v.contains( f ) ) throw("block is out of window");
        MP_Off_t woff = f.off - wf_v.off; // offset in a window
        memcpy( wbuf_v+woff, data, f.size );
        mark_as_received(f);
        }
    
d191 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d80 12
d95 1
a95 1
        char       wbuf_v;
d99 7
a105 1
    
d125 10
d166 11
@

head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.02.01.58.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@something goes through, framer/frame classes
are work somehow.
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Frame, Framer, Probability
 *
 *      $Log: MpFrame.h $
 *      Revision 1.4  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.3  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.2  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.1  1996/07/02 01:58:48  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#ifndef MPFRAME_H
#define MPFRAME_H

#include "MpUtil.h"

#include "vector.h"

#include "time.h"





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

        int prob() const { return prob_v; }
    };

extern const MP_Prob MP_75_percent;


class MP_Frame
    {
    public:
        ~MP_Frame();
        MP_Frame( MP_Off o, MP_Off s ) : off(o), size(s) {}
          //MP_Frame() : off(-1), size(-1) {}
        MP_Frame() {}
        
        MP_Off_t   off;  // NB - _absolute_, not relative offset
        MP_Off_t   size;
        
        // First byte that is _not_ ours
        MP_Off     end() const { return off+size; }
        
        bool       contains( const MP_Frame & ) const;
        bool       has_common_point_with( const MP_Frame & ) const;
        
        // Extend this frame to include arg.
        // Throw error if arg is not side by side with us
        MP_Frame & operator += ( const MP_Frame & );
        
        // make sure this frame doesn't contain given range
        // No errors.
        void       operator -= ( const MP_Frame & );
        
        int        operator == ( const MP_Frame &f ) const { return off==f.off && size==f.size; }

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
    private:
        int last_pos;

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
        MP_Framer()
            { last_pos = 0; }
        // All the frames here are sorted. Lower index corresponds
        // to lower offset. No overlapping allowed.
        vector<MP_WFrame>    frames;
        
        // We have at least one frame with probability < 100%
        // in range r
        bool                 have_non_acked( MP_Frame r );
        MP_Prob              lowest_prob();
        MP_WFrame &          next_with_prob(const MP_Prob &p);
        
        // rip off and DELETE this frame from link, possibly dividing
        // some el. into a pair. 
        void                 operator -= ( MP_Frame );
        
        // Just make framer to contatin ths frame - don't care, how.
        MP_WFrame &          find_or_rip_or_make( const MP_Frame& f );
        bool                 empty() const { return frames.size() == 0 ? Yes : No; }
        const MP_WFrame &    first() const
            { 
            if( empty() ) throw(Mp_Ex_Fail("MP_WFrame::first","empty",""));
            return frames[0];
            }

        void       dump();
    };




#endif  // MPFRAME_H

@


1.4
log
@debugging depacketizer
@
text
@d8 3
d65 1
d75 3
d97 1
d158 2
@


1.3
log
@Debugging has begone
@
text
@d8 3
d109 3
d126 2
@


1.2
log
@writing
@
text
@d8 3
d67 2
@


1.1
log
@Initial revision
@
text
@d5 5
a9 1
 *	Module 	:	
a10 1
 *      $Log: MpWin.h $
d17 2
a18 2
#ifndef MPWIN_H
#define MPWIN_H
a26 3
// defaults

const MP_Def_Win_Size = 128*1024;
d77 5
d115 2
d121 3
a123 43
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
d127 1
a127 1
        MP_WFrame            operator -= ( MP_Frame );
d130 1
a130 1
        MP_WFrame &          find_or_rip_or_make( MP_Frame );
a139 103
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

        void put( const MP_Frame &f, const char* data );

        bool completed_up_to(MP_Off_t pos);

        //void skip( MP_Off_t ); // unused!


        // Wait until buffer has this many received data at the beginning
        //void wait( MP_Off_t ); // unused!
    
        // Client (reliable) side.
        int  read( char *data, int size );   // Blocking!!

    };
d142 1
a142 1
#endif  // MPWIN_H
@
